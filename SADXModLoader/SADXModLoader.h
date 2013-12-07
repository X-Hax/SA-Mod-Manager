#include "ninja.h"
#ifndef SADXMODLOADER_H
#define SADXMODLOADER_H

static const int ModLoaderVer = 2;

#define arrayptrandlength(data) data, LengthOfArray(data)
#define arraylengthandptr(data) LengthOfArray(data), data
#define arrayptrandsize(data) data, SizeOfArray(data)
#define arraysizeandptr(data) SizeOfArray(data), data

struct PatchInfo
{
	void *address;
	void *data;
	int datasize;
};

#define patchdecl(address,data) { (void*)address, arrayptrandsize(data) }

struct PointerInfo
{
	void *address;
	void *data;
};

#define ptrdecl(address,data) { (void*)address, (void*)data }

struct ExportInfo
{
	const char *const name;
	void *data;
};

struct ModInfo
{
	int Version;
	void (*Init)();
	PatchInfo *Patches;
	int PatchCount;
	PointerInfo *Jumps;
	int JumpCount;
	PointerInfo *Calls;
	int CallCount;
	PointerInfo *Pointers;
	int PointerCount;
	ExportInfo *Exports;
	int ExportCount;
};

// Utility Functions
template <typename T, size_t N>
inline size_t LengthOfArray( const T(&)[ N ] )
{
	return N;
}

template <typename T, size_t N>
inline size_t SizeOfArray( const T(&)[ N ] )
{
	return N * sizeof(T);
}

static BOOL WriteData(void *writeaddress, void *data, SIZE_T datasize, SIZE_T *byteswritten)
{
	return WriteProcessMemory(GetCurrentProcess(), writeaddress, data, datasize, byteswritten);
}

static BOOL WriteData(void *writeaddress, void *data, SIZE_T datasize)
{
	SIZE_T written;
	return WriteData(writeaddress, data, datasize, &written);
}

template<typename T> static BOOL WriteData(T const *writeaddress, T data, SIZE_T *byteswritten)
{
	return WriteData((void*)writeaddress, (void*)&data, (SIZE_T)sizeof(data), byteswritten);
}

template<typename T> static BOOL WriteData(T const *writeaddress, T data)
{
	SIZE_T written;
	return WriteData(writeaddress, data, &written);
}

template<typename T> static BOOL WriteData(T *writeaddress, T data, SIZE_T *byteswritten)
{
	return WriteData(writeaddress, &data, sizeof(data), byteswritten);
}

template<typename T> static BOOL WriteData(T *writeaddress, T data)
{
	SIZE_T written;
	return WriteData(writeaddress, data, &written);
}

template <typename T, size_t N> static BOOL WriteData(void *writeaddress, T(&data)[N], SIZE_T *byteswritten)
{
	return WriteData(writeaddress, data, SizeOfArray(data), byteswritten);
}

template <typename T, size_t N> static BOOL WriteData(void *writeaddress, T(&data)[N])
{
	SIZE_T written;
	return WriteData(writeaddress, data, &written);
}

static BOOL WriteData(void *address, char data, int count, SIZE_T *byteswritten)
{
	char *buf = new char[count];
	memset(buf, data, count);
	int result = WriteData(address, buf, count, byteswritten);
	delete[] buf;
	return result;
}

static BOOL WriteData(void *address, char data, int count)
{
	SIZE_T written;
	return WriteData(address, data, count, &written);
}

static BOOL WriteJump(void *writeaddress, void *funcaddress)
{
	unsigned char data[5];
	data[0] = 0xE9;
	*(signed int *)(data + 1) = (unsigned int)funcaddress - ((unsigned int)writeaddress + 5);
	return WriteData(writeaddress, data);
}

static BOOL WriteCall(void *writeaddress, void *funcaddress)
{
	unsigned char data[5];
	data[0] = 0xE8;
	*(signed int *)(data + 1) = (unsigned int)funcaddress - ((unsigned int)writeaddress + 5);
	return WriteData(writeaddress, data);
}

// SADX Enums
#define makemask(enumname,membername) enumname##_##membername = (1 << enumname##Bits_##membername)
#define makemasks(enumname,membername) enumname##s_##membername = (1 << enumname##Bits_##membername)

enum StatusBits
{
	StatusBits_Ground = 0,
	StatusBits_Hurt = 2,
	StatusBits_Ball = 8,
	StatusBits_LightDash,
	StatusBits_Attack,
	StatusBits_HoldObject,
	StatusBits_DoNextAct
};

enum Status : unsigned short
{
	makemask(Status, Ground),
	makemask(Status, Hurt),
	makemask(Status, Ball),
	makemask(Status, LightDash),
	makemask(Status, Attack),
	makemask(Status, HoldObject),
	makemask(Status, DoNextAct)
};

enum PowerupBits
{
	PowerupBits_Barrier,
	PowerupBits_MagneticBarrier,
	PowerupBits_Invincibility = 15
};

enum Powerups : unsigned short
{
	makemasks(Powerup, Barrier),
	makemasks(Powerup, MagneticBarrier),
	makemasks(Powerup, Invincibility)
};

enum UpgradeBits
{
	UpgradeBits_CrystalRing,
	UpgradeBits_LightShoes,
	UpgradeBits_AncientLight,
	UpgradeBits_JetAnklet,
	UpgradeBits_RhythymBadge,
	UpgradeBits_FightingGloves,
	UpgradeBits_ShovelClaw,
	UpgradeBits_LongHammer,
	UpgradeBits_WarriorFeather,
	UpgradeBits_LaserBlaster,
	UpgradeBits_JetBooster,
	UpgradeBits_PowerRod,
	UpgradeBits_LifeRing,
	UpgradeBits_SuperSonic
};

enum Upgrades
{
	makemasks(Upgrade, CrystalRing),
	makemasks(Upgrade, LightShoes),
	makemasks(Upgrade, AncientLight),
	makemasks(Upgrade, JetAnklet),
	makemasks(Upgrade, RhythymBadge),
	makemasks(Upgrade, FightingGloves),
	makemasks(Upgrade, ShovelClaw),
	makemasks(Upgrade, LongHammer),
	makemasks(Upgrade, WarriorFeather),
	makemasks(Upgrade, LaserBlaster),
	makemasks(Upgrade, JetBooster),
	makemasks(Upgrade, PowerRod),
	makemasks(Upgrade, LifeRing),
	makemasks(Upgrade, SuperSonic)
};

enum Characters
{
	Characters_Sonic,
	Characters_Eggman,
	Characters_Tails,
	Characters_Knuckles,
	Characters_Tikal,
	Characters_Amy,
	Characters_Gamma,
	Characters_Big,
	Characters_MetalSonic
};

enum CharacterFlags
{
	CharacterFlags_Sonic = 1 << Characters_Sonic,
	CharacterFlags_Eggman = 1 << Characters_Eggman,
	CharacterFlags_Tails = 1 << Characters_Tails,
	CharacterFlags_Knuckles = 1 << Characters_Knuckles,
	CharacterFlags_Tikal = 1 << Characters_Tikal,
	CharacterFlags_Amy = 1 << Characters_Amy,
	CharacterFlags_Gamma = 1 << Characters_Gamma,
	CharacterFlags_Big = 1 << Characters_Big
};

enum LevelIDs
{
	LevelIDs_HedgehogHammer,
	LevelIDs_EmeraldCoast,
	LevelIDs_WindyValley,
	LevelIDs_TwinklePark,
	LevelIDs_SpeedHighway,
	LevelIDs_RedMountain,
	LevelIDs_SkyDeck,
	LevelIDs_LostWorld,
	LevelIDs_IceCap,
	LevelIDs_Casinopolis,
	LevelIDs_FinalEgg,
	LevelIDs_B,
	LevelIDs_HotShelter,
	LevelIDs_D,
	LevelIDs_E,
	LevelIDs_Chaos0,
	LevelIDs_Chaos2,
	LevelIDs_Chaos4,
	LevelIDs_Chaos6,
	LevelIDs_PerfectChaos,
	LevelIDs_EggHornet,
	LevelIDs_EggWalker,
	LevelIDs_EggViper,
	LevelIDs_Zero,
	LevelIDs_E101,
	LevelIDs_E101R,
	LevelIDs_StationSquare,
	LevelIDs_EggCarrierOutside = 29,
	LevelIDs_EggCarrierInside = 32,
	LevelIDs_MysticRuins,
	LevelIDs_Past,
	LevelIDs_TwinkleCircuit,
	LevelIDs_SkyChase1,
	LevelIDs_SkyChase2,
	LevelIDs_SandHill,
	LevelIDs_SSGarden,
	LevelIDs_ECGarden,
	LevelIDs_MRGarden,
	LevelIDs_ChaoRace,
	LevelIDs_Invalid
};

enum GameModes
{
	GameModes_Restart,
	GameModes_Logo,
	GameModes_Exit,
	GameModes_Adventure_ActionStg = 4,
	GameModes_Adventure_Field,
	GameModes_Movie = 8,
	GameModes_Trial,
	GameModes_Mission,
	GameModes_CharSel,
	GameModes_Menu,
	GameModes_Restart2,
	GameModes_StartAdventure = 17,
	GameModes_Adventure_Story,
	GameModes_StartCredits = 21,
	GameModes_Credits,
};

enum LevelAndActIDs
{
	LevelAndActIDs_HedgehogHammer,
	LevelAndActIDs_EmeraldCoast1 = LevelIDs_EmeraldCoast << 8,
	LevelAndActIDs_EmeraldCoast2,
	LevelAndActIDs_EmeraldCoast3,
	LevelAndActIDs_WindyValley1 = LevelIDs_WindyValley << 8,
	LevelAndActIDs_WindyValley2,
	LevelAndActIDs_WindyValley3,
	LevelAndActIDs_TwinklePark1 = LevelIDs_TwinklePark << 8,
	LevelAndActIDs_TwinklePark2,
	LevelAndActIDs_TwinklePark3,
	LevelAndActIDs_SpeedHighway1 = LevelIDs_SpeedHighway << 8,
	LevelAndActIDs_SpeedHighway2,
	LevelAndActIDs_SpeedHighway3,
	LevelAndActIDs_RedMountain1 = LevelIDs_RedMountain << 8,
	LevelAndActIDs_RedMountain2,
	LevelAndActIDs_RedMountain3,
	LevelAndActIDs_SkyDeck1 = LevelIDs_SkyDeck << 8,
	LevelAndActIDs_SkyDeck2,
	LevelAndActIDs_SkyDeck3,
	LevelAndActIDs_LostWorld1 = LevelIDs_LostWorld << 8,
	LevelAndActIDs_LostWorld2,
	LevelAndActIDs_LostWorld3,
	LevelAndActIDs_IceCap1 = LevelIDs_IceCap << 8,
	LevelAndActIDs_IceCap2,
	LevelAndActIDs_IceCap3,
	LevelAndActIDs_IceCap4,
	LevelAndActIDs_Casinopolis1 = LevelIDs_Casinopolis << 8,
	LevelAndActIDs_Casinopolis2,
	LevelAndActIDs_Casinopolis3,
	LevelAndActIDs_Casinopolis4,
	LevelAndActIDs_FinalEgg1 = LevelIDs_FinalEgg << 8,
	LevelAndActIDs_FinalEgg2,
	LevelAndActIDs_FinalEgg3,
	LevelAndActIDs_HotShelter1 = LevelIDs_HotShelter << 8,
	LevelAndActIDs_HotShelter2,
	LevelAndActIDs_HotShelter3,
	LevelAndActIDs_HotShelter4,
	LevelAndActIDs_Chaos0 = LevelIDs_Chaos0 << 8,
	LevelAndActIDs_Chaos2 = LevelIDs_Chaos2 << 8,
	LevelAndActIDs_Chaos4 = LevelIDs_Chaos4 << 8,
	LevelAndActIDs_Chaos6 = LevelIDs_Chaos6 << 8,
	LevelAndActIDs_PerfectChaos = LevelIDs_PerfectChaos << 8,
	LevelAndActIDs_EggHornet = LevelIDs_EggHornet << 8,
	LevelAndActIDs_EggWalker = LevelIDs_EggWalker << 8,
	LevelAndActIDs_EggViper = LevelIDs_EggViper << 8,
	LevelAndActIDs_Zero = LevelIDs_Zero << 8,
	LevelAndActIDs_E101 = LevelIDs_E101 << 8,
	LevelAndActIDs_E101R = LevelIDs_E101R << 8,
	LevelAndActIDs_StationSquare1 = LevelIDs_StationSquare << 8,
	LevelAndActIDs_StationSquare2,
	LevelAndActIDs_StationSquare3,
	LevelAndActIDs_StationSquare4,
	LevelAndActIDs_StationSquare5,
	LevelAndActIDs_StationSquare6,
	LevelAndActIDs_EggCarrierOutside1 = LevelIDs_EggCarrierOutside << 8,
	LevelAndActIDs_EggCarrierOutside2,
	LevelAndActIDs_EggCarrierOutside3,
	LevelAndActIDs_EggCarrierOutside4,
	LevelAndActIDs_EggCarrierOutside5,
	LevelAndActIDs_EggCarrierOutside6,
	LevelAndActIDs_EggCarrierOutside7,
	LevelAndActIDs_EggCarrierInside1 = LevelIDs_EggCarrierInside << 8,
	LevelAndActIDs_EggCarrierInside2,
	LevelAndActIDs_EggCarrierInside3,
	LevelAndActIDs_EggCarrierInside4,
	LevelAndActIDs_EggCarrierInside5,
	LevelAndActIDs_EggCarrierInside6,
	LevelAndActIDs_MysticRuins1 = LevelIDs_MysticRuins << 8,
	LevelAndActIDs_MysticRuins2,
	LevelAndActIDs_MysticRuins3,
	LevelAndActIDs_MysticRuins4,
	LevelAndActIDs_Past1 = LevelIDs_Past << 8,
	LevelAndActIDs_Past2,
	LevelAndActIDs_Past3,
	LevelAndActIDs_TwinkleCircuit1 = LevelIDs_TwinkleCircuit << 8,
	LevelAndActIDs_TwinkleCircuit2,
	LevelAndActIDs_TwinkleCircuit3,
	LevelAndActIDs_TwinkleCircuit4,
	LevelAndActIDs_TwinkleCircuit5,
	LevelAndActIDs_TwinkleCircuit6,
	LevelAndActIDs_SkyChase1 = LevelIDs_SkyChase1 << 8,
	LevelAndActIDs_SkyChase2 = LevelIDs_SkyChase2 << 8,
	LevelAndActIDs_SandHill = LevelIDs_SandHill << 8,
	LevelAndActIDs_SSGarden = LevelIDs_SSGarden << 8,
	LevelAndActIDs_ECGarden = LevelIDs_ECGarden << 8,
	LevelAndActIDs_MRGarden = LevelIDs_MRGarden << 8,
	LevelAndActIDs_ChaoRace = LevelIDs_ChaoRace << 8
};

#define levelact(level, act) (level << 8) | act

enum Levels2P
{
	Levels2P_WindyValley,
	Levels2P_SpeedHighway,
	Levels2P_SkyDeck,
	Levels2P_IceCap,
	Levels2P_Casinopolis
};

enum ColFlags : signed int
{
	ColFlags_Solid = 0x1,
	ColFlags_Water = 0x2,
	ColFlags_NoFriction = 0x4,
	ColFlags_NoAccel = 0x8,
	ColFlags_IncAccel = 0x80,
	ColFlags_Dig = 0x100,
	ColFlags_NoClimb = 0x1000,
	ColFlags_Hurt = 0x10000,
	ColFlags_Footprints = 0x100000,
	ColFlags_Visible = (int)0x80000000
};

enum LevelItemAccess_Levels
{
	LevelItemAccess_Levels_StationSquare,
	LevelItemAccess_Levels_MysticRuins,
	LevelItemAccess_Levels_EggCarrier,
	LevelItemAccess_Levels_Other
};

enum EventFlags
{
	EventFlags_SonicUnlockedAdventure = 0x41,
	EventFlags_TailsUnlockedAdventure = 0x42,
	EventFlags_KnucklesUnlockedAdventure = 0x43,
	EventFlags_AmyUnlockedAdventure = 0x44,
	EventFlags_GammaUnlockedAdventure = 0x45,
	EventFlags_BigUnlockedAdventure = 0x46,
	EventFlags_SuperSonicUnlockedAdventure = 0x47,
	EventFlags_SonicAdventureComplete = 0x48,
	EventFlags_TailsAdventureComplete = 0x49,
	EventFlags_KnucklesAdventureComplete = 0x4A,
	EventFlags_AmyAdventureComplete = 0x4B,
	EventFlags_GammaAdventureComplete = 0x4C,
	EventFlags_BigAdventureComplete = 0x4D,
	EventFlags_SuperSonicAdventureComplete = 0x4E,
	EventFlags_GoldChaoEgg = 0x4F,
	EventFlags_SilverChaoEgg = 0x50,
	EventFlags_BlackChaoEgg = 0x51,
	EventFlags_Sonic_HotelOpen = 0x81,
	EventFlags_Sonic_StationOpen = 0x82,
	EventFlags_Sonic_CasinoAlwaysOpen = 0x84,
	EventFlags_Sonic_IceStoneAvailable = 0x85,
	EventFlags_Sonic_HotelToCasinoOpen = 0x86,
	EventFlags_Sonic_CasinopolisOpen = 0x87,
	EventFlags_Sonic_StationToCasinoOpen = 0x88,
	EventFlags_Sonic_TwinkleParkOpen = 0x89,
	EventFlags_Sonic_TwinkleCircuitOpen = 0x8A,
	EventFlags_Sonic_SpeedHighwayOpen = 0x8C,
	EventFlags_Sonic_LightShoes = 0x8D,
	EventFlags_Sonic_CrystalRing = 0x8E,
	EventFlags_Sonic_EggCarrierTransformed = 0x91,
	EventFlags_Sonic_EggCarrierOpen = 0x92,
	EventFlags_Sonic_EggCarrierSunk = 0x93,
	EventFlags_Sonic_WindyValleyOpen = 0x94,
	EventFlags_Sonic_AngelIslandOpen = 0x96,
	EventFlags_Sonic_IceCapOpen = 0x97,
	EventFlags_Sonic_RedMountainOpen = 0x99,
	EventFlags_Sonic_ShrineOpen = 0x9A,
	EventFlags_Sonic_JungleOpen = 0x9B,
	EventFlags_Sonic_LostWorldOpen = 0x9C,
	EventFlags_Sonic_FinalEggFieldOpen = 0x9D,
	EventFlags_Sonic_AncientLight = 0x9F,
	EventFlags_Sonic_FinalEggOpen = 0xA0,
	EventFlags_Sonic_TrainOpen = 0xA1,
	EventFlags_Sonic_GammaDefeated = 0xA4,
	EventFlags_Sonic_KnucklesDefeated = 0xA5,
	EventFlags_Sonic_EmeraldCoastClear = 0xA6,
	EventFlags_Sonic_WindyValleyClear = 0xA7,
	EventFlags_Sonic_CasinopolisClear = 0xA8,
	EventFlags_Sonic_TwinkleParkClear = 0xA9,
	EventFlags_Sonic_SpeedHighwayClear = 0xAA,
	EventFlags_Sonic_RedMountainClear = 0xAB,
	EventFlags_Sonic_IceCapClear = 0xAC,
	EventFlags_Sonic_SkyDeckClear = 0xAD,
	EventFlags_Sonic_LostWorldClear = 0xAE,
	EventFlags_Sonic_FinalEggClear = 0xAF,
	EventFlags_Sonic_Chaos0Clear = 0xB0,
	EventFlags_Sonic_Chaos4Clear = 0xB1,
	EventFlags_Sonic_Chaos6Clear = 0xB2,
	EventFlags_Sonic_EggHornetClear = 0xB3,
	EventFlags_Sonic_EggViperClear = 0xB4,
	EventFlags_Sonic_SkyChaseAct1Clear = 0xB5,
	EventFlags_Sonic_SkyChaseAct2Clear = 0xB6,
	EventFlags_Sonic_AdventureClear1 = 0xB7,
	EventFlags_Sonic_AdventureClear2 = 0xB8,
	EventFlags_Sonic_AdventureClear3 = 0xB9,
	EventFlags_Tails_HotelOpen = 0xC1,
	EventFlags_Tails_CasinopolisOpen = 0xC5,
	EventFlags_Tails_StationOpen = 0xCA,
	EventFlags_Tails_JetAnklet = 0xCD,
	EventFlags_Tails_EggCarrierSunk = 0xD2,
	EventFlags_Tails_WindyValleyOpen = 0xD3,
	EventFlags_Tails_IceCapOpen = 0xD6,
	EventFlags_Tails_RhythmBadge = 0xDB,
	EventFlags_Tails_TrainOpen = 0xDC,
	EventFlags_Tails_WindyValleyClear = 0xE1,
	EventFlags_Tails_CasinopolisClear = 0xE2,
	EventFlags_Tails_SpeedHighwayClear = 0xE3,
	EventFlags_Tails_IceCapClear = 0xE4,
	EventFlags_Tails_SkyDeckClear = 0xE5,
	EventFlags_Tails_SandHillClear = 0xE6,
	EventFlags_Tails_Chaos4Clear = 0xE7,
	EventFlags_Tails_EggWalkerClear = 0xE8,
	EventFlags_Tails_EggHornetClear = 0xE9,
	EventFlags_Tails_SkyChaseAct1Clear = 0xEA,
	EventFlags_Tails_SkyChaseAct2Clear = 0xEB,
	EventFlags_Tails_EmeraldCoastClear = 0xEC,
	EventFlags_Tails_RedMountainClear = 0xED,
	EventFlags_Knuckles_CasinopolisOpen = 0x103,
	EventFlags_Knuckles_EggCarrierSunk = 0x10F,
	EventFlags_Knuckles_ShovelClaw = 0x119,
	EventFlags_Knuckles_FightingGloves = 0x11A,
	EventFlags_Knuckles_CasinopolisClear = 0x11F,
	EventFlags_Knuckles_SpeedHighwayClear = 0x120,
	EventFlags_Knuckles_RedMountainClear = 0x121,
	EventFlags_Knuckles_LostWorldClear = 0x122,
	EventFlags_Knuckles_Chaos2Clear = 0x123,
	EventFlags_Knuckles_Chaos6Clear = 0x124,
	EventFlags_Knuckles_Chaos4Clear = 0x125,
	EventFlags_Knuckles_SkyDeckClear = 0x126,
	EventFlags_Amy_EggCarrierSunk = 0x14D,
	EventFlags_Amy_WarriorFeather = 0x152,
	EventFlags_Amy_TwinkleParkClear = 0x156,
	EventFlags_Amy_HotShelterClear = 0x157,
	EventFlags_Amy_FinalEggClear = 0x158,
	EventFlags_Amy_ZeroClear = 0x159,
	EventFlags_Amy_LongHammer = 0x15E,
	EventFlags_Gamma_JetBooster = 0x189,
	EventFlags_Gamma_LaserBlaster = 0x18A,
	EventFlags_Gamma_EggCarrierSunk = 0x18C,
	EventFlags_Gamma_WindyValleyOpen = 0x190,
	EventFlags_Gamma_EmeraldCoastClear = 0x19B,
	EventFlags_Gamma_WindyValleyClear = 0x19C,
	EventFlags_Gamma_RedMountainClear = 0x19D,
	EventFlags_Gamma_HotShelterClear = 0x19E,
	EventFlags_Gamma_FinalEggClear = 0x19F,
	EventFlags_Gamma_E101Clear = 0x1A0,
	EventFlags_Gamma_E101mkIIClear = 0x1A1,
	EventFlags_Big_EggCarrierSunk = 0x1CB,
	EventFlags_Big_LifeRing = 0x1D0,
	EventFlags_Big_PowerRod = 0x1D1,
	EventFlags_Big_EmeraldCoastClear = 0x1D5,
	EventFlags_Big_HotShelterClear = 0x1D6,
	EventFlags_Big_TwinkleParkClear = 0x1D7,
	EventFlags_Big_IceCapClear = 0x1D8,
	EventFlags_Big_Chaos6Clear = 0x1D9
};

enum TimesOfDay
{
	TimesOfDay_Day,
	TimesOfDay_Evening,
	TimesOfDay_Night
};

enum ChaoTypes : __int8
{
	ChaoTypes_Null,
	ChaoTypes_Egg,
	ChaoTypes_Child,
	ChaoTypes_ChildNormal,
	ChaoTypes_ChaosNormal,
	ChaoTypes_NeutralNormal,
	ChaoTypes_HeroNormal,
	ChaoTypes_DarkNormal,
	ChaoTypes_NeutralSwim,
	ChaoTypes_HeroSwim,
	ChaoTypes_DarkSwim,
	ChaoTypes_NeutralFly,
	ChaoTypes_HeroFly,
	ChaoTypes_DarkFly,
	ChaoTypes_NeutralRun,
	ChaoTypes_HeroRun,
	ChaoTypes_DarkRun,
	ChaoTypes_NeutralPower,
	ChaoTypes_HeroPower,
	ChaoTypes_DarkPower,
	ChaoTypes_NeutralChaos,
	ChaoTypes_HeroChaos,
	ChaoTypes_DarkChaos,
	ChaoTypes_Tails,
	ChaoTypes_Knuckles,
	ChaoTypes_Amy
};

/*  136 */
enum ChaoGardens : __int8
{
	ChaoGardens_None,
	ChaoGardens_StationSquare = 4,
	ChaoGardens_EggCarrier,
	ChaoGardens_MysticRuins,
};

/*  137 */
enum ChaoModels : __int8
{
	ChaoModels_Normal,
	ChaoModels_EggChao,
	ChaoModels_Omochao
};

/*  138 */
enum ChaoParts : __int8
{
	ChaoParts_Seal,
	ChaoParts_Swallow,
	ChaoParts_Otter,
	ChaoParts_Peacock,
	ChaoParts_Mole,
	ChaoParts_Parrot,
	ChaoParts_Deer,
	ChaoParts_Rabbit,
	ChaoParts_Kangaroo,
	ChaoParts_Gorilla,
	ChaoParts_Lion,
	ChaoParts_Elephant,
	ChaoParts_Penguin,
	ChaoParts_Koala,
	ChaoParts_Skunk,
	ChaoParts_Normal = -1
};

enum MissionFlagBits
{
	MissionFlagBits_Started,
	MissionFlagBits_Found = 6,
	MissionFlagBits_Complete
};

enum MissionFlags
{
	makemasks(MissionFlag, Started),
	makemasks(MissionFlag, Found),
	makemasks(MissionFlag, Complete)
};

enum LandTableFlagBits
{
	LandTableFlagBits_Animation,
	LandTableFlagBits_TextureList,
	LandTableFlagBits_TextureName = 3
};

enum LandTableFlags
{
	makemasks(LandTableFlag, Animation),
	makemasks(LandTableFlag, TextureList),
	makemasks(LandTableFlag, TextureName)
};

enum NPCTextControl : short
{
	NPCTextControl_EventFlag = -7,
	NPCTextControl_NPCFlag,
	NPCTextControl_Character,
	NPCTextControl_Voice,
	NPCTextControl_SetEventFlag,
	NPCTextControl_NewGroup,
	NPCTextControl_End
};

enum ButtonBits
{
	ButtonBits_B = 1,
	ButtonBits_A,
	ButtonBits_Start,
	ButtonBits_Up,
	ButtonBits_Down,
	ButtonBits_Left,
	ButtonBits_Right,
	ButtonBits_Z,
	ButtonBits_Y,
	ButtonBits_X,
	ButtonBits_R = 16,
	ButtonBits_L
};

enum Buttons : int
{
	makemasks(Button, B),
	makemasks(Button, A),
	makemasks(Button, Start),
	makemasks(Button, Up),
	makemasks(Button, Down),
	makemasks(Button, Left),
	makemasks(Button, Right),
	makemasks(Button, Z),
	makemasks(Button, Y),
	makemasks(Button, X),
	makemasks(Button, R),
	makemasks(Button, L)
};

// SADX Structs
typedef NJS_VECTOR Vertex, Vector3;
typedef NJS_TEXLIST TexList;

#pragma pack(push, 1)
typedef struct Rotation {
	int x, y, z;
} Rotation3;

struct ObjectMaster;
typedef void (__cdecl *ObjectFuncPtr)(ObjectMaster *);

struct FogData
{
	float Layer;
	float Distance;
	int Color;
	int Toggle;
};

struct PVMEntry
{
	char *Name;
	TexList *TexList;
};

struct LevelPVMList
{
	__int16 Level;
	__int16 NumTextures;
	PVMEntry *PVMList;
};

struct ObjectListEntry
{
	char Arg1;
	char Arg2;
	__int16 UseDistance;
	float Distance;
	int field_8;
	ObjectFuncPtr LoadSub;
	char *Name;
};

struct StartPosition
{
	__int16 LevelID;
	__int16 ActID;
	Vector3 Position;
	int YRot;
};

struct ControllerData
{
	int field_0;
	int field_4;
	int HeldButtons;
	int NotHeldButtons;
	int PressedButtons;
	int ReleasedButtons;
	__int16 CameraRight;
	__int16 CameraLeft;
	__int16 ControlX;
	__int16 ControlY;
	__int16 CameraX;
	__int16 CameraY;
	int field_24;
	int field_28;
	int HeldButtons_Copy;
	int field_30;
};

struct SET_Entry
{
	__int16 ObjectType;
	__int16 XRotation;
	__int16 YRotation;
	__int16 ZRotation;
	Vector3 Position;
	Vector3 Properties;
};

struct RecapScreen
{
	float Speed;
	int LineCount;
	char **TextData;
};

struct COL
{
	Vector3 Center;
	float Radius;
	int Padding[2];
	NJS_OBJECT *OBJECT;
	int anonymous_6;
	int Flags;
};

struct GeoAnimData
{
	int anonymous_0;
	float anonymous_1;
	float anonymous_2;
	NJS_OBJECT *Model;
	NJS_ACTION *Animation;
	TexList *TexList;
};

struct LandTable
{
	__int16 COLCount;
	__int16 AnimCount;
	int Flags;
	float Unknown_1;
	COL *COL;
	GeoAnimData *AnimData;
	char *TexName;
	TexList *TexList;
	int Unknown_4;
	int Unknown_5;
};

struct P2Path
{
	int field_0;
	Vector3 Position;
};

struct Loop
{
	__int16 Ang_X;
	__int16 Ang_Y;
	float Dist;
	Vector3 Position;
};

struct LoopHead
{
	__int16 Unknown_0;
	__int16 Count;
	float TotalDist;
	Loop *LoopList;
	ObjectFuncPtr Object;
};

struct AnimData
{
	NJS_ACTION *Animation;
	char field_4;
	char Property;
	__int16 NextAnim;
	float TransitionSpeed;
	float AnimationSpeed;
};

struct PhysicsData
{
	int HangTime;
	float FloorGrip;
	float HSpeedCap;
	float VSpeedCap;
	float MaxAccel;
	float field_14;
	float JumpSpeed;
	float SpringControl;
	float field_20;
	float RollCancel;
	float RollEnd;
	float Run1;
	float Knockback;
	float Run2;
	float JumpAddSpeed;
	float GroundAccel;
	float AirAccel;
	float GroundDecel;
	float Brake;
	float AirBrake;
	float AirDecel;
	float RollDecel;
	float GravityAdd;
	float HitSpeed;
	float MinSpeed;
	float field_64;
	float field_68;
	float field_6C;
	float RippleSize;
	float CollisionSize;
	float Gravity;
	float CameraY;
	float YOff;
};

struct CharObj2
{
	float SpindashSpeed;
	__int16 Upgrades;
	__int16 Powerups;
	int field_8;
	__int16 UnderwaterTime;
	__int16 IdleTime;
	char gap_10[8];
	float LoopDist;
	char gap_1c[28];
	float HSpeed;
	float VSpeed;
	char gap_40[16];
	Vector3 field_50;
	char gap_5c[12];
	ObjectMaster *ObjectHeld;
	char gap_6c[48];
	PhysicsData PhysicsData;
	char gap_120[4];
	__int16 Animation;
	char gap_126[26];
	AnimData *AnimData;
	NJS_OBJECT **field_144;
	char gap_148[148];
	int field_1DC;
	char gap_1e0[4];
	int field_1E4;
	char gap_1e8[20];
	float field_1FC;
};

struct ChaoData
{
	char gap_0[18];
	char field_12[7];
	char gap_19[7];
	char SwimFraction;
	char FlyFraction;
	char RunFraction;
	char PowerFraction;
	char StaminaFraction;
	char gap_25[3];
	char SwimGrade;
	char FlyGrade;
	char RunGrade;
	char PowerGrade;
	char StaminaGrade;
	char gap_2d[3];
	char SwimLevel;
	char FlyLevel;
	char RunLevel;
	char PowerLevel;
	char StaminaLevel;
	char gap_35[3];
	__int16 SwimStat;
	__int16 FlyStat;
	__int16 RunStat;
	__int16 PowerStat;
	__int16 StaminaStat;
	char gap_42[62];
	ChaoTypes Type;
	ChaoGardens Garden;
	char Reincarnate;
	char gap_83[3];
	char ClockRollovers;
	char gap_87[3];
	__int16 LifeTimeLeft;
	char gap_8c[28];
	float PowerRun;
	float FlySwim;
	float HeroDark;
	char gap_b4[12];
	float EvolutionProgress;
	char gap_c4[13];
	char EyeType;
	char MouthType;
	char BallType;
	char gap_d4[1];
	char Headgear;
	char HideFeet;
	char Medal;
	char Color;
	char Highlights;
	char Texture;
	char Shiny;
	char EggColor;
	char Model;
	char gap_de[81];
	char Happiness;
	char gap_130[11];
	char Mating;
	char gap_13c[936];
	char ArmType;
	char EarType;
	char EyebrowType;
	char ForeheadType;
	char LegType;
	char TailType;
	char WingType;
	char gap_4eb[17];
	char SonicLike;
	char gap_4fd[5];
	char TailsLike;
	char gap_503[5];
	char KnucklesLike;
	char gap_509[5];
	char AmyLike;
	char gap_50f[5];
	char BigLike;
	char gap_515[5];
	char GammaLike;
	char gap_51b[740];
	char field_7FF;
};

struct CharObj1
{
	char Action;
	char NextAction;
	__int16 field_2;
	__int16 Status;
	__int16 InvulnerableTime;
	char CharIndex;
	char CharID;
	char gap_a[4];
	char field_E;
	char field_F;
	Loop *LoopData;
	Rotation3 Rotation;
	Vector3 Position;
	Vector3 Scale;
	void *Ptr3;
	char gap_3c[44];
	int field_68;
	ChaoData *field_6C;
	char gap_70[80];
	CharObj2 *Ptr2;
};

struct ObjectMaster
{
	ObjectMaster *PreviousSibling;
	ObjectMaster *NextSibling;
	ObjectMaster *Parent;
	ObjectMaster *Child;
	ObjectFuncPtr MainSub;
	ObjectFuncPtr DisplaySub;
	ObjectFuncPtr DeleteSub;
	void *field_1C;
	CharObj1 *Data1;
	void *field_24;
	void *field_28;
	void *field_2C;
	int field_30;
};

struct VideoData
{
	char *Filename;
	int field_4;
	int NumFrames;
	__int16 Width;
	__int16 Height;
};

struct PathDataPtr
{
	int LevelAct;
	LoopHead **PathList;
};

struct ObjectList
{
	int Count;
	ObjectListEntry *List;
};

struct LevelItemAccess
{
	int Character;
	int Level;
};

struct CharSelData
{
	ObjectFuncPtr CodePointer;
	TexList *TextureList;
	NJS_ACTION **anonymous_1;
	int *anonymous_2;
};

struct CharacterLightData
{
	float field_0;
	char anonymous_0;
	char anonymous_1;
	char anonymous_2;
	char anonymous_3;
	float anonymous_4;
	float anonymous_5;
	float anonymous_6;
	float anonymous_7;
	float anonymous_8;
	float anonymous_9;
	float anonymous_10;
	float anonymous_11;
	float anonymous_12;
	float anonymous_13;
};

struct SecondaryEntrance
{
	Vector3 Position;
	int YRot;
};

struct TextureAnimationData
{
	NJS_OBJECT *Model;
	char MatCount;
	char MatIDs[8];
	char TexID;
	__int16 TexTime;
};

struct LevelClearFlagData
{
	__int16 Level;
	__int16 FlagOffset;
};

struct NextLevelData
{
	char CGMovie;
	char CurrentLevel;
	char NextLevelAdventure;
	char NextActAdventure;
	char StartPointAdventure;
	char AltNextLevel;
	char AltNextAct;
	char AltStartPoint;
};

struct FieldStartPosition
{
	__int16 LevelID;
	__int16 FieldID;
	Vector3 Position;
	int YRot;
};

struct LevelCutsceneData
{
	char Level;
	char Act;
	__int16 Character;
	char field_4;
	char field_5;
	__int16 Cutscene;
	char field_8;
	char field_9;
	char field_A;
	char field_B;
};

struct CutsceneData
{
	ObjectFuncPtr Function;
	void *Textures;
};

struct HintText_Text
{
	char *Message;
	int Time;
};

struct HintText_Entry
{
	__int16 *Properties;
	HintText_Text *Text;
};

struct AdventureData
{
	char TimeOfDay;
	char field_1;
	__int16 field_2;
	__int16 field_4;
	__int16 Entrance;
	__int16 LevelAct;
	__int16 field_A;
};

struct SceneSelectData
{
	void *Code;
	char Level;
	char Act;
	__int16 Entrance;
	char Name[32];
};

struct TrialLevelListEntry
{
	char Level;
	char Act;
};

struct TrialLevelList
{
	TrialLevelListEntry *Levels;
	int Count;
};

struct SoundFileInfo
{
	int Bank;
	char *Filename;
};

struct SoundList
{
	int Count;
	SoundFileInfo *List;
};

struct ItemBoxData
{
	int Texture;
	ObjectFuncPtr Function;
};

struct Time
{
	char Minutes;
	char Seconds;
	char Frames;
};

struct TwinkleCircuitTimes
{
	Time BestTimes[3];
	Time Lap1Time;
	Time Lap2Time;
};

struct SaveFileData
{
	int Checksum;
	int PlayTime;
	int HighScores[32];
	Time BestTimes[28];
	__int16 BestWeights[12];
	__int16 anonymous_4[8];
	__int16 MostRings[32];
	int SkyChase1HighScores[6];
	int SkyChase2HighScores[6];
	int IceCapHighScores[6];
	int SandHillHighScores[6];
	int HedgehogHammerHighScore1;
	int HedgehogHammerHighScore2;
	int HedgehogHammerHighScore3;
	TwinkleCircuitTimes TwinkleCircuitBestTimes[6];
	Time BossBestTimes[18];
	char Emblems[17];
	char Options;
	char Lives[7];
	char LastCharacter;
	char Rumble;
	char gap_25b[1];
	__int16 LastLevel;
	char gap_25e[2];
	char EventFlags[64];
	char NPCFlags[64];
	char gap_2e0[8];
	AdventureData AdventureData[8];
	char LevelClear[344];
	char MissionFlags[60];
	int BlackMarketRings;
	int MetalHighScores[10];
	Time MetalBestTimes[10];
	__int16 MetalMostRings[10];
	char gap_53a[2];
	int MetalIceCapHighScores[3];
	int MetalSandHillHighScores[3];
	TwinkleCircuitTimes MetalTwinkleCircuitBestTimes;
	Time MetalBossBestTimes[3];
	int MetalEmblems;
};

struct DrawDistance
{
	float Minimum;
	float Maximum;
};

struct RestartData
{
	__int16 Level;
	char Minute;
	char Second;
	char Frame;
	char anonymous_0[3];
	int CheckpointHit;
	Vector3 Position;
	Rotation3 Rotation;
};

struct MESFileText
{
	__int16 *field_0;
	char **field_4;
};

struct MESFileHeader
{
	int Count;
	MESFileText *Texts;
};

struct FieldNPCDataYRot
{
	Vector3 Position;
	__int16 YRot;
	char TextNum;
	char CharacterNum;
};

struct FieldNPCDataXYZRot
{
	Vector3 Position;
	__int16 XRot;
	__int16 YRot;
	__int16 ZRot;
	char TextNum;
	char CharacterNum;
};

struct FieldNPCList
{
	void *Data;
	int Count;
};

struct SETObjData
{
	char LoadCount;
	char f1;
	__int16 Flags;
	int dword4;
	SET_Entry *SETEntry;
	float Distance;
};

struct MusicInfo
{
	char *Name;
	int Loop;
};

struct SoundTestEntry
{
	char *Name;
	int ID;
};

struct SoundTestCategory
{
	SoundTestEntry *Entries;
	int Count;
};

struct DeathZone
{
	int Characters;
	NJS_OBJECT *Model;
};

struct SkyboxScale
{
	Vertex Far, Normal, Near;
};
#pragma pack(pop)

// SADX Variables
#define DataPointer(type,name,address) static type &name = *(type *)address
#define DataArray(type,name,address,length) static type *const name = (type *)address; static const int name##_Length = length
DataArray(TrialLevelList, TrialLevels, 0x7EF5EC, 7);
DataArray(TrialLevelList, TrialSubgames, 0x7EF624, 7);
DataArray(SoundTestCategory, SoundtestData, 0x7EFEB8, 6);
DataArray(CharSelData, CharSelDataList, 0x7F0788, 7);
DataPointer(float, HorizontalStretch, 0x8928C0);
DataPointer(float, VerticalStretch, 0x8928C4);
DataPointer(int, HorizontalResolution, 0x892990);
DataPointer(int, VerticalResolution, 0x892994);
DataArray(PVMEntry *, TexLists_Obj, 0x90EB68, 44);
DataArray(PVMEntry *, TexLists_ObjRegular, 0x90EC70, 3);
DataArray(PVMEntry *, TexLists_Characters, 0x90ED54, 9);
DataArray(LevelPVMList *, TexLists_Level, 0x90F010, 59);
DataArray(char *, CharIDStrings, 0x90F424, 9);
DataArray(SoundList, SoundLists, 0x90FCB8, 123);
DataArray(MusicInfo, MusicList, 0x910298, 125);
DataArray(PhysicsData, PhysicsArray, 0x9154E8, 8);
DataArray(PVMEntry *, CharSel_PVMNames, 0x10DB88C, 8);
DataPointer(int, GameMode, 0x3ABDC7C);
DataPointer(char, LastStoryFlag, 0x3B18DB4);
DataPointer(char, MetalSonicFlag, 0x3B18DB5);
DataPointer(short, ObjectPlacementDebug, 0x3B29D08);

// SADX Functions
#define FunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) static RETURN_TYPE (__cdecl *const NAME)ARGS = (RETURN_TYPE (*)ARGS)ADDRESS
#define FastcallFunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) static RETURN_TYPE (__fastcall *const NAME)ARGS = (RETURN_TYPE (__fastcall *)ARGS)ADDRESS
#define ThiscallFunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) static RETURN_TYPE (__thiscall *const NAME)ARGS = (RETURN_TYPE (__thiscall *)ARGS)ADDRESS
#define VoidFunc(name,address) FunctionPointer(void,name,(void),address)
#define ObjectFunc(name,address) FunctionPointer(void,name,(ObjectMaster *obj),address)
FunctionPointer(int, GetEventFlag, (int flag), 0x412D20);
FunctionPointer(int, CountUnlockedCharacters, (), 0x512C50);
#endif