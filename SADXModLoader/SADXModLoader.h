#include "ninja.h"
#include <d3d9.h>
#ifndef SADXMODLOADER_H
#define SADXMODLOADER_H

#ifdef _MSC_VER
// MSVC doesn't have snprintf(), but it does have _snprintf().
#define snprintf(str, size, format, ...) _snprintf(str, size, format, __VA_ARGS__)
#endif

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

static BOOL WriteData(void *writeaddress, const void *data, SIZE_T datasize, SIZE_T *byteswritten)
{
	return WriteProcessMemory(GetCurrentProcess(), writeaddress, data, datasize, byteswritten);
}

static BOOL WriteData(void *writeaddress, const void *data, SIZE_T datasize)
{
	return WriteData(writeaddress, data, datasize, nullptr);
}

template<typename T> static BOOL WriteData(T const *writeaddress, const T data, SIZE_T *byteswritten)
{
	return WriteData((void*)writeaddress, (void*)&data, (SIZE_T)sizeof(data), byteswritten);
}

template<typename T> static BOOL WriteData(T const *writeaddress, const T data)
{
	return WriteData(writeaddress, data, nullptr);
}

template<typename T> static BOOL WriteData(T *writeaddress, const T &data, SIZE_T *byteswritten)
{
	return WriteData(writeaddress, &data, sizeof(data), byteswritten);
}

template<typename T> static BOOL WriteData(T *writeaddress, const T &data)
{
	return WriteData(writeaddress, data, nullptr);
}

template <typename T, size_t N> static BOOL WriteData(void *writeaddress, const T(&data)[N], SIZE_T *byteswritten)
{
	return WriteData(writeaddress, data, SizeOfArray(data), byteswritten);
}

template <typename T, size_t N> static BOOL WriteData(void *writeaddress, const T(&data)[N])
{
	return WriteData(writeaddress, data, nullptr);
}

static BOOL WriteData(void *address, const char data, int count, SIZE_T *byteswritten)
{
	char *buf = new char[count];
	memset(buf, data, count);
	int result = WriteData(address, buf, count, byteswritten);
	delete[] buf;
	return result;
}

static BOOL WriteData(void *address, char data, int count)
{
	return WriteData(address, data, count, nullptr);
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

static void ResizeTextureList(NJS_TEXLIST *texlist, Uint32 count)
{
	texlist->textures = new NJS_TEXNAME[count];
	texlist->nbTexture = count;
}

static void ResizeTextureList(NJS_TEXLIST *texlist, NJS_TEXNAME *textures, Uint32 count)
{
	texlist->textures = textures;
	texlist->nbTexture = count;
}

template <Uint32 N>
static void ResizeTextureList(NJS_TEXLIST *texlist, NJS_TEXNAME(&textures)[N])
{
	ResizeTextureList(texlist, textures, N);
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

enum ChaoTypes
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
enum ChaoGardens
{
	ChaoGardens_None,
	ChaoGardens_StationSquare = 4,
	ChaoGardens_EggCarrier,
	ChaoGardens_MysticRuins,
};

/*  137 */
enum ChaoModels
{
	ChaoModels_Normal,
	ChaoModels_EggChao,
	ChaoModels_Omochao
};

/*  138 */
enum ChaoParts
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

enum NPCTextControl
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
typedef NJS_TEXLIST TexList, TexListHead;
typedef NJS_OBJECT OBJECT;
typedef NJS_MODEL ATTACH;

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
	char gap_126[10];
	float AnimationFrame;
	char gap_134[12];
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

struct StageLightData
{
	char level;
	char act;
	char light_num;
	char use_yxz;
	float xyz[3];
	float dif;
	float mutliplier;
	float rgb[3];
	float amb_rgb[3];
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
DataPointer(NJS_VECTOR, Gravity, 0x3B0F0F8);
DataPointer(int, LevelFrameCount, 0x3B0F108);
DataPointer(int, FrameCounter, 0x3B0F13C);
DataPointer(char, LastStoryFlag, 0x3B18DB4);
DataPointer(char, MetalSonicFlag, 0x3B18DB5);
DataPointer(short, CurrentCharacter, 0x3B22DC0);
DataPointer(short, CurrentLevel, 0x3B22DCC);
DataPointer(char, CurrentAct, 0x3B22DEC);
DataPointer(short, ObjectPlacementDebug, 0x3B29D08);
DataPointer(RestartData, RestartLevel, 0x3B42F80);

// SADX Functions
#define FunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) static RETURN_TYPE (__cdecl *const NAME)ARGS = (RETURN_TYPE (__cdecl *)ARGS)ADDRESS
#define StdcallFunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) static RETURN_TYPE (__stdcall *const NAME)ARGS = (RETURN_TYPE (__stdcall *)ARGS)ADDRESS
#define FastcallFunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) static RETURN_TYPE (__fastcall *const NAME)ARGS = (RETURN_TYPE (__fastcall *)ARGS)ADDRESS
#define ThiscallFunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) static RETURN_TYPE (__thiscall *const NAME)ARGS = (RETURN_TYPE (__thiscall *)ARGS)ADDRESS
#define VoidFunc(NAME, ADDRESS) FunctionPointer(void,NAME,(void),ADDRESS)
#define ObjectFunc(NAME, ADDRESS) FunctionPointer(void,NAME,(ObjectMaster *obj),ADDRESS)

FunctionPointer(void, PrintDebug, (char *Format, ...), 0x401000);
VoidFunc(CheckSettings, 0x4025B0);
FunctionPointer(void, SetTextures, (TexListHead *texturelist), 0x403070);
FunctionPointer(bool, CheckModelDistance, (Vector3 *center, float radius), 0x403330);
FunctionPointer(bool, CheckModelDistance2, (float x, float y, float z, float a4), 0x406F70);
FunctionPointer(void, ProcessModelNode, (OBJECT *obj, int a2, float a3), 0x4074A0);
FunctionPointer(void, DoTimeOfDayLighting, (int *a1, int *a2), 0x40A4A0);
FunctionPointer(void, RunObjectIndex, (int index), 0x40B0C0);
ObjectFunc(DisplayObject, 0x40B130);
FunctionPointer(void *, AllocateMemory, (int size), 0x40B220);
FunctionPointer(void, DisplayObjectIndex, (int index), 0x40B4F0);
ObjectFunc(DeleteObject_, 0x40B570);
VoidFunc(DeleteMostObjects, 0x40B6E0);
VoidFunc(DeleteAllObjects, 0x40B730);
FunctionPointer(ObjectMaster *, LoadObject, (char flags, int index, void (__cdecl *loadSub)(ObjectMaster *)), 0x40B860);
FunctionPointer(ObjectMaster *, LoadChildObject, (char a1, void (__cdecl *address)(ObjectMaster *), ObjectMaster *parent), 0x40B940);
FunctionPointer(void, PlayMusicFile, (LPCSTR filename, int loop), 0x40CD20);
FunctionPointer(int, PlayVoiceFile, (LPCSTR filename), 0x40CE30);
VoidFunc(Control, 0x40FDC0);
StdcallFunctionPointer(DWORD, StartAddress, (LPVOID), 0x40FF50);
FunctionPointer(void, SetTimeOfDay, (char a1), 0x412C00);
FunctionPointer(int, GetCharacterUnlockedAdventure, (Characters character), 0x412C30);
FunctionPointer(void, UnlockCharacterAdventure, (int character), 0x412CA0);
FunctionPointer(void, SetEventFlag, (EventFlags a1), 0x412D00);
FunctionPointer(void, ClearEventFlag, (EventFlags a1), 0x412D10);
FunctionPointer(bool, GetEventFlag, (EventFlags offset), 0x412D20);
FunctionPointer(void, SetLevelClear, (LevelIDs level), 0x412E20);
VoidFunc(LoadFieldNPCs, 0x412E90);
VoidFunc(LoadLevelObject, 0x4143C0);
VoidFunc(LoadSkyboxObject, 0x414420);
FunctionPointer(int, SetCharacter, (__int16 character), 0x4144D0);
FunctionPointer(Characters, GetCurrentCharacterID, (), 0x4144E0);
VoidFunc(SetStartPos_ReturnToField, 0x414500);
FunctionPointer(int, SetLevelAndAct, (char Level, char Act), 0x414570);
FunctionPointer(int, SetNextLevelAndAct, (char Level, char Act), 0x4145B0);
FunctionPointer(int, GoToNextLevel, (), 0x414610);
FunctionPointer(int, GetPrevLevelAndAct, (), 0x414670);
FunctionPointer(int, GetNextLevelAndAct, (), 0x414690);
FunctionPointer(bool, IsLevelChaoGarden, (), 0x4146B0);
FunctionPointer(int, SetLevelEntrance, (char num), 0x4147F0);
FunctionPointer(int, GetLevelEntranceID, (), 0x414800);
FunctionPointer(void, SetNextLevel, (unsigned __int16 level), 0x414B00);
FunctionPointer(bool, IsGamePaused, (), 0x414D70);
VoidFunc(LoadLevelResults, 0x415540);
VoidFunc(LoadCharacter, 0x4157C0);
FunctionPointer(void, UnloadCharTextures, (int Character), 0x420E90);
FunctionPointer(int, UnloadLevelTextures, (__int16 levelact), 0x421040);
FunctionPointer(int, LoadPVM, (char *PVMName, TexListHead *TexList), 0x421180);
VoidFunc(LoadObjectTextures, 0x4213A0);
FunctionPointer(void, LoadLevelTextures, (__int16 level), 0x4215B0);
VoidFunc(WriteSaveFile, 0x421FD0);
FunctionPointer(void, LoadFile, (char *name, LPVOID lpBuffer), 0x422200);
FunctionPointer(void, LoadFileWithMalloc, (char *Name, LPVOID *Data), 0x422310);
VoidFunc(ReleaseCamFile, 0x4224B0);
VoidFunc(LoadLevelFiles, 0x422AD0);
FunctionPointer(void, LoadSoundList, (signed int soundlist), 0x4238E0);
FunctionPointer(int, PlaySound, (int ID, int, int, int), 0x423D70);
FunctionPointer(signed int, PlaySound2, (int ID, int a2, int a3, int a4), 0x423E20);
ObjectFunc(SoundManager_Main, 0x4253D0);
ObjectFunc(SoundManager_Delete, 0x425620);
FunctionPointer(void, PlayMusic, (int song), 0x425690);
FunctionPointer(void, PlayVoice, (int a1), 0x425710);
ObjectFunc(SoundManager_Load, 0x425830);
FunctionPointer(bool, GetDebugMode, (), 0x4258F0);
VoidFunc(Set0Rings, 0x425AB0);
FunctionPointer(int, GetMarketRings, (), 0x425AC0);
FunctionPointer(int, ResetLives, (), 0x425AF0);
FunctionPointer(void, GiveLives, (__int16 lives), 0x425B60);
FunctionPointer(void, AddRings, (__int16 rings), 0x425BE0);
FunctionPointer(int, GetRings, (), 0x425CC0);
VoidFunc(IncrementFrameCounter, 0x426050);
FunctionPointer(void, GetTime, (char &minutes, char &seconds), 0x4265B0);
FunctionPointer(void, SetTime, (char minutes, char seconds), 0x4265D0);
FunctionPointer(void, SetTime2, (char minute, char second, char frame), 0x4265F0);
FunctionPointer(void, GetTime2, (char &minutes, char &seconds, char &frames), 0x426610);
FunctionPointer(void, AddSeconds, (int seconds), 0x426640);
FunctionPointer(bool, IsTime0, (), 0x426690);
FunctionPointer(bool, CheckSaveFile, (SaveFileData *a2), 0x42D0B0);
ThiscallFunctionPointer(void, WriteAdventureDataToSaveFile, (void *data), 0x42D200);
FunctionPointer(void, DisplayDialogueText, (char *text), 0x42FB20);
FunctionPointer(ObjectMaster *, j_GetCharacterObject, (unsigned __int8 a1), 0x42FC40);
FunctionPointer(void, LoadEventCharacter, (unsigned __int8 charID, void (__cdecl *code)(ObjectMaster *), float xpos, float ypos, float zpos, int xrot, int yrot, int zrot), 0x42FD80);
FunctionPointer(void, LoadEventObject, (ObjectMaster **a1, ObjectMaster *(*a2)(void), float a3, float a4, float a5, int a6, int a7, int a8), 0x4316C0);
ObjectFunc(MainMenu_Display, 0x432480);
ObjectFunc(MainMenu_Main, 0x432720);
ObjectFunc(Camera_Main, 0x438090);
FunctionPointer(void, ResetGeoPointer, (int Level, int Act), 0x43A4A0);
FunctionPointer(int, IsPlayerInsideSphere, (NJS_VECTOR *position, float radius), 0x441840);
FunctionPointer(int, GetCharacterID, (char index), 0x441970);
FunctionPointer(ObjectMaster *, GetCharacterObject, (unsigned __int8 character), 0x441AC0);
FunctionPointer(CharObj2 *, GetCharObj2, (char index), 0x441B00);
FunctionPointer(void, GiveMagneticBarrier, (char character), 0x441E30);
FunctionPointer(void, GiveBarrier, (char character), 0x441EA0);
FunctionPointer(void, GiveInvincibility, (int character), 0x441F10);
FunctionPointer(void, InitCharacterVars, (int ID, ObjectMaster *character), 0x442750);
FunctionPointer(void, RestartCheckpoint, (Vector3 *Position, Rotation3 *Rotation), 0x44EDB0);
FunctionPointer(void, SetRestartData, (Vector3 *Position, Rotation3 *Rotation), 0x44EE70);
ObjectFunc(CheckPoint_Display, 0x44EEF0);
ObjectFunc(CheckPoint_Main, 0x44F540);
ObjectFunc(Ring_Display, 0x44F6B0);
ObjectFunc(Ring_Main, 0x450370);
FunctionPointer(void, HurtCharacter, (char a1), 0x4506F0);
FunctionPointer(signed int, GetMissionType, (int character, signed int a2), 0x4575D0);
ObjectFunc(Tails_Delete, 0x45BEA0);
ObjectFunc(Snowboard__Tails_Main, 0x461510);
ObjectFunc(Tails_Main, 0x461700);
ObjectFunc(Capsule_Display, 0x46ADB0);
ObjectFunc(Capsule_Delete, 0x46AE30);
ObjectFunc(Capsule_Main, 0x46B130);
ObjectFunc(Capsule_Load, 0x46B170);
VoidFunc(LoadSETObjs_P2NotTailsAndNotDebug, 0x46B7B0);
VoidFunc(LoadSETObjs_NoP2OrDebugOrP2Tails, 0x46BA40);
VoidFunc(LoadSETObjs, 0x46BCE0);
ObjectFunc(Knuckles_Main, 0x47A770);
FunctionPointer(bool, IsFastSonicAI, (), 0x47C200);
FunctionPointer(void, SetTailsRaceVictory, (), 0x47C450);
FunctionPointer(void, SetOpponentRaceVictory, (), 0x47C460);
ObjectFunc(Sonic2PAI_Main, 0x47D640);
ObjectFunc(Eggman2PAI_Main, 0x47D750);
ObjectFunc(Sonic2PAI_Load, 0x47D820);
ObjectFunc(Eggman2PAI, 0x47D8C0);
ObjectFunc(TailsAI_Main, 0x47E8F0);
FunctionPointer(bool, Load2PTails, (ObjectMaster *player1), 0x47ED60);
ObjectFunc(Gamma_Main, 0x483430);
ObjectFunc(Amy_Main, 0x48ABF0);
ObjectFunc(Big_Main, 0x490A00);
FunctionPointer(void, Sonic_InitLightDash, (CharObj1 *a1, int a2, CharObj2 *a3), 0x492AB0);
FunctionPointer(void, LoadSonicDashTrail, (CharObj1 *a1), 0x494050);
ObjectFunc(Sonic_Super_Physics, 0x494350);
ObjectFunc(Sonic_Delete, 0x494860);
ObjectFunc(Sonic_Display, 0x4948C0);
ObjectFunc(Snowboard__Sonic_Main, 0x4959E0);
ObjectFunc(MetalSonic_AfterImages_Main, 0x495A50);
FunctionPointer(void, Sonic_Act1, (CharObj1 *obj1, int a2, CharObj2 *obj2), 0x496F50);
ObjectFunc(Sonic_Main, 0x49A9B0);
ObjectFunc(EmeraldPiece_Display, 0x4A2FB0);
ObjectFunc(EmeraldPiece_Main, 0x4A2FD0);
ObjectFunc(EmeraldPiece_Load, 0x4A3420);
ObjectFunc(Leon_Main, 0x4A85C0);
ObjectFunc(Spinner_Display, 0x4AFD80);
ObjectFunc(SpinnerA_Main, 0x4B0DF0);
ObjectFunc(SpinnerB_Main, 0x4B0F40);
ObjectFunc(SpinnerC_Main, 0x4B1090);
FunctionPointer(void, SetEmblemCollected, (SaveFileData *savefile, signed int index), 0x4B3F30);
FunctionPointer(int, CountEmblems, (SaveFileData *a1), 0x4B3F90);
FunctionPointer(int, GetEmblemBitIndex, (int character, signed int level, int mission), 0x4B3FC0);
ObjectFunc(DashPanel_Delete, 0x4B4160);
FunctionPointer(bool, GetLevelEmblemCollected, (SaveFileData *savefile, int character, signed int level, int mission), 0x4B45C0);
FunctionPointer(void, SetLevelEmblemCollected, (SaveFileData *savefile, int character, signed int level, int mission), 0x4B4640);
ObjectFunc(Emblem_Main, 0x4B4940);
FunctionPointer(signed int, GetHintText, (int id, int *data), 0x4B7C10);
FunctionPointer(OBJECT *, ProcessAnimatedModelNode, (OBJECT *a1, NJS_MDATA3 *a2), 0x4B7D00);
VoidFunc(ResetJumpPanels, 0x4B8320);
ObjectFunc(JumpPanel_Main, 0x4B8D10);
ObjectFunc(JumpPanel_Load, 0x4B8DC0);
ObjectFunc(MagneticBarrier_Display, 0x4B9D90);
ObjectFunc(Barrier_Display, 0x4B9F40);
ObjectFunc(Invincibility_Display, 0x4BA070);
ObjectFunc(MagneticBarrier_Main, 0x4BA2A0);
ObjectFunc(Barrier_Main, 0x4BA380);
ObjectFunc(Invincibility_Main, 0x4BA440);
ObjectFunc(MagneticBarrier_Load, 0x4BA550);
ObjectFunc(Barrier_Load, 0x4BA580);
ObjectFunc(Invincibility_Load, 0x4BA5B0);
ObjectFunc(LevelItem_Display, 0x4BF5A0);
ObjectFunc(LevelItem_Main, 0x4BF600);
ObjectFunc(LevelItem_Delete, 0x4BFDA0);
ObjectFunc(ItemBoxAir_Display, 0x4BFDD0);
ObjectFunc(ItemBoxAir_Main, 0x4C07D0);
ObjectFunc(Switch_Main, 0x4CBA80);
ObjectFunc(ZERO_Main, 0x4D2480);
ObjectFunc(ZERO_Load, 0x4D3E00);
ObjectFunc(ColSphere_Main, 0x4D4700);
ObjectFunc(ColCube_Main, 0x4D47E0);
ObjectFunc(Wall_Main, 0x4D4850);
ObjectFunc(BigWJump_Main, 0x4D4BE0);
FunctionPointer(void, SpeedShoesPowerup, (CharObj1 *a1), 0x4D6BF0);
FunctionPointer(void, InvincibilityPowerup, (CharObj1 *a1), 0x4D6D80);
ObjectFunc(ItemBox_Main, 0x4D6F10);
ObjectFunc(DelayedMusicPlayer_Main, 0x4DAA40);
ObjectFunc(Obj_WValley, 0x4DDB30);
ObjectFunc(SkyBox_WindyValley_Load, 0x4DDBF0);
ObjectFunc(Boss_Delete, 0x4E21D0);
ObjectFunc(E103Enemy_Display, 0x4E7AC0);
ObjectFunc(E103Enemy_Main, 0x4E7E90);
ObjectFunc(E103Enemy_Load, 0x4E7F50);
ObjectFunc(Snowboard_Load, 0x4E9660);
ObjectFunc(ACT12JOIN_Main, 0x4EBDF0);
ObjectFunc(ACT23JOIN_Main, 0x4EBEB0);
ObjectFunc(ChaosEmeGoal_Main, 0x4ECFA0);
ObjectFunc(Obj_ECoast, 0x4F6D60);
ObjectFunc(SkyBox_EmeraldCoast_Load, 0x4F7230);
FunctionPointer(int, ObjectSelectedDebug, (ObjectMaster *obj), 0x4F88A0);
ObjectFunc(Obj_EC1Water_Display, 0x501130);
ObjectFunc(Obj_EC1Water_Main, 0x5017D0);
ObjectFunc(Obj_EC1Water_Load, 0x501970);
FunctionPointer(bool, IsAdventureComplete, (int character), 0x5063E0);
ObjectFunc(CharSel_Qmark_Main, 0x5125A0);
FunctionPointer(void, CharSel, (int a1), 0x512910);
FunctionPointer(int, CountUnlockedCharacters, (), 0x512C50);
ObjectFunc(EC0NPC_Main, 0x51ACD0);
ObjectFunc(EC0NPC_Load, 0x51ADD0);
VoidFunc(LoadEC0NPCs, 0x51AE00);
VoidFunc(nullsub_1, 0x51F680);
ObjectFunc(EC3NPC_Main, 0x5254D0);
ObjectFunc(EC3NPC_Load, 0x5255D0);
VoidFunc(LoadEC3NPCs, 0x525600);
FunctionPointer(void, GetEntrance_ECarrier, (CharObj1 *a1), 0x52D820);
VoidFunc(LoadMRNPCs, 0x52F140);
FunctionPointer(void, GetEntrance_MRuins, (CharObj1 *a1), 0x530790);
ObjectFunc(MasterEme_Display, 0x53CC10);
ObjectFunc(MasterEme_Main, 0x53CF60);
ObjectFunc(ChaosEme_Display, 0x53D0B0);
ObjectFunc(ChaosEme_Main, 0x53D1B0);
VoidFunc(LoadPastNPCs, 0x541890);
FunctionPointer(void, GetEntrance_Past, (CharObj1 *a1), 0x542180);
ObjectFunc(ChaosEmePast_Display, 0x544560);
ObjectFunc(ChaosEmePast_Main, 0x5445D0);
ObjectFunc(MasterEmePast_Display, 0x544640);
ObjectFunc(MasterEmePast_Main, 0x5446B0);
ObjectFunc(Chaos0_Rain, 0x5462F0);
ObjectFunc(Chaos0_Main, 0x548640);
ObjectFunc(Chaos2_Main, 0x54DB90);
ObjectFunc(Chaos4_Main, 0x552960);
ObjectFunc(Chaos6_Main, 0x559FC0);
ObjectFunc(Sonic_SuperAura_Delete, 0x55E5B0);
ObjectFunc(Sonic_SuperAura_Display, 0x55E620);
ObjectFunc(Sonic_SuperAura_Main, 0x55E920);
ObjectFunc(Sonic_SuperAura_Load, 0x55FAF0);
ObjectFunc(PerfectChaos_Delete, 0x55FEB0);
ObjectFunc(PerfectChaos_Display, 0x55FED0);
ObjectFunc(PerfectChaos_Main, 0x560260);
ObjectFunc(AxelPanel_Main, 0x565AC0);
ObjectFunc(EggHornet_Main, 0x572010);
ObjectFunc(EggHornet_Missile_Main, 0x574E00);
ObjectFunc(EggHornet_Missile_Loader_Main, 0x5750F0);
ObjectFunc(EggWalker_Main, 0x576650);
ObjectFunc(E105Enemy_Load, 0x5A4990);
FunctionPointer(void, SpawnBeat, (ObjectMaster *a1, void *a2), 0x5B0590);
ObjectFunc(E104Enemy_Main, 0x605B40);
ObjectFunc(SkyBox_SpeedHighway_Load, 0x610A70);
ObjectFunc(SkyBox_TwinklePark_Load, 0x61D570);
ObjectFunc(Tornado_Delete, 0x627410);
ObjectFunc(Tornado_Display, 0x627430);
ObjectFunc(Tornado_Main, 0x627EC0);
ObjectFunc(Boss_Display, 0x62CC50);
ObjectFunc(Boss_Main, 0x62D140);
ObjectFunc(SSNPC_Main, 0x62F450);
ObjectFunc(SSNPC_Load, 0x62F5A0);
VoidFunc(LoadSSNPCs, 0x62F5D0);
FunctionPointer(void, GetEntrance_SSquare, (CharObj1 *a1), 0x62F6D0);
ObjectFunc(People_Main, 0x634980);
ObjectFunc(GoldEgg_Main, 0x6364C0);
ObjectFunc(FakeEgg_Main, 0x6364F0);
ObjectFunc(SSTrain_Main, 0x640430);
ObjectFunc(SceneChange_Main, 0x640850);
FunctionPointer(int, PRSDec, (char *a1, char *a2), 0x6FC900);
FunctionPointer(char *, GetChaoSaveAddress, (), 0x716FA0);
VoidFunc(SetupChaoSaveFile, 0x7172D0);
ObjectFunc(Chao_Main, 0x7202D0);
ObjectFunc(Chao_Display, 0x7204B0);
FunctionPointer(ObjectMaster *, CreateChao, (ChaoData *a2, int a3, void *a4, Vector3 *a5, int a6), 0x720670);
ObjectFunc(Chao_Tree_Main, 0x7212A0);
ObjectFunc(Chao_Transport_Display, 0x729250);
ObjectFunc(Chao_Transport_Main, 0x7293E0);
ObjectFunc(Chao_Name_Machine_Main, 0x729610);
ObjectFunc(Chao_Name_Machine_Delete, 0x729D80);
ObjectFunc(Chao_Name_Machine_Display, 0x729DB0);
ObjectFunc(Chao_Name_Machine_Load, 0x729EE0);
FunctionPointer(void *, LoadChaoMessageFile, (char *filename, int language), 0x72C060);
FunctionPointer(void *, LoadChaoMessageFile2, (char *filename), 0x72C180);
ObjectFunc(ChaoEgg_Main, 0x7435F0);
FunctionPointer(void, SetDebugTextColor, (int a1), 0x7808E0);
FunctionPointer(void, DisplayDebugString, (signed int position, const char *text), 0x7808F0);
FunctionPointer(void, PrintDebugNumber, (signed int position, int value, signed int numdigits), 0x780970);
FunctionPointer(void, DisplayDebugStringFormatted, (signed int position, const char *format, ...), 0x780B30);
ThiscallFunctionPointer(void, MatrixScale, (Vector3 *Scale), 0x781700);
FastcallFunctionPointer(void, MatrixRotateXYZ, (D3DMATRIX *a1, signed int XRot, signed int YRot, signed int ZRot), 0x781770);
FastcallFunctionPointer(void, MatrixRotateZYX, (int a1, signed int XRot, signed int YRot, signed int ZRot), 0x7819C0);
ThiscallFunctionPointer(void, MatrixTranslate, (Vector3 *), 0x781C10);
ThiscallFunctionPointer(void, MatrixTranslate3, (D3DMATRIX *a1, float XDist, float YDist, float ZDist), 0x784BE0);
ThiscallFunctionPointer(void, MatrixScale3, (D3DMATRIX *a1, float XScale, float YScale, float ZScale), 0x784C70);
FastcallFunctionPointer(void, TransformCoordinate, (D3DMATRIX *a1, Vector3 *a3, Vector3 *a2), 0x784D60);
FastcallFunctionPointer(void, MatrixRotateY, (D3DMATRIX *a1, signed int a2), 0x784E20);
FastcallFunctionPointer(void, MatrixRotateX, (D3DMATRIX *a1, signed int a2), 0x784EE0);
FastcallFunctionPointer(void, MatrixRotateZ, (D3DMATRIX *a1, signed int a2), 0x784FB0);
FastcallFunctionPointer(void, MatrixTranslate2, (D3DMATRIX *a1, NJS_VECTOR *a2), 0x785070);
ThiscallFunctionPointer(void, PushMatrix2, (D3DMATRIX *a1), 0x7850F0);
ThiscallFunctionPointer(void, PopMatrices, (int numMatrices), 0x785140);
FastcallFunctionPointer(void, Vector3_Subtract, (Vector3 *a1, Vector3 *a2), 0x787610);
FastcallFunctionPointer(void, MatrixScale2, (D3DMATRIX *a1, Vector3 *a2), 0x788A50);
FunctionPointer(void, RenderSA2Model, (OBJECT *a1), 0x78AB80);
FunctionPointer(char *, GetWindowClassName, (), 0x793F60);
StdcallFunctionPointer(void, CompletionRoutine, (DWORD, DWORD, LPOVERLAPPED), 0x795340);
ObjectFunc(Cart_Main, 0x79A9E0);
ObjectFunc(RingGroup_Main, 0x79B2F0);
ObjectFunc(BoaBoa_Main, 0x7A0330);
ObjectFunc(TikalHint_Load, 0x7A1AA0);
ObjectFunc(TikalHint_Main, 0x7A1CD0);
ObjectFunc(Balloon_Display, 0x7A1E10);
ObjectFunc(Balloon_Main, 0x7A21C0);
ObjectFunc(DashHoop_Main, 0x7A26F0);
ObjectFunc(Weed_Main, 0x7A2B60);
ObjectFunc(Spikes_Main, 0x7A30E0);
ObjectFunc(FallingSpikeBall_Main, 0x7A3A50);
ObjectFunc(FallingSpikeBall_Load, 0x7A3E50);
ObjectFunc(SwingSpikeBall_Load, 0x7A4260);
ObjectFunc(DashPanel_Display, 0x7A4360);
ObjectFunc(DashPanel_Main, 0x7A4450);
ObjectFunc(Spring_Display, 0x7A4980);
ObjectFunc(Spring_Main, 0x7A4C30);
ObjectFunc(Froggy_Display, 0x7A6BB0);
ObjectFunc(Froggy_Main, 0x7A7AD0);
ObjectFunc(Bubbles_Main, 0x7A8A60);
ObjectFunc(Bubble_Main, 0x7A8E50);
ObjectFunc(HintMonitor_Display, 0x7A9460);
ObjectFunc(HintMonitor_Main, 0x7A9B40);
ObjectFunc(HintMonitor_Load, 0x7A9C60);
ObjectFunc(Sweep_Load, 0x7AA960);
FunctionPointer(signed int, ProcessNBFile, (void *a1), 0x7B0260);
ObjectFunc(Tikal_Delete, 0x7B32C0);
ObjectFunc(Tikal_Display, 0x7B33A0);
ObjectFunc(Tikal_Main, 0x7B40C0);
ObjectFunc(Eggman_Delete, 0x7B4420);
ObjectFunc(Eggman_Main, 0x7B4EF0);
VoidFunc(InitSonicAnimData, 0x7CEC90);
VoidFunc(InitEC0NPCData, 0x7D2720);
VoidFunc(InitEC3NPCData, 0x7D2CF0);
VoidFunc(InitSSNPCData, 0x7D5810);

// ObjectMaster *__usercall<eax>(signed int a1<edx>, void (__cdecl *LoadSub)(ObjectMaster *)<edi>)
static const void *const LoadObject2Ptr = (void*)0x40B030;
static inline ObjectMaster * LoadObject2(signed int a1, void (__cdecl *LoadSub)(ObjectMaster *))
{
	ObjectMaster * result;
	__asm
	{
		mov edi, [LoadSub]
		mov edx, [a1]
		call LoadObject2Ptr
		mov result, eax
	}
	return result;
}

// void __usercall(__int16 a1)
static const void *const StartLevelCutscenePtr = (void*)0x413C90;
static inline void StartLevelCutscene(__int16 a1)
{
	__asm
	{
		push [a1]
		call StartLevelCutscenePtr
		add esp, 4
	}
}

// signed int __usercall<eax>(int a1<ebp>, char *a2, TexListHead *a3)
static const void *const LoadPvmMEM2Ptr = (void*)0x421260;
static inline signed int LoadPvmMEM2(int a1, char *a2, TexListHead *a3)
{
	signed int result;
	__asm
	{
		push [a3]
		push [a2]
		mov ebp, [a1]
		call LoadPvmMEM2Ptr
		add esp, 8
		mov result, eax
	}
	return result;
}

// void __usercall(int act<ecx>, char *FileName)
static const void *const LoadCamFilePtr = (void*)0x4224F0;
static inline void LoadCamFile(int act, char *FileName)
{
	__asm
	{
		push [FileName]
		mov ecx, [act]
		call LoadCamFilePtr
		add esp, 4
	}
}

// void __usercall(int act<ecx>, char *FileName)
static const void *const LoadSetFilePtr = (void*)0x422930;
static inline void LoadSetFile(int act, char *FileName)
{
	__asm
	{
		push [FileName]
		mov ecx, [act]
		call LoadSetFilePtr
		add esp, 4
	}
}

// void __usercall(int a2<ebp>)
static const void *const LoadCharBossSoundsPtr = (void*)0x424760;
static inline void LoadCharBossSounds(int a2)
{
	__asm
	{
		mov ebp, [a2]
		call LoadCharBossSoundsPtr
	}
}

// unsigned __int16 __usercall<ax>(byte *a2<esi>)
static const void *const CalcSaveChecksumPtr = (void*)0x42CF90;
static inline unsigned __int16 CalcSaveChecksum(byte *a2)
{
	unsigned __int16 result;
	__asm
	{
		mov esi, [a2]
		call CalcSaveChecksumPtr
		mov result, ax
	}
	return result;
}

// void __usercall(SaveFileData *a1<edi>)
static const void *const WriteEventFlagsToSaveFilePtr = (void*)0x42D0D0;
static inline void WriteEventFlagsToSaveFile(SaveFileData *a1)
{
	__asm
	{
		mov edi, [a1]
		call WriteEventFlagsToSaveFilePtr
	}
}

// void __usercall(SaveFileData *a1<edi>)
static const void *const WriteNPCFlagsToSaveFilePtr = (void*)0x42D130;
static inline void WriteNPCFlagsToSaveFile(SaveFileData *a1)
{
	__asm
	{
		mov edi, [a1]
		call WriteNPCFlagsToSaveFilePtr
	}
}

// void __usercall(SaveFileData *a2<esi>)
static const void *const ReadEventFlagsFromSaveFilePtr = (void*)0x42D250;
static inline void ReadEventFlagsFromSaveFile(SaveFileData *a2)
{
	__asm
	{
		mov esi, [a2]
		call ReadEventFlagsFromSaveFilePtr
	}
}

// void __usercall(SaveFileData *a2<esi>)
static const void *const ReadNPCFlagsFromSaveFilePtr = (void*)0x42D2A0;
static inline void ReadNPCFlagsFromSaveFile(SaveFileData *a2)
{
	__asm
	{
		mov esi, [a2]
		call ReadNPCFlagsFromSaveFilePtr
	}
}

// bool __usercall<eax>(CharObj1 *a1<esi>, CharObj2 *a2<edi>)
static const void *const Sonic_JumpCancelPtr = (void*)0x492F50;
static inline int Sonic_JumpCancel(CharObj1 *a1, CharObj2 *a2)
{
	int result;
	__asm
	{
		mov edi, [a2]
		mov esi, [a1]
		call Sonic_JumpCancelPtr
		mov result, eax
	}
	return result;
}

// bool __usercall<eax>(CharObj2 *a1<edi>, CharObj1 *a2<esi>)
static const void *const Sonic_CheckLightDashPtr = (void*)0x492F90;
static inline int Sonic_CheckLightDash(CharObj2 *a1, CharObj1 *a2)
{
	int result;
	__asm
	{
		mov esi, [a2]
		mov edi, [a1]
		call Sonic_CheckLightDashPtr
		mov result, eax
	}
	return result;
}

// signed int __usercall<eax>(CharObj2 *a1<edi>, CharObj1 *a2<esi>)
static const void *const Sonic_CheckPickUpObjectPtr = (void*)0x493120;
static inline signed int Sonic_CheckPickUpObject(CharObj2 *a1, CharObj1 *a2)
{
	signed int result;
	__asm
	{
		mov esi, [a2]
		mov edi, [a1]
		call Sonic_CheckPickUpObjectPtr
		mov result, eax
	}
	return result;
}

// void __usercall(CharObj2 *a1<esi>)
static const void *const Sonic_Run1AniPtr = (void*)0x495B00;
static inline void Sonic_Run1Ani(CharObj2 *a1)
{
	__asm
	{
		mov esi, [a1]
		call Sonic_Run1AniPtr
	}
}

// void __usercall(CharObj2 *a1<esi>)
static const void *const Sonic_SpringAniPtr = (void*)0x495B50;
static inline void Sonic_SpringAni(CharObj2 *a1)
{
	__asm
	{
		mov esi, [a1]
		call Sonic_SpringAniPtr
	}
}

// void __usercall(int a1<esi>)
static const void *const Sonic_Walk3AniPtr = (void*)0x495BA0;
static inline void Sonic_Walk3Ani(int a1)
{
	__asm
	{
		mov esi, [a1]
		call Sonic_Walk3AniPtr
	}
}

// void __usercall(int a1<esi>)
static const void *const Sonic_Run2AniPtr = (void*)0x495BE0;
static inline void Sonic_Run2Ani(int a1)
{
	__asm
	{
		mov esi, [a1]
		call Sonic_Run2AniPtr
	}
}

// char __usercall<al>(CharObj2 *a1<esi>)
static const void *const Sonic_SpinPtr = (void*)0x495C30;
static inline char Sonic_Spin(CharObj2 *a1)
{
	char result;
	__asm
	{
		mov esi, [a1]
		call Sonic_SpinPtr
		mov result, al
	}
	return result;
}

// void __usercall(int a1<esi>)
static const void *const Sonic_JumpPadAniPtr = (void*)0x495C80;
static inline void Sonic_JumpPadAni(int a1)
{
	__asm
	{
		mov esi, [a1]
		call Sonic_JumpPadAniPtr
	}
}

// signed int __usercall<eax>(CharObj2 *a1<eax>, CharObj1 *a2<edi>, int a3)
static const void *const Sonic_NActPtr = (void*)0x495FA0;
static inline signed int Sonic_NAct(CharObj2 *a1, CharObj1 *a2, int a3)
{
	signed int result;
	__asm
	{
		push [a3]
		mov edi, [a2]
		mov eax, [a1]
		call Sonic_NActPtr
		add esp, 4
		mov result, eax
	}
	return result;
}

// signed int __usercall<eax>(CharObj2 *a1<eax>, CharObj1 *a2<edi>)
static const void *const Sonic_CheckSpindashPtr = (void*)0x496EE0;
static inline signed int Sonic_CheckSpindash(CharObj2 *a1, CharObj1 *a2)
{
	signed int result;
	__asm
	{
		mov edi, [a2]
		mov eax, [a1]
		call Sonic_CheckSpindashPtr
		mov result, eax
	}
	return result;
}

// signed int __usercall<eax>(CharObj1 *a1<eax>, ObjectMaster *a2<ebx>)
static const void *const Spinner_InitPtr = (void*)0x4B0D00;
static inline signed int Spinner_Init(CharObj1 *a1, ObjectMaster *a2)
{
	signed int result;
	__asm
	{
		mov ebx, [a2]
		mov eax, [a1]
		call Spinner_InitPtr
		mov result, eax
	}
	return result;
}

// void __usercall(ObjectMaster *this<eax>, __int16 type)
static const void *const DrawColObjPtr = (void*)0x4D42A0;
static inline void DrawColObj(ObjectMaster *obj, __int16 _type)
{
	__asm
	{
		push [_type]
		mov eax, [obj]
		call DrawColObjPtr
		add esp, 4
	}
}

// int __usercall<eax>(int a1<esi>, LPCSTR filename)
static const void *const PlayVideoFilePtr = (void*)0x513ED0;
static inline int PlayVideoFile(int a1, LPCSTR filename)
{
	int result;
	__asm
	{
		push [filename]
		mov esi, [a1]
		call PlayVideoFilePtr
		add esp, 4
		mov result, eax
	}
	return result;
}

// void __usercall(Vector3 *a2<ebx>, CharObj1 *a3<esi>, signed int index)
static const void *const CalcRingPosLinePtr = (void*)0x79B140;
static inline void CalcRingPosLine(Vector3 *a2, CharObj1 *a3, signed int index)
{
	__asm
	{
		push [index]
		mov esi, [a3]
		mov ebx, [a2]
		call CalcRingPosLinePtr
		add esp, 4
	}
}

// void __usercall(Vector3 *position<ebx>, CharObj1 *parent<esi>, signed int index)
static const void *const CalcRingPosCirclePtr = (void*)0x79B210;
static inline void CalcRingPosCircle(Vector3 *position, CharObj1 *parent, signed int index)
{
	__asm
	{
		push [index]
		mov esi, [parent]
		mov ebx, [position]
		call CalcRingPosCirclePtr
		add esp, 4
	}
}

// ModInfo

static const int ModLoaderVer = 3;

#define arrayptrandlength(data) data, LengthOfArray(data)
#define arraylengthandptr(data) LengthOfArray(data), data
#define arrayptrandsize(data) data, SizeOfArray(data)
#define arraysizeandptr(data) SizeOfArray(data), data

struct PatchInfo
{
	void *address;
	const void *data;
	int datasize;
};

#define patchdecl(address,data) { (void*)address, arrayptrandsize(data) }

struct PointerInfo
{
	void *address;
	void *data;
};

#define ptrdecl(address,data) { (void*)address, (void*)data }

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

typedef HelperFunctions_v3 HelperFunctions;

struct ModInfo
{
	int Version;
	void (__cdecl *Init)(const char *path, const HelperFunctions &helperFunctions);
	const PatchInfo *Patches;
	int PatchCount;
	const PointerInfo *Jumps;
	int JumpCount;
	const PointerInfo *Calls;
	int CallCount;
	const PointerInfo *Pointers;
	int PointerCount;
};
#endif