#include "stdafx.h"

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


void Init_NOGbixHack()
{
	WriteJump((void*)0x77F5B0, GetCachedTexture_r);
}