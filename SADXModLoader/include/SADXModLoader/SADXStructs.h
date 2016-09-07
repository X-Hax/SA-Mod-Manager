/**
 * SADX Mod Loader.
 * SADX structs.
 */

#ifndef SADXMODLOADER_SADXSTRUCTS_H
#define SADXMODLOADER_SADXSTRUCTS_H

#include <stdint.h>
#include "../ninja.h"
#include "d3d8types.h"

typedef NJS_VECTOR Vector3;

// All structs should be packed.
#pragma pack(push, 1)

typedef struct Rotation {
	int x, y, z;
} Rotation3;

struct ObjectMaster;
typedef void(__cdecl *ObjectFuncPtr)(ObjectMaster *);

// TODO: Grab actual structs from disassembly.
typedef void ABC_TXT_struct;
typedef void AnimFrame_PosRot;
typedef void AnimFrame_PosRotScale;
typedef void AnimHead2;
typedef void AnimHead;
typedef void ChaoHudThing;
typedef void PDS_VIBPARAM;
typedef void pvm_thing;
typedef void struct_1;
typedef void what;

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
	char *Name;
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
	char Arg1;
	char Arg2;
	int16_t UseDistance;
	float Distance;
	int field_8;
	ObjectFuncPtr LoadSub;
	char *Name;
};

struct StartPosition
{
	int16_t LevelID;
	int16_t ActID;
	Vector3 Position;
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

struct SET_Entry
{
	int16_t ObjectType;
	int16_t XRotation;
	int16_t YRotation;
	int16_t ZRotation;
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
	NJS_TEXLIST *TexList;
};

struct LandTable
{
	int16_t COLCount;
	int16_t AnimCount;
	int Flags;
	float Unknown_1;
	COL *COLList;
	GeoAnimData *AnimData;
	char *TexName;
	NJS_TEXLIST *TexList;
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
	int16_t Ang_X;
	int16_t Ang_Y;
	float Dist;
	Vector3 Position;
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
	char field_4;
	char Property;
	int16_t NextAnim;
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

struct AnimThing
{
	short field_0;
	short field_2;
	short Animation;
	short field_6;
	short field_8;
	short field_A;
	short field_C;
	short field_E;
	float AnimationFrame;
	int dword14;
	float *pfloat18;
	float *pfloat1C;
	AnimData_t *AnimData;
	NJS_OBJECT **object_ptr_ptr;
	void *unknown_ptr;
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
	char gap_10[2];
	int field_12;
	char gap_16[2];
	float LoopDist;
	char gap_1C[28];
	NJS_VECTOR Speed;
	char gap_44[12];
	NJS_VECTOR field_50;
	char gap_5C[8];
	void *array_1x132;
	ObjectMaster *ObjectHeld;
	char gap_6C[12];
	void *array_15x32;
	short field_7C;
	short field_7E;
	short LightdashTime;
	short LightdashTimer;
	char gap_84[20];
	float TailsFlightTime;
	PhysicsData_t PhysicsData;
	AnimThing AnimThing;
	char gap_14C[144];
	int field_1DC;
	char gap_1E0[4];
	int field_1E4;
	char gap_1E8[20];
	float field_1FC;
};

struct CollisionData
{
	__int16 field_0;
	__int16 field_2;
	int field_4;
	NJS_VECTOR v;
	float anonymous_1;
	float anonymous_2;
	float anonymous_3;
	int field_20;
	int field_24;
	int field_28;
	int field_2C;
};

struct CollisionInfo
{
	__int16 flags_a;
	__int16 field_2;
	__int16 flags_b;
	__int16 Count;
	float f8;
	CollisionData *CollisionArray;
	Uint8 field_10[140];
	ObjectMaster *Object;
	__int16 field_A0;
	__int16 field_A2;
	int field_A4;
};

struct EntityData1
{
	char Action;
	char NextAction;
	char Unknown;
	char Index;
	__int16 Status;
	__int16 InvulnerableTime;
	char CharIndex;
	char CharID;
	__int16 field_A;
	float field_C;
	Loop *LoopData;
	Rotation3 Rotation;
	NJS_VECTOR Position;
	NJS_VECTOR Scale;
	CollisionInfo *CollisionInfo;
	char field_3C;
	char field_3D;
	char field_3E;
	char field_3F;
};

struct EntityData2
{
	CharObj2 *CharacterData;
	NJS_VECTOR field_4;
	int field_10;
	float field_14;
	int field_18;
	int field_1C;
	int field_20;
	int field_24;
	int field_28;
	int field_2C;
	int field_30;
	float field_34;
	float field_38;
	float field_3C;
};

struct ObjectData2
{
	char gap_0[4];
	float field_4;
	char gap_8[4];
	float field_C;
	char gap_10[62];
	__int16 field_4E;
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
	AnimHead *AnimHead_ptr_A;
	AnimHead *AnimHead_ptr_B;
	NJS_MATRIX matrix;
};

struct SETObjData
{
	char LoadCount;
	char f1;
	int16_t Flags;
	int dword4;
	SET_Entry *SETEntry;
	float Distance;
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
	SETObjData *SETData;
	EntityData1 *Data1;
	void *Data2;
	void *UnknownA_ptr;
	void *UnknownB_ptr;
	int field_30;
};

struct VideoData
{
	char *Filename;
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
	Vector3 Position;
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
	char *Message;
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
	char *Filename;
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
	Vector3 Position;
	Rotation3 Rotation;
};

struct MESFileText
{
	int16_t *field_0;
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
	int16_t YRot;
	char TextNum;
	char CharacterNum;
};

struct FieldNPCDataXYZRot
{
	Vector3 Position;
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
	NJS_VECTOR Far, Normal, Near;
};

struct StageLightData
{
	char level;
	char act;
	char light_num;
	char use_yxz;
	NJS_VECTOR xyz;
	float dif;
	float mutliplier;
	float rgb[3];
	float amb_rgb[3];
};

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
	__int16 SwimStat;
	__int16 FlyStat;
	__int16 RunStat;
	__int16 PowerStat;
	__int16 StaminaStat;
	__int16 LuckStat;
	char IntelligenceStat;
	char UnknownStat;
	char field_46[58];
	ChaoType Type;
	char Garden;
	__int16 Happiness;
	__int16 field_84;
	__int16 ClockRollovers;
	__int16 field_88;
	__int16 Lifespan;
	__int16 Lifespan2;
	__int16 Reincarnations;
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
	__int16 Sleepiness;
	__int16 Tiredness;
	__int16 Hunger;
	__int16 MateDesire;
	__int16 Boredom;
	char field_13E[10];
	__int16 Energy;
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
	__int16 SA2BToys;
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
	__int16 field_0;
	__int16 field_2;
	__int16 field_4;
	__int16 field_6;
	float field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
};

struct ChaoUnknown
{
	__int16 field_0;
	__int16 field_2;
	int field_4;
	int field_8;
	int field_C;
	float field_10;
	__int16 field_14;
	__int16 field_16;
	ChaoUnknownB field_18[32];
};

struct ChaoUnknownE
{
	char pad[960];
};

struct ChaoUnknownD
{
	__int16 field_0;
	__int16 field_2;
	__int16 field_4;
	__int16 field_6;
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
	__int16 field_6A8;
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
	__int16 field_42;
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


#pragma pack(pop)

#ifdef _MSC_VER
// Compatibility macros for old struct names on MSVC.
typedef NJS_TEXLIST TexList;
typedef AnimData_t AnimData;
typedef PhysicsData_t PhysicsData;
typedef AdventureData_t AdventureData;
#endif /* _MSC_VER */

#endif /* SADXMODLOADER_SADXSTRUCTS_H */
