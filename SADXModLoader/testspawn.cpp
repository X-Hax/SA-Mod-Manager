#include "stdafx.h"
#include <ShellAPI.h>
#include <string>
#include <unordered_map>

static bool testspawn_enabled = false;
static Trampoline* MovePlayerToStartPoint_t;
static StartPosition position_override = { 0, 0, { 0, 0, 0 }, 0 };

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
	int scene_select;
};

CutsceneLevelData CutsceneList[]
{
	// Sonic events
	{ 0x001, 26, 3, 0, 0  }, // Sonic Intro
	{ 0x002, 15, 0, 0, 0  }, // Sonic defeats Chaos 0
	{ 0x003, 26, 4, 0, 1  }, // Sonic sees Tails crash
	{ 0x006, 26, 4, 0, 1  }, // Sonic and Tails poolside
	{ 0x007, 33, 0, 0, 2  }, // Sonic faces off with the Egg Hornet
	{ 0x008, 33, 0, 0, 2  }, // Chaos 1 Emerges
	{ 0x009, 26, 1, 0, 0  }, // Sonic and Tails are gassed
	{ 0x00B, 33, 0, 0, 5  }, // Chaos 4 Transformation
	{ 0x00C, 33, 0, 0, 5  }, // Sonic and Tails part ways with Knuckles
	{ 0x00D, 33, 0, 0, 5  }, // Tornado 1 Lift Off Cutscene
	{ 0x011, 26, 4, 0, 6  }, // Sonic falling into Station Square
	{ 0x012, 26, 1, 0, 6  }, // Amy finds Sonic
	{ 0x013, 26, 3, 0, 7  }, // Amy and Sonic go to Twinkle Park
	{ 0x014, 26, 5, 0, 8  }, // Sonic goes looking for Amy
	{ 0x015, 26, 1, 0, 9  }, // Sonic finds Zero and Amy
	{ 0x016, 33, 0, 0, 9  }, // Zero transported to the Egg Carrier
	{ 0x017, 5,  1, 0, 9  }, // Sonic and Tails on the Tornado 2
	{ 0x01A, 29, 2, 0, 10 }, // Eggman takes Birdie's Emerald
	{ 0x01B, 29, 2, 0, 10 }, // Sonic goes to put Eggman out of commission
	{ 0x01C, 18, 0, 0, 11 }, // Sonic finds Chaos 6
	{ 0x01D, 18, 0, 0, 12 }, // Sonic chases Eggman to the Mystic Ruins
	{ 0x01E, 33, 2, 0, 12 }, // Sonic prepares to enter Lost World
	{ 0x020, 7,  2, 0, 12 }, // Sonic views the Perfect Chaos Mural
	{ 0x021, 34, 2, 0, 13 }, // Sonic enters the Past
	{ 0x022, 34, 2, 0, 13 }, // Sonic listens to Tikal in the Past
	{ 0x023, 33, 2, 0, 14 }, // Sonic sees Eggman heading to his base
	{ 0x024, 22, 0, 0, 14 }, // Sonic's Final Battle with Eggman
	{ 0x026, 33, 0, 0, 1  }, // Sonic's Outro
	{ 0x028, 33, 0, 0, 5  }, // Sonic vs. Knuckles
	{ 0x029, 29, 0, 0, 10 }, // Tornado 2 lands on the Egg Carrier
	{ 0x02A, 26, 1, 0, 4  }, // Sonic and Tails awaken after being gassed
	{ 0x02B, 15, 0, 0, 0  }, // Sonic meets Chaos 0

	// Tails events
	{ 0x030, 26, 3, 2, 0  }, // Tails Intro
	{ 0x031, 1,  1, 2, 0  }, // Tails is rescued by Sonic
	{ 0x032, 26, 4, 2, 0  }, // Tails and Sonic poolside
	{ 0x033, 33, 0, 2, 1  }, // Tails faces off with Egg Hornet
	{ 0x034, 33, 0, 2, 1  }, // Chaos 1 Emerges
	{ 0x035, 26, 1, 2, 2  }, // Tails and Sonic are gassed at Casinopolis
	{ 0x038, 33, 0, 2, 4  }, // Tails vs. Knuckles
	{ 0x039, 33, 0, 2, 4  }, // Chaos 4 Emerges
	{ 0x03A, 33, 0, 2, 4  }, // Tails and Sonic follow Eggman after Chaos 4
	{ 0x03B, 33, 0, 2, 4  }, // Tails and Sonic depart on the Tornado 1
	{ 0x03E, 33, 2, 2, 5  }, // Tails' flashback
	{ 0x040, 33, 0, 2, 6  }, // Tails wakes up from his dream
	{ 0x042, 33, 2, 2, 6  }, // Tails chases Froggy
	{ 0x044, 34, 0, 2, 6  }, // Tails enters the Past
	{ 0x045, 34, 0, 2, 6  }, // Tails talks to Tikal
	{ 0x046, 33, 0, 2, 7  }, // Tails returns and meets Big
	{ 0x047, 33, 0, 2, 7  }, // The Tornado 2 takes flight
	{ 0x048, 5,  1, 2, 7  }, // Tails finds Sonic in Red Mountain
	{ 0x04B, 29, 2, 2, 8  }, // Tails faces off with Gamma
	{ 0x04C, 29, 2, 2, 9  }, // Tails departs the Egg Carrier with Amy
	{ 0x04D, 26, 3, 2, 10 }, // Eggman launches his missile attack
	{ 0x04E, 26, 3, 2, 10 }, // Tails follows Eggman after the missile
	{ 0x050, 26, 1, 2, 11 }, // Tails takes on the Egg Walker
	{ 0x051, 26, 1, 2, 11 }, // Egg Walker defeated, Station Square saved
	{ 0x052, 33, 0, 2, 0  }, // Tails Outro
	{ 0x053, 26, 0, 2, -1 }, // Error
	{ 0x054, 29, 1, 2, 8  }, // Gonna land on the Egg Carrier
	{ 0x055, 38, 0, 2, 6  }, // Cutscene with Froggy after Sand Hill
	{ 0x056, 26, 1, 2, 3  }, // Tails and Sonic awake after being gassed

	// Amy events
	{ 0x058, 26, 0, 5, 0  }, // Amy's pre-intro ??
	{ 0x060, 26, 0, 5, 0  }, // Amy's intro
	{ 0x061, 26, 0, 5, 0  }, // Amy meets Birdie
	{ 0x062, 26, 1, 5, 1  }, // Amy meets up with Sonic
	{ 0x063, 26, 3, 5, 1  }, // Amy and Sonic visit Twinkle Park
	{ 0x064, 26, 3, 5, 2  }, // Amy's kidnapped by Zero
	{ 0x065, 32, 3, 5, 2  }, // Amy locked up, talking to Gamma
	{ 0x066, 12, 1, 5, 2  }, // Amy goes to the past after Hot Shelter
	{ 0x067, 34, 0, 5, 3  }, // Amy enters the past
	{ 0x068, 34, 1, 5, 3  }, // Amy meets Tikal
	{ 0x069, 29, 2, 5, 4  }, // Eggman takes Birdie's Emerald
	{ 0x06A, 29, 2, 5, 4  }, // Amy and Tails leave the Egg Carrier
	{ 0x06B, 26, 0, 5, -1 }, // Error
	{ 0x06C, 29, 5, 5, 3  }, // Amy returns to the present
	{ 0x06D, 26, 3, 5, 5  }, // Hunt to find Birdie's family
	{ 0x06E, 33, 2, 5, 6  }, // Amy discovers the Egg Base
	{ 0x06F, 10, 0, 5, 6  }, // Amy chased by Zero in Final Egg
	{ 0x070, 33, 3, 5, 7  }, // Amy and Birdie head back to the Egg Carrier
	{ 0x071, 23, 0, 5, 8  }, // Zero confronts Amy
	{ 0x072, 29, 0, 5, 8  }, // Amy's Outro
	{ 0x075, 26, 1, 5, 2  }, // Amy's kidnapped to the Mystic Ruins

	// Knuckles events
	{ 0x080, 33, 1, 3, 0  }, // Knuckles Intro
	{ 0x082, 26, 3, 3, 1  }, // Knuckles goes hunting for the Master Emerald
	{ 0x083, 9,  0, 3, 2  }, // Knuckles enters the Past
	{ 0x084, 34, 0, 3, 2  }, // Knuckles while in the Past
	{ 0x085, 34, 0, 3, 2  }, // Tikal's Crisis
	{ 0x086, 26, 1, 3, 2  }, // Knuckles returns from the Past
	{ 0x087, 16, 0, 3, 3  }, // Knuckles and Chaos 2 face off
	{ 0x088, 16, 0, 3, 3  }, // Eggman tricks Knuckles
	{ 0x089, 33, 0, 3, 3  }, // Knuckles goes after Sonic
	{ 0x08A, 33, 0, 3, 4  }, // Knuckles vs. Sonic
	{ 0x08B, 33, 0, 3, 4  }, // Chaos 4 emerges
	{ 0x08C, 33, 0, 3, 4  }, // Knuckles departs from Sonic and Tails
	{ 0x08D, 7,  1, 3, 5  }, // Knuckles goes to the Past (from Lost World)
	{ 0x08E, 34, 1, 3, 5  }, // Knuckles back in the Past
	{ 0x08F, 34, 1, 3, 5  }, // Tikal's crisis again
	{ 0x091, 33, 1, 3, 6  }, // Knuckles restores most of the Master Emerald
	{ 0x092, 33, 2, 3, 6  }, // Knuckles follows Gamma to Final Egg base
	{ 0x094, 29, 0, 3, 6  }, // Knuckles on the Egg Carrier
	{ 0x095, 6,  2, 3, 7  }, // Knuckles finds the last missing piece
	{ 0x096, 34, 2, 3, 8  }, // Knuckles travels back to the past one last time
	{ 0x097, 34, 2, 3, 8  }, // The aftermath of Tikal's plight
	{ 0x098, 29, 5, 3, 8  }, // Knuckles returns to the present
	{ 0x099, 18, 1, 3, 9  }, // Knuckles fights Chaos 6
	{ 0x09A, 29, 0, 3, 9  }, // Knuckles has all collected the final shards
	{ 0x09B, 18, 1, 3, 9  }, // Knuckles defeats Chaos 6
	{ 0x09C, 26, 0, 3, -1 }, // Error
	{ 0x09D, 33, 1, 3, 10 }, // Knuckles restores the Master Emerald
	{ 0x09F, 33, 1, 3, 10 }, // Knuckles Outro
	{ 0x0A0, 26, 4, 3, 3  }, // Knuckles follows Eggman in Station Square hotel

	// Gamma events
	{ 0x0B0, 33, 3, 6, 0  }, // Gamma Intro
	{ 0x0B1, 33, 3, 6, 1  }, // Gamma Enters Final Egg
	{ 0x0B2, 33, 3, 6, 1  }, // Gamma Exits Final Egg
	{ 0x0B3, 33, 3, 6, 1  }, // Useless machine
	{ 0x0B4, 33, 3, 6, 1  }, // Gamma's Fight with Beta
	{ 0x0B5, 33, 3, 6, 1  }, // Gamma defeats Beta
	{ 0x0B7, 32, 1, 6, 2  }, // The hunt for Froggy begins
	{ 0x0B8, 1,  0, 6, 2  }, // Gamma goes to the Past
	{ 0x0B9, 34, 1, 6, 3  }, // Gamma in the Past
	{ 0x0BA, 34, 1, 6, 3  }, // Gamma and Tikal meet
	{ 0x0BB, 32, 1, 6, 3  }, // Gamma returns to the Egg Carrier
	{ 0x0BC, 32, 1, 6, 4  }, // Gamma goes to the wrong room
	{ 0x0BD, 32, 1, 6, 4  }, // Beta's new body being built
	{ 0x0BE, 32, 1, 6, 4  }, // Gamma leaves Beta's room
	{ 0x0BF, 32, 3, 6, 4  }, // Gamma enters the jail, meets Amy
	{ 0x0C0, 32, 1, 6, 4  }, // Gamma heading to the rear of the ship
	{ 0x0C1, 29, 2, 6, 4  }, // Gamma emerges to fight Sonic
	{ 0x0C2, 29, 2, 6, 4  }, // Gamma after the battle with Sonic
	{ 0x0C3, 33, 0, 6, 5  }, // Gamma's objectives changed (wtf?)
	{ 0x0C5, 33, 1, 6, 7  }, // Gamma remembers his brothers
	{ 0x0C7, 25, 0, 6, 8  }, // Gamma Outro

	// Big events
	{ 0x0D0, 33, 2, 7, 0  }, // Big Intro
	{ 0x0D1, 26, 3, 7, 0  }, // Big goes searching for Froggy
	{ 0x0D2, 26, 0, 7, 1  }, // Froggy heads into the sewers
	{ 0x0D3, 33, 0, 7, 0  }, // Big finds Froggy with Tails
	{ 0x0D4, 1,  2, 7, 3  }, // Big loses Froggy to Gamma
	{ 0x0D8, 32, 1, 7, 4  }, // Big enters Hot Shelter
	{ 0x0D9, 12, 0, 7, 4  }, // Big spots Froggy inside the tanks
	{ 0x0DA, 12, 0, 7, 4  }, // Big saves Froggy - broken
	{ 0x0DB, 34, 1, 7, 5  }, // Big heads into the past
	{ 0x0DC, 34, 1, 7, 5  }, // Tikal talks to Big
	{ 0x0DD, 32, 1, 7, 5  }, // Big returns and is ready to leave the Egg Carrier
	{ 0x0DE, 18, 0, 7, 6  }, // Chaos 6 takes Froggy
	{ 0x0DF, 18, 0, 7, 6  }, // Sonic saves Froggy
	{ 0x0E0, 29, 0, 7, 6  }, // Big finds the Tornado 2 and leaves
	{ 0x0E1, 26, 0, 7, -1 }, // Error
	{ 0x0E2, 33, 2, 7, 7  }, // Big Outro
	{ 0x0E3, 26, 3, 7, 1  }, // Big sees Froggy heading to the beach

	// Last Story
	{ 0x0F0, 33, 2, 0, 0  }, // Tornado 2 Flash scene
	{ 0x0F2, 33, 2, 0, 1  }, // Eggman heading to the Mystic Ruins base
	{ 0x0F3, 33, 1, 0, 1  }, // Knuckles at the Master Emerald
	{ 0x0F4, 33, 0, 0, 1  }, // Tails runs to Sonic
	{ 0x0F5, 33, 1, 0, 1  }, // Sonic and Tails find Eggman and Knuckles
	{ 0x0F6, 34, 2, 0, 1  }, // Sonic travels to the past
	{ 0x0F7, 34, 2, 0, 1  }, // Tikal pleads with her father
	{ 0x0F8, 34, 2, 0, 1  }, // Tikal seals Chaos
	{ 0x0F9, 33, 1, 0, 1  }, // Sonic returns to the present
	{ 0x0FA, 33, 2, 0, 2  }, // Sonic and Tails find the Tornado 2
	{ 0x0FB, 34, 2, 0, 2  }, // Sonic checks on Tikal in the past
	{ 0x0FD, 19, 0, 0, 2  }, // Perfect Chaos reveals himself
	{ 0x0FE, 19, 0, 0, 2  }, // Last Story Outro
	{ 0x0FF, 19, 0, 0, 2  }, // Everyone brings Sonic the emeralds

	// Additional Sonic events
	{ 0x100, 29, 0, 0, 10 }, // Sonic and Tails after landing on the Egg Carrier
	{ 0x101, 29, 0, 0, 10 }, // Sonic and Tails Sky Chase attack (don't get too many ideas)
	{ 0x102, 29, 1, 0, 10 }, // The Egg Carrier Transforms
	{ 0x103, 29, 1, 0, 10 }, // Sonic at the Sky Deck Entrance - broken
	{ 0x104, 32, 1, 0, 10 }, // Sonic right after Sky Deck (Is that it?)
	{ 0x106, 29, 2, 0, 10 }, // Sonic heading to transform the Egg Carrier
	{ 0x107, 29, 3, 0, 10 }, // Emergency altert cancelled (Sonic) - broken

	// Additional Tails events
	{ 0x110, 29, 0, 2, 8  }, // Tails and Sonic after landing on the Egg Carrier
	{ 0x111, 29, 0, 2, 8  }, // Tails' Sky Chase Attack
	{ 0x112, 29, 1, 2, 8  }, // The Egg Carrier Transforms
	{ 0x113, 29, 1, 2, 8  }, // Tails at the Sky Deck Entrance - broken
	{ 0x114, 32, 1, 2, 8  }, // Tails right after Sky Deck

	// Additional Knuckles events
	{ 0x120, 29, 2, 3, 9  }, // Egg Carrier Transforms 1
	{ 0x121, 29, 2, 3, 9  }, // Egg Carrier Transforms 2
	{ 0x122, 29, 5, 3, 9  }, // Knuckles sensing the emeralds on the Egg Carrier - broken

	// Additional Amy events
	{ 0x130, 32, 2, 5, 2  }, // Introduction to Hedgehog Hammer
	{ 0x131, 32, 2, 5, 2  }, // Winning at Hedgehog Hammer - broken

	// Additional Gamma events
	{ 0x140, 32, 1, 6, 4  }, // Gamma is told to find the Jet Booster - broken
	{ 0x141, 29, 0, 6, 7  }, // Gamma heads to Hot Shelter
	{ 0x142, 29, 0, 6, 7  }, // Gamma rescues E-105

	// Additional Big events
	{ 0x150, 29, 3, 7, 4  }, // Egg Carrier Transforms - broken

	// Additional Last Story events
	{ 0x160, 34, 2, 0, 1  }, // The Echidna tribe faces Chaos

	// Upgrade Cutscenes
	{ 0x165, 26, 4, 0, 0  }, // Sonic gets the Crystal Ring
	{ 0x166, 26, 2, 0, 0  }, // Sonic gets the LSDash Shoe
	{ 0x167, 33, 1, 0, 0  }, // Sonic gets the Ancient Light
	{ 0x168, 26, 3, 2, 0  }, // Tails gets the Jet Anklet
	{ 0x169, 34, 0, 2, 0  }, // Tails gets the Rhythm Badge
	{ 0x16A, 33, 2, 3, 0  }, // Knuckles gets the Fighting Gloves
	{ 0x16B, 33, 0, 3, 0  }, // Knuckles gets the Shovel Claw
	{ 0x16C, 32, 2, 5, 0  }, // Amy gets the Long Hammer
	{ 0x16D, 32, 2, 5, 0  }, // Amy gets the Warrior Feather
	{ 0x16E, 32, 4, 6, 0  }, // Gamma gets the Laser Blaster
	{ 0x16F, 32, 0, 6, 0  }, // Gamma gets the Jet Booster
	{ 0x170, 33, 2, 7, 0  }, // Big gets the PowerRod
	{ 0x171, 33, 1, 7, 0  }, // Big gets the Life Belt

	// Misc Events
	{ 0x176, 26, 1, 0, 1  }, // Ice Stone appears (Sonic)
	{ 0x177, 26, 1, 2, 0  }, // Ice Stone appears (Tails)
	{ 0x178, 26, 3, 7, 0  }, // Ice Stone appears (Big)
	{ 0x179, 26, 3, 0, 1  }, // Employee Card appears
	{ 0x17A, 33, 0, 0, 1  }, // Passage to Angel Island opens (Sonic)
	{ 0x17B, 33, 0, 2, 0  }, // Passage to Angel Island opens (Tails)
	{ 0x17C, 33, 0, 6, 1  }, // Passage to Angel Island opens (Gamma)
	{ 0x180, 5,  0, 0, 9  }, // Egg Carrier in Red Mountain
};

static CutsceneLevelData* GetCutsceneData(int cutscene)
{
	for (auto &i : CutsceneList)
	{
		if (i.cutscene_id == cutscene)
		{
			return &i;
		}
	}

	return nullptr;
}

static void SetEventFlagsForCutscene(int eventID)
{
	switch (eventID)
	{
	case 29: // Sonic jumps from the Egg Carrier
		WriteData<5>((char*)0x5578DE, 0x90u); // Don't load Chaos 6
		break;
	case 32: // Sonic sees the mural
		WriteData<1>((char*)0x7B0DA0, 0xC3u); // Lost World 3 end level object
		break;
	case 9: // Sonic and Tails gassed
		SetEventFlag((EventFlags)FLAG_SONIC_SS_ENTRANCE_CASINO);
		break;
	case 41: // Sonic and Tails land on the Egg Carrier
		SetEventFlag((EventFlags)FLAG_SONIC_EC_TORNADO2_LOST);
		break;
	case 53: // Tails and Sonic gassed
		SetEventFlag((EventFlags)FLAG_MILES_SS_ENTRANCE_CASINO);
		break;
	case 64: // Tails wakes up from his flashback
		SetTimeOfDay_Evening();
		break;
	case 66: // Tails chases Froggy
		SetTimeOfDay_Evening();
		break;
	case 80: // Egg Walker
		SetTimeOfDay_Night();
		break;
	case 81: // Egg Walker defeated
		SetTimeOfDay_Night();
		break;
	case 84: // Tails and Sonic land on the Egg Carrier
		SetEventFlag((EventFlags)FLAG_MILES_EC_TORNADO2_LOST);
		break;
	case 85: // Tails saves Froggy in Sand Hill
		WriteData<1>((char*)0x598040, 0xC3u); // Osfrog
		WriteData<1>((char*)0x79E4C0, 0xC3u); // Plays level music
		WriteData<5>((char*)0x597BF3, 0x90u); // Snowboard
		break;
	case 100: // Amy kidnapped by Zero
		SetTimeOfDay_Evening();
		break;
	case 110: // Amy discovers Final Egg base
		SetEventFlag((EventFlags)FLAG_AMY_MR_APPEAR_FINALEGG); // Open Final Egg for Amy
		SetEventFlag((EventFlags)FLAG_AMY_MR_ENTRANCE_FINALEGG); // Open Final Egg for Amy
		break;
	case 114: // Amy outro
		SetEventFlag((EventFlags)FLAG_AMY_EC_SINK); // Egg Carrier sunk in Amy's outro
		break;
	case 117: // Amy taken to the Mystic Ruins by Zero
		SetTimeOfDay_Evening();
		break;
	case 134: // Knuckles returns from the Past to Station Square
		SetTimeOfDay_Night();
		SetEventFlag((EventFlags)FLAG_KNUCKLES_SS_ENTRANCE_CASINO);
		break;
	case 136: // Knuckles is tricked by Eggman
		WriteData<5>((char*)0x54A62E, 0x90u); // Don't load Chaos 2
		break;
	case 146: // Knuckles follows Gamma to Final Egg
		SetEventFlag((EventFlags)FLAG_KNUCKLES_MR_APPEAR_FINALEGG); // Open Final Egg for Knuckles
		break;
	case 149: // Knuckles finds the last missing piece in Sky Deck
		WriteData<5>((char*)0x5EF6D0, 0x90u); // Sky Deck music
		WriteData<1>((char*)0x450370, 0xC3u); // Rings
		WriteData<1>((char*)0x7A1AA0, 0xC3u); // Tikal hints
		WriteData<1>((char*)0x476440, 0xC3u); // Radar
		break;
	case 155: // Knuckles defeats Chaos 6
		WriteData<5>((char*)0x5578DE, 0x90u); // Don't load Chaos 6
		break;
	case 179: // Useless machine
		SetEventFlag((EventFlags)FLAG_E102_MR_FREEPASS); // Open Final Egg for useless machine
		SetEventFlag((EventFlags)FLAG_E102_CLEAR_BEACH); // Open Final Egg for useless machine
		SetEventFlag((EventFlags)FLAG_E102_CLEAR_FINALEGG); // Open Final Egg for useless machine
		SetEventFlag((EventFlags)FLAG_E102_MR_APPEAR_FINALEGG); // Open Final Egg for useless machine
		break;
	case 184: // Gamma goes to the Past
		WriteData<1>((char*)0x61CA90, 0xC3u); // Kill Emerald Coast music
		WriteData<1>((char*)0x4AD140, 0xC3u); // Kill Kikis
		WriteData<1>((char*)0x4FA320, 0xC3u); // Kill OFrog
		break;
	case 192: // Gamma heading to the rear of the ship
		SetEventFlag((EventFlags)FLAG_E102_EC_BOOSTER); // Cutscenes where Gamma appears with the Jet Booster
		break;
	case 197: // Gamma remembers his brothers
		SetEventFlag((EventFlags)FLAG_E102_MR_ENTRANCE_MOUNTAIN);
		break;
	case 212: // Big loses Froggy to Gamma
		WriteData<1>((char*)0x61CA90, 0xC3u); // Kill Emerald Coast music
		break;
	case 321: // Gamma heads to Hot Shelter
	case 322: // Gamma rescues E-105
		SetEventFlag((EventFlags)FLAG_E102_EC_SINK); // Egg Carrier sunk in Gamma's outro
		break;
	case 357: // Sonic gets the Crystal Ring
		SetEventFlag((EventFlags)FLAG_SONIC_SS_CRYSTALRING);
		break;
	case 358: // Sonic gets the Light Speed Shoes
		SetEventFlag((EventFlags)FLAG_SONIC_SS_LIGHTSHOOSE);
		break;
	case 359: // Sonic gets the Ancient Light
		SetEventFlag((EventFlags)FLAG_SONIC_MR_ANCIENT_LIGHT);
		break;
	case 360: // Tails gets the Jet Anklet
		SetEventFlag((EventFlags)FLAG_MILES_SS_JETANKLET);
		break;
	case 361: // Tails gets the Rhythm Badge
		SetEventFlag((EventFlags)FLAG_MILES_MR_RHYTHMBROOCH);
		break;
	case 362: // Knuckles gets the Fighting Gloves
		SetEventFlag((EventFlags)FLAG_KNUCKLES_MR_FIGHTINGGROVE);
		break;
	case 363: // Knuckles gets the Shovel Claw
		SetEventFlag((EventFlags)FLAG_KNUCKLES_MR_SHOVELCLAW);
		break;
	case 364: // Amy gets the Long Hammer
		SetEventFlag((EventFlags)FLAG_AMY_MR_FIGHTERSFEATHER);
		SetEventFlag((EventFlags)FLAG_AMY_EC_LONGHAMMER);
		break;
	case 365: // Amy gets the Warrior Feather
		SetEventFlag((EventFlags)FLAG_AMY_MR_FIGHTERSFEATHER);
		break;
	case 366: // Gamma gets the Laser Blaster
		SetEventFlag((EventFlags)FLAG_E102_EC_TYPE3LASER);
		position_override.Position.x = 30;
		break;
	case 367: // Gamma gets the Jet Booster
		SetEventFlag((EventFlags)FLAG_E102_EC_BOOSTER);
		break;
	case 368: // Big gets the Power Rod
		SetEventFlag((EventFlags)FLAG_BIG_MR_POWERROD);
		break;
	case 369: // Big gets the Life Belt
		SetEventFlag((EventFlags)FLAG_BIG_MR_LIFEBELT);
		break;
	case 374: // Ice Stone appears (Sonic)
		SetEventFlag((EventFlags)FLAG_SONIC_SS_ICESTONE);
		break;
	case 375: // Ice Stone appears (Tails)
		SetEventFlag((EventFlags)FLAG_MILES_SS_ICESTONE);
		break;
	case 376: // Ice Stone appears (Big)
		SetEventFlag((EventFlags)FLAG_BIG_SS_ICESTONE);
		break;
	case 377: // Employee Card appears
		SetEventFlag((EventFlags)FLAG_SONIC_SS_CARD);
		break;
	case 378: // Passage to Angel Island opens (Sonic)
		SetEventFlag((EventFlags)FLAG_SONIC_MR_WESTROCK);
		break;
	case 379: // Passage to Angel Island opens (Tails)
		SetEventFlag((EventFlags)FLAG_MILES_MR_WESTROCK);
		break;
	case 380: // Passage to Angel Island opens (Gamma)
		SetEventFlag((EventFlags)FLAG_E102_MR_WESTROCK);
		break;
	}
}

static void __cdecl ForceEventMode()
{
	CutsceneLevelData* data = GetCutsceneData(CurrentDemoCutsceneID);

	if (data != nullptr)
	{
		SetLevelAndAct(data->level, data->act);
		SetupCharacter(data->character);
		InitFlagsAndThings();
		slSeqRunning = 1;
		// Last Story check
		if ((CurrentDemoCutsceneID >= 240 && CurrentDemoCutsceneID <= 255) || CurrentDemoCutsceneID == 355)
		{
			LastStoryFlag = 1;
			SetEventFlag((EventFlags)FLAG_PLAYING_SUPERSONIC);
			ssSceneNo = 4;
			pCurSequence = &seqTable[4];
			pCurSectionList = &SeqGetSectionList(4)[data->scene_select];
		}
		else
		{
			ssSceneNo = data->character;
			pCurSequence = &seqTable[data->character];
			pCurSectionList = &SeqGetSectionList(data->character)[data->scene_select];
		}
		SetEventFlagsForCutscene(CurrentDemoCutsceneID);
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

static void __cdecl MovePlayerToStartPoint_r(EntityData1* data1)
{
	auto original = static_cast<decltype(MovePlayerToStartPoint_r)*>(MovePlayerToStartPoint_t->Target());
	original(data1);
	if (position_override.Position.x != 0)
		data1->Position.x = position_override.Position.x;
	if (position_override.Position.y != 0)
	data1->Position.y = position_override.Position.y;
	if (position_override.Position.z != 0)
	data1->Position.z = position_override.Position.z;
	if (position_override.YRot != 0)
	data1->Rotation.y = position_override.YRot;
}

void ProcessTestSpawn(const HelperFunctions& helperFunctions)
{
	if (GetModuleHandle(L"sadx-test-spawn") != NULL)
	{
		return;
	}

	int argc = 0;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	for (int i = 1; i < argc; i++)
	{
		if (!wcscmp(argv[i], L"--level") || !wcscmp(argv[i], L"-l"))
		{
			CurrentLevel = parse_level_id(argv[++i]);
			PrintDebug("Loading level: %d\n", CurrentLevel);
			testspawn_enabled = true;
		}
		else if (!wcscmp(argv[i], L"--act") || !wcscmp(argv[i], L"-a"))
		{
			CurrentAct = _wtoi(argv[++i]);
			PrintDebug("Loading act: %d\n", CurrentAct);
			testspawn_enabled = true;
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
			if (testspawn_enabled == false)
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

			position_override = {
				CurrentLevel,
				static_cast<int16_t>(CurrentAct),
				{ x, y, z },	// Position
				0				// YRot
			};

			// Hook the start position setting
			switch (CurrentLevel)
			{
			case LevelIDs_StationSquare:
			case LevelIDs_EggCarrierOutside:
			case LevelIDs_EggCarrierInside:
			case LevelIDs_MysticRuins:
			case LevelIDs_Past:
				MovePlayerToStartPoint_t = new Trampoline(0x414810, 0x414815, MovePlayerToStartPoint_r);
				break;
			default:
				helperFunctions.RegisterStartPosition(static_cast<Uint8>(CurrentCharacter), position_override);
				break;
			}
		}
		else if (!wcscmp(argv[i], L"--event") || !wcscmp(argv[i], L"-e"))
		{
			CurrentDemoCutsceneID = _wtoi(argv[++i]);
			PrintDebug("Loading event: %d\n", CurrentDemoCutsceneID);

			// Disable "Press Start" during the cutscene.
			WriteData<1>((char*)0x457D10, 0xC3u);

			// Hook the copyright GameMode and launch the event.
			WriteJump(reinterpret_cast<void*>(0x0040C106), ForceEventMode_asm);

			// Hook the start position setting
			MovePlayerToStartPoint_t = new Trampoline(0x414810, 0x414815, MovePlayerToStartPoint_r);
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

	LocalFree(argv);
}

void ApplyTestSpawn()
{
	if (testspawn_enabled == true)
	{
		WriteData(reinterpret_cast<GameModes*>(0x0040C10C), GameModes_Trial);
	}
}