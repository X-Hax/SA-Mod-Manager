#include "stdafx.h"

#include <d3dx8tex.h>
#include <SADXModLoader.h>

#include "AutoMipmap.h"

static bool mipmapsEnabled = false;

inline void SetSurface(IDirect3DTexture8* d3d_texture, NJS_TEXSURFACE* texsurface)
{
	D3DSURFACE_DESC info;
	d3d_texture->GetLevelDesc(0, &info);

	texsurface->pSurface = (Uint32*)d3d_texture;
	texsurface->TextureSize = info.Size;
}

/// <summary>
/// Generates mipmaps if the specified texture has the levels to accommodate them.
/// </summary>
/// <param name="d3d_texture">The DirectX texture to apply mipmaps to.</param>
/// <param name="njs_texture">The Dremcast texture to receive the DirectX texture.</param>
static void __fastcall GenerateMipmaps_c(IDirect3DTexture8* d3d_texture, NJS_TEXMEMLIST* njs_texture)
{
	if (d3d_texture == nullptr || njs_texture == nullptr)
		return;

	// TODO: Figure out how Chao palettized textures are handled.
	Uint32 format = njs_texture->texinfo.texsurface.PixelFormat;
	if (format == NJD_PIXELFORMAT_PALETTIZED_4BPP || format == NJD_PIXELFORMAT_PALETTIZED_8BPP || d3d_texture->GetLevelCount() < 2)
	{
		SetSurface(d3d_texture, &njs_texture->texinfo.texsurface);
		return;
	}

	// TODO: Consider different filtering. By default, it's D3DX_FILTER_BOX | D3DX_FILTER_DITHER
	HRESULT result = D3DXFilterTexture(d3d_texture, nullptr, D3DX_DEFAULT, D3DX_DEFAULT);

	if (result != 0)
		PrintDebug("Mipmap generation failed with error code 0x%08X\n", result);

	SetSurface(d3d_texture, &njs_texture->texinfo.texsurface);
}

/// <summary>
/// Disables mipmaps for palettized textures as a work around.
/// </summary>
/// <param name="format">The texture's pixel format.</param>
static void __stdcall EnableMipmaps(uint32_t format)
{
	if (format == NJD_PIXELFORMAT_PALETTIZED_4BPP || format == NJD_PIXELFORMAT_PALETTIZED_8BPP)
	{
		if (mipmapsEnabled)
		{
			// Default behavior
			WriteData((uint8_t*)0x0078C8B0, (uint8_t)1);
			WriteData((uint8_t*)0x0078C8F6, (uint8_t)1);
			mipmapsEnabled = false;
		}
	}
	else if (!mipmapsEnabled)
	{
		// Forces generation of empty mipmap levels for textures that don't normally have them
		WriteData((uint8_t*)0x0078C8B0, (uint8_t)0);
		WriteData((uint8_t*)0x0078C8F6, (uint8_t)0);
		mipmapsEnabled = true;
	}
}

#pragma region Assembly

static void* GenerateMipmaps_return = (void*)0x0078CD37;
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

		jmp		GenerateMipmaps_return
	}
}

static void* EnableMipmaps_return = (void*)0x0078C889;
static void __declspec(naked) EnableMipmaps_asm()
{
	__asm
	{
		push	eax
		push	ecx
		push	[esp + 16]
		call	EnableMipmaps
		pop		ecx
		pop		eax

		// Stuff overwritten by the jump to this code
		sub		esp, 14h
		push	ebx
		push	ebp
		mov		ebp, [esp+1Ch+0Ch]
		jmp		EnableMipmaps_return
	}
}

#pragma endregion

void ConfigureAutoMipmaps()
{
	WriteJump((void*)0x0078CD2A, GenerateMipmaps_asm);	// Hooks the end of the function that converts PVRs to D3D textures
	WriteJump((void*)0x0078C880, EnableMipmaps_asm);	// Toggles mipmaps off for palettized textures, on otherwise.
}