/**
 * SADX Mod Loader.
 * Common header file for SADXModLoader mods.
 */

#ifndef SADXMODLOADER_H
#define SADXMODLOADER_H

#include "ninja.h"
#include <d3d9.h>

#ifdef _MSC_VER
// MSVC doesn't have snprintf(), but it does have _snprintf().
#define snprintf(str, size, format, ...) _snprintf(str, size, format, __VA_ARGS__)
#endif

// Split-out header files.
#include "ModLoader/MemAccess.h"
#include "SADXModLoader/SADXEnums.h"
#include "SADXModLoader/SADXStructs.h"
#include "SADXModLoader/SADXVars.h"
#include "SADXModLoader/SADXFunctions.h"

static inline void ResizeTextureList(NJS_TEXLIST *texlist, Uint32 count)
{
	texlist->textures = new NJS_TEXNAME[count];
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

// ModInfo

// SADX Mod Loader API version.
static const int ModLoaderVer = 3;

#define arrayptrandlength(data) data, LengthOfArray(data)
#define arraylengthandptr(data) LengthOfArray(data), data
#define arrayptrandsize(data) data, SizeOfArray(data)
#define arraysizeandptr(data) SizeOfArray(data), data

struct PatchInfo
{
	void *address;
	const void *data;
	int datasize;
};

#define patchdecl(address,data) { (void*)address, arrayptrandsize(data) }

struct PointerInfo
{
	void *address;
	void *data;
};

#define ptrdecl(address,data) { (void*)address, (void*)data }

struct HelperFunctions_v3
{
	// The version of the structure.
	int Version;
	// Registers a start position for a character.
	void (__cdecl *RegisterStartPosition)(unsigned char character, const StartPosition &position);
	// Clears the list of registered start positions for a character.
	void (__cdecl *ClearStartPositionList)(unsigned char character);
	// Registers a field start position for a character.
	void (__cdecl *RegisterFieldStartPosition)(unsigned char character, const FieldStartPosition &position);
	// Clears the list of registered field start positions for a character.
	void (__cdecl *ClearFieldStartPositionList)(unsigned char character);
	// Registers a path list.
	void (__cdecl *RegisterPathList)(const PathDataPtr &paths);
	// Clears the list of registered path lists.
	void (__cdecl *ClearPathListList)();
	// Registers a PVM file for a character.
	void (__cdecl *RegisterCharacterPVM)(unsigned char character, const PVMEntry &pvm);
	// Clears the list of registered PVM files for a character.
	void (__cdecl *ClearCharacterPVMList)(unsigned char character);
	// Registers a PVM file for a common object.
	void (__cdecl *RegisterCommonObjectPVM)(const PVMEntry &pvm);
	// Clears the list of registered PVM files for common objects.
	void (__cdecl *ClearCommonObjectPVMList)();
	// Registers a trial level entry for a character.
	void (__cdecl *RegisterTrialLevel)(unsigned char character, const TrialLevelListEntry &level);
	// Clears the list of registered trial level entries for a character.
	void (__cdecl *ClearTrialLevelList)(unsigned char character);
	// Registers a trial subgame entry for a character.
	void (__cdecl *RegisterTrialSubgame)(unsigned char character, const TrialLevelListEntry &level);
	// Clears the list of registered trial subgame entries for a character.
	void (__cdecl *ClearTrialSubgameList)(unsigned char character);
};

typedef HelperFunctions_v3 HelperFunctions;

struct ModInfo
{
	int Version;
	void (__cdecl *Init)(const char *path, const HelperFunctions &helperFunctions);
	const PatchInfo *Patches;
	int PatchCount;
	const PointerInfo *Jumps;
	int JumpCount;
	const PointerInfo *Calls;
	int CallCount;
	const PointerInfo *Pointers;
	int PointerCount;
};

#endif /* SADXMODLOADER_H */
