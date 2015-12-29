#include "stdafx.h"

#include "include/d3d8types.h"
#include "SADXModLoader.h"

FunctionPointer(void,	SetHorizontalFOV_BAMS,		(int bams), 0x00402ED0);
FunctionPointer(int,	GetHorizontalFOV_BAMS,		(void),		0x00402F00);
FunctionPointer(void,	SetClippingRelatedThing,	(int bams), 0x007815C0);

DataPointer(D3DMATRIX,	ProjectionMatrix,		0x03AAD0A0);
DataPointer(float,		ClippingRelated,		0x03D0F9E0);
DataPointer(int,		HorizontalFOV_BAMS,		0x03AB98EC);
DataPointer(int,		LastHorizontalFOV_BAMS,	0x03B2CBB4);

static const int	bams_default	= 12743;
static int			last_bams		= bams_default;
static int			fov_bams;

static double fov_rads;
static double fov_scale = 1.0;

static void __cdecl SetClippingRelatedThing_hook(int bams)
{
	// We're scaling here because this function
	// can be called independently of SetHorizontalFOV_BAMS
	double m = (double)bams_default / bams;
	bams = (int)(fov_bams / m);

	double tan = BAMStan(bams / 2) * 2;
	ClippingRelated = (float)((double)VerticalResolution / tan);
}

// TODO: Patch code instead
static int __cdecl GetHorizontalFOV_BAMS_hook()
{
	return last_bams;
}

static void __cdecl SetHorizontalFOV_BAMS_hook(int bams)
{
	fov_scale = (double)bams_default / bams;
	int scaled = (bams == fov_bams) ? fov_bams : (int)(fov_bams * fov_scale);

	int* _24 = (int*)&ProjectionMatrix._24;

	SetClippingRelatedThing_hook(bams);

	HorizontalFOV_BAMS = scaled;
	*_24 = scaled;
	last_bams = bams;
}

#pragma region Assembly

static const void* setfov_return = (void*)0x00791251;
static void __declspec(naked) SetFOV()
{
	__asm
	{
		fstp	dword ptr[esp]
		fstp	dword ptr[esp]

		fld		fov_rads
		fdiv	fov_scale
		fstp	dword ptr[esp]

		jmp		setfov_return
	}
}

// HACK: Dirty hack to disable a write to ClippingRelated and keep the floating point stack balanced.
static float dummy;
static const void* fix_to = (void*)0x00781529;
static void __declspec(naked) FixFloatStackPls()
{
	__asm
	{
		fstp dummy
		jmp fix_to
	}
}

// Handles a case where the FOV is accessed directly. If somebody wants to name this, be my guest :V
// TODO: Patch code to replace address with last_bams instead of jumping here
static const void* dothething_return = (void*)0x0040872F;
static void __declspec(naked) FixDirectAccess()
{
	__asm
	{
		mov eax, last_bams
		jmp dothething_return
	}
}

#pragma endregion

DataPointer(NJS_SPRITE, VideoFrame, 0x03C600A4);

static void DisplayVideoFrame_FixAspectRatio()
{
	VideoFrame.sx = VideoFrame.sy = min(HorizontalStretch, VerticalStretch);
}

void ConfigureFOV()
{
	static const double default_ratio = 4.0 / 3.0;
	const uint32_t width = HorizontalResolution;
	const uint32_t height = VerticalResolution;

	// Taking advantage of a nullsub call.
	WriteCall((void*)0x00513A88, DisplayVideoFrame_FixAspectRatio);

	// 4:3 and "tallscreen" (5:4, portrait, etc)
	// We don't need to do anything since these resolutions work fine with the default code.
	if ((height * default_ratio) == width || (height * default_ratio) > width)
		return;

	fov_rads = 0.96712852;	// 55.412382 degrees
	fov_bams = NJM_RAD_ANG(fov_rads);

	// Function hooks
	WriteJump(SetHorizontalFOV_BAMS, SetHorizontalFOV_BAMS_hook);
	WriteJump(GetHorizontalFOV_BAMS, GetHorizontalFOV_BAMS_hook);
	WriteJump(SetClippingRelatedThing, SetClippingRelatedThing_hook);
	// Code patches
	WriteJump((void*)0x0079124A, SetFOV);
	WriteJump((void*)0x00781523, FixFloatStackPls);
	WriteJump((void*)0x0040872A, FixDirectAccess);

	SetHorizontalFOV_BAMS_hook(bams_default);

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
}
