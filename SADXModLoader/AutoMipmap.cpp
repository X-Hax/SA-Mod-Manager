#include "stdafx.h"
#include <d3dx8tex.h>
#include <SADXModLoader.h>
#include "AutoMipmap.h"

static IDirect3DTexture8* __fastcall GenerateMipmaps_c(IDirect3DTexture8* d3d_texture, NJS_TEXMEMLIST* njs_texture)
{
	if (d3d_texture == nullptr || njs_texture == nullptr)
		return nullptr;

	Uint32 format = njs_texture->texinfo.texsurface.PixelFormat;

	// TODO: Figure out how Chao palettized textures are handled.
	if (format == NJD_PIXELFORMAT_PALETTIZED_4BPP || format == NJD_PIXELFORMAT_PALETTIZED_8BPP)
		PrintDebug("Palettized texture detected.\n");

	// TODO: Consider different filtering. By default, it's D3DX_FILTER_BOX | D3DX_FILTER_DITHER
	HRESULT result = D3DXFilterTexture(d3d_texture, nullptr, D3DX_DEFAULT, D3DX_DEFAULT);

	if (result != 0)
		PrintDebug("Mipmap generation failed with error code 0x%08X\n", result);

	D3DSURFACE_DESC info;
	d3d_texture->GetLevelDesc(0, &info);

	njs_texture->texinfo.texsurface.pSurface = (Uint32*)d3d_texture;
	njs_texture->texinfo.texsurface.TextureSize = info.Size;

	return d3d_texture;
}

void* ret_addr = (void*)0x0078CD37;
static void __declspec(naked) GenerateMipmaps_asm()
{
	// This could probably use some optimizing.
	__asm
	{
		mov		edx, esi
		mov		ecx, eax

		push	eax
		call	GenerateMipmaps_c
		pop		eax

		jmp		ret_addr
	}
}

void ConfigureAutoMipmaps()
{
	WriteJump((void*)0x0078CD2A, GenerateMipmaps_asm);	// Hooks the end of the function that converts PVRs to D3D textures
	WriteData((uint8_t*)0x0078C8B0, (uint8_t)0);		// Forces generation of empty mipmap levels for textures that don't normally have them
	WriteData((uint8_t*)0x0078C8F6, (uint8_t)0);		// Ditto
}