#include "stdafx.h"
#include <ShellAPI.h>
#include <string>
#include <unordered_map>

static bool testspawn_enabled		= false;
static bool testspawn_eventenabled	= false;

static int testspawn_eventid;
static int testspawn_timeofday;

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

static int8_t ForceTimeOfDay()
{
	return testspawn_timeofday;
}

static const std::unordered_map<std::wstring, int> time_ids_map = {
	{ L"day",		TimesOfDay_Day },
	{ L"evening",   TimesOfDay_Evening },
	{ L"night",		TimesOfDay_Night },
};

static int parse_time_id(const std::wstring& str)
{
	std::wstring lowercase = str;
	std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), ::towlower);

	const auto it = time_ids_map.find(lowercase);

	if (it != time_ids_map.end())
		return it->second;

	return std::stol(lowercase);
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
	int scene_select; // if -1, load standalone event
	int seqno; // subscene of a scene
};

CutsceneLevelData CutsceneList[] {
	// Sonic events
	{ 0x001, 26, 3, 0, 0, 0 }, // Sonic Intro
	{ 0x002, 15, 0, 0, 0, 1 }, // Sonic defeats Chaos 0
	{ 0x003, 26, 4, 0, 1, 1 }, // Sonic sees Tails crash
	{ 0x006, 26, 4, 0, 1, 3 }, // Sonic and Tails poolside
	{ 0x007, 33, 0, 0, 2, 1 }, // Sonic faces off with the Egg Hornet
	{ 0x008, 33, 0, 0, 2, 3 }, // Chaos 1 Emerges
	{ 0x009, 26, 1, 0, 0, 6 }, // Sonic and Tails are gassed
	{ 0x00B, 33, 0, 0, 5, 5 }, // Chaos 4 Transformation
	{ 0x00C, 33, 0, 0, 5, 8 }, // Sonic and Tails part ways with Knuckles
	{ 0x00D, 33, 0, 0, 5, 10 }, // Tornado 1 Lift Off Cutscene
	{ 0x011, 26, 4, 0, 6, 0 }, // Sonic falling into Station Square
	{ 0x012, 26, 1, 0, 6, 2 }, // Amy finds Sonic
	{ 0x013, 26, 3, 0, 6, 3 }, // Amy and Sonic go to Twinkle Park
	{ 0x014, 26, 5, 0, 7, 0 }, // Sonic goes looking for Amy
	{ 0x015, 26, 1, 0, 8, 0 }, // Sonic finds Zero and Amy
	{ 0x016, 33, 0, 0, 9, 2 }, // Zero transported to the Egg Carrier
	{ 0x017, 5,  1, 0, 9, 3  }, // Sonic and Tails on the Tornado 2
	{ 0x01A, 29, 2, 0, 11, 2 }, // Eggman takes Birdie's Emerald
	{ 0x01B, 29, 2, 0, 11, 5 }, // Sonic goes to put Eggman out of commission
	{ 0x01C, 18, 0, 0, 11, 10 }, // Sonic finds Chaos 6
	{ 0x01D, 18, 0, 0, 11, 11 }, // Sonic chases Eggman to the Mystic Ruins
	{ 0x01E, 33, 2, 0, 12, 1 }, // Sonic prepares to enter Lost World
	{ 0x020, 7,  2, 0, 12, 3 }, // Sonic views the Perfect Chaos Mural
	{ 0x021, 34, 2, 0, 13, 0 }, // Sonic enters the Past
	{ 0x022, 34, 2, 0, 13, 2 }, // Sonic listens to Tikal in the Past
	{ 0x023, 33, 2, 0, 14, 1 }, // Sonic sees Eggman heading to his base
	{ 0x024, 22, 0, 0, 14, 3 }, // Sonic's Final Battle with Eggman
	{ 0x026, 33, 0, 0, 15, 0 }, // Sonic's Outro
	{ 0x028, 33, 0, 0, 5, 2 }, // Sonic vs. Knuckles
	{ 0x029, 29, 0, 0, 10, 0 }, // Tornado 2 lands on the Egg Carrier
	{ 0x02A, 26, 1, 0, 4, 0 }, // Sonic and Tails awaken after being gassed
	{ 0x02B, 15, 0, 0, 0, 1 }, // Sonic meets Chaos 0

	// Tails events
	{ 0x030, 26, 3, 2, 0, 0 }, // Tails Intro
	{ 0x031, 1,  1, 2, 0, 1 }, // Tails is rescued by Sonic
	{ 0x032, 26, 4, 2, 0, 2 }, // Tails and Sonic poolside
	{ 0x033, 33, 0, 2, 1, 1 }, // Tails faces off with Egg Hornet
	{ 0x034, 33, 0, 2, 1, 3 }, // Chaos 1 Emerges
	{ 0x035, 26, 1, 2, 2, 3 }, // Tails and Sonic are gassed at Casinopolis
	{ 0x038, 33, 0, 2, 4, 2 }, // Tails vs. Knuckles
	{ 0x039, 33, 0, 2, 4, 5 }, // Chaos 4 Emerges
	{ 0x03A, 33, 0, 2, 4, 8 }, // Tails and Sonic follow Eggman after Chaos 4
	{ 0x03B, 33, 0, 2, 4, 10 }, // Tails and Sonic depart on the Tornado 1
	{ 0x03E, 33, 2, 2, 5, 1 }, // Tails' flashback
	{ 0x040, 33, 0, 2, 5, 3 }, // Tails wakes up from his dream
	{ 0x042, 33, 2, 2, 5, 4 }, // Tails chases Froggy
	{ 0x044, 34, 0, 2, 6, 0 }, // Tails enters the Past
	{ 0x045, 34, 0, 2, 6, 3 }, // Tails talks to Tikal
	{ 0x046, 33, 0, 2, 7, 0 }, // Tails returns and meets Big
	{ 0x047, 33, 0, 2, 7, 2 }, // The Tornado 2 takes flight
	{ 0x048, 5,  1, 2, 7, 3 }, // Tails finds Sonic in Red Mountain
	{ 0x04B, 29, 2, 2, 9, 2 }, // Tails faces off with Gamma
	{ 0x04C, 29, 2, 2, 9, 5 }, // Tails departs the Egg Carrier with Amy
	{ 0x04D, 26, 3, 2, 10, 0 }, // Eggman launches his missile attack
	{ 0x04E, 26, 3, 2, 10, 2 }, // Tails follows Eggman after the missile
	{ 0x050, 26, 1, 2, 10, 4 }, // Tails takes on the Egg Walker
	{ 0x051, 26, 1, 2, 10, 6 }, // Egg Walker defeated, Station Square saved
	{ 0x052, 33, 0, 2, 11, 0 }, // Tails Outro
	{ 0x053, 26, 0, 2, -1 }, // Error
	{ 0x054, 29, 1, 2, 8, 0 }, // Gonna land on the Egg Carrier
	{ 0x055, 38, 0, 2, 5, 5 }, // Cutscene with Froggy after Sand Hill
	{ 0x056, 26, 1, 2, 3, 0 }, // Tails and Sonic awake after being gassed

	// Amy events
	{ 0x058, 26, 0, 5, 0, 1 }, // Amy's pre-intro ??
	{ 0x060, 26, 0, 5, 0, 0 }, // Amy's intro
	{ 0x061, 26, 0, 5, 1, 2 }, // Amy meets Birdie
	{ 0x062, 26, 1, 5, 1, 4 }, // Amy meets up with Sonic
	{ 0x063, 26, 3, 5, 1, 5 }, // Amy and Sonic visit Twinkle Park
	{ 0x064, 26, 3, 5, 1, 8 }, // Amy's kidnapped by Zero
	{ 0x065, 32, 3, 5, 2, 1 }, // Amy locked up, talking to Gamma
	{ 0x066, 12, 1, 5, 2, 6 }, // Amy goes to the past after Hot Shelter
	{ 0x067, 34, 0, 5, 3, 0 }, // Amy enters the past
	{ 0x068, 34, 1, 5, 3, 3 }, // Amy meets Tikal
	{ 0x069, 29, 2, 5, 4, 2 }, // Eggman takes Birdie's Emerald
	{ 0x06A, 29, 2, 5, 4, 3 }, // Amy and Tails leave the Egg Carrier
	{ 0x06B, 26, 0, 5, -1 }, // Error
	{ 0x06C, 29, 5, 5, 4, 1 }, // Amy returns to the present
	{ 0x06D, 26, 3, 5, 5, 0 }, // Hunt to find Birdie's family
	{ 0x06E, 33, 2, 5, 6, 1 }, // Amy discovers the Egg Base
	{ 0x06F, 10, 0, 5, 6, 3 }, // Amy chased by Zero in Final Egg
	{ 0x070, 33, 3, 5, 7, 0 }, // Amy and Birdie head back to the Egg Carrier
	{ 0x071, 23, 0, 5, 8, 1 }, // Zero confronts Amy
	{ 0x072, 29, 0, 5, 8, 4 }, // Amy's Outro
	{ 0x075, 26, 1, 5, 1, 10 }, // Amy's kidnapped to the Mystic Ruins

	// Knuckles events
	{ 0x080, 33, 1, 3, 0, 0 }, // Knuckles Intro
	{ 0x082, 26, 3, 3, 1, 0 }, // Knuckles goes hunting for the Master Emerald
	{ 0x083, 9,  0, 3, 1, 4 }, // Knuckles enters the Past
	{ 0x084, 34, 0, 3, 2, 0 }, // Knuckles while in the Past
	{ 0x085, 34, 0, 3, 2, 2}, // Tikal's Crisis
	{ 0x086, 26, 1, 3, 3, 0 }, // Knuckles returns from the Past
	{ 0x087, 16, 0, 3, 3, 3 }, // Knuckles and Chaos 2 face off
	{ 0x088, 16, 0, 3, 3, 3 }, // Eggman tricks Knuckles
	{ 0x089, 33, 0, 3, 4, 1 }, // Knuckles goes after Sonic
	{ 0x08A, 33, 0, 3, 4, 6 }, // Knuckles vs. Sonic
	{ 0x08B, 33, 0, 3, 4, 9 }, // Chaos 4 emerges
	{ 0x08C, 33, 0, 3, 4, 12 }, // Knuckles departs from Sonic and Tails
	{ 0x08D, 7, 1, 3, 4, 14 }, // Knuckles goes to the Past (from Lost World)
	{ 0x08E, 34, 1, 3, 5, 0 }, // Knuckles back in the Past
	{ 0x08F, 34, 1, 3, 5, 2 }, // Tikal's crisis again
	{ 0x091, 33, 1, 3, 6, 1 }, // Knuckles restores most of the Master Emerald
	{ 0x092, 33, 2, 3, 6, 4 }, // Knuckles follows Gamma to Final Egg base
	{ 0x094, 29, 0, 3, 7, 0 }, // Knuckles on the Egg Carrier
	{ 0x095, 6, 2, 3, 7, 7 }, // Knuckles finds the last missing piece
	{ 0x096, 34, 2, 3, 8, 0 }, // Knuckles travels back to the past one last time
	{ 0x097, 34, 2, 3, 8, 2 }, // The aftermath of Tikal's plight
	{ 0x098, 29, 5, 3, 9, 0 }, // Knuckles returns to the present
	{ 0x099, 18, 1, 3, 9, 4 }, // Knuckles fights Chaos 6
	{ 0x09A, 29, 0, 3, 9, 2 }, // Knuckles has all collected the final shards
	{ 0x09B, 18, 1, 3, 9, 5 }, // Knuckles defeats Chaos 6
	{ 0x09C, 26, 0, 3, -1 }, // Error
	{ 0x09D, 33, 1, 3, 10, 0 }, // Knuckles restores the Master Emerald
	{ 0x09F, 33, 1, 3, 11, 2 }, // Knuckles Outro
	{ 0x0A0, 26, 4, 3, 3, 2 }, // Knuckles follows Eggman in Station Square hotel
		
	// Gamma events
	{ 0x0B0, 33, 3, 6, 0, 0 }, // Gamma Intro
	{ 0x0B1, 33, 3, 6, 0, 2 }, // Gamma Enters Final Egg
	{ 0x0B2, 33, 3, 6, 0, 5 }, // Gamma Exits Final Egg
	{ 0x0B3, 33, 3, 6, 0, 4 }, // Useless machine
	{ 0x0B4, 33, 3, 6, 0, 7 }, // Gamma's Fight with Beta
	{ 0x0B5, 33, 3, 6, 0, 9 }, // Gamma defeats Beta
	{ 0x0B7, 32, 1, 6, 1, 1 }, // The hunt for Froggy begins
	{ 0x0B8, 1, 0, 6, 2, 2 }, // Gamma goes to the Past
	{ 0x0B9, 34, 1, 6, 3, 0 }, // Gamma in the Past
	{ 0x0BA, 34, 1, 6, 3, 2 }, // Gamma and Tikal meet
	{ 0x0BB, 32, 1, 6, 4, 0 }, // Gamma returns to the Egg Carrier
	{ 0x0BC, 32, 1, 6, 4, 2 }, // Gamma goes to the wrong room
	{ 0x0BD, 32, 1, 6, 4, 3 }, // Beta's new body being built
	{ 0x0BE, 32, 1, 6, 4, 4 }, // Gamma leaves Beta's room
	{ 0x0BF, 32, 3, 6, 4, 6 }, // Gamma enters the jail, meets Amy
	{ 0x0C0, 32, 1, 6, 4, 10 }, // Gamma heading to the rear of the ship
	{ 0x0C1, 29, 2, 6, 4, 12 }, // Gamma emerges to fight Sonic
	{ 0x0C2, 29, 2, 6, 4, 15 }, // Gamma after the battle with Sonic
	{ 0x0C3, 33, 0, 6, 5, 1 }, // Gamma's objectives changed
	{ 0x0C5, 33, 1, 6, 7, 0 }, // Gamma remembers his brothers
	{ 0x0C7, 25, 0, 6, 8, 6 }, // Gamma Outro

	// Big events
	{ 0x0D0, 33, 2, 7, 0, 0 }, // Big Intro
	{ 0x0D1, 26, 3, 7, 1, 0 }, // Big goes searching for Froggy
	{ 0x0D2, 26, 0, 7, 1, 2 }, // Froggy heads into the sewers
	{ 0x0D3, 33, 0, 7, 2, 3 }, // Big finds Froggy with Tails
	{ 0x0D4, 1,  2, 7, 3, 2 }, // Big loses Froggy to Gamma
	{ 0x0D8, 32, 1, 7, 4, 1 }, // Big enters Hot Shelter
	{ 0x0D9, 12, 0, 7, 4, 2 }, // Big spots Froggy inside the tanks
	{ 0x0DA, 12, 0, 7, 4, 2 }, // Big saves Froggy
	{ 0x0DB, 34, 1, 7, 5, 0 }, // Big heads into the past
	{ 0x0DC, 34, 1, 7, 5, 2 }, // Tikal talks to Big
	{ 0x0DD, 32, 1, 7, 6, 0 }, // Big returns and is ready to leave the Egg Carrier
	{ 0x0DE, 18, 0, 7, 6, 4 }, // Chaos 6 takes Froggy
	{ 0x0DF, 18, 0, 7, 6, 5 }, // Sonic saves Froggy
	{ 0x0E0, 29, 0, 7, 6, 7 }, // Big finds the Tornado 2 and leaves
	{ 0x0E1, 26, 0, 7, -1 }, // Error
	{ 0x0E2, 33, 2, 7, 7, 1  }, // Big Outro
	{ 0x0E3, 26, 3, 7, 3, 1 }, // Big sees Froggy heading to the beach

	// Last Story
	{ 0x0F0, 33, 2, 0, 0, 0 }, // Tornado 2 Flash scene
	{ 0x0F2, 33, 2, 0, 1, 2 }, // Eggman heading to the Mystic Ruins base
	{ 0x0F3, 33, 1, 0, 1, 4 }, // Knuckles at the Master Emerald
	{ 0x0F4, 33, 0, 0, 1, 6 }, // Tails runs to Sonic
	{ 0x0F5, 33, 1, 0, 1, 7 }, // Sonic and Tails find Eggman and Knuckles
	{ 0x0F6, 34, 2, 0, 2, 0 }, // Sonic travels to the past
	{ 0x0F7, 34, 2, 0, 2, 1 }, // Tikal pleads with her father
	{ 0x0F8, 34, 2, 0, 2, 4 }, // Tikal seals Chaos
	{ 0x0F9, 33, 1, 0, 3, 0 }, // Sonic returns to the present
	{ 0x0FA, 33, 2, 0, 3, 1 }, // Sonic and Tails find the Tornado 2
	{ 0x0FB, 34, 2, 0, 2, 3}, // Sonic checks on Tikal in the past
	{ 0x0FD, 19, 0, 0, 3, 2 }, // Perfect Chaos reveals himself
	{ 0x0FE, 19, 0, 0, 3, 2 }, // Last Story Outro
	{ 0x0FF, 19, 0, 0, 3, 2 }, // Everyone brings Sonic the emeralds

	// Additional Sonic events
	{ 0x100, 29, 0, 0, 10, 1 }, // Sonic and Tails after landing on the Egg Carrier
	{ 0x101, 29, 0, 0, 10, 3 }, // Sonic and Tails Sky Chase attack (don't get too many ideas)
	{ 0x102, 29, 1, 0, 10, 5 }, // The Egg Carrier Transforms
	{ 0x103, 29, 1, 0, 10, 7 }, // Sonic at the Sky Deck Entrance
	{ 0x104, 32, 1, 0, 11, 0 }, // Sonic right after Sky Deck (Is that it?)
	{ 0x106, 29, 2, 0, 11, 7 }, // Sonic heading to transform the Egg Carrier
	{ 0x107, 29, 3, 0, 11, 8 }, // Emergency altert cancelled (Sonic)

	// Additional Tails events
	{ 0x110, 29, 0, 2, 8, 1 }, // Tails and Sonic after landing on the Egg Carrier
	{ 0x111, 29, 0, 2, 8, 3 }, // Tails' Sky Chase Attack
	{ 0x112, 29, 1, 2, 8, 5 }, // The Egg Carrier Transforms
	{ 0x113, 29, 1, 2, 8, 7 }, // Tails at the Sky Deck Entrance
	{ 0x114, 32, 1, 2, 9, 0 }, // Tails right after Sky Deck

	// Additional Knuckles events
	{ 0x120, 29, 2, 3, 7, 2 }, // Egg Carrier Transforms 1
	{ 0x121, 29, 2, 3, 7, 4 }, // Egg Carrier Transforms 2
	{ 0x122, 29, 5, 3, 7, 6 }, // Knuckles sensing the emeralds on the Egg Carrier

	// Additional Amy events
	{ 0x130, 32, 2, 5, 2, 3 }, // Introduction to Hedgehog Hammer
	{ 0x131, 32, 2, 5, 2, 4 }, // Winning at Hedgehog Hammer

	// Additional Gamma events
	{ 0x140, 32, 1, 6, 4, 8 }, // Gamma is told to find the Jet Booster
	{ 0x141, 29, 0, 6, 8, 0 }, // Gamma heads to Hot Shelter
	{ 0x142, 29, 0, 6, 8, 3 }, // Gamma rescues E-105

	// Additional Big events
	{ 0x150, 29, 3, 7, 6, 1 }, // Egg Carrier Transforms

	// Additional Last Story events
	{ 0x160, 34, 2, 0, 2, 2 }, // The Echidna tribe faces Chaos
		
	// Upgrade Cutscenes
	{ 0x165, 26, 4, 0, -1 }, // Sonic gets the Crystal Ring
	{ 0x166, 26, 2, 0, 3, 2 }, // Sonic gets the LSDash Shoe
	{ 0x167, 33, 1, 0, 9, 3 }, // Sonic gets the Ancient Light
	{ 0x168, 26, 3, 2, -1 }, // Tails gets the Jet Anklet
	{ 0x169, 34, 0, 2, 6, 2 }, // Tails gets the Rhythm Badge
	{ 0x16A, 33, 2, 3, -1 }, // Knuckles gets the Fighting Gloves
	{ 0x16B, 33, 0, 3, 4, 5 }, // Knuckles gets the Shovel Claw
	{ 0x16C, 32, 2, 5, -1 }, // Amy gets the Long Hammer
	{ 0x16D, 32, 2, 5, 2, 4 }, // Amy gets the Warrior Feather
	{ 0x16E, 32, 4, 6, -1 }, // Gamma gets the Laser Blaster
	{ 0x16F, 32, 0, 6, 4, 9 }, // Gamma gets the Jet Booster
	{ 0x170, 33, 2, 7, -1 }, // Big gets the PowerRod
	{ 0x171, 33, 1, 7, 2, 2 }, // Big gets the Life Belt

	// Misc Events
	{ 0x176, 26, 1, 0, 3, 5 }, // Ice Stone appears (Sonic)
	{ 0x177, 26, 1, 2, 2, 2 }, // Ice Stone appears (Tails)
	{ 0x178, 26, 3, 7, 1, 7 }, // Ice Stone appears (Big)
	{ 0x179, 26, 3, 0, 7, 1 }, // Employee Card appears
	{ 0x17A, 33, 0, 0, 5, 0 /*2,0 for Big*/ }, // Passage to Angel Island opens (Sonic, Big)
	{ 0x17B, 33, 0, 2, 4, 0 }, // Passage to Angel Island opens (Tails)
	{ 0x17C, 33, 0, 6, 6, 0 }, // Passage to Angel Island opens (Gamma)
	{ 0x180, 5, 0, 0, 9, 3 }, // Egg Carrier in Red Mountain
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

static void SetLevelCleared(int level, int character)
{
	LevelClearCounts[43 * character + level] = 1;
}

static void __cdecl PATCH_EV0103_EV0113()
{
	EV_SetPos(EV_GetPlayer(0), 0.0f, 740.0f, 362.0f);
	EV_SetAng(EV_GetPlayer(0), 0, 0, 0);
}

static void __cdecl PATCH_EV0122()
{
	EV_SetPos(EV_GetPlayer(0), 80.0f, -70.0f, 0.0f);
	EV_SetAng(EV_GetPlayer(0), 0, 0xC000, 0);
}

static void __cdecl PATCH_EV0131()
{
	EV_SetPos(EV_GetPlayer(0), 0.0f, 12.8f, 0.0f);
	EV_SetAng(EV_GetPlayer(0), 0, 0x8000, 0);
}

static void __cdecl PATCH_EV0140()
{
	EV_SetPos(EV_GetPlayer(0), -47.0f, 0.0f, 172.0f);
	EV_SetAng(EV_GetPlayer(0), 0, 0x6000, 0);
}

static void __cdecl PATCH_EV016E()
{
	EV_SetPos(EV_GetPlayer(0), 35.0f, 56.72f, 0.0f);
	EV_SetAng(EV_GetPlayer(0), 0, 0x8000, 0);
}

static void __cdecl PATCH_EV00BA(int n)
{
	EV_InitPlayer(n);
	EV_SetAction(EV_GetPlayer(n), E102_ACTIONS[0], &E102_TEXLIST, 0.3f, 1, 0);
}

// Patch EV_ALife's GetShadowPos to handle no ground detected situations.
static float PATCH_EV0022(float x, float y, float z, Angle3* ang)
{
	auto ret = GetShadowPos(x, y, z, ang);
	return ret > -1000000.0f ? ret : y - 1.4f;
}

static void SetEventFlagsForCutscene(int eventID)
{
	switch (eventID)
	{
	case 0x0002:
		LevelCutscenes2[2].Cutscene = eventID;
		break;
	case 0x0009: // Sonic and Tails gassed
		SetEventFlag((EventFlags)FLAG_SONIC_SS_ENTRANCE_CASINO);
		break;
	case 0x0014:
		SetLevelCleared(LevelIDs_TwinklePark, Characters_Sonic);
		break;
	case 0x0015:
		SetLevelCleared(LevelIDs_SpeedHighway, Characters_Sonic);
		break;
	case 0x001D: // Sonic jumps from the Egg Carrier
		WriteData((char*)0x559FC0, (char)0xC3); // Don't load Chaos 6
		LevelCutscenes2[4].Cutscene = eventID;
		break;
	case 0x0020: // Sonic sees the mural
		WriteData((char*)0x7B0DA0, (char)0xC3u); // Lost World 3 end level object
		WriteData((char*)0x5E18B0, (char)0xC3u); // Level object that plays music
		break;
	case 0x0022:
		WriteCall((void*)0x6EF1F7, PATCH_EV0022);
		break;
	case 0x0023:
		SetEventFlag((EventFlags)FLAG_SONIC_MR_APPEAR_FINALEGG);
		break;
	case 0x0026:
		SetLevelCleared(LevelIDs_FinalEgg, Characters_Sonic);
		break;
	case 0x0029: // Sonic and Tails land on the Egg Carrier
		SetEventFlag((EventFlags)FLAG_SONIC_EC_TORNADO2_LOST);
		break;
	case 0x0035: // Tails and Sonic gassed
		SetEventFlag((EventFlags)FLAG_MILES_SS_ENTRANCE_CASINO);
		break;
	case 0x0040: // Tails wakes up from his flashback
	case 0x0042: // Tails chases Froggy
		CutsceneFlagArray[0x003E] = 1;
		break;
	case 0x0050: // Egg Walker
	case 0x0051: // Egg Walker defeated
		SetLevelCleared(LevelIDs_SpeedHighway, Characters_Tails);
		break;
	case 0x0054: // Tails and Sonic land on the Egg Carrier
		SetEventFlag((EventFlags)FLAG_MILES_EC_TORNADO2_LOST);
		break;
	case 0x0055: // Tails saves Froggy in Sand Hill
		WriteData((char*)0x598040, (char)0xC3u); // Osfrog
		WriteData((char*)0x79E4C0, (char)0xC3u); // Plays level music
		WriteData<5>((char*)0x597BF3, 0x90u); // Snowboard
		break;
	case 0x006E: // Amy discovers Final Egg base
		SetEventFlag((EventFlags)FLAG_AMY_MR_APPEAR_FINALEGG); // Open Final Egg for Amy
		SetEventFlag((EventFlags)FLAG_AMY_MR_ENTRANCE_FINALEGG); // Open Final Egg for Amy
		break;
	case 0x0070:
		SetLevelCleared(LevelIDs_FinalEgg, Characters_Amy);
		break;
	case 0x0072: // Amy outro
		SetEventFlag((EventFlags)FLAG_AMY_EC_SINK); // Egg Carrier sunk in Amy's outro
		break;
	case 0x0086: // Knuckles returns from the Past to Station Square
		SetEventFlag((EventFlags)FLAG_KNUCKLES_SS_ENTRANCE_CASINO);
		break;
	case 0x0088: // Knuckles is tricked by Eggman
		WriteData((char*)0x54DF00, (char)0xC3); // Don't load Chaos 2
		LevelCutscenes2[3].Cutscene = eventID;
		break;
	case 0x008D: // Knuckles goes to the Past from Lost World
		WriteData<1>((char*)0x5E18B0, 0xC3u); // Level object that plays music
		break;
	case 0x0092: // Knuckles follows Gamma to Final Egg
		SetEventFlag((EventFlags)FLAG_KNUCKLES_MR_APPEAR_FINALEGG); // Open Final Egg for Knuckles
		break;
	case 0x0095: // Knuckles finds the last missing piece in Sky Deck
		WriteData<5>((char*)0x5EF6D0, 0x90u); // Remove Sky Deck music
		WriteData((char*)0x450370, (char)0xC3u); // Remove Rings
		WriteData((char*)0x7A1AA0, (char)0xC3u); // Remove Tikal hints
		WriteData((char*)0x476440, (char)0xC3u); // Remove Radar
		break;
	case 0x009B: // Knuckles defeats Chaos 6
		WriteData((char*)0x559FC0, (char)0xC3); // Don't load Chaos 6
		LevelCutscenes2[5].Cutscene = eventID;
		break;
	case 0x00B3: // Useless machine
		SetEventFlag((EventFlags)FLAG_E102_MR_FREEPASS); // Open Final Egg for useless machine
		SetEventFlag((EventFlags)FLAG_E102_CLEAR_BEACH); // Open Final Egg for useless machine
		SetEventFlag((EventFlags)FLAG_E102_CLEAR_FINALEGG); // Open Final Egg for useless machine
		SetEventFlag((EventFlags)FLAG_E102_MR_APPEAR_FINALEGG); // Open Final Egg for useless machine
		break;
	case 0x00B8: // Gamma goes to the Past
		WriteData((char*)0x61CA90, (char)0xC3u); // Remove Emerald Coast music
		WriteData((char*)0x4AD140, (char)0xC3u); // Remove Kikis
		WriteData((char*)0x4FA320, (char)0xC3u); // Remove OFrog
		break;
	case 0x00C0: // Gamma heading to the rear of the ship
	case 0x00C1: // Gamma vs Sonic
	case 0x00C2: // Gamma after fighting Sonic
	case 0x00C3: // Gamma's objectives changed
		SetEventFlag((EventFlags)FLAG_E102_EC_BOOSTER); // Cutscenes where Gamma appears with the Jet Booster
		break;
	case 0x00BA: // Gamma meets Tikal
		WriteCall((void*)0x67DD88, PATCH_EV00BA);
		break;
	case 0x00C5: // Gamma remembers his brothers
		SetLevelCleared(LevelIDs_RedMountain, Characters_Gamma);
		SetEventFlag((EventFlags)FLAG_E102_MR_ENTRANCE_MOUNTAIN);
		break;
	case 0x00D3: // Big finds Froggy with Tails
		SetLevelCleared(LevelIDs_IceCap, Characters_Big);
		break;
	case 0x00D4: // Big loses Froggy to Gamma
		WriteData((char*)0x61CA90, (char)0xC3u); // Remove Emerald Coast music
		break;
	case 0x00DA: // Big saves Froggy in Hot Shelter
		CutsceneFlagArray[217] = 1;
		break;
	case 0x00DF: // Sonic saves Froggy
		LevelCutscenes2[6].Cutscene = eventID;
		break;
	case 0x0103:
		WriteCall((void*)0x657378, PATCH_EV0103_EV0113);
		break;
	case 0x0113:
		WriteCall((void*)0x655F38, PATCH_EV0103_EV0113);
		break;
	case 0x0122:
		WriteCall((void*)0x655712, PATCH_EV0122);
		SetEventFlag((EventFlags)FLAG_KNUCKLES_EC_PALMSWITCH);
		break;
	case 0x0131:
		WriteCall((void*)0x655574, PATCH_EV0131);
		break;
	case 0x0140:
		WriteCall((void*)0x6554C8, PATCH_EV0140);
		break;
	case 0x0141: // Gamma heads to Hot Shelter
	case 0x0142: // Gamma rescues E-105
		SetEventFlag((EventFlags)FLAG_E102_EC_SINK);
		break;
	case 0x0165: // Sonic gets the Crystal Ring
		SetEventFlag((EventFlags)FLAG_SONIC_SS_CRYSTALRING);
		break;
	case 0x0166: // Sonic gets the Light Speed Shoes
		SetEventFlag((EventFlags)FLAG_SONIC_SS_LIGHTSHOOSE);
		break;
	case 0x0167: // Sonic gets the Ancient Light
		SetEventFlag((EventFlags)FLAG_SONIC_MR_ANCIENT_LIGHT);
		break;
	case 0x0168: // Tails gets the Jet Anklet
		SetEventFlag((EventFlags)FLAG_MILES_SS_JETANKLET);
		break;
	case 0x0169: // Tails gets the Rhythm Badge
		SetEventFlag((EventFlags)FLAG_MILES_MR_RHYTHMBROOCH);
		break;
	case 0x016A: // Knuckles gets the Fighting Gloves
		SetEventFlag((EventFlags)FLAG_KNUCKLES_MR_FIGHTINGGROVE);
		break;
	case 0x016B: // Knuckles gets the Shovel Claw
		SetEventFlag((EventFlags)FLAG_KNUCKLES_MR_SHOVELCLAW);
		break;
	case 0x016C: // Amy gets the Long Hammer
		SetEventFlag((EventFlags)FLAG_AMY_MR_FIGHTERSFEATHER);
		SetEventFlag((EventFlags)FLAG_AMY_EC_LONGHAMMER);
		break;
	case 0x016D: // Amy gets the Warrior Feather
		SetEventFlag((EventFlags)FLAG_AMY_MR_FIGHTERSFEATHER);
		break;
	case 0x016E: // Gamma gets the Laser Blaster
		WriteCall((void*)0x650FA8, PATCH_EV016E);
		SetEventFlag((EventFlags)FLAG_E102_EC_TYPE3LASER);
		break;
	case 0x016F: // Gamma gets the Jet Booster
		SetEventFlag((EventFlags)FLAG_E102_EC_BOOSTER);
		break;
	case 0x0170: // Big gets the Power Rod
		SetEventFlag((EventFlags)FLAG_BIG_MR_POWERROD);
		break;
	case 0x0171: // Big gets the Life Belt
		SetEventFlag((EventFlags)FLAG_BIG_MR_LIFEBELT);
		break;
	case 0x0177: // Ice Stone appears (Tails)
		SetEventFlag((EventFlags)FLAG_MILES_SS_ICESTONE);
		SetEventFlag((EventFlags)FLAG_MILES_SS_ENTRANCE_CASINO);
		break;
	case 0x017C: // Angel Island opens (Gamma)
		SetLevelCleared(LevelIDs_WindyValley, Characters_Gamma);
		break;
	case 0x00F0:
	case 0x00F2:
	case 0x00F3:
	case 0x00F4:
	case 0x00F5:
	case 0x00F6:
	case 0x00F7:
	case 0x00F8:
	case 0x00F9:
	case 0x00FA:
	case 0x00FB:
	case 0x0160:
		LastStoryFlag = 1;
		break;
	case 0x00FD: // Perfect Chaos level cutscenes
	case 0x00FE:
	case 0x00FF:
		LevelCutscenes2[7].Cutscene = eventID;
		LastStoryFlag = 1;
		break;
	}
}

static Trampoline* CheckStandaloneEvent_t = nullptr;

static void __cdecl CustomEventTask(task* tp)
{
	if (GameState == 15)
	{
		tp->exec = (void(__cdecl*)(task*))0x42CAC0;
		SoundManager_Delete2();
		LoadCutscene(testspawn_eventid);
		LoadEVThread();

		// Remove our code changes
		delete CheckStandaloneEvent_t;
	}
}

static void __cdecl CheckStandaloneEvent_r()
{
	if (!EV_MainThread_ptr)
	{
		CreateElementalTask(0, LEV_0, CustomEventTask);
	}
}

static void __cdecl ForceEventMode()
{
	auto data = GetCutsceneData(testspawn_eventid);

	if (data != nullptr)
	{
		SetupCharacter(data->character);
		SetEventFlagsForCutscene(data->cutscene_id);
		SetLevelAndAct(data->level, data->act);

		// If the event has story integration
		if (data->scene_select != -1)
		{
			StoryEventMode = 2; // Force story event to play
			StoryEventID = testspawn_eventid;
			SeqSetPlayer(data->character); // Get story information
			pCurSectionList = &pCurSectionList[data->scene_select]; // Get scene information
			pCurSection = pCurSectionList->psec; // Current scene manager functions
			pCurSequence->seqno = data->seqno; // Current subscene of the scene
			pCurSequence->destination = -1; // Force story progression
			GameMode = GameModes_Adventure_Field;
			return;
		}
	}

	// If we have no story information for the event, run it as standalone.
	CheckStandaloneEvent_t = new Trampoline(0x413A10, 0x413A15, CheckStandaloneEvent_r, true);
	GameMode = GetLevelType() == 1 ? GameModes_Adventure_Field : GameModes_Adventure_ActionStg;
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

void ProcessTestSpawn(const HelperFunctions& helperFunctions)
{
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
		else if (!wcscmp(argv[i], L"--time") || !wcscmp(argv[i], L"-t"))
		{
			testspawn_timeofday = parse_time_id(argv[++i]);
			WriteJump(GetTimeOfDay, ForceTimeOfDay);
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
			testspawn_eventenabled = true;
			testspawn_eventid = _wtoi(argv[++i]);
			PrintDebug("Loading event: EV%04x (%d)\n", testspawn_eventid, testspawn_eventid);

			// NOP. Prevents story sequence manager to be reset.
			WriteData<5>((void*)0x413884, 0x90); 
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
	if (testspawn_eventenabled == true)
	{
		WriteJump(reinterpret_cast<void*>(0x0040C106), ForceEventMode_asm);
	}
	else if (testspawn_enabled == true)
	{
		WriteData(reinterpret_cast<GameModes*>(0x0040C10C), GameModes_Trial);
	}
}