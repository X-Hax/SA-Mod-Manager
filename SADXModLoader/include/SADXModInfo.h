/**
 * SADX Mod Loader.
 * Mod metadata structures.
 */

#ifndef SADXMODLOADER_SADXMODINFO_H
#define SADXMODLOADER_SADXMODINFO_H

#include "SADXStructs.h"
#include "SADXStructsNew.h"
#include "ScaleInfo.h"

 // SADX Mod Loader API version.
static const int ModLoaderVer = 14;
struct PatchInfo
{
	void* address;
	const void* data;
	int datasize;
};

struct PatchList
{
	const PatchInfo* Patches;
	int Count;
};

struct PointerInfo
{
	void* address;
	void* data;
};

struct PointerList
{
	const PointerInfo* Pointers;
	int Count;
};

#undef ReplaceFile // Windows function macro

struct HelperFunctions
{
	// The version of the structure.
	int Version;
	// Registers a start position for a character.
	void(__cdecl* RegisterStartPosition)(unsigned char character, const StartPosition& position);
	// Clears the list of registered start positions for a character.
	void(__cdecl* ClearStartPositionList)(unsigned char character);
	// Registers a field start position for a character.
	void(__cdecl* RegisterFieldStartPosition)(unsigned char character, const FieldStartPosition& position);
	// Clears the list of registered field start positions for a character.
	void(__cdecl* ClearFieldStartPositionList)(unsigned char character);
	// Registers a path list.
	void(__cdecl* RegisterPathList)(const PathDataPtr& paths);
	// Clears the list of registered path lists.
	void(__cdecl* ClearPathListList)();
	// Registers a PVM file for a character.
	void(__cdecl* RegisterCharacterPVM)(unsigned char character, const PVMEntry& pvm);
	// Clears the list of registered PVM files for a character.
	void(__cdecl* ClearCharacterPVMList)(unsigned char character);
	// Registers a PVM file for a common object.
	void(__cdecl* RegisterCommonObjectPVM)(const PVMEntry& pvm);
	// Clears the list of registered PVM files for common objects.
	void(__cdecl* ClearCommonObjectPVMList)();
	// Registers a trial level entry for a character.
	void(__cdecl* RegisterTrialLevel)(unsigned char character, const TrialLevelListEntry& level);
	// Clears the list of registered trial level entries for a character.
	void(__cdecl* ClearTrialLevelList)(unsigned char character);
	// Registers a trial subgame entry for a character.
	void(__cdecl* RegisterTrialSubgame)(unsigned char character, const TrialLevelListEntry& level);
	// Clears the list of registered trial subgame entries for a character.
	void(__cdecl* ClearTrialSubgameList)(unsigned char character);
	// Returns the path where main game save files are stored.
	// Requires version >= 4.
	const char* (__cdecl* GetMainSavePath)();
	// Returns the path where Chao save files are stored.
	// Requires version >= 4.
	const char* (__cdecl* GetChaoSavePath)();

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
	const char* (__cdecl* GetReplaceablePath)(const char* path);
	// Replaces the source file with the destination file.
	// Requires version >= 6.
	void(__cdecl* ReplaceFile)(const char* src, const char* dst);
	// Sets the window title.
	// Requires version >= 7.
	void(__cdecl* SetWindowTitle)(const char* title);

	// Registers a sound list.
	// Returns the index of the new sound list.
	// Requires version >= 8.
	int(__cdecl* RegisterSoundList)(const SoundList& list);

	// Registers a music file.
	// Returns the index of the new music file.
	// Requires version >= 9.
	int(__cdecl* RegisterMusicFile)(const MusicInfo& track);

	// Loads and processes an EXEData INI file.
	// Requires version >= 10.
	void(__cdecl* LoadEXEData)(const wchar_t* filename, const wchar_t* mod_dir);

	// Loads and processes a DLLData INI file.
	// Requires version >= 10.
	void(__cdecl* LoadDLLData)(const wchar_t* filename, const wchar_t* mod_dir);

	// Replaces the source file with the destination file without checking if the destination file is also being replaced.
	// Requires version >= 10.
	void(__cdecl* ReplaceFileForce)(const char* src, const char* dst);

	/**
	* @brief Push a UI scale element onto the scale stack.
	*
	* By default, draw your sprites as if they were in a 640x480 canvas and the mod loader will handle scaling.
	* Once the sprites are drawn, please remove the UI scale element from the stack with PopScaleUI.
	* Requires version >= 11.
	*
	* @param align: The canvas anchor.
	* @param background: Indicates if the canvas should be treated as background.
	* @param ratio_h: The horizontal ratio of the canvas. When in doubt, leave as \c 1.0f
	* @param ratio_v: The vertical ratio of the canvas. When in doubt, leave as \c 1.0f
	*/
	void(__cdecl* PushScaleUI)(uiscale::Align align, bool is_background, float ratio_h, float ratio_v);

	// Pop a UI scale element off of the top of the scale stack.
	// Requires version >= 11.
	void(__cdecl* PopScaleUI)();

	// Force a specific filling method for background sprites.
	// Make sure to reset the original value.
	// Requires version >= 11.
	void(__cdecl* SetScaleFillMode)(uiscale::FillMode mode);

	// Returns the current filling method for background sprites.
	// Requires version >= 11.
	uiscale::FillMode(__cdecl* GetScaleFillMode)();

	// Replaces an individual texture from a PVM file with an image file.
	// Requires version >= 12.
	void(__cdecl* ReplaceTexture)(const char* pvm_name, const char* tex_name, const char* file_path, uint32_t gbix, uint32_t width, uint32_t height);

	// Disables mipmap generation for the specified Global Index.
	// Requires version >= 13.
	void(__cdecl* MipmapBlacklistGBIX)(Uint32 index);

	// Registers a voice duration, use it when you edit or add voices to fix cutscene progress.
	// Requires version >= 14.
	void(__cdecl* RegisterEnglishVoiceDuration)(const uint32_t voiceID, const uint32_t durationMS);
	void(__cdecl* RegisterJapaneseVoiceDuration)(const uint32_t voiceID, const uint32_t durationMS);
};

typedef void(__cdecl* ModInitFunc)(const char* path, const HelperFunctions& helperFunctions);

typedef void(__cdecl* ModEvent)();

struct ModInfo
{
	int Version;
	ModInitFunc Init;
	const PatchInfo* Patches;
	int PatchCount;
	const PointerInfo* Jumps;
	int JumpCount;
	const PointerInfo* Calls;
	int CallCount;
	const PointerInfo* Pointers;
	int PointerCount;
};

#endif // SADXMODLOADER_SADXMODINFO_H
