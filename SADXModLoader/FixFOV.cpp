#include "stdafx.h"

#include "include/d3d8types.h"
#include "SADXModLoader.h"

FunctionPointer(void,	SetHorizontalFOV_BAMS,		(int bams), 0x00402ED0);
//FunctionPointer(int,	GetHorizontalFOV_BAMS,		(void),		0x00402F00);
FunctionPointer(void,	SetClippingRelatedThing,	(int bams), 0x007815C0);

// This probably isn't even a D3DMATRIX.
DataPointer(D3DMATRIX,	ProjectionMatrix,		0x03AAD0A0);
DataPointer(float,		ClippingRelated,		0x03D0F9E0);
DataPointer(int,		HorizontalFOV_BAMS,		0x03AB98EC);
DataPointer(int,		LastHorizontalFOV_BAMS,	0x03B2CBB4);
DataPointer(NJS_SPRITE,	VideoFrame,				0x03C600A4);

static const int	bams_default	= 12743;
static int			last_bams		= bams_default;
static int			fov_bams;

static double fov_rads = 1.0f;
static double fov_scale = 1.0;
static float video_scale = 1.0f;
static float dummy;

static void DisplayVideoFrame_FixAspectRatio()
{
	VideoFrame.sx = VideoFrame.sy = video_scale;
}

static void __cdecl SetClippingRelatedThing_hook(int bams)
{
	// We're scaling here because this function
	// can be called independently of SetHorizontalFOV_BAMS
	double m = (double)bams_default / bams;
	bams = (int)(fov_bams / m);

	double tan = BAMStan(bams / 2) * 2;
	ClippingRelated = (float)((double)VerticalResolution / tan);
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

void ConfigureFOV()
{
	static const double default_ratio = 4.0 / 3.0;
	const uint32_t width = HorizontalResolution;
	const uint32_t height = VerticalResolution;

	// Taking advantage of a nullsub call.
	WriteCall((void*)0x00513A88, DisplayVideoFrame_FixAspectRatio);

	// Fits video to screen. For "fill", use max instead.
	video_scale = min(HorizontalStretch, VerticalStretch);

	// 4:3 and "tallscreen" (5:4, portrait, etc)
	// We don't need to do anything since these resolutions work fine with the default code.
	if ((height * default_ratio) == width || (height * default_ratio) > width)
		return;

	fov_rads = 0.96712852;	// 55.412382 degrees
	fov_bams = NJM_RAD_ANG(fov_rads);

	// Function hooks
	WriteJump(SetHorizontalFOV_BAMS, SetHorizontalFOV_BAMS_hook);
	WriteJump(SetClippingRelatedThing, SetClippingRelatedThing_hook);

	// Code patches
	WriteJump((void*)0x0079124A, SetFOV);
	WriteData((float**)0x00781525, &dummy); // Dirty hack to disable a write to ClippingRelated and keep the floating point stack balanced.
	WriteData((int**)0x0040872B, &last_bams); // Fixes a case of direct access to HorizontalFOV_BAMS
	WriteData((int**)0x00402F01, &last_bams); // Changes return value of GetHorizontalFOV_BAMS

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
