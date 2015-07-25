#include "stdafx.h"

#include "include/d3d8types.h"

#include "SADXModLoader.h"
#include "FixFOV.h"

FunctionPointer(void,	SetHorizontalFOV_BAMS,		(int bams), 0x00402ED0);
FunctionPointer(void,	SetClippingRelatedThing,	(int bams), 0x007815C0);

DataPointer(D3DMATRIX,	MyCoolMatrix,		0x03AAD0A0);
DataPointer(float,		ClippingRelated,	0x03D0F9E0);
DataPointer(int,		HorizontalFOV_BAMS,	0x03AB98EC);
//DataPointer(int, HorizontalFOV_BAMS_copy, 0x03B2CBB4);

int		fov_bams;
double	fov_rads;

void __cdecl SetClippingRelatedThing_hook(int bams)
{
	double tan = BAMStan(bams / 2) * 2;
	ClippingRelated = (float)((double)VerticalResolution / tan);
}

void __cdecl SetHorizontalFOV_BAMS_hook(int bams)
{
	// Instead of changing every call that sets this default value,
	// it's better to just detect it and act accordingly.
	if (bams == 12743)
		bams = fov_bams;

	int* _24 = (int*)&MyCoolMatrix._24;

	if (*_24 != bams)
		SetClippingRelatedThing_hook(bams);

	HorizontalFOV_BAMS = bams;
	*_24 = bams;
}

// TODO: Stop calculating FOV every frame. Use fov_rads instead.
void __declspec(naked) CalculateNewFOV()
{
	__asm
	{
		fstp	dword ptr[esp]
		fstp	dword ptr[esp]
		mov		eax, 03D12788h	// Direct3D_Viewport.Width
		mov		ecx, 03D1278Ch	// Direct3D_Viewport.Height
		fild	dword ptr[eax]
		fild	dword ptr[ecx]
		fpatan					// atan2 of width, height; FOV in radians
		fstp	dword ptr[esp]

		mov		eax, 00791251h	// return addr
		jmp		eax
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

void ConfigureFOV()
{
	static const double _4x3 = 4.0 / 3.0;
	const uint32_t width = HorizontalResolution;
	const uint32_t height = VerticalResolution;
	const double aspectRatio = (double)width / (double)height;

	// 4:3 and "tallscreen" (5:4, portrait, etc)
	// We don't need to do anything since these resolutions work fine with the default code.
	if ((height * _4x3) == width || (height * _4x3) > width)
		return;

	// Widescreen (16:9, 16:10, etc)
	if ((height * _4x3) < width)
	{
		fov_rads = atan2((double)width, (double)height);
		fov_bams = (int)((fov_rads * (180.0f / D3DX_PI)) * (65536.0 / 360.0));

		WriteJump(SetHorizontalFOV_BAMS, SetHorizontalFOV_BAMS_hook);
		WriteJump(SetClippingRelatedThing, SetClippingRelatedThing_hook);
		WriteJump((void*)0x0079124A, &CalculateNewFOV);
		WriteJump((void*)0x00781523, &FixFloatStackPls);

		SetHorizontalFOV_BAMS_hook(fov_bams);
	}
}
