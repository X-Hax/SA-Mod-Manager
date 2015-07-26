#include "stdafx.h"

#include "include/d3d8types.h"

#include "SADXModLoader.h"
#include "FixFOV.h"

FunctionPointer(void,	SetHorizontalFOV_BAMS,		(int bams), 0x00402ED0);
FunctionPointer(int,	GetHorizontalFOV_BAMS,		(void),		0x00402F00);
FunctionPointer(void,	SetClippingRelatedThing,	(int bams), 0x007815C0);

DataPointer(D3DMATRIX,	MyCoolMatrix,			0x03AAD0A0);
DataPointer(float,		ClippingRelated,		0x03D0F9E0);
DataPointer(int,		HorizontalFOV_BAMS,		0x03AB98EC);
DataPointer(int,		LastHorizontalFOV_BAMS,	0x03B2CBB4);

const int bams_default = 12743;

int	fov_bams;
int	last_bams = bams_default;

double fov_rads;
double fov_scale = 1.0;

void __cdecl SetClippingRelatedThing_hook(int bams)
{
	// We're scaling here because this function
	// can be called independently of SetHorizontalFOV_BAMS
	double m = (double)bams_default / bams;
	bams = (int)(fov_bams / m);

	double tan = BAMStan(bams / 2) * 2;
	ClippingRelated = (float)((double)VerticalResolution / tan);
}

int __cdecl GetHorizontalFOV_BAMS_hook()
{
	return last_bams;
}

void __cdecl SetHorizontalFOV_BAMS_hook(int bams)
{
	fov_scale = (double)bams_default / bams;
	int scaled = (bams == fov_bams) ? fov_bams : (int)(fov_bams * fov_scale);

	int* _24 = (int*)&MyCoolMatrix._24;

	SetClippingRelatedThing_hook(bams);

	HorizontalFOV_BAMS = scaled;
	*_24 = scaled;
	last_bams = bams;
}

#pragma region assembly

void* setfov_return = (void*)0x00791251;
void __declspec(naked) SetFOV()
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
void* fix_to = (void*)0x00781529;
float dummy;

void __declspec(naked) FixFloatStackPls()
{
	__asm
	{
		fstp dummy
		jmp fix_to
	}
}

void* dothething_return = (void*)0x0040872F;
void __declspec(naked) dothething()
{
	__asm
	{
		mov eax, last_bams
		jmp dothething_return
	}
}

#pragma endregion

void ConfigureFOV()
{
	static const double ratio = 4.0 / 3.0;
	const uint32_t width = HorizontalResolution;
	const uint32_t height = VerticalResolution;

	// 4:3 and "tallscreen" (5:4, portrait, etc)
	// We don't need to do anything since these resolutions work fine with the default code.
	if ((height * ratio) == width || (height * ratio) > width)
		return;

	// Widescreen (16:9, 16:10, etc)
	if ((height * ratio) < width)
	{
		fov_rads = atan2((double)width, (double)height);
		fov_bams = (int)((fov_rads * (180.0f / D3DX_PI)) * (65536.0 / 360.0));

		WriteJump(SetHorizontalFOV_BAMS, SetHorizontalFOV_BAMS_hook);
		WriteJump(GetHorizontalFOV_BAMS, GetHorizontalFOV_BAMS_hook);
		WriteJump(SetClippingRelatedThing, SetClippingRelatedThing_hook);
		WriteJump((void*)0x0079124A, &SetFOV);
		WriteJump((void*)0x00781523, &FixFloatStackPls);
		WriteJump((void*)0x0040872A, &dothething);

		SetHorizontalFOV_BAMS_hook(bams_default);
	}
}
