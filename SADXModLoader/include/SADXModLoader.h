/**
 * SADX Mod Loader.
 * Common header file for SADXModLoader mods.
 */

#ifndef SADXMODLOADER_H
#define SADXMODLOADER_H

#include "ninja.h"

// HACK: Fixes conflict with Windows API PlaySound.
#ifdef PlaySound
#undef PlaySound
#endif

// Split-out header files.
#include "MemAccess.h"
#include "SADXEnums.h"
#include "SADXStructs.h"
#include "SADXVariables.h"
#include "SADXFunctions.h"

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
static const int ModLoaderVer = 4;

struct PatchInfo
{
	void *address;
	const void *data;
	int datasize;
};

struct PatchList
{
	const PatchInfo *Patches;
	int Count;
};

struct PointerInfo
{
	void *address;
	void *data;
};

struct PointerList
{
	const PointerInfo *Pointers;
	int Count;
};

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

struct HelperFunctions_v4
{
	// The version of the structure.
	int Version;
	// Registers a start position for a character.
	void(__cdecl *RegisterStartPosition)(unsigned char character, const StartPosition &position);
	// Clears the list of registered start positions for a character.
	void(__cdecl *ClearStartPositionList)(unsigned char character);
	// Registers a field start position for a character.
	void(__cdecl *RegisterFieldStartPosition)(unsigned char character, const FieldStartPosition &position);
	// Clears the list of registered field start positions for a character.
	void(__cdecl *ClearFieldStartPositionList)(unsigned char character);
	// Registers a path list.
	void(__cdecl *RegisterPathList)(const PathDataPtr &paths);
	// Clears the list of registered path lists.
	void(__cdecl *ClearPathListList)();
	// Registers a PVM file for a character.
	void(__cdecl *RegisterCharacterPVM)(unsigned char character, const PVMEntry &pvm);
	// Clears the list of registered PVM files for a character.
	void(__cdecl *ClearCharacterPVMList)(unsigned char character);
	// Registers a PVM file for a common object.
	void(__cdecl *RegisterCommonObjectPVM)(const PVMEntry &pvm);
	// Clears the list of registered PVM files for common objects.
	void(__cdecl *ClearCommonObjectPVMList)();
	// Registers a trial level entry for a character.
	void(__cdecl *RegisterTrialLevel)(unsigned char character, const TrialLevelListEntry &level);
	// Clears the list of registered trial level entries for a character.
	void(__cdecl *ClearTrialLevelList)(unsigned char character);
	// Registers a trial subgame entry for a character.
	void(__cdecl *RegisterTrialSubgame)(unsigned char character, const TrialLevelListEntry &level);
	// Clears the list of registered trial subgame entries for a character.
	void(__cdecl *ClearTrialSubgameList)(unsigned char character);
	// Returns the path where main game save files are stored.
	const char *(__cdecl *GetMainSavePath)();
	// Returns the path where Chao save files are stored.
	const char *(__cdecl *GetChaoSavePath)();
};

typedef HelperFunctions_v4 HelperFunctions;

typedef void (__cdecl *ModInitFunc)(const char *path, const HelperFunctions &helperFunctions);

typedef void (__cdecl *ModEvent)();

struct ModInfo
{
	int Version;
	ModInitFunc Init;
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
