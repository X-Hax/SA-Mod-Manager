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

struct CutsceneLevelData
{
	int cutscene_id;
	int level;
	int act;
	int character;
};

CutsceneLevelData CutsceneList[]
{
	// Sonic events
	{ 0x001, 26, 3, 0 }, // Sonic Intro
	{ 0x002, 15, 0, 0 }, // Sonic defeats Chaos 0
	{ 0x003, 26, 4, 0 }, // Sonic sees Tails crash
	{ 0x006, 26, 4, 0 }, // Sonic and Tails poolside
	{ 0x007, 33, 0, 0 }, // Sonic faces off with the Egg Hornet
	{ 0x008, 33, 0, 0 }, // Chaos 1 Emerges
	{ 0x009, 26, 1, 0 }, // Sonic and Tails are gassed
	{ 0x00B, 33, 0, 0 }, // Chaos 4 Transformation
	{ 0x00C, 33, 0, 0 }, // Sonic and Tails part ways with Knuckles
	{ 0x00D, 33, 0, 0 }, // Tornado 1 Lift Off Cutscene
	{ 0x011, 26, 4, 0 }, // Sonic falling into Station Square
	{ 0x012, 26, 1, 0 }, // Amy finds Sonic
	{ 0x013, 26, 3, 0 }, // Amy and Sonic go to Twinkle Park
	{ 0x014, 26, 5, 0 }, // Sonic goes looking for Amy
	{ 0x015, 26, 1, 0 }, // Sonic finds Zero and Amy
	{ 0x016, 33, 0, 0 }, // Zero transported to the Egg Carrier
	{ 0x017, 5,  1, 0 }, // Sonic and Tails on the Tornado 2
	{ 0x01A, 29, 2, 0 }, // Eggman takes Birdie's Emerald
	{ 0x01B, 29, 2, 0 }, // Sonic goes to put Eggman out of commission
	{ 0x01C, 18, 0, 0 }, // Sonic finds Chaos 6
	{ 0x01D, 18, 0, 0 }, // Sonic chases Eggman to the Mystic Ruins
	{ 0x01E, 33, 2, 0 }, // Sonic prepares to enter Lost World
	{ 0x020, 7,  2, 0 }, // Sonic views the Perfect Chaos Mural
	{ 0x021, 34, 2, 0 }, // Sonic enters the Past
	{ 0x022, 34, 2, 0 }, // Sonic listens to Tikal in the Past
	{ 0x023, 33, 2, 0 }, // Sonic sees Eggman heading to his base
	{ 0x024, 22, 0, 0 }, // Sonic's Final Battle with Eggman
	{ 0x026, 33, 0, 0 }, // Sonic's Outro
	{ 0x028, 33, 0, 0 }, // Sonic vs. Knuckles
	{ 0x029, 29, 0, 0 }, // Tornado 2 lands on the Egg Carrier
	{ 0x02A, 26, 1, 0 }, // Sonic and Tails awaken after being gassed
	{ 0x02B, 15, 0, 0 }, // Sonic meets Chaos 0

	// Tails events
	{ 0x030, 26, 3, 2 }, // Tails Intro
	{ 0x031, 1,  1, 2 }, // Tails is rescued by Sonic
	{ 0x032, 26, 4, 2 }, // Tails and Sonic poolside
	{ 0x033, 33, 0, 2 }, // Tails faces off with Egg Hornet
	{ 0x034, 33, 0, 2 }, // Chaos 1 Emerges
	{ 0x035, 26, 1, 2 }, // Tails and Sonic are gassed at Casinopolis - broken
	{ 0x038, 33, 0, 2 }, // Tails vs. Knuckles
	{ 0x039, 33, 0, 2 }, // Chaos 4 Emerges
	{ 0x03A, 33, 0, 2 }, // Tails and Sonic follow Eggman after Chaos 4
	{ 0x03B, 33, 0, 2 }, // Tails and Sonic depart on the Tornado 1
	{ 0x03E, 33, 2, 2 }, // Tails' flashback
	{ 0x040, 33, 0, 2 }, // Tails wakes up from his dream
	{ 0x042, 33, 2, 2 }, // Tails chases Froggy
	{ 0x044, 34, 0, 2 }, // Tails enters the Past
	{ 0x045, 34, 0, 2 }, // Tails talks to Tikal
	{ 0x046, 33, 0, 2 }, // Tails returns and meets Big
	{ 0x047, 33, 0, 2 }, // The Tornado 2 takes flight
	{ 0x048, 5,  1, 2 }, // Tails finds Sonic in Red Mountain
	{ 0x04B, 29, 2, 2 }, // Tails faces off with Gamma
	{ 0x04C, 29, 2, 2 }, // Tails departs the Egg Carrier with Amy
	{ 0x04D, 26, 3, 2 }, // Eggman launches his missile attack
	{ 0x04E, 26, 3, 2 }, // Tails follows Eggman after the missile
	{ 0x050, 26, 1, 2 }, // Tails takes on the Egg Walker
	{ 0x051, 26, 1, 2 }, // Egg Walker defeated, Station Square saved
	{ 0x052, 33, 0, 2 }, // Tails Outro
	{ 0x053, 26, 0, 2 }, // Error
	{ 0x054, 29, 1, 2 }, // Gonna land on the Egg Carrier
	{ 0x055, 38, 0, 2 }, // Cutscene with Froggy after Sand Hill
	{ 0x056, 26, 1, 2 }, // Tails and Sonic awake after being gassed

	// Amy events
	{ 0x058, 26, 0, 5 }, // Amy's pre-intro ??
	{ 0x060, 26, 0, 5 }, // Amy's intro
	{ 0x061, 26, 0, 5 }, // Amy meets Birdie
	{ 0x062, 26, 1, 5 }, // Amy meets up with Sonic
	{ 0x063, 26, 3, 5 }, // Amy and Sonic visit Twinkle Park
	{ 0x064, 26, 3, 5 }, // Amy's kidnapped by Zero
	{ 0x065, 32, 3, 5 }, // Amy locked up, talking to Gamma
	{ 0x066, 12, 1, 5 }, // Amy goes to the past after Hot Shelter
	{ 0x067, 34, 0, 5 }, // Amy enters the past
	{ 0x068, 34, 1, 5 }, // Amy meets Tikal
	{ 0x069, 29, 2, 5 }, // Eggman takes Birdie's Emerald
	{ 0x06A, 29, 2, 5 }, // Amy and Tails leave the Egg Carrier
	{ 0x06B, 26, 0, 5 }, // Error
	{ 0x06C, 29, 5, 5 }, // Amy returns to the present
	{ 0x06D, 26, 3, 5 }, // Hunt to find Birdie's family
	{ 0x06E, 33, 2, 5 }, // Amy discovers the Egg Base
	{ 0x06F, 10, 0, 5 }, // Amy chased by Zero in Final Egg
	{ 0x070, 33, 3, 5 }, // Amy and Birdie head back to the Egg Carrier
	{ 0x071, 23, 0, 5 }, // Zero confronts Amy
	{ 0x072, 29, 0, 5 }, // Amy's Outro
	{ 0x075, 26, 1, 5 }, // Amy's kidnapped to the Mystic Ruins

	// Knuckles events
	{ 0x080, 33, 1, 3 }, // Knuckles Intro
	{ 0x082, 26, 3, 3 }, // Knuckles goes hunting for the Master Emerald
	{ 0x083, 9,  0, 3 }, // Knuckles enters the Past
	{ 0x084, 34, 0, 3 }, // Knuckles while in the Past
	{ 0x085, 34, 0, 3 }, // Tikal's Crisis
	{ 0x086, 26, 1, 3 }, // Knuckles returns from the Past
	{ 0x087, 26, 4, 3 }, // Knuckles and Chaos 2 face off
	{ 0x088, 16, 0, 3 }, // Eggman tricks Knuckles
	{ 0x089, 33, 0, 3 }, // Knuckles goes after Sonic
	{ 0x08A, 33, 0, 3 }, // Knuckles vs. Sonic
	{ 0x08B, 33, 0, 3 }, // Chaos 4 emerges
	{ 0x08C, 33, 0, 3 }, // Knuckles departs from Sonic and Tails
	{ 0x08D, 7,  1, 3 }, // Knuckles goes to the Past (from Lost World)
	{ 0x08E, 34, 1, 3 }, // Knuckles back in the Past
	{ 0x08F, 34, 1, 3 }, // Tikal's crisis again
	{ 0x091, 33, 1, 3 }, // Knuckles restores most of the Master Emerald
	{ 0x092, 33, 2, 3 }, // Knuckles follows Gamma to Final Egg base
	{ 0x094, 29, 0, 3 }, // Knuckles on the Egg Carrier
	{ 0x095, 6,  2, 3 }, // Knuckles finds the last missing piece
	{ 0x096, 34, 2, 3 }, // Knuckles travels back to the past one last time
	{ 0x097, 34, 2, 3 }, // The aftermath of Tikal's plight
	{ 0x098, 29, 5, 3 }, // Knuckles returns to the present
	{ 0x099, 18, 1, 3 }, // Knuckles fights Chaos 6
	{ 0x09A, 29, 0, 3 }, // Knuckles has all collected the final shards
	{ 0x09B, 18, 1, 3 }, // Knuckles defeats Chaos 6
	{ 0x09C, 26, 0, 3 }, // Error
	{ 0x09D, 33, 1, 3 }, // Knuckles restores the Master Emerald
	{ 0x09F, 33, 1, 3 }, // Knuckles Outro
	{ 0x0A0, 26, 4, 3 }, // Knuckles follows Eggman in Station Square hotel

	// Gamma events
	{ 0x0B0, 33, 3, 6 }, // Gamma Intro
	{ 0x0B1, 33, 3, 6 }, // Gamma Enters Final Egg
	{ 0x0B2, 33, 3, 6 }, // Gamma Exits Final Egg
	{ 0x0B3, 33, 3, 6 }, // Useless machine
	{ 0x0B4, 33, 3, 6 }, // Gamma's Fight with Beta
	{ 0x0B5, 33, 3, 6 }, // Gamma defeats Beta
	{ 0x0B7, 32, 1, 6 }, // The hunt for Froggy begins
	{ 0x0B8, 1,  0, 6 }, // Gamma goes to the Past
	{ 0x0B9, 34, 1, 6 }, // Gamma in the Past
	{ 0x0BA, 34, 1, 6 }, // Gamma and Tikal meet
	{ 0x0BB, 32, 1, 6 }, // Gamma returns to the Egg Carrier
	{ 0x0BC, 32, 1, 6 }, // Gamma goes to the wrong room
	{ 0x0BD, 32, 1, 6 }, // Beta's new body being built
	{ 0x0BE, 32, 1, 6 }, // Gamma leaves Beta's room
	{ 0x0BF, 32, 3, 6 }, // Gamma enters the jail, meets Amy
	{ 0x0C0, 32, 1, 6 }, // Gamma heading to the rear of the ship
	{ 0x0C1, 29, 2, 6 }, // Gamma emerges to fight Sonic
	{ 0x0C2, 29, 2, 6 }, // Gamma after the battle with Sonic
	{ 0x0C3, 33, 0, 6 }, // Gamma's objectives changed
	{ 0x0C5, 33, 1, 6 }, // Gamma remembers his brothers
	{ 0x0C7, 25, 0, 6 }, // Gamma Outro

	// Big events
	{ 0x0D0, 33, 2, 7 }, // Big Intro
	{ 0x0D1, 26, 3, 7 }, // Big goes searching for Froggy
	{ 0x0D2, 26, 0, 7 }, // Froggy heads into the sewers
	{ 0x0D3, 33, 0, 7 }, // Big finds Froggy with Tails
	{ 0x0D4, 1,  2, 7 }, // Big loses Froggy to Gamma
	{ 0x0D8, 32, 1, 7 }, // Big enters Hot Shelter
	{ 0x0D9, 12, 0, 7 }, // Big spots Froggy inside the tanks
	{ 0x0DA, 12, 0, 7 }, // Big saves Froggy - brokent
	{ 0x0DB, 34, 1, 7 }, // Big heads into the past
	{ 0x0DC, 34, 1, 7 }, // Tikal talks to Big
	{ 0x0DD, 32, 1, 7 }, // Big returns and is ready to leave the Egg Carrier
	{ 0x0DE, 18, 0, 7 }, // Chaos 6 takes Froggy
	{ 0x0DF, 18, 0, 7 }, // Sonic saves Froggy
	{ 0x0E0, 29, 0, 7 }, // Big finds the Tornado 2 and leaves
	{ 0x0E1, 26, 0, 7 }, // Error
	{ 0x0E2, 33, 2, 7 }, // Big Outro
	{ 0x0E3, 26, 3, 7 }, // Big sees Froggy heading to the beach

	// Last Story
	{ 0x0F0, 33, 2, 0 }, // Tornado 2 Flash scene
	{ 0x0F2, 33, 2, 0 }, // Eggman heading to the Mystic Ruins base
	{ 0x0F3, 33, 1, 0 }, // Knuckles at the Master Emerald
	{ 0x0F4, 33, 0, 0 }, // Tails runs to Sonic
	{ 0x0F5, 33, 1, 0 }, // Sonic and Tails find Eggman and Knuckles
	{ 0x0F6, 34, 2, 0 }, // Sonic travels to the past
	{ 0x0F7, 34, 2, 0 }, // Tikal pleads with her father
	{ 0x0F8, 34, 2, 0 }, // Tikal seals Chaos
	{ 0x0F9, 33, 1, 0 }, // Sonic returns to the present
	{ 0x0FA, 33, 2, 0 }, // Sonic and Tails find the Tornado 2
	{ 0x0FB, 34, 2, 0 }, // Sonic checks on Tikal in the past
	{ 0x0FD, 19, 0, 0 }, // Perfect Chaos reveals himself
	{ 0x0FE, 19, 0, 0 }, // Last Story Outro
	{ 0x0FF, 19, 0, 0 }, // Everyone brings Sonic the emeralds

	// Additional  Sonic events
	{ 0x100, 29, 0, 0 }, // Sonic and Tails after landing on the Egg Carrier
	{ 0x101, 29, 0, 0 }, // Sonic and Tails Sky Chase attack (don't get too many ideas)
	{ 0x102, 29, 1, 0 }, // The Egg Carrier Transforms
	{ 0x103, 29, 1, 0 }, // Sonic at the Sky Deck Entrance - broken
	{ 0x104, 32, 1, 0 }, // Sonic right after Sky Deck (Is that it?)
	{ 0x106, 29, 2, 0 }, // Sonic heading to transform the Egg Carrier
	{ 0x107, 29, 3, 3 }, // Egg Carrier returns to normal - broken

	// Additional Tails events
	{ 0x110, 29, 0, 2 }, // Tails and Sonic after landing on the Egg Carrier
	{ 0x111, 29, 0, 2 }, // Tails' Sky Chase Attack
	{ 0x112, 29, 1, 2 }, // The Egg Carrier Transforms
	{ 0x113, 29, 1, 2 }, // Tails at the Sky Deck Entrance - broken
	{ 0x114, 32, 1, 2 }, // Tails right after Sky Deck

	// Additional Knuckles events
	{ 0x120, 29, 2, 3 }, // Egg Carrier Transforms 1
	{ 0x121, 29, 2, 3 }, // Egg Carrier Transforms 2
	{ 0x122, 29, 5, 3 }, // Knuckles sensing the emeralds on the Egg Carrier - broken

	// Additional Amy events
	{ 0x130, 32, 2, 5 }, // Introduction to Hedgehog Hammer
	{ 0x131, 32, 2, 5 }, // Winning at Hedgehog Hammer - broken

	// Additional Gamma events
	{ 0x140, 32, 1, 6 }, // Gamma is told to find the Jet Booster - broken
	{ 0x141, 29, 0, 6 }, // Gamma heads to Hot Shelter
	{ 0x142, 29, 0, 6 }, // Gamma rescues E-105

	// Additional Big events
	{ 0x150, 29, 3, 7 }, // Egg Carrier Transforms - broken

	// Additional Last Story events
	{ 0x160, 34, 2, 0 }, // The Echidna tribe faces Chaos

	// Upgrade Cutscenes
	{ 0x165, 26, 4, 0 }, // Sonic gets the Crystal Ring
	{ 0x166, 26, 2, 0 }, // Sonic gets the LSDash Shoe
	{ 0x167, 33, 1, 0 }, // Sonic gets the Ancient Light
	{ 0x168, 26, 3, 2 }, // Tails gets the Jet Anklet
	{ 0x169, 34, 0, 2 }, // Tails gets the Rhythm Badge
	{ 0x16A, 33, 2, 3 }, // Knuckles gets the Fighting Gloves
	{ 0x16B, 33, 0, 3 }, // Knuckles gets the Shovel Claw
	{ 0x16C, 32, 2, 5 }, // Amy gets the Long Hammer
	{ 0x16D, 32, 2, 5 }, // Amy gets the Warrior Feather
	{ 0x16E, 32, 4, 6 }, // Gamma gets the Laser Blaster
	{ 0x16F, 32, 0, 6 }, // Gamma gets the Jet Booster
	{ 0x170, 33, 2, 7 }, // Big gets the PowerRod
	{ 0x171, 33, 1, 7 }, // Big gets the Life Belt

	// Misc Events
	{ 0x176, 26, 1, 0 }, // Ice Stone appears (Sonic)
	{ 0x177, 26, 1, 2 }, // Ice Stone appears (Tails)
	{ 0x178, 26, 3, 7 }, // Ice Stone appears (Big)
	{ 0x179, 26, 3, 0 }, // Employee Card appears
	{ 0x17A, 33, 0, 0 }, // Passage to Angel Island opens (Sonic)
	{ 0x17B, 33, 0, 2 }, // Passage to Angel Island opens (Tails)
	{ 0x17C, 33, 0, 7 }, // Passage to Angel Island opens (Big)
	{ 0x180, 5,  0, 0 }, // Egg Carrier in Red Mountain
};

static CutsceneLevelData* GetCutsceneData(int cutscene)
{
	for (auto i : CutsceneList)
	{
		if (i.cutscene_id == cutscene)
		{
			return &i;
		}
	}

	return nullptr;
}

static void __cdecl ForceEventMode()
{
	CutsceneLevelData* data = GetCutsceneData(CurrentDemoCutsceneID);

	if (data != nullptr)
	{	
		SetLevelAndAct(data->level, data->act);
		SetupCharacter(data->character);
	}

	DemoPlaying = 1;
	GameMode = static_cast<GameModes>(5 - (GetLevelType() != 1)); // Sends to Adventure or Level GameMode
}

static const auto loc_40C95F = reinterpret_cast<const void*>(0x0040C95F);

__declspec(naked) void ForceEventMode_asm()
{
	__asm
	{
		call ForceEventMode
		jmp loc_40C95F
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
		else if (!wcscmp(argv[i], L"--event") || !wcscmp(argv[i], L"-e"))
		{
			CurrentDemoCutsceneID = _wtoi(argv[++i]);
			PrintDebug("Loading event: %d\n", CurrentDemoCutsceneID);

			// Disable "Press Start" during the cutscene.
			WriteData<1>((char*)0x457D10, 0xC3u);

			// Hook the copyright GameMode and launch the event.
			WriteJump(reinterpret_cast<void*>(0x0040C106), ForceEventMode_asm);
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