/**
 * SADX Mod Loader.
 * SADX functions.
 */

#ifndef SADXMODLOADER_SADXFUNCTIONS_H
#define SADXMODLOADER_SADXFUNCTIONS_H

#include <stdint.h>
#include "../ModLoader/MemAccess.h"
#include "../d3d8types.h"
#include "SADXStructs.h"
#include "SADXEnums.h"

typedef Uint32 NJD_SPRITE;

// SADX Functions
#define ObjectFunc(NAME, ADDRESS) FunctionPointer(void,NAME,(ObjectMaster *obj),ADDRESS)

FunctionPointer(void, PrintDebug, (char *Format, ...), 0x401000);
FunctionPointer(void, CopyToGlobalSprite_ScalePosition, (NJS_SPRITE *sprite), 0x401030);
FunctionPointer(void, CopyToGlobalSprite_NativeToCurrent, (NJS_SPRITE *sprite), 0x401070);
FunctionPointer(void, Set_D3DCMPFUNC_Mode, (int a1), 0x401410);
FunctionPointer(int, Direct3D_Default_ZFUNC_thing_maybe, (), 0x401420);
FunctionPointer(void *, ReturnsPartOfObjectMasterWhyDoYouDoThisToMe, (ObjectMaster *a1), 0x4015E0);
FunctionPointer(int, InitializeSomeStuff, (), 0x401830);
VoidFunc(CheckSettings, 0x4025B0);
FunctionPointer(double, VertexMaxAbs, (Vector3 *a1), 0x402E70);
FunctionPointer(void, njSetPerspective, (Angle bams), 0x402ED0);
FunctionPointer(Angle, GetHorizontalFOV_BAMS, (), 0x402F00);
FunctionPointer(void, SetSpriteColor, (NJS_ARGB *a1), 0x402F40);
FunctionPointer(Sint32, njSetTexture, (NJS_TEXLIST *texlist), 0x403070);
FunctionPointer(void, njSetTextureNum_, (Uint32 num), 0x403090);
FunctionPointer(int, SetTextureAndGetNum, (), 0x4030B0);
FunctionPointer(void, Direct3D_SetAlphaBlend_, (int a1, int a2), 0x4030D0);
FunctionPointer(void, Direct3D_SetNearFarPlanes, (float min_, float max_), 0x403180);
FunctionPointer(Sint32, njReleaseTexture, (NJS_TEXLIST *texlist), 0x403290);
FunctionPointer(bool, CheckModelDistance_v, (NJS_VECTOR *center, float radius), 0x403330);
FunctionPointer(void, DrawLineList, (LineInfo *a1, int a2, int a3), 0x403BC0);
FunctionPointer(void *, late_alloca, (int a1), 0x403D30);
FunctionPointer(void, SetSpriteColorAndZFuncThing, (char a1), 0x404240);
FunctionPointer(void, Draw2DSprite, (NJS_SPRITE *sp, Int n, Float pri, NJD_SPRITE attr, char zfunc_type), 0x404660);
FunctionPointer(void, njDrawSprite3D_2, (NJS_SPRITE *_sp, Int n, NJD_SPRITE attr, int zfunc_type), 0x404760);
FunctionPointer(void, SomethingAboutObjectsAndMatrices, (void (__cdecl *function)(ObjectMaster *), ObjectMaster *obj, float a3, char a4), 0x404840);
FunctionPointer(bool, CheckModelDistance_xyz, (float x, float y, float z, float a4), 0x406F70);
FunctionPointer(void, DisplayAfterImage, (AnimHead *animHead, float frameNumber), 0x407020);
FunctionPointer(void, njDrawSprite3D, (NJS_SPRITE *_sp, Int n, NJD_SPRITE attr), 0x407070);
FunctionPointer(void, njDrawSprite2D_4, (NJS_SPRITE *hud, Int n, Float pri, NJD_SPRITE attr), 0x4070A0);
FunctionPointer(void, AnotherDrawSprite3DWrapper, (NJS_SPRITE *a1, Int n, NJD_SPRITE attr), 0x4072F0);
FunctionPointer(void, DrawRect, (float left, float top, float right, float bottom, float a5, int a6, char a7), 0x4073B0);
FunctionPointer(void, ProcessModelNode, (NJS_OBJECT *obj, int a2, float a3), 0x4074A0);
FunctionPointer(void, DrawModelIGuess_N, (NJS_MODEL *a1, char a2), 0x4078F0);
FunctionPointer(void, DrawModelIGuess, (NJS_MODEL *a1), 0x407970);
FunctionPointer(void, DoTimeOfDayLighting, (int *a1, int *a2), 0x40A4A0);
FunctionPointer(StageLightData *, GetStageLight, (int level, int act, int light_num), 0x40A8F0);
FunctionPointer(signed int, NaiZoGola, (ObjectMaster *a1), 0x40AFB0);
FunctionPointer(void, RunObjectIndex, (int index), 0x40B0C0);
ObjectFunc(DisplayObject, 0x40B130);
FunctionPointer(int, RunAllObjects, (), 0x40B170);
FunctionPointer(int, InitHeap, (), 0x40B1A0);
FunctionPointer(void *, AllocateMemory, (int size), 0x40B220);
FunctionPointer(void *, AllocateArray, (int count, int size), 0x40B2D0);
FunctionPointer(void, FreeMemory, (void *data), 0x40B310);
FunctionPointer(void, RunObjectChildren, (ObjectMaster *a1), 0x40B420);
FunctionPointer(void, DisplayObjectIndex, (int index), 0x40B4F0);
FunctionPointer(int, DisplayAllObjects, (), 0x40B540);
ObjectFunc(DeleteObjectMaster, 0x40B570);
ObjectFunc(CheckThingButThenDeleteObject, 0x40B6C0);
VoidFunc(DeleteMostObjects, 0x40B6E0);
VoidFunc(DeleteAllObjects, 0x40B730);
ObjectFunc(DeleteChildObjects, 0x40B7E0);
FunctionPointer(ObjectMaster *, LoadObject, (LoadObj flags, int index, void (__cdecl *loadSub)(ObjectMaster *)), 0x40B860);
FunctionPointer(ObjectMaster *, LoadChildObject, (LoadObj a1, void (__cdecl *address)(ObjectMaster *), ObjectMaster *parent), 0x40B940);
FunctionPointer(void, InitDInputMouse, (HINSTANCE hinst, HWND a2), 0x40BB20);
FunctionPointer(int, GetDInputMouseState, (), 0x40BB80);
VoidFunc(ReleaseDInputMouse, 0x40BC40);
FunctionPointer(int, DoSomethingRelatedToText_, (char *a2), 0x40BD30);
FunctionPointer(void, PlayMusicFile, (LPCSTR filename, int loop), 0x40CD20);
FunctionPointer(int, PlayVoiceFile, (LPCSTR filename), 0x40CE30);
FunctionPointer(int, WMPRestartMusic, (), 0x40CF50);
FunctionPointer(int, WMPClose, (int), 0x40CFF0);
FunctionPointer(int, PauseSound, (), 0x40D060);
FunctionPointer(int, ResumeSound, (), 0x40D0A0);
FunctionPointer(int, WMPInit, (), 0x40D0D0);
FunctionPointer(int, WMPRelease, (), 0x40D1F0);
FunctionPointer(int, DoesTheThingWithFOVAndDrawsGlobalSprite, (), 0x40D4D0);
FunctionPointer(void, DoSomethingRelatedToText, (void *a1, char *a2), 0x40E570);
FunctionPointer(signed int, SetCustomMouseButton, (unsigned __int16 index, int mouseButton), 0x40E8B0);
FunctionPointer(int, MouseUpdate, (), 0x40E900);
FunctionPointer(int, RenderCursorTriangle, (), 0x40EAB0);
VoidFunc(KeyboardUpdate, 0x40EE10);
FunctionPointer(double, EnableControl, (), 0x40EF40);
FunctionPointer(int, DisableControl, (), 0x40EF50);
FunctionPointer(Bool, IsControlEnabled, (), 0x40EF60);
FunctionPointer(void, EnableController, (Uint8 index), 0x40EF70);
FunctionPointer(void, DisableController, (Uint8 index), 0x40EFA0);
FunctionPointer(Bool, IsControllerEnabled, (Uint8 index), 0x40EFD0);
FunctionPointer(void, UpdateMenuInput, (int index), 0x40F070);
VoidFunc(WriteAnalogs, 0x40F170);
VoidFunc(InputThing__Ctor, 0x40F350);
VoidFunc(InitRawControllers, 0x40F3A0);
FunctionPointer(int, WriteControllerData, (), 0x40F460);
VoidFunc(Control, 0x40FDC0);
FunctionPointer(int, Enable3DSound, (int), 0x40FF20);
FunctionPointer(int, ItDisablesFog, (), 0x4119B0);
FunctionPointer(int, DoMoreThingsWithFog, (), 0x411A20);
VoidFunc(ToggleStageFog, 0x411AF0);
VoidFunc(DisableFog, 0x411B40);
FunctionPointer(void, SetGlobalColorThing, (float a, float r, float g, float b), 0x4128A0);
FunctionPointer(int, ClampGlobalColorThing_, (), 0x4128E0);
VoidFunc(ClampGlobalColorThing_Thing, 0x4128F0);
FunctionPointer(void, SetTimeOfDay, (char a1), 0x412C00);
FunctionPointer(int, GetTimeOfDay, (), 0x412C10);
FunctionPointer(int, GetCharacterUnlockedAdventure, (Characters character), 0x412C30);
FunctionPointer(void, UnlockCharacterAdventure, (int character), 0x412CA0);
FunctionPointer(void, SetEventFlag, (EventFlags a1), 0x412D00);
FunctionPointer(void, ClearEventFlag, (EventFlags a1), 0x412D10);
FunctionPointer(bool, GetEventFlag, (EventFlags offset), 0x412D20);
FunctionPointer(void, SetLevelClear, (LevelIDs level), 0x412E20);
FunctionPointer(int, IsEggCarrierSunk, (), 0x412E70);
VoidFunc(LoadFieldNPCs, 0x412E90);
VoidFunc(InitFlagsAndThings, 0x413300);
FunctionPointer(int, EnablePause, (), 0x413980);
FunctionPointer(int, DisablePause, (), 0x413990);
FunctionPointer(int, SomethingAboutSandHill, (), 0x413BC0);
FunctionPointer(void, StartLevelCutscene, (__int16 a1), 0x413C90);
FunctionPointer(int, DoStageStuff, (), 0x413D40);
FunctionPointer(int, DoReallyWeirdInputStuff, (), 0x413DD0);
FunctionPointer(int, DrawHudAndDoWeirdInputStuff, (), 0x413F00);
FunctionPointer(int, DelayedSound_Main, (ObjectMaster *), 0x414120);
FunctionPointer(ObjectMaster *, Load_DelayedSound_SFX, (int index), 0x414170);
FunctionPointer(ObjectMaster *, Load_DelayedSound_BGM, (int index), 0x4141B0);
VoidFunc(LoadLevelObject, 0x4143C0);
VoidFunc(LoadSkyboxObject, 0x414420);
FunctionPointer(int, SetCharacter, (__int16 character), 0x4144D0);
FunctionPointer(Characters, GetCurrentCharacterID, (), 0x4144E0);
VoidFunc(SetStartPos_ReturnToField, 0x414500);
FunctionPointer(int, SetLevelAndAct, (char level, char act), 0x414570);
FunctionPointer(int, SetNextLevelAndAct, (char Level, char Act), 0x4145B0);
FunctionPointer(int, GoToNextLevel, (), 0x414610);
FunctionPointer(int, GetLevelAndAct, (), 0x414650);
FunctionPointer(int, GetPrevLevelAndAct, (), 0x414670);
FunctionPointer(int, GetNextLevelAndAct, (), 0x414690);
FunctionPointer(bool, IsLevelChaoGarden, (), 0x4146B0);
FunctionPointer(int, SetLevelEntrance, (char num), 0x4147F0);
FunctionPointer(char, GetLevelEntranceID, (), 0x414800);
FunctionPointer(void, PutPlayerAtStartPointIGuess, (EntityData1 *data1), 0x414810);
FunctionPointer(void, SetNextLevel, (unsigned __int16 level), 0x414B00);
FunctionPointer(bool, IsGamePaused, (), 0x414D70);
FunctionPointer(int, IsIngame, (), 0x414D90);
FunctionPointer(__int16, DisplayPauseMenu, (), 0x415420);
VoidFunc(LoadLevelResults, 0x415540);
VoidFunc(LoadCharacter, 0x4157C0);
VoidFunc(Load_PRESSSTART_PVM, 0x4159A0);
FunctionPointer(__int16, GameStateHandler_Mission, (), 0x415AF0);
FunctionPointer(__int16, GameStateHandler_Trial, (), 0x416610);
FunctionPointer(__int16, GameStateHandler_Adventure, (), 0x4173B0);
FunctionPointer(float, njUnitVector, (NJS_VECTOR *a1), 0x418AC0);
FunctionPointer(void, FreeCollision, (ObjectMaster *a1), 0x41CAA0);
FunctionPointer(void, InitCollision, (ObjectMaster *obj, CollisionData *collisionArray, int count, unsigned __int8 flags), 0x41CAF0);
VoidFunc(SetupSomeScreenStuff, 0x4208E0);
FunctionPointer(void, ABC_TXT_load, (ABC_TXT_struct *structure), 0x420A00);
FunctionPointer(void, ABC_TXT_release, (ABC_TXT_struct *a1), 0x420A30);
FunctionPointer(void, ABC_TXT_copy, (ABC_TXT_struct *a1), 0x420A50);
FunctionPointer(void, UnloadCharTextures, (int charID), 0x420E90);
FunctionPointer(int, SetCurrentTextureToCommon, (), 0x420F90);
FunctionPointer(int, UnloadLevelTextures, (__int16 levelact), 0x421040);
FunctionPointer(int, LoadPVM_C, (const char *filename, NJS_TEXLIST *texList), 0x4210A0);
FunctionPointer(void, LoadPVM, (const char *filename, NJS_TEXLIST *texlist), 0x421180);
FunctionPointer(signed int, LoadPvmMEM2, (char *pvmName, NJS_TEXLIST *texList), 0x421260);
FunctionPointer(void, LoadCharTextures, (int charID), 0x421280);
FunctionPointer(void, LoadRegObjTextures, (int a1), 0x4212E0);
VoidFunc(LoadChaoObjTxtures, 0x4213A0);
FunctionPointer(void, LoadLevelTextures, (__int16 level), 0x4215B0);
FunctionPointer(void, ShowNoMemoryCardText, (void *a1), 0x421C20);
FunctionPointer(int, ShowNoMemoryCardText_0, (int), 0x421C70);
FunctionPointer(int, ShowNoMemoryCardText_1, (int), 0x421CD0);
VoidFunc(WriteSaveFile, 0x421FD0);
VoidFunc(j_WriteSaveFile, 0x4221D0);
FunctionPointer(void, LoadFile, (char *name, LPVOID lpBuffer), 0x422200);
FunctionPointer(void, LoadFileWithMalloc, (char *Name, LPVOID *Data), 0x422310);
FunctionPointer(int, ReleaseSetFile, (), 0x422440);
FunctionPointer(int, GetCharIDString, (), 0x422490);
VoidFunc(ReleaseCamFile, 0x4224B0);
FunctionPointer(signed int, njLoadTexture_Wrapper, (NJS_TEXLIST *texlist), 0x4228E0);
VoidFunc(LoadLevelFiles, 0x422AD0);
FunctionPointer(void, LoadSoundList, (signed int soundlist), 0x4238E0);
FunctionPointer(int, PlaySound, (int ID, int a2, int a3, int a4), 0x423D70);
FunctionPointer(signed int, PlaySound2, (int ID, int a2, int a3, int a4), 0x423E20);
FunctionPointer(int, DoSoundQueueThing, (int num), 0x424210);
FunctionPointer(int, LoadCharVoices, (), 0x4245F0);
VoidFunc(LoadCharBossSounds, 0x424760);
FunctionPointer(int, LoadLevelSounds, (), 0x424C80);
FunctionPointer(int, j_WMPInit, (), 0x4253C0);
ObjectFunc(SoundManager_Main, 0x4253D0);
ObjectFunc(SoundManager_Delete, 0x425620);
FunctionPointer(int, SoundManager_Delete2, (), 0x425670);
FunctionPointer(void, PlayMusic, (MusicIDs song), 0x425690);
FunctionPointer(int, RestoreLastSong, (), 0x4256E0);
FunctionPointer(void, PlayVoice, (int a1), 0x425710);
ObjectFunc(JingleMain, 0x425760);
ObjectFunc(SoundManager_Load, 0x425830);
FunctionPointer(void, PlayJingle, (int a1), 0x425860);
FunctionPointer(int, InitializeSoundManager, (), 0x4258B0);
FunctionPointer(bool, GetDebugMode, (), 0x4258F0);
FunctionPointer(void, njDrawSprite2D_3, (NJS_SPRITE *_sp, Int n, Float pri, NJD_SPRITE attr), 0x425910);
VoidFunc(Set0Rings, 0x425AB0);
FunctionPointer(int, GetMarketRings, (), 0x425AC0);
FunctionPointer(int, ResetLives, (), 0x425AF0);
FunctionPointer(void, GiveLives, (__int16 lives), 0x425B60);
FunctionPointer(void, AddRings, (__int16 rings), 0x425BE0);
FunctionPointer(int, GetRings, (), 0x425CC0);
VoidFunc(HudDisplayRingTimeLife, 0x425CD0);
VoidFunc(HudDisplayRingTimeLife_Check, 0x425F90);
FunctionPointer(int, GetLives, (), 0x425FE0);
VoidFunc(ResetTime, 0x425FF0);
FunctionPointer(int, DisableTimeThing, (), 0x426040);
VoidFunc(IncrementFrameCounter, 0x426050);
VoidFunc(TickClock, 0x426070);
FunctionPointer(void, HudShowTimer, (float a1, float a2), 0x4261C0);
FunctionPointer(void, GetTime, (char *minutes, char *seconds), 0x4265B0);
FunctionPointer(void, SetTime, (char minutes, char seconds), 0x4265D0);
FunctionPointer(void, SetTime2, (char minute, char second, char frame), 0x4265F0);
FunctionPointer(void, GetTime2, (char *minutes, char *seconds, char *frames), 0x426610);
FunctionPointer(void, AddSeconds, (int seconds), 0x426640);
FunctionPointer(bool, IsTime0, (), 0x426690);
FunctionPointer(double, GetTimeBonus, (), 0x4266C0);
FunctionPointer(int, DrawShapeMaybe, (float, float, float, float), 0x426740);
FunctionPointer(int, CheckMissionRequirements, (), 0x426AA0);
FunctionPointer(int, DrawScoreAct, (), 0x427430);
VoidFunc(ResetTime2, 0x427F10);
FunctionPointer(int, HudDisplayScoreOrTimer, (), 0x427F50);
FastcallFunctionPointer(int, CheckMissionRequirementsSubgame, (int character, int mission), 0x4282D0);
FunctionPointer(void, StageResult_Main, (ObjectMaster *a1), 0x42A680);
ObjectFunc(ScoreDisplay_Main, 0x42BCC0);
ObjectFunc(j_ScoreDisplay_Main, 0x42BEE0);
FunctionPointer(int, DrawScreenFade, (), 0x42BEF0);
FunctionPointer(int, ScreenFadeThing, (), 0x42BF70);
FunctionPointer(int, ScreenFadeThing_B, (), 0x42C570);
FunctionPointer(int, Load_SEGALOGO_E, (), 0x42C730);
FunctionPointer(int, InitializeSegaLogo, (), 0x42C7B0);
FunctionPointer(int, DisplayLogoScreen, (unsigned __int8 textureStart), 0x42CB20);
FunctionPointer(int, RunSegaSonicTeamLogos, (), 0x42CC70);
FunctionPointer(int, GetCharacterSelection, (), 0x42CDE0);
FunctionPointer(ObjectMaster *, j_LoadCharBoss, (ObjectMaster *a1), 0x42CE40);
FunctionPointer(bool, CheckSaveFile, (SaveFileData *saveFile), 0x42D0B0);
ThiscallFunctionPointer(void, WriteAdventureDataToSaveFile, (void *data), 0x42D200);
FunctionPointer(int, InitSaveData, (), 0x42D3B0);
VoidFunc(SaveSave, 0x42D630);
VoidFunc(LoadSave, 0x42D770);
FunctionPointer(int, VideoMain, (int n), 0x42F020);
FunctionPointer(double, j_EnableControl, (), 0x42F610);
FunctionPointer(int, j_DisableControl, (), 0x42F620);
FunctionPointer(void, DisplayDialogueText, (char *text), 0x42FB20);
FunctionPointer(ObjectMaster *, j_GetCharacterObject, (unsigned __int8 a1), 0x42FC40);
FunctionPointer(void, LoadEventCharacter, (unsigned __int8 charID, void (__cdecl *mainsub)(ObjectMaster *), float xpos, float ypos, float zpos, int xrot, int yrot, int zrot), 0x42FD80);
FunctionPointer(void, EV_CanselThread, (void (__cdecl *func)(int)), 0x431290);
FunctionPointer(void, LoadEventObject, (ObjectMaster **a1, ObjectMaster *(*a2)(void), float a3, float a4, float a5, int a6, int a7, int a8), 0x4316C0);
ObjectFunc(MenuObj_Display, 0x432480);
FunctionPointer(int, RunCurrentMenuObj_ZfuncThing, (), 0x4326F0);
ObjectFunc(MenuObj_Main, 0x432720);
FunctionPointer(int, LoadMenuObject, (), 0x432C60);
VoidFunc(RunCurrentMenuObj_KillDisplaySub, 0x432DD0);
FunctionPointer(int, DoSomeWeirdColorThingWithMenusParticularlyTheFileSelect, (unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4), 0x432F90);
FunctionPointer(int, AlphaBlend_08_16, (), 0x433170);
FunctionPointer(Uint32, GetGlobalIndex, (NJS_TEXLIST *a1, int texIndex), 0x4332F0);
FunctionPointer(void, DisplayScreenTexture, (int that_cant_be_right, float a2, float a3, float a4), 0x4338D0);
FunctionPointer(int, DrawBG, (int, float, float, float, float, float), 0x433F80);
FunctionPointer(int, GetOtherLastHorizontalFOV_BAMS, (), 0x4345F0);
VoidFunc(Camera_BasicallyCtor, 0x434600);
FunctionPointer(void, Camera_Delete, (ObjectMaster *), 0x434670);
FunctionPointer(int, camerahax_adventurefields, (), 0x434870);
FunctionPointer(int, camerahax_b, (), 0x434880);
FunctionPointer(double, SetCameraControlEnabled, (int), 0x4348A0);
FunctionPointer(Bool, IsCameraControlEnabled, (), 0x4348E0);
FunctionPointer(void, SetCameraMode_, (int a1), 0x4348F0);
FunctionPointer(int, GetCameraMode_, (), 0x434930);
FunctionPointer(void, SetCameraMode, (char a1), 0x436560);
FunctionPointer(int, GetCameraMode, (), 0x436590);
FunctionPointer(int, Get_camera_thing_b, (), 0x4365A0);
FunctionPointer(int, Set_camera_thing_b, (int), 0x4365B0);
FunctionPointer(int, DoSomethingWithFOV, (int), 0x437240);
FunctionPointer(void, Camera_Display, (ObjectMaster *a1), 0x4373D0);
ObjectFunc(Camera_Main, 0x438090);
FunctionPointer(int, LoadCamera, (), 0x438260);
FunctionPointer(void, ResetGeoPointer, (int Level, int Act), 0x43A4A0);
FunctionPointer(int, ResetGravity, (), 0x43B490);
FunctionPointer(void, SetGravityMaybe, (int a1, int a2), 0x43B4C0);
VoidFunc(ClearPlayerArrays, 0x43B920);
FunctionPointer(Bool, DoJumpThing, (EntityData1 *a1, CharObj2 *a2), 0x43BF90);
FunctionPointer(bool, HaveTarget, (char charid), 0x43C110);
FunctionPointer(void, j_RestoreLastSong, (ObjectMaster *a1), 0x440E10);
FunctionPointer(void, ForcePlayerAction, (unsigned __int8 playerNum, char action), 0x441260);
FunctionPointer(void, EnemyBounceThing, (unsigned __int8 playerID, float speedX, float speedY, float speedZ), 0x441280);
FunctionPointer(void, EnemyBounceThing_Wrapper, (unsigned __int8 playerID, float a2, float a3, float a4), 0x441370);
FunctionPointer(void, dothedash, (unsigned __int8 playerID, NJS_VECTOR *s, Rotation3 *r, __int16 a4), 0x441490);
FunctionPointer(void, PositionPlayer, (Uint8 charIndex, float x, float y, float z), 0x441780);
FunctionPointer(int, IsPlayerInsideSphere, (NJS_VECTOR *a1, float a2), 0x441840);
FunctionPointer(int, GetCharacterID, (char index), 0x441970);
FunctionPointer(ObjectMaster *, GetCharacterObject, (unsigned __int8 character), 0x441AC0);
FunctionPointer(CharObj2 *, GetCharObj2, (char index), 0x441B00);
FunctionPointer(int, GetPlayerCount, (), 0x441B10);
FunctionPointer(void, MoveSubsToArraysForSomeReason, (unsigned __int8 id), 0x441C70);
FunctionPointer(void, GiveMagneticBarrier, (char character), 0x441E30);
FunctionPointer(void, GiveBarrier, (char character), 0x441EA0);
FunctionPointer(void, GiveInvincibility, (int character), 0x441F10);
FunctionPointer(void, InitCharacterVars, (int ID, ObjectMaster *character), 0x442750);
FunctionPointer(void, SomethingAboutWaterButNotReally, (EntityData1 *entity1, EntityData2 *entity2, CharObj2 *charobj2), 0x445D10);
FunctionPointer(void, SomethingAboutWater, (EntityData1 *entity1, EntityData2 *entity2, CharObj2 *charobj2), 0x449500);
FunctionPointer(void, PlayAnimation, (AnimThing *a1), 0x44A800);
StdcallFunctionPointer(int, DirectSound_Init, (int, int, int, int, int), 0x44DE80);
VoidFunc(ResetRestartData, 0x44ED50);
FunctionPointer(int, CheckRestartLevel, (), 0x44ED70);
FunctionPointer(void, RestartCheckpoint, (Vector3 *Position, Rotation3 *Rotation), 0x44EDB0);
FunctionPointer(void, SetRestartData, (Vector3 *Position, Rotation3 *Rotation), 0x44EE70);
ObjectFunc(CheckPoint_Display, 0x44EEF0);
FunctionPointer(int, DisplayCheckpointTime_B, (), 0x44F2D0);
ObjectFunc(CheckPoint_Main, 0x44F540);
ObjectFunc(Ring_Display, 0x44F6B0);
ObjectFunc(Ring_Collect_Maybe, 0x44FC80);
FunctionPointer(void, DroppedRing_Main, (ObjectMaster *a1), 0x44FD10);
FunctionPointer(void, SpawnDroppedRings, (float x, float y, float z, int count), 0x4502D0);
ObjectFunc(Ring_Main, 0x450370);
FunctionPointer(void, HurtCharacter, (char a1), 0x4506F0);
FunctionPointer(void, DrawStageMissionImage, (ObjectMaster *a1), 0x457120);
FunctionPointer(int, LoadStageMissionImage, (), 0x457450);
FunctionPointer(signed int, GetMissionType, (int character, signed int a2), 0x4575D0);
FunctionPointer(void, FreeStageMissionImage, (ObjectMaster *), 0x457B90);
FunctionPointer(void, PressStartSprite_Main, (ObjectMaster *a1), 0x457D10);
FunctionPointer(void, PressStartSprite_Draw, (ObjectMaster *), 0x457D30);
FunctionPointer(void, ReleasePressStartTexList, (ObjectMaster *), 0x457E90);
FunctionPointer(void, PauseMenu_ActuallyDrawTheThings, (int n, NJS_POINT2 *pos, NJS_POINT2 *scale), 0x457EB0);
FunctionPointer(int, PauseMenu_Draw, (int, int, int), 0x458020);
FunctionPointer(int, PauseMenu_PrepareToActuallyDrawThethings, (int a1, int, int, int), 0x458080);
FunctionPointer(char, DecPauseSelection, (char index), 0x458260);
FunctionPointer(char, SetPauseDisplayOptions, (unsigned __int8 *a1), 0x4582E0);
FunctionPointer(char, IncPauseSelection, (char index), 0x4583F0);
FunctionPointer(signed int, UpdatePauseSelectionMaybe, (unsigned __int16 selection), 0x458470);
FunctionPointer(int, LevelHasMap, (), 0x458720);
FunctionPointer(int, PauseMenu_CameraIGuess, (int), 0x458D00);
FunctionPointer(signed int, PauseMenu_SetControls, (int mode_maybe), 0x458F90);
FunctionPointer(void, Snowboard_Tails_Display, (ObjectMaster *), 0x45BC40);
FunctionPointer(void, Snowboard_Delete, (ObjectMaster *arg_0), 0x45BDD0);
FunctionPointer(int, DelayedVoice_Main, (ObjectMaster *), 0x45BE40);
ObjectFunc(Tails_Delete, 0x45BEA0);
FunctionPointer(void, Snowboard_Tails_Main, (ObjectMaster *), 0x45E4B0);
FunctionPointer(int, Load_DelayedSound_Voice, (int), 0x45E5A0);
FunctionPointer(void, Tails_Display, (ObjectMaster *a1), 0x460C40);
ObjectFunc(Snowboard_Tails_Load, 0x461510);
ObjectFunc(Tails_Main, 0x461700);
FunctionPointer(int, ActuallyDoCameraRotationMaybe, (), 0x4629A0);
FunctionPointer(int, CameraFunc_FirstPerson, (ObjectMaster *a1), 0x465F50);
VoidFunc(Camera_SetCameraFunc_Unknown, 0x466B60);
FunctionPointer(bool, CameraFunc_Unknown, (EntityData1 *a1, EntityData1 *a2, int a3), 0x4682B0);
ObjectFunc(Capsule_Display, 0x46ADB0);
ObjectFunc(Capsule_Delete, 0x46AE30);
ObjectFunc(Capsule_Main, 0x46B130);
ObjectFunc(Capsule_Load, 0x46B170);
FunctionPointer(void, AnimalPickup_Main, (ObjectMaster *a1), 0x46B330);
FunctionPointer(void, AnimalPickup_Load, (ObjectMaster *a1), 0x46B5F0);
FunctionPointer(int, AnimalPickup_Load_, (), 0x46B650);
VoidFunc(LoadSETObjs_P2NotTailsAndNotDebug, 0x46B7B0);
VoidFunc(LoadSETObjs_NoP2OrDebugOrP2Tails, 0x46BA40);
VoidFunc(LoadSETObjs, 0x46BCE0);
FunctionPointer(signed int, ClipObject, (ObjectMaster *a1, float dist), 0x46C010);
FunctionPointer(void, FishingHud_Delete, (ObjectMaster *), 0x46C8D0);
FunctionPointer(void, FishingHud_DrawHIT, (ObjectMaster *a1), 0x46C920);
FunctionPointer(int, FishingHud_DrawReel, (), 0x46C9F0);
FunctionPointer(int, FishingHud_DrawRod, (), 0x46CAB0);
FunctionPointer(int, FishingHud_DrawMeters, (float), 0x46CC70);
FunctionPointer(void, BigHud_DrawWeightAndLife, (ObjectMaster *), 0x46FB00);
FunctionPointer(void, BigHud_Main, (ObjectMaster *a1), 0x470090);
FunctionPointer(int, ResetBigStuff, (), 0x470120);
FunctionPointer(void, FishingHud_Display, (ObjectMaster *), 0x470580);
FunctionPointer(void, FishingHud_Main, (ObjectMaster *a1), 0x471580);
FunctionPointer(ObjectMaster *, FishingHud_Load, (ObjectMaster *a1), 0x471ED0);
ObjectFunc(Knuckles_Delete, 0x472180);
ObjectFunc(Knuckles_Display, 0x4721B0);
FunctionPointer(int, Knuckles_Upgrades, (), 0x4726A0);
FunctionPointer(int, Knuckles_Something_B, (int), 0x472F80);
FunctionPointer(int, Knuckles_Something_E, (), 0x473570);
FunctionPointer(void, EmeraldRadarHud_Main, (ObjectMaster *a1), 0x475A70);
FunctionPointer(void, EmeraldRadarHud_Load, (ObjectMaster *), 0x475BA0);
FunctionPointer(int, EmeraldRadarHud_Grab_Draw, (ObjectMaster *), 0x475D50);
FunctionPointer(void, EmeraldRadarHud_Display, (ObjectMaster *), 0x475E50);
FunctionPointer(int, EmeraldRadarHud_Load_Load, (ObjectMaster *), 0x476440);
FunctionPointer(int, DisplayCheckpointTime_A, (ObjectMaster *), 0x476740);
ObjectFunc(Knuckles_Main, 0x47A770);
FunctionPointer(bool, IsFastSonicAI, (), 0x47C200);
FunctionPointer(void, TailsVS_DrawBarThing, (ObjectMaster *), 0x47C260);
FunctionPointer(int, SetTailsRaceVictory, (), 0x47C450);
FunctionPointer(int, SetOpponentRaceVictory, (), 0x47C460);
FunctionPointer(int, GetRaceWinnerPlayer, (), 0x47C470);
FunctionPointer(void, MRace_EggMobile_TexlistManager, (ObjectMaster *a1), 0x47D2A0);
ObjectFunc(Sonic2PAI_Main, 0x47D640);
ObjectFunc(Eggman2PAI_Main, 0x47D750);
ObjectFunc(Sonic2PAI_Load, 0x47D820);
ObjectFunc(Eggman2PAI, 0x47D8C0);
FunctionPointer(int, LoadTailsOpponent, (__int16 character, __int16 loop, __int16 level), 0x47D940);
FunctionPointer(int, CheckTailsAI, (), 0x47E7C0);
ObjectFunc(TailsAI_Main, 0x47E8F0);
FunctionPointer(bool, Load2PTails, (ObjectMaster *player1), 0x47ED60);
ObjectFunc(Gamma_Delete, 0x47EED0);
FunctionPointer(void, Gamma_Display, (ObjectMaster *a1), 0x47FD50);
ObjectFunc(Gamma_Main, 0x483430);
ObjectFunc(Amy_Delete, 0x484360);
ObjectFunc(Amy_Display, 0x487260);
ObjectFunc(Amy_Main, 0x48ABF0);
FunctionPointer(void, Big_Delete, (ObjectMaster *a2), 0x48B730);
ObjectFunc(Big_Display, 0x48B8B0);
ObjectFunc(Big_Main, 0x490A00);
FunctionPointer(void, Sonic_InitLightDash, (EntityData1 *a1, int a2, CharObj2 *a3), 0x492AB0);
FunctionPointer(void, LoadSonicDashTrail, (EntityData1 *a1), 0x494050);
FunctionPointer(void, Sonic_Snowboard_Display, (ObjectMaster *), 0x494140);
ObjectFunc(Sonic_SuperPhysics_Delete, 0x4942D0);
FunctionPointer(void, Sonic_SuperPhysics_Main, (ObjectMaster *), 0x494320);
ObjectFunc(Sonic_SuperPhysics_Load, 0x494350);
ObjectFunc(MetalSonic_AfterImage_Display, 0x4946E0);
FastcallFunctionPointer(void, DoSonicGroundAnimation, (CharObj2 *a1, EntityData1 *a2), 0x494840);
ObjectFunc(Sonic_Delete, 0x494860);
ObjectFunc(Sonic_Display, 0x4948C0);
FunctionPointer(void, Sonic_Snowboard_Main, (ObjectMaster *), 0x4958C0);
FunctionPointer(int, MetalSonic_AfterImage_Main, (ObjectMaster *), 0x4959A0);
ObjectFunc(Snowboard_Sonic_Load, 0x4959E0);
ObjectFunc(MetalSonic_AfterImages_Main, 0x495A50);
FunctionPointer(void, Sonic_Act1, (EntityData1 *entity1, EntityData2 *entity2, CharObj2 *obj2), 0x496F50);
FunctionPointer(void, Sonic_Main, (ObjectMaster *character), 0x49A9B0);
FunctionPointer(int, DoStatusThing, (EntityData1 *data1, EntityData2 *data2, int n), 0x49CDA0);
FunctionPointer(void, O_HANAB_Delete, (ObjectMaster *a1), 0x49D050);
FunctionPointer(signed int, O_HANAB_Main, (ObjectMaster *a1), 0x49D070);
FunctionPointer(void, E102_TimeAddHud_Display, (ObjectMaster *a1), 0x49FDA0);
FunctionPointer(void, E102_TimeAddHud_Delete, (ObjectMaster *a1), 0x49FEE0);
FunctionPointer(int, E102_TimeAddHud_Main, (ObjectMaster *), 0x49FF10);
ObjectFunc(EmeraldPiece_Display, 0x4A2FB0);
ObjectFunc(EmeraldPiece_Main, 0x4A2FD0);
ObjectFunc(EmeraldPiece_Load, 0x4A3420);
FunctionPointer(int, TikalHint_Child_Child, (ObjectMaster *), 0x4A38D0);
FunctionPointer(int, TikalHint_Child, (ObjectMaster *a1), 0x4A39F0);
ObjectFunc(ERobo_0, 0x4A6700);
FunctionPointer(void, Leon_Display_B, (ObjectMaster *a1), 0x4A8390);
FunctionPointer(void, Leon_Display_A, (ObjectMaster *a1), 0x4A83B0);
ObjectFunc(Leon_Main, 0x4A83D0);
ObjectFunc(Leon_Load, 0x4A85C0);
ObjectFunc(EAclift, 0x4AA620);
FunctionPointer(void, Kiki_Display, (ObjectMaster *), 0x4AB2B0);
FunctionPointer(void, Kiki_Main, (ObjectMaster *), 0x4ACF80);
ObjectFunc(Kiki_Load, 0x4AD140);
FunctionPointer(void, Boss_Display_, (ObjectMaster *a1), 0x4AD3F0);
ObjectFunc(Boss_Main_, 0x4AD660);
FunctionPointer(void, Bos_Main__, (ObjectMaster *), 0x4ADB30);
FunctionPointer(void, Boss_Display__, (ObjectMaster *a1), 0x4ADF10);
FunctionPointer(int, Bos_Main___, (ObjectMaster *), 0x4AE5B0);
ObjectFunc(UnidusA_Main, 0x4AF190);
ObjectFunc(UnidusB_Main, 0x4AF500);
ObjectFunc(UnidusC_Main, 0x4AF860);
ObjectFunc(Spinner_Display, 0x4AFD80);
ObjectFunc(SpinnerA_Main, 0x4B0DF0);
ObjectFunc(SpinnerB_Main, 0x4B0F40);
ObjectFunc(SpinnerC_Main, 0x4B1090);
FunctionPointer(void, EPolice_Display, (ObjectMaster *a3), 0x4B2710);
FunctionPointer(int, EPolice_Main, (ObjectMaster *a1), 0x4B30E0);
ObjectFunc(EPolice, 0x4B3210);
FunctionPointer(int, DisplayBossName, (ObjectMaster *), 0x4B33D0);
FunctionPointer(void, LifeGague_Main, (ObjectMaster *), 0x4B3830);
FunctionPointer(int, LoadEggRoboTextures, (), 0x4B3DA0);
FunctionPointer(void, SetEmblemCollected, (SaveFileData *savefile, signed int index), 0x4B3F30);
FunctionPointer(int, CountEmblems, (SaveFileData *a1), 0x4B3F90);
FunctionPointer(int, GetEmblemBitIndex, (int character, signed int level, int mission), 0x4B3FC0);
FunctionPointer(void, Emblem_Display, (ObjectMaster *), 0x4B4100);
ObjectFunc(DeleteObject_, 0x4B4160);
FunctionPointer(void, EmblemScreen_Main, (ObjectMaster *a1), 0x4B4200);
FunctionPointer(void, EmblemScreen_Delete, (ObjectMaster *), 0x4B4530);
FunctionPointer(bool, GetLevelEmblemCollected, (SaveFileData *savefile, int character, signed int level, int mission), 0x4B45C0);
FunctionPointer(void, SetLevelEmblemCollected, (SaveFileData *savefile, int character, signed int level, int mission), 0x4B4640);
ObjectFunc(Emblem_Main, 0x4B4940);
FunctionPointer(bool, IsBoardingSubGame, (), 0x4B5000);
FunctionPointer(bool, IsTwinkleCircuit, (), 0x4B5040);
FunctionPointer(int, HudShowScore, (int), 0x4B50E0);
FunctionPointer(int, GetCurrentCharIDOrSomething, (), 0x4B5A30);
FunctionPointer(ObjectMaster *, LoadCharBoss, (ObjectMaster *a1), 0x4B6050);
FunctionPointer(signed int, GetHintText, (int id, int *data), 0x4B7C10);
FunctionPointer(NJS_OBJECT *, ProcessAnimatedModelNode, (NJS_OBJECT *a1, AnimFrame_PosRotScale *a2), 0x4B7D00);
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
FunctionPointer(void, DisplayCheckpointTime, (void *a1, signed int a2, signed int a3), 0x4BABE0);
FunctionPointer(int, Rumble_Main, (ObjectMaster *), 0x4BCB20);
FunctionPointer(void, Rumble_Load, (Uint32 time), 0x4BCB60);
FunctionPointer(void, RumbleA, (Uint32 port, Sint32 time), 0x4BCBC0);
FunctionPointer(void, RumbleB, (Uint32 port, Uint32 time, int a3, int a4), 0x4BCC10);
FunctionPointer(signed int, Weed_SpawnItem, (EntityData1 *entity, int n), 0x4BD000);
ObjectFunc(LevelItem_Display, 0x4BF5A0);
ObjectFunc(LevelItem_Main, 0x4BF600);
ObjectFunc(LevelItem_Delete, 0x4BFDA0);
ObjectFunc(ItemBoxAir_Display, 0x4BFDD0);
FunctionPointer(int, DoThingWithItemBoxPowerupIndex, (int n), 0x4C00B0);
ObjectFunc(ItemBoxAir_Main, 0x4C07D0);
FunctionPointer(void, EmeraldRadarHud_Draw, (NJS_VECTOR *pos, int n, float scale, float alpha), 0x4C0DC0);
ObjectFunc(ESman, 0x4C8FB0);
FunctionPointer(ObjectMaster *, RocketHThingB, (ObjectMaster *a1), 0x4CA290);
FunctionPointer(ObjectMaster *, RocketVThingB, (ObjectMaster *a1), 0x4CA3C0);
ObjectFunc(RocketH_Main, 0x4CA530);
ObjectFunc(RocketHS_Main, 0x4CA660);
ObjectFunc(RocketV_Main, 0x4CA7D0);
ObjectFunc(RocketVS_Main, 0x4CA900);
ObjectFunc(Switch_Main, 0x4CBA80);
ObjectFunc(ODynamite, 0x4CBDE0);
FunctionPointer(ObjectData2 *, AllocateObjectData2, (ObjectMaster *_this, EntityData1 *a2), 0x4CC990);
FunctionPointer(char, IsVisible, (NJS_VECTOR *v), 0x4CD730);
ObjectFunc(ZERO_Delete, 0x4D1780);
ObjectFunc(ZERO_Main, 0x4D2480);
ObjectFunc(ZERO_Load, 0x4D3E00);
ObjectFunc(CSphere, 0x4D4700);
ObjectFunc(ColCylinder_Main, 0x4D4770);
ObjectFunc(ColCube_Main, 0x4D47E0);
ObjectFunc(Wall_Main, 0x4D4850);
ObjectFunc(Ottotto_Main, 0x4D4B70);
ObjectFunc(BigWJump_Main, 0x4D4BE0);
ObjectFunc(NoWater, 0x4D4E10);
ObjectFunc(ItemBox_Display, 0x4D6810);
ObjectFunc(ItemBox_Display_Unknown, 0x4D6990);
ObjectFunc(ItemBox_Display_Destroyed, 0x4D6B20);
FunctionPointer(void, ItemBox_Delete, (ObjectMaster *a1), 0x4D6BA0);
FunctionPointer(void, SpeedShoesPowerup, (EntityData1 *a1), 0x4D6BF0);
FunctionPointer(int, FiveRingsPowerup, (), 0x4D6C50);
FunctionPointer(int, TenRingsPowerup, (), 0x4D6C90);
FunctionPointer(int, RandomRingPowerup, (), 0x4D6CD0);
FunctionPointer(int, ExtraLifePowerup, (), 0x4D6D40);
FunctionPointer(void, InvincibilityPowerup, (EntityData1 *a1), 0x4D6D80);
FunctionPointer(int, BarrierPowerup, (), 0x4D6DC0);
FunctionPointer(int, BombPowerup, (int), 0x4D6E00);
FunctionPointer(int, MagneticBarrierPowerup, (), 0x4D6E40);
ObjectFunc(ItemBox_Main, 0x4D6F10);
FunctionPointer(void, SpawnItemBox, (NJS_VECTOR *position, signed int kind), 0x4D7100);
ObjectFunc(AltItemBox_Main, 0x4D7170);
FunctionPointer(void, DisplayDebugModeString, (signed int offset, const char *fmt, ...), 0x4D9740);
FunctionPointer(void, DrawLensFlare, (NJS_VECTOR *a1), 0x4DA000);
FunctionPointer(void, DrawLensFlare_B, (NJS_VECTOR *a1, NJS_VECTOR *a2), 0x4DA3A0);
ObjectFunc(DelayedMusicPlayer_Main, 0x4DAA40);
FunctionPointer(void, TwinkleCircuit_DrawHUD, (ObjectMaster *a1), 0x4DB5E0);
ObjectFunc(GoalZone, 0x4DBC10);
ObjectFunc(PreviousZone, 0x4DBC30);
ObjectFunc(NextZone, 0x4DBC50);
ObjectFunc(Signboard1, 0x4DBD80);
ObjectFunc(Signboard2, 0x4DBDA0);
ObjectFunc(Signboard3, 0x4DBDC0);
ObjectFunc(Signboard4, 0x4DBDE0);
ObjectFunc(Signboard5, 0x4DBE00);
ObjectFunc(GoalRobot, 0x4DC250);
ObjectFunc(StartRobot, 0x4DC510);
ObjectFunc(Dial, 0x4DC530);
FunctionPointer(int, TwinkleCircuit_DrawColon, (), 0x4DC7A0);
FunctionPointer(void, TwinkleCircuit_DrawLaps, (char n), 0x4DCFB0);
ObjectFunc(Obj_WValley, 0x4DDB30);
ObjectFunc(SkyBox_WindyValley_Load, 0x4DDBF0);
ObjectFunc(O_setsmo, 0x4DE2F0);
ObjectFunc(OTorout, 0x4DE8E0);
FunctionPointer(int, ChaosEmeGoal_WValley_CheckTouch, (), 0x4DF2F0);
ObjectFunc(ChaosEmeGoal_WValley_Main, 0x4DF3B0);
ObjectFunc(OSetiff, 0x4DF7D0);
ObjectFunc(OHanaa, 0x4DFEF0);
ObjectFunc(OHanab, 0x4DFF10);
ObjectFunc(O_tanpopo, 0x4DFF30);
ObjectFunc(OPopo, 0x4E0310);
ObjectFunc(OWasi, 0x4E0690);
ObjectFunc(OTuri1, 0x4E0760);
ObjectFunc(OStbird, 0x4E0A40);
ObjectFunc(OTuribr, 0x4E0F40);
ObjectFunc(OTuribr2, 0x4E0F80);
ObjectFunc(OE102br, 0x4E0FC0);
ObjectFunc(OHasie, 0x4E13E0);
ObjectFunc(OSakuc, 0x4E1460);
ObjectFunc(OHanea, 0x4E14A0);
ObjectFunc(OHaneb, 0x4E14E0);
ObjectFunc(OBigflo, 0x4E1520);
ObjectFunc(OBall, 0x4E16C0);
ObjectFunc(OKazami, 0x4E19D0);
ObjectFunc(O_cubes, 0x4E1B50);
ObjectFunc(O_cubem, 0x4E1B90);
ObjectFunc(O_cubel, 0x4E1BD0);
ObjectFunc(O_ukisim, 0x4E1C10);
ObjectFunc(OBigfla, 0x4E1C50);
ObjectFunc(OTatel, 0x4E1C70);
ObjectFunc(OTates, 0x4E1CB0);
ObjectFunc(OGrass, 0x4E1D80);
ObjectFunc(OGate, 0x4E1FA0);
ObjectFunc(OIshia, 0x4E2040);
ObjectFunc(OIshib, 0x4E2080);
ObjectFunc(Enemy_Delete, 0x4E21D0);
ObjectFunc(OBrocu, 0x4E2880);
ObjectFunc(OBroobj, 0x4E2AB0);
ObjectFunc(OTreem, 0x4E2BC0);
ObjectFunc(OSakua, 0x4E2C40);
ObjectFunc(Osakub, 0x4E2C80);
ObjectFunc(OPoline, 0x4E3200);
ObjectFunc(OYure, 0x4E33D0);
ObjectFunc(WcWind, 0x4E3C50);
ObjectFunc(PuWind, 0x4E4C10);
ObjectFunc(BLeaf, 0x4E5510);
ObjectFunc(O_vrock, 0x4E5920);
ObjectFunc(OTorbri, 0x4E6770);
FunctionPointer(void, E103Enemy_Delete, (ObjectMaster *a1), 0x4E7A30);
ObjectFunc(E103Enemy_Display, 0x4E7AC0);
ObjectFunc(E103Enemy_Main, 0x4E7E90);
ObjectFunc(E103Enemy_Load, 0x4E7F50);
ObjectFunc(Snowboard_Load, 0x4E9660);
FunctionPointer(void, Cold_Main, (ObjectMaster *), 0x4EA280);
ObjectFunc(Cold_Load, 0x4EA3B0);
ObjectFunc(ACT12JOIN_Main, 0x4EBDF0);
ObjectFunc(ACT23JOIN_Main, 0x4EBEB0);
ObjectFunc(OKuguri, 0x4EC090);
FunctionPointer(void, IceCapScoreMultiplier_Draw, (ObjectMaster *), 0x4EC120);
ObjectFunc(Yurayura, 0x4ECE30);
FunctionPointer(int, ChaosEmeGoal_IceCap_CheckTouch, (), 0x4ECF30);
ObjectFunc(ChaosEmeGoal_IceCap_Main, 0x4ECFA0);
ObjectFunc(OHasihasi, 0x4ED250);
ObjectFunc(OSnownewa, 0x4ED4A0);
ObjectFunc(OSnowmounta, 0x4ED4B0);
ObjectFunc(OSnowmountb, 0x4ED4C0);
ObjectFunc(OSnowmountc, 0x4ED4D0);
ObjectFunc(OSnowmountd, 0x4ED4E0);
ObjectFunc(OSnowrocka, 0x4ED4F0);
ObjectFunc(OSnowrockb, 0x4ED500);
ObjectFunc(OSnowrockc, 0x4ED510);
ObjectFunc(OSnowrockd, 0x4ED520);
ObjectFunc(OSnowstonea, 0x4ED530);
ObjectFunc(OSnowstoneb, 0x4ED540);
ObjectFunc(OSnowstonec, 0x4ED550);
ObjectFunc(OSnowstoned, 0x4ED560);
ObjectFunc(OSnowstonee, 0x4ED570);
ObjectFunc(OSnowstonef, 0x4ED580);
ObjectFunc(OSnowboard, 0x4EDA90);
ObjectFunc(OBighone, 0x4EDBB0);
ObjectFunc(OBiciwa, 0x4EE800);
ObjectFunc(OFutam, 0x4EF320);
ObjectFunc(OFutam2, 0x4EF340);
ObjectFunc(OFutal, 0x4EF360);
ObjectFunc(OBalloon1, 0x4EF4E0);
ObjectFunc(OBalloon2, 0x4EF510);
ObjectFunc(OBalloon3, 0x4EF540);
ObjectFunc(OBalloon4, 0x4EF570);
ObjectFunc(OSuitaore, 0x4EF880);
ObjectFunc(OSuisyoub, 0x4EF8C0);
ObjectFunc(OSuisyoubm, 0x4EF8F0);
ObjectFunc(OSuisyour, 0x4EF920);
ObjectFunc(OBigturara, 0x4F0EC0);
ObjectFunc(OMizuiwaa, 0x4F11F0);
ObjectFunc(OMizuiwab, 0x4F1210);
ObjectFunc(OMizuiwac, 0x4F1230);
ObjectFunc(OFuusya1, 0x4F13F0);
ObjectFunc(OIta02, 0x4F14E0);
ObjectFunc(OIta01, 0x4F15C0);
ObjectFunc(OMizukusaa, 0x4F1750);
ObjectFunc(OMizukusab, 0x4F1770);
ObjectFunc(OIcebar2, 0x4F26E0);
ObjectFunc(OIcebar1, 0x4F3850);
ObjectFunc(Turibasi1, 0x4F3A00);
ObjectFunc(OSnowDoa, 0x4F3FF0);
ObjectFunc(OMokuA, 0x4F40F0);
ObjectFunc(OMokuB, 0x4F4120);
ObjectFunc(OMokuC, 0x4F4150);
ObjectFunc(ODryA, 0x4F4180);
ObjectFunc(ODryB, 0x4F41B0);
ObjectFunc(Maruta, 0x4F41E0);
ObjectFunc(Bigita, 0x4F4210);
ObjectFunc(Kareki01, 0x4F4240);
ObjectFunc(Kareki02, 0x4F4270);
ObjectFunc(Kareki03, 0x4F42A0);
ObjectFunc(Ki01, 0x4F42D0);
ObjectFunc(O_housea, 0x4F4370);
ObjectFunc(O_houseb, 0x4F4390);
ObjectFunc(O_housec, 0x4F43B0);
ObjectFunc(O_housed, 0x4F43D0);
ObjectFunc(O_hashra1b, 0x4F4900);
ObjectFunc(O_hashra_c, 0x4F4950);
ObjectFunc(O_hashra10c, 0x4F4990);
ObjectFunc(O_saku01, 0x4F49F0);
ObjectFunc(O_saku01b, 0x4F4A20);
ObjectFunc(O_saku02, 0x4F4A50);
ObjectFunc(O_saku02b, 0x4F4A80);
ObjectFunc(O_ele, 0x4F4AB0);
ObjectFunc(O_fiw4m, 0x4F4AE0);
ObjectFunc(O_fiw5m, 0x4F4B10);
ObjectFunc(O_fiw6m, 0x4F4B40);
ObjectFunc(O_hako, 0x4F4B70);
ObjectFunc(OIcicle, 0x4F4D80);
ObjectFunc(OIcejmp, 0x4F4DE0);
ObjectFunc(OHyo1noexp, 0x4F57F0);
ObjectFunc(OHyo1exp, 0x4F5830);
ObjectFunc(OHyo2noexp, 0x4F5870);
ObjectFunc(OHyo2exp, 0x4F58B0);
ObjectFunc(OHyo3noexp, 0x4F58F0);
ObjectFunc(OHyo3exp, 0x4F5930);
ObjectFunc(ODd, 0x4F5CD0);
FunctionPointer(void, SetClip_ECoast1, (signed int clipLevel), 0x4F68E0);
FunctionPointer(void, EmeraldCoast_SkyBox_Delete, (ObjectMaster *a1), 0x4F6C60);
ObjectFunc(Obj_ECoast, 0x4F6D60);
ObjectFunc(EmeraldCoast_SkyBox_Display, 0x4F6DB0);
ObjectFunc(EmeraldCoast_SkyBox_Main, 0x4F71A0);
ObjectFunc(SkyBox_EmeraldCoast_Load, 0x4F7230);
FunctionPointer(void, ECoast_AniTexMan_Display, (ObjectMaster *a1), 0x4F7250);
FunctionPointer(void, ECoast1_AniTexMan1_Main, (ObjectMaster *a1), 0x4F7300);
ObjectFunc(OAsiato, 0x4F7E90);
FunctionPointer(int, ObjectSelectedDebug, (ObjectMaster *obj), 0x4F88A0);
ObjectFunc(OBz, 0x4F9FB0);
ObjectFunc(OFrog, 0x4FA320);
ObjectFunc(OBewind, 0x4FA3B0);
ObjectFunc(ODolphin, 0x4FACD0);
ObjectFunc(ODolsw, 0x4FAE30);
ObjectFunc(OKasa, 0x4FB050);
ObjectFunc(OBed, 0x4FB160);
ObjectFunc(j_DeleteChildObjects, 0x4FB290);
ObjectFunc(OArchrock, 0x4FB5F0);
ObjectFunc(OBfloot, 0x4FB710);
ObjectFunc(OBunga, 0x4FB730);
ObjectFunc(ORock, 0x4FB750);
ObjectFunc(OIwapo, 0x4FB770);
ObjectFunc(OSekityu, 0x4FB790);
ObjectFunc(OKomomo, 0x4FB7E0);
ObjectFunc(ORock2, 0x4FB800);
ObjectFunc(OTaki, 0x4FBA70);
ObjectFunc(OPoSummon, 0x4FBBE0);
ObjectFunc(OAoKill, 0x4FBCE0);
ObjectFunc(OAoSummon, 0x4FBE40);
ObjectFunc(OBigrock, 0x4FC550);
ObjectFunc(OSakana8k, 0x4FD160);
ObjectFunc(OMkusa, 0x4FD770);
ObjectFunc(OBkusa, 0x4FD9D0);
ObjectFunc(OK2, 0x4FDA90);
FunctionPointer(signed int, OGomban_Delete, (ObjectMaster *a1), 0x4FDCD0);
FunctionPointer(void, OGomban_Display, (ObjectMaster *a1), 0x4FDD20);
FunctionPointer(void, OGomban_Main, (ObjectMaster *a1), 0x4FDFC0);
ObjectFunc(OGomban_Load, 0x4FE8C0);
FunctionPointer(int, OPier_Display, (ObjectMaster *a2), 0x4FE8E0);
FunctionPointer(__int16, OPier_Delete, (ObjectMaster *a1), 0x4FE970);
FunctionPointer(void, OPier_Main, (ObjectMaster *a1), 0x4FED30);
ObjectFunc(OPier_Load, 0x4FF060);
FunctionPointer(int, OParasol_Display, (ObjectMaster *a2), 0x4FF140);
ObjectFunc(OParasol_Main, 0x4FF4C0);
ObjectFunc(OParasol_Load, 0x4FF5F0);
ObjectFunc(OKamomel, 0x4FFB60);
ObjectFunc(OKamome, 0x4FFC30);
ObjectFunc(OGrasa, 0x4FFD00);
ObjectFunc(OGrasb, 0x4FFD20);
ObjectFunc(OGrasc, 0x4FFD40);
ObjectFunc(OGrasd, 0x4FFD60);
ObjectFunc(OCora, 0x4FFD80);
ObjectFunc(OCorb, 0x4FFDA0);
ObjectFunc(OFena, 0x4FFDC0);
ObjectFunc(OFenb, 0x4FFE00);
ObjectFunc(OFenc, 0x4FFE40);
ObjectFunc(OFend, 0x4FFE80);
ObjectFunc(OJump, 0x5001E0);
ObjectFunc(OIwa, 0x500770);
ObjectFunc(OBgiro, 0x500B90);
FunctionPointer(void, YASI0_Display, (ObjectMaster *), 0x500E60);
ObjectFunc(Yasi3, 0x500EC0);
ObjectFunc(Yasi0, 0x500F50);
ObjectFunc(Yasi1, 0x500FF0);
ObjectFunc(Yasi2, 0x501090);
ObjectFunc(Obj_EC1Water_Display, 0x501130);
ObjectFunc(Obj_EC1Water_Main, 0x5017D0);
ObjectFunc(Obj_EC1Water_Load, 0x501970);
FunctionPointer(void, DoSomethingWithSaveInfo, (LPCSTR a1, _WIN32_FIND_DATAA a2), 0x504E50);
FunctionPointer(unsigned __int8, FindSaveFiles, (), 0x505050);
FunctionPointer(void, FileSelect_Main, (ObjectMaster *), 0x5053F0);
FunctionPointer(void, FileSelect_Display, (ObjectMaster *), 0x505550);
FunctionPointer(bool, IsAdventureComplete, (int character), 0x5063E0);
FunctionPointer(int, IsMetalSonicUnlocked, (), 0x506490);
VoidFunc(j_LoadSave, 0x506620);
FunctionPointer(int, GetCharacterUnlockedFlag, (int), 0x506630);
FunctionPointer(int, MenuSelectButtonsPressed, (), 0x5067E0);
FunctionPointer(int, MenuBackButtonsPressed, (), 0x5067F0);
FunctionPointer(int, GetCurrentSave, (), 0x506800);
FunctionPointer(int, GetMenuVoice, (), 0x5068E0);
FunctionPointer(int, CountEmblemsCurrentSave, (), 0x507B30);
FunctionPointer(int, DrawAVA_BACK, (float a5), 0x507BB0);
ObjectFunc(CharSel_Qmark_Main, 0x5125A0);
FunctionPointer(int, CharSel_Load, (), 0x512840);
FunctionPointer(void, CharSel, (int a1), 0x512910);
FunctionPointer(int, CountUnlockedCharacters, (), 0x512C50);
FunctionPointer(int, PlayVideo, (int a1), 0x5130A0);
FunctionPointer(void, DisplayVideoFrame, (int width, int height), 0x5139F0);
ObjectFunc(EC0NPC_Main, 0x51ACD0);
ObjectFunc(EC0NPC_Load, 0x51ADD0);
VoidFunc(LoadEC0NPCs, 0x51AE00);
ObjectFunc(BoatEC_Main, 0x51C9A0);
ObjectFunc(OIkada, 0x51CC30);
ObjectFunc(OMast, 0x51CD20);
FunctionPointer(int, OBook1_Main, (ObjectMaster *obj), 0x51CFB0);
ObjectFunc(OBook1, 0x51D030);
ObjectFunc(OBook2, 0x51D050);
ObjectFunc(OBook3, 0x51D070);
ObjectFunc(OPier, 0x51D400);
ObjectFunc(OPalmbase_0, 0x51D760);
ObjectFunc(OParasol_1, 0x51D840);
ObjectFunc(OBchair, 0x51D930);
ObjectFunc(OPalmbase, 0x51DAB0);
ObjectFunc(OPoolwater, 0x51DC30);
ObjectFunc(OSkydeck, 0x51DFA0);
ObjectFunc(OPropeller_1, 0x51E180);
ObjectFunc(OPoolDoor, 0x51E320);
ObjectFunc(OEggmanbed, 0x51E430);
ObjectFunc(OMoveseat, 0x51E670);
ObjectFunc(OElevator_1, 0x51E910);
ObjectFunc(OLivinglight, 0x51EB50);
ObjectFunc(OSlot, 0x51EF40);
ObjectFunc(OWasher, 0x51F560);
ObjectFunc(nullsub, 0x51F680);
ObjectFunc(ODosei, 0x51F6D0);
ObjectFunc(OGunsight, 0x51FEE0);
ObjectFunc(OEggcap, 0x51FF90);
ObjectFunc(OEggseat, 0x520230);
ObjectFunc(OMonorail, 0x520CC0);
ObjectFunc(OStation, 0x521360);
ObjectFunc(OEgglift, 0x521B30);
ObjectFunc(OLightning, 0x5228A0);
ObjectFunc(OAGate, 0x522A20);
ObjectFunc(OTornado2, 0x522C00);
ObjectFunc(OSidelift, 0x523340);
ObjectFunc(OTaihou, 0x523E60);
ObjectFunc(OEcDoors, 0x524130);
ObjectFunc(OEcDoor, 0x524370);
ObjectFunc(OSLight, 0x5248D0);
ObjectFunc(OHbox, 0x524E10);
ObjectFunc(SceneChangeEC_Main, 0x525060);
ObjectFunc(EC3NPC_Main, 0x5254D0);
ObjectFunc(EC3NPC_Load, 0x5255D0);
VoidFunc(LoadEC3NPCs, 0x525600);
ObjectFunc(OReservoir, 0x525D50);
ObjectFunc(OKurotama, 0x526060);
ObjectFunc(OEggwrap, 0x526660);
ObjectFunc(OBooster, 0x526740);
ObjectFunc(OContainer_2, 0x526840);
ObjectFunc(OBlift, 0x526B20);
ObjectFunc(OJdoor, 0x526D60);
ObjectFunc(OJswitch, 0x526F30);
FunctionPointer(int, SomethingAboutHedgehogHammerStrings, (), 0x5283F0);
ObjectFunc(MinigameMogura_0, 0x5289F0);
ObjectFunc(OHammersw, 0x528E20);
ObjectFunc(OEgglift_0, 0x529290);
ObjectFunc(Monorail2_Main, 0x529EC0);
ObjectFunc(ORotary, 0x52A2C0);
ObjectFunc(OStation_0, 0x52A620);
ObjectFunc(OBridge_0, 0x52AA70);
ObjectFunc(OHallduct, 0x52ACA0);
ObjectFunc(OWRobo, 0x52B230);
ObjectFunc(OBatten, 0x52B340);
ObjectFunc(OEggdoor, 0x52B5D0);
ObjectFunc(OTarai, 0x52CB20);
ObjectFunc(OOsouji, 0x52D4F0);
ObjectFunc(SceneChangeEC2_Main, 0x52D710);
FunctionPointer(void, GetEntrance_ECarrier, (EntityData1 *a1), 0x52D820);
VoidFunc(LoadMRNPCs, 0x52F140);
FunctionPointer(void, GetEntrance_MRuins, (EntityData1 *a1), 0x530790);
ObjectFunc(OHandKey, 0x532400);
ObjectFunc(OSandEntry, 0x532760);
ObjectFunc(OKillSwitch, 0x5354E0);
ObjectFunc(PeopleMR_Main, 0x5364D0);
ObjectFunc(OTornado2_0, 0x536BC0);
ObjectFunc(LongLadder_Main, 0x536CB0);
ObjectFunc(OChgCave2, 0x536ED0);
ObjectFunc(OWLeaf2, 0x5373B0);
ObjectFunc(OWBoard, 0x5375F0);
ObjectFunc(OWLeaf1, 0x537800);
ObjectFunc(OChgWindy, 0x537CE0);
ObjectFunc(OChgCave, 0x537FB0);
ObjectFunc(OGrass_0, 0x538260);
ObjectFunc(OBigGrass, 0x538340);
ObjectFunc(OFinalegg, 0x5385A0);
ObjectFunc(OFinalway, 0x538610);
ObjectFunc(OSe, 0x5386C0);
ObjectFunc(OTree, 0x5388C0);
ObjectFunc(OPalm, 0x538980);
ObjectFunc(OEggStand, 0x538FD0);
ObjectFunc(OBigBed, 0x539070);
FunctionPointer(int, ChangeSceneMR, (), 0x539220);
ObjectFunc(OScenechg, 0x5394F0);
ObjectFunc(OIkada_0, 0x539BB0);
FunctionPointer(int, TrainsInService, (), 0x539C60);
ObjectFunc(TrainMR_Main, 0x53A6B0);
ObjectFunc(OButterfly, 0x53AFB0);
ObjectFunc(Ladder_Main, 0x53B0E0);
ObjectFunc(OKnucklesStutu, 0x53B330);
ObjectFunc(BlockEntry, 0x53BB90);
ObjectFunc(OHiddenGate, 0x53C3E0);
ObjectFunc(OLock, 0x53CA60);
ObjectFunc(MasterEme_Display, 0x53CC10);
ObjectFunc(MasterEme_Main, 0x53CF60);
ObjectFunc(ChaosEme_Display, 0x53D0B0);
ObjectFunc(ChaosEme_Main, 0x53D1B0);
ObjectFunc(OTorokkoDoor, 0x53D700);
ObjectFunc(OTorokko, 0x53DE80);
ObjectFunc(OIcecapDoor, 0x53E440);
ObjectFunc(OMountainDoor, 0x53E990);
ObjectFunc(OSandSwitch, 0x53EC30);
ObjectFunc(OEggmanDoor, 0x53EE90);
ObjectFunc(OIslandDoor, 0x53F4D0);
ObjectFunc(OMonkeyCage, 0x540730);
ObjectFunc(ODigPlace2, 0x540B30);
ObjectFunc(ODigPlace1, 0x540EE0);
ObjectFunc(ItemStand_Main, 0x5413E0);
VoidFunc(LoadPastNPCs, 0x541890);
FunctionPointer(void, GetEntrance_Past, (EntityData1 *a1), 0x542180);
ObjectFunc(OPeople, 0x543AF0);
ObjectFunc(OFiresmoke, 0x544080);
ObjectFunc(OBurning, 0x5442B0);
ObjectFunc(OFire, 0x5444F0);
ObjectFunc(ChaosEmePast_Display, 0x544560);
ObjectFunc(ChaosEmePast_Main, 0x5445D0);
ObjectFunc(MasterEmePast_Display, 0x544640);
ObjectFunc(MasterEmePast_Main, 0x5446B0);
ObjectFunc(OKnulamp, 0x544810);
ObjectFunc(OSound, 0x544890);
ObjectFunc(OSmoke, 0x544CD0);
ObjectFunc(OShida, 0x5450A0);
ObjectFunc(OSotetsu, 0x545170);
ObjectFunc(OKnudoor, 0x5452A0);
ObjectFunc(OPystairs, 0x545460);
ObjectFunc(OBigstairs, 0x545490);
ObjectFunc(OKnutree, 0x5455D0);
ObjectFunc(OCScenechg, 0x545670);
ObjectFunc(OWell, 0x5457F0);
ObjectFunc(OFallTree, 0x5458D0);
ObjectFunc(OSnake, 0x545A20);
ObjectFunc(OTree_0, 0x545AF0);
ObjectFunc(OPalm_0, 0x545C30);
ObjectFunc(Chaos0_Rain, 0x5462F0);
FunctionPointer(int, Chaos0_Rain_Load, (), 0x546320);
ObjectFunc(Chaos0_Main, 0x548640);
FunctionPointer(int, Chaos0_Load, (), 0x548B70);
ObjectFunc(OBclock_0, 0x548D30);
ObjectFunc(_0_pole, 0x548E70);
ObjectFunc(OPato0, 0x549770);
FunctionPointer(void, OHeli0_Display, (ObjectMaster *a1), 0x549840);
FunctionPointer(void, OHeli0_Main, (ObjectMaster *a1), 0x549990);
ObjectFunc(OHeli0, 0x549A30);
ObjectFunc(OGras0, 0x549A90);
ObjectFunc(OGras1, 0x549AC0);
ObjectFunc(OGras2, 0x549AF0);
ObjectFunc(OGras3, 0x549B20);
ObjectFunc(Chaos2_Main, 0x54DB90);
ObjectFunc(Chand, 0x54E2E0);
ObjectFunc(Table, 0x54E420);
VoidFunc(j_ClampGlobalColorThing_Thing, 0x551510);
ObjectFunc(Chaos4_Main, 0x552960);
FunctionPointer(int, SetClip_Chaos6K, (int), 0x556D60);
FunctionPointer(int, SetClip_Chaos6S, (int), 0x556E40);
ObjectFunc(Chaos6_Main, 0x559FC0);
ObjectFunc(Smoke, 0x55A940);
ObjectFunc(Sonic_SuperAura_Delete, 0x55E5B0);
ObjectFunc(Sonic_SuperAura_Display, 0x55E620);
ObjectFunc(Sonic_SuperAura_Main, 0x55E920);
ObjectFunc(Sonic_SuperAura_Load, 0x55FAF0);
ObjectFunc(PerfectChaos_Delete, 0x55FEB0);
ObjectFunc(PerfectChaos_Display, 0x55FED0);
ObjectFunc(PerfectChaos_Main, 0x560260);
ObjectFunc(ORedCar, 0x5640A0);
ObjectFunc(OYellowCar, 0x5640C0);
ObjectFunc(OBlueCar, 0x5640E0);
ObjectFunc(OWallA, 0x564100);
ObjectFunc(OWallB, 0x564120);
ObjectFunc(OWallC, 0x564140);
ObjectFunc(OFall0, 0x564800);
ObjectFunc(OCorn, 0x564980);
ObjectFunc(OSignal_0, 0x5649A0);
ObjectFunc(OTire, 0x5649C0);
ObjectFunc(ORoad0, 0x564C40);
ObjectFunc(ORoad1, 0x564C60);
ObjectFunc(ORoad2, 0x564C80);
ObjectFunc(ORoad3, 0x564CA0);
ObjectFunc(ORoad4, 0x564CC0);
ObjectFunc(OTentacle, 0x565280);
ObjectFunc(OGrayBld, 0x5659E0);
ObjectFunc(OBrownBld, 0x565A00);
ObjectFunc(ORedBld, 0x565A20);
ObjectFunc(AxelPanel_Main, 0x565AC0);
FunctionPointer(int, SetClip_E101R, (int), 0x568D20);
FunctionPointer(int, PlayVoiceButCheckThing, (), 0x571990);
ObjectFunc(EggHornet_Main, 0x572010);
ObjectFunc(EggHornet_Missile_Main, 0x574E00);
ObjectFunc(EggHornet_Missile_Loader_Main, 0x5750F0);
FunctionPointer(int, EggHornet_Missile_Rumble, (), 0x5751F0);
FunctionPointer(int, EggHornet_Something_Rumble, (), 0x575200);
FunctionPointer(int, PlayEggmanVoice, (int), 0x5759C0);
ObjectFunc(EggWalker_Main, 0x576650);
ObjectFunc(OKazari, 0x577470);
FunctionPointer(int, SetClip_ZERO, (int), 0x5850F0);
FunctionPointer(int, Release_MIS_C_TEXLIST, (), 0x590660);
FunctionPointer(int, MissionCompleteScreen_ProbablyDraws, (), 0x590690);
FunctionPointer(Sint32, MissionCompleteScreen, (), 0x590E60);
FunctionPointer(void, MissionStartClear_Delete, (ObjectMaster *), 0x591040);
FunctionPointer(void, MissionStartClear_Main, (ObjectMaster *), 0x591260);
FunctionPointer(int, Mission_Run, (), 0x5919C0);
FunctionPointer(int, Mission_DrawCounter, (), 0x592A60);
FunctionPointer(int, Mission_DrawTimer, (), 0x592D50);
ObjectFunc(SGene, 0x597660);
ObjectFunc(SBass, 0x5977F0);
ObjectFunc(OSfrog, 0x598040);
ObjectFunc(SandGate, 0x598200);
ObjectFunc(SandJump, 0x598220);
ObjectFunc(SnakeStatue, 0x598240);
ObjectFunc(Rock, 0x598260);
ObjectFunc(RockyHead, 0x598280);
ObjectFunc(PillarS, 0x598430);
ObjectFunc(PillarM, 0x598450);
ObjectFunc(PillarL, 0x598470);
ObjectFunc(SmallCactusA, 0x598940);
ObjectFunc(SmallCactusB, 0x598960);
ObjectFunc(BigCactusA, 0x598E30);
ObjectFunc(BigCactusB, 0x598E50);
ObjectFunc(BigCactusC, 0x598E70);
FunctionPointer(double, SandHillScoreMultiplier_Draw, (float x, float y, float _pri, __int16 n), 0x5990D0);
FunctionPointer(void, SandHillScoreMultiplier_Main, (ObjectMaster *), 0x5991A0);
ObjectFunc(OBoxSwitch, 0x59AAF0);
ObjectFunc(OKurage, 0x59B9F0);
ObjectFunc(OConnection, 0x59BB40);
ObjectFunc(OEnemycontainer, 0x59BDA0);
ObjectFunc(OCargocontainer, 0x59C0B0);
ObjectFunc(OCargotop, 0x59C3D0);
ObjectFunc(OCargo, 0x59C620);
ObjectFunc(OGate2, 0x59CA60);
ObjectFunc(Gate13, 0x59CA90);
ObjectFunc(OKaitenashiba, 0x59CDF0);
ObjectFunc(OWcmark1_0, 0x59CFB0);
ObjectFunc(OWcmark1, 0x59D0D0);
ObjectFunc(OKanso, 0x59D1F0);
ObjectFunc(Ofens, 0x59D350);
ObjectFunc(OHikari, 0x59D460);
ObjectFunc(OLight4, 0x59D5C0);
ObjectFunc(OBluelight, 0x59D760);
ObjectFunc(OGateside, 0x59D840);
ObjectFunc(OHasigo_0, 0x59D970);
ObjectFunc(OUkijima, 0x59DFA0);
ObjectFunc(OHako_0, 0x59E170);
ObjectFunc(ODaruma, 0x59E430);
ObjectFunc(OToile_door, 0x59E660);
ObjectFunc(OKaitendai, 0x59E940);
ObjectFunc(OComputer, 0x59EB00);
ObjectFunc(OSyoumei, 0x59EBF0);
ObjectFunc(OKazari2, 0x59ED50);
ObjectFunc(OKazari1, 0x59EEA0);
ObjectFunc(ODrumcan, 0x59EFE0);
ObjectFunc(OKaidan, 0x59F470);
ObjectFunc(OPropeller_0, 0x59F620);
ObjectFunc(OTukikou, 0x59F770);
ObjectFunc(OHaguruma6, 0x59FA40);
ObjectFunc(OHaguruma5, 0x59FD50);
ObjectFunc(OHaguruma4, 0x59FF30);
ObjectFunc(OHaguruma3, 0x5A0230);
ObjectFunc(OHaguruma2, 0x5A0540);
ObjectFunc(OHaguruma1, 0x5A0720);
ObjectFunc(OSasae1d, 0x5A0890);
ObjectFunc(OSasae1c, 0x5A09D0);
ObjectFunc(OSasae1b, 0x5A0B10);
ObjectFunc(OSasae1a, 0x5A0C50);
ObjectFunc(OBridge, 0x5A16B0);
ObjectFunc(OElevator, 0x5A1D70);
ObjectFunc(OGate1, 0x5A2120);
ObjectFunc(OBiribiri, 0x5A2250);
ObjectFunc(OKaitenmeter, 0x5A2490);
ObjectFunc(OKaitenkey, 0x5A2950);
ObjectFunc(OContainer_0, 0x5A2DC0);
ObjectFunc(OLight3, 0x5A2F10);
ObjectFunc(OLight2_1, 0x5A3030);
ObjectFunc(OLight1_1, 0x5A3210);
ObjectFunc(OEntotsu, 0x5A33D0);
ObjectFunc(CSwitch, 0x5A3520);
ObjectFunc(E105Enemy_Load, 0x5A4990);
ObjectFunc(OCargostart, 0x5A4D70);
ObjectFunc(OCarne, 0x5A55A0);
FunctionPointer(void, OGateSw_Display, (ObjectMaster *a2), 0x5A57E0);
FunctionPointer(void, OGateSw_Main, (ObjectMaster *a1), 0x5A5890);
ObjectFunc(OGateSw, 0x5A5960);
ObjectFunc(OBoxAna, 0x5A5B70);
ObjectFunc(OCrashTv, 0x5A6800);
ObjectFunc(ORoboTv, 0x5A6BB0);
ObjectFunc(OSubTv, 0x5A6EB0);
ObjectFunc(OBanji, 0x5A70C0);
ObjectFunc(OBreakGlass, 0x5A7B30);
ObjectFunc(OBreakKabe, 0x5A87D0);
ObjectFunc(OHiyokoyuka, 0x5A8E60);
ObjectFunc(OSuimen_0, 0x5A9100);
ObjectFunc(OKowaresuisou, 0x5A9A90);
FunctionPointer(int, SetClip_FEgg2, (int), 0x5ADC40);
FunctionPointer(int, PlayDestroyTargetVoices, (ObjectMaster *), 0x5ADE00);
FunctionPointer(int, PlayFinalEggDisposeIntrudersVoice, (ObjectMaster *), 0x5ADE90);
ObjectFunc(OTexture, 0x5AE450);
ObjectFunc(ColliGacha, 0x5AFE10);
ObjectFunc(OEGacha, 0x5B03B0);
FunctionPointer(void, SpawnBeat, (ObjectMaster *a1, void *a2), 0x5B0590);
ObjectFunc(OColorDoor2, 0x5B0730);
ObjectFunc(CFakeAmy, 0x5B07A0);
ObjectFunc(OColorDoor, 0x5B0910);
ObjectFunc(ODrum, 0x5B23B0);
ObjectFunc(CRelease, 0x5B24F0);
ObjectFunc(OLight2, 0x5B2680);
ObjectFunc(OLight1, 0x5B27B0);
ObjectFunc(OFsaku, 0x5B3620);
ObjectFunc(OGacha, 0x5B3C40);
ObjectFunc(OGshooter, 0x5B3E30);
ObjectFunc(CHane, 0x5B40A0);
ObjectFunc(OShutter_0, 0x5B41F0);
ObjectFunc(CAppZero, 0x5B43D0);
ObjectFunc(CKillZero, 0x5B4480);
ObjectFunc(OAmyDrum, 0x5B45D0);
ObjectFunc(Otatekan, 0x5B4840);
ObjectFunc(OElevator2, 0x5B4B30);
ObjectFunc(OHasigo, 0x5B4D00);
ObjectFunc(OEggKanban, 0x5B5100);
ObjectFunc(OStandLight, 0x5B5260);
ObjectFunc(OPinLamp, 0x5B5530);
ObjectFunc(OSideLamp, 0x5B5550);
ObjectFunc(OUpTarget2, 0x5B5E70);
ObjectFunc(OUpTarget1, 0x5B6590);
ObjectFunc(OTarget, 0x5B66E0);
FunctionPointer(int, FinalEggLevel2IsolationWalls, (ObjectMaster *a1), 0x5B6B30);
ObjectFunc(OHallGate, 0x5B6C20);
ObjectFunc(OElevator1, 0x5B7210);
ObjectFunc(OFun, 0x5B7600);
ObjectFunc(OBigLight, 0x5B78A0);
ObjectFunc(OUkishima, 0x5B7BF0);
ObjectFunc(OHakidashi, 0x5B7D70);
ObjectFunc(OSuikomi, 0x5B7EB0);
ObjectFunc(OArmWall, 0x5B85C0);
ObjectFunc(ORoboShutter, 0x5B8F60);
ObjectFunc(ORoboWall, 0x5B9730);
ObjectFunc(ODouble_gate, 0x5B9C40);
ObjectFunc(OGate_g, 0x5BA020);
ObjectFunc(OGate_f, 0x5BA490);
ObjectFunc(OGate_e, 0x5BA900);
ObjectFunc(OGate_d, 0x5BACF0);
ObjectFunc(OGate_c, 0x5BB160);
ObjectFunc(OGate_b, 0x5BB5D0);
ObjectFunc(OGate_a, 0x5BBA40);
ObjectFunc(_0BlueLight, 0x5BBDC0);
ObjectFunc(_0LightCamera, 0x5BC0A0);
ObjectFunc(OPurs_camera, 0x5BC200);
ObjectFunc(ORotdoor, 0x5BC4F0);
ObjectFunc(OHammer, 0x5BC760);
ObjectFunc(OSpin_tubes, 0x5BCA80);
ObjectFunc(OSpin_tubem, 0x5BCDB0);
ObjectFunc(OSpin_tubeb, 0x5BD110);
ObjectFunc(OConv_stop, 0x5BD310);
ObjectFunc(OConv_belt, 0x5BD880);
ObjectFunc(ODschg_lamp, 0x5BE660);
ObjectFunc(OMova_thorn, 0x5BE880);
FunctionPointer(void, OSetstep_Display, (ObjectMaster *a2), 0x5BE920);
ObjectFunc(OSetstep_Init, 0x5BE9B0);
ObjectFunc(OContainer, 0x5BEBA0);
ObjectFunc(OSide_arm_fix, 0x5BED90);
ObjectFunc(OUp_arm_fix, 0x5BEFA0);
ObjectFunc(OSide_arm, 0x5BF9A0);
ObjectFunc(OUp_arm, 0x5C0260);
FunctionPointer(int, Casino_Setup, (), 0x5C0960);
FunctionPointer(void, DrawBALLS, (ObjectMaster *), 0x5C0B70);
FunctionPointer(void, Casino_HandleSpawnPoints, (ObjectMaster *object), 0x5C0CC0);
ObjectFunc(OHahen1, 0x5C3750);
ObjectFunc(OHahen2, 0x5C3770);
ObjectFunc(OHahen3, 0x5C3790);
ObjectFunc(OHahen4, 0x5C37B0);
ObjectFunc(OHahen5, 0x5C37D0);
ObjectFunc(OHahen6, 0x5C37F0);
ObjectFunc(OSonzcol, 0x5C3880);
ObjectFunc(OKsoniczo, 0x5C4210);
FunctionPointer(int, SetCasinoRings, (int), 0x5C43C0);
ObjectFunc(OKinwall, 0x5C4520);
ObjectFunc(OKin, 0x5C4880);
ObjectFunc(OLighta, 0x5C48B0);
ObjectFunc(OLightb, 0x5C48D0);
ObjectFunc(OFka, 0x5C48F0);
ObjectFunc(OFkb, 0x5C4910);
ObjectFunc(OSlotisu, 0x5C4930);
ObjectFunc(ODento, 0x5C4950);
ObjectFunc(OFlowera, 0x5C4970);
ObjectFunc(OFlowerb, 0x5C4990);
ObjectFunc(OFloweral, 0x5C49B0);
ObjectFunc(OFlowerbl, 0x5C49D0);
ObjectFunc(OSaku, 0x5C49F0);
ObjectFunc(OTaru, 0x5C4A10);
ObjectFunc(OHako, 0x5C4A30);
ObjectFunc(OGbox, 0x5C4A50);
ObjectFunc(OGomib, 0x5C4A70);
ObjectFunc(OGomir, 0x5C4A90);
ObjectFunc(ORlt, 0x5C4AB0);
ObjectFunc(OYukh, 0x5C4AD0);
ObjectFunc(ORlti, 0x5C4AF0);
ObjectFunc(OKb, 0x5C4B10);
ObjectFunc(OKbr, 0x5C4B30);
ObjectFunc(OKb1, 0x5C4B50);
ObjectFunc(OKb2, 0x5C4B80);
ObjectFunc(OKb3, 0x5C4BB0);
ObjectFunc(OKb4, 0x5C4BE0);
ObjectFunc(OTdenki, 0x5C4C10);
ObjectFunc(OSdfutir, 0x5C4C30);
ObjectFunc(OSdfutib, 0x5C4C60);
ObjectFunc(OMap2f, 0x5C4C90);
ObjectFunc(OCfo, 0x5C4CB0);
ObjectFunc(OMizua, 0x5C4CD0);
ObjectFunc(OMizub, 0x5C4CF0);
ObjectFunc(OSya, 0x5C4D10);
ObjectFunc(OSyb, 0x5C4D30);
ObjectFunc(OShwrl, 0x5C4D50);
ObjectFunc(OShwhd, 0x5C4D70);
ObjectFunc(ONeokab, 0x5C4D90);
ObjectFunc(OSuiteki, 0x5C5270);
ObjectFunc(OHosin, 0x5C56F0);
ObjectFunc(OMud, 0x5C5960);
ObjectFunc(OMudout, 0x5C59A0);
ObjectFunc(OKaizs, 0x5C63D0);
ObjectFunc(OKaizc, 0x5C6EC0);
ObjectFunc(OKaizb, 0x5C79B0);
ObjectFunc(OKaiza, 0x5C8560);
ObjectFunc(OLetsgo, 0x5C88D0);
ObjectFunc(OHae, 0x5C8A10);
ObjectFunc(OWsr2, 0x5C96B0);
ObjectFunc(OWsr, 0x5C9940);
ObjectFunc(OLhtg, 0x5C9B40);
ObjectFunc(OLhtr, 0x5C9D60);
ObjectFunc(O6slot2, 0x5CA180);
ObjectFunc(OCcyl2, 0x5CA680);
ObjectFunc(OCcap, 0x5CA700);
ObjectFunc(OCrect2, 0x5CA850);
ObjectFunc(OKdanl, 0x5CAA20);
ObjectFunc(OKdanm, 0x5CAA50);
ObjectFunc(ONeonk, 0x5CACD0);
ObjectFunc(OTikeianim, 0x5CB160);
ObjectFunc(OFook, 0x5CB440);
ObjectFunc(OFanfan, 0x5CBFE0);
ObjectFunc(OHasgo, 0x5CC5F0);
ObjectFunc(OSlotkan, 0x5CCAE0);
ObjectFunc(OCardkan, 0x5CCD90);
ObjectFunc(OM1fneo, 0x5CCF40);
ObjectFunc(OTenss, 0x5CD2D0);
ObjectFunc(OTensc, 0x5CD2F0);
ObjectFunc(OShat1, 0x5CDA50);
ObjectFunc(OShat2, 0x5CDA60);
ObjectFunc(OMb, 0x5CDBE0);
ObjectFunc(OTsdoor, 0x5CE240);
ObjectFunc(OSarm, 0x5CE570);
ObjectFunc(O_SLX_Display, 0x5CE5A0);
ObjectFunc(O_SLX_Main, 0x5CE650);
ObjectFunc(O_SLB, 0x5CE740);
ObjectFunc(O_SLG, 0x5CE760);
ObjectFunc(O_SLR, 0x5CE780);
ObjectFunc(O_SLY, 0x5CE7A0);
ObjectFunc(OKbs, 0x5CE910);
ObjectFunc(OKbc, 0x5CEB30);
ObjectFunc(OFl, 0x5CED50);
ObjectFunc(OSonz, 0x5CEF20);
ObjectFunc(Shower_Main, 0x5CF450);
ObjectFunc(OKinka, 0x5CF6C0);
ObjectFunc(OHand, 0x5D0480);
ObjectFunc(OKaza, 0x5D0BA0);
ObjectFunc(OKazb, 0x5D0C00);
ObjectFunc(OKazc, 0x5D0C20);
ObjectFunc(OKazd, 0x5D0C40);
ObjectFunc(OKaze, 0x5D0C60);
ObjectFunc(OKazf, 0x5D0C80);
ObjectFunc(OKazg, 0x5D0CA0);
ObjectFunc(OTeleport, 0x5D1450);
ObjectFunc(OLion, 0x5D22D0);
ObjectFunc(OHyoujiki, 0x5D2890);
ObjectFunc(OManf, 0x5D2DF0);
ObjectFunc(OSrdoor, 0x5D33B0);
ObjectFunc(OKkdoor, 0x5D3990);
ObjectFunc(ODmgsphere, 0x5D3A20);
ObjectFunc(OHaguruma, 0x5D3C80);
ObjectFunc(OCrystal, 0x5D3E00);
ObjectFunc(O6slot, 0x5D4200);
ObjectFunc(OTutua, 0x5D4D40);
ObjectFunc(OTutub, 0x5D4D70);
ObjectFunc(OTutuc, 0x5D4DC0);
ObjectFunc(banjudoor, 0x5D4EF0);
ObjectFunc(Sitaalegg, 0x5D5060);
ObjectFunc(Uealegg, 0x5D5090);
ObjectFunc(Emergency, 0x5D5240);
ObjectFunc(Starneon, 0x5D5430);
ObjectFunc(Yajineon, 0x5D5460);
ObjectFunc(Densyoku, 0x5D54C0);
ObjectFunc(Carduv, 0x5D56E0);
ObjectFunc(Piantama, 0x5D5AE0);
ObjectFunc(Pianpach, 0x5D5B30);
ObjectFunc(Pianwalk, 0x5D5B70);
ObjectFunc(Pianfish, 0x5D5BB0);
ObjectFunc(Pianslee, 0x5D5BF0);
ObjectFunc(Tanemono, 0x5D5D00);
ObjectFunc(Kazariyaji, 0x5D5E20);
ObjectFunc(Loop_Main, 0x5D6040);
ObjectFunc(Bigstar, 0x5D61A0);
ObjectFunc(Saico, 0x5D6350);
ObjectFunc(Path, 0x5D6AC0);
FunctionPointer(void, Carddisp_Display, (ObjectMaster *), 0x5D73F0);
FunctionPointer(void, Carddisp_Main, (ObjectMaster *), 0x5D7500);
ObjectFunc(Carddisp, 0x5D7510);
ObjectFunc(Card, 0x5D7760);
ObjectFunc(Idea, 0x5D78E0);
ObjectFunc(Ideacap, 0x5D79F0);
ObjectFunc(Houdail, 0x5D7E20);
ObjectFunc(Houdair, 0x5D7E40);
ObjectFunc(Shoogail, 0x5D8170);
ObjectFunc(Shoogair, 0x5D8180);
ObjectFunc(Kurokabe, 0x5D82C0);
ObjectFunc(Neon, 0x5D8560);
ObjectFunc(Neonkabe, 0x5D85A0);
ObjectFunc(Akaaomoido, 0x5D8920);
ObjectFunc(Tuuka, 0x5D9450);
ObjectFunc(Rightlane, 0x5D9520);
ObjectFunc(Rollupr, 0x5D98A0);
ObjectFunc(Rollupl, 0x5D98F0);
ObjectFunc(Rolldownl, 0x5D9940);
ObjectFunc(Dtarget, 0x5DA1C0);
ObjectFunc(Dtarget2, 0x5DA1E0);
ObjectFunc(Jdail, 0x5DA8A0);
ObjectFunc(Jdair, 0x5DA8B0);
ObjectFunc(Roll, 0x5DB150);
ObjectFunc(Stoperc, 0x5DB6A0);
ObjectFunc(Stoperl, 0x5DB700);
ObjectFunc(Stoperr, 0x5DB790);
ObjectFunc(Slingl, 0x5DBBF0);
ObjectFunc(Slingr, 0x5DBC60);
ObjectFunc(Bumper1, 0x5DC000);
ObjectFunc(Bumper2, 0x5DC040);
ObjectFunc(Flipperl, 0x5DCB40);
ObjectFunc(Flipperrr, 0x5DCB90);
ObjectFunc(Flipperlx, 0x5DCBE0);
ObjectFunc(Flipperrx, 0x5DCC00);
ObjectFunc(Startshot, 0x5DCCE0);
ObjectFunc(Cc3, 0x5DCEC0);
ObjectFunc(ChaosEmeGoal_Casinopolis_Main, 0x5DD0A0);
FunctionPointer(void, PinballJackpot_Display, (ObjectMaster *), 0x5E1020);
FunctionPointer(void, PinballJackpot_Sprite_Main, (ObjectMaster *), 0x5E11A0);
FunctionPointer(int, PinballJackpot_Main, (ObjectMaster *parent), 0x5E12C0);
ThiscallFunctionPointer(void, PinballJackpot_LoadMaybe, (ObjectMaster *obj), 0x5E14E1);
FunctionPointer(void, ORmirror_Display, (ObjectMaster *), 0x5E2380);
FunctionPointer(void, LostWorldFog_Fade, (float a1, float increment), 0x5E24B0);
FunctionPointer(void, LostWorldFogThing_Delete, (ObjectMaster *), 0x5E2530);
ObjectFunc(OFog_sw, 0x5E25A0);
ObjectFunc(ORmirror_Main, 0x5E2850);
ObjectFunc(O_mrsna, 0x5E2B80);
ObjectFunc(LostWorldFogThing_Main, 0x5E3020);
ObjectFunc(OFog_li, 0x5E3240);
ObjectFunc(OHoghasira, 0x5E32A0);
ObjectFunc(OBigmr, 0x5E3310);
ObjectFunc(OBroobj_0, 0x5E62D0);
ObjectFunc(OTaki_0, 0x5E6550);
ObjectFunc(Aokiswitch, 0x5E6B80);
ObjectFunc(Ukiuki, 0x5E6F60);
ObjectFunc(Rndbox, 0x5E7210);
ObjectFunc(ORDbox, 0x5E7490);
ObjectFunc(ODou1, 0x5E7590);
ObjectFunc(ODou2, 0x5E75B0);
ObjectFunc(OSwDoorX, 0x5E7940);
ObjectFunc(OSwDoor, 0x5E8020);
ObjectFunc(Ofire, 0x5E82F0);
FunctionPointer(void, OSuimen_Switch_Display, (ObjectMaster *obj), 0x5E83C0);
FunctionPointer(void, OSuimen_Switch, (ObjectMaster *), 0x5E8410);
FunctionPointer(void, OSuimen_Display, (ObjectMaster *a2), 0x5E8520);
FunctionPointer(void, OSuimen_Main, (ObjectMaster *a1), 0x5E8580);
ObjectFunc(OSuimen, 0x5E86C0);
ObjectFunc(OTap, 0x5E8900);
ObjectFunc(ORlight, 0x5E8960);
ObjectFunc(OTswitch, 0x5E8DB0);
ObjectFunc(OTpanel, 0x5E9430);
ObjectFunc(Hakoaf, 0x5E9840);
ObjectFunc(Turncube, 0x5E9AB0);
ObjectFunc(Lostjumpdai, 0x5E9D10);
ObjectFunc(Burningbow, 0x5E9FE0);
ObjectFunc(Kusaa, 0x5EA680);
ObjectFunc(Kusab, 0x5EA6A0);
ObjectFunc(Kusac, 0x5EA6C0);
ObjectFunc(Shitakusa, 0x5EA6E0);
ObjectFunc(Gakekusa, 0x5EA700);
ObjectFunc(Togedaiud, 0x5EAB30);
ObjectFunc(Togedaid, 0x5EAB90);
ObjectFunc(Gate_0, 0x5EAD30);
ObjectFunc(Toge, 0x5EAD70);
ObjectFunc(Hebigate, 0x5EAEF0);
ObjectFunc(Hasira1, 0x5EB0E0);
ObjectFunc(Hasira2, 0x5EB100);
ObjectFunc(Sekicyuu, 0x5EB120);
ObjectFunc(Terasu, 0x5EB140);
ObjectFunc(Hebizou, 0x5EB790);
ObjectFunc(Hebizoul, 0x5EB7A0);
ObjectFunc(Hebizour, 0x5EB7C0);
ObjectFunc(Ashiba01, 0x5EB8B0);
ObjectFunc(Ashiba02, 0x5EB900);
ObjectFunc(Ashiba03, 0x5EB950);
ObjectFunc(Ashikabe01, 0x5EB9A0);
ObjectFunc(Ashikabe02, 0x5EB9F0);
ObjectFunc(ORraf, 0x5EC340);
FunctionPointer(void, Radder_Display, (ObjectMaster *a2), 0x5EE330);
FunctionPointer(void, VariousSolidThings_Delete, (ObjectMaster *a1), 0x5EE520);
ObjectFunc(W_wall, 0x5EF2B0);
ObjectFunc(NextAct_Main, 0x5EF9D0);
ObjectFunc(Hangpole, 0x5EFB00);
FunctionPointer(void, Radder_Main, (ObjectMaster *a1), 0x5EFC50);
ObjectFunc(Fence_l, 0x5EFD90);
ObjectFunc(Fence_m, 0x5EFDE0);
ObjectFunc(Fence_s, 0x5EFE30);
ObjectFunc(Hang_c, 0x5F0040);
ObjectFunc(Trail, 0x5F00E0);
ObjectFunc(Homing, 0x5F02B0);
ObjectFunc(Edge_0, 0x5F0740);
ObjectFunc(Branch_l, 0x5F0750);
ObjectFunc(Branch_s, 0x5F0760);
ObjectFunc(Pole_s, 0x5F0770);
ObjectFunc(Pole_l, 0x5F0780);
ObjectFunc(Base_l, 0x5F0790);
ObjectFunc(Rest_6, 0x5F07A0);
ObjectFunc(Rest_45, 0x5F07B0);
ObjectFunc(Block_0, 0x5F07C0);
ObjectFunc(Block_1, 0x5F07D0);
ObjectFunc(Block_2, 0x5F07E0);
ObjectFunc(Block_3, 0x5F07F0);
ObjectFunc(Block_4, 0x5F0800);
ObjectFunc(Block_5, 0x5F0810);
ObjectFunc(Cover, 0x5F0820);
ObjectFunc(Screw, 0x5F0830);
ObjectFunc(Launcher, 0x5F0840);
ObjectFunc(Block_6, 0x5F0850);
ObjectFunc(Block_7, 0x5F0860);
ObjectFunc(Rail_l, 0x5F0870);
ObjectFunc(Rail_s, 0x5F0880);
ObjectFunc(Lantern, 0x5F0890);
ObjectFunc(Tower_i, 0x5F08A0);
ObjectFunc(Rest_9, 0x5F08B0);
ObjectFunc(ColdFan, 0x5F08C0);
ObjectFunc(Carrier, 0x5F08D0);
ObjectFunc(Carrier2, 0x5F08E0);
ObjectFunc(Carrier3, 0x5F08F0);
ObjectFunc(Ctrltower, 0x5F0900);
ObjectFunc(Scope, 0x5F0910);
ObjectFunc(Edge, 0x5F0920);
ObjectFunc(La, 0x5F0930);
ObjectFunc(Lb, 0x5F0940);
ObjectFunc(Lc, 0x5F0950);
ObjectFunc(Stop, 0x5F0960);
ObjectFunc(S_wing, 0x5F0970);
ObjectFunc(S_map, 0x5F0980);
ObjectFunc(S_obj, 0x5F0990);
ObjectFunc(S_eng, 0x5F09A0);
ObjectFunc(Cyl_s, 0x5F09B0);
ObjectFunc(Tank_h, 0x5F0A40);
ObjectFunc(Top_0, 0x5F0AB0);
ObjectFunc(Radder, 0x5F0B30);
ObjectFunc(Duct_0, 0x5F0CC0);
ObjectFunc(Untei, 0x5F0E60);
ObjectFunc(Hang_a, 0x5F0FB0);
ObjectFunc(T_colli, 0x5F1060);
ObjectFunc(Flap01, 0x5F1080);
ObjectFunc(Flap02, 0x5F10B0);
ObjectFunc(OLever, 0x5F2070);
ObjectFunc(O_vent, 0x5F26C0);
ObjectFunc(O_decal, 0x5F27A0);
ObjectFunc(O_signal, 0x5F2860);
ObjectFunc(O_crane, 0x5F3200);
ObjectFunc(Snd_eng, 0x5F3280);
ObjectFunc(Cannon_h2, 0x5F40A0);
ObjectFunc(OMeka, 0x5F4180);
ObjectFunc(OTutu, 0x5F4240);
ObjectFunc(OEscalator, 0x5F43C0);
ObjectFunc(ORoboA, 0x5F4830);
ObjectFunc(ORoboB, 0x5F4D70);
ObjectFunc(OBuket, 0x5F5330);
ObjectFunc(OBox, 0x5F5530);
ObjectFunc(OUeki, 0x5F56E0);
ObjectFunc(OSkyEv, 0x5F5C70);
ObjectFunc(ORaneA, 0x5F6130);
ObjectFunc(ORaneB, 0x5F6190);
ObjectFunc(ORaneC, 0x5F61F0);
ObjectFunc(ORaneD, 0x5F6250);
ObjectFunc(ORaneE, 0x5F62B0);
ObjectFunc(ORaneF, 0x5F6310);
ObjectFunc(OAnaA, 0x5F6370);
ObjectFunc(OAnaB, 0x5F63D0);
ObjectFunc(Chambr, 0x5F7150);
ObjectFunc(OCon, 0x5F7530);
ObjectFunc(ODCon, 0x5F76A0);
ObjectFunc(Cannon_m, 0x5F8530);
ObjectFunc(Cannon_s2, 0x5F8E50);
ObjectFunc(Cannon_s1, 0x5F9760);
ObjectFunc(Cyl_2, 0x5F9A10);
ObjectFunc(Cyl_1, 0x5F9C90);
ObjectFunc(Edge_1, 0x5FA570);
ObjectFunc(Cyl_0, 0x5FA7F0);
ObjectFunc(Talap_1, 0x5FAA70);
ObjectFunc(Connect_0, 0x5FB210);
ObjectFunc(Talap_0, 0x5FB980);
ObjectFunc(Cannon_h, 0x5FC620);
ObjectFunc(Cannon_s, 0x5FCCC0);
ObjectFunc(Cannon_b, 0x5FD840);
ObjectFunc(ExRock_Main, 0x601EB0);
ObjectFunc(O_yougan_Main, 0x602670);
ObjectFunc(OYe102_Main, 0x6028B0);
ObjectFunc(OMyougan_Main, 0x602960);
ObjectFunc(OZako_Main, 0x603F80);
ObjectFunc(OZako_b_Main, 0x603FF0);
ObjectFunc(OBpole_Main, 0x6042A0);
ObjectFunc(E104Enemy_Main, 0x605B40);
ObjectFunc(OBlow_Main, 0x605C50);
ObjectFunc(OBlow2_Main, 0x605D40);
ObjectFunc(OSaku_Main, 0x605DF0);
ObjectFunc(OSaku2_Main, 0x605E40);
ObjectFunc(ORouya_Main, 0x605E90);
ObjectFunc(Mt_soto_Main, 0x605EE0);
ObjectFunc(Mt_tbox_Main, 0x605F00);
ObjectFunc(Mt_tsaku_Main, 0x605F50);
ObjectFunc(OHataki_Main, 0x6063B0);
ObjectFunc(OLamp1_Main, 0x606820);
ObjectFunc(OLamp2_Main, 0x606840);
ObjectFunc(ODeka_Main, 0x606B20);
ObjectFunc(OHago_Main, 0x606F20);
ObjectFunc(OBat, 0x6077D0);
ObjectFunc(OTree1_Main, 0x607BA0);
ObjectFunc(OTree2_Main, 0x607CB0);
ObjectFunc(OAsiba1_Main, 0x607F60);
ObjectFunc(OTuri1_Main, 0x6080D0);
ObjectFunc(OTuri2_Main, 0x6080F0);
ObjectFunc(OTuri3_Main, 0x608110);
ObjectFunc(OTuri4_Main, 0x608130);
FunctionPointer(int, OMtKusa_Thing, (), 0x608150);
FunctionPointer(void, OMtKusa_Display, (ObjectMaster *a2), 0x6082C0);
ObjectFunc(OMtKusa_Main, 0x608380);
ObjectFunc(SyuujinB_Main, 0x6085B0);
ObjectFunc(SyuujinR_Main, 0x608630);
ObjectFunc(SyuujinG_Main, 0x6086B0);
ObjectFunc(Magmacnt_Main, 0x608730);
ObjectFunc(OFuta_Main, 0x6089B0);
ObjectFunc(MtnSpidera_Main, 0x609360);
ObjectFunc(MtnSpiderb_Main, 0x609560);
ObjectFunc(OBreakstep2_Main, 0x609F00);
ObjectFunc(OBreakstep_Main, 0x60A9F0);
ObjectFunc(OPropeller, 0x60AFC0);
ObjectFunc(OSignboard_Main, 0x60B080);
ObjectFunc(ORelayP_Main, 0x60B1E0);
ObjectFunc(OGear_Main, 0x60B390);
ObjectFunc(OErupt_Main, 0x60C640);
ObjectFunc(Ogl_Main, 0x60C820);
ObjectFunc(OLightb_Main, 0x60CAF0);
ObjectFunc(OLight_Main, 0x60CB20);
ObjectFunc(OHanama_Main, 0x60D260);
ObjectFunc(OVent_Main, 0x60D4F0);
ObjectFunc(OPipei_Main, 0x60D570);
ObjectFunc(OPipebox_Main, 0x60D5B0);
ObjectFunc(OPipel_Main, 0x60D5F0);
ObjectFunc(OGrave_Main, 0x60D810);
ObjectFunc(OHaka_Main, 0x60DD50);
ObjectFunc(OChair_Main, 0x60DD80);
ObjectFunc(OMasiba_Main, 0x60E150);
ObjectFunc(OBonehead_Main, 0x60E410);
ObjectFunc(OBonestick_Main, 0x60E4D0);
ObjectFunc(O_untei1_Main, 0x60E8B0);
ObjectFunc(O_untei2_Main, 0x60E8F0);
ObjectFunc(O_untei3_Main, 0x60E930);
ObjectFunc(O_untei16_Main, 0x60E980);
ObjectFunc(O_komokosu_Main, 0x60EA70);
ObjectFunc(O_honoo_Main, 0x60EE00);
ObjectFunc(SkyBox_SpeedHighway_Load, 0x610A70);
ObjectFunc(OHelia, 0x613E30);
ObjectFunc(OHelib, 0x613E80);
ObjectFunc(Missile, 0x614380);
ObjectFunc(OGfence, 0x614B00);
ObjectFunc(OGcurb, 0x614B30);
ObjectFunc(OGfence02, 0x614B60);
ObjectFunc(OGpinpin, 0x614B90);
ObjectFunc(OGff, 0x614BC0);
ObjectFunc(OGrafta, 0x614BF0);
ObjectFunc(OGraftc, 0x614C20);
ObjectFunc(OGgrena, 0x614C50);
ObjectFunc(OGgrenb, 0x614C80);
ObjectFunc(OGgrend, 0x614CB0);
ObjectFunc(OGclight, 0x614CE0);
ObjectFunc(ORing2, 0x614D80);
ObjectFunc(HighRaftA, 0x614E40);
ObjectFunc(HighRaftC, 0x614E60);
ObjectFunc(OTunagi, 0x614E80);
ObjectFunc(OHelipL, 0x614ED0);
ObjectFunc(OHwBell, 0x615200);
ObjectFunc(OStplant02, 0x615310);
ObjectFunc(OStplant01, 0x6153C0);
ObjectFunc(OGg, 0x615410);
ObjectFunc(OFf, 0x615450);
ObjectFunc(OLmpa, 0x6155A0);
ObjectFunc(OTokei, 0x615740);
ObjectFunc(OSiba01, 0x615780);
ObjectFunc(OSiba02, 0x6157D0);
ObjectFunc(OGreen, 0x615810);
ObjectFunc(OGreena, 0x615830);
ObjectFunc(OGreenb, 0x615880);
ObjectFunc(OGreend, 0x6158D0);
ObjectFunc(OGreene, 0x615920);
ObjectFunc(OFence02, 0x615940);
ObjectFunc(OCurb, 0x615990);
ObjectFunc(OCone1, 0x615C60);
ObjectFunc(OCone2, 0x615CB0);
ObjectFunc(OAntena, 0x615EB0);
FunctionPointer(void, OSuimen_Delete, (ObjectMaster *a1), 0x615F30);
ObjectFunc(OEscalator1, 0x616150);
ObjectFunc(OEscalator2, 0x6161B0);
ObjectFunc(OPinpin, 0x616210);
ObjectFunc(OLamp01, 0x6163D0);
ObjectFunc(OLamp02, 0x616410);
ObjectFunc(OLamp_0, 0x616450);
ObjectFunc(OClight, 0x616770);
ObjectFunc(OHydbass, 0x616C90);
ObjectFunc(OBaketu, 0x617160);
ObjectFunc(OKanbana, 0x617290);
ObjectFunc(OKanbanb, 0x617330);
ObjectFunc(OSign1, 0x6173D0);
ObjectFunc(OPoster0, 0x617420);
ObjectFunc(OPoster1, 0x617440);
ObjectFunc(OPoster2, 0x617460);
ObjectFunc(ONeon1, 0x617680);
ObjectFunc(ONeon2, 0x6176C0);
ObjectFunc(ONeon3, 0x617700);
ObjectFunc(ONeon4, 0x617740);
ObjectFunc(OFence, 0x617780);
ObjectFunc(OBench, 0x6177C0);
ObjectFunc(ONbox1, 0x6178F0);
ObjectFunc(ONbox2, 0x617940);
ObjectFunc(ONbox3, 0x617990);
ObjectFunc(ONbox4, 0x6179E0);
ObjectFunc(OPost1, 0x617AE0);
ObjectFunc(OPost4, 0x617B10);
ObjectFunc(OStp4s, 0x617C70);
ObjectFunc(OStp4t, 0x617F00);
ObjectFunc(OFlyst, 0x618030);
ObjectFunc(OJamer, 0x6186D0);
ObjectFunc(OArcade01, 0x6188E0);
ObjectFunc(OArcade02, 0x6188F0);
ObjectFunc(OArcade03, 0x618900);
ObjectFunc(OSlight, 0x618AB0);
ObjectFunc(OTurnasi, 0x619340);
ObjectFunc(OSignb, 0x619960);
ObjectFunc(OTanka, 0x61A330);
ObjectFunc(OGlass2, 0x61ACA0);
ObjectFunc(OGlass, 0x61AE80);
ObjectFunc(OCrane, 0x61B500);
ObjectFunc(OFount, 0x61BDC0);
ObjectFunc(OEv, 0x61C740);
ObjectFunc(SkyBox_TwinklePark_Load, 0x61D570);
ObjectFunc(OCoaster, 0x61DC90);
ObjectFunc(ODoor, 0x61E5F0);
ObjectFunc(ODoorB, 0x61E7C0);
ObjectFunc(ODoubleDoor, 0x61EAC0);
ObjectFunc(ODoorMirror, 0x61EEE0);
ObjectFunc(ORoof, 0x61F030);
ObjectFunc(OTpRing, 0x61F4A0);
ObjectFunc(OFence1, 0x61F7D0);
ObjectFunc(OFence2, 0x61F7F0);
ObjectFunc(OPlant1, 0x61F810);
ObjectFunc(OPlant2, 0x61F830);
ObjectFunc(OPlant3, 0x61F850);
ObjectFunc(OPlant4, 0x61F870);
ObjectFunc(OPlant5, 0x61F890);
ObjectFunc(OPlant6, 0x61F8B0);
ObjectFunc(OBowWindow, 0x61F8D0);
ObjectFunc(OGirder, 0x61F8F0);
ObjectFunc(OLamp, 0x61FA00);
ObjectFunc(OCandle1, 0x61FE00);
ObjectFunc(OCandle2, 0x61FF50);
ObjectFunc(OFlag1, 0x6201A0);
ObjectFunc(OFlag2, 0x620230);
ObjectFunc(OFlagWLamp, 0x620540);
ObjectFunc(OPit, 0x6207F0);
ObjectFunc(OPirates, 0x620AF0);
ObjectFunc(OGoRound, 0x621670);
ObjectFunc(OFloat, 0x621850);
ObjectFunc(OFlyer, 0x621BD0);
ObjectFunc(OFoothold, 0x621DB0);
ObjectFunc(OCatapult, 0x6223C0);
ObjectFunc(OBowling, 0x623450);
ObjectFunc(OBall_0, 0x623BB0);
ObjectFunc(OBarrel, 0x624020);
ObjectFunc(OShutter, 0x624380);
ObjectFunc(OMonitor, 0x6247C0);
ObjectFunc(MinigameMogura, 0x6268D0);
FunctionPointer(int, CheckSkyChaseActionThing, (), 0x626CC0);
ObjectFunc(Tornado_Delete, 0x627410);
ObjectFunc(Tornado_Display, 0x627430);
FunctionPointer(int, TornadoTarget_UsingLockOn, (), 0x627740);
ObjectFunc(Tornado_Main, 0x627EC0);
FunctionPointer(int, SkyChaseInit, (), 0x6281A0);
FunctionPointer(void, TornadoHP_Draw, (ObjectMaster *), 0x628490);
FunctionPointer(int, TornadoHP_Main, (ObjectMaster *), 0x628740);
FunctionPointer(char, TornadoHP_Load, (int x, int _y, signed int a3), 0x628890);
FunctionPointer(void, TornadoTarget_Display, (ObjectMaster *a1), 0x6288F0);
FunctionPointer(int, TornadoTarget_MoveTargetWithinBounds, (), 0x628970);
FunctionPointer(void, TornadoTargetSprite_TargetLock_Display, (ObjectMaster *a1), 0x628DB0);
FunctionPointer(int, TornadoTarget_IDK, (), 0x628FB0);
FunctionPointer(int, TornadoTargetSprite_TargetLock_Main, (ObjectMaster *), 0x629060);
ObjectFunc(TornadoTarget_Main, 0x6293B0);
ObjectFunc(Boss_Display, 0x62CC50);
ObjectFunc(Boss_Main, 0x62D140);
ObjectFunc(Rocket, 0x62D300);
ObjectFunc(Arm, 0x62D490);
ObjectFunc(Renzako, 0x62D680);
ObjectFunc(Hodai, 0x62DBE0);
ObjectFunc(Kirai, 0x62DDC0);
ObjectFunc(Tv1, 0x62DFE0);
ObjectFunc(Isu1, 0x62E000);
ObjectFunc(Isu2, 0x62E020);
ObjectFunc(Tansu, 0x62E040);
ObjectFunc(Kabin, 0x62E060);
ObjectFunc(Kaiga, 0x62E080);
ObjectFunc(Tv2, 0x62E0A0);
ObjectFunc(Reizou, 0x62E0C0);
ObjectFunc(Sofa, 0x62E0E0);
ObjectFunc(Tana, 0x62E100);
ObjectFunc(Yakan, 0x62E120);
ObjectFunc(Zako1, 0x62E360);
FunctionPointer(int, WelcomeToTwinklePark, (ObjectMaster *), 0x62EC60);
FunctionPointer(void, j_DisableFog, (ObjectMaster *), 0x62EFE0);
ObjectFunc(SSNPC_Main, 0x62F450);
ObjectFunc(SSNPC_Load, 0x62F5A0);
VoidFunc(LoadSSNPCs, 0x62F5D0);
FunctionPointer(void, GetEntrance_SSquare, (EntityData1 *a1), 0x62F6D0);
ObjectFunc(BurgerShopStatue_Main, 0x630780);
ObjectFunc(OHotelFront, 0x630FB0);
ObjectFunc(OHotelBack, 0x631030);
ObjectFunc(OHotelPool, 0x6310B0);
ObjectFunc(ORouka, 0x631480);
ObjectFunc(People_Main, 0x634980);
FunctionPointer(int, LoadSSMessages, (), 0x634DE0);
ObjectFunc(Cream_Display, 0x634F40);
FunctionPointer(int, Cream_Main, (ObjectMaster *), 0x635270);
ObjectFunc(OKartchanger, 0x635900);
ObjectFunc(OLightchanger, 0x635B90);
ObjectFunc(OHiddendoor, 0x635EE0);
ObjectFunc(ChaoCard_Main, 0x6360E0);
ObjectFunc(GoldEgg_Main, 0x6364C0);
ObjectFunc(FakeEgg_Main, 0x6364F0);
ObjectFunc(OPowerbox, 0x6365C0);
ObjectFunc(OKeyblock, 0x636940);
ObjectFunc(OContainer_1, 0x636B30);
ObjectFunc(OOfficeDoor, 0x6370C0);
ObjectFunc(OOfficeBarricade, 0x637580);
ObjectFunc(KartPass_Main, 0x637C90);
ObjectFunc(IceKey_Main, 0x637FE0);
FunctionPointer(int, CheckCasinoOpen, (), 0x6380B0);
ObjectFunc(CasinoDoor_Main, 0x638350);
ObjectFunc(OCasinoDoor, 0x6387E0);
ObjectFunc(OStationBack, 0x638990);
ObjectFunc(OBarricadde, 0x638BF0);
ObjectFunc(OTenbouEle, 0x6392F0);
ObjectFunc(OHotelEle, 0x639380);
ObjectFunc(OHighEle, 0x6393F0);
ObjectFunc(OWakusei, 0x6396E0);
ObjectFunc(ORotyu, 0x639920);
ObjectFunc(OMSaku, 0x63A100);
ObjectFunc(OHwbshutter, 0x63A530);
ObjectFunc(OParasol_0, 0x63A700);
ObjectFunc(OPoolchair, 0x63A7E0);
ObjectFunc(OGaitou, 0x63AA10);
ObjectFunc(OSoukaki, 0x63AB30);
ObjectFunc(OStdoor, 0x63B160);
ObjectFunc(OS2Boxmi, 0x63B230);
ObjectFunc(OS2Boxfuta, 0x63B2C0);
ObjectFunc(OS2Aorobot, 0x63B350);
ObjectFunc(Boat_Main, 0x63B610);
ObjectFunc(OKartkanban, 0x63B840);
ObjectFunc(OTwkanban, 0x63B940);
ObjectFunc(OTwtesuri, 0x63BA20);
ObjectFunc(OElekanban, 0x63BB50);
ObjectFunc(OS2Frame, 0x63BBF0);
ObjectFunc(OS2Robot, 0x63BC80);
ObjectFunc(OS2Radio, 0x63BD00);
ObjectFunc(OS2Kaseki, 0x63BD70);
ObjectFunc(OS1Chair, 0x63BE20);
ObjectFunc(OCard, 0x63C370);
ObjectFunc(OSignal, 0x63C470);
ObjectFunc(OS1knbn, 0x63C5C0);
ObjectFunc(OS1solt, 0x63C700);
ObjectFunc(OS1dnto, 0x63C820);
ObjectFunc(OS2saku, 0x63C980);
ObjectFunc(OBclock, 0x63D040);
ObjectFunc(OKomonokeikoto, 0x63D190);
ObjectFunc(OKomonormlt, 0x63D1C0);
ObjectFunc(OKomonobin, 0x63D230);
ObjectFunc(OKomonoexit, 0x63D280);
ObjectFunc(OKomonoisua, 0x63D2D0);
ObjectFunc(OKomonoisub, 0x63D320);
ObjectFunc(OEggstand, 0x63D5B0);
ObjectFunc(OEleboxIn, 0x63DB70);
ObjectFunc(OEleboxOut, 0x63DB90);
ObjectFunc(OElevator_0, 0x63E0D0);
ObjectFunc(OTwbdoor, 0x63E4A0);
ObjectFunc(OTwadoor, 0x63E760);
ObjectFunc(OUradoor, 0x63EA40);
ObjectFunc(OTwindoor, 0x63EE40);
ObjectFunc(OShop2door, 0x63F330);
ObjectFunc(OShop1door, 0x63F7D0);
ObjectFunc(OKartdoor, 0x63FBC0);
ObjectFunc(OPole_0, 0x63FCA0);
ObjectFunc(SSTrain_Main, 0x640430);
ObjectFunc(SceneChange_Main, 0x640850);
FunctionPointer(void, DrawTiledBG, (__int16 *a1, int a2, float a3, float a4, float a5, float hscale, float vscale), 0x642AE0);
FunctionPointer(ObjectMaster *, LoadRecapBackground, (int a2, int bgNum), 0x643D40);
FunctionPointer(int, start, (), 0x643F96);
FunctionPointer(void, SetSeed, (int newSeed), 0x6443B5);
FunctionPointer(void, j__free, (void *Memory), 0x6443E2);
FunctionPointer(__int32, j__atol, (const char *Str), 0x6453A2);
FunctionPointer(int, j_unknown_libname_27, (), 0x6487BE);
FunctionPointer(void, TailsVS_DrawWinLose, (ObjectMaster *), 0x6EF510);
FunctionPointer(void, j_RunObject, (ObjectMaster *a1), 0x6F2E40);
FunctionPointer(Sint32, PRSDec, (Uint8 *a1, Uint8 *a2), 0x6FC900);
FunctionPointer(int, LoadGGMenuPVM, (), 0x6FE010);
VoidFunc(j_CopyRawControllers, 0x703430);
VoidFunc(j_j_CopyRawControllers, 0x703440);
FunctionPointer(void, SetNextChaoStage, (SADXChaoStage stage), 0x715110);
VoidFunc(GoToNextChaoStage, 0x715130);
FunctionPointer(SADXChaoStage, GetCurrentChaoStage, (), 0x715140);
VoidFunc(ChaoMain_Constructor, 0x715440);
FunctionPointer(int, LoadNextChaoStage, (), 0x715580);
ObjectFunc(ChaoGarden, 0x715820);
VoidFunc(LoadHeldChao, 0x716200);
FunctionPointer(void, GBAManager_Main, (ObjectMaster *), 0x716850);
FunctionPointer(void, GBAManagerDestruct, (ObjectMaster *), 0x716A60);
FunctionPointer(int, GBAManagerCreate, (), 0x716BA0);
FunctionPointer(Uint8 *, GetChaoSaveAddress, (), 0x716FA0);
VoidFunc(SetupChaoSaveFile, 0x7172D0);
FunctionPointer(Uint8 *, j_GetChaoSaveAddress, (), 0x717450);
VoidFunc(LoadHeldItem, 0x717C80);
FunctionPointer(int, LoadChaoPVPs, (), 0x717E90);
FunctionPointer(void, LoadChaoTexlist, (const char *PVMName, NJS_TEXLIST *TexListPtr, unsigned __int16 a3), 0x718880);
FunctionPointer(void, LoadGameDLL, (const char *name, int a4), 0x718A20);
FunctionPointer(int, LoadMarket, (), 0x718BB0);
FunctionPointer(int, LoadMRGarden, (), 0x718E90);
FunctionPointer(int, LoadECGarden, (), 0x7191D0);
FunctionPointer(int, LoadSSGarden, (), 0x719600);
FunctionPointer(int, LoadRaceEntry, (), 0x7199B0);
FunctionPointer(int, LoadChaoNamingMachine, (), 0x719C40);
FunctionPointer(int, LoadChaoRace, (), 0x719DB0);
FunctionPointer(ObjectMaster *, GetChaoObject, (unsigned __int16 zero, unsigned __int16 index), 0x71A1F0);
FunctionPointer(signed int, AddToGlobalChaoThingMaybe, (unsigned __int16 a1, ObjectMaster *obj, __int16 a3, ChaoData *data), 0x71A600);
FunctionPointer(signed int, _AddToGlobalChaoThingMaybe, (unsigned __int16 a1, ObjectMaster *a2, __int16 a3), 0x71A840);
VoidFunc(ChaoDX_Message_PlayerAction, 0x71B3B0);
ObjectFunc(Butterfly, 0x71BBB0);
ObjectFunc(Cracker, 0x71BEA0);
ObjectFunc(BowChao, 0x71BFF0);
ObjectFunc(CheerChao, 0x71C180);
ObjectFunc(StartMark, 0x71C2E0);
ObjectFunc(SmallScreen, 0x71C300);
ObjectFunc(Islet, 0x71C320);
ObjectFunc(Scoop, 0x71C3D0);
ObjectFunc(Bugle, 0x71C5F0);
ObjectFunc(WateringCan, 0x71C7D0);
ObjectFunc(WhiteFlower, 0x71C9D0);
ObjectFunc(PurpleFlower, 0x71C9F0);
ObjectFunc(Flag, 0x71CA70);
ObjectFunc(Coconut, 0x71CBC0);
ObjectFunc(Palm, 0x71CD50);
ObjectFunc(Zone, 0x71CE60);
ObjectFunc(EntryButton, 0x71D230);
FunctionPointer(void, InitChaoDNA, (ChaoDNA *a1), 0x71D250);
ObjectFunc(Chao_Main, 0x7202D0);
FunctionPointer(void, Chao_Delete, (ObjectMaster *a2), 0x720390);
ObjectFunc(Chao_Display, 0x7204B0);
FunctionPointer(ObjectMaster *, CreateChao, (ChaoData *chaoData, int a2, void *buffer, NJS_VECTOR *position, Angle angle), 0x720670);
ObjectFunc(Chao_Tree_Main, 0x7212A0);
FunctionPointer(void, DisplayDebugShape, (int a1, int a2, int a3, int a4, Uint32 color, float depth_maybe), 0x723B70);
FunctionPointer(int, Load_al_stg_kinder_ad_tex, (), 0x724EA0);
FunctionPointer(int, j_GetMarketRings, (), 0x7257E0);
ObjectFunc(Chao_Transport_Display, 0x729250);
ObjectFunc(Chao_Transport_Main, 0x7293E0);
FunctionPointer(void, LoadChaoTransporter, (ObjectMaster *parent, int destination, float xpos, float ypos, float zpos, int yrot), 0x729550);
ObjectFunc(Chao_Name_Machine_Main, 0x729610);
ObjectFunc(Chao_Name_Machine_Delete, 0x729D80);
ObjectFunc(Chao_Name_Machine_Display, 0x729DB0);
ObjectFunc(Chao_Name_Machine_Load, 0x729EE0);
FunctionPointer(void, LoadChaoNameMachine, (Vector3 *position, int yrotation), 0x729F40);
FunctionPointer(int, Print_ChaoStgGarden02MR_Daytime_epilog, (), 0x72A810);
FunctionPointer(int, Print_ChaoStgGarden02MR_Evening_epilog, (), 0x72A8A0);
FunctionPointer(int, Print_ChaoStgGarden02MR_Night_epilog, (), 0x72A930);
FunctionPointer(int, Chao_Race_Entry_Door, (ObjectMaster *a1), 0x72AAA0);
FunctionPointer(int, Chao_Station_Door, (ObjectMaster *a1), 0x72AD60);
FunctionPointer(void, Chao_Market_Door, (ObjectMaster *), 0x72AEE0);
VoidFunc(LoadButtonIndicators, 0x72B330);
FunctionPointer(void *, LoadChaoMessageFile, (char *filename, int language), 0x72C060);
FunctionPointer(void, j__HeapFree, (LPVOID lpMem), 0x72C110);
FunctionPointer(void *, LoadChaoMessageFile2, (char *filename), 0x72C180);
FunctionPointer(void, DrawOverlaySprite, (NJS_SPRITE *a1, Int n, float pri), 0x72CC70);
FunctionPointer(void, ProgressChaoEvolution, (ObjectMaster *a1, float a2), 0x730870);
FunctionPointer(void, IncrementPowerRun, (ObjectMaster *a1, float increment), 0x7308D0);
FunctionPointer(void, IncrementFlySwim, (ObjectMaster *a1, float increment), 0x730930);
FunctionPointer(void, IncrementAlignment, (ObjectMaster *a1, float increment), 0x730990);
FunctionPointer(void, SetupChaoAppearance, (ChaoDataBase *a1, char *a2), 0x730B30);
FunctionPointer(int, DisableChao, (), 0x731CE0);
FunctionPointer(int, IsChaoEnabled, (), 0x731CF0);
FunctionPointer(void, ChaoDebug_MenuBack, (ObjectMaster *a1), 0x731DE0);
FunctionPointer(void, SetChaoDebugFunction_Disabled, (void (__cdecl *object_function)(ObjectMaster *), char *name), 0x731E00);
FunctionPointer(void, SetChaoDebugFunction_Enabled, (void (__cdecl *object_function)(ObjectMaster *), char *name), 0x731E40);
VoidFunc(SetChaoDebugFunctions, 0x731E80);
FunctionPointer(void, DoChaoHudThingThing, (ChaoHudThing *a1, NJS_COLOR color), 0x732850);
FunctionPointer(void, SetDefaultChaoAnimalParts, (ObjectMaster *a1), 0x734930);
FunctionPointer(int, LoadChaoMotionTable, (ObjectMaster *a1), 0x734EC0);
FunctionPointer(ChaoData2 *, AllocateChaoData2, (ObjectMaster *chao), 0x73FE70);
FunctionPointer(double, ChaoDistanceFromPlayer, (ObjectMaster *a1, int n), 0x7401A0);
FunctionPointer(void, FreeSomeChaoData, (ObjectMaster *a1), 0x742060);
FunctionPointer(signed int, ResetChao, (ObjectMaster *chao, ChaoType type), 0x7420C0);
FunctionPointer(int, GetChaoBondIndexFromCharID, (), 0x742140);
FunctionPointer(void, ConvertBattleCharBondsToDX, (ObjectMaster *a1), 0x7421A0);
FunctionPointer(char, GetChaoCharacterBond, (ObjectMaster *a1), 0x742220);
FunctionPointer(void, ChaoDebug_LIKE, (ObjectMaster *a1), 0x7423E0);
ObjectFunc(ChaoEgg_Main, 0x7435F0);
FunctionPointer(ObjectMaster *, CreateChaoEgg, (ChaoDNA *dna, ChaoDataBase *data, int n, Vector3 *a4, int a5), 0x743960);
ThiscallFunctionPointer(void, SetChaoName, (void *data), 0x74B2B0);
FunctionPointer(void, ChaoDebug_SHAPE, (ObjectMaster *a1), 0x757580);
FunctionPointer(void, ChaoDebug_ENV_MAP, (ObjectMaster *a1), 0x757B70);
FunctionPointer(void, ChaoDebug_COLOR, (ObjectMaster *a1), 0x757C30);
FunctionPointer(void, ChaoDebug_PARTS_DX, (ObjectMaster *a1), 0x757CD0);
FunctionPointer(void, ChaoDebug_MINIMAL_FLAG_DX, (ObjectMaster *a1), 0x757F00);
FunctionPointer(void, ChaoDebug_PARTS_BTL, (ObjectMaster *a1), 0x7580A0);
FunctionPointer(void, ChaoDebug_MINIMAL_FLAG_BTL, (ObjectMaster *a1), 0x7581A0);
FunctionPointer(void, ChaoDebug_MEDAL, (ObjectMaster *a1), 0x758370);
FunctionPointer(void, ChaoDebug_OBAKE, (ObjectMaster *a1), 0x7584A0);
FunctionPointer(void, ChaoDebug_BEHAVIOR, (ObjectMaster *a1), 0x758640);
FunctionPointer(void, ChaoDebug_GROUP, (ObjectMaster *a1), 0x758BE0);
FunctionPointer(void, ChaoDebug_STORY_FLAG, (ObjectMaster *a1), 0x758DC0);
FunctionPointer(void, ChaoDebug_ART_FLAG, (ObjectMaster *a1), 0x758F40);
FunctionPointer(void, ChaoDebug_DANCE_FLAG, (ObjectMaster *a1), 0x759080);
FunctionPointer(void, ChaoDebug_SONG_FLAG, (ObjectMaster *a1), 0x7591C0);
FunctionPointer(void, ChaoDebug_INST_FLAG, (ObjectMaster *a1), 0x759300);
FunctionPointer(void, ChaoDebug_LANDMARK, (ObjectMaster *a1), 0x759460);
FunctionPointer(void, ChaoDebug_MOTION, (ObjectMaster *a1), 0x7594C0);
FunctionPointer(void, ChaoDebug_LIFE, (ObjectMaster *a1), 0x759590);
FunctionPointer(void, ChaoDebug_SE, (ObjectMaster *a1), 0x759620);
FunctionPointer(void, ChaoDebug_VOICE, (ObjectMaster *a1), 0x759760);
FunctionPointer(void, ChaoDebug_VOICE2, (ObjectMaster *a1), 0x7598A0);
FunctionPointer(void, ChaoDebug_JINGLE, (ObjectMaster *a1), 0x7599E0);
FunctionPointer(void, ChaoDebug_SKILL, (ObjectMaster *a1), 0x759B80);
FunctionPointer(void, ChaoDebug_PARAMETER, (ObjectMaster *a1), 0x759DC0);
VoidFunc(j_nullsub_1, 0x76DAB0);
FunctionPointer(int, RegKeyExists, (int, LPCSTR lpValueName, LPBYTE lpData, DWORD cbData), 0x773261);
StdcallFunctionPointer(int, DisableD3DXPSGP, (BOOL Data), 0x77350C);
FunctionPointer(int, j_nullsub_2, (), 0x77DD80);
FunctionPointer(void, Direct3D_EnableHudAlpha, (bool enable), 0x77DD90);
VoidFunc(j_Direct3D_TextureFilterLinear, 0x77DDE0);
FunctionPointer(void, SetHudColorAndTextureNum, (int n, NJS_COLOR color), 0x77DDF0);
FunctionPointer(void, njDrawSprite2D, (NJS_SPRITE *_sp, Int n, Float pri, NJD_SPRITE attr), 0x77E050);
FunctionPointer(int, Direct3D_SetAlphaBlend, (int, int), 0x77EC60);
FunctionPointer(int, Direct3D_EnableAlphaBlend, (int), 0x77ECE0);
FunctionPointer(void, Direct3D_ZFUNC_thing, (char index), 0x77ED00);
FunctionPointer(int, Direct3D_EnableZWrite, (int), 0x77ED20);
FunctionPointer(void, j_DeltaTime_Multiplier, (int a1), 0x77ED40);
FunctionPointer(int, Direct3D_SetCullMode, (int), 0x77ED50);
FunctionPointer(int, ReleaseKeyboardDevice, (), 0x77F0F0);
FunctionPointer(void, njGetTexSurface_Maybe, (NJS_TEXMEMLIST *a1, Uint8 *data), 0x77F2F0);
ThiscallFunctionPointer(Sint32, Direct3D_SetNJSTexture_List, (NJS_TEXLIST *_this), 0x77F3D0);
FastcallFunctionPointer(Sint32, njSetTextureNum, (int num), 0x77F440);
FunctionPointer(signed int, AddExtension, (char *destination, const char *source, char *extension), 0x77F470);
FunctionPointer(NJS_TEXMEMLIST *, GetCachedTexture, (int gbix), 0x77F5B0);
FunctionPointer(void, SetGlobalTextureArray, (NJS_TEXMEMLIST *textureList, int length), 0x77F610);
FunctionPointer(Sint32, njLoadPaletteFile, (const char *filename, NJS_TEXPALETTE *pl), 0x77F700);
FunctionPointer(NJS_TEXMEMLIST *, TexMemList_PixelFormat, (NJS_TEXINFO *texinfo, int gbix), 0x77F7F0);
FunctionPointer(void, LoadPVM_ReadsGlobalIndex, (NJS_TEXMEMLIST *tex), 0x77F910);
ThiscallFunctionPointer(Sint32, njSetTextureNumG, (Uint32 gbix), 0x77F9D0);
ThiscallFunctionPointer(signed int, j_njSetTextureNumG, (int gbix), 0x77FA60);
VoidFunc(njReleaseTextureAll, 0x77FB30);
FunctionPointer(NJS_TEXMEMLIST *, LoadPVR, (void *data, int gbix), 0x77FBD0);
FunctionPointer(Sint32, njLoadTexture, (NJS_TEXLIST *texlist), 0x77FC80);
FunctionPointer(Sint32, njLoadTexturePvmMemory, (Uint8 *addr, NJS_TEXLIST *texList), 0x77FD80);
FunctionPointer(Sint32, njLoadTexturePvmFile, (const char *filename, NJS_TEXLIST *texList), 0x77FEB0);
FunctionPointer(void, njSetConstantMaterial, (NJS_ARGB *color), 0x77FF90);
StdcallFunctionPointer(BOOL, QueryPerformanceCounter_, (LARGE_INTEGER *lpPerformanceCount), 0x780140);
FastcallFunctionPointer(void, njInitMatrix, (NJS_MATRIX *matrixStack, Sint32 n, Int flag), 0x780180);
FunctionPointer(void, SetDebugFontSize, (unsigned __int16 a1), 0x7808C0);
FunctionPointer(void, SetDebugFontColor, (int a1), 0x7808E0);
FunctionPointer(void, DisplayDebugString, (signed int position, char *text), 0x7808F0);
FunctionPointer(void, PrintDebugNumber, (signed int position, int value, signed int numdigits), 0x780970);
FunctionPointer(void, DisplayDebugFloat, (int position, float value, signed int percision), 0x780AC0);
FunctionPointer(void, DisplayDebugStringFormatted, (signed int position, char *text, ...), 0x780B30);
VoidFunc(Direct3D_NextScene, 0x780BE0);
FunctionPointer(double, SetNjScreenToPreset, (int a1, int a2, int frame_multiplier), 0x780C30);
FunctionPointer(int, HeapAllocateThing, (DWORD dwBytes), 0x780F60);
FunctionPointer(LPVOID, _HeapAlloc, (int count, int size), 0x780F90);
FunctionPointer(void, _HeapFree, (LPVOID lpMem), 0x780FB0);
FunctionPointer(int, j_CompareOSVersionInfoForSomeReason, (), 0x781010);
VoidFunc(j_Direct3D_EndScene, 0x781020);
FunctionPointer(int, j_Direct3D_Present, (), 0x781030);
FunctionPointer(int, j_Direct3D_Clear, (), 0x781040);
VoidFunc(j_Direct3D_BeginScene, 0x781050);
VoidFunc(j_j_DeltaSleep, 0x781060);
FunctionPointer(void, Direct3D_SetProjectionMatrix_, (D3DMATRIX *matrix), 0x781250);
ThiscallFunctionPointer(void, SetStartupProjection, (D3DMATRIX *_this), 0x781570);
FunctionPointer(void, njSetScreenDist, (int bams), 0x7815C0);
FunctionPointer(void, SetDrawDistance, (float min, float max), 0x7815E0);
FastcallFunctionPointer(D3DMATRIX *, njMatrixSomething, (NJS_MATRIX *matrix, NJS_VECTOR *vectorA, NJS_VECTOR *vectorB), 0x781670);
VoidFunc(PopMatrix, 0x7816E0);
ThiscallFunctionPointer(void, njScaleEx, (NJS_VECTOR *Scale), 0x781700);
FastcallFunctionPointer(void, njRotateXYZ, (NJS_MATRIX *m, Angle angx, Angle angy, Angle angz), 0x781770);
FastcallFunctionPointer(void, njRotateZYX, (NJS_MATRIX *m, Angle angx, Angle angy, Angle angz), 0x7819C0);
ThiscallFunctionPointer(void, njTranslateEx, (NJS_VECTOR *v), 0x781C10);
VoidFunc(PushMatrix, 0x781C80);
FunctionPointer(int, Direct3D_SetDiffuse, (), 0x781CC0);
ThiscallFunctionPointer(int, SetMaterialDiffuse, (NJS_MATERIAL *m), 0x781EC0);
ThiscallFunctionPointer(void, Direct3D_ParseMaterial, (NJS_MATERIAL *material), 0x784850);
ThiscallFunctionPointer(void, CopyMatrixFromStack, (NJS_MATRIX *m), 0x784BC0);
ThiscallFunctionPointer(void, njTranslate, (NJS_MATRIX *m, Float x, Float y, Float z), 0x784BE0);
ThiscallFunctionPointer(void, njScale, (NJS_MATRIX *m, Float sx, Float sy, Float sz), 0x784C70);
FastcallFunctionPointer(void, CopyMatrixToStack, (NJS_MATRIX *a1, NJS_MATRIX *a2), 0x784D00);
FunctionPointer(void, njTranslate2, (NJS_MATRIX *m, float x, float y, float z), 0x784D30);
FunctionPointer(void, GetIdentityMatrix_0, (NJS_MATRIX_PTR a2), 0x784D50);
FastcallFunctionPointer(void, TransformCoordinate, (NJS_MATRIX *m, NJS_VECTOR *dir, NJS_VECTOR *out), 0x784D60);
FastcallFunctionPointer(void, njRotateY, (NJS_MATRIX *m, Angle ang), 0x784E20);
FastcallFunctionPointer(void, njRotateX, (NJS_MATRIX *m, Angle ang), 0x784EE0);
FastcallFunctionPointer(void, njRotateZ, (NJS_MATRIX *m, Angle ang), 0x784FB0);
FastcallFunctionPointer(void, njTranslateV, (NJS_MATRIX *m, NJS_VECTOR *v), 0x785070);
ThiscallFunctionPointer(void, njPushMatrix, (NJS_MATRIX *m), 0x7850F0);
ThiscallFunctionPointer(void, njPopMatrix, (Uint32 n), 0x785140);
FastcallFunctionPointer(void, njAddVector, (NJS_VECTOR *vd, NJS_VECTOR *vs), 0x785170);
FunctionPointer(Sint32, pdVibMxStart, (Uint32 port, const PDS_VIBPARAM *a2), 0x785280);
FunctionPointer(Sint32, pdVibMxStop, (Uint32 port), 0x785330);
FunctionPointer(int, CreatesVertexBufferAndLocks, (int), 0x7853D0);
FastcallFunctionPointer(void, SetTexnameInfo, (NJS_TEXNAME *texname, char *filename, NJS_TEXMEMLIST *texaddr, int attr), 0x7871C0);
StdcallFunctionPointer(double, squareroot, (float), 0x7871F0);
VoidFunc(Direct3D_EnableFog_, 0x787220);
VoidFunc(Direct3D_DisableFog_, 0x787230);
FunctionPointer(void, j_njSetFogColor, (Uint32 c), 0x787240);
FunctionPointer(void, njGenerateFogTable3, (float *fog, float n, float f), 0x787260);
ThiscallFunctionPointer(void, GetIdentityMatrix, (NJS_MATRIX* m), 0x7875F0);
FastcallFunctionPointer(void, njSubVector, (NJS_VECTOR *vd, NJS_VECTOR *vs), 0x787610);
FastcallFunctionPointer(float, njScalor, (NJS_VECTOR *v), 0x787CC0);
FastcallFunctionPointer(double, njInnerProduct, (NJS_VECTOR *v1, NJS_VECTOR *v2), 0x787CF0);
FastcallFunctionPointer(double, njScalor2, (NJS_VECTOR *v), 0x787E10);
ThiscallFunctionPointer(Float, njTan, (Angle n), 0x787E30);
FunctionPointer(void, SetupScreen, (NJS_SCREEN *m), 0x788240);
FastcallFunctionPointer(void, njSetPvmTextureList_Maybe, (NJS_TEXLIST *texlist, NJS_TEXNAME *texname, pvm_thing *filename, int size), 0x788260);
FastcallFunctionPointer(Float, njSin, (Angle n), 0x788290);
FastcallFunctionPointer(Float, njCos, (Angle n), 0x7883C0);
FastcallFunctionPointer(void, njProjectScreen, (NJS_MATRIX *m, NJS_VECTOR *p3, NJS_POINT2 *p2), 0x788700);
FastcallFunctionPointer(void, njScaleV, (NJS_MATRIX *m, Vector3 *v), 0x788A50);
FunctionPointer(void, DrawChunkModel, (NJS_OBJECT *a1), 0x789830);
VoidFunc(QueryPerformance, 0x789960);
FunctionPointer(void, DeltaTime_Multiplier, (int a1), 0x7899A0);
VoidFunc(DeltaSleep, 0x7899E0);
VoidFunc(HandleWindowMessages, 0x789BD0);
FunctionPointer(int, Exit, (), 0x789DD0);
StdcallFunctionPointer(LRESULT, WndProc, (HWND a1, UINT Msg, WPARAM wParam, LPARAM a4), 0x789DE0);
VoidFunc(Direct3D_SetChunkModelRenderState, 0x78A010);
FunctionPointer(void, RenderChunkModel, (NJS_OBJECT *a1), 0x78AB80);
FunctionPointer(void, DisplayDebugShape_, (int color, float a2, float a3, float a4, float a5, float depth_maybe), 0x78B4C0);
VoidFunc(Direct3D_SetMenuMode, 0x78B780);
VoidFunc(Direct3D_TextureFilterLinear, 0x78B800);
FunctionPointer(int, Direct3D_DiffuseSourceVertexColor, (), 0x78B840);
VoidFunc(Direct3D_BeginScene, 0x78B880);
VoidFunc(Direct3D_EndScene, 0x78B960);
FunctionPointer(int, Direct3D_Present, (), 0x78BA30);
FunctionPointer(int, Direct3D_Clear, (), 0x78BA50);
FunctionPointer(int, CompareOSVersionInfoForSomeReason, (), 0x78BAB0);
VoidFunc(j_DeltaSleep, 0x78BAC0);
FunctionPointer(int, Direct3D_SetNullTexture, (), 0x78BBA0);
ThiscallFunctionPointer(void, njReleaseTextureLow, (NJS_TEXMEMLIST *texmemlist), 0x78CD50);
ThiscallFunctionPointer(int, Direct3D_SetNJSTexture, (NJS_TEXMEMLIST *t), 0x78CF20);
ThiscallFunctionPointer(void, DoSomethingWithPalette, (NJS_TEXPALETTE *p), 0x78CF80);
FastcallFunctionPointer(void, Direct3D_SetTextureNum, (int index), 0x78D140);
ThiscallFunctionPointer(void, Direct3D_SetHudColor, (NJS_COLOR color), 0x78D3E0);
FunctionPointer(int, Direct3D_EnableLighting, (), 0x78E700);
StdcallFunctionPointer(void, Direct3D_SetProjectionMatrix, (float hfov, float NearPlane, float FarPlane), 0x791170);
VoidFunc(Direct3D_SetViewportAndTransform, 0x7912E0);
FunctionPointer(int, InitDirect3D8, (), 0x791610);
FunctionPointer(int, Direct3D_SetDefaultRenderState, (), 0x791860);
ThiscallFunctionPointer(HRESULT, Direct3D_SetTextureAlphaMode, (char a1), 0x791940);
FunctionPointer(int, Direct3D_SetTransform256, (), 0x791AD0);
FunctionPointer(int, CreateKeyboardDevice, (int, int), 0x791B10);
FunctionPointer(void, DirectInput_CreateDevice, (int a1, int a2, int a3, int a4), 0x791D60);
FunctionPointer(Uint8 *, LoadPVx, (const char *str), 0x7929D0);
FunctionPointer(void, j__HeapFree_0, (LPVOID lpMem), 0x792A70);
ThiscallFunctionPointer(Uint8 *, LoadPVM_Seek, (void *data), 0x792A90);
FunctionPointer(int, QueueDebugMessage, (), 0x793A80);
FunctionPointer(int, _InitDirect3D8, (), 0x793D80);
FunctionPointer(int, Direct3D_EndScene_Release, (), 0x793D90);
FunctionPointer(signed int, GetTimeMaybe, (time_t *a1), 0x793DA0);
FunctionPointer(char *, GetWindowClassName, (), 0x793F60);
FunctionPointer(signed int, Direct3D_SetupPresentParameters, (D3DFORMAT *a1, D3DFORMAT *a2), 0x794060);
FunctionPointer(int, Direct3D_CheckAdapters, (), 0x7941D0);
FunctionPointer(int, SetDisplayAdapterMaybe, (), 0x7943C0);
FunctionPointer(int, Direct3D_SetDefaultTextureStageState, (), 0x7943F0);
FunctionPointer(int, Direct3D_EnvironmentMap, (), 0x7944B0);
VoidFunc(Direct3D_ResetTextureTransform, 0x794500);
VoidFunc(Direct3D_EnableFog, 0x794950);
VoidFunc(Direct3D_DisableFog, 0x794980);
FunctionPointer(void, njSetFogColor, (Uint32 c), 0x7949B0);
VoidFunc(SetFogDensityMaybe_2, 0x7949F0);
FunctionPointer(void, SetFogDensityMaybe, (unsigned __int16 n), 0x794DB0);
FunctionPointer(int, Direct3D_ConfigureFog, (), 0x794EC0);
FunctionPointer(int, GetD3DCapThing, (int), 0x795930);
VoidFunc(Direct3D_ConfigureLights, 0x796710);
ObjectFunc(Cart_Main, 0x79A9E0);
ObjectFunc(RingGroup_Main, 0x79B2F0);
ObjectFunc(OShip, 0x79B8B0);
ObjectFunc(OSatellite, 0x79BC80);
ObjectFunc(ORocket, 0x79C210);
ObjectFunc(OArch1, 0x79C430);
ObjectFunc(OArch2, 0x79C750);
ObjectFunc(OLight1_0, 0x79CB20);
ObjectFunc(OLight2_0, 0x79CBB0);
ObjectFunc(OPole, 0x79CDF0);
ObjectFunc(OPlanet1, 0x79D120);
ObjectFunc(OPlanet2, 0x79D140);
ObjectFunc(OPlanet3, 0x79D160);
ObjectFunc(OMeteorite1, 0x79D180);
ObjectFunc(OMeteorite2, 0x79D1A0);
ObjectFunc(OMeteorite3, 0x79D1C0);
ObjectFunc(ODashPanel4Cart, 0x79D450);
ObjectFunc(OJumpPanel4Cart, 0x79D9D0);
ObjectFunc(OPanel, 0x79DBC0);
FunctionPointer(void, SpringH_Display, (ObjectMaster *), 0x79F650);
FunctionPointer(void, SpringH_Main, (ObjectMaster *), 0x79F710);
ObjectFunc(SpringH_Load, 0x79F860);
ObjectFunc(BoaBoa_Main, 0x7A0330);
ObjectFunc(RhinoTank_Main, 0x7A1380);
FunctionPointer(void, TikalHint_Main_1, (ObjectMaster *), 0x7A15A0);
FunctionPointer(void, TikalHint_Main_2, (ObjectMaster *a1), 0x7A1610);
FunctionPointer(void, TikalHint_Main_3, (ObjectMaster *a1), 0x7A1770);
FunctionPointer(void, TikalHint_Main_4, (ObjectMaster *a1), 0x7A17F0);
FunctionPointer(void, TikalHint_Delete, (int a1), 0x7A1910);
ObjectFunc(TikalHint_Load, 0x7A1AA0);
FunctionPointer(void, TikalHint_Main_5, (ObjectMaster *a2), 0x7A1C90);
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
FunctionPointer(void, SpringB_Display, (ObjectMaster *a1), 0x7A4AE0);
ObjectFunc(Spring_Main, 0x7A4C30);
ObjectFunc(SpringB_Main, 0x7A4E50);
ObjectFunc(Froggy_Display, 0x7A6BB0);
ObjectFunc(Froggy_Main, 0x7A7AD0);
ObjectFunc(Bubbles_Main, 0x7A8A60);
ObjectFunc(Bubble_Main, 0x7A8E50);
ObjectFunc(KnucklesHint_Main, 0x7A9140);
ObjectFunc(HintMonitor_Display, 0x7A9460);
FunctionPointer(double, HintMonitor_Delete, (void *), 0x7A9620);
ObjectFunc(HintMonitor_Main, 0x7A9B40);
ObjectFunc(HintBox, 0x7A9C60);
FunctionPointer(void, Sweep_Display, (ObjectMaster *), 0x7AA3D0);
FunctionPointer(void, Sweep_Main, (ObjectMaster *), 0x7AA870);
ObjectFunc(Sweep_Load, 0x7AA960);
ObjectFunc(RingLine_Main, 0x7ABE90);
ObjectFunc(RingLineV_Main, 0x7AC4F0);
ObjectFunc(OSampleS, 0x7AC7B0);
ObjectFunc(OSampleM, 0x7AC810);
ObjectFunc(OSampleL, 0x7AC870);
ObjectFunc(OSampleSw, 0x7AC8D0);
FunctionPointer(signed int, ProcessNBFile, (void *a1), 0x7B0260);
ObjectFunc(Zero_Main, 0x7B0640);
ObjectFunc(Zero_Load, 0x7B0A70);
ObjectFunc(CScenechanger, 0x7B0C80);
ObjectFunc(OClrobj, 0x7B0DA0);
ObjectFunc(OCartStopper, 0x7B0EC0);
FunctionPointer(void, j_ObjectSelectedDebug, (ObjectMaster *obj), 0x7B14F0);
ObjectFunc(EBuyon, 0x7B2E00);
ObjectFunc(Tikal_Delete, 0x7B32C0);
ObjectFunc(Tikal_Display, 0x7B33A0);
ObjectFunc(Tikal_Main, 0x7B40C0);
ObjectFunc(Eggman_Delete, 0x7B4420);
ObjectFunc(Eggman_Main, 0x7B4EF0);
FunctionPointer(void, DebugSetMute_wrapper, (int a1), 0x7B7798);
VoidFunc(InitSonicAnimData, 0x7CEC90);
VoidFunc(InitEC0NPCData, 0x7D2720);
VoidFunc(InitEC3NPCData, 0x7D2CF0);
VoidFunc(InitSSNPCData, 0x7D5810);

// Bool __usercall@<eax>(NJS_TEXLIST *a1@<eax>)
static const void *const VerifyTexListPtr = (void*)0x403250;
static inline Bool VerifyTexList(NJS_TEXLIST *a1)
{
	Bool result;
	__asm
	{
		mov eax, [a1]
		call VerifyTexListPtr
		mov result, eax
	}
	return result;
}

// void __usercall(int a1@<eax>)
static const void *const UnloadsAllTheThingsBecauseYouKnowPtr = (void*)0x406F00;
static inline void UnloadsAllTheThingsBecauseYouKnow(int a1)
{
	__asm
	{
		mov eax, [a1]
		call UnloadsAllTheThingsBecauseYouKnowPtr
	}
}

// void __usercall(int a1@<ebp>, NJS_SPRITE *_sp, int n, NJD_SPRITE attr)
static const void *const njDrawSprite3D_4Ptr = (void*)0x407130;
static inline void njDrawSprite3D_4(int a1, NJS_SPRITE *_sp, int n, NJD_SPRITE attr)
{
	__asm
	{
		push [attr]
		push [n]
		push [_sp]
		mov ebp, [a1]
		call njDrawSprite3D_4Ptr
		add esp, 12
	}
}

// ObjectMaster *__usercall@<eax>(signed int index@<edx>, void (__cdecl *LoadSub)(ObjectMaster *)@<edi>)
static const void *const AllocateObjectMasterPtr = (void*)0x40B030;
static inline ObjectMaster * AllocateObjectMaster(signed int index, void (__cdecl *LoadSub)(ObjectMaster *))
{
	ObjectMaster * result;
	__asm
	{
		mov edi, [LoadSub]
		mov edx, [index]
		call AllocateObjectMasterPtr
		mov result, eax
	}
	return result;
}

// void __usercall(int act@<ecx>, char *FileName)
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

// void __usercall(int act@<ecx>, char *FileName)
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

// void __usercall(signed int ringCount@<eax>, unsigned __int8 digits@<bl>, NJS_SPRITE *hud@<esi>)
static const void *const HudDisplayRingsPtr = (void*)0x425960;
static inline void HudDisplayRings(signed int ringCount, unsigned __int8 digits, NJS_SPRITE *hud)
{
	__asm
	{
		mov esi, [hud]
		mov bl, [digits]
		mov eax, [ringCount]
		call HudDisplayRingsPtr
	}
}

// void __usercall(ObjectMaster *a1@<eax>)
static const void *const ScoreMole_MainPtr = (void*)0x42A1B0;
static inline void ScoreMole_Main(ObjectMaster *a1)
{
	__asm
	{
		mov eax, [a1]
		call ScoreMole_MainPtr
	}
}

// unsigned __int16 __usercall@<ax>(Uint8 *a2@<esi>)
static const void *const CalcSaveChecksumPtr = (void*)0x42CF90;
static inline unsigned __int16 CalcSaveChecksum(Uint8 *a2)
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

// void __usercall(SaveFileData *a1@<edi>)
static const void *const WriteEventFlagsToSaveFilePtr = (void*)0x42D0D0;
static inline void WriteEventFlagsToSaveFile(SaveFileData *a1)
{
	__asm
	{
		mov edi, [a1]
		call WriteEventFlagsToSaveFilePtr
	}
}

// void __usercall(SaveFileData *a1@<edi>)
static const void *const WriteNPCFlagsToSaveFilePtr = (void*)0x42D130;
static inline void WriteNPCFlagsToSaveFile(SaveFileData *a1)
{
	__asm
	{
		mov edi, [a1]
		call WriteNPCFlagsToSaveFilePtr
	}
}

// void __usercall(SaveFileData *a2@<esi>)
static const void *const ReadEventFlagsFromSaveFilePtr = (void*)0x42D250;
static inline void ReadEventFlagsFromSaveFile(SaveFileData *a2)
{
	__asm
	{
		mov esi, [a2]
		call ReadEventFlagsFromSaveFilePtr
	}
}

// void __usercall(SaveFileData *a2@<esi>)
static const void *const ReadNPCFlagsFromSaveFilePtr = (void*)0x42D2A0;
static inline void ReadNPCFlagsFromSaveFile(SaveFileData *a2)
{
	__asm
	{
		mov esi, [a2]
		call ReadNPCFlagsFromSaveFilePtr
	}
}

// void __usercall(NJS_TEXLIST *a1@<eax>)
static const void *const EV_LoadPvmPtr = (void*)0x42F940;
static inline void EV_LoadPvm(NJS_TEXLIST *a1)
{
	__asm
	{
		mov eax, [a1]
		call EV_LoadPvmPtr
	}
}

// void __usercall(ObjectMaster *a1@<eax>)
static const void *const InitMenuStructPtr = (void*)0x432690;
static inline void InitMenuStruct(ObjectMaster *a1)
{
	__asm
	{
		mov eax, [a1]
		call InitMenuStructPtr
	}
}

// Bool __usercall@<eax>(EntityData1 *a1@<eax>)
static const void *const Camera_DoesThingWithFlagsPtr = (void*)0x434D80;
static inline Bool Camera_DoesThingWithFlags(EntityData1 *a1)
{
	Bool result;
	__asm
	{
		mov eax, [a1]
		call Camera_DoesThingWithFlagsPtr
		mov result, eax
	}
	return result;
}

// void __usercall(EntityData1 *a1@<eax>)
static const void *const Camera_Display_Ptr = (void*)0x435600;
static inline void Camera_Display_(EntityData1 *a1)
{
	__asm
	{
		mov eax, [a1]
		call Camera_Display_Ptr
	}
}

// void __usercall(EntityData1 *a1@<ebx>)
static const void *const Camera_CallFirstPersonFuncPtr = (void*)0x437A20;
static inline void Camera_CallFirstPersonFunc(EntityData1 *a1)
{
	__asm
	{
		mov ebx, [a1]
		call Camera_CallFirstPersonFuncPtr
	}
}

// void __usercall(EntityData1 *a1@<esi>)
static const void *const Ring_Collect_Maybe_AgainPtr = (void*)0x44F7A0;
static inline void Ring_Collect_Maybe_Again(EntityData1 *a1)
{
	__asm
	{
		mov esi, [a1]
		call Ring_Collect_Maybe_AgainPtr
	}
}

// signed int __usercall@<eax>(CharObj2 *a1@<edi>, EntityData1 *a2@<esi>, EntityData2 *a3@<ebx>)
static const void *const Tails_HurtPtr = (void*)0x45BF10;
static inline signed int Tails_Hurt(CharObj2 *a1, EntityData1 *a2, EntityData2 *a3)
{
	signed int result;
	__asm
	{
		mov ebx, [a3]
		mov esi, [a2]
		mov edi, [a1]
		call Tails_HurtPtr
		mov result, eax
	}
	return result;
}

// void __usercall(CharObj2 *a1@<edi>, EntityData1 *a2@<esi>, EntityData2 *a3)
static const void *const Tails_FlyMaybePtr = (void*)0x45D760;
static inline void Tails_FlyMaybe(CharObj2 *a1, EntityData1 *a2, EntityData2 *a3)
{
	__asm
	{
		push [a3]
		mov esi, [a2]
		mov edi, [a1]
		call Tails_FlyMaybePtr
		add esp, 4
	}
}

// void __usercall(CharObj2 *data2@<eax>, EntityData2 *data2_pp@<ecx>, EntityData1 *data1)
static const void *const Tails_Act1Ptr = (void*)0x45E5D0;
static inline void Tails_Act1(CharObj2 *data2, EntityData2 *data2_pp, EntityData1 *data1)
{
	__asm
	{
		push [data1]
		mov ecx, [data2_pp]
		mov eax, [data2]
		call Tails_Act1Ptr
		add esp, 4
	}
}

// bool __usercall@<eax>(Vector3 *from@<ecx>, float x, float y, float z, float range)
static const void *const ObjectInRangePtr = (void*)0x46B750;
static inline Bool ObjectInRange(Vector3 *from, float x, float y, float z, float range)
{
	Bool result;
	__asm
	{
		push [range]
		push [z]
		push [y]
		push [x]
		mov ecx, [from]
		call ObjectInRangePtr
		add esp, 16
		mov result, eax
	}
	return result;
}

// void __usercall(NJS_VECTOR *a1@<eax>, NJS_VECTOR *a2@<ecx>)
static const void *const FishingHud_DrawMeters_Ptr = (void*)0x46CEE0;
static inline void FishingHud_DrawMeters_(NJS_VECTOR *a1, NJS_VECTOR *a2)
{
	__asm
	{
		mov ecx, [a2]
		mov eax, [a1]
		call FishingHud_DrawMeters_Ptr
	}
}

// void __usercall(EntityData1 *a1@<ebx>, int a2@<esi>)
static const void *const Knuckles_Something_APtr = (void*)0x472D60;
static inline void Knuckles_Something_A(EntityData1 *a1, int a2)
{
	__asm
	{
		mov esi, [a2]
		mov ebx, [a1]
		call Knuckles_Something_APtr
	}
}

// void __usercall(EntityData1 *data1@<edi>, int a2@<esi>, int a3)
static const void *const Knuckles_Something_CPtr = (void*)0x4730D0;
static inline void Knuckles_Something_C(EntityData1 *data1, int a2, int a3)
{
	__asm
	{
		push [a3]
		mov esi, [a2]
		mov edi, [data1]
		call Knuckles_Something_CPtr
		add esp, 4
	}
}

// void __usercall(EntityData1 *a1@<edi>, int a2@<esi>, int a3)
static const void *const Knuckles_Something_DPtr = (void*)0x473350;
static inline void Knuckles_Something_D(EntityData1 *a1, int a2, int a3)
{
	__asm
	{
		push [a3]
		mov esi, [a2]
		mov edi, [a1]
		call Knuckles_Something_DPtr
		add esp, 4
	}
}

// signed int __usercall@<eax>(CharObj2 *a1@<eax>, EntityData1 *a2@<ecx>)
static const void *const Knuckles_HurtPtr = (void*)0x4735B0;
static inline signed int Knuckles_Hurt(CharObj2 *a1, EntityData1 *a2)
{
	signed int result;
	__asm
	{
		mov ecx, [a2]
		mov eax, [a1]
		call Knuckles_HurtPtr
		mov result, eax
	}
	return result;
}

// void __usercall(EntityData1 *a1@<edi>, CharObj2 *a2@<esi>)
static const void *const Character_GrabPtr = (void*)0x473B00;
static inline void Character_Grab(EntityData1 *a1, CharObj2 *a2)
{
	__asm
	{
		mov esi, [a2]
		mov edi, [a1]
		call Character_GrabPtr
	}
}

// void __usercall(int a1@<esi>, EntityData1 *a2, signed int a3)
static const void *const EmeraldRadarHud_BlueOrOffPtr = (void*)0x475C50;
static inline void EmeraldRadarHud_BlueOrOff(int a1, EntityData1 *a2, signed int a3)
{
	__asm
	{
		push [a3]
		push [a2]
		mov esi, [a1]
		call EmeraldRadarHud_BlueOrOffPtr
		add esp, 8
	}
}

// signed int __usercall@<eax>(CharObj2 *a1@<edi>, EntityData1 *a2@<esi>)
static const void *const Amy_HurtPtr = (void*)0x484F40;
static inline signed int Amy_Hurt(CharObj2 *a1, EntityData1 *a2)
{
	signed int result;
	__asm
	{
		mov esi, [a2]
		mov edi, [a1]
		call Amy_HurtPtr
		mov result, eax
	}
	return result;
}

// void __usercall(CharObj2 *a1@<edi>, EntityData1 *a2@<esi>, EntityData2 *a3)
static const void *const Sonic_HomingAttackThingPtr = (void*)0x492300;
static inline void Sonic_HomingAttackThing(CharObj2 *a1, EntityData1 *a2, EntityData2 *a3)
{
	__asm
	{
		push [a3]
		mov esi, [a2]
		mov edi, [a1]
		call Sonic_HomingAttackThingPtr
		add esp, 4
	}
}

// signed int __usercall@<eax>(CharObj2 *a1@<edi>, EntityData1 *a2@<esi>)
static const void *const Sonic_HurtPtr = (void*)0x492D70;
static inline signed int Sonic_Hurt(CharObj2 *a1, EntityData1 *a2)
{
	signed int result;
	__asm
	{
		mov esi, [a2]
		mov edi, [a1]
		call Sonic_HurtPtr
		mov result, eax
	}
	return result;
}

// bool __usercall@<eax>(EntityData1 *a1@<esi>, CharObj2 *a2@<edi>)
static const void *const Sonic_JumpCancelPtr = (void*)0x492F50;
static inline Bool Sonic_JumpCancel(EntityData1 *a1, CharObj2 *a2)
{
	Bool result;
	__asm
	{
		mov edi, [a2]
		mov esi, [a1]
		call Sonic_JumpCancelPtr
		mov result, eax
	}
	return result;
}

// bool __usercall@<eax>(CharObj2 *a1@<edi>, EntityData1 *a2@<esi>)
static const void *const Sonic_CheckLightDashPtr = (void*)0x492F90;
static inline Bool Sonic_CheckLightDash(CharObj2 *a1, EntityData1 *a2)
{
	Bool result;
	__asm
	{
		mov esi, [a2]
		mov edi, [a1]
		call Sonic_CheckLightDashPtr
		mov result, eax
	}
	return result;
}

// signed int __usercall@<eax>(CharObj2 *a1@<edi>, EntityData1 *a2@<esi>)
static const void *const Sonic_CheckPickUpObjectPtr = (void*)0x493120;
static inline signed int Sonic_CheckPickUpObject(CharObj2 *a1, EntityData1 *a2)
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

// signed int __usercall@<eax>(EntityData2 *a1@<eax>, CharObj2 *a2@<ecx>, EntityData1 *a3@<esi>)
static const void *const SuperSonic_NActPtr = (void*)0x494CD0;
static inline signed int SuperSonic_NAct(EntityData2 *a1, CharObj2 *a2, EntityData1 *a3)
{
	signed int result;
	__asm
	{
		mov esi, [a3]
		mov ecx, [a2]
		mov eax, [a1]
		call SuperSonic_NActPtr
		mov result, eax
	}
	return result;
}

// signed int __usercall@<eax>(EntityData1 *a1@<edi>, CharObj2 *a2@<esi>)
static const void *const Sonic_ReleaseSpindashPtr = (void*)0x495080;
static inline signed int Sonic_ReleaseSpindash(EntityData1 *a1, CharObj2 *a2)
{
	signed int result;
	__asm
	{
		mov esi, [a2]
		mov edi, [a1]
		call Sonic_ReleaseSpindashPtr
		mov result, eax
	}
	return result;
}

// void __usercall(CharObj2 *a1@<esi>)
static const void *const Sonic_Run1AniPtr = (void*)0x495B00;
static inline void Sonic_Run1Ani(CharObj2 *a1)
{
	__asm
	{
		mov esi, [a1]
		call Sonic_Run1AniPtr
	}
}

// void __usercall(CharObj2 *a1@<esi>)
static const void *const Sonic_SpringAniPtr = (void*)0x495B50;
static inline void Sonic_SpringAni(CharObj2 *a1)
{
	__asm
	{
		mov esi, [a1]
		call Sonic_SpringAniPtr
	}
}

// void __usercall(CharObj2 *a1@<esi>)
static const void *const Sonic_Walk3AniPtr = (void*)0x495BA0;
static inline void Sonic_Walk3Ani(CharObj2 *a1)
{
	__asm
	{
		mov esi, [a1]
		call Sonic_Walk3AniPtr
	}
}

// void __usercall(CharObj2 *a1@<esi>)
static const void *const Sonic_Run2AniPtr = (void*)0x495BE0;
static inline void Sonic_Run2Ani(CharObj2 *a1)
{
	__asm
	{
		mov esi, [a1]
		call Sonic_Run2AniPtr
	}
}

// void __usercall(CharObj2 *a1@<esi>)
static const void *const Sonic_SpinPtr = (void*)0x495C30;
static inline void Sonic_Spin(CharObj2 *a1)
{
	__asm
	{
		mov esi, [a1]
		call Sonic_SpinPtr
	}
}

// void __usercall(int a1@<esi>)
static const void *const Sonic_JumpPadAniPtr = (void*)0x495C80;
static inline void Sonic_JumpPadAni(int a1)
{
	__asm
	{
		mov esi, [a1]
		call Sonic_JumpPadAniPtr
	}
}

// signed int __usercall@<eax>(CharObj2 *a1@<eax>, EntityData1 *a2@<edi>, EntityData2 *a3)
static const void *const Sonic_NActPtr = (void*)0x495FA0;
static inline signed int Sonic_NAct(CharObj2 *a1, EntityData1 *a2, EntityData2 *a3)
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

// BOOL __usercall@<eax>(CharObj2 *Data2@<eax>, EntityData1 *Data1@<edi>)
static const void *const Sonic_ChargeSpindashPtr = (void*)0x496EE0;
static inline BOOL Sonic_ChargeSpindash(CharObj2 *Data2, EntityData1 *Data1)
{
	BOOL result;
	__asm
	{
		mov edi, [Data1]
		mov eax, [Data2]
		call Sonic_ChargeSpindashPtr
		mov result, eax
	}
	return result;
}

// void __usercall(NJS_VECTOR *pos@<edi>, NJS_SPRITE *sp@<esi>, int color, Int n, float scale)
static const void *const DrawParticleProbablyPtr = (void*)0x4A3530;
static inline void DrawParticleProbably(NJS_VECTOR *pos, NJS_SPRITE *_sp, int color, Int n, float scale)
{
	__asm
	{
		push [scale]
		push [n]
		push [color]
		mov esi, [_sp]
		mov edi, [pos]
		call DrawParticleProbablyPtr
		add esp, 12
	}
}

// signed int __usercall@<eax>(NJS_VECTOR *a1@<eax>, ObjectMaster *a2@<edi>, float a3, int a4)
static const void *const DrawParticleSomeMoreProbablyPtr = (void*)0x4A3800;
static inline signed int DrawParticleSomeMoreProbably(NJS_VECTOR *a1, ObjectMaster *a2, float a3, int a4)
{
	signed int result;
	__asm
	{
		push [a4]
		push [a3]
		mov edi, [a2]
		mov eax, [a1]
		call DrawParticleSomeMoreProbablyPtr
		add esp, 8
		mov result, eax
	}
	return result;
}

// void __usercall(ObjectData2 *a1@<eax>, EntityData1 *a2@<ecx>)
static const void *const Leon_DoSomething_APtr = (void*)0x4A8040;
static inline void Leon_DoSomething_A(ObjectData2 *a1, EntityData1 *a2)
{
	__asm
	{
		mov ecx, [a2]
		mov eax, [a1]
		call Leon_DoSomething_APtr
	}
}

// void __usercall(ObjectData2 *a1@<eax>, EntityData1 *a2@<ecx>)
static const void *const Leon_DoSomething_BPtr = (void*)0x4A8150;
static inline void Leon_DoSomething_B(ObjectData2 *a1, EntityData1 *a2)
{
	__asm
	{
		mov ecx, [a2]
		mov eax, [a1]
		call Leon_DoSomething_BPtr
	}
}

// void __usercall(ObjectData2 *a1@<eax>, EntityData1 *a2@<ecx>)
static const void *const Leon_DoSomething_CPtr = (void*)0x4A8280;
static inline void Leon_DoSomething_C(ObjectData2 *a1, EntityData1 *a2)
{
	__asm
	{
		mov ecx, [a2]
		mov eax, [a1]
		call Leon_DoSomething_CPtr
	}
}

// signed int __usercall@<eax>(EntityData1 *a1@<eax>, ObjectMaster *a2@<ebx>)
static const void *const Spinner_InitPtr = (void*)0x4B0D00;
static inline signed int Spinner_Init(EntityData1 *a1, ObjectMaster *a2)
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

// void __usercall(EntityData1 *data1@<ebx>, EntityData2 *data2)
static const void *const EPolice_DoSomethingPtr = (void*)0x4B2E90;
static inline void EPolice_DoSomething(EntityData1 *data1, EntityData2 *data2)
{
	__asm
	{
		push [data2]
		mov ebx, [data1]
		call EPolice_DoSomethingPtr
		add esp, 4
	}
}

// void __usercall(signed int a1@<eax>, float x, Float scale, int a4)
static const void *const DrawItemBoxSpritePtr = (void*)0x4C02A0;
static inline void DrawItemBoxSprite(signed int a1, float x, Float scale, int a4)
{
	__asm
	{
		push [a4]
		push [scale]
		push [x]
		mov eax, [a1]
		call DrawItemBoxSpritePtr
		add esp, 12
	}
}

// void __usercall(ObjectMaster *a1@<eax>, unsigned int n@<ecx>)
static const void *const E102_TimeRemaining_DrawPtr = (void*)0x4C5090;
static inline void E102_TimeRemaining_Draw(ObjectMaster *a1, unsigned int n)
{
	__asm
	{
		mov ecx, [n]
		mov eax, [a1]
		call E102_TimeRemaining_DrawPtr
	}
}

// void __usercall(ObjectMaster *a1@<eax>)
static const void *const RocketThingAPtr = (void*)0x4C9250;
static inline void RocketThingA(ObjectMaster *a1)
{
	__asm
	{
		mov eax, [a1]
		call RocketThingAPtr
	}
}

// void __usercall(ObjectMaster *a1@<eax>)
static const void *const RocketThingBPtr = (void*)0x4C9280;
static inline void RocketThingB(ObjectMaster *a1)
{
	__asm
	{
		mov eax, [a1]
		call RocketThingBPtr
	}
}

// void __usercall(ObjectMaster *a1@<edi>)
static const void *const RocketHThingAPtr = (void*)0x4C9780;
static inline void RocketHThingA(ObjectMaster *a1)
{
	__asm
	{
		mov edi, [a1]
		call RocketHThingAPtr
	}
}

// void __usercall(ObjectMaster *a1@<eax>)
static const void *const RocketVThingAPtr = (void*)0x4C9830;
static inline void RocketVThingA(ObjectMaster *a1)
{
	__asm
	{
		mov eax, [a1]
		call RocketVThingAPtr
	}
}

// int __usercall@<eax>(ObjectMaster *result@<eax>, NJS_OBJECT *a2@<edi>, int a3)
static const void *const YASI0_Display_2Ptr = (void*)0x500CD0;
static inline int YASI0_Display_2(ObjectMaster *obj, NJS_OBJECT *a2, int a3)
{
	int result;
	__asm
	{
		push [a3]
		mov edi, [a2]
		mov eax, [obj]
		call YASI0_Display_2Ptr
		add esp, 4
		mov result, eax
	}
	return result;
}

// void __usercall(struct_1 *a1@<esi>)
static const void *const FileSelect_LastAdventureFieldPtr = (void*)0x503E50;
static inline void FileSelect_LastAdventureField(struct_1 *a1)
{
	__asm
	{
		mov esi, [a1]
		call FileSelect_LastAdventureFieldPtr
	}
}

// void __usercall(struct_1 *a1@<esi>)
static const void *const FileSelect_StageCompletedPtr = (void*)0x503EF0;
static inline void FileSelect_StageCompleted(struct_1 *a1)
{
	__asm
	{
		mov esi, [a1]
		call FileSelect_StageCompletedPtr
	}
}

// void __usercall(struct_1 *a1@<esi>)
static const void *const FileSelect_PlayTimePtr = (void*)0x503FF0;
static inline void FileSelect_PlayTime(struct_1 *a1)
{
	__asm
	{
		mov esi, [a1]
		call FileSelect_PlayTimePtr
	}
}

// void __usercall(struct_1 *a1@<esi>)
static const void *const FileSelect_EmblemCountPtr = (void*)0x504100;
static inline void FileSelect_EmblemCount(struct_1 *a1)
{
	__asm
	{
		mov esi, [a1]
		call FileSelect_EmblemCountPtr
	}
}

// void __usercall(struct_1 *a1@<edi>)
static const void *const FileSelect_TheWholeDialogWowPtr = (void*)0x5041E0;
static inline void FileSelect_TheWholeDialogWow(struct_1 *a1)
{
	__asm
	{
		mov edi, [a1]
		call FileSelect_TheWholeDialogWowPtr
	}
}

// void __usercall(struct_1 *a1@<esi>)
static const void *const FileSelect_BackgroundForStuffBelowThisFunctionPtr = (void*)0x5045D0;
static inline void FileSelect_BackgroundForStuffBelowThisFunction(struct_1 *a1)
{
	__asm
	{
		mov esi, [a1]
		call FileSelect_BackgroundForStuffBelowThisFunctionPtr
	}
}

// void __usercall(struct_1 *result@<eax>)
static const void *const FileSelect_CharacterNamePtr = (void*)0x5047C0;
static inline void FileSelect_CharacterName(struct_1 *result)
{
	__asm
	{
		mov eax, [result]
		call FileSelect_CharacterNamePtr
	}
}

// void __usercall(struct_1 *a1@<eax>)
static const void *const FileSelect_CharacterSphereThingsPtr = (void*)0x504810;
static inline void FileSelect_CharacterSphereThings(struct_1 *a1)
{
	__asm
	{
		mov eax, [a1]
		call FileSelect_CharacterSphereThingsPtr
	}
}

// void __usercall(struct_1 *a1@<eax>)
static const void *const FileSelect_WhyIsThereAMemoryCardPtr = (void*)0x504F40;
static inline void FileSelect_WhyIsThereAMemoryCard(struct_1 *a1)
{
	__asm
	{
		mov eax, [a1]
		call FileSelect_WhyIsThereAMemoryCardPtr
	}
}

// int __usercall@<eax>(int a1@<esi>, LPCSTR filename)
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

// void __usercall(ObjectMaster *a1@<esi>)
static const void *const ODosei_LoadPtr = (void*)0x51F690;
static inline void ODosei_Load(ObjectMaster *a1)
{
	__asm
	{
		mov esi, [a1]
		call ODosei_LoadPtr
	}
}

// void __usercall(int a1@<ebp>, NJS_SPRITE *_sp, Int n, NJD_SPRITE attr)
static const void *const njDrawSprite3D_5Ptr = (void*)0x527930;
static inline void njDrawSprite3D_5(int a1, NJS_SPRITE *_sp, Int n, NJD_SPRITE attr)
{
	__asm
	{
		push [attr]
		push [n]
		push [_sp]
		mov ebp, [a1]
		call njDrawSprite3D_5Ptr
		add esp, 12
	}
}

// void __usercall(EntityData1 *a1@<esi>)
static const void *const SuperSonic_SubtractRingsPtr = (void*)0x55FDB0;
static inline void SuperSonic_SubtractRings(EntityData1 *a1)
{
	__asm
	{
		mov esi, [a1]
		call SuperSonic_SubtractRingsPtr
	}
}

// signed int __usercall@<eax>(ObjectMaster *a1@<esi>, signed int a2)
static const void *const O_SLXPtr = (void*)0x5CE6E0;
static inline signed int O_SLX(ObjectMaster *a1, signed int a2)
{
	signed int result;
	__asm
	{
		push [a2]
		mov esi, [a1]
		call O_SLXPtr
		add esp, 4
		mov result, eax
	}
	return result;
}

// void __usercall(int a1@<ebp>, NJS_SPRITE *_sp, Int n, NJD_SPRITE attr)
static const void *const njDrawSprite3D_6Ptr = (void*)0x6253F0;
static inline void njDrawSprite3D_6(int a1, NJS_SPRITE *_sp, Int n, NJD_SPRITE attr)
{
	__asm
	{
		push [attr]
		push [n]
		push [_sp]
		mov ebp, [a1]
		call njDrawSprite3D_6Ptr
		add esp, 12
	}
}

// int __usercall@<eax>(EntityData1 *a1@<esi>)
static const void *const Tornado_HurtPtr = (void*)0x627930;
static inline int Tornado_Hurt(EntityData1 *a1)
{
	int result;
	__asm
	{
		mov esi, [a1]
		call Tornado_HurtPtr
		mov result, eax
	}
	return result;
}

// void __usercall(EntityData1 *a1@<eax>)
static const void *const TornadoTargetSprite_TargetLock_LoadPtr = (void*)0x6291C0;
static inline void TornadoTargetSprite_TargetLock_Load(EntityData1 *a1)
{
	__asm
	{
		mov eax, [a1]
		call TornadoTargetSprite_TargetLock_LoadPtr
	}
}

// void __usercall(int ebx0@<ebx>, int a2@<ebp>, int esi0@<esi>, NJS_SPRITE *a1, float n, NJD_SPRITE attr, float a7)
static const void *const njDrawSprite3D_3Ptr = (void*)0x77E390;
static inline void njDrawSprite3D_3(int ebx0, int a2, int esi0, NJS_SPRITE *a1, float n, NJD_SPRITE attr, float a7)
{
	__asm
	{
		push [a7]
		push [attr]
		push [n]
		push [a1]
		mov esi, [esi0]
		mov ebp, [a2]
		mov ebx, [ebx0]
		call njDrawSprite3D_3Ptr
		add esp, 16
	}
}

// void __usercall(HINSTANCE a1@<eax>, int nCmdShow)
static const void *const CreateSADXWindowPtr = (void*)0x789E50;
static inline void CreateSADXWindow(HINSTANCE a1, int nCmdShow)
{
	__asm
	{
		push [nCmdShow]
		mov eax, [a1]
		call CreateSADXWindowPtr
		add esp, 4
	}
}

// char __usercall@<al>(void *unknown@<edi>, NJS_TEXMEMLIST *texmem)
static const void *const Direct3D_ProbablyAppliesPalettePtr = (void*)0x78CDC0;
static inline char Direct3D_ProbablyAppliesPalette(void *unknown, NJS_TEXMEMLIST *texmem)
{
	char result;
	__asm
	{
		push [texmem]
		mov edi, [unknown]
		call Direct3D_ProbablyAppliesPalettePtr
		add esp, 4
		mov result, al
	}
	return result;
}

// HRESULT __usercall@<eax>(int index@<edx>, int stage@<ecx>, int dummy_1@<edi>, int dummy_2@<esi>)
static const void *const Direct3D_SetTextureStageThingPtr = (void*)0x78D380;
static inline HRESULT Direct3D_SetTextureStageThing(int index, int stage, int dummy_1, int dummy_2)
{
	HRESULT result;
	__asm
	{
		mov esi, [dummy_2]
		mov edi, [dummy_1]
		mov ecx, [stage]
		mov edx, [index]
		call Direct3D_SetTextureStageThingPtr
		mov result, eax
	}
	return result;
}

// void __usercall(int n@<eax>)
static const void *const InitD3DLight_DirectionalPtr = (void*)0x7965E0;
static inline void InitD3DLight_Directional(int n)
{
	__asm
	{
		mov eax, [n]
		call InitD3DLight_DirectionalPtr
	}
}

// void __usercall(int n@<eax>)
static const void *const InitD3DLight_PointPtr = (void*)0x796670;
static inline void InitD3DLight_Point(int n)
{
	__asm
	{
		mov eax, [n]
		call InitD3DLight_PointPtr
	}
}

// void __usercall(Vector3 *a2@<ebx>, EntityData1 *a3@<esi>, signed int index)
static const void *const CalcRingPosLinePtr = (void*)0x79B140;
static inline void CalcRingPosLine(Vector3 *a2, EntityData1 *a3, signed int index)
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

// void __usercall(Vector3 *position@<ebx>, EntityData1 *parent@<esi>, signed int index)
static const void *const CalcRingPosCirclePtr = (void*)0x79B210;
static inline void CalcRingPosCircle(Vector3 *position, EntityData1 *parent, signed int index)
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

// void __usercall(EntityData1 *player@<eax>, EntityData2 *a2@<ecx>, EntityData1 *me@<esi>)
static const void *const RhinoTank_DoSomethingPtr = (void*)0x7A0790;
static inline void RhinoTank_DoSomething(EntityData1 *player, EntityData2 *a2, EntityData1 *me)
{
	__asm
	{
		mov esi, [me]
		mov ecx, [a2]
		mov eax, [player]
		call RhinoTank_DoSomethingPtr
	}
}

#endif /* SADXMODLOADER_SADXFUNCTIONS_H */
