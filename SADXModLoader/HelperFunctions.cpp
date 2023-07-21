#include "stdafx.h"
#include "IniFile.hpp"
#include "jvList.h"
#include "DLLData.h"
#include "EXEData.h"
#include "MediaFns.hpp"

using std::vector;
using std::unordered_map;
using std::string;

extern unordered_map<unsigned char, unordered_map<int, StartPosition>> StartPositions;

static void RegisterStartPosition(unsigned char character, const StartPosition& position)
{
	auto iter = StartPositions.find(character);

	if (iter == StartPositions.end())
	{
		// No start positions registered for this character.
		// Initialize it with the default start positions.
		const StartPosition* origlist;
		switch (character)
		{
		case Characters_Sonic:
			origlist = SonicStartArray;
			break;
		case Characters_Tails:
			origlist = TailsStartArray;
			break;
		case Characters_Knuckles:
			origlist = KnucklesStartArray;
			break;
		case Characters_Amy:
			origlist = AmyStartArray;
			break;
		case Characters_Gamma:
			origlist = GammaStartArray;
			break;
		case Characters_Big:
			origlist = BigStartArray;
			break;
		default:
			return;
		}

		unordered_map<int, StartPosition>& newlist = StartPositions[character];

		for (; origlist->LevelID != LevelIDs_Invalid; origlist++)
		{
			newlist[levelact(origlist->LevelID, origlist->ActID)] = *origlist;
		}

		// Update the start position for the specified level.
		newlist[levelact(position.LevelID, position.ActID)] = position;
	}
	else
	{
		// Start positions have already been registered.
		// Update the existing map.
		iter->second[levelact(position.LevelID, position.ActID)] = position;
	}
}

static void ClearStartPositionList(unsigned char character)
{
	switch (character)
	{
	case Characters_Sonic:
	case Characters_Tails:
	case Characters_Knuckles:
	case Characters_Amy:
	case Characters_Gamma:
	case Characters_Big:
		break;
	default:
		return;
	}

	StartPositions[character].clear();
}

extern unordered_map<unsigned char, unordered_map<int, FieldStartPosition>> FieldStartPositions;

static void RegisterFieldStartPosition(unsigned char character, const FieldStartPosition& position)
{
	if (character >= Characters_MetalSonic)
	{
		return;
	}

	auto iter = FieldStartPositions.find(character);

	if (iter == FieldStartPositions.end())
	{
		// No field start positions registered for this character.
		// Initialize it with the default field start positions.
		const FieldStartPosition* origlist = StartPosList_FieldReturn[character];
		unordered_map<int, FieldStartPosition>& newlist = FieldStartPositions[character];
		for (; origlist->LevelID != LevelIDs_Invalid; origlist++)
		{
			newlist[levelact(origlist->LevelID, origlist->FieldID)] = *origlist;
		}

		// Update the field start position for the specified level.
		newlist[levelact(position.LevelID, position.FieldID)] = position;
	}
	else
	{
		// Field start positions have already been registered.
		// Update the existing map.
		iter->second[levelact(position.LevelID, position.FieldID)] = position;
	}
}

static void ClearFieldStartPositionList(unsigned char character)
{
	if (character >= Characters_MetalSonic)
	{
		return;
	}

	FieldStartPositions[character].clear();
}

extern unordered_map<int, PathDataPtr> Paths;
extern bool PathsInitialized;

static void RegisterPathList(const PathDataPtr& paths)
{
	if (!PathsInitialized)
	{
		for (const PathDataPtr* oldlist = PathDataPtrs; oldlist->LevelAct != 0xFFFF; oldlist++)
		{
			Paths[oldlist->LevelAct] = *oldlist;
		}

		PathsInitialized = true;
	}
	Paths[paths.LevelAct] = paths;
}

static void ClearPathListList()
{
	Paths.clear();
	PathsInitialized = true;
}

extern unordered_map<unsigned char, vector<PVMEntry>> CharacterPVMs;

static void RegisterCharacterPVM(unsigned char character, const PVMEntry& pvm)
{
	if (character > Characters_MetalSonic)
	{
		return;
	}

	auto iter = CharacterPVMs.find(character);

	if (iter == CharacterPVMs.end())
	{
		// Character PVM vector has not been created yet.
		// Initialize it with the texture list.
		const PVMEntry* origlist = CharacterPVMEntries[character];
		vector<PVMEntry>& newlist = CharacterPVMs[character];
		for (; origlist->TexList != nullptr; origlist++)
		{
			newlist.push_back(*origlist);
		}
		// Add the new PVM.
		newlist.push_back(pvm);
	}
	else
	{
		// Character PVM vector has been created.
		// Add the new texture.
		iter->second.push_back(pvm);
	}
}

static void ClearCharacterPVMList(unsigned char character)
{
	if (character > Characters_MetalSonic)
	{
		return;
	}

	CharacterPVMs[character].clear();
}

extern vector<PVMEntry> CommonObjectPVMs;
extern bool CommonObjectPVMsInitialized;

static void RegisterCommonObjectPVM(const PVMEntry& pvm)
{
	if (!CommonObjectPVMsInitialized)
	{
		const PVMEntry* oldlist = &OBJ_REGULAR_TEXLISTS[0];
		for (; oldlist->TexList != nullptr; oldlist++)
		{
			CommonObjectPVMs.push_back(*oldlist);
		}
		CommonObjectPVMsInitialized = true;
	}
	CommonObjectPVMs.push_back(pvm);
}

static void ClearCommonObjectPVMList()
{
	CommonObjectPVMs.clear();
	CommonObjectPVMsInitialized = true;
}

static unsigned char trialcharacters[] = { 0, 0xFFu, 1, 2, 0xFFu, 3, 5, 4, 6 };

static inline unsigned char gettrialcharacter(unsigned char character)
{
	if (character >= LengthOfArray(trialcharacters))
		return 0xFF;
	return trialcharacters[character];
}

extern unordered_map<unsigned char, vector<TrialLevelListEntry>> _TrialLevels;

static void RegisterTrialLevel(unsigned char character, const TrialLevelListEntry& level)
{
	character = gettrialcharacter(character);

	if (character == 0xFF)
	{
		return;
	}

	auto iter = _TrialLevels.find(character);
	if (iter == _TrialLevels.end())
	{
		// Trial level vector has not been registered yet.
		// Initialize it with the standard trial level list.
		const TrialLevelList* const origlist = &TrialLevels[character];
		vector<TrialLevelListEntry>& newlist = _TrialLevels[character];
		newlist.resize(origlist->Count);
		memcpy(newlist.data(), origlist->Levels, sizeof(TrialLevelListEntry) * origlist->Count);
		// Add the new trial level.
		newlist.push_back(level);
	}
	else
	{
		// Trial level vector has already been created.
		// Add the new level.
		iter->second.push_back(level);
	}
}

static void ClearTrialLevelList(unsigned char character)
{
	character = gettrialcharacter(character);

	if (character == 0xFF)
	{
		return;
	}

	_TrialLevels[character].clear();
}

extern unordered_map<unsigned char, vector<TrialLevelListEntry>> _TrialSubgames;

static void RegisterTrialSubgame(unsigned char character, const TrialLevelListEntry& level)
{
	character = gettrialcharacter(character);

	if (character == 0xFF)
	{
		return;
	}

	auto iter = _TrialSubgames.find(character);
	if (iter == _TrialSubgames.end())
	{
		// Trial subgame vector has not been registered yet.
		// Initialize it with the standard trial subgame list.
		const TrialLevelList* const origlist = &TrialSubgames[character];
		vector<TrialLevelListEntry>& newlist = _TrialSubgames[character];
		newlist.resize(origlist->Count);
		memcpy(newlist.data(), origlist->Levels, sizeof(TrialLevelListEntry) * origlist->Count);
		// Add the new trial subgame.
		newlist.push_back(level);
	}
	else
	{
		// Trial subgame vector has already been created.
		// Add the new subgame.
		iter->second.push_back(level);
	}
}

static void ClearTrialSubgameList(unsigned char character)
{
	character = gettrialcharacter(character);

	if (character == 0xFF)
	{
		return;
	}

	_TrialSubgames[character].clear();
}

extern const char* mainsavepath;

static const char* GetMainSavePath()
{
	return mainsavepath;
}

extern const char* chaosavepath;

static const char* GetChaoSavePath()
{
	return chaosavepath;
}

const char* __cdecl GetReplaceablePath(const char* path)
{
	return sadx_fileMap.replaceFile(path);
}

void _ReplaceFile(const char* src, const char* dst)
{
	sadx_fileMap.addReplaceFile(src, dst);
}

void _ReplaceFileForce(const char* src, const char* dst)
{
	sadx_fileMap.addReplaceFile(src, dst, true);
}

extern string windowtitle;

void SetWindowTitle(const char* title)
{
	if (WindowHandle)
		SetWindowTextA(WindowHandle, title);
	else
		windowtitle = title;
}

extern vector<SoundList> _SoundLists;

int RegisterSoundList(const SoundList& list)
{
	if (_SoundLists.empty())
	{
		_SoundLists.resize(SoundLists.size());
		memcpy(_SoundLists.data(), SoundLists, sizeof(SoundList) * SoundLists.size());
	}
	_SoundLists.push_back(list);
	SoundLists_Cust = _SoundLists.data();
	SoundLists_Cust_Length = _SoundLists.size();
	return _SoundLists.size() - 1;
}

extern vector<MusicInfo> _MusicList;

int RegisterMusicFile(const MusicInfo& track)
{
	if (_MusicList.empty())
	{
		_MusicList.resize(MusicList.size());
		memcpy(_MusicList.data(), MusicList, sizeof(MusicInfo) * MusicList.size());
	}
	_MusicList.push_back(track);
	return _MusicList.size() - 1;
}

void LoadEXEData(const wchar_t* filename, const wchar_t* mod_dir)
{
	ProcessEXEData(filename, mod_dir);
}

void LoadDLLData(const wchar_t* filename, const wchar_t* mod_dir)
{
	ProcessDLLData(filename, mod_dir);
}

void PushScaleUI(uiscale::Align align, bool is_background, float ratio_h, float ratio_v)
{
	uiscale::initialize_common(); // make sure sprite functions are hooked
	uiscale::scale_push(align, is_background, ratio_h, ratio_v);
}

void PopScaleUI()
{
	uiscale::scale_pop();
}

void SetScaleFillMode(uiscale::FillMode mode)
{
	uiscale::bg_fill = mode;
}

uiscale::FillMode GetScaleFillMode()
{
	return uiscale::bg_fill;
}

bool isInteger(const std::string& s)
{
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

	char* p;
	strtol(s.c_str(), &p, 10);

	return (*p == 0);
}

std::string base_name(std::string const& path)
{
	std::string remove = path.substr(path.find_last_of("/\\") + 1);
	std::string::size_type const p(remove.find_last_of('.'));
	std::string file_without_extension = remove.substr(0, p);
	return file_without_extension;

}

extern vector<__int16> _USVoiceDurationList;
extern vector<__int16> _JPVoiceDurationList;

void SetVoiceDuration(vector<__int16>& vec, int* voiceArray, uint32_t size, const uint16_t voiceID, const uint16_t duration)
{
	if (vec.empty()) //copy original duration voice list
	{
		vec.resize(size);
		memcpy(&vec[vec.size() - size], &voiceArray[0], sizeof(__int16) * size);
	}

	short curSize = _USVoiceDurationList.size();

	if (voiceID > INT16_MAX)
		return;

	if (voiceID >= curSize) //if the user requested a voice ID out of bound, resize vector length.
	{
		vec.resize(voiceID + 1);
	}

	vec.at(voiceID) = duration; //finally, add the new duration to the specificied voice.
#ifdef _DEBUG
	PrintDebug("Edited Voice %d, new duration: %d\n", voiceID, duration);
#endif
}

void RegisterEnglishVoiceDuration(const uint16_t voiceID, const uint16_t duration)
{
	SetVoiceDuration(_USVoiceDurationList, duration_us, duration_us.size(), voiceID, duration);
}

void RegisterJapaneseVoiceDuration(const uint16_t voiceID, const uint16_t duration)
{
	SetVoiceDuration(_JPVoiceDurationList, duration_jp, duration_jp.size(), voiceID, duration);
}


void RegisterCharacterWelds(const uint8_t character, const char* iniPath)
{
	if (!FileExists(iniPath))
	{
#ifdef _DEBUG
		PrintDebug("Failed to read welds ini file; welds won't be edited.\n");
#endif
		return;
	}

	IniFile* ini = new IniFile(std::string(iniPath));
	SetNewWelds(character, ini);
}

extern std::vector<Mod> modlist;

namespace ModListImpl
{
	ModList::iterator begin()
	{
		return &*modlist.cbegin();
	}

	ModList::iterator end()
	{
		return &*modlist.cend();
	}

	ModList::reference at(ModList::size_type pos)
	{
		return modlist[pos];
	}

	ModList::pointer data()
	{
		return modlist.data();
	}

	ModList::size_type size()
	{
		return modlist.size();
	}

	ModList::iterator find(const char* id)
	{
		for (auto& iter : modlist)
			if (!strcmp(iter.ID, id))
				return &iter;
		return nullptr;
	}

	ModList::iterator find_by_name(const char* name)
	{
		for (auto& iter : modlist)
			if (!strcmp(iter.Name, name))
				return &iter;
		return nullptr;
	}

	ModList::iterator find_by_folder(const char* folder)
	{
		for (auto& iter : modlist)
			if (!strcmp(iter.Folder, folder))
				return &iter;
		return nullptr;
	}

	ModList::iterator find_by_dll(HMODULE handle)
	{
		for (auto& iter : modlist)
			if (iter.DLLHandle == handle)
				return &iter;
		return nullptr;
	}
}

ModList modList = {
	ModListImpl::begin,
	ModListImpl::end,
	ModListImpl::at,
	ModListImpl::data,
	ModListImpl::size,
	ModListImpl::find,
	ModListImpl::find_by_name,
	ModListImpl::find_by_folder,
	ModListImpl::find_by_dll
};

extern LoaderSettings loaderSettings;

HelperFunctions helperFunctions =
{
	ModLoaderVer,
	&RegisterStartPosition,
	&ClearStartPositionList,
	&RegisterFieldStartPosition,
	&ClearFieldStartPositionList,
	&RegisterPathList,
	&ClearPathListList,
	&RegisterCharacterPVM,
	&ClearCharacterPVMList,
	&RegisterCommonObjectPVM,
	&ClearCommonObjectPVMList,
	&RegisterTrialLevel,
	&ClearTrialLevelList,
	&RegisterTrialSubgame,
	&ClearTrialSubgameList,
	&GetMainSavePath,
	&GetChaoSavePath,
	&GetReplaceablePath,
	&_ReplaceFile,
	&SetWindowTitle,
	&RegisterSoundList,
	&RegisterMusicFile,
	&LoadEXEData,
	&LoadDLLData,
	&_ReplaceFileForce,
	&PushScaleUI,
	&PopScaleUI,
	&SetScaleFillMode,
	&GetScaleFillMode,
	&ReplaceTexture,
	&MipmapBlacklistGBIX,
	&RegisterEnglishVoiceDuration,
	&RegisterJapaneseVoiceDuration,
	&RegisterCharacterWelds,
	&loaderSettings,
	&modList
};
