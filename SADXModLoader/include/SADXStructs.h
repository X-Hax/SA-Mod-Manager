/**
 * SADX Mod Loader.
 * SADX structs.
 */

#ifndef SADXMODLOADER_SADXSTRUCTS_H
#define SADXMODLOADER_SADXSTRUCTS_H

#include <stdint.h>
#include "ninja.h"
#include "d3d8types.h"
#include "SADXEnums.h"

// All structs should be packed.
#pragma pack(push, 1)

typedef struct Rotation {
	int x, y, z;
} Rotation3;

struct ObjectMaster;
typedef void(__cdecl *ObjectFuncPtr)(ObjectMaster *);
typedef void(__cdecl *VBufferFuncPtr)(NJS_MESHSET_SADX*, NJS_POINT3*, NJS_VECTOR*);
typedef const Float* NJS_MATRIX_CONST_PTR;

// TODO: Grab actual structs from disassembly.
typedef void ABC_TXT_struct;
typedef void ChaoHudThing;
typedef void PDS_VIBPARAM;
typedef void pvm_thing;
typedef void struct_1;
typedef void what;

struct AllocatedMem
{
	AllocatedMem *next;
	AllocatedMem *field_4;
	AllocatedMem *field_8;
	int PointsToLastThingMaybe;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	int data;
	int field_24;
	int field_28;
	int field_2C;
	int field_30;
	int field_34;
	int field_38;
	int field_3C;
};

struct SaveFileInfo
{
	const char *Filename;
	uint32_t LowDate;
	uint32_t HighDate;
	SaveFileInfo *Next;
};

struct LineInfo
{
	NJS_POINT3 *Points;
	int *Colors;
	int field_8;
	int Count;
};

struct FogData
{
	float Layer;
	float Distance;
	int Color;
	int Toggle;
};

struct PVMEntry
{
	const char* Name;
	NJS_TEXLIST *TexList;
};

struct LevelPVMList
{
	int16_t Level;
	int16_t NumTextures;
	PVMEntry *PVMList;
};

struct ObjectListEntry
{
	char Flags;
	char ObjectListIndex;
	short UseDistance;
	float Distance;
	int field_8;
	ObjectFuncPtr LoadSub;
	const char *Name;
};

struct StartPosition
{
	int16_t LevelID;
	int16_t ActID;
	NJS_VECTOR Position;
	int YRot;
};

struct ControllerData
{
	uint32_t ID;
	uint32_t Support;
	uint32_t HeldButtons;
	uint32_t NotHeldButtons;
	uint32_t PressedButtons;
	uint32_t ReleasedButtons;
	uint16_t RTriggerPressure;
	uint16_t LTriggerPressure;
	int16_t LeftStickX;
	int16_t LeftStickY;
	int16_t RightStickX;
	int16_t RightStickY;
	char* Name;
	void* Extend;
	uint32_t Old;
	void* Info;
};

struct PolarCoord
{
	Angle direction;
	float magnitude;
};

struct SETEntry
{
	int16_t ObjectType;
	int16_t XRotation;
	int16_t YRotation;
	int16_t ZRotation;
	NJS_VECTOR Position;
	NJS_VECTOR Properties;
};

struct RecapScreen
{
	float Speed;
	int LineCount;
	const char **TextData;
};

struct COL
{
	NJS_VECTOR Center;
	float Radius;
	int Padding[2];
	NJS_OBJECT *Model;
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
	NJS_TEXLIST *TexList;
};

struct LandTable
{
	int16_t COLCount;
	int16_t AnimCount;
	// see LandTableFlags enum
	int Flags;
	float Unknown_1;
	COL *Col;
	GeoAnimData *AnimData;
	const char *TexName;
	NJS_TEXLIST *TexList;
	int Unknown_4;
	int Unknown_5;
};

struct P2Path
{
	int field_0;
	NJS_VECTOR Position;
};

struct Loop
{
	int16_t Ang_X;
	int16_t Ang_Y;
	float Dist;
	NJS_VECTOR Position;
};

struct LoopHead
{
	int16_t Unknown_0;
	int16_t Count;
	float TotalDist;
	Loop *LoopList;
	ObjectFuncPtr Object;
};

struct AnimData_t
{
	NJS_ACTION *Animation;
	char Instance;
	char Property;
	short NextAnim;
	float TransitionSpeed;
	float AnimationSpeed;
};

struct PhysicsData_t
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

struct WeldInfo
{
	NJS_OBJECT *BaseModel;
	NJS_OBJECT *ModelA;
	NJS_OBJECT *ModelB;
	char VertexPairCount;
	char WeldType;
	short anonymous_5;
	NJS_VECTOR *VertexBuffer;
	unsigned short *VertIndexes;
};

struct OffsetAnimData
{
	int FrameNum;
	void *anonymous_0;
};

struct AnimFrame_1
{
	void *Item1Off;
	int Item1Count;
};

struct AnimFrame_2
{
	void *Item1Off;
	void *Item2Off;
	int Item1Count;
	int Item2Count;
};

struct AnimFrame_3
{
	void *Item1Off;
	void *Item2Off;
	void *Item3Off;
	int Item1Count;
	int Item2Count;
	int Item3Count;
};

struct AnimThing
{
	short State;
	short field_2;
	short Index;
	short LastIndex;
	short LastIndex2;
	short field_A;
	short field_C;
	short field_E;
	float Frame;
	int dword14;
	float *pfloat18;
	float *pfloat1C;
	AnimData_t *AnimData;
	WeldInfo *WeldInfo;
	NJS_ACTION *action;
};

struct struct_a3
{
	int ShadowRotationX;
	int ShadowRotationY;
	int ColFlagsA;
	int ColFlagsB;
	float DistanceMin;
	float DistanceMax;
	int ColFlagsC;
	int ColFlagsD;
	float ShadowScale;
};

struct CharObj2
{
	float SpindashSpeed;
	short Upgrades;
	short Powerups;
	short JumpTime;
	short field_A;
	short UnderwaterTime;
	short IdleTime;
	short StatusBackup;
	int field_12;
	char gap16[2];
	float LoopDist;
	float Up;
	NJS_VECTOR SomeKindOfSpeedOffset;
	NJS_VECTOR field_2C;
	NJS_VECTOR Speed;
	NJS_VECTOR field_44;
	NJS_VECTOR SurfaceNormal;
	int SurfaceFlags;
	int SurfaceFlags_Old;
	void *array_1x132;
	ObjectMaster *ObjectHeld;
	void *field_6C;
	void *SomePointer;
	int field_74;
	void *array_15x32;
	short SonicSpinTimeProbably;
	short SonicSpinTimer;
	short LightdashTime;
	short LightdashTimer;
	int field_84;
	NJS_VECTOR field_88;
	float SomeFrameNumberThing;
	float TailsFlightTime;
	PhysicsData_t PhysicsData;
	AnimThing AnimationThing;
	NJS_VECTOR SoManyVectors[12];
	struct_a3 _struct_a3;
};

struct CollisionData
{
	char field_0;
	char field_1;
	char field_2;
	char field_3;
	int field_4;
	NJS_VECTOR origin;
	NJS_VECTOR scale;
	int field_20;
	Rotation3 rotation;
};

struct EntityData1;
struct CollisionThing
{
	char field_0;
	char field_1;
	short FlagsMaybe;
	EntityData1 *Entity;
};

struct CollisionInfo
{
	short List;
	short ThingCount;
	short Flags;
	short Count;
	float Radius;
	CollisionData *CollisionArray;
	CollisionThing CollisionThings[16];
	int CollisionThingsEnd;
	int field_94;
	int field_98;
	ObjectMaster *Object;
	short field_A0;
	short field_A2;
	CollisionInfo *CollidingObject;
};

struct DynamicCOL
{
	int Flags;
	NJS_OBJECT *Model;
	ObjectMaster *Entity;
};

struct EntityData1
{
	char Action;
	char NextAction;
	char Unknown;
	char Index;
	short Status;
	short InvulnerableTime;
	char CharIndex;
	char CharID;
	short field_A;
	NJS_OBJECT* Object;
	Loop* LoopData;
	Rotation3 Rotation;
	NJS_VECTOR Position;
	NJS_VECTOR Scale;
	CollisionInfo* CollisionInfo;
	void *field_3C;
};

struct HomingAttackTarget
{
	EntityData1 *entity;
	float distance;
};

struct EntityData2
{
	CharObj2 *CharacterData;
	NJS_VECTOR VelocityDirection;
	NJS_VECTOR SomeCollisionVector;
	Rotation3 Forward;
	int field_28;
	int field_2C;
	int field_30;
	float field_34;
	float field_38;
	float SomeMultiplier;
};

struct ObjectData2
{
	char gap_0[4];
	NJS_VECTOR field_4;
	char gap_10[62];
	short field_4E;
	NJS_VECTOR vector_a;
	NJS_VECTOR vector_b;
	NJS_VECTOR vector_c;
	char gap_74[4];
	float field_78;
	char gap_7C[16];
	float field_8C;
	char gap_90[12];
	int field_9C;
	float field_A0;
	int field_A4;
	float field_A8;
	float field_AC;
	float field_B0;
	float field_B4;
	float field_B8;
	float field_BC;
	float field_C0;
	float field_C4;
	int field_C8;
	char gap_CC[220];
	int field_1A8;
	char gap_1AC[4];
	int field_1B0;
	char gap_1B4[28];
	int field_1D0;
	int field_1D4;
	float field_1D8;
	float field_1DC;
	float field_1E0;
	char gap_1E4[16];
	NJS_ACTION *AnimHead_ptr_A;
	NJS_ACTION *AnimHead_ptr_B;
	NJS_MATRIX matrix;
};

struct SETObjData
{
	char LoadCount;
	Uint8 f1;
	short Flags;
	ObjectMaster *ObjInstance;
	SETEntry *SETEntry;
	float Distance;
};

struct PRM_Entry
{
	char Mission;
	char Display;
	char List;
	char field_3;
	char Goal;
	char NumItems;
	char TimerSecs;
	char field_7;
	char Appearance;
	char field_9;
	char field_A;
	char field_B;
};

struct MissionSETData : SETObjData
{
	PRM_Entry *PRMEntry;
};

union SETDataUnion
{
	SETObjData *SETData;
	MissionSETData *MissionSETData;
};

struct ObjectMaster
{
	ObjectMaster *Next;
	ObjectMaster *Previous;
	ObjectMaster *Parent;
	ObjectMaster *Child;
	ObjectFuncPtr MainSub;
	ObjectFuncPtr DisplaySub;
	ObjectFuncPtr DeleteSub;
	SETDataUnion SETData;
	EntityData1 *Data1;
	void *Data2;
	void *UnknownA_ptr;
	void *UnknownB_ptr;
	int field_30;
};

struct VideoData
{
	const char *Filename;
	int field_4;
	int NumFrames;
	int16_t Width;
	int16_t Height;
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
	NJS_TEXLIST *TextureList;
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

struct PaletteLight
{
	Uint8 Level;
	Uint8 Act;
	Uint8 Type;
	Uint8 Flags;
	NJS_VECTOR Direction;
	float DIF;     // (0~4 ) Diffuse
	float AMB_R;   // (0~4 ) Ambient R
	float AMB_G;   // (0~4 ) Ambient G
	float AMB_B;   // (0~4 ) Ambient B
	float CO_pow;  // (0~99) [unused] Upper Color Power (higher is less visible)
	float CO_R;    // (0~4 ) [unused] Upper Color R
	float CO_G;    // (0~4 ) [unused] Upper Color G
	float CO_B;    // (0~4 ) [unused] Upper Color B
	float SP_pow;  // (0~99) Upper Specular Power (higher is less visible)
	float SP_R;    // (0~4 ) Upper Specular R
	float SP_G;    // (0~4 ) Upper Specular G
	float SP_B;    // (0~4 ) Upper Specular B
	float CO2_pow; // (0~99) [unused] Lower Color Power (higher is less visible)
	float CO2_R;   // (0~4 ) [unused] Lower Color R
	float CO2_G;   // (0~4 ) [unused] Lower Color G
	float CO2_B;   // (0~4 ) [unused] Lower Color B
	float SP2_pow; // (0~99) [unused] Lower Specular Power (higher is less visible)
	float SP2_R;   // (0~4 ) [unused] Lower Specular R
	float SP2_G;   // (0~4 ) [unused] Lower Specular G
	float SP2_B;   // (0~4 ) [unused] Lower Specular B
};

struct SecondaryEntrance
{
	NJS_VECTOR Position;
	int YRot;
};

struct TextureAnimationData
{
	NJS_OBJECT *Model;
	char MatCount;
	char MatIDs[8];
	char TexID;
	int16_t TexTime;
};

struct LevelClearFlagData
{
	int16_t Level;
	int16_t FlagOffset;
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
	int16_t LevelID;
	int16_t FieldID;
	NJS_VECTOR Position;
	int YRot;
};

struct LevelCutsceneData
{
	char Level;
	char Act;
	int16_t Character;
	char field_4;
	char field_5;
	int16_t Cutscene;
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
	const char *Message;
	int Time;
};

struct HintText_Entry
{
	int16_t *Properties;
	HintText_Text *Text;
};

struct AdventureData_t
{
	char TimeOfDay;
	char field_1;
	int16_t field_2;
	int16_t field_4;
	int16_t Entrance;
	int16_t LevelAct;
	int16_t field_A;
};

struct SceneSelectData
{
	void *Code;
	char Level;
	char Act;
	int16_t Entrance;
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
	const char *Filename;
};

struct SoundList
{
	int Count;
	SoundFileInfo *List;
};

typedef void(__cdecl *ItemBoxFunction)(EntityData1 *);
struct ItemBoxData
{
	int Texture;
	ItemBoxFunction Function;
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
	int16_t BestWeights[12];
	int16_t anonymous_4[8];
	int16_t MostRings[32];
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
	int16_t LastLevel;
	char gap_25e[2];
	char EventFlags[64];
	char NPCFlags[64];
	char gap_2e0[8];
	AdventureData_t AdventureData[8];
	char LevelClear[344];
	char MissionFlags[60];
	int BlackMarketRings;
	int MetalHighScores[10];
	Time MetalBestTimes[10];
	int16_t MetalMostRings[10];
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
	int16_t Level;
	char Minute;
	char Second;
	char Frame;
	char anonymous_0[3];
	int CheckpointHit;
	NJS_VECTOR Position;
	Rotation3 Rotation;
};

struct MESFileText
{
	int16_t *field_0;
	const char **field_4;
};

struct MESFileHeader
{
	int Count;
	MESFileText *Texts;
};

struct FieldNPCDataYRot
{
	NJS_VECTOR Position;
	int16_t YRot;
	char TextNum;
	char CharacterNum;
};

struct FieldNPCDataXYZRot
{
	NJS_VECTOR Position;
	int16_t XRot;
	int16_t YRot;
	int16_t ZRot;
	char TextNum;
	char CharacterNum;
};

struct FieldNPCList
{
	void *Data;
	int Count;
};

struct MusicInfo
{
	const char *Name;
	int Loop;
};

struct SoundTestEntry
{
	const char *Name;
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
	NJS_VECTOR Far, Normal, Near;
};

struct StageLightData
{
	char level;
	char act;
	char index;
	char use_direction;
	NJS_VECTOR direction;
	float specular;
	float multiplier;
	float diffuse[3];
	float ambient[3];
};

struct OceanData
{
	NJS_VECTOR Position;
	char TextureIndex;
	char OtherIndexIdk;
	char PlaneCount;
	char PrimitiveCount;
	NJS_POINT2 Offset;
	char VBuffIndex;
	char field_19;
	char field_1A;
	char field_1B;
};

struct CreditsInfo
{
	const char **names;
	NJS_TEXLIST **texlists;
	int count;
	const char *endimagename;
	NJS_TEXLIST *endtexlist;
	short *coordinates;
};

struct CreditsEntry
{
	char SomeIndex;
	char field_1;
	char field_2;
	char field_3;
	const char *Line;
};

struct CreditsList
{
	CreditsEntry *Entries;
	int Count;
};

// Chao

struct ChaoCharacterBond
{
	char a;
	char b;
	int unknown;
};

struct ChaoDNA
{
	char ResetTrigger;
	char field_1[91];
	char SwimStatGrade1;
	char SwimStatGrade2;
	char FlyStatGrade1;
	char FlyStatGrade2;
	char RunStatGrade1;
	char RunStatGrade2;
	char PowerStatGrade1;
	char PowerStatGrade2;
	char StaminaStatGrade1;
	char StaminaStatGrade2;
	char LuckStatGrade1;
	char LuckStatGrade2;
	char IntelligenceStatGrade1;
	char IntelligenceStatGrade2;
	char UnknownStatGrade1;
	char UnknownStatGrade2;
	char field_4A4[34];
	char FavoriteFruit1;
	char FavoriteFruit2;
	char field_4C8[4];
	char Color1;
	char Color2;
	char MonotoneFlag1;
	char MonotoneFlag2;
	char Texture1;
	char Texture2;
	char ShinyFlag1;
	char ShinyFlag2;
	char EggColor1;
	char EggColor2;
	char gap_4D6[6];
};

struct ChaoDataBase
{
	char gap_0[18];
	char Name[7];
	char gap_19[7];
	char SwimFraction;
	char FlyFraction;
	char RunFraction;
	char PowerFraction;
	char StaminaFraction;
	char LuckyFraction;
	char IntelligenceFraction;
	char UnknownFraction;
	char SwimGrade;
	char FlyGrade;
	char RunGrade;
	char PowerGrade;
	char StaminaGrade;
	char LuckyGrade;
	char IntelligenceGrade;
	char UnknownGrade;
	char SwimLevel;
	char FlyLevel;
	char RunLevel;
	char PowerLevel;
	char StaminaLevel;
	char LuckLevel;
	char IntelligenceLevel;
	char UnknownLevel;
	short SwimStat;
	short FlyStat;
	short RunStat;
	short PowerStat;
	short StaminaStat;
	short LuckStat;
	char IntelligenceStat;
	char UnknownStat;
	char field_46[58];
	ChaoType Type;
	char Garden;
	short Happiness;
	short field_84;
	short ClockRollovers;
	short field_88;
	short Lifespan;
	short Lifespan2;
	short Reincarnations;
	char field_90[24];
	float PowerRun;
	float FlySwim;
	float Alignment;
	char gap_B4[12];
	float EvolutionProgress;
	char gap_C4[13];
	char EyeType;
	char MouthType;
	char BallType;
	char gap_D4[1];
	char Headgear;
	char HideFeet;
	char Medal;
	char Color;
	char MonotoneHighlights;
	char Texture;
	char Shiny;
	char EggColor;
	SADXBodyType BodyType;
	char BodyTypeAnimal;
	char field_DF[57];
	int SA2AnimalBehavior;
	SA2BAnimal SA2BArmType;
	SA2BAnimal SA2BEarType;
	SA2BAnimal SA2BForeheadType;
	SA2BAnimal SA2BHornType;
	SA2BAnimal SA2BLegType;
	SA2BAnimal SA2BTailType;
	SA2BAnimal SA2BWingType;
	SA2BAnimal SA2BFaceType;
	char field_124[8];
	char Joy;
	char field_12D;
	char UrgeToCry;
	char Fear;
	char field_130;
	char Dizziness;
	char field_132[2];
	short Sleepiness;
	short Tiredness;
	short Hunger;
	short MateDesire;
	short Boredom;
	char field_13E[10];
	short Energy;
	char Normal_Curiosity;
	char field_14B;
	char CryBaby_Energetic;
	char Naive_Normal;
	char field_14E[2];
	char Normal_BigEater;
	char field_151[4];
	char Normal_Carefree;
	char field_156;
	char FavoriteFruit;
	char field_158[2];
	char CoughLevel;
	char ColdLevel;
	char RashLevel;
	char RunnyNoseLevel;
	char HiccupsLevel;
	char StomachAcheLevel;
	char field_160[4];
	short SA2BToys;
	char field_166[6];
	ChaoCharacterBond SA2BCharacterBonds[6];
	char field_190[680];
	ChaoDNA DNA;
	int field_4DC;
	int SADXAnimalBehaviors;
	SADXAnimal ArmType;
	SADXAnimal EarType;
	SADXAnimal EyebrowType;
	SADXAnimal ForeheadType;
	SADXAnimal LegType;
	SADXAnimal TailType;
	SADXAnimal WingType;
	SADXAnimal UnknownType;
	char field_4EC[16];
	ChaoCharacterBond SADXCharacterBonds[6];
};

struct ChaoData
{
	ChaoDataBase data;
	char field_520[736];
};

struct ChaoUnknownB
{
	short field_0;
	short field_2;
	short field_4;
	short field_6;
	float field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
};

struct ChaoUnknown
{
	short field_0;
	short field_2;
	int field_4;
	int field_8;
	int field_C;
	float field_10;
	short field_14;
	short field_16;
	ChaoUnknownB field_18[32];
};

struct ChaoUnknownE
{
	char pad[960];
};

struct ChaoUnknownD
{
	short field_0;
	short field_2;
	short field_4;
	short field_6;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
};

struct ChaoData1
{
	EntityData1 entity;
	char field_40[8];
	ObjectMaster *ObjectMaster_ptr1;
	char field_4C[4];
	ObjectMaster *ObjectMaster_ptr2;
	char field_54[12];
	int field_60;
	char field_64[4];
	int field_68;
	ChaoDataBase *ChaoDataBase_ptr;
	char field_70[64];
	int field_B0;
	char field_B4[4];
	int field_B8;
	char MotionTable[264];
	ChaoUnknownD unknown_d[7];
	char field_288[424];
	int field_430;
	char field_434[16];
	int field_444;
	char field_448[8];
	int field_450;
	char field_454[208];
	int field_524;
	char field_528[160];
	ChaoUnknownE *unknown_e_1;
	ChaoUnknownE *unknown_e_2;
	char field_5D0[216];
	short field_6A8;
	char field_6AA[310];
	char field_7E0;
	char field_7E1[19];
	ChaoUnknown UnknownArray[5];
	char UnknownArrayEnd;
	char field_19ED[922];
	char field_1D87;
};

struct ChaoData2
{
	char field_0[44];
	int field_2C;
	char gap_30[4];
	float field_34;
	char gap_38[4];
	float field_3C;
	char field_40;
	char gap_41[1];
	short field_42;
	char gap_44[4];
	int field_48;
	char gap_4C[20];
	NJS_VECTOR some_vector;
	char gap_6C[64];
	float field_AC;
	char field_B0[4];
	float field_B4;
	float field_B8;
	float field_BC;
	float field_C0;
	float field_C4;
	float field_C8;
	float field_CC;
	float field_D0;
	char gap_d4[407];
	char field_26B;
};

struct ChaoDebugFunction
{
	int Enabled;
	void(__cdecl *Function)(ObjectMaster *_this);
	const char *Name;
};

struct BlackMarketItem
{
	ChaoItemCategory Category;
	Sint8 Type;
};


struct ChaoTreeSpawn
{
	NJS_VECTOR a;
	NJS_VECTOR b;
	NJS_VECTOR c;
	NJS_VECTOR d;
	NJS_VECTOR e;
	NJS_VECTOR f;
	NJS_VECTOR g;
	NJS_VECTOR h;
	NJS_VECTOR i;
	NJS_VECTOR j;
};

// Model Queue

union PointType
{
	NJS_POINT2COL POINT2COL;
	NJS_POINT3COL POINT3COL;
};

struct QueuedModelNode
{
	QueuedModelNode *Next;
	float Depth;
	Uint8 Flags;
	char BlendMode;
	short TexNum;
	NJS_TEXLIST *TexList;
	NJS_ARGB Color;
	int Control3D;
	int ConstantAndAttr;
	int ConstantOrAttr;
};

struct QueuedModelMotionThing
{
	QueuedModelNode Node;
	int Unknown;
	PointType Points;
	char field_40[48];
	float FrameNumber;
	NJS_OBJECT *Object;
	NJS_MOTION *Motion;
	NJS_MOTION *MotionMaybe;
	Uint32 Attributes;
	float Scale;
	void(__cdecl *DrawCallback)(NJS_MODEL_SADX *);
};

struct QueuedModelTextureMemList
{
	QueuedModelNode Node;
	NJS_MODEL_SADX *Model;
	NJS_MATRIX Transform;
	NJS_TEXTURE_VTX *TexturedVertices;
	int Count;
	int TextureId;
	int Flags;
};

struct QueuedModelAction
{
	QueuedModelNode Node;
	int field_2C;
	NJS_MATRIX Transform;
	float FrameNumber;
	NJS_ACTION Action;
};

struct QueuedModelObject
{
	QueuedModelNode Node;
	NJS_OBJECT *Object;
	NJS_MATRIX Transform;
};

struct QueuedModelActionPtr
{
	QueuedModelNode Node;
	int field_2C;
	NJS_MATRIX Transform;
	NJS_ACTION *Action;
	float FrameNumber;
};

struct QueuedModelLineA
{
	QueuedModelNode Node;
	int Unknown;
	PointType Points;
	NJS_MATRIX Transform;
	Uint32 Attributes;
	Uint32 TextureNum;
};

struct QueuedModelLineB
{
	QueuedModelLineA Base;
	float Pri;
};

struct QueuedModelParticle
{
	QueuedModelNode Node;
	char idk[36];
};

struct QueuedModelCallback
{
	QueuedModelNode Node;
	int Unknown;
	NJS_MATRIX Transform;
	void(__cdecl *UserFunction)(void *);
	void *UserData;
};

struct QueuedModelRect
{
	QueuedModelNode Node;
	int Unknown;
	float Left;
	float Top;
	float Right;
	float Bottom;
	float Depth;
	NJS_COLOR Color;
};

struct QueuedModelSprite
{
	QueuedModelNode Node;
	int Unknown;
	NJS_SPRITE Sprite;
	Uint32 SpriteFlags;
	float SpritePri;
	NJS_MATRIX Transform;
};

struct QueuedModelPointer
{
	QueuedModelNode Node;
	NJS_MODEL_SADX *Model;
	NJS_MATRIX Transform;
};

struct QueuedModel
{
	NJS_MODEL_SADX model;
	NJS_MESHSET_SADX meshset;
};

struct DrawQueueHead
{
	DrawQueueHead *next;
	DrawQueueHead *prev;
};

// Vertex Buffers

struct FVFStruct_A
{
	NJS_VECTOR position;
	Uint32 diffuse;
	float u;
	float v;
};

struct FVFStruct_B
{
	NJS_VECTOR position;
};

struct FVFStruct_C
{
	NJS_VECTOR position;
	NJS_VECTOR normal;
};

struct FVFStruct_D
{
	NJS_VECTOR position;
	float u;
	float v;
};

struct FVFStruct_E
{
	NJS_VECTOR position;
	NJS_VECTOR normal;
	float u;
	float v;
};

struct FVFStruct_F
{
	NJS_VECTOR position;
	Uint32 diffuse;
};

struct FVFStruct_G
{
	NJS_VECTOR position;
	NJS_VECTOR normal;
	Uint32 diffuse;
};

struct FVFStruct_H
{
	NJS_VECTOR position;
	Uint32 diffuse;
	float u;
	float v;
};

struct FVFStruct_I
{
	NJS_VECTOR position;
	NJS_VECTOR normal;
	Uint32 diffuse;
	float u;
	float v;
};

struct FVFStruct_J
{
	float x;
	float y;
	float z;
	float rhw;
	Uint32 diffuse;
};

struct FVFStruct_H_B
{
	NJS_VECTOR position;
	float u;
	float v;
	int diffuse;
};

struct FVFStruct_K
{
	FVFStruct_J base;
	float u;
	float v;
};

struct OceanGarbage
{
	FVFStruct_H_B points[4];
};

struct struct_v3
{
	int SomeFlag;
	int dword4;
	int ShadowRotationX;
	int ShadowRotationY;
	float Distance;
	NJS_VECTOR njs_vector14;
};

struct Mysterious64Bytes
{
	NJS_VECTOR Position;
	struct_v3 struct_v3_a;
	struct_v3 struct_v3_b;
};

struct ChunkModelThing
{
	NJS_CNK_OBJECT *model;
	NJS_TEXLIST *texlist;
	float scale;
	int what;
};

struct ChunkObjectPointer
{
	NJS_CNK_OBJECT base[2];
	char field_68[48];
	ChunkModelThing thing;
	int useTransform;
	NJS_VECTOR position;
	Rotation3 rotation;
};

struct NBChunk
{
	short Type;
	short field_2;
	int Size;
};

struct TitleCardTexture
{
	int Level;
	const char *TextureName;
};

struct TitleCardTextureList
{
	int Count;
	TitleCardTexture *List;
};

struct CharBossData
{
	int BossID;
	ObjectMaster *Player1;
	ObjectMaster *BossCharacter;
	int anonymous_3;
	void(__cdecl *DeleteFunc)();
};

struct BlackMarketItemAttributes
{
	int PurchasePrice;
	int SalePrice;
	__int16 RequiredEmblems;
	__int16 Name;
	__int16 Description;
	__int16 anonymous_7;
};

struct BlackMarketItemAttributesList
{
	BlackMarketItemAttributes *Items;
	int Count;
};

#pragma pack(pop)

#ifdef _MSC_VER
// Compatibility macros for old struct names on MSVC.
typedef NJS_TEXLIST TexList;
typedef AnimData_t AnimData;
typedef PhysicsData_t PhysicsData;
typedef AdventureData_t AdventureData;
#endif /* _MSC_VER */

#endif /* SADXMODLOADER_SADXSTRUCTS_H */
