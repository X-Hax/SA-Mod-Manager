/**
 * SADX Mod Loader.
 * Common header file for SADXModLoader mods.
 */

#ifndef SADXMODLOADER_H
#define SADXMODLOADER_H

#if !defined(_M_IX86) && !defined(__i386__)
#error Mods must be built targeting 32-bit x86, change your settings.
#endif

#include "ninja.h"

// HACK: Fixes conflict with Windows API PlaySound.
#ifdef PlaySound
#undef PlaySound
#endif

// Split-out header files.
#include "MemAccess.h"
#include "SADXModInfo.h"
#include "SADXEnums.h"
#include "SADXStructs.h"
#include "SADXVariables.h"
#include "SADXFunctions.h"
#include "SADXEnumsNew.h"
#include "SADXStructsNew.h"
#include "SADXVariablesNew.h"
#include "SADXFunctionsNew.h"

static inline void ResizeTextureList(NJS_TEXLIST *texlist, Uint32 count)
{
	texlist->textures = new NJS_TEXNAME[count] {};
	texlist->nbTexture = count;
}

static inline void ResizeTextureList(NJS_TEXLIST *texlist, NJS_TEXNAME *textures, Uint32 count)
{
	texlist->textures = textures;
	texlist->nbTexture = count;
}

template <Uint32 N>
static inline void ResizeTextureList(NJS_TEXLIST *texlist, NJS_TEXNAME(&textures)[N])
{
	ResizeTextureList(texlist, textures, N);
}

#endif /* SADXMODLOADER_H */
