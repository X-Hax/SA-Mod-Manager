/**
 * SADX Mod Loader.
 * SADX enums.
 */

#ifndef SADXMODLOADER_SADXENUMS_H
#define SADXMODLOADER_SADXENUMS_H

// SADX Enums
#define makemask(enumname,membername) enumname##_##membername = (1 << enumname##Bits_##membername)
#define makemasks(enumname,membername) enumname##s_##membername = (1 << enumname##Bits_##membername)

enum StatusBits
{
	StatusBits_Ground,
	StatusBits_Unknown1,
	StatusBits_Hurt,
	StatusBits_ObjectInteract,
	StatusBits_Unkown2,
	StatusBits_Unkown3,
	StatusBits_Unkown4,
	StatusBits_Unkown5,
	StatusBits_Ball,
	StatusBits_LightDash,
	StatusBits_Attack,
	StatusBits_HoldObject,
	StatusBits_DoNextAction,
	StatusBits_OnPath,
	StatusBits_DisableControl,
	StatusBits_Unkown7
};

enum Status : uint16_t
{
	makemask(Status, Ground),
	makemask(Status, Unknown1),
	makemask(Status, Hurt),
	makemask(Status, ObjectInteract),
	makemask(Status, Unkown2),
	makemask(Status, Unkown3),
	makemask(Status, Unkown4),
	makemask(Status, Unkown5),
	makemask(Status, Ball),
	makemask(Status, LightDash),
	makemask(Status, Attack),
	makemask(Status, HoldObject),
	makemask(Status, DoNextAction),
	makemask(Status, OnPath),
	makemask(Status, DisableControl),
	makemask(Status, Unkown7)
};

enum PowerupBits
{
	PowerupBits_Barrier,
	PowerupBits_MagneticBarrier,
	PowerupBits_Invincibility = 15
};

enum Powerups : uint16_t
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

#endif /* SADXMODLOADER_SADXENUMS_H */
