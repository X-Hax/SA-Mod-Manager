/**
 * SADX Mod Loader.
 * Mod metadata structures.
 */

#ifndef SADXMODLOADER_SADXMODINFO_H
#define SADXMODLOADER_SADXMODINFO_H

#include "SADXStructs.h"
#include "SADXStructsNew.h"
#include "ScaleInfo.h"
#include <string>

// SADX Mod Loader API version.
static const int ModLoaderVer = 11;

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

#undef ReplaceFile // Windows function macro

struct HelperFunctions
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
	// Requires version >= 4.
	const char *(__cdecl *GetMainSavePath)();
	// Returns the path where Chao save files are stored.
	// Requires version >= 4.
	const char *(__cdecl *GetChaoSavePath)();

	/**
	* @brief Gets the real path to a replaceable file.
	*
	* If your mod contains files in its SYSTEM folder that it loads manually,
	* you can use this function to retrieve the full path to the file. This
	* allows other mods to replace this file without any extra work from you.
	* Requires version >= 5.
	*
	* @param path The file path (e.g "SYSTEM\\my_cool_file.bin")
	* @return The replaced path to the file.
	*/
	const char *(__cdecl* GetReplaceablePath)(const char* path);
	// Replaces the source file with the destination file.
	// Requires version >= 6.
	void(__cdecl *ReplaceFile)(const char *src, const char *dst);
	// Sets the window title.
	// Requires version >= 7.
	void(__cdecl *SetWindowTitle)(const char *title);

	// Registers a sound list.
	// Returns the index of the new sound list.
	// Requires version >= 8.
	int(__cdecl *RegisterSoundList)(const SoundList &list);

	// Registers a music file.
	// Returns the index of the new music file.
	// Requires version >= 9.
	int(__cdecl *RegisterMusicFile)(const MusicInfo &track);

	// Loads and processes an EXEData INI file.
	// Requires version >= 10.
	void(__cdecl *LoadEXEData)(const wchar_t *filename, const std::wstring &mod_dir);

	// Loads and processes a DLLData INI file.
	// Requires version >= 10.
	void(__cdecl *LoadDLLData)(const wchar_t *filename, const std::wstring &mod_dir);

	// Replaces the source file with the destination file without checking if the destination file is also being replaced.
	// Requires version >= 10.
	void(__cdecl* ReplaceFileForce)(const char* src, const char* dst);

	/**
	* @brief Adds a UI scale method to the queue, scaling all sprites drawn between this and PopScaleUI.
	*
	* By default, draw your sprites as if they were in a 640x480 window and the mod loader will handle scaling.
	* Requires version >= 11.
	*
	* @param align: The frame anchor.
	* @param background: Indicates if the frame should be treated as background.
	* @param ratio_h: The horizontal ratio of the frame (1.0f by default.)
	* @param ratio_v: The vertical ratio of the frame (1.0f by default.)
	*/
	void(__cdecl* PushScaleUI)(uiscale::Align align, bool is_background, float ratio_h, float ratio_v);

	// Removes the latest UI scale method from the queue.
	// Requires version >= 11.
	void(__cdecl* PopScaleUI)();

	// Force a specific filling method for background sprites, make sure to reset the original value once you're done.
	// Requires version >= 11.
	void(__cdecl* SetScaleFillMode)(uiscale::FillMode mode);

	// Returns the current filling method for background sprites.
	// Requires version >= 11.
	uiscale::FillMode(__cdecl* GetScaleFillMode)();
};

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
