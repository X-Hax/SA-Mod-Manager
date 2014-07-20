/**
 * SADX Mod Loader.
 * SADX structs.
 */

#ifndef SADXMODLOADER_SADXSTRUCTS_H
#define SADXMODLOADER_SADXSTRUCTS_H

#include <stdint.h>
#include "../ninja.h"

typedef NJS_VECTOR Vertex, Vector3;
typedef NJS_TEXLIST TexList, TexListHead;
typedef NJS_OBJECT OBJECT;
typedef NJS_MODEL ATTACH;

// All structs should be packed.
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
	int field_0;
	int field_4;
	int HeldButtons;
	int NotHeldButtons;
	int PressedButtons;
	int ReleasedButtons;
	int16_t CameraRight;
	int16_t CameraLeft;
	int16_t ControlX;
	int16_t ControlY;
	int16_t CameraX;
	int16_t CameraY;
	int field_24;
	int field_28;
	int HeldButtons_Copy;
	int field_30;
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
	TexList *TexList;
};

struct LandTable
{
	int16_t COLCount;
	int16_t AnimCount;
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

struct AnimData
{
	NJS_ACTION *Animation;
	char field_4;
	char Property;
	int16_t NextAnim;
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
	int16_t Upgrades;
	int16_t Powerups;
	int field_8;
	int16_t UnderwaterTime;
	int16_t IdleTime;
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
	int16_t Animation;
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
	int16_t SwimStat;
	int16_t FlyStat;
	int16_t RunStat;
	int16_t PowerStat;
	int16_t StaminaStat;
	char gap_42[62];
	ChaoTypes Type;
	ChaoGardens Garden;
	char Reincarnate;
	char gap_83[3];
	char ClockRollovers;
	char gap_87[3];
	int16_t LifeTimeLeft;
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
	int16_t field_2;
	int16_t Status;
	int16_t InvulnerableTime;
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

struct AdventureData
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
	AdventureData AdventureData[8];
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

struct SETObjData
{
	char LoadCount;
	char f1;
	int16_t Flags;
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

#endif /* SADXMODLOADER_SADXSTRUCTS_H */
