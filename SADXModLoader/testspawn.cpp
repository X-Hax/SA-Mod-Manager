#include "stdafx.h"
#include <ShellAPI.h>
#include <string>
#include <unordered_map>

static const std::unordered_map<std::wstring, uint8_t> level_name_ids_map = {
	{ L"hedgehoghammer",    LevelIDs_HedgehogHammer },
	{ L"emeraldcoast",      LevelIDs_EmeraldCoast },
	{ L"windyvalley",       LevelIDs_WindyValley },
	{ L"twinklepark",       LevelIDs_TwinklePark },
	{ L"speedhighway",      LevelIDs_SpeedHighway },
	{ L"redmountain",       LevelIDs_RedMountain },
	{ L"skydeck",           LevelIDs_SkyDeck },
	{ L"lostworld",         LevelIDs_LostWorld },
	{ L"icecap",            LevelIDs_IceCap },
	{ L"casinopolis",       LevelIDs_Casinopolis },
	{ L"finalegg",          LevelIDs_FinalEgg },
	{ L"hotshelter",        LevelIDs_HotShelter },
	{ L"chaos0",            LevelIDs_Chaos0 },
	{ L"chaos2",            LevelIDs_Chaos2 },
	{ L"chaos4",            LevelIDs_Chaos4 },
	{ L"chaos6",            LevelIDs_Chaos6 },
	{ L"perfectchaos",      LevelIDs_PerfectChaos },
	{ L"egghornet",         LevelIDs_EggHornet },
	{ L"eggwalker",         LevelIDs_EggWalker },
	{ L"eggviper",          LevelIDs_EggViper },
	{ L"zero",              LevelIDs_Zero },
	{ L"e101",              LevelIDs_E101 },
	{ L"e101r",             LevelIDs_E101R },
	{ L"stationsquare",     LevelIDs_StationSquare },
	{ L"eggcarrieroutside", LevelIDs_EggCarrierOutside },
	{ L"eggcarrierinside",  LevelIDs_EggCarrierInside },
	{ L"mysticruins",       LevelIDs_MysticRuins },
	{ L"past",              LevelIDs_Past },
	{ L"twinklecircuit",    LevelIDs_TwinkleCircuit },
	{ L"skychase1",         LevelIDs_SkyChase1 },
	{ L"skychase2",         LevelIDs_SkyChase2 },
	{ L"sandhill",          LevelIDs_SandHill },
	{ L"ssgarden",          LevelIDs_SSGarden },
	{ L"ecgarden",          LevelIDs_ECGarden },
	{ L"mrgarden",          LevelIDs_MRGarden },
	{ L"chaorace",          LevelIDs_ChaoRace }
};

static uint8_t parse_level_id(const std::wstring& str)
{
	std::wstring lowercase = str;
	std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), ::towlower);

	const auto it = level_name_ids_map.find(lowercase);

	if (it != level_name_ids_map.end())
		return it->second;

	return static_cast<uint8_t>(std::stol(lowercase));
}

static const std::unordered_map<std::wstring, uint8_t> character_name_ids_map = {
	{ L"sonic",      Characters_Sonic },
	{ L"eggman",     Characters_Eggman },
	{ L"tails",      Characters_Tails },
	{ L"knuckles",   Characters_Knuckles },
	{ L"tikal",      Characters_Tikal },
	{ L"amy",        Characters_Amy },
	{ L"gamma",      Characters_Gamma },
	{ L"big",        Characters_Big },
	{ L"metalsonic", Characters_MetalSonic }
};

static uint8_t parse_character_id(const std::wstring& str)
{
	std::wstring lowercase = str;
	transform(lowercase.begin(), lowercase.end(), lowercase.begin(), ::towlower);

	const auto it = character_name_ids_map.find(lowercase);

	if (it != character_name_ids_map.end())
		return it->second;

	return static_cast<uint8_t>(std::stol(lowercase));
}

static void parse_save(const std::wstring str, const HelperFunctions& helperFunctions)
{
	// To normal chars
	std::string str_(str.begin(), str.end());

	// Insert leading zero if needed
	if (str_.size() < 2)
	{
		str_.insert(0, "0");
	}

	// Get path to save file
	std::string savepath = (std::string)helperFunctions.GetMainSavePath() + (std::string)"\\SonicDX" + str_ + (std::string)".snc";

	PrintDebug("Loading save: \"%s\"\n", savepath.c_str());

	FILE* file;

	// Open save file
	if (!fopen_s(&file, savepath.c_str(), "rb"))
	{
		fread_s(&SaveFile, sizeof(SaveFileData), sizeof(SaveFileData), 1, file);
		fclose(file);

		// Prevent save stuff from being overwritten
		WriteData<1>(reinterpret_cast<void*>(0x425AF0), 0xC3); // Lives
		WriteData<24>(reinterpret_cast<void*>(0x41330F), 0x90); // EventFlags
		WriteData<5>(reinterpret_cast<void*>(0x0042D7CC), 0x90u); // CurrentCharacter

		LoadSave();
	}
}

static void Obj_Icecap_r(ObjectMaster* o)
{
	if (o)
	{
		MovePlayerToStartPoint(EntityData1Ptrs[0]);
		o->MainSub = Obj_Icecap;
		Obj_Icecap(o);
	}
}

static void DisableMusic()
{
	Music_Enabled = false;
}

static void DisableVoice()
{	
	EnableVoice = false;
}

static void DisableSound()
{
	// RET. Prevents the SoundQueue from running.
	WriteData<1>(reinterpret_cast<void*>(0x004250D0), 0xC3);
}

void TestSpawnCheckArgs(const HelperFunctions& helperFunctions)
{
	int argc = 0;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	bool level_set = false;
	bool act_set = false;

	for (int i = 1; i < argc; i++)
	{
		if (!wcscmp(argv[i], L"--level") || !wcscmp(argv[i], L"-l"))
		{
			CurrentLevel = parse_level_id(argv[++i]);
			PrintDebug("Loading level: %d\n", CurrentLevel);
			level_set = true;
		}
		else if (!wcscmp(argv[i], L"--act") || !wcscmp(argv[i], L"-a"))
		{
			CurrentAct = _wtoi(argv[++i]);
			PrintDebug("Loading act: %d\n", CurrentAct);
			act_set = true;
		}
		else if (!wcscmp(argv[i], L"--character") || !wcscmp(argv[i], L"-c"))
		{
			uint8_t character_id = parse_character_id(argv[++i]);

			if (character_id == Characters_MetalSonic)
			{
				MetalSonicFlag = 1;
				character_id = 0;
			}

			CurrentCharacter = character_id;

			// NOP. Prevents CurrentCharacter from being overwritten at initialization.
			WriteData<5>(reinterpret_cast<void*>(0x00415007), 0x90u);

			PrintDebug("Loading character: %d\n", CurrentCharacter);
		}
		else if (!wcscmp(argv[i], L"--save") || !wcscmp(argv[i], L"-s"))
		{
			parse_save(argv[++i], helperFunctions);
		}
		else if (!wcscmp(argv[i], L"--position") || !wcscmp(argv[i], L"-p"))
		{
			if (!level_set && !act_set)
			{
				MessageBoxA(nullptr, "Insufficient arguments for parameter: --position.\n"
					"Either --level or --act must be specified before --position.",
					"Insufficient arguments", MB_OK);

				continue;
			}

			if (i + 3 >= argc)
			{
				MessageBoxA(nullptr, "Insufficient arguments for parameter: --position.\n"
					"All 3 components (X, Y, Z) of the spawn position must be provided. Default spawn point will be used.",
					"Insufficient arguments", MB_OK);

				continue;
			}

			if (CurrentLevel == LevelIDs_IceCap)
			{
				// NOPs to disable several checks for LevelIDs_IceCap which prevent
				// correct player positioning and orienting.
				WriteData<2>(reinterpret_cast<void*>(0x004149EC), 0x90u);
				WriteData<2>(reinterpret_cast<void*>(0x0041497F), 0x90u);
				WriteData<2>(reinterpret_cast<void*>(0x00414A70), 0x90u);

				LevelObjects[LevelIDs_IceCap] = Obj_Icecap_r;
			}

			const float x = std::stof(argv[++i]);
			const float y = std::stof(argv[++i]);
			const float z = std::stof(argv[++i]);

			StartPosition position = {
				CurrentLevel,
				static_cast<int16_t>(CurrentAct),
				{ x, y, z },	// Position
				0				// YRot
			};

			helperFunctions.RegisterStartPosition(static_cast<Uint8>(CurrentCharacter), position);
		}
		else if (!wcscmp(argv[i], L"--no-music"))
		{
			DisableMusic();
		}
		else if (!wcscmp(argv[i], L"--no-voice"))
		{
			DisableVoice();
		}
		else if (!wcscmp(argv[i], L"--no-sound"))
		{
			DisableSound();
		}
		else if (!wcscmp(argv[i], L"--no-audio"))
		{
			DisableMusic();
			DisableVoice();
			DisableSound();
		}
	}

	if (level_set || act_set)
	{
		WriteData(reinterpret_cast<GameModes*>(0x0040C10C), GameModes_Trial);
	}

	LocalFree(argv);
}