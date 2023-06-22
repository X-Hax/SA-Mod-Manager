#include "stdafx.h"

FastcallFunctionPointer(void, Direct3D_PvrToTexture, (NJS_TEXMEMLIST* tex, IDirect3DTexture8* surface), 0x78CBD0);

NJS_TEXMEMLIST* __cdecl GetCachedTexture_r(int gbix)
{
	NJS_TEXMEMLIST* result = nullptr;
	NJS_TEXMEMLIST* t = GlobalTextures_p;
	int i = 0;

	if (GlobalTextureCount)
	{
		while (t->count)
		{
			++i;
			++t;
			if (i >= GlobalTextureCount)
			{
				return result;
			}
		}
		result = t;
	}

	return result;
}

Sint32 __cdecl sub_77FA10_r(Uint32 gbix, IDirect3DTexture8* texture)
{
	NJS_TEXMEMLIST* memlist = GlobalTextures_p;

	DoSomethingWithPalette((NJS_TEXPALETTE*)memlist->bank);
	Direct3D_PvrToTexture(memlist, texture);

	return 1;
}

void Init_NOGbixHack()
{
	WriteJump((void*)0x77F5B0, GetCachedTexture_r);
	WriteJump((void*)0x77FA10, sub_77FA10_r);
}