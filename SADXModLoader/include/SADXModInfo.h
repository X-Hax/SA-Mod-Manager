/**
 * SADX Mod Loader.
 * Mod metadata structures.
 */

#ifndef SADXMODLOADER_SADXMODINFO_H
#define SADXMODLOADER_SADXMODINFO_H

#include "SADXStructs.h"

// SADX Mod Loader API version.
static const int ModLoaderVer = 5;

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

struct HelperFunctions_v5
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

	/**
	* @brief Gets the real path to a replaceable file.
	*
	* If your mod contains files in its SYSTEM folder that it loads manually,
	* you can use this function to retrieve the full path to the file. This
	* allows other mods to replace this file without any extra work from you.
	*
	* @param path The file path (e.g "SYSTEM\\my_cool_file.bin")
	* @return The replaced path to the file.
	*/
	const char* (__cdecl* GetReplaceablePath)(const char* path);
};

typedef HelperFunctions_v5 HelperFunctions;

typedef void(__cdecl *ModInitFunc)(const char *path, const HelperFunctions &helperFunctions);

typedef void(__cdecl *ModEvent)();

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

#endif // SADXMODLOADER_SADXMODINFO_H
