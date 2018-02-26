#include "stdafx.h"

#include "include/d3d8types.h"
#include "SADXModLoader.h"

#include "FixFOV.h"

/**
 * \brief Default *horizontal* field of view in BAMS used internally.
 */
static const Angle FOV_H = 12743; // Roughly 70 degrees

/**
* \brief Default *vertical* field of view in radians.
*/
static const double FOV_V_RADS = 0.96712852; // 55.412382 degrees

/**
 * \brief Default *vertical* field of view in BAMS.
 */
static const Angle FOV_V_BAMS = NJM_RAD_ANG(FOV_V_RADS);

/**
 * \brief Used to determine code path.
 * If the resolution is not widescreen, fall back to vanilla 4:3 code.
 */
static bool is_wide = false;

/**
* \brief Last unmodified horizontal field of view in BAMS provided by the game code.
*/
static Angle last_bams = FOV_H;

/**
 * \brief FOV scalar for converting from horizontal to vertical FOV quickly and easily.
 */
static float fov_scale = 1.0;

// Fix for neglected width and height in global NJS_SCREEN
static void __cdecl SetupScreen_r(NJS_SCREEN* screen)
{
	_nj_screen_.w  = screen->w;
	_nj_screen_.h  = screen->h;
	_nj_screen_.cx = screen->cx;
	_nj_screen_.cy = screen->cy;

	// Specifically excluding .dist because there's no reason to overwrite it here.
}

static void __cdecl njSetScreenDist_r(Angle bams);
static void __cdecl njSetPerspective_r(Angle bams);
static Trampoline njSetScreenDist_trampoline(0x007815C0, 0x007815C5, njSetScreenDist_r);
static Trampoline njSetPerspective_trampoline(0x00402ED0, 0x00402ED5, njSetPerspective_r);

static void __cdecl njSetScreenDist_r(const Angle bams)
{
	last_bams = bams;

	if (!is_wide)
	{
		NonStaticFunctionPointer(void, original, (Angle), njSetScreenDist_trampoline.Target());
		original(bams);
		return;
	}

	// We're scaling here because this function
	// can be called independently of njSetPerspective
	float m = static_cast<float>(FOV_H) / bams;
	auto scaled = static_cast<Angle>(FOV_V_BAMS / m);

	float tan = njTan(scaled / 2) * 2.0f;
	_nj_screen_.dist = _nj_screen_.h / tan;
}

static void __cdecl njSetPerspective_r(Angle bams)
{
	if (!is_wide)
	{
		NonStaticFunctionPointer(void, original, (Angle), njSetPerspective_trampoline.Target());
		original(bams);
		last_bams = HorizontalFOV_BAMS;
		return;
	}

	fov_scale = static_cast<float>(FOV_H) / bams;
	Angle scaled = (bams == FOV_V_BAMS) ? FOV_V_BAMS : static_cast<Angle>(FOV_V_BAMS * fov_scale);

	njSetScreenDist_r(bams);

	auto _24 = reinterpret_cast<int*>(&ProjectionMatrix._24);
	HorizontalFOV_BAMS = scaled;
	*_24 = scaled;
	last_bams = bams;
}

static const auto loc_791251 = reinterpret_cast<void*>(0x00791251);
static void __declspec(naked) SetFOV()
{
	__asm
	{
		cmp    is_wide, 1
		je     wide

		// default behavior:
		// atan2(1.0, tan(hfov / 2) / aspect ratio)
		fpatan
		fadd   st, st
		fstp   dword ptr [esp]

		jmp    loc_791251

	wide:
		// dropping some values off of the stack that we don't need
		fstp   st(0)
		fstp   st(0)

		// FOV_V_RADS / fov_scale
		fld    FOV_V_RADS
		fdiv   fov_scale
		fstp   dword ptr [esp]

		jmp    loc_791251
	}
}

static const auto loc_781529 = reinterpret_cast<void*>(0x00781529);
static const auto _nj_screen_dist = &_nj_screen_.dist;
static void __declspec(naked) dummyfstp()
{
	__asm
	{
		cmp  is_wide, 1
		je   wide
		// in 4:3, write the value to _nj_screen_.dist
		fstp [_nj_screen_dist]
		jmp  loc_781529

	wide:
		// in widescreen, just drop the value
		fstp st(0)
		jmp  loc_781529
	}
}

static constexpr double default_ratio = 4.0 / 3.0;

void fov::check_aspect_ratio()
{
	uint32_t width = HorizontalResolution;
	uint32_t height = VerticalResolution;

	// 4:3 and "tallscreen" (5:4, portrait, etc)
	// We don't need to do anything since these resolutions work fine with the default code.
	is_wide = !(height * default_ratio == width || height * default_ratio > width);
}

void fov::initialize()
{
	WriteJump(SetupScreen, SetupScreen_r);

	// Stops the Pause Menu from using horizontal stretch in place of vertical stretch in coordinate calculation
	// Main Pause Menu
	WriteData(reinterpret_cast<float**>(0x00457F69), &VerticalStretch); // Elipse/Oval
	WriteData(reinterpret_cast<float**>(0x004584EE), &VerticalStretch); // Blue Transparent Box
	WriteData(reinterpret_cast<float**>(0x0045802F), &VerticalStretch); // Pause Menu Options
	// Camera options
	WriteData(reinterpret_cast<float**>(0x00458D5C), &VerticalStretch); // Blue Transparent Box
	WriteData(reinterpret_cast<float**>(0x00458DF4), &VerticalStretch); // Auto Cam
	WriteData(reinterpret_cast<float**>(0x00458E3A), &VerticalStretch); // Free Cam
	// Controls
	WriteData(reinterpret_cast<float**>(0x0045905A), &VerticalStretch); // Blue Transparent Box
	WriteData(reinterpret_cast<float**>(0x004590BB), &VerticalStretch); // Each Control Element
	WriteData(reinterpret_cast<float**>(0x00459133), &VerticalStretch); // Default Button

	check_aspect_ratio();

	// Hijacks some code before a call to D3DXMatrixPerpsectiveFovRH to correct the vertical field of view.
	WriteJump(reinterpret_cast<void*>(0x0079124A), SetFOV);
	// Dirty hack to disable a write to _nj_screen_.dist and keep the floating point stack balanced.
	WriteJump(reinterpret_cast<void*>(0x00781523), dummyfstp);
	// Fixes a case of direct access to HorizontalFOV_BAMS
	WriteData(reinterpret_cast<Angle**>(0x0040872B), &last_bams);
	// Changes return value of GetHorizontalFOV_BAMS
	WriteData(reinterpret_cast<Angle**>(0x00402F01), &last_bams);

	// Fixes lens flare positioning
	// Standard lens flare
	WriteData(reinterpret_cast<float**>(0x004DA1CF + 2), &_nj_screen_.cx);
	WriteData(reinterpret_cast<float**>(0x004DA1DD + 2), &_nj_screen_.cy);
	// Chaos 0 helicopter lens flare
	WriteData(reinterpret_cast<float**>(0x004DA584 + 2), &_nj_screen_.cx);
	WriteData(reinterpret_cast<float**>(0x004DA592 + 2), &_nj_screen_.cy);

	njSetPerspective(FOV_H);
}

Angle fov::get_fov()
{
	return last_bams;
}
