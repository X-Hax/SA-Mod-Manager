/**
 * SADX Mod Loader.
 * SADX enums.
 */

#ifndef SADXMODLOADER_SADXENUMS_H
#define SADXMODLOADER_SADXENUMS_H

#include <stdint.h>

// SADX Enums

#define makemask(enumname,membername) enumname##_##membername = (1 << enumname##Bits_##membername)
#define makemasks(enumname,membername) enumname##s_##membername = (1 << enumname##Bits_##membername)
#define levelact(level, act) (((level) << 8) | (act))

enum StatusBits
{
	StatusBits_Ground,
	StatusBits_Unknown1,
	StatusBits_Hurt,
	StatusBits_ObjectInteract,
	StatusBits_Unknown2,
	StatusBits_Unknown3,
	StatusBits_Unknown4,
	StatusBits_Unknown5,
	StatusBits_Ball,
	StatusBits_LightDash,
	StatusBits_Attack,
	StatusBits_HoldObject,
	StatusBits_DoNextAction,
	StatusBits_OnPath,
	StatusBits_DisableControl,
	StatusBits_Unknown6
};

enum Status : uint16_t
{
	makemask(Status, Ground),
	makemask(Status, Unknown1),
	makemask(Status, Hurt),
	makemask(Status, ObjectInteract),
	makemask(Status, Unknown2),
	makemask(Status, Unknown3),
	makemask(Status, Unknown4),
	makemask(Status, Unknown5),
	makemask(Status, Ball),
	makemask(Status, LightDash),
	makemask(Status, Attack),
	makemask(Status, HoldObject),
	makemask(Status, DoNextAction),
	makemask(Status, OnPath),
	makemask(Status, DisableControl),
	makemask(Status, Unknown6)
};

enum PowerupBits
{
	PowerupBits_Barrier,
	PowerupBits_MagneticBarrier,
	PowerupBits_Unknown1,
	PowerupBits_Unknown2,
	PowerupBits_Unknown3,
	PowerupBits_Unknown4,
	PowerupBits_Unknown5,
	PowerupBits_Unknown6,
	PowerupBits_Unknown7,
	PowerupBits_Unknown8,
	PowerupBits_Unknown9,
	PowerupBits_Unknown10,
	PowerupBits_Unknown11,
	PowerupBits_Unknown12,
	PowerupBits_Dead,
	PowerupBits_Invincibility
};

enum Powerups : unsigned short
{
	makemasks(Powerup, Barrier),
	makemasks(Powerup, MagneticBarrier),
	makemasks(Powerup, Unknown1),
	makemasks(Powerup, Unknown2),
	makemasks(Powerup, Unknown3),
	makemasks(Powerup, Unknown4),
	makemasks(Powerup, Unknown5),
	makemasks(Powerup, Unknown6),
	makemasks(Powerup, Unknown7),
	makemasks(Powerup, Unknown8),
	makemasks(Powerup, Unknown9),
	makemasks(Powerup, Unknown10),
	makemasks(Powerup, Unknown11),
	makemasks(Powerup, Unknown12),
	makemasks(Powerup, Dead),
	makemasks(Powerup, Invincibility)
};

enum UpgradeBits
{
	UpgradeBits_CrystalRing,
	UpgradeBits_LightShoes,
	UpgradeBits_AncientLight,
	UpgradeBits_JetAnklet,
	UpgradeBits_RhythmBadge,
	UpgradeBits_FightingGloves,
	UpgradeBits_ShovelClaw,
	UpgradeBits_LongHammer,
	UpgradeBits_WarriorFeather,
	UpgradeBits_LaserBlaster,
	UpgradeBits_JetBooster,
	UpgradeBits_PowerRod,
	UpgradeBits_LifeRing,
	UpgradeBits_SuperSonic = 15
};

enum Upgrades
{
	makemasks(Upgrade, CrystalRing),
	makemasks(Upgrade, LightShoes),
	makemasks(Upgrade, AncientLight),
	Upgrades_SonicMask = Upgrades_CrystalRing | Upgrades_LightShoes | Upgrades_AncientLight,
	makemasks(Upgrade, JetAnklet),
	makemasks(Upgrade, RhythmBadge),
	Upgrades_TailsMask = Upgrades_JetAnklet | Upgrades_RhythmBadge,
	makemasks(Upgrade, FightingGloves),
	makemasks(Upgrade, ShovelClaw),
	Upgrades_KnucklesMask = Upgrades_FightingGloves | Upgrades_ShovelClaw,
	makemasks(Upgrade, LongHammer),
	makemasks(Upgrade, WarriorFeather),
	Upgrades_AmyMask = Upgrades_LongHammer | Upgrades_WarriorFeather,
	makemasks(Upgrade, LaserBlaster),
	makemasks(Upgrade, JetBooster),
	Upgrades_GammaMask = Upgrades_LaserBlaster | Upgrades_JetBooster,
	makemasks(Upgrade, PowerRod),
	makemasks(Upgrade, LifeRing),
	Upgrades_BigMask = Upgrades_PowerRod | Upgrades_LifeRing,
	Upgrades_UpgradesMask = Upgrades_SonicMask | Upgrades_TailsMask | Upgrades_KnucklesMask | Upgrades_AmyMask | Upgrades_GammaMask | Upgrades_BigMask,
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
	CharacterFlags_Sonic	= (1 << Characters_Sonic),
	CharacterFlags_Eggman	= (1 << Characters_Eggman),
	CharacterFlags_Tails	= (1 << Characters_Tails),
	CharacterFlags_Knuckles	= (1 << Characters_Knuckles),
	CharacterFlags_Tikal	= (1 << Characters_Tikal),
	CharacterFlags_Amy	= (1 << Characters_Amy),
	CharacterFlags_Gamma	= (1 << Characters_Gamma),
	CharacterFlags_Big	= (1 << Characters_Big)
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

enum Levels2P
{
	Levels2P_WindyValley,
	Levels2P_SpeedHighway,
	Levels2P_SkyDeck,
	Levels2P_IceCap,
	Levels2P_Casinopolis
};

enum ColFlags : Uint32
{
	ColFlags_Solid          = 0x1,
	ColFlags_Water          = 0x2,
	ColFlags_NoFriction     = 0x4,
	ColFlags_NoAccel        = 0x8,
	ColFlags_UseSkyDrawDist = 0x20,
	ColFlags_IncAccel       = 0x80,
	ColFlags_Dig            = 0x100,
	ColFlags_NoClimb        = 0x1000,
	ColFlags_Hurt           = 0x10000,
	ColFlags_Footprints     = 0x100000,
	ColFlags_NoAlphaSortPls = 0x1000000,
	ColFlags_AlphaSortThing = 0x2000000,
	ColFlags_UvManipulation = 0x4000000,
	ColFlags_UseRotation    = 0x10000000,
	ColFlags_Visible        = 0x80000000
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
	ButtonBits_C,
	ButtonBits_B,
	ButtonBits_A,
	ButtonBits_Start,
	ButtonBits_Up,
	ButtonBits_Down,
	ButtonBits_Left,
	ButtonBits_Right,
	ButtonBits_Z,
	ButtonBits_Y,
	ButtonBits_X,
	ButtonBits_D,
	ButtonBits_Up2,
	ButtonBits_Down2,
	ButtonBits_Left2,
	ButtonBits_Right2,
	ButtonBits_R,
	ButtonBits_L
};

enum Buttons : int
{
	makemasks(Button, C),
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
	makemasks(Button, D),
	makemasks(Button, Up2),
	makemasks(Button, Down2),
	makemasks(Button, Left2),
	makemasks(Button, Right2),
	makemasks(Button, R),
	makemasks(Button, L)
};

enum Languages
{
	Languages_Japanese,
	Languages_English,
	Languages_French,
	Languages_Spanish,
	Languages_German
};

enum PauseOptions
{
	PauseOptions_Continue	= 0x01,
	PauseOptions_Restart	= 0x02,
	PauseOptions_Controls	= 0x04,
	PauseOptions_Quit		= 0x08,
	PauseOptions_Map		= 0x10,
	PauseOptions_Camera		= 0x20,
	PauseOptions_Missions	= 0x40,
	PauseOptions_Unknown	= 0x80
};

enum MusicIDs
{
	MusicIDs_advamy, MusicIDs_AmyTutorial = MusicIDs_advamy,
	MusicIDs_advbig, MusicIDs_BigTutorial = MusicIDs_advbig,
	MusicIDs_adve102, MusicIDs_E102Tutorial = MusicIDs_adve102,
	MusicIDs_advknkls, MusicIDs_KnucklesTutorial = MusicIDs_advknkls,
	MusicIDs_advmiles, MusicIDs_TailsTutorial = MusicIDs_advmiles,
	MusicIDs_advsonic, MusicIDs_SonicTutorial = MusicIDs_advsonic,
	MusicIDs_amy, MusicIDs_AmyMySweetPassion = MusicIDs_amy,
	MusicIDs_big, MusicIDs_BigLazyDaysLivinInParadise = MusicIDs_big,
	MusicIDs_bossall, MusicIDs_BossOthers = MusicIDs_bossall,
	MusicIDs_bosse101, MusicIDs_BossE101mkII = MusicIDs_bosse101,
	MusicIDs_bossevnt, MusicIDs_BossEvent = MusicIDs_bossevnt,
	MusicIDs_bosstrgt, MusicIDs_BossESeries = MusicIDs_bosstrgt,
	MusicIDs_casino1, MusicIDs_CasinopolisMainHall = MusicIDs_casino1,
	MusicIDs_casino2, MusicIDs_CasinopolisDilapidatedWay = MusicIDs_casino2,
	MusicIDs_casino3, MusicIDs_CasinopolisSONICPinball = MusicIDs_casino3,
	MusicIDs_casino4, MusicIDs_CasinopolisNiGHTSPinball = MusicIDs_casino4,
	MusicIDs_chao, MusicIDs_ThemeOfChao = MusicIDs_chao,
	MusicIDs_chaogoal, MusicIDs_ChaoRaceGoal = MusicIDs_chaogoal,
	MusicIDs_chaohall, MusicIDs_ChaoRaceEntrance_OLD = MusicIDs_chaohall,
	MusicIDs_chaorace, MusicIDs_ChaoRace = MusicIDs_chaorace,
	MusicIDs_chaos, MusicIDs_BossChaos = MusicIDs_chaos,
	MusicIDs_chaos_6, MusicIDs_BossChaos6 = MusicIDs_chaos_6,
	MusicIDs_chaos_p1, MusicIDs_BossPerfectChaos1 = MusicIDs_chaos_p1,
	MusicIDs_chaos_p2, MusicIDs_BossPerfectChaos2 = MusicIDs_chaos_p2,
	MusicIDs_charactr, MusicIDs_CharacterSelect = MusicIDs_charactr,
	MusicIDs_circuit, MusicIDs_TwinkleCircuit = MusicIDs_circuit,
	MusicIDs_continue, MusicIDs_Continue = MusicIDs_continue,
	MusicIDs_e102, MusicIDs_ThemeOfE102 = MusicIDs_e102,
	MusicIDs_ecoast1, MusicIDs_EmeraldCoastAzureBlueWorld = MusicIDs_ecoast1,
	MusicIDs_ecoast2, MusicIDs_EmeraldCoastWindyAndRipply = MusicIDs_ecoast2,
	MusicIDs_ecoast3, MusicIDs_EmeraldCoastBigFishin = MusicIDs_ecoast3,
	MusicIDs_egcarer1, MusicIDs_EggCarrier = MusicIDs_egcarer1,
	MusicIDs_egcarer2, MusicIDs_EggCarrierTheOcean = MusicIDs_egcarer2,
	MusicIDs_eggman, MusicIDs_ThemeOfDrEggman = MusicIDs_eggman,
	MusicIDs_eggmbl23, MusicIDs_BossEggWalkerViper = MusicIDs_eggmbl23,
	MusicIDs_eggrobo, MusicIDs_BossZERO = MusicIDs_eggrobo,
	MusicIDs_evtbgm00, MusicIDs_EventSadness = MusicIDs_evtbgm00,
	MusicIDs_evtbgm01, MusicIDs_EventStrain = MusicIDs_evtbgm01,
	MusicIDs_evtbgm02, MusicIDs_EventUnbound = MusicIDs_evtbgm02,
	MusicIDs_evtbgm03, MusicIDs_EventGoodbye = MusicIDs_evtbgm03,
	MusicIDs_evtbgm04, MusicIDs_EventThePast = MusicIDs_evtbgm04,
	MusicIDs_evtbgm05, MusicIDs_EventFanfare4DrEggman = MusicIDs_evtbgm05,
	MusicIDs_finaleg1, MusicIDs_FinalEggMechanicalResonance = MusicIDs_finaleg1,
	MusicIDs_finaleg2, MusicIDs_FinalEggCrankTheHeatUp = MusicIDs_finaleg2,
	MusicIDs_fishget, MusicIDs_FishGet = MusicIDs_fishget,
	MusicIDs_fishing, MusicIDs_FishHit = MusicIDs_fishing,
	MusicIDs_fishmiss, MusicIDs_FishMiss = MusicIDs_fishmiss,
	MusicIDs_hammer, MusicIDs_HedgehogHammer = MusicIDs_hammer,
	MusicIDs_highway1, MusicIDs_SpeedHighwaySpeedHighway = MusicIDs_highway1,
	MusicIDs_highway2, MusicIDs_SpeedHighwayGoinDown = MusicIDs_highway2,
	MusicIDs_highway3, MusicIDs_SpeedHighwayAtDawn = MusicIDs_highway3,
	MusicIDs_hurryup, MusicIDs_AmyHurryUp = MusicIDs_hurryup,
	MusicIDs_icecap1, MusicIDs_IcecapSnowyMountain = MusicIDs_icecap1,
	MusicIDs_icecap2, MusicIDs_IcecapLimestoneCave = MusicIDs_icecap2,
	MusicIDs_icecap3, MusicIDs_IcecapSnowboard = MusicIDs_icecap3,
	MusicIDs_invncibl, MusicIDs_Invincibility = MusicIDs_invncibl,
	MusicIDs_item1, MusicIDs_Item = MusicIDs_item1,
	MusicIDs_jingle_1, MusicIDs_JingleA = MusicIDs_jingle_1,
	MusicIDs_jingle_2, MusicIDs_JingleB = MusicIDs_jingle_2,
	MusicIDs_jingle_3, MusicIDs_JingleC = MusicIDs_jingle_3,
	MusicIDs_jingle_4, MusicIDs_JingleD = MusicIDs_jingle_4,
	MusicIDs_jingle_5, MusicIDs_JingleE = MusicIDs_jingle_5,
	MusicIDs_KNUCKLES, MusicIDs_KnucklesUnknownFromME = MusicIDs_KNUCKLES,
	MusicIDs_lstwrld1, MusicIDs_LostWorldTrickyMaze = MusicIDs_lstwrld1,
	MusicIDs_lstwrld2, MusicIDs_LWorldDangerChasedbyRock = MusicIDs_lstwrld2,
	MusicIDs_lstwrld3, MusicIDs_LostWorldLeadingLights = MusicIDs_lstwrld3,
	MusicIDs_mainthem, MusicIDs_OpenYourHeart = MusicIDs_mainthem,
	MusicIDs_mstcln, MusicIDs_MysticRuins = MusicIDs_mstcln,
	MusicIDs_nights_a,
	MusicIDs_nights_k,
	MusicIDs_nights_s,
	MusicIDs_one_up, MusicIDs_Extend = MusicIDs_one_up,
	MusicIDs_option, MusicIDs_Options = MusicIDs_option,
	MusicIDs_redmntn1, MusicIDs_RedMountainMtRedASymbolOfThrill = MusicIDs_redmntn1,
	MusicIDs_redmntn2, MusicIDs_RedMountainRedHotSkull = MusicIDs_redmntn2,
	MusicIDs_rndclear, MusicIDs_RoundClear = MusicIDs_rndclear,
	MusicIDs_s_square, MusicIDs_StationSquare = MusicIDs_s_square,
	MusicIDs_sandhill, MusicIDs_SandHill = MusicIDs_sandhill,
	MusicIDs_scramble, MusicIDs_TornadoScramble = MusicIDs_scramble,
	MusicIDs_shelter1, MusicIDs_HotShelterBadTasteAquarium = MusicIDs_shelter1,
	MusicIDs_shelter2, MusicIDs_HotShelterRedBarrageArea = MusicIDs_shelter2,
	MusicIDs_skydeck1, MusicIDs_SkyDeckSkydeckAGoGo = MusicIDs_skydeck1,
	MusicIDs_skydeck2, MusicIDs_SkyDeckGeneralOffensive = MusicIDs_skydeck2,
	MusicIDs_sonic, MusicIDs_SonicItDoesntMatter = MusicIDs_sonic,
	MusicIDs_sonic_cd, MusicIDs_PalmtreePanic = MusicIDs_sonic_cd,
	MusicIDs_speedup, MusicIDs_SpeedUp = MusicIDs_speedup,
	MusicIDs_sprsonic, MusicIDs_ThemeOfSuperSonic = MusicIDs_sprsonic,
	MusicIDs_ssracing, MusicIDs_SuperSonicRacing = MusicIDs_ssracing,
	MusicIDs_tails, MusicIDs_TailsBelieveInMyself = MusicIDs_tails,
	MusicIDs_theamy, MusicIDs_AmyAppearance = MusicIDs_theamy,
	MusicIDs_thebig, MusicIDs_BigAppearance = MusicIDs_thebig,
	MusicIDs_thee102, MusicIDs_E102Appearance = MusicIDs_thee102,
	MusicIDs_theknkls, MusicIDs_KnucklesAppearance = MusicIDs_theknkls,
	MusicIDs_themiles, MusicIDs_TailsAppearance = MusicIDs_themiles,
	MusicIDs_thesonic, MusicIDs_SonicAppearance = MusicIDs_thesonic,
	MusicIDs_tical, MusicIDs_ThemeOfTikal = MusicIDs_tical,
	MusicIDs_timer, MusicIDs_Timer = MusicIDs_timer,
	MusicIDs_titl_egg, MusicIDs_EggCarrierTransform = MusicIDs_titl_egg,
	MusicIDs_titl_mr1,
	MusicIDs_titl_mr2,
	MusicIDs_titl_ss,
	MusicIDs_title, MusicIDs_MainMenu = MusicIDs_title,
	MusicIDs_title2, MusicIDs_TitleScreen = MusicIDs_title2,
	MusicIDs_trial, MusicIDs_Trial = MusicIDs_trial,
	MusicIDs_twnklpk1, MusicIDs_TwinkleParkTwinklePark = MusicIDs_twnklpk1,
	MusicIDs_twnklpk2, MusicIDs_TwinkleParkPleasureCastle = MusicIDs_twnklpk2,
	MusicIDs_twnklpk3, MusicIDs_TwinkleParkAmyMirrorRoom = MusicIDs_twnklpk3,
	MusicIDs_wndyvly1, MusicIDs_WindyValleyWindyHill = MusicIDs_wndyvly1,
	MusicIDs_wndyvly2, MusicIDs_WindyValleyTornado = MusicIDs_wndyvly2,
	MusicIDs_wndyvly3, MusicIDs_WindyValleyTheAir = MusicIDs_wndyvly3,
	MusicIDs_MSTART_44, MusicIDs_MissionStart = MusicIDs_MSTART_44,
	MusicIDs_MCLEAR_44, MusicIDs_MissionClear = MusicIDs_MCLEAR_44,
	MusicIDs_chao_k_net_fine, MusicIDs_ChaoLevelUp = MusicIDs_chao_k_net_fine,
	MusicIDs_chao_g_iede, MusicIDs_ChaoGoodbye = MusicIDs_chao_g_iede,
	MusicIDs_chao_r_e, MusicIDs_ChaoNaming = MusicIDs_chao_r_e,
	MusicIDs_c_btl_cv, MusicIDs_ChaoRaceEntrance = MusicIDs_c_btl_cv,
	MusicIDs_chao_r_gate_open, MusicIDs_ChaoRaceGateOpen = MusicIDs_chao_r_gate_open,
	MusicIDs_chao_g_born_h2, MusicIDs_ChaoHeroChaosBorn2 = MusicIDs_chao_g_born_h2,
	MusicIDs_chao_g_born_d2, MusicIDs_ChaoDarkChaosBorn2 = MusicIDs_chao_g_born_d2,
	MusicIDs_chao_g_born_c, MusicIDs_ChaoChaosBorn = MusicIDs_chao_g_born_c,
	MusicIDs_chao_g_born_h, MusicIDs_ChaoHeroChaosBorn = MusicIDs_chao_g_born_h,
	MusicIDs_chao_g_born_d, MusicIDs_ChaoDarkChaosBorn = MusicIDs_chao_g_born_h,
	MusicIDs_chao_g_dead, MusicIDs_ChaoDied = MusicIDs_chao_g_dead,
	MusicIDs_chao_g_dance, MusicIDs_ChaoDance = MusicIDs_chao_g_dance,
	MusicIDs_chao_k_m, MusicIDs_ChaoBlackMarket = MusicIDs_chao_k_m
};

enum LoadObj : Sint8
{
	LoadObj_Data2 = 0x1,
	LoadObj_Data1 = 0x2,
	LoadObj_UnknownA = 0x4,
	LoadObj_UnknownB = 0x8,
};


// Chao Enums
// From here with slight modifications: http://chao.tehfusion.co.uk/chao-hacking/
// Chao gardens

enum SADXChaoStage : Sint8
{
	SADXChaoStage_None            = -1,
	SADXChaoStage_Race            = 1,
	SADXChaoStage_RaceEntry       = 2,
	SADXChaoStage_ChaoTransporter = 3,
	SADXChaoStage_StationSquare   = 4,
	SADXChaoStage_EggCarrier      = 5,
	SADXChaoStage_MysticRuins     = 6,
	SADXChaoStage_BlackMarket     = 7,
};

enum SA2BChaoStage : Sint8
{
	SA2BChaoStage_None            = -1,
	SA2BChaoStage_NeutralGarden   = 1,
	SA2BChaoStage_HeroGarden      = 2,
	SA2BChaoStage_DarkGarden      = 3,
	SA2BChaoStage_Race            = 4,
	SA2BChaoStage_RaceEntrance    = 5,
	SA2BChaoStage_Kindergarten    = 6,
	SA2BChaoStage_Lobby           = 7,
	SA2BChaoStage_RaceEntrance_2P = 8,
	SA2BChaoStage_Stadium         = 9,
	SA2BChaoStage_Karate          = 10,
	SA2BChaoStage_Karate_2P       = 11,
	SA2BChaoStage_Transporter     = 12,
};

// Chao evolution

enum ChaoType : Sint8
{
	ChaoType_Empty          = 0,
	ChaoType_Egg            = 1,
	ChaoType_Child          = 2,
	ChaoType_Good           = 3, // Unobtainable
	ChaoType_Bad            = 4, // Unobtainable
	ChaoType_Neutral_Normal = 5,
	ChaoType_Hero_Normal    = 6,
	ChaoType_Dark_Normal    = 7,
	ChaoType_Neutral_Swim   = 8,
	ChaoType_Hero_Swim      = 9,
	ChaoType_Dark_Swim      = 10,
	ChaoType_Neutral_Fly    = 11,
	ChaoType_Hero_Fly       = 12,
	ChaoType_Dark_Fly       = 13,
	ChaoType_Neutral_Run    = 14,
	ChaoType_Hero_Run       = 15,
	ChaoType_Dark_Run       = 16,
	ChaoType_Neutral_Power  = 17,
	ChaoType_Hero_Power     = 18,
	ChaoType_Dark_Power     = 19,
	ChaoType_Neutral_Chaos  = 20,
	ChaoType_Hero_Chaos     = 21,
	ChaoType_Dark_Chaos     = 22,
	ChaoType_Tails          = 23, // Character chao
	ChaoType_Knuckles       = 24, // Character chao
	ChaoType_Amy            = 25, // Character chao
};

// Chao stats

enum ChaoGrade : Sint8
{
	ChaoGrade_E = 0,
	ChaoGrade_D = 1,
	ChaoGrade_C = 2,
	ChaoGrade_B = 3,
	ChaoGrade_A = 4,
	ChaoGrade_S = 5,
};

// Chao appearance

enum ChaoEmotiball : Sint8
{
	ChaoEmotiball_Normal = 0,
	ChaoEmotiball_Flame  = 1,
	ChaoEmotiball_None   = 2,
};

enum ChaoEyes : Sint8
{
	ChaoEyes_Normal         = 0,
	ChaoEyes_Painful        = 1,
	ChaoEyes_ClosedStraight = 2,
	ChaoEyes_ClosedHappy    = 3,
	ChaoEyes_ClosedUp       = 4,
	ChaoEyes_Tiny           = 5,
	ChaoEyes_Circles        = 6,
	ChaoEyes_ClosedDown     = 7,
	ChaoEyes_Tiny2          = 8, // Identical to "Tiny"
	ChaoEyes_HalfClosed     = 9,
	ChaoEyes_Mean           = 10,
	ChaoEyes_GreenChaos     = 11,
	ChaoEyes_BlueChaos      = 12,
	ChaoEyes_YellowChaos    = 13,
};

enum ChaoMedal : Sint8
{
	ChaoMedal_None       = 0, // Won in SA2B
	ChaoMedal_Aquamarine = 1, // Won in SA2B
	ChaoMedal_Topaz      = 2, // Won in SA2B
	ChaoMedal_Peridot    = 3, // Won in SA2B
	ChaoMedal_Garnet     = 4, // Won in SA2B
	ChaoMedal_Onyx       = 5, // Won in SA2B
	ChaoMedal_Diamond    = 6, // Won in SA2B
	ChaoMedal_Beginner   = 7, // Won in SA2B
	ChaoMedal_Challenge  = 8, // Won in SA2B
	ChaoMedal_Hero       = 9, // Won in SA2B
	ChaoMedal_Dark       = 10, // Won in SA2B
	ChaoMedal_Pearl      = 11, // Won in SADX
	ChaoMedal_Amethyst   = 12, // Won in SADX
	ChaoMedal_Emerald    = 13, // Won in SADX
	ChaoMedal_Ruby       = 14, // Won in SADX
	ChaoMedal_Sapphire   = 15, // Won in SADX
};

enum ChaoMouth : Sint8
{
	ChaoMouth_None               = 0,
	ChaoMouth_ToothySmile        = 1,
	ChaoMouth_Open               = 2,
	ChaoMouth_ClosedSmile        = 3,
	ChaoMouth_ClosedFrown        = 4,
	ChaoMouth_OpenSmile          = 5,
	ChaoMouth_OpenFrown          = 6,
	ChaoMouth_ClosedSmile2       = 7, // Identical to "Closed Smile"
	ChaoMouth_Squiggly           = 8,
	ChaoMouth_ToothyFrown        = 9,
	ChaoMouth_ClosedFrown2       = 10, // Identical to "Closed Frown"
	ChaoMouth_WideOpen           = 11,
	ChaoMouth_ClosedFrown3       = 12, // Identical to "Closed Frown"
	ChaoMouth_StraightMoustache  = 13,
	ChaoMouth_SquigglyMoustache1 = 14,
	ChaoMouth_SquigglyMoustache2 = 15,
};

enum SADXBodyType : Sint8
{
	SADXBodyType_Normal  = 0,
	SADXBodyType_EggChao = 1,
	SADXBodyType_Omochao = 2,
	SADXBodyType_None    = 3,
};

enum SA2BBodyType : Sint8
{
	SA2BBodyType_Normal  = 0,
	SA2BBodyType_EggChao = 1,
	SA2BBodyType_Omochao = 2,
	SA2BBodyType_Animal  = 3,
	SA2BBodyType_None    = 5,
};

enum SADXRetailColour : Sint8
{
	SADXRetailColour_Normal            = 0,
	SADXRetailColour_Yellow            = 1,
	SADXRetailColour_White             = 2,
	SADXRetailColour_Brown             = 3,
	SADXRetailColour_Aqua              = 4,
	SADXRetailColour_Pink              = 5,
	SADXRetailColour_Blue              = 6,
	SADXRetailColour_Grey              = 7,
	SADXRetailColour_Green             = 8,
	SADXRetailColour_Red               = 9,
	SADXRetailColour_LightGreen        = 10,
	SADXRetailColour_Purple            = 11,
	SADXRetailColour_Orange            = 12,
	SADXRetailColour_Black             = 13,
	SADXRetailColour_TransparentRed    = 14, // Unobtainable
	SADXRetailColour_TransparentPurple = 15, // Unobtainable
	SADXRetailColour_Invisible         = 16, // Unobtainable
};

enum SADXSteamColour : Sint8
{
	SADXSteamColour_Normal          = 0,
	SADXSteamColour_Yellow          = 1,
	SADXSteamColour_White           = 2,
	SADXSteamColour_Brown           = 3,
	SADXSteamColour_Aqua            = 4,
	SADXSteamColour_Pink            = 5,
	SADXSteamColour_Blue            = 6,
	SADXSteamColour_Grey            = 7,
	SADXSteamColour_Green           = 8,
	SADXSteamColour_Red             = 9,
	SADXSteamColour_LightGreen      = 10,
	SADXSteamColour_Purple          = 11,
	SADXSteamColour_Orange          = 12,
	SADXSteamColour_Black           = 13,
	SADXSteamColour_Invisible       = 14, // Unobtainable
	SADXSteamColour_TransparentGrey = 16, // Unobtainable
};

enum SA2BColour : Sint8
{
	SA2BColour_Normal    = 0,
	SA2BColour_Yellow    = 1,
	SA2BColour_White     = 2,
	SA2BColour_Brown     = 3,
	SA2BColour_SkyBlue   = 4,
	SA2BColour_Pink      = 5,
	SA2BColour_Blue      = 6,
	SA2BColour_Grey      = 7,
	SA2BColour_Green     = 8,
	SA2BColour_Red       = 9,
	SA2BColour_LimeGreen = 10,
	SA2BColour_Purple    = 11,
	SA2BColour_Orange    = 12,
	SA2BColour_Black     = 13,
};
// Values 15-255 all have various different, unobtainable colours in SA2B. However, there are far too many to list here at this time. Contact me if you happen to need more information about these.

enum SADXEggColour : Sint8
{
	SADXEggColour_Normal                   = 0,
	SADXEggColour_Yellow_MonoTone          = 1,
	SADXEggColour_White_MonoTone           = 2,
	SADXEggColour_Brown_MonoTone           = 3,
	SADXEggColour_Aqua_MonoTone            = 4,
	SADXEggColour_Pink_MonoTone            = 5,
	SADXEggColour_Blue_MonoTone            = 6,
	SADXEggColour_Grey_MonoTone            = 7,
	SADXEggColour_Green_MonoTone           = 8,
	SADXEggColour_Red_MonoTone             = 9,
	SADXEggColour_LightGreen_MonoTone      = 10,
	SADXEggColour_Purple_MonoTone          = 11,
	SADXEggColour_Orange_MonoTone          = 12,
	SADXEggColour_Black_MonoTone           = 13,
	SADXEggColour_Yellow_TwoTone           = 14,
	SADXEggColour_White_TwoTone            = 15,
	SADXEggColour_Brown_TwoTone            = 16,
	SADXEggColour_Aqua_TwoTone             = 17,
	SADXEggColour_Pink_TwoTone             = 18,
	SADXEggColour_Blue_TwoTone             = 19,
	SADXEggColour_Grey_TwoTone             = 20,
	SADXEggColour_Green_TwoTone            = 21,
	SADXEggColour_Red_TwoTone              = 22,
	SADXEggColour_LightGreen_TwoTone       = 23,
	SADXEggColour_Purple_TwoTone           = 24,
	SADXEggColour_Orange_TwoTone           = 25,
	SADXEggColour_Black_TwoTone            = 26,
	SADXEggColour_NormalShiny              = 27,
	SADXEggColour_YellowShiny_MonoTone     = 28,
	SADXEggColour_WhiteShiny_MonoTone      = 29,
	SADXEggColour_BrownShiny_MonoTone      = 30,
	SADXEggColour_AquaShiny_MonoTone       = 31,
	SADXEggColour_PinkShiny_MonoTone       = 32,
	SADXEggColour_BlueShiny_MonoTone       = 33,
	SADXEggColour_GreyShiny_MonoTone       = 34,
	SADXEggColour_GreenShiny_MonoTone      = 35,
	SADXEggColour_RedShiny_MonoTone        = 36,
	SADXEggColour_LightGreenShiny_MonoTone = 37,
	SADXEggColour_PurpleShiny_MonoTone     = 38,
	SADXEggColour_OrangeShiny_MonoTone     = 39,
	SADXEggColour_BlackShiny_MonoTone      = 40,
	SADXEggColour_YellowShiny_TwoTone      = 41,
	SADXEggColour_WhiteShiny_TwoTone       = 42,
	SADXEggColour_BrownShiny_TwoTone       = 43,
	SADXEggColour_AquaShiny_TwoTone        = 44,
	SADXEggColour_PinkShiny_TwoTone        = 45,
	SADXEggColour_BlueShiny_TwoTone        = 46,
	SADXEggColour_GreyShiny_TwoTone        = 47,
	SADXEggColour_GreenShiny_TwoTone       = 48,
	SADXEggColour_RedShiny_TwoTone         = 49,
	SADXEggColour_LightGreenShiny_TwoTone  = 50,
	SADXEggColour_PurpleShiny_TwoTone      = 51,
	SADXEggColour_OrangeShiny_TwoTone      = 52,
	SADXEggColour_BlackShiny_TwoTone       = 53,
	SADXEggColour_YellowJewel              = 54,
	SADXEggColour_WhiteJewel               = 55,
	SADXEggColour_PinkJewel                = 56,
	SADXEggColour_BlueJewel                = 57,
	SADXEggColour_GreenJewel               = 58,
	SADXEggColour_PurpleJewel              = 59,
	SADXEggColour_AquaJewel                = 60,
	SADXEggColour_RedJewel                 = 61,
	SADXEggColour_BlackJewel               = 62,
	SADXEggColour_LightGreenJewel          = 63,
	SADXEggColour_OrangeJewel              = 64,
	SADXEggColour_Pearl                    = 65, // Unobtainable
	SADXEggColour_Metal1                   = 66, // Unobtainable
	SADXEggColour_Metal2                   = 67, // Unobtainable
	SADXEggColour_Glass                    = 68, // Unobtainable
};

enum SA2BEggColour : Sint8
{
	SA2BEggColour_Normal                  = 0,
	SA2BEggColour_Yellow_MonoTone         = 1,
	SA2BEggColour_White_MonoTone          = 2,
	SA2BEggColour_Brown_MonoTone          = 3,
	SA2BEggColour_SkyBlue_MonoTone        = 4,
	SA2BEggColour_Pink_MonoTone           = 5,
	SA2BEggColour_Blue_MonoTone           = 6,
	SA2BEggColour_Grey_MonoTone           = 7,
	SA2BEggColour_Green_MonoTone          = 8,
	SA2BEggColour_Red_MonoTone            = 9,
	SA2BEggColour_LimeGreen_MonoTone      = 10,
	SA2BEggColour_Purple_MonoTone         = 11,
	SA2BEggColour_Orange_MonoTone         = 12,
	SA2BEggColour_Black_MonoTone          = 13,
	SA2BEggColour_Yellow_TwoTone          = 14,
	SA2BEggColour_White_TwoTone           = 15,
	SA2BEggColour_Brown_TwoTone           = 16,
	SA2BEggColour_SkyBlue_TwoTone         = 17,
	SA2BEggColour_Pink_TwoTone            = 18,
	SA2BEggColour_Blue_TwoTone            = 19,
	SA2BEggColour_Grey_TwoTone            = 20,
	SA2BEggColour_Green_TwoTone           = 21,
	SA2BEggColour_Red_TwoTone             = 22,
	SA2BEggColour_LimeGreen_TwoTone       = 23,
	SA2BEggColour_Purple_TwoTone          = 24,
	SA2BEggColour_Orange_TwoTone          = 25,
	SA2BEggColour_Black_TwoTone           = 26,
	SA2BEggColour_NormalShiny             = 27,
	SA2BEggColour_YellowShiny_MonoTone    = 28,
	SA2BEggColour_WhiteShiny_MonoTone     = 29,
	SA2BEggColour_BrownShiny_MonoTone     = 30,
	SA2BEggColour_SkyBlueShiny_MonoTone   = 31,
	SA2BEggColour_PinkShiny_MonoTone      = 32,
	SA2BEggColour_BlueShiny_MonoTone      = 33,
	SA2BEggColour_GreyShiny_MonoTone      = 34,
	SA2BEggColour_GreenShiny_MonoTone     = 35,
	SA2BEggColour_RedShiny_MonoTone       = 36,
	SA2BEggColour_LimeGreenShiny_MonoTone = 37,
	SA2BEggColour_PurpleShiny_MonoTone    = 38,
	SA2BEggColour_OrangeShiny_MonoTone    = 39,
	SA2BEggColour_BlackShiny_MonoTone     = 40,
	SA2BEggColour_YellowShiny_TwoTone     = 41,
	SA2BEggColour_WhiteShiny_TwoTone      = 42,
	SA2BEggColour_BrownShiny_TwoTone      = 43,
	SA2BEggColour_SkyBlueShiny_TwoTone    = 44,
	SA2BEggColour_PinkShiny_TwoTone       = 45,
	SA2BEggColour_BlueShiny_TwoTone       = 46,
	SA2BEggColour_GreyShiny_TwoTone       = 47,
	SA2BEggColour_GreenShiny_TwoTone      = 48,
	SA2BEggColour_RedShiny_TwoTone        = 49,
	SA2BEggColour_LimeGreenShiny_TwoTone  = 50,
	SA2BEggColour_PurpleShiny_TwoTone     = 51,
	SA2BEggColour_OrangeShiny_TwoTone     = 52,
	SA2BEggColour_BlackShiny_TwoTone      = 53,
	SA2BEggColour_GlitchyNormal           = 54, // Unobtainable
};

enum SADXTexture : Sint8
{
	SADXTexture_None            = 0,
	SADXTexture_YellowJewel     = 1,
	SADXTexture_WhiteJewel      = 2,
	SADXTexture_PinkJewel       = 3,
	SADXTexture_BlueJewel       = 4,
	SADXTexture_GreenJewel      = 5,
	SADXTexture_PurpleJewel     = 6,
	SADXTexture_AquaJewel       = 7,
	SADXTexture_RedJewel        = 8,
	SADXTexture_BlackJewel      = 9,
	SADXTexture_LightGreenJewel = 10,
	SADXTexture_OrangeJewel     = 11,
	SADXTexture_Pearl           = 12, // Unobtainable
	SADXTexture_Metal1          = 13, // Unobtainable
	SADXTexture_Metal2          = 14, // Unobtainable
	SADXTexture_Glass           = 15, // Unobtainable
	SADXTexture_Moon            = 16, // Unobtainable
};
// Values 17-125 all have various different, unobtainable textures. These appear to be random game textures applied to the chao. I have documented all of these in my notes, but there are far too many to list here at this time. Contact me if you happen to need more information about these.

enum SA2BTexture : Sint8
{
	SA2BTexture_None           = 0,
	SA2BTexture_YellowJewel    = 1,
	SA2BTexture_WhiteJewel     = 2,
	SA2BTexture_PinkJewel      = 3,
	SA2BTexture_BlueJewel      = 4,
	SA2BTexture_GreenJewel     = 5,
	SA2BTexture_PurpleJewel    = 6,
	SA2BTexture_SkyBlueJewel   = 7,
	SA2BTexture_RedJewel       = 8,
	SA2BTexture_BlackJewel     = 9,
	SA2BTexture_LimeGreenJewel = 10,
	SA2BTexture_OrangeJewel    = 11,
	SA2BTexture_Pearl          = 12, // Unobtainable
	SA2BTexture_Metal1         = 13, // Unobtainable
	SA2BTexture_Metal2         = 14, // Unobtainable
	SA2BTexture_Glass          = 15, // Unobtainable
	SA2BTexture_Moon           = 16, // Unobtainable
};
// Values 17-109 all have various different, unobtainable textures. These appear to be random game textures applied to the chao. I have documented all of these in my notes, but there are far too many to list here at this time. Contact me if you happen to need more information about these.

// Chao personality

enum ChaoFavouriteFruit : Sint8
{
	ChaoFavouriteFruit_RoundFruit1    = 0,
	ChaoFavouriteFruit_RoundFruit2    = 1,
	ChaoFavouriteFruit_TriangleFruit1 = 2,
	ChaoFavouriteFruit_TriangleFruit2 = 3,
	ChaoFavouriteFruit_SquareFruit1   = 4,
	ChaoFavouriteFruit_SquareFruit2   = 5,
	ChaoFavouriteFruit_None1          = 6,
	ChaoFavouriteFruit_None2          = 7,
};

// Chao learnt abilities

enum ChaoClassroomLessonFlags : Sint32
{
	ChaoClassroomLessonFlags_None          = 0,
	ChaoClassroomLessonFlags_DrawingLevel1 = 1,
	ChaoClassroomLessonFlags_DrawingLevel2 = 1 << 1,
	ChaoClassroomLessonFlags_DrawingLevel3 = 1 << 2,
	ChaoClassroomLessonFlags_DrawingLevel4 = 1 << 3,
	ChaoClassroomLessonFlags_DrawingLevel5 = 1 << 4,
	ChaoClassroomLessonFlags_ShakeDance    = 1 << 8,
	ChaoClassroomLessonFlags_SpinDance     = 1 << 9,
	ChaoClassroomLessonFlags_StepDance     = 1 << 10,
	ChaoClassroomLessonFlags_GoGoDance     = 1 << 11,
	ChaoClassroomLessonFlags_Exercise      = 1 << 12,
	ChaoClassroomLessonFlags_SongLevel1    = 1 << 16,
	ChaoClassroomLessonFlags_SongLevel2    = 1 << 17,
	ChaoClassroomLessonFlags_SongLevel3    = 1 << 18,
	ChaoClassroomLessonFlags_SongLevel4    = 1 << 19,
	ChaoClassroomLessonFlags_SongLevel5    = 1 << 20,
	ChaoClassroomLessonFlags_Bell          = 1 << 24,
	ChaoClassroomLessonFlags_Castanets     = 1 << 25,
	ChaoClassroomLessonFlags_Cymbals       = 1 << 26,
	ChaoClassroomLessonFlags_Drum          = 1 << 27,
	ChaoClassroomLessonFlags_Flute         = 1 << 28,
	ChaoClassroomLessonFlags_Maracas       = 1 << 29,
	ChaoClassroomLessonFlags_Trumpet       = 1 << 30,
	ChaoClassroomLessonFlags_Tambourine    = 1 << 31,
};

enum SADXAnimalFlags : Sint16
{
	SADXAnimalFlags_None     = 0,
	SADXAnimalFlags_Seal     = 1,
	SADXAnimalFlags_Penguin  = 1 << 1,
	SADXAnimalFlags_Otter    = 1 << 2,
	SADXAnimalFlags_Peacock  = 1 << 3,
	SADXAnimalFlags_Swallow  = 1 << 4,
	SADXAnimalFlags_Parrot   = 1 << 5,
	SADXAnimalFlags_Deer     = 1 << 6,
	SADXAnimalFlags_Rabbit   = 1 << 7,
	SADXAnimalFlags_Kangaroo = 1 << 8,
	SADXAnimalFlags_Gorilla  = 1 << 9,
	SADXAnimalFlags_Lion     = 1 << 10,
	SADXAnimalFlags_Elephant = 1 << 11,
	SADXAnimalFlags_Mole     = 1 << 12,
	SADXAnimalFlags_Koala    = 1 << 13,
	SADXAnimalFlags_Skunk    = 1 << 14,
};

enum SA2BAnimalFlags : Uint32
{
	SA2BAnimalFlags_None        = 0,
	SA2BAnimalFlags_Penguin     = 1,
	SA2BAnimalFlags_Seal        = 1 << 1,
	SA2BAnimalFlags_Otter       = 1 << 2,
	SA2BAnimalFlags_Rabbit      = 1 << 3,
	SA2BAnimalFlags_Cheetah     = 1 << 4,
	SA2BAnimalFlags_Warthog     = 1 << 5,
	SA2BAnimalFlags_Bear        = 1 << 6,
	SA2BAnimalFlags_Tiger       = 1 << 7,
	SA2BAnimalFlags_Gorilla     = 1 << 8,
	SA2BAnimalFlags_Peacock     = 1 << 9,
	SA2BAnimalFlags_Parrot      = 1 << 10,
	SA2BAnimalFlags_Condor      = 1 << 11,
	SA2BAnimalFlags_Skunk       = 1 << 12,
	SA2BAnimalFlags_Sheep       = 1 << 13,
	SA2BAnimalFlags_Raccoon     = 1 << 14,
	SA2BAnimalFlags_HalfFish    = 1 << 15,
	SA2BAnimalFlags_SkeletonDog = 1 << 16,
	SA2BAnimalFlags_Bat         = 1 << 17,
	SA2BAnimalFlags_Dragon      = 1 << 18,
	SA2BAnimalFlags_Unicorn     = 1 << 19,
	SA2BAnimalFlags_Phoenix     = 1 << 20,
};

enum ChaoToyFlags : Sint16
{
	ChaoToyFlags_None        = 0,
	ChaoToyFlags_Rattle      = 1,
	ChaoToyFlags_Car         = 1 << 1,
	ChaoToyFlags_PictureBook = 1 << 2,
	ChaoToyFlags_SonicDoll   = 1 << 4,
	ChaoToyFlags_Broomstick  = 1 << 5,
	ChaoToyFlags_Unknown     = 1 << 6, // (glitch toy)
	ChaoToyFlags_PogoStick   = 1 << 7,
	ChaoToyFlags_Crayons     = 1 << 8,
	ChaoToyFlags_BubbleWand  = 1 << 9,
	ChaoToyFlags_Shovel      = 1 << 10,
	ChaoToyFlags_WateringCan = 1 << 11,
};

// Garden items

enum ChaoItemCategory : Sint8
{
	ChaoItemCategory_Egg       = 1,
	ChaoItemCategory_Fruit     = 3,
	ChaoItemCategory_Seed      = 7, // SA2B only
	ChaoItemCategory_Hat       = 9,
	ChaoItemCategory_MenuTheme = 16, // SA2B only
};

enum SADXAnimal : Sint8
{
	SADXAnimal_None     = -1,
	SADXAnimal_Seal     = 0,
	SADXAnimal_Penguin  = 1,
	SADXAnimal_Otter    = 2,
	SADXAnimal_Peacock  = 3,
	SADXAnimal_Swallow  = 4,
	SADXAnimal_Parrot   = 5,
	SADXAnimal_Deer     = 6,
	SADXAnimal_Rabbit   = 7,
	SADXAnimal_Kangaroo = 8,
	SADXAnimal_Gorilla  = 9,
	SADXAnimal_Lion     = 10,
	SADXAnimal_Elephant = 11,
	SADXAnimal_Mole     = 12,
	SADXAnimal_Koala    = 13,
	SADXAnimal_Skunk    = 14,
};

enum SA2BAnimal : Sint8
{
	SA2BAnimal_None             = -1,
	SA2BAnimal_Penguin          = 0,
	SA2BAnimal_Seal             = 1,
	SA2BAnimal_Otter            = 2,
	SA2BAnimal_Rabbit           = 3,
	SA2BAnimal_Cheetah          = 4,
	SA2BAnimal_Warthog          = 5,
	SA2BAnimal_Bear             = 6,
	SA2BAnimal_Tiger            = 7,
	SA2BAnimal_Gorilla          = 8,
	SA2BAnimal_Peacock          = 9,
	SA2BAnimal_Parrot           = 10,
	SA2BAnimal_Condor           = 11,
	SA2BAnimal_Skunk            = 12,
	SA2BAnimal_Sheep            = 13,
	SA2BAnimal_Raccoon          = 14,
	SA2BAnimal_HalfFish         = 15,
	SA2BAnimal_SkeletonDog      = 16,
	SA2BAnimal_Bat              = 17,
	SA2BAnimal_Dragon           = 18,
	SA2BAnimal_Unicorn          = 19,
	SA2BAnimal_Phoenix          = 20,
	SA2BAnimal_YellowChaosDrive = 21,
	SA2BAnimal_GreenChaosDrive  = 22,
	SA2BAnimal_RedChaosDrive    = 23,
	SA2BAnimal_PurpleChaosDrive = 24,
};

enum SADXFruit : Sint8
{
	SADXFruit_None            = -1,
	SADXFruit_RegularFruit    = 24, // The standard garden fruit of SADX
	SADXFruit_StrongFruit     = 3, // Comes from SA2B
	SADXFruit_TastyFruit      = 4, // Comes from SA2B
	SADXFruit_HeroFruit       = 5,
	SADXFruit_DarkFruit       = 6,
	SADXFruit_RoundFruit      = 7,
	SADXFruit_TriangularFruit = 8,
	SADXFruit_CubicleFruit    = 9,
	SADXFruit_HeartFruit      = 10,
	SADXFruit_ChaoFruit       = 11,
	SADXFruit_Mushroom        = 20,
	SADXFruit_MushroomAlt     = 21, // Gives 2x the bonus of "Mushroom"
	SADXFruit_OrangeFruit     = 13, // Comes from TCG
	SADXFruit_BlueFruit       = 14, // Comes from TCG
	SADXFruit_PinkFruit       = 15, // Comes from TCG
	SADXFruit_GreenFruit      = 16, // Comes from TCG
	SADXFruit_PurpleFruit     = 17, // Comes from TCG
	SADXFruit_YellowFruit     = 18, // Comes from TCG
	SADXFruit_RedFruit        = 19, // Comes from TCG
	SADXFruit_SmartFruit      = 12, // Unobtainable
};

enum SA2BFruit : Sint8
{
	SA2BFruit_None            = -1,
	SA2BFruit_ChaoGardenFruit = 0,
	SA2BFruit_HeroGardenFruit = 1,
	SA2BFruit_DarkGardenFruit = 2,
	SA2BFruit_StrongFruit     = 3,
	SA2BFruit_TastyFruit      = 4,
	SA2BFruit_HeroFruit       = 5,
	SA2BFruit_DarkFruit       = 6,
	SA2BFruit_RoundFruit      = 7,
	SA2BFruit_TriangleFruit   = 8,
	SA2BFruit_SquareFruit     = 9,
	SA2BFruit_HeartFruit      = 10,
	SA2BFruit_ChaoFruit       = 11,
	SA2BFruit_Mushroom        = 20,
	SA2BFruit_MushroomAlt     = 21, // Gives 2x the bonus of "Mushroom"
	SA2BFruit_OrangeFruit     = 13, // Comes from TCG
	SA2BFruit_BlueFruit       = 14, // Comes from TCG
	SA2BFruit_PinkFruit       = 15, // Comes from TCG
	SA2BFruit_GreenFruit      = 16, // Comes from TCG
	SA2BFruit_PurpleFruit     = 17, // Comes from TCG
	SA2BFruit_YellowFruit     = 18, // Comes from TCG
	SA2BFruit_RedFruit        = 19, // Comes from TCG
	SA2BFruit_SmartFruit      = 12, // Unobtainable
	SA2BFruit_MintCandy       = 22, // Unobtainable
	SA2BFruit_Grapes          = 23, // Unobtainable
};

enum SADXHat : Sint8
{
	SADXHat_None                              = 0,
	SADXHat_Pumpkin                           = 1,
	SADXHat_Skull                             = 2,
	SADXHat_Apple                             = 3,
	SADXHat_Bucket                            = 4,
	SADXHat_EmptyCan                          = 5,
	SADXHat_CardboardBox                      = 6,
	SADXHat_FlowerPot                         = 7,
	SADXHat_PaperBag                          = 8,
	SADXHat_Pan                               = 9,
	SADXHat_Stump                             = 10,
	SADXHat_Watermelon                        = 11,
	SADXHat_RedWoolBeanie                     = 12,
	SADXHat_BlueWoolBeanie                    = 13,
	SADXHat_BlackWoolBeanie                   = 14,
	SADXHat_Pacifier                          = 15,
	SADXHat_NormalEggShell                    = 16,
	SADXHat_Yellow_MonoTone_EggShell          = 17,
	SADXHat_White_MonoTone_EggShell           = 18,
	SADXHat_Brown_MonoTone_EggShell           = 19,
	SADXHat_Aqua_MonoTone_EggShell            = 20,
	SADXHat_Pink_MonoTone_EggShell            = 21,
	SADXHat_Blue_MonoTone_EggShell            = 22,
	SADXHat_Grey_MonoTone_EggShell            = 23,
	SADXHat_Green_MonoTone_EggShell           = 24,
	SADXHat_Red_MonoTone_EggShell             = 25,
	SADXHat_LightGreen_MonoTone_EggShell      = 26,
	SADXHat_Purple_MonoTone_EggShell          = 27,
	SADXHat_Orange_MonoTone_EggShell          = 28,
	SADXHat_Black_MonoTone_EggShell           = 29,
	SADXHat_Yellow_TwoTone_EggShell           = 30,
	SADXHat_White_TwoTone_EggShell            = 31,
	SADXHat_Brown_TwoTone_EggShell            = 32,
	SADXHat_Aqua_TwoTone_EggShell             = 33,
	SADXHat_Pink_TwoTone_EggShell             = 34,
	SADXHat_Blue_TwoTone_EggShell             = 35,
	SADXHat_Grey_TwoTone_EggShell             = 36,
	SADXHat_Green_TwoTone_EggShell            = 37,
	SADXHat_Red_TwoTone_EggShell              = 38,
	SADXHat_LightGreen_TwoTone_EggShell       = 39,
	SADXHat_Purple_TwoTone_EggShell           = 40,
	SADXHat_Orange_TwoTone_EggShell           = 41,
	SADXHat_Black_TwoTone_EggShell            = 42,
	SADXHat_NormalShinyEggShell               = 43,
	SADXHat_YellowShiny_MonoTone_EggShell     = 44,
	SADXHat_WhiteShiny_MonoTone_EggShell      = 45,
	SADXHat_BrownShiny_MonoTone_EggShell      = 46,
	SADXHat_AquaShiny_MonoTone_EggShell       = 47,
	SADXHat_PinkShiny_MonoTone_EggShell       = 48,
	SADXHat_BlueShiny_MonoTone_EggShell       = 49,
	SADXHat_GreyShiny_MonoTone_EggShell       = 50,
	SADXHat_GreenShiny_MonoTone_EggShell      = 51,
	SADXHat_RedShiny_MonoTone_EggShell        = 52,
	SADXHat_LightGreenShiny_MonoTone_EggShell = 53,
	SADXHat_PurpleShiny_MonoTone_EggShell     = 54,
	SADXHat_OrangeShiny_MonoTone_EggShell     = 55,
	SADXHat_BlackShiny_MonoTone_EggShell      = 56,
	SADXHat_YellowShiny_TwoTone_EggShell      = 57,
	SADXHat_WhiteShiny_TwoTone_EggShell       = 58,
	SADXHat_BrownShiny_TwoTone_EggShell       = 59,
	SADXHat_AquaShiny_TwoTone_EggShell        = 60,
	SADXHat_PinkShiny_TwoTone_EggShell        = 61,
	SADXHat_BlueShiny_TwoTone_EggShell        = 62,
	SADXHat_GreyShiny_TwoTone_EggShell        = 63,
	SADXHat_GreenShiny_TwoTone_EggShell       = 64,
	SADXHat_RedShiny_TwoTone_EggShell         = 65,
	SADXHat_LightGreenShiny_TwoTone_EggShell  = 66,
	SADXHat_PurpleShiny_TwoTone_EggShell      = 67,
	SADXHat_OrangeShiny_TwoTone_EggShell      = 68,
	SADXHat_BlackShiny_TwoTone_EggShell       = 69,
	SADXHat_YellowJewelEggShell               = 70,
	SADXHat_WhiteJewelEggShell                = 71,
	SADXHat_PinkJewelEggShell                 = 72,
	SADXHat_BlueJewelEggShell                 = 73,
	SADXHat_GreenJewelEggShell                = 74,
	SADXHat_PurpleJewelEggShell               = 75,
	SADXHat_AquaJewelEggShell                 = 76,
	SADXHat_RedJewelEggShell                  = 77,
	SADXHat_BlackJewelEggShell                = 78,
	SADXHat_LightGreenJewelEggShell           = 79,
	SADXHat_OrangeJewelEggShell               = 80,
	SADXHat_PearlEggShell                     = 81, // Unobtainable
	SADXHat_Metal1EggShell                    = 82, // Unobtainable
	SADXHat_Metal2EggShell                    = 83, // Unobtainable
	SADXHat_GlassEggShell                     = 84, // Unobtainable
};

enum SA2BHat : Sint8
{
	SA2BHat_None                             = 0,
	SA2BHat_Pumpkin                          = 1,
	SA2BHat_Skull                            = 2,
	SA2BHat_Apple                            = 3,
	SA2BHat_Bucket                           = 4,
	SA2BHat_EmptyCan                         = 5,
	SA2BHat_CardboardBox                     = 6,
	SA2BHat_FlowerPot                        = 7,
	SA2BHat_PaperBag                         = 8,
	SA2BHat_Pan                              = 9,
	SA2BHat_Stump                            = 10,
	SA2BHat_Watermelon                       = 11,
	SA2BHat_RedWoolBeanie                    = 12,
	SA2BHat_BlueWoolBeanie                   = 13,
	SA2BHat_BlackWoolBeanie                  = 14,
	SA2BHat_Pacifier                         = 15,
	SA2BHat_NormalEggShell                   = 16,
	SA2BHat_Yellow_MonoTone_EggShell         = 17,
	SA2BHat_White_MonoTone_EggShell          = 18,
	SA2BHat_Brown_MonoTone_EggShell          = 19,
	SA2BHat_SkyBlue_MonoTone_EggShell        = 20,
	SA2BHat_Pink_MonoTone_EggShell           = 21,
	SA2BHat_Blue_MonoTone_EggShell           = 22,
	SA2BHat_Grey_MonoTone_EggShell           = 23,
	SA2BHat_Green_MonoTone_EggShell          = 24,
	SA2BHat_Red_MonoTone_EggShell            = 25,
	SA2BHat_LimeGreen_MonoTone_EggShell      = 26,
	SA2BHat_Purple_MonoTone_EggShell         = 27,
	SA2BHat_Orange_MonoTone_EggShell         = 28,
	SA2BHat_Black_MonoTone_EggShell          = 29,
	SA2BHat_Yellow_TwoTone_EggShell          = 30,
	SA2BHat_White_TwoTone_EggShell           = 31,
	SA2BHat_Brown_TwoTone_EggShell           = 32,
	SA2BHat_SkyBlue_TwoTone_EggShell         = 33,
	SA2BHat_Pink_TwoTone_EggShell            = 34,
	SA2BHat_Blue_TwoTone_EggShell            = 35,
	SA2BHat_Grey_TwoTone_EggShell            = 36,
	SA2BHat_Green_TwoTone_EggShell           = 37,
	SA2BHat_Red_TwoTone_EggShell             = 38,
	SA2BHat_LimeGreen_TwoTone_EggShell       = 39,
	SA2BHat_Purple_TwoTone_EggShell          = 40,
	SA2BHat_Orange_TwoTone_EggShell          = 41,
	SA2BHat_Black_TwoTone_EggShell           = 42,
	SA2BHat_NormalShinyEggShell              = 43,
	SA2BHat_YellowShiny_MonoTone_EggShell    = 44,
	SA2BHat_WhiteShiny_MonoTone_EggShell     = 45,
	SA2BHat_BrownShiny_MonoTone_EggShell     = 46,
	SA2BHat_SkyBlueShiny_MonoTone_EggShell   = 47,
	SA2BHat_PinkShiny_MonoTone_EggShell      = 48,
	SA2BHat_BlueShiny_MonoTone_EggShell      = 49,
	SA2BHat_GreyShiny_MonoTone_EggShell      = 50,
	SA2BHat_GreenShiny_MonoTone_EggShell     = 51,
	SA2BHat_RedShiny_MonoTone_EggShell       = 52,
	SA2BHat_LimeGreenShiny_MonoTone_EggShell = 53,
	SA2BHat_PurpleShiny_MonoTone_EggShell    = 54,
	SA2BHat_OrangeShiny_MonoTone_EggShell    = 55,
	SA2BHat_BlackShiny_MonoTone_EggShell     = 56,
	SA2BHat_YellowShiny_TwoTone_EggShell     = 57,
	SA2BHat_WhiteShiny_TwoTone_EggShell      = 58,
	SA2BHat_BrownShiny_TwoTone_EggShell      = 59,
	SA2BHat_SkyBlueShiny_TwoTone_EggShell    = 60,
	SA2BHat_PinkShiny_TwoTone_EggShell       = 61,
	SA2BHat_BlueShiny_TwoTone_EggShell       = 62,
	SA2BHat_GreyShiny_TwoTone_EggShell       = 63,
	SA2BHat_GreenShiny_TwoTone_EggShell      = 64,
	SA2BHat_RedShiny_TwoTone_EggShell        = 65,
	SA2BHat_LimeGreenShiny_TwoTone_EggShell  = 66,
	SA2BHat_PurpleShiny_TwoTone_EggShell     = 67,
	SA2BHat_OrangeShiny_TwoTone_EggShell     = 68,
	SA2BHat_BlackShiny_TwoTone_EggShell      = 69,
	SA2BHat_GlitchyNormalEggShell            = 70,
};

enum ChaoSeed : Sint8
{
	ChaoSeed_None = -1,
	ChaoSeed_StrongSeed = 0,
	ChaoSeed_TastySeed = 1,
	ChaoSeed_HeroSeed = 2,
	ChaoSeed_DarkTheme = 3,
	ChaoSeed_RoundSeed = 4,
	ChaoSeed_TriangleTheme = 5,
	ChaoSeed_SquareSeed = 6,
};

enum ChaoStatIndex
{
	ChaoStatIndex_Swim         = 0x0,
	ChaoStatIndex_Fly          = 0x1,
	ChaoStatIndex_Run          = 0x2,
	ChaoStatIndex_Power        = 0x3,
	ChaoStatIndex_Stamina      = 0x4,
	ChaoStatIndex_Luck         = 0x5,
	ChaoStatIndex_Intelligence = 0x6,
	ChaoStatIndex_Unknown      = 0x7,
};

enum Theme : Sint8
{
	Theme_Omochao = 0,
	Theme_Amy = 1,
	Theme_Maria = 2,
};

// Draw Queue

enum QueuedModelType : __int8
{
	QueuedModelType_00             = 0x0,
	QueuedModelType_BasicModel     = 0x1,
	QueuedModelType_Sprite2D       = 0x2,
	QueuedModelType_Sprite3D       = 0x3,
	QueuedModelType_Line3D         = 0x4,
	QueuedModelType_3DLinesMaybe   = 0x5,
	QueuedModelType_2DLinesMaybe   = 0x6,
	QueuedModelType_3DTriFanThing  = 0x7,
	QueuedModelType_ActionPtr      = 0x8,
	QueuedModelType_Rect           = 0x9,
	QueuedModelType_Object         = 0xA,
	QueuedModelType_Action         = 0xB,
	QueuedModelType_Callback       = 0xC,
	QueuedModelType_TextureMemList = 0xD,
	QueuedModelType_Line2D         = 0xE,
	QueuedModelType_MotionThing    = 0xF,
};

enum QueuedModelFlags
{
	QueuedModelFlags_ZTestWrite = 0x10,
	QueuedModelFlags_FogEnabled = 0x20,
	QueuedModelFlags_02         = 0x40,
	QueuedModelFlags_03         = 0x80,
};

enum QueuedModelFlagsB : __int8
{
	QueuedModelFlagsB_EnableZWrite     = 0x1,
	QueuedModelFlagsB_NoColor          = 0x2,
	QueuedModelFlagsB_SomeTextureThing = 0x4,
	QueuedModelFlagsB_3                = 0x8,
	QueuedModelFlagsB_AlwaysShow       = 0x10,
};

enum RenderFlags : int
{
	EnvironmentMap   = 0x1,
	ConstantMaterial = 0x2,
	OffsetMaterial   = 0x4,
	RenderFlags_8    = 0x8,
	RenderFlags_10   = 0x10,
};

#endif /* SADXMODLOADER_SADXENUMS_H */
