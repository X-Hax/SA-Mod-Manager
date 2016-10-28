#include "stdafx.h"

#include "include/d3d8types.h"
#include "SADXModLoader.h"

static bool is_wide = false;
static const Angle bams_default = 12743;

static Angle last_bams = bams_default;
static Angle fov_bams = 0;

static double fov_rads = 1.0f;
static double fov_scale = 1.0;
static float dummy;

static const bool fill = false;
static void DisplayVideoFrame_FixAspectRatio()
{
	// I would be using .sx and .sy (size), but those are always 640x480 no matter the video resolution.
	int video_width = VideoFrame.tanim[0].cx * 2;
	int video_height = VideoFrame.tanim[0].cy * 2;
	float video_resolution;
	float screen_resolution;

	if (!fill && HorizontalResolution > VerticalResolution || fill && HorizontalResolution < VerticalResolution)
	{
		video_resolution = (float)video_height;
		screen_resolution = (float)VerticalResolution;
	}
	else
	{
		video_resolution = (float)video_width;
		screen_resolution = (float)HorizontalResolution;
	}

	float scale = screen_resolution / video_resolution;
	VideoFrame.sx = VideoFrame.sy = scale;
}

// Fix for neglected width and height in global NJS_SCREEN
static void __cdecl SetupScreen_r(NJS_SCREEN* screen)
{
	_nj_screen_ = *screen;
}

static void __cdecl njSetScreenDist_r(Angle bams);
static void __cdecl njSetPerspective_r(Angle bams);
static Trampoline njSetScreenDist_trampoline(0x007815C0, 0x007815C5, njSetScreenDist_r);
static Trampoline njSetPerspective_trampoline(0x00402ED0, 0x00402ED5, njSetPerspective_r);

static void __cdecl njSetScreenDist_r(Angle bams)
{
	if (!is_wide)
	{
		FunctionPointer(void, original, (Angle), njSetScreenDist_trampoline.Target());
		original(bams);
		return;
	}

	// We're scaling here because this function
	// can be called independently of njSetPerspective
	double m = (double)bams_default / bams;
	bams = (Angle)(fov_bams / m);

	double tan = njTan(bams / 2) * 2;
	_nj_screen_.dist = (float)((double)_nj_screen_.h / tan);
}

static void __cdecl njSetPerspective_r(Angle bams)
{
	if (!is_wide)
	{
		FunctionPointer(void, original, (Angle), njSetPerspective_trampoline.Target());
		original(bams);
		last_bams = HorizontalFOV_BAMS;
		return;
	}

	fov_scale = (double)bams_default / bams;
	Angle scaled = (bams == fov_bams) ? fov_bams : (Angle)(fov_bams * fov_scale);

	njSetScreenDist_r(bams);

	int* _24 = (int*)&ProjectionMatrix._24;
	HorizontalFOV_BAMS = scaled;
	*_24 = scaled;
	last_bams = bams;
}

static const void* setfov_return = (void*)0x00791251;
static void __declspec(naked) SetFOV()
{
	__asm
	{
		cmp		is_wide, 1
		je		wide

		// default behavior:
		fpatan
		fadd	st, st
		fstp	dword ptr[esp]

		jmp		setfov_return

	wide:
		fstp	dword ptr[esp]
		fstp	dword ptr[esp]

		fld		fov_rads
		fdiv	fov_scale
		fstp	dword ptr[esp]

		jmp		setfov_return
	}
}

static const void* dummyfstp_return = (void*)0x00781529;
static const void* wtf_ptr = (void*)0x03D0F9E0;
static void __declspec(naked) dummyfstp()
{
	__asm
	{
		cmp		is_wide, 1
		je		wide
		fstp	dword ptr wtf_ptr
		jmp		dummyfstp_return

	wide:
		fstp	dummy
		jmp		dummyfstp_return
	}
}

static const double default_ratio = 4.0 / 3.0;
void CheckAspectRatio()
{
	uint32_t width = HorizontalResolution;
	uint32_t height = VerticalResolution;

	// 4:3 and "tallscreen" (5:4, portrait, etc)
	// We don't need to do anything since these resolutions work fine with the default code.
	is_wide = !(height * default_ratio == width || height * default_ratio > width);
}

void ConfigureFOV()
{
	WriteJump(SetupScreen, SetupScreen_r);

	// Taking advantage of a nullsub call.
	WriteCall((void*)0x00513A88, DisplayVideoFrame_FixAspectRatio);

	// Stops the Pause Menu from using horizontal stretch in place of vertical stretch in coordinate calculation
	// Main Pause Menu
	WriteData((float**)0x00457F69, &VerticalStretch); // Elipse/Oval
	WriteData((float**)0x004584EE, &VerticalStretch); // Blue Transparent Box
	WriteData((float**)0x0045802F, &VerticalStretch); // Pause Menu Options
	// Camera options
	WriteData((float**)0x00458D5C, &VerticalStretch); // Blue Transparent Box
	WriteData((float**)0x00458DF4, &VerticalStretch); // Auto Cam
	WriteData((float**)0x00458E3A, &VerticalStretch); // Free Cam
	// Controls
	WriteData((float**)0x0045905A, &VerticalStretch); // Blue Transparent Box
	WriteData((float**)0x004590BB, &VerticalStretch); // Each Control Element
	WriteData((float**)0x00459133, &VerticalStretch); // Default Button

	CheckAspectRatio();

	fov_rads = 0.96712852;	// 55.412382 degrees
	fov_bams = NJM_RAD_ANG(fov_rads);

	// Code patches
	WriteJump((void*)0x0079124A, SetFOV);
	WriteJump((void*)0x00781523, dummyfstp); // Dirty hack to disable a write to ClippingRelated and keep the floating point stack balanced.
	WriteData((Angle**)0x0040872B, &last_bams); // Fixes a case of direct access to HorizontalFOV_BAMS
	WriteData((Angle**)0x00402F01, &last_bams); // Changes return value of GetHorizontalFOV_BAMS

	njSetPerspective(bams_default);
}
