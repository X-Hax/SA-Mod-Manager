#include "stdafx.h"

#include <SADXModLoader.h>

#include "AutoMipmap.h"
#include "D3DCommon.h"

static HRESULT CopyTexture(IDirect3DTexture8* dest, IDirect3DTexture8* src, UINT height)
{
	HRESULT result;
	D3DLOCKED_RECT rectA, rectB;

	result = src->LockRect(0, &rectA, nullptr, 0);

	if (!SUCCEEDED(result))
		return result;

	result = dest->LockRect(0, &rectB, nullptr, 0);

	if (!SUCCEEDED(result))
	{
		src->UnlockRect(0);
		return result;
	}

	memcpy(rectB.pBits, rectA.pBits, rectA.Pitch * height);

	src->UnlockRect(0);
	dest->UnlockRect(0);

	return result;
}

inline void SetSurface(IDirect3DTexture8* src, NJS_TEXSURFACE* surface)
{
	D3DSURFACE_DESC info;
	src->GetLevelDesc(0, &info);

	surface->pSurface = (Uint32*)src;
	surface->TextureSize = info.Size;
}

/// <summary>
/// Generates mipmaps if the specified texture has the levels to accommodate them.
/// </summary>
/// <param name="src">The DirectX texture to apply mipmaps to.</param>
/// <param name="njs_texture">The Dremcast texture to receive the DirectX texture.</param>
static void __fastcall GenerateMipmaps_c(IDirect3DTexture8* src, NJS_TEXMEMLIST* njs_texture)
{
	if (src == nullptr || njs_texture == nullptr)
		return;

#ifndef PALLETIZED_MIPMAPS
	Uint32 format = njs_texture->texinfo.texsurface.PixelFormat;

	if (format == NJD_PIXELFORMAT_PALETTIZED_4BPP || format == NJD_PIXELFORMAT_PALETTIZED_8BPP || src->GetLevelCount() > 1)
		goto ABORT;
#endif

	if (src->GetLevelCount() > 1)
		goto ABORT;

	HRESULT result;

	D3DSURFACE_DESC info;
	result = src->GetLevelDesc(0, &info);
	if (!SUCCEEDED(result))
		goto ABORT;

	IDirect3DTexture8* dest;
	result = Direct3D_Device->CreateTexture(info.Width, info.Height, 0, info.Usage, info.Format, info.Pool, &dest);
	if (!SUCCEEDED(result))
		goto ABORT;

	result = CopyTexture(dest, src, info.Height);
	if (!SUCCEEDED(result))
		goto ABORT;

	result = D3DXFilterTexture(dest, nullptr, D3DX_DEFAULT, D3DX_DEFAULT);
	if (!SUCCEEDED(result))
	{
		PrintDebug("Mipmap generation failed with error code 0x%08X\n", result);
		dest->Release();
		goto ABORT;
	}

	src->Release();
	SetSurface(dest, &njs_texture->texinfo.texsurface);
	return;

ABORT:
	SetSurface(src, &njs_texture->texinfo.texsurface);
	return;
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

#pragma endregion

#ifdef PALLETIZED_MIPMAPS
void __cdecl GeneratePalletizedMipmaps_c(IDirect3DTexture8* src)
{
	if (src->GetLevelCount() >= 2)
		D3DXFilterTexture(src, nullptr, 0, D3DX_DEFAULT);
}

void* loc_78CE10 = (void*)0x0078CE10;
void __declspec(naked) GeneratePalletizedMipmaps_asm()
{
	__asm
	{
		push	edi
		call	test_c
		pop		edi
		jmp		loc_78CE10
	}
}
#endif

void EnableAutoMipmaps()
{
	WriteJump((void*)0x0078CD2A, GenerateMipmaps_asm);	// Hooks the end of the function that converts PVRs to D3D textures
#ifdef PALLETIZED_MIPMAPS
	// This happens every frame for every palletized texture in the scene. Rather inefficient where mipmap generation is concerned.
	// Assuming somebody can figure out a method of keeping track of palette changes and textures who's mips have already been generated,
	// we can enable this for mainstream builds. Otherwise it should stay off.
	WriteJump((void*)0x0078CF06, GeneratePalletizedMipmaps_asm);
#endif
}