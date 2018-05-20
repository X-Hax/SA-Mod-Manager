/**
 * SADX Mod Loader.
 * SADX functions.
 */

#ifndef SADXMODLOADER_SADXFUNCTIONS_H
#define SADXMODLOADER_SADXFUNCTIONS_H

#include <stdint.h>
#include "MemAccess.h"
#include "d3d8types.h"
#include "SADXStructs.h"
#include "SADXEnums.h"

typedef Uint32 NJD_SPRITE;
typedef Uint32 NJD_FLAG;
typedef Uint32 NJD_CONTROL_3D;
typedef DWORD _DWORD;
typedef Uint32 NJD_COLOR_TARGET;
typedef Uint32 NJD_COLOR_BLENDING;
typedef Uint32 NJD_TEX_SHADING;
typedef Uint32 NJD_DRAW;
typedef Uint32 NJD_TEXATTR;
typedef bool _BOOL1;

// SADX Functions
#define ObjectFunc(NAME, ADDRESS) FunctionPointer(void,NAME,(ObjectMaster *obj),ADDRESS)

FunctionPointer(void, PrintDebug, (const char *Format, ...), 0x401000);
FunctionPointer(void, CopyToGlobalSprite_ScalePosition, (NJS_SPRITE *sprite), 0x401030);
FunctionPointer(void, CopyToGlobalSprite_NativeToCurrent, (NJS_SPRITE *sprite), 0x401070);
FunctionPointer(void, njDrawTriangle2D_SomeOtherVersion, (NJS_POINT2COL *p, int count, float pri, NJD_DRAW attr), 0x4010D0);
FunctionPointer(void, Set_D3DCMPFUNC_Mode, (int a1), 0x401410);
VoidFunc(Direct3D_ResetZFunc, 0x401420);
FunctionPointer(int, DrawModel_ResetRenderFlags_, (NJS_MODEL_SADX *model), 0x401430);
FunctionPointer(void, SetCurrentLightType_Copy, (char a1), 0x401440);
VoidFunc(InitializeSomeStuff, 0x401830);
StdcallFunctionPointer(LRESULT, WndProc_B, (HWND handle, UINT Msg, WPARAM wParam, LPARAM a4), 0x401900);
ThiscallFunctionPointer(void, LoadDLLFiles, (HMODULE *_this), 0x401AC0);
VoidFunc(CheckSettings, 0x4025B0);
FunctionPointer(float, VectorMaxAbs, (NJS_VECTOR *v), 0x402E70);
FunctionPointer(void, njSetPerspective, (Angle bams), 0x402ED0);
FunctionPointer(Angle, GetHorizontalFOV_BAMS, (), 0x402F00);
FunctionPointer(void, SetGlobalPoint2Col_Colors, (Uint32 one, Uint32 two, Uint32 threefour), 0x402F10);
FunctionPointer(void, SetMaterialAndSpriteColor, (NJS_ARGB *a1), 0x402F40);
FunctionPointer(Sint32, njSetTexture, (NJS_TEXLIST *texlist), 0x403070);
FunctionPointer(void, njSetTextureNum, (Uint32 num), 0x403090);
FunctionPointer(int, SetTextureAndGetNum, (), 0x4030B0);
FunctionPointer(void, njColorBlendingMode, (NJD_COLOR_TARGET target, NJD_COLOR_BLENDING mode), 0x4030D0);
FunctionPointer(void, njDrawTextureMemList_NoSkippedFrames, (NJS_TEXTURE_VTX *points, Int count, Uint32 gbix, Int flag), 0x403130);
FunctionPointer(void, Direct3D_SetNearFarPlanes, (float min_, float max_), 0x403180);
FunctionPointer(Sint32, njReleaseTexture, (NJS_TEXLIST *texlist), 0x403290);
FunctionPointer(bool, IsVisible, (NJS_VECTOR *center, float radius), 0x403330);
FunctionPointer(void, DrawModelThing, (NJS_MODEL_SADX *a1), 0x403470);
FunctionPointer(void, ProcessModelNode_B, (NJS_OBJECT *obj, float scale), 0x4037F0);
VoidFunc(SetCurrentLightTypeTo0, 0x403BA0);
FunctionPointer(void, DrawLineList, (NJS_POINT3COL *a1, int a2, int a3), 0x403BC0);
FunctionPointer(void, njDrawTriangle2D_SomeOtherVersion_, (NJS_POINT2COL *a1, int a2, float a3, NJD_DRAW a4), 0x403C60);
FunctionPointer(void, Set3DSpriteDepth, (Float a1), 0x403D20);
FunctionPointer(void *, late_alloca, (int size), 0x403D30);
FunctionPointer(void, late_alloca_sub, (int size), 0x403D80);
FunctionPointer(Uint8 *, late_alloca_from_end, (int a1), 0x403DB0);
VoidFunc(SetQueueDrawingState_BlankScreen, 0x403E60);
FunctionPointer(void, SetSpriteColorAndZFuncThing, (QueuedModelFlagsB a1), 0x404240);
FunctionPointer(void, DrawLine3D_Queue, (NJS_POINT3COL *a1, int a2, NJD_DRAW a3, QueuedModelFlagsB a4), 0x404310);
FunctionPointer(void, Draw3DLinesMaybe_Queue, (NJS_POINT3COL *a1, int a2, NJD_DRAW attr, QueuedModelFlagsB a4), 0x4043D0);
FunctionPointer(void, Draw2DLinesMaybe_Queue, (NJS_POINT2COL *points, int count, float depth, NJD_DRAW attr, QueuedModelFlagsB flags), 0x404490);
FunctionPointer(void, DrawTriFanThing_Queue, (NJS_POINT3COL *a1, int texnum, NJD_DRAW n, QueuedModelFlagsB a4), 0x4045A0);
FunctionPointer(void, njDrawSprite2D_Queue, (NJS_SPRITE *sp, Int n, Float pri, NJD_SPRITE attr, QueuedModelFlagsB queue_flags), 0x404660);
FunctionPointer(void, njDrawSprite3D_Queue, (NJS_SPRITE *sp, Int n, NJD_SPRITE attr, QueuedModelFlagsB zfunc_type), 0x404760);
FunctionPointer(void, DrawModelCallback_Queue, (void (__cdecl *function)(void *), void *data, float depth, QueuedModelFlagsB queueflags), 0x404840);
FunctionPointer(void, DrawModel_Queue, (NJS_MODEL_SADX *a1, QueuedModelFlagsB a2), 0x4048E0);
FunctionPointer(void, WhatQueueThing, (QueuedModelNode *a1, NJS_MODEL_SADX *a2, float a3), 0x404990);
FunctionPointer(void, TranslateObject, (NJS_OBJECT *a1), 0x404CC0);
FunctionPointer(void, ScaleObject, (NJS_OBJECT *a1), 0x404D40);
FunctionPointer(void, RotateObjectXYZ, (NJS_OBJECT *a1), 0x404DC0);
FunctionPointer(void, RotateObjectZYX, (NJS_OBJECT *a1), 0x404E50);
FunctionPointer(void, TranslateObject_Simple, (NJS_OBJECT *a1), 0x404EE0);
FunctionPointer(void, RotateObjectXYZ_Simple, (NJS_OBJECT *a1), 0x404F00);
FunctionPointer(void, RotateObjectZYX_Simple, (NJS_OBJECT *a1), 0x404F20);
FunctionPointer(void, ScaleObject_Simple, (NJS_OBJECT *a1), 0x404F40);
FunctionPointer(void, ProcessAnimatedModelNode__, (NJS_OBJECT *a1), 0x4050C0);
ThiscallFunctionPointer(void, SetObjectDrawFunctions, (NJS_OBJECT *_this), 0x405260);
FunctionPointer(void, njAction_Queue, (NJS_ACTION *action, float frame, QueuedModelFlagsB flags), 0x405470);
VoidFunc(njReleaseTextureAll_, 0x406F00);
FunctionPointer(bool, CheckModelDistance_xyz, (float x, float y, float z, float a4), 0x406F70);
FunctionPointer(void, njAction, (NJS_ACTION *action, Float frame), 0x407020);
FunctionPointer(void, njAction_QueueObject, (NJS_OBJECT *a1, NJS_MOTION *a2, float a3), 0x407040);
FunctionPointer(void, njDrawSprite3D, (NJS_SPRITE *_sp, Int n, NJD_SPRITE attr), 0x407070);
FunctionPointer(void, njDrawSprite2D_4, (NJS_SPRITE *hud, Int n, Float pri, NJD_SPRITE attr), 0x4070A0);
FunctionPointer(void, njDrawSprite3D_ADV01C_C, (NJS_SPRITE *sp, int n, NJD_SPRITE attr), 0x407130);
FunctionPointer(void, DrawRect_DrawNowMaybe, (float left, float top, float right, float bottom, float depth, int color), 0x4071C0);
FunctionPointer(void, njDrawSprite3D_NoSkippedFrames, (NJS_SPRITE *a1, Int n, NJD_SPRITE attr), 0x4072F0);
VoidFunc(ResetDrawQueue, 0x407330);
FunctionPointer(void, DrawRect_Queue, (float left, float top, float right, float bottom, float depth, int color, QueuedModelFlagsB queueflags), 0x4073B0);
FunctionPointer(void, ProcessModelNode, (NJS_OBJECT *obj, QueuedModelFlagsB blend_mode, float scale), 0x4074A0);
FunctionPointer(void, DrawModel_QueueVisible, (NJS_MODEL_SADX *model, QueuedModelFlagsB blend, float scale), 0x407870);
FunctionPointer(void, DrawVisibleModel_Queue, (NJS_MODEL_SADX *model, QueuedModelFlagsB blend), 0x4078F0);
FunctionPointer(void, DrawModel, (NJS_MODEL_SADX *a1), 0x407970);
VoidFunc(late_alloca_reset, 0x407AB0);
FunctionPointer(void, ProcessModelNode_A_Wrapper, (NJS_OBJECT *a1, QueuedModelFlagsB a2, float a3), 0x407B40);
FunctionPointer(void, ProcessModelNode_A_WrapperB, (NJS_OBJECT *a1, QueuedModelFlagsB a2), 0x407B70);
FunctionPointer(void, _sub_407BB0, (NJS_MODEL_SADX *a1, QueuedModelFlagsB queueFlags), 0x407BB0);
FunctionPointer(void, njAction_Queue_407FC0, (NJS_ACTION *a1, float a2, int a3), 0x4083D0);
FunctionPointer(void, ProcessModelNode_AB_Wrapper, (NJS_OBJECT *obj, float scale), 0x4085A0);
VoidFunc(late_alloca_init, 0x408630);
FunctionPointer(void, ProcessModelNode_A_WrapperC, (NJS_OBJECT *a1, float a2), 0x408690);
VoidFunc(DrawQueuedModels, 0x4086F0);
FunctionPointer(void, ProcessModelNode_C, (NJS_OBJECT *obj, QueuedModelFlagsB blend_mode, float scale), 0x409080);
FunctionPointer(void, ProcessModelNode_D, (NJS_OBJECT *obj, int blend_mode, float scale), 0x409A20);
FunctionPointer(void, ProcessModelNode_C_VerifyTexList, (NJS_OBJECT *a1, QueuedModelFlagsB a2, float a3), 0x40A170);
FunctionPointer(void, ProcessModelNode_C_WrapperB, (NJS_OBJECT *a1, int a2), 0x40A1A0);
FunctionPointer(void, ProcessModelNode_D_Wrapper, (NJS_OBJECT *a1, int a2), 0x40A210);
FunctionPointer(void, ProcessModelNode_D_WrapperB, (NJS_OBJECT *a1, int blend_mode, float scale), 0x40A250);
VoidFunc(CorrectMaterial, 0x40A340);
FunctionPointer(void, GetTimeOfDayLevelAndAct, (int *level, int *act), 0x40A4A0);
FunctionPointer(void, StageLightEnableDirection, (int i, char a2), 0x40A510);
FunctionPointer(void, StageLightDisableDirection, (int i), 0x40A550);
FunctionPointer(void, SetStageLightSpecularAndAmbient, (int i, float specular, float multiplier, float ambient), 0x40A580);
FunctionPointer(void, SetStageLightDiffuse, (int i, float diffuse_r, float diffuse_g, float diffuse_b), 0x40A5D0);
FunctionPointer(void, SetStageLightDirection, (int a1, float dir_x, float dir_y, float dir_z), 0x40A600);
VoidFunc(ApplyLSPalette, 0x40A630);
FunctionPointer(Sint32, InitDefaultStageLight, (), 0x40A8A0);
FunctionPointer(StageLightData *, GetStageLight, (int level, int act, int light_num), 0x40A8F0);
FunctionPointer(void, SetCurrentStageLights, (int level, int act), 0x40A950);
FunctionPointer(void, SetCurrentLSPalette, (int level, int act, int type), 0x40AAB0);
FunctionPointer(void, GetAndSetCurrentLSPalette, (unsigned __int16 level, unsigned __int16 act), 0x40AB60);
FunctionPointer(void, DoCharacterLightStuff_A, (int level, int act), 0x40AD10);
FunctionPointer(void, DoCharacterLightStuff_B, (int level, int act), 0x40AD60);
FunctionPointer(Sint32, ObjectIsSiblingOfChildren, (ObjectMaster *object, ObjectMaster *child), 0x40AF70);
FunctionPointer(Sint32, NaiZoGola, (ObjectMaster *object), 0x40AFB0);
FunctionPointer(void, RunObjectIndex, (int index), 0x40B0C0);
ObjectFunc(DisplayObject, 0x40B130);
VoidFunc(RunAllObjects, 0x40B170);
FunctionPointer(int, InitHeap, (), 0x40B1A0);
FunctionPointer(void *, AllocateMemory, (int size), 0x40B220);
FunctionPointer(void *, AllocateArray, (int count, int size), 0x40B2D0);
FunctionPointer(void, FreeMemory, (void *data), 0x40B310);
ObjectFunc(RunObjectChildren, 0x40B420);
VoidFunc(InitMasterObjectArray, 0x40B460);
FunctionPointer(void, DisplayObjectIndex, (int index), 0x40B4F0);
VoidFunc(DisplayAllObjects, 0x40B540);
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
FunctionPointer(int, DoSomethingRelatedToText_, (const char *a2), 0x40BD30);
VoidFunc(InitVarious, 0x40BF40);
VoidFunc(InitGameLoop, 0x40C030);
VoidFunc(GameModeHandler, 0x40C090);
VoidFunc(GameLoop, 0x40CAD0);
FunctionPointer(void, PlayMusicFile, (LPCSTR filename, int loop), 0x40CD20);
FunctionPointer(int, PlayVoiceFile, (LPCSTR filename), 0x40CE30);
FunctionPointer(int, WMPRestartMusic, (), 0x40CF50);
FunctionPointer(void, WMPClose, (int a1), 0x40CFF0);
VoidFunc(PauseSound, 0x40D060);
VoidFunc(ResumeSound, 0x40D0A0);
FunctionPointer(int, WMPInit, (), 0x40D0D0);
FunctionPointer(int, WMPRelease, (), 0x40D1F0);
VoidFunc(DrawSubtitles, 0x40D4D0);
FunctionPointer(void, SetMouseMode, (Bool a1), 0x40E8A0);
FunctionPointer(void, SetCustomMouseButton, (unsigned __int16 index, int mouseButton), 0x40E8B0);
VoidFunc(MouseUpdate, 0x40E900);
VoidFunc(CursorTriangle_Draw, 0x40EAB0);
VoidFunc(KeyboardUpdate, 0x40EE10);
FunctionPointer(int, KeyboardKeyPressed, (int key), 0x40EF10);
FunctionPointer(int, GetKey, (int index), 0x40EF20);
VoidFunc(EnableControl, 0x40EF40);
VoidFunc(DisableControl, 0x40EF50);
FunctionPointer(Bool, IsControlEnabled, (), 0x40EF60);
FunctionPointer(void, EnableController, (Uint8 index), 0x40EF70);
FunctionPointer(void, DisableController, (Uint8 index), 0x40EFA0);
FunctionPointer(Bool, IsControllerEnabled, (Uint8 index), 0x40EFD0);
FunctionPointer(void, UpdateMenuInput, (int index), 0x40F070);
VoidFunc(WriteAnalogs, 0x40F170);
VoidFunc(InputThing__Ctor, 0x40F350);
VoidFunc(InitRawControllers, 0x40F3A0);
VoidFunc(PollInput, 0x40F460);
VoidFunc(Control, 0x40FDC0);
FunctionPointer(void, Enable3DSound, (signed int a1), 0x40FF20);
FunctionPointer(Bool, Get3DSoundEnabled, (), 0x40FF40);
StdcallFunctionPointer(DWORD, AudioThread, (LPVOID lpThreadParameter), 0x40FF50);
FunctionPointer(signed int, StartAudioThread, (), 0x410510);
FunctionPointer(signed int, StopAudioThread, (), 0x410630);
VoidFunc(ItDisablesFog, 0x4119B0);
FunctionPointer(int, UpdateFogTable, (), 0x411A20);
VoidFunc(ToggleStageFog, 0x411AF0);
VoidFunc(DisableFog, 0x411B40);
FunctionPointer(void, SetFrameRateMode, (int mode, int frameIncrement), 0x411E30);
FunctionPointer(void, ScaleVectorThing_Scale, (float scale), 0x411E90);
VoidFunc(ScaleVectorThing_RestoreB, 0x411EF0);
VoidFunc(ScaleVectorThing_Restore, 0x411F30);
FunctionPointer(void, SetCurrentLightType, (int a1), 0x412160);
FunctionPointer(void, SetCurrentLightType_Div2, (signed int a1), 0x412400);
FunctionPointer(void, Direct3D_PerformLighting, (int type_level), 0x412420);
FunctionPointer(void, SetScaleVectorThingButOnlyTheEggCarrier, (NJS_VECTOR *a1, float rmaybe, float gmaybe, float bmaybe, float idka, float idkb, float idkc), 0x412740);
FunctionPointer(void, SetMaterialAndSpriteColor_Float, (float a, float r, float g, float b), 0x4128A0);
VoidFunc(SetMaterialAndSpriteColor_Temp, 0x4128E0);
VoidFunc(ClampGlobalColorThing_Thing, 0x4128F0);
FunctionPointer(void, DoThatThingWhereYouGetCloseAndItLightsUp, (EntityData1 *a1, unsigned __int16 a2), 0x412A20);
FunctionPointer(void, SetTimeOfDay, (Sint8 time), 0x412C00);
FunctionPointer(Sint8, GetTimeOfDay, (), 0x412C10);
FunctionPointer(int, GetCharacterUnlockedAdventure, (Characters character), 0x412C30);
FunctionPointer(void, UnlockCharacterAdventure, (int character), 0x412CA0);
FunctionPointer(void, SetEventFlag, (EventFlags a1), 0x412D00);
FunctionPointer(void, ClearEventFlag, (EventFlags a1), 0x412D10);
FunctionPointer(bool, GetEventFlag, (EventFlags offset), 0x412D20);
FunctionPointer(void, SetLevelClear, (Uint32 level), 0x412E20);
FunctionPointer(bool, IsEggCarrierSunk, (), 0x412E70);
VoidFunc(LoadFieldNPCs, 0x412E90);
VoidFunc(InitFlagsAndThings, 0x413300);
ObjectFunc(CharacterBossSetup_Main, 0x413790);
FunctionPointer(void, LoadCharacterBoss, (int boss_id), 0x413840);
VoidFunc(DoSomeMissedFrameThing, 0x413920);
VoidFunc(EnablePause, 0x413980);
VoidFunc(DisablePause, 0x413990);
FunctionPointer(int, SomethingAboutSandHill, (), 0x413BC0);
FunctionPointer(void, StartLevelCutscene, (__int16 a1), 0x413C90);
FunctionPointer(__int16, RunSceneLogic, (), 0x413D40);
VoidFunc(RunLogic, 0x413DD0);
FunctionPointer(__int16, DrawHudAndRunLogic, (), 0x413F00);
ObjectFunc(DelayedSound_Main, 0x414120);
FunctionPointer(ObjectMaster *, Load_DelayedSound_SFX, (int index), 0x414170);
FunctionPointer(ObjectMaster *, Load_DelayedSound_BGM, (int index), 0x4141B0);
VoidFunc(LoadLevelObject, 0x4143C0);
VoidFunc(LoadSkyboxObject, 0x414420);
FunctionPointer(int, SetCharacter, (__int16 character), 0x4144D0);
FunctionPointer(Characters, GetCurrentCharacterID, (), 0x4144E0);
FunctionPointer(int, get_word_3B22DE8, (), 0x4144F0);
VoidFunc(SetStartPos_ReturnToField, 0x414500);
FunctionPointer(void, SetLevelAndAct, (Uint8 level, Uint8 act), 0x414570);
FunctionPointer(void, SetNextLevelAndAct, (Uint8 level, Uint8 act), 0x4145B0);
VoidFunc(GoToNextLevel, 0x414610);
FunctionPointer(int, GetLevelAndAct, (), 0x414650);
FunctionPointer(int, GetLastLevelAndAct, (), 0x414670);
FunctionPointer(int, GetNextLevelAndAct, (), 0x414690);
FunctionPointer(bool, IsLevelChaoGarden, (), 0x4146B0);
FunctionPointer(void, IncrementAct, (int amount), 0x4146E0);
FunctionPointer(int, SetLevelEntrance, (char num), 0x4147F0);
FunctionPointer(char, GetLevelEntranceID, (), 0x414800);
FunctionPointer(void, MovePlayerToStartPoint, (EntityData1 *data1), 0x414810);
FunctionPointer(void, SetNextLevel, (unsigned __int16 level), 0x414B00);
VoidFunc(SetNextLevelEx, 0x414C70);
ThiscallFunctionPointer(signed int, SetNextBossLevel, (void *), 0x414C90);
FunctionPointer(bool, IsGamePaused, (), 0x414D70);
FunctionPointer(bool, IsIngame, (), 0x414D90);
FunctionPointer(__int16, DisplayPauseMenu, (), 0x415420);
VoidFunc(LoadLevelResults, 0x415540);
VoidFunc(LoadCharacter, 0x4157C0);
VoidFunc(LoadLevel, 0x4159A0);
FunctionPointer(__int16, GameStateHandler_Mission, (), 0x415AF0);
FunctionPointer(__int16, GameStateHandler_Trial, (), 0x416610);
FunctionPointer(__int16, GameStateHandler_Adventure, (), 0x4173B0);
FunctionPointer(void, MaybeDoVertexWeldingOrSomething, (EntityData1 *data1, int light), 0x417FB0);
FunctionPointer(void, FreeWhateverField3CIs, (EntityData1 *a1), 0x4187A0);
FunctionPointer(float, njUnitVector, (NJS_VECTOR *a1), 0x418AC0);
FunctionPointer(float, VectorDeltaSquared, (NJS_VECTOR *a, NJS_VECTOR *b), 0x418B30);
FunctionPointer(void, Collision_InitThings, (EntityData1 *entity), 0x418B60);
FunctionPointer(void, CheckCollision_, (EntityData1 *a1, EntityData1 *a2), 0x41B8B0);
VoidFunc(ResetObjectCollisionCounts, 0x41B970);
FunctionPointer(void, Collision_CalculateRadius, (EntityData1 *entity), 0x41BAC0);
FunctionPointer(void, AddToCollisionList, (EntityData1 *entity), 0x41C280);
FunctionPointer(void, FreeEntityCollision, (EntityData1 *a1), 0x41C4E0);
FunctionPointer(void, SetCollisionInfoRadius, (CollisionInfo *info, float radius), 0x41C530);
VoidFunc(ResetHomingAttackTargetsAndThings, 0x41C680);
ObjectFunc(Collision_Free, 0x41CAA0);
FunctionPointer(void, Collision_Init, (ObjectMaster *obj, CollisionData *collisionArray, int count, unsigned __int8 list), 0x41CAF0);
FunctionPointer(EntityData1 *, GetCollidingEntityA, (EntityData1 *a1), 0x41CBC0);
FunctionPointer(EntityData1 *, GetCollidingEntityB, (EntityData1 *a1), 0x41CBE0);
FunctionPointer(void, CheckCollision, (EntityData1 *a1, EntityData1 *a2), 0x41FC30);
FunctionPointer(void, CheckCollision__, (EntityData1 *a1, EntityData1 *a2), 0x41FEC0);
VoidFunc(RunPlayerCollision, 0x420010);
VoidFunc(RunCollision_1, 0x420210);
VoidFunc(RunCollision_9, 0x4203C0);
VoidFunc(RunCollision_3, 0x420560);
VoidFunc(RunCollision_4, 0x420640);
VoidFunc(RunCollision_5, 0x4206A0);
VoidFunc(RunObjectCollision, 0x420700);
FunctionPointer(Bool, get_byte_3B280A8, (), 0x420820);
VoidFunc(set_byte_3B280A8_1, 0x420830);
VoidFunc(set_byte_3B280A8_0, 0x420840);
VoidFunc(SetupSomeScreenStuff, 0x4208E0);
FunctionPointer(void, ABC_TXT_load, (ABC_TXT_struct *structure), 0x420A00);
FunctionPointer(void, ABC_TXT_release, (ABC_TXT_struct *a1), 0x420A30);
FunctionPointer(void, ABC_TXT_copy, (ABC_TXT_struct *a1), 0x420A50);
FunctionPointer(void, DrawStringCharacter, (char _char, float x, float y, float scale), 0x420AA0);
FunctionPointer(void, DrawString, (const char *text, float x, float y, float scale), 0x420C50);
FunctionPointer(void, LoadNoNamePVM, (NJS_TEXLIST *a1), 0x420D80);
FunctionPointer(void, UnloadCharTextures, (int charID), 0x420E90);
VoidFunc(FreeCharacterTexlists, 0x420EF0);
VoidFunc(FreeRegObjTexlists, 0x420F40);
FunctionPointer(Sint32, SetTextureToCommon, (), 0x420F90);
FunctionPointer(bool, LevelObjTexlistLoaded, (), 0x420FB0);
FunctionPointer(Sint32, SetTextureToLevelObj, (), 0x420FC0);
FunctionPointer(int, UnloadLevelTextures, (__int16 levelact), 0x421040);
FunctionPointer(int, LoadSystemPVM, (const char *filename, NJS_TEXLIST *texList), 0x4210A0);
FunctionPointer(void, LoadPVM, (const char *filename, NJS_TEXLIST *texlist), 0x421180);
FunctionPointer(Sint32, LoadPvmMEM2, (const char *filename, NJS_TEXLIST *texlist), 0x421260);
FunctionPointer(void, LoadCharTextures, (int charID), 0x421280);
FunctionPointer(void, LoadRegObjTextures, (int a1), 0x4212E0);
VoidFunc(LoadLevelObjTextures, 0x4213A0);
FunctionPointer(void, LoadLevelTextures, (__int16 level), 0x4215B0);
FunctionPointer(void, ShowNoMemoryCardText, (void *a1), 0x421C20);
FunctionPointer(int, ShowNoMemoryCardText_0, (int), 0x421C70);
FunctionPointer(int, ShowNoMemoryCardText_1, (int), 0x421CD0);
VoidFunc(WriteSaveFile, 0x421FD0);
VoidFunc(j_WriteSaveFile, 0x4221D0);
FunctionPointer(void, LoadFile, (const char *name, LPVOID lpBuffer), 0x422200);
FunctionPointer(int, LoadFileWithMalloc, (const char *name, LPVOID *data), 0x422310);
VoidFunc(ReleaseSetFile, 0x422440);
FunctionPointer(const char *, GetCharIDString, (), 0x422490);
VoidFunc(ReleaseCamFile, 0x4224B0);
FunctionPointer(signed int, njLoadTexture_Wrapper, (NJS_TEXLIST *texlist), 0x4228E0);
FunctionPointer(void, RunLevelDestructor, (int a1), 0x4228F0);
VoidFunc(LoadLevelFiles, 0x422AD0);
FunctionPointer(void, LoadSoundList, (signed int soundlist), 0x4238E0);
FunctionPointer(int, PlaySound, (int ID, void *a2, int a3, void *a4), 0x423D70);
FunctionPointer(signed int, PlaySound2, (int ID, void *ptr, int a3, void *a4), 0x423E20);
FunctionPointer(Sint32, QueueSound_DualEntity, (int id, void *ptra, int a3, void *ptrb, int a5), 0x423F50);
FunctionPointer(Sint32, QueueSound_XYZ, (int id, EntityData1 *entity, int a3, int a4, int a5, float x, float y, float z), 0x424000);
FunctionPointer(signed int, SomeOtherSoundThing, (int a1, EntityData1 *a2, int field_0, int field_1C, int field_4, float x, float y, float z), 0x424100);
FunctionPointer(int, DoSoundQueueThing, (int num), 0x424210);
FunctionPointer(int, LoadCharVoices, (), 0x4245F0);
VoidFunc(LoadCharBossSounds, 0x424760);
FunctionPointer(void, QueueSound, (int id, EntityData1 *entity, int a3, int a4, int a5, float multiplier, EntityData1 *origin_entity), 0x4249E0);
FunctionPointer(int, InitSoundQueue, (), 0x424C20);
FunctionPointer(int, LoadLevelSounds, (), 0x424C80);
VoidFunc(PlayQueuedSounds, 0x4250D0);
FunctionPointer(int, j_WMPInit, (), 0x4253C0);
ObjectFunc(SoundManager_Main, 0x4253D0);
ObjectFunc(SoundManager_Delete, 0x425620);
FunctionPointer(int, SoundManager_Delete2, (), 0x425670);
FunctionPointer(void, PlayMusic, (MusicIDs song), 0x425690);
VoidFunc(StopMusic, 0x4256B0);
FunctionPointer(int, RestoreLastSong, (), 0x4256E0);
FunctionPointer(void, PlayVoice, (int a1), 0x425710);
VoidFunc(StopVoices, 0x425730);
ObjectFunc(JingleMain, 0x425760);
VoidFunc(ResetCurrentVoice, 0x4257E0);
ObjectFunc(SoundManager_Load, 0x425830);
FunctionPointer(void, PlayJingle, (int a1), 0x425860);
FunctionPointer(int, InitializeSoundManager, (), 0x4258B0);
FunctionPointer(bool, GetDebugMode, (), 0x4258F0);
FunctionPointer(void, njDrawSprite2D_ForcePriority, (NJS_SPRITE *sp, Int n, Float pri, NJD_SPRITE attr), 0x425910);
VoidFunc(Set0Rings, 0x425AB0);
FunctionPointer(int, GetMarketRings, (), 0x425AC0);
FunctionPointer(int, ResetLives, (), 0x425AF0);
FunctionPointer(void, GiveLives, (__int16 lives), 0x425B60);
FunctionPointer(void, AddRings, (Sint16 amount), 0x425BE0);
FunctionPointer(Sint16, GetRings, (), 0x425CC0);
VoidFunc(HudDisplayRingTimeLife, 0x425CD0);
VoidFunc(HudDisplayRingTimeLife_Check, 0x425F90);
FunctionPointer(int, GetLives, (), 0x425FE0);
VoidFunc(ResetTime, 0x425FF0);
FunctionPointer(int, DisableTimeThing, (), 0x426040);
VoidFunc(IncrementFrameCounter, 0x426050);
VoidFunc(TickClock, 0x426070);
FunctionPointer(int, GetFrameCounter, (), 0x4261B0);
FunctionPointer(void, HudShowTimer, (float a1, float a2), 0x4261C0);
FunctionPointer(void, GetTime, (char *minutes, char *seconds), 0x4265B0);
FunctionPointer(void, SetTime, (char minutes, char seconds), 0x4265D0);
FunctionPointer(void, SetTime2, (char minute, char second, char frame), 0x4265F0);
FunctionPointer(void, GetTime2, (char *minutes, char *seconds, char *frames), 0x426610);
FunctionPointer(void, AddSeconds, (int seconds), 0x426640);
FunctionPointer(bool, IsTimeZero, (), 0x426690);
FunctionPointer(float, GetTimeBonus, (), 0x4266C0);
FunctionPointer(int, DrawShapeMaybe, (float, float, float, float), 0x426740);
FunctionPointer(int, CheckMissionRequirements, (), 0x426AA0);
FunctionPointer(int, DrawScoreAct, (), 0x427430);
VoidFunc(ResetScoreAndBonuses, 0x427EF0);
VoidFunc(ResetTime2, 0x427F10);
VoidFunc(HudDisplayScoreOrTimer, 0x427F50);
ObjectFunc(StageResult_Main, 0x42A680);
ObjectFunc(ScoreDisplay_Main, 0x42BCC0);
ObjectFunc(j_ScoreDisplay_Main, 0x42BEE0);
VoidFunc(ScreenFade_Draw, 0x42BEF0);
VoidFunc(ScreenFade_Start, 0x42BF70);
VoidFunc(ScreenFade_Start_, 0x42BF90);
FunctionPointer(signed __int16, GameOver_Init, (int a1), 0x42C350);
FunctionPointer(void, ScreenFade_SetColor, (NJS_COLOR *a1), 0x42C4E0);
VoidFunc(ScreenFade_DrawColor, 0x42C4F0);
FunctionPointer(Sint16, ScreenFade_Run, (), 0x42C570);
VoidFunc(ScreenFade_Start_CopyAlpha, 0x42C630);
FunctionPointer(Sint16, ScreenFade_RunActive, (), 0x42C650);
VoidFunc(Load_SEGALOGO_E, 0x42C730);
VoidFunc(InitializeSegaLogo, 0x42C7B0);
FunctionPointer(void, DisplayLogoScreen, (unsigned __int8 textureStart), 0x42CB20);
FunctionPointer(Sint32, RunSegaSonicTeamLogos, (), 0x42CC70);
FunctionPointer(int, GetCharacterSelection, (), 0x42CDE0);
FunctionPointer(ObjectMaster *, j_LoadCharBossAI, (CharBossData *a1), 0x42CE30);
FunctionPointer(ObjectMaster *, j_LoadCharBoss, (CharBossData *a1), 0x42CE40);
FunctionPointer(int, j_LoadTitleCardTexture, (int minDispTime), 0x42CE80);
FunctionPointer(Sint32, j_DisplayTitleCard, (), 0x42CE90);
FunctionPointer(bool, CheckSaveFile, (SaveFileData *saveFile), 0x42D0B0);
ThiscallFunctionPointer(void, WriteAdventureDataToSaveFile, (void *data), 0x42D200);
VoidFunc(InitSaveData, 0x42D3B0);
VoidFunc(SaveSave, 0x42D630);
VoidFunc(LoadSave, 0x42D770);
FunctionPointer(void, SetTGSCharSelThing, (int a1), 0x42E470);
FunctionPointer(int, VideoMain, (int n), 0x42F020);
FunctionPointer(void, DisplayDialogueText, (const char *text), 0x42FB20);
FunctionPointer(ObjectMaster *, j_GetCharacterObject, (unsigned __int8 a1), 0x42FC40);
FunctionPointer(void, LoadEventCharacter, (unsigned __int8 charID, void (__cdecl *mainsub)(ObjectMaster *), float xpos, float ypos, float zpos, int xrot, int yrot, int zrot), 0x42FD80);
VoidFunc(j_StopVoices, 0x431110);
FunctionPointer(void, EV_MainThread, (void (__cdecl *func)(_DWORD)), 0x431180);
FunctionPointer(void, EV_CanselThread, (void (__cdecl *func)(int)), 0x431290);
FunctionPointer(void, LoadEventObject, (ObjectMaster **a1, ObjectMaster *(*func)(void), float x, float y, float z, int rx, int ry, int rz), 0x4316C0);
FunctionPointer(void, DrawBG_ava_csr_a, (unsigned __int8 a1, float x, float y, float z, float sx, float sy), 0x431B10);
FunctionPointer(void, DrawBG_ava_square_a, (unsigned __int8 n, float x, float y, float z, float scaleX, float scaleY), 0x431BB0);
FunctionPointer(void, DrawBG_ava_csr_b, (unsigned int n, float x, float y, float z, float scaleX, float scaleY), 0x431C30);
FunctionPointer(void, DrawBG_ava_square_b, (unsigned int n, float x, float y, float z, float sx, float sy), 0x431CB0);
ObjectFunc(MenuObj_Display, 0x432480);
VoidFunc(RunCurrentMenuObj_ZfuncThing, 0x4326F0);
ObjectFunc(MenuObj_Main, 0x432720);
FunctionPointer(int, LoadMenuObject, (), 0x432C60);
VoidFunc(RunCurrentMenuObj_KillDisplaySub, 0x432DD0);
FunctionPointer(Uint32, BlendColors, (Uint32 a1, Uint32 a2), 0x432DF0);
FunctionPointer(void, SetVtxColorB, (Uint32 a1), 0x432F40);
FunctionPointer(void, DoColorGradientThingMaybe, (unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4), 0x432F90);
FunctionPointer(void, SetVtxColorA, (Uint32 a1), 0x433010);
VoidFunc(SetDefaultAlphaBlend, 0x433170);
FunctionPointer(void, GetTextureDimensions, (NJS_TEXLIST *texlist, int index, int *width, int *height), 0x4332B0);
FunctionPointer(Uint32, GetGlobalIndex, (NJS_TEXLIST *a1, int texIndex), 0x4332F0);
FunctionPointer(void, GreenMenuRect_Draw, (float x, float y, float z, float width, float height), 0x4334F0);
FunctionPointer(void, DisplayScreenTexturePoints, (int texnum, float x, float y, float z, float centerX, float centerY, NJS_TEXTURE_VTX *points), 0x4335A0);
FunctionPointer(void, DisplayScreenTexture, (int texid, float x, float y, float z), 0x4338D0);
FunctionPointer(void, DisplayScreenTextureCenter, (int texnum, float x, float y, float z, float centerX, float centerY), 0x433C90);
FunctionPointer(void, DrawBG, (int texnum, float x, float y, float z, float scaleX, float scaleY), 0x433F80);
FunctionPointer(int, GetOtherLastHorizontalFOV_BAMS, (), 0x4345F0);
VoidFunc(Camera_BasicallyCtor, 0x434600);
ObjectFunc(Camera_Delete, 0x434670);
FunctionPointer(int, camerahax_adventurefields, (), 0x434870);
FunctionPointer(int, camerahax_b, (), 0x434880);
FunctionPointer(void, SetCameraControlEnabled, (Bool a1), 0x4348A0);
FunctionPointer(Bool, IsCameraControlEnabled, (), 0x4348E0);
FunctionPointer(void, SetCameraMode_, (int a1), 0x4348F0);
FunctionPointer(Uint32, GetCameraMode_, (), 0x434930);
FunctionPointer(void, SetCameraMode, (char a1), 0x436560);
FunctionPointer(int, GetCameraMode, (), 0x436590);
FunctionPointer(int, Get_camera_thing_b, (), 0x4365A0);
FunctionPointer(int, Set_camera_thing_b, (int), 0x4365B0);
VoidFunc(Camera_SetSomeOtherTarget, 0x436690);
VoidFunc(SetCameraPositionDelta, 0x436700);
FunctionPointer(int, Camera_DisplayB_, (), 0x436CD0);
ObjectFunc(Camera_DisplayB, 0x4370F0);
FunctionPointer(int, DoSomethingWithFOV, (int), 0x437240);
VoidFunc(Camera_LookAtTargetMaybe, 0x4372A0);
ObjectFunc(Camera_Display, 0x4373D0);
FunctionPointer(signed int, njCameraAction, (NJS_CACTION *a1, float a2), 0x437E40);
ObjectFunc(Camera_Main, 0x438090);
VoidFunc(LoadCamera, 0x438260);
FunctionPointer(int, BAMS_SubWrap, (__int16 bams_a, unsigned __int16 bams_b, int limit), 0x438350);
FunctionPointer(int, BAMS_Subtract, (int a1, int a2), 0x4383B0);
FunctionPointer(void, MorphPoints_, (NJS_POINT3 *points_a, NJS_POINT3 *points_b, NJS_POINT3 *destination, float a4, signed int nbPoint), 0x4387B0);
FunctionPointer(void, CalculateVertexNormals, (NJS_MODEL_SADX *a1), 0x4390F0);
VoidFunc(BackupConstantAttr, 0x439520);
VoidFunc(RestoreConstantAttr, 0x439540);
FunctionPointer(void, AddConstantAttr, (NJD_FLAG _and, NJD_FLAG _or), 0x439560);
FunctionPointer(void, RemoveConstantAttr, (NJD_FLAG _and, NJD_FLAG _or), 0x439590);
VoidFunc(njControl3D_Backup, 0x4395C0);
VoidFunc(njControl3D_Restore, 0x4395D0);
FunctionPointer(void, njControl3D_Add, (NJD_CONTROL_3D a1), 0x4395E0);
FunctionPointer(void, njControl3D_Remove, (NJD_CONTROL_3D a1), 0x439600);
FunctionPointer(void, SetMatrixInGiganticMatrixArrayIndex, (NJS_MATRIX *a3, int index), 0x439B00);
FunctionPointer(void, InitSprite, (NJS_SPRITE *a1, NJS_TEXLIST *a2, NJS_TEXANIM *a3), 0x439B20);
FunctionPointer(float, SkyDeck_IncrementFloat, (float a1, float a2, float a3), 0x439ED0);
FunctionPointer(void, MorphPoints, (NJS_MODEL_SADX *a, NJS_MODEL_SADX *b, NJS_MODEL_SADX *destination, float factor), 0x439F20);
FunctionPointer(void, InitLandTable, (int level, int act), 0x43A260);
FunctionPointer(void, FreeLandTable, (int level, int act), 0x43A350);
FunctionPointer(void, ResetLandTablePointer, (int level, int act), 0x43A4A0);
FunctionPointer(void, SetChaoLandTable, (LandTable *geo), 0x43A4C0);
ObjectFunc(LandTableObj_Delete, 0x43A500);
VoidFunc(DrawLandTable, 0x43A6A0);
VoidFunc(UpdateLandTableAnimationMaybe, 0x43A8A0);
VoidFunc(ListGroundForDrawing, 0x43A900);
FunctionPointer(void, DynamicCOL_Add, (ColFlags flags, ObjectMaster *entity, NJS_OBJECT *model), 0x43B2E0);
FunctionPointer(void, DynamicCOL_Remove, (ObjectMaster *entity, NJS_OBJECT *model), 0x43B380);
FunctionPointer(NJS_OBJECT *, ObjectArray_GetFreeObject, (), 0x43B400);
FunctionPointer(int, ObjectArray_Remove, (NJS_OBJECT *a1), 0x43B450);
FunctionPointer(int, ResetGravity, (), 0x43B490);
FunctionPointer(void, SetGravityMaybe, (int a1, int a2), 0x43B4C0);
FunctionPointer(void, LandTable_MinimumRadius_Set, (float a1), 0x43B6E0);
VoidFunc(LandTable_MinimumRadius_Reset, 0x43B6F0);
VoidFunc(SetCurrentLandTable, 0x43B700);
ObjectFunc(LandTableObj_MainB, 0x43B780);
ObjectFunc(LandTableObj_Display, 0x43B830);
ObjectFunc(LandTableObj_Main, 0x43B8B0);
VoidFunc(ClearPlayerArrays, 0x43B920);
FunctionPointer(void, SetSomeCollisionVector, (EntityData1 *a2, EntityData2 *a3, CharObj2 *a4), 0x43BB60);
FunctionPointer(bool, GetAnalog, (EntityData1 *data1, Angle *angle, float *magnitude), 0x43BDC0);
FunctionPointer(Bool, DoJumpThing, (EntityData1 *a1, CharObj2 *a2), 0x43BF90);
FunctionPointer(bool, HaveTarget, (char charid), 0x43C110);
FunctionPointer(void, NullifyVelocity, (EntityData2 *a1, CharObj2 *a2), 0x43C550);
FunctionPointer(void, SetUpDirectionAndThings, (EntityData1 *a1, EntityData2 *a2, CharObj2 *a3), 0x43EE70);
FunctionPointer(void, ProcessVertexWelds, (EntityData1 *a1, EntityData2 *a2, CharObj2 *a3), 0x43FA90);
FunctionPointer(void, SpawnRipples, (unsigned __int8 a1, NJS_VECTOR *a2), 0x4407C0);
FunctionPointer(int, SpawnSplashParticles, (EntityData1 *a1), 0x440890);
ObjectFunc(j_RestoreLastSong, 0x440E10);
FunctionPointer(void, ForcePlayerAction, (unsigned __int8 playerNum, char action), 0x441260);
FunctionPointer(void, EnemyBounceThing, (unsigned __int8 playerID, float speedX, float speedY, float speedZ), 0x441280);
FunctionPointer(void, EnemyBounceThing_Wrapper, (unsigned __int8 playerID, float a2, float a3, float a4), 0x441370);
FunctionPointer(void, dothedash, (unsigned __int8 playerID, NJS_VECTOR *_speed, Rotation3 *angle, __int16 disableTime), 0x441490);
FunctionPointer(void, EntityData2_SetSomeCollisionVector, (unsigned __int8 a1, float x, float y, float z), 0x441750);
FunctionPointer(void, PositionPlayer, (Uint8 charIndex, float x, float y, float z), 0x441780);
FunctionPointer(int, IsPlayerInsideSphere, (NJS_VECTOR *x_1, float radius), 0x441840);
FunctionPointer(int, GetCharacterID, (char index), 0x441970);
FunctionPointer(ObjectMaster *, GetCharacterObject, (unsigned __int8 character), 0x441AC0);
FunctionPointer(CharObj2 *, GetCharObj2, (char index), 0x441B00);
FunctionPointer(int, GetPlayerCount, (), 0x441B10);
FunctionPointer(Sint32, GetTargetableCharacter, (unsigned __int8 pnum), 0x441BF0);
FunctionPointer(void, BackupPlayerSubs, (unsigned __int8 id), 0x441C70);
FunctionPointer(void, RestorePlayerSubs, (unsigned __int8 id), 0x441CA0);
FunctionPointer(void, GiveMagneticBarrier, (char character), 0x441E30);
FunctionPointer(void, GiveBarrier, (char character), 0x441EA0);
FunctionPointer(void, GiveInvincibility, (int character), 0x441F10);
FunctionPointer(void, SetHeldObject, (unsigned __int8 a1, ObjectMaster *a2), 0x4420A0);
FunctionPointer(void, InitCharacterVars, (int ID, ObjectMaster *character), 0x442750);
FunctionPointer(void, SomethingAboutWaterButNotReally, (EntityData1 *entity1, EntityData2 *entity2, CharObj2 *charobj2), 0x445D10);
FunctionPointer(void, RunUnderwaterStuff, (EntityData1 *a1, EntityData2 *a2, CharObj2 *data2), 0x446F10);
FunctionPointer(int, SomethingAboutWater, (EntityData1 *data1, EntityData2 *a1, CharObj2 *a3), 0x449500);
FunctionPointer(void, PlayAnimation, (AnimThing *anim), 0x44A800);
ObjectFunc(DeathZoneHandler, 0x44AE80);
VoidFunc(ResetRestartData, 0x44ED50);
FunctionPointer(int, CheckRestartLevel, (), 0x44ED70);
FunctionPointer(void, RestartCheckpoint, (NJS_VECTOR *Position, Rotation3 *Rotation), 0x44EDB0);
FunctionPointer(void, SetRestartData, (NJS_VECTOR *Position, Rotation3 *Rotation), 0x44EE70);
ObjectFunc(CheckPoint_Display, 0x44EEF0);
FunctionPointer(int, DisplayCheckpointTime_B, (), 0x44F2D0);
ObjectFunc(CheckPoint_Main, 0x44F540);
ObjectFunc(Ring_Display, 0x44F6B0);
FunctionPointer(void, ResetParticle, (EntityData1 *a1, NJS_SPRITE *a2), 0x44F930);
ObjectFunc(Ring_Collect_Maybe, 0x44FC80);
ObjectFunc(DroppedRing_Main, 0x44FD10);
FunctionPointer(void, SpawnDroppedRings, (float x, float y, float z, int count), 0x4502D0);
ObjectFunc(Ring_Main, 0x450370);
FunctionPointer(void, HurtCharacter, (char a1), 0x4506F0);
FunctionPointer(int, TestObjectIntersect, (Mysterious64Bytes *mysterious, NJS_OBJECT *a2), 0x456510);
FunctionPointer(void, InitSpriteTable, (QueuedModelParticle *array, int count), 0x456B80);
VoidFunc(Direct3D_DrawSpriteTable, 0x456CD0);
VoidFunc(ClearSpriteTable, 0x456DE0);
FunctionPointer(void, SetParticleDepthOverride, (float a1), 0x456EB0);
ObjectFunc(DrawStageMissionImage, 0x457120);
FunctionPointer(int, LoadStageMissionImage, (), 0x457450);
FunctionPointer(signed int, GetMissionType, (int character, signed int a2), 0x4575D0);
ObjectFunc(FreeStageMissionImage, 0x457B90);
ObjectFunc(PressStartSprite_Main, 0x457D10);
ObjectFunc(PressStartSprite_Draw, 0x457D30);
ObjectFunc(PressStartSprite_Delete, 0x457E90);
FunctionPointer(void, PauseMenu_ActuallyDrawTheThings, (int n, NJS_POINT2 *pos, NJS_POINT2 *scale), 0x457EB0);
FunctionPointer(int, PauseMenu_Draw, (int, int, int), 0x458020);
FunctionPointer(int, PauseMenu_PrepareToActuallyDrawThethings, (int a1, int, int, int), 0x458080);
FunctionPointer(char, DecPauseSelection, (char index), 0x458260);
FunctionPointer(Uint8, GetPauseDisplayOptions, (Uint8 *a1), 0x4582E0);
FunctionPointer(char, IncPauseSelection, (char index), 0x4583F0);
FunctionPointer(signed int, UpdatePauseSelectionMaybe, (unsigned __int16 selection), 0x458470);
FunctionPointer(bool, LevelHasMap, (), 0x458720);
VoidFunc(PauseMenu_Map_DisplayCallback, 0x458830);
VoidFunc(PauseMenu_Map_Display, 0x458B00);
FunctionPointer(int, PauseMenu_CameraIGuess, (int), 0x458D00);
FunctionPointer(signed int, PauseMenu_SetControls, (int mode_maybe), 0x458F90);
ObjectFunc(Tails_Jiggle_Delete, 0x45B810);
ObjectFunc(Tails_Jiggle_Main, 0x45B840);
ObjectFunc(Snowboard_Tails_Display, 0x45BC40);
ObjectFunc(Snowboard_Delete, 0x45BDD0);
ObjectFunc(DelayedVoice_Main, 0x45BE40);
ObjectFunc(Tails_Delete, 0x45BEA0);
ObjectFunc(Snowboard_Tails_Main, 0x45E4B0);
FunctionPointer(int, Load_DelayedSound_Voice, (int), 0x45E5A0);
ObjectFunc(Tails_Display, 0x460C40);
ObjectFunc(Snowboard_Tails_Load, 0x461510);
ObjectFunc(Tails_Main, 0x461700);
FunctionPointer(int, ActuallyDoCameraRotationMaybe, (), 0x4629A0);
ObjectFunc(CameraFunc_FirstPerson, 0x465F50);
VoidFunc(Camera_SetCameraFunc_Unknown, 0x466B60);
FunctionPointer(bool, CameraFunc_Unknown, (EntityData1 *a1, EntityData1 *a2, int a3), 0x4682B0);
ObjectFunc(Capsule_Display, 0x46ADB0);
ObjectFunc(Capsule_Delete, 0x46AE30);
ObjectFunc(Capsule_Main, 0x46B130);
ObjectFunc(Capsule_Load, 0x46B170);
ObjectFunc(AnimalPickup_Main, 0x46B330);
ObjectFunc(AnimalPickup_Load, 0x46B5F0);
FunctionPointer(int, AnimalPickup_Load_, (), 0x46B650);
VoidFunc(LoadSETObjs_P2NotTailsAndNotDebug, 0x46B7B0);
VoidFunc(LoadSETObjs_NoP2OrDebugOrP2Tails, 0x46BA40);
VoidFunc(LoadSETObjs, 0x46BCE0);
VoidFunc(CountSetItemsMaybe, 0x46BD20);
VoidFunc(CountSetItems, 0x46BE50);
FunctionPointer(signed int, ClipObject, (ObjectMaster *a1, float dist), 0x46C010);
VoidFunc(SetObjList, 0x46C1D0);
FunctionPointer(int, ClipSetObject, (ObjectMaster *a1), 0x46C360);
FunctionPointer(signed int, ClipSetObject_Min, (ObjectMaster *a1), 0x46C390);
ObjectFunc(FishingHud_Delete, 0x46C8D0);
ObjectFunc(FishingHud_DrawHIT, 0x46C920);
VoidFunc(FishingHud_DrawReel, 0x46C9F0);
VoidFunc(FishingHud_DrawRod, 0x46CAB0);
FunctionPointer(void, FishingHud_DrawMeters, (float length), 0x46CC70);
ObjectFunc(BigHud_DrawWeightAndLife, 0x46FB00);
ObjectFunc(BigHud_Main, 0x470090);
FunctionPointer(int, ResetBigStuff, (), 0x470120);
ObjectFunc(FishingHud_Display, 0x470580);
ObjectFunc(FishingHud_Main, 0x471580);
FunctionPointer(ObjectMaster *, FishingHud_Load, (ObjectMaster *a1), 0x471ED0);
ObjectFunc(Knuckles_Delete, 0x472180);
ObjectFunc(Knuckles_Display, 0x4721B0);
FunctionPointer(int, Knuckles_Something_E, (), 0x473570);
FunctionPointer(int, IsInDeathZone_, (EntityData1 *a1), 0x4751B0);
ObjectFunc(EmeraldRadarHud_Main, 0x475A70);
ObjectFunc(EmeraldRadarHud_Load, 0x475BA0);
ObjectFunc(EmeraldRadarHud_Grab_Draw, 0x475D50);
ObjectFunc(EmeraldRadarHud_Display, 0x475E50);
ObjectFunc(EmeraldRadarHud_Load_Load, 0x476440);
ObjectFunc(DisplayCheckpointTime_A, 0x476740);
ObjectFunc(Knuckles_Main, 0x47A770);
FunctionPointer(bool, IsFastSonicAI, (), 0x47C200);
ObjectFunc(TailsVS_DrawBarThing, 0x47C260);
FunctionPointer(int, SetTailsRaceVictory, (), 0x47C450);
FunctionPointer(int, SetOpponentRaceVictory, (), 0x47C460);
FunctionPointer(int, GetRaceWinnerPlayer, (), 0x47C470);
ObjectFunc(MRace_EggMobile_TexlistManager, 0x47D2A0);
ObjectFunc(Sonic2PAI_Main, 0x47D640);
ObjectFunc(Eggman2PAI_Main, 0x47D750);
ObjectFunc(Sonic2PAI_Load, 0x47D820);
ObjectFunc(Eggman2PAI, 0x47D8C0);
FunctionPointer(void, LoadTailsOpponent, (__int16 character, __int16 loop, __int16 level), 0x47D940);
FunctionPointer(float, njScalor_copy, (NJS_VECTOR *a1), 0x47DA70);
FunctionPointer(float, SomeNormalizationThingIGuess, (NJS_VECTOR *a, NJS_VECTOR *b), 0x47DAA0);
ObjectFunc(TailsAI_Delete, 0x47DB10);
FunctionPointer(int, LoadTitleCardTexture, (int minDispTime), 0x47DFB0);
FunctionPointer(Sint32, DisplayTitleCard, (), 0x47E170);
FunctionPointer(bool, CheckTailsAI, (), 0x47E7C0);
ObjectFunc(TailsAI_Main, 0x47E8F0);
ObjectFunc(Load2PTails, 0x47ED60);
ObjectFunc(Gamma_Delete, 0x47EED0);
ObjectFunc(Gamma_Display, 0x47FD50);
ObjectFunc(Gamma_Main, 0x483430);
ObjectFunc(Amy_Delete, 0x484360);
ObjectFunc(Amy_Display, 0x487260);
ObjectFunc(Amy_Main, 0x48ABF0);
ObjectFunc(Big_Delete, 0x48B730);
ObjectFunc(Big_Display, 0x48B8B0);
ObjectFunc(Big_Main, 0x490A00);
FunctionPointer(EntityData1 *, Sonic_LightSpeedAttack_GetTarget, (EntityData1 *entity, CharObj2 *charobj2), 0x492710);
FunctionPointer(void, Sonic_InitLightDash, (EntityData1 *a1, EntityData2 *a2, CharObj2 *a3), 0x492AB0);
ObjectFunc(Sonic_Jiggle_Delete, 0x493790);
ObjectFunc(Sonic_Jiggle_Main, 0x4937B0);
ObjectFunc(CollisionCrashThing_Main, 0x493C70);
FunctionPointer(void, LoadSonicDashTrail, (EntityData1 *a1), 0x494050);
ObjectFunc(Sonic_Snowboard_Display, 0x494140);
ObjectFunc(Sonic_SuperPhysics_Delete, 0x4942D0);
ObjectFunc(Sonic_SuperPhysics_Main, 0x494320);
ObjectFunc(Sonic_SuperPhysics_Load, 0x494350);
ObjectFunc(MetalSonic_AfterImage_Display, 0x4946E0);
FastcallFunctionPointer(void, DoSonicGroundAnimation, (CharObj2 *a1, EntityData1 *a2), 0x494840);
ObjectFunc(Sonic_Delete, 0x494860);
ObjectFunc(Sonic_Display, 0x4948C0);
ObjectFunc(Sonic_Snowboard_Main, 0x4958C0);
ObjectFunc(MetalSonic_AfterImage_Main, 0x4959A0);
ObjectFunc(Snowboard_Sonic_Load, 0x4959E0);
ObjectFunc(MetalSonic_AfterImages_Main, 0x495A50);
FunctionPointer(void, Sonic_Act1, (EntityData1 *entity1, EntityData2 *entity2, CharObj2 *obj2), 0x496F50);
ObjectFunc(Sonic_Main, 0x49A9B0);
FunctionPointer(int, DoStatusThing, (EntityData1 *data1, EntityData2 *data2, int n), 0x49CDA0);
ObjectFunc(RedMountainSetObj_Delete, 0x49D050);
ObjectFunc(O_HANAB_Main, 0x49D070);
ObjectFunc(OBowWindow_Main, 0x49D140);
ObjectFunc(RedMountainSetObj_Display, 0x49D220);
FunctionPointer(int, RedMountainSetObj_Main, (ObjectMaster *a1), 0x49D450);
FunctionPointer(NJS_OBJECT *, DynamicCOL_AddFromEntity, (NJS_OBJECT *a1, ObjectMaster *a2, ColFlags surfaceFlags), 0x49D6C0);
FunctionPointer(Sint32, RedMountainSetObj_SetFunctions, (ObjectMaster *a1), 0x49DFF0);
FunctionPointer(void, EPJoinVertexes, (char index, NJS_OBJECT *a2, NJS_MOTION *a3, float a4), 0x49E300);
FunctionPointer(float, DrawShadow, (EntityData1 *a1, float a2), 0x49EE30);
FunctionPointer(void, RunEntityIntersections, (EntityData1 *a1, struct_a3 *a2), 0x49F1A0);
ObjectFunc(E102_TimeAddHud_Display, 0x49FDA0);
ObjectFunc(E102_TimeAddHud_Delete, 0x49FEE0);
ObjectFunc(E102_TimeAddHud_Main, 0x49FF10);
ObjectFunc(CharBubble_Delete, 0x4A0720);
ObjectFunc(CharBubble_Display, 0x4A0760);
ObjectFunc(SonicDashTrail_Display, 0x4A1190);
ObjectFunc(SonicDashTrail_Main, 0x4A1260);
FunctionPointer(void, Sonic_DisplayLightDashModel, (EntityData1 *data1, EntityData2 *data2_pp, CharObj2 *data2), 0x4A1630);
ObjectFunc(CharBubble_Main, 0x4A2570);
ObjectFunc(SonicDashTrail_Init, 0x4A2AA0);
ObjectFunc(CharBubble_Init, 0x4A2B60);
ObjectFunc(EmeraldPiece_Display, 0x4A2FB0);
ObjectFunc(EmeraldPiece_Main, 0x4A2FD0);
ObjectFunc(EmeraldPiece_Load, 0x4A3420);
ObjectFunc(TikalHint_Child_Delete, 0x4A3500);
ObjectFunc(TikalHint_Child_Display, 0x4A3650);
ObjectFunc(TikalHint_Child_Child, 0x4A38D0);
ObjectFunc(TikalHint_Child, 0x4A39F0);
ObjectFunc(ERobo_0_child_Display, 0x4A4220);
FunctionPointer(int, ERobo_0_child_Main, (ObjectMaster *a1), 0x4A4940);
ObjectFunc(ERobo_0_child, 0x4A65F0);
ObjectFunc(ERobo_0, 0x4A6700);
ObjectFunc(Leon_Display_B, 0x4A8390);
ObjectFunc(Leon_Display_A, 0x4A83B0);
ObjectFunc(Leon_Main, 0x4A83D0);
ObjectFunc(Leon_Load, 0x4A85C0);
ObjectFunc(EAclift, 0x4AA620);
ObjectFunc(Kiki_Display, 0x4AB2B0);
ObjectFunc(Kiki_Main, 0x4ACF80);
ObjectFunc(Kiki_Load, 0x4AD140);
ObjectFunc(Boss_Display_, 0x4AD3F0);
ObjectFunc(Boss_Main_, 0x4AD660);
ObjectFunc(Bos_Main__, 0x4ADB30);
ObjectFunc(Boss_Display__, 0x4ADF10);
ObjectFunc(Bos_Main___, 0x4AE5B0);
ObjectFunc(UnidusA_Main, 0x4AF190);
ObjectFunc(UnidusB_Main, 0x4AF500);
ObjectFunc(UnidusC_Main, 0x4AF860);
ObjectFunc(Spinner_Display, 0x4AFD80);
ObjectFunc(SpinnerA_Main, 0x4B0DF0);
ObjectFunc(SpinnerB_Main, 0x4B0F40);
ObjectFunc(SpinnerC_Main, 0x4B1090);
ObjectFunc(EPolice_Display, 0x4B2710);
ObjectFunc(EPolice_Main, 0x4B30E0);
ObjectFunc(EPolice, 0x4B3210);
ObjectFunc(DisplayBossName, 0x4B33D0);
ObjectFunc(LifeGauge_Main, 0x4B3830);
FunctionPointer(int, LoadEggRoboTextures, (), 0x4B3DA0);
FunctionPointer(void, SetEmblemCollected, (SaveFileData *savefile, signed int index), 0x4B3F30);
FunctionPointer(int, CountEmblems, (SaveFileData *a1), 0x4B3F90);
FunctionPointer(int, GetEmblemBitIndex, (int character, signed int level, int mission), 0x4B3FC0);
ObjectFunc(Emblem_Display, 0x4B4100);
ObjectFunc(DeleteObject_, 0x4B4160);
ObjectFunc(EmblemCollected_Display, 0x4B4170);
ObjectFunc(EmblemScreen_Main, 0x4B4200);
ObjectFunc(EmblemScreen_Delete, 0x4B4530);
FunctionPointer(bool, GetLevelEmblemCollected, (SaveFileData *savefile, int character, signed int level, int mission), 0x4B45C0);
FunctionPointer(void, SetLevelEmblemCollected, (SaveFileData *savefile, int character, signed int level, int mission), 0x4B4640);
ObjectFunc(EmblemCollected_Main, 0x4B46A0);
ObjectFunc(EmblemCollected_Init, 0x4B4860);
ObjectFunc(Emblem_Main, 0x4B4940);
FunctionPointer(void, SetCurrentSetData, (int level), 0x4B4BF0);
FunctionPointer(void, SetCurrentCamData, (int level), 0x4B4C70);
FunctionPointer(bool, IsBoardingSubGame, (), 0x4B5000);
FunctionPointer(bool, IsTwinkleCircuit, (), 0x4B5040);
FunctionPointer(void, HudShowScore, (int), 0x4B50E0);
FunctionPointer(int, GetCurrentCharIDOrSomething, (), 0x4B5A30);
FunctionPointer(void, LoadCharBoss, (CharBossData *a1), 0x4B6050);
ObjectFunc(SetupCharBossArena, 0x4B6D20);
FunctionPointer(ObjectMaster *, LoadSonicBossAI, (CharBossData *a1), 0x4B7030);
FunctionPointer(ObjectMaster *, LoadCharBossAI, (CharBossData *a1), 0x4B71A0);
FunctionPointer(void, DisplayHintText, (const char *const *strings, int duration), 0x4B79C0);
FunctionPointer(signed int, GetHintText, (int id, int *data), 0x4B7C10);
FunctionPointer(signed int, ProcessAnimatedModelNode_Instanced, (NJS_MATRIX_PTR a1, NJS_ACTION *a2, float n, Uint32 instance, NJS_MATRIX_PTR a5), 0x4B81F0);
FunctionPointer(Sint32, SetInstancedMatrix, (Uint32 index, NJS_MATRIX_PTR matrix), 0x4B82D0);
VoidFunc(ResetJumpPanels, 0x4B8320);
ObjectFunc(JumpPanel_Display, 0x4B8660);
ObjectFunc(JumpPanel_Delete, 0x4B87D0);
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
ObjectFunc(Rumble_Main, 0x4BCB20);
FunctionPointer(void, Rumble_Load, (Uint32 time), 0x4BCB60);
FunctionPointer(void, RumbleA, (Uint32 port, Sint32 time), 0x4BCBC0);
FunctionPointer(void, RumbleB, (Uint32 port, Uint32 time, int a3, int a4), 0x4BCC10);
FunctionPointer(void, DirectionToRotation, (const NJS_VECTOR *dir, Angle *x, Angle *y), 0x4BCCA0);
FunctionPointer(void, DashHoop_Transform, (NJS_VECTOR *position, NJS_VECTOR *delta), 0x4BCDC0);
FunctionPointer(void, DrawCollisionSphere, (NJS_VECTOR *pos, float scale), 0x4BCF40);
FunctionPointer(signed int, Weed_SpawnItem, (EntityData1 *entity, int n), 0x4BD000);
FunctionPointer(NJS_OBJECT *, DynamicCOL_AddObject, (ObjectMaster *a1, NJS_MODEL_SADX *a2), 0x4BD090);
ObjectFunc(LevelItem_Display, 0x4BF5A0);
ObjectFunc(LevelItem_Main, 0x4BF600);
ObjectFunc(LevelItem_Delete, 0x4BFDA0);
ObjectFunc(ItemBoxAir_Display, 0x4BFDD0);
FunctionPointer(int, DoThingWithItemBoxPowerupIndex, (int n), 0x4C00B0);
ObjectFunc(ItemBoxAir_DisplayDebugStrings, 0x4C05C0);
ObjectFunc(ItemBoxAir_Main, 0x4C07D0);
FunctionPointer(void, EmeraldRadarHud_Draw, (NJS_VECTOR *pos, int n, float scale, float alpha), 0x4C0DC0);
FunctionPointer(void, Knuckles_MaximumHeat_Draw, (NJS_VECTOR *position, float alpha), 0x4C1330);
FunctionPointer(void, SetToCameraPosition, (NJS_VECTOR *a1), 0x4C2790);
FunctionPointer(void, DashPanel_ForceGammaModeChange, (int a1), 0x4C38C0);
ObjectFunc(GammaTargetThing, 0x4C4A90);
ObjectFunc(ESman, 0x4C8FB0);
ObjectFunc(RocketPlatform_Display, 0x4C92C0);
ObjectFunc(RocketPlatform_Delete, 0x4C9330);
ObjectFunc(RocketHThingB, 0x4CA290);
ObjectFunc(RocketVThingB, 0x4CA3C0);
ObjectFunc(RocketH_Main, 0x4CA530);
ObjectFunc(RocketHS_Main, 0x4CA660);
ObjectFunc(RocketV_Main, 0x4CA7D0);
ObjectFunc(RocketVS_Main, 0x4CA900);
FunctionPointer(void, SetSwitchPressedState, (int index, int on), 0x4CB4C0);
FunctionPointer(Sint32, IsSwitchPressed, (int index), 0x4CB4F0);
VoidFunc(PressedSwitches_Reset, 0x4CB500);
ObjectFunc(Switch_Display, 0x4CB590);
ObjectFunc(Switch_Main, 0x4CBA80);
ObjectFunc(ODynamite, 0x4CBDE0);
FunctionPointer(ObjectData2 *, AllocateObjectData2, (ObjectMaster *obj, EntityData1 *entity), 0x4CC990);
FunctionPointer(void, ObjectData2_SetStartPosition, (EntityData1 *a1, ObjectData2 *a2), 0x4CD370);
FunctionPointer(float, GetPlayerDistance, (EntityData1 *entity, int playerIndex), 0x4CD610);
FunctionPointer(Uint8, IsOnScreen, (NJS_VECTOR *v), 0x4CD730);
FunctionPointer(int, EnemyBounceAndRumble, (unsigned __int8 playerID), 0x4CDFE0);
FunctionPointer(bool, OhNoImDead, (EntityData1 *a1, ObjectData2 *a2), 0x4CE030);
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
FunctionPointer(ObjectMaster *, LoadGammaBossAI, (CharBossData *a1), 0x4D5CF0);
FunctionPointer(ObjectMaster *, LoadKnucklesBossAI, (CharBossData *a1), 0x4D6590);
ObjectFunc(ItemBox_Display, 0x4D6810);
ObjectFunc(ItemBox_Display_Unknown, 0x4D6990);
ObjectFunc(ItemBox_Display_Destroyed, 0x4D6B20);
ObjectFunc(ItemBox_Delete, 0x4D6BA0);
FunctionPointer(void, SpeedShoesPowerup, (EntityData1 *entity), 0x4D6BF0);
FunctionPointer(void, FiveRingsPowerup, (EntityData1 *entity), 0x4D6C50);
FunctionPointer(void, TenRingsPowerup, (EntityData1 *entity), 0x4D6C90);
FunctionPointer(void, RandomRingPowerup, (EntityData1 *entity), 0x4D6CD0);
FunctionPointer(void, ExtraLifePowerup, (EntityData1 *entity), 0x4D6D40);
FunctionPointer(void, InvincibilityPowerup, (EntityData1 *entity), 0x4D6D80);
FunctionPointer(void, BarrierPowerup, (EntityData1 *entity), 0x4D6DC0);
FunctionPointer(void, BombPowerup, (EntityData1 *entity), 0x4D6E00);
FunctionPointer(void, MagneticBarrierPowerup, (EntityData1 *entity), 0x4D6E40);
ObjectFunc(ItemBox_Main, 0x4D6F10);
FunctionPointer(void, SpawnItemBox, (NJS_VECTOR *position, signed int kind), 0x4D7100);
ObjectFunc(AltItemBox_Main, 0x4D7170);
FunctionPointer(void, DisplayDebugModeString, (signed int offset, const char *fmt, ...), 0x4D9740);
FunctionPointer(void, DrawLensFlare, (NJS_VECTOR *position), 0x4DA000);
FunctionPointer(void, DrawLensFlare_B, (NJS_VECTOR *position, NJS_VECTOR *a2), 0x4DA3A0);
ObjectFunc(SkyBox_TwinkleCircuit_Load, 0x4DA810);
ObjectFunc(DelayedMusicPlayer_Main, 0x4DAA40);
ObjectFunc(Obj_TwinkleCircuit, 0x4DAA80);
ObjectFunc(TwinkleCircuitResultsMaybe, 0x4DAFB0);
ObjectFunc(TwinkleCircuit_DrawHUD, 0x4DB5E0);
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
ObjectFunc(Obj_WindyValley, 0x4DDB30);
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
ObjectFunc(OTuribr_Main, 0x4E0A60);
ObjectFunc(OTuribr, 0x4E0F40);
ObjectFunc(OTuribr2, 0x4E0F80);
ObjectFunc(OE102br, 0x4E0FC0);
ObjectFunc(OBigflo_Display, 0x4E1010);
ObjectFunc(OBigflo_Main, 0x4E1360);
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
ObjectFunc(E103Enemy_Delete, 0x4E7A30);
ObjectFunc(E103Enemy_Display, 0x4E7AC0);
ObjectFunc(E103Enemy_Main, 0x4E7E90);
ObjectFunc(E103Enemy_Load, 0x4E7F50);
ObjectFunc(Snowboard_Load, 0x4E9660);
ObjectFunc(SkyBox_IceCap_Delete, 0x4E9760);
ObjectFunc(SkyBox_IceCap_Display, 0x4E9950);
ObjectFunc(Obj_Icecap, 0x4E9D90);
ObjectFunc(SkyBox_IceCap_Main, 0x4EA1D0);
ObjectFunc(SkyBox_IceCap_Load, 0x4EA260);
ObjectFunc(Cold_Main, 0x4EA280);
ObjectFunc(Cold_Load, 0x4EA3B0);
ObjectFunc(ACT12JOIN_Main, 0x4EBDF0);
ObjectFunc(ACT23JOIN_Main, 0x4EBEB0);
ObjectFunc(OKuguri, 0x4EC090);
ObjectFunc(IceCapScoreMultiplier_Draw, 0x4EC120);
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
ObjectFunc(OIcicle_Display, 0x4F4BA0);
ObjectFunc(OIcicle_Main, 0x4F4C20);
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
ObjectFunc(EmeraldCoast_SkyBox_Delete, 0x4F6C60);
ObjectFunc(Obj_EmeraldCoast, 0x4F6D60);
ObjectFunc(EmeraldCoast_SkyBox_Display, 0x4F6DB0);
ObjectFunc(EmeraldCoast_SkyBox_Main, 0x4F71A0);
ObjectFunc(SkyBox_EmeraldCoast_Load, 0x4F7230);
ObjectFunc(ECoast_AniTexMan_Display, 0x4F7250);
ObjectFunc(ECoast1_AniTexMan1_Main, 0x4F7300);
ObjectFunc(ECWaterSurface_Display, 0x4F76C0);
ObjectFunc(ECWaterSurface_Main, 0x4F7800);
ObjectFunc(ECWaterSurface_Load, 0x4F79C0);
ObjectFunc(OAsiato, 0x4F7E90);
FunctionPointer(int, ObjectSelectedDebug, (ObjectMaster *obj), 0x4F88A0);
FunctionPointer(void, EmeraldCoast_OceanDraw, (OceanData *o), 0x4F8A30);
VoidFunc(Obj_EmeraldCoast_InitOceanData, 0x4F8C10);
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
ObjectFunc(OGomban_Delete, 0x4FDCD0);
ObjectFunc(OGomban_Display, 0x4FDD20);
ObjectFunc(OGomban_Main, 0x4FDFC0);
ObjectFunc(OGomban_Load, 0x4FE8C0);
ObjectFunc(OPier_Display, 0x4FE8E0);
ObjectFunc(OPier_Delete, 0x4FE970);
ObjectFunc(OPier_Main, 0x4FED30);
ObjectFunc(OPier_Load, 0x4FF060);
ObjectFunc(OParasol_Display, 0x4FF140);
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
ObjectFunc(Yasi3_Display, 0x500C50);
ObjectFunc(YASI0_Display, 0x500E60);
ObjectFunc(Yasi1_Display, 0x500E80);
ObjectFunc(Yasi2_Display, 0x500EA0);
ObjectFunc(Yasi3, 0x500EC0);
ObjectFunc(Yasi0, 0x500F50);
ObjectFunc(Yasi1, 0x500FF0);
ObjectFunc(Yasi2, 0x501090);
ObjectFunc(Obj_EC1Water_Display, 0x501130);
ObjectFunc(Obj_EC1Water_Main, 0x5017D0);
ObjectFunc(Obj_EC1Water_Load, 0x501970);
FunctionPointer(void, DoSomethingWithSaveInfo, (LPCSTR a1, _WIN32_FIND_DATAA a2), 0x504E50);
FunctionPointer(unsigned __int8, FindSaveFiles, (), 0x505050);
ObjectFunc(FileSelect_Main, 0x5053F0);
ObjectFunc(FileSelect_Display, 0x505550);
FunctionPointer(ObjectMaster *, FileSelect_Load, (int *a1), 0x505600);
VoidFunc(ResetSelectedTrialLevel, 0x5060E0);
FunctionPointer(bool, IsAdventureComplete, (int character), 0x5063E0);
FunctionPointer(int, IsMetalSonicUnlocked, (), 0x506490);
VoidFunc(j_LoadSave, 0x506620);
FunctionPointer(int, GetCharacterUnlockedFlag, (int), 0x506630);
FunctionPointer(int, MenuSelectButtonsPressed, (), 0x5067E0);
FunctionPointer(int, MenuBackButtonsPressed, (), 0x5067F0);
FunctionPointer(int, GetCurrentSave, (), 0x506800);
FunctionPointer(signed int, GetMenuVoice, (), 0x5068E0);
FunctionPointer(int, CountEmblemsCurrentSave, (), 0x507B30);
FunctionPointer(void, DrawTiledBG_AVA_BACK, (float depth), 0x507BB0);
ObjectFunc(OptionsMenu_Main, 0x5096F0);
ObjectFunc(OptionsMenu_Display, 0x509810);
FunctionPointer(ObjectMaster *, OptionsMenu_Load, (int a1), 0x509860);
ObjectFunc(SubGameLevelList_Main, 0x50A450);
ObjectFunc(SubGameLevelList_Display, 0x50A640);
FunctionPointer(void, SubGameLevelList_Load, (int a1), 0x50A6C0);
ObjectFunc(LevelSelect_Main, 0x50B220);
ObjectFunc(LevelSelect_Display, 0x50B410);
FunctionPointer(void, LevelSelect_Load, (int a1), 0x50B490);
FunctionPointer(void, DrawAVA_TITLE_BACK, (float depth), 0x50BA90);
FunctionPointer(int, EmblemResultMenu_Main, (ObjectMaster *), 0x50DEA0);
ObjectFunc(EmblemResultMenu_Display, 0x50DFD0);
FunctionPointer(void, EmblemResultMenu_Load, (int a1), 0x50E020);
ObjectFunc(SoundTest_Main, 0x511270);
ObjectFunc(SoundTest_Display, 0x511390);
FunctionPointer(int, SoundTest_Load, (int), 0x511400);
ObjectFunc(CharacterSelectMenu_Main, 0x5122D0);
ObjectFunc(CharacterSelectMenu_Display, 0x512450);
FunctionPointer(ObjectMaster *, CharacterSelectMenu_Load, (int a1), 0x5124A0);
ObjectFunc(CharSel_Qmark_Main, 0x5125A0);
FunctionPointer(void, CharSel_KeepCharactersFromFalling, (void *a1), 0x512910);
ObjectFunc(CharSel_Main, 0x512B40);
ObjectFunc(CharSel_Display, 0x512B60);
ObjectFunc(CharSel_Delete, 0x512BA0);
VoidFunc(CharSel_LoadA, 0x512BC0);
VoidFunc(DeleteCharSel, 0x512C20);
FunctionPointer(int, CountUnlockedCharacters, (), 0x512C50);
FunctionPointer(int, PlayVideo, (int a1), 0x5130A0);
VoidFunc(DoCrashingEggCarrierCutsceneThing, 0x5136E0);
FunctionPointer(void, DisplayVideoFrame, (int width, int height), 0x5139F0);
ObjectFunc(EC0NPC_Main, 0x51ACD0);
ObjectFunc(EC0NPC_Load, 0x51ADD0);
VoidFunc(LoadEC0NPCs, 0x51AE00);
ObjectFunc(SkyBox_EggCarrierOutside_Load, 0x51B7A0);
ObjectFunc(Obj_EggCarrier0, 0x51C360);
FunctionPointer(void, EggCarrier_OceanDraw, (OceanData *arg_0), 0x51C440);
ObjectFunc(BoatEC_Main, 0x51C9A0);
ObjectFunc(OIkada, 0x51CC30);
ObjectFunc(OMast, 0x51CD20);
ObjectFunc(OBook1_Main, 0x51CFB0);
ObjectFunc(OBook1, 0x51D030);
ObjectFunc(OBook2, 0x51D050);
ObjectFunc(OBook3, 0x51D070);
ObjectFunc(OPier, 0x51D400);
ObjectFunc(OPalmbase_0, 0x51D760);
ObjectFunc(OParasol_1, 0x51D840);
ObjectFunc(OBchair, 0x51D930);
ObjectFunc(OPalmbase, 0x51DAB0);
ObjectFunc(OPoolwater_Display, 0x51DBA0);
ObjectFunc(OPoolwater, 0x51DC30);
ObjectFunc(OSkydeck_Display, 0x51DF90);
ObjectFunc(OSkydeck, 0x51DFA0);
ObjectFunc(OPropeller_1, 0x51E180);
ObjectFunc(OPoolDoor, 0x51E320);
ObjectFunc(OEggmanbed, 0x51E430);
ObjectFunc(OMoveseat, 0x51E670);
ObjectFunc(OElevator_1, 0x51E910);
ObjectFunc(OLivinglight, 0x51EB50);
ObjectFunc(OSlot, 0x51EF40);
ObjectFunc(OWasher, 0x51F560);
FunctionPointer(int, nullsub, (), 0x51F680);
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
ObjectFunc(SkyBox_EggCarrierInside_Load, 0x525BB0);
ObjectFunc(Obj_EggCarrier3, 0x525C30);
ObjectFunc(OReservoir, 0x525D50);
ObjectFunc(OKurotama, 0x526060);
ObjectFunc(OEggwrap, 0x526660);
ObjectFunc(OBooster, 0x526740);
ObjectFunc(OContainer_2, 0x526840);
ObjectFunc(OBlift, 0x526B20);
ObjectFunc(OJdoor, 0x526D60);
ObjectFunc(OJswitch, 0x526F30);
FunctionPointer(void, njDrawSprite3D_ADV01C_A, (NJS_SPRITE *sp, Int n, NJD_SPRITE attr), 0x527930);
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
ObjectFunc(SkyBox_MysticRuins_Display, 0x52FB90);
ObjectFunc(SkyBox_MysticRuins_Load, 0x530670);
ObjectFunc(Obj_MysticRuins, 0x5306F0);
FunctionPointer(void, GetEntrance_MRuins, (EntityData1 *a1), 0x530790);
VoidFunc(SetTimeOfDay_Day, 0x5307E0);
VoidFunc(SetTimeOfDay_Night, 0x530960);
VoidFunc(SetTimeOfDay_Evening, 0x5319C0);
ObjectFunc(OHandKey, 0x532400);
FunctionPointer(void, MysticRuins_OceanDraw, (OceanData *x), 0x532500);
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
ObjectFunc(OPalm_Display, 0x538900);
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
ObjectFunc(Obj_Past_Delete, 0x5419C0);
ObjectFunc(SkyBox_Past_Load, 0x542030);
ObjectFunc(Obj_Past, 0x5420C0);
FunctionPointer(void, GetEntrance_Past, (EntityData1 *a1), 0x542180);
FunctionPointer(void, Past_OceanDraw, (OceanData *x), 0x542850);
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
ObjectFunc(SkyBox_Chaos0_Load, 0x545DF0);
ObjectFunc(Obj_Chaos0, 0x545E60);
ObjectFunc(Chaos0_Rain, 0x5462F0);
FunctionPointer(int, Chaos0_Rain_Load, (), 0x546320);
ObjectFunc(Chaos0_Delete, 0x5481D0);
ObjectFunc(Chaos0_Display, 0x548360);
ObjectFunc(Chaos0_Main, 0x548640);
VoidFunc(Chaos0_Load, 0x548B70);
ObjectFunc(OBclock_0, 0x548D30);
ObjectFunc(_0_pole, 0x548E70);
ObjectFunc(OPato0, 0x549770);
ObjectFunc(OHeli0_Display, 0x549840);
ObjectFunc(OHeli0_Main, 0x549990);
ObjectFunc(OHeli0, 0x549A30);
ObjectFunc(OGras0, 0x549A90);
ObjectFunc(OGras1, 0x549AC0);
ObjectFunc(OGras2, 0x549AF0);
ObjectFunc(OGras3, 0x549B20);
ObjectFunc(SkyBox_Chaos2_Load, 0x54A670);
ObjectFunc(Obj_Chaos2, 0x54A700);
ObjectFunc(Chaos2_Delete, 0x54D790);
ObjectFunc(Chaos2_Display, 0x54D840);
ObjectFunc(Chaos2_Main, 0x54DB90);
FunctionPointer(ObjectMaster *, Chaos2_Load, (), 0x54DF00);
ObjectFunc(Chand, 0x54E2E0);
ObjectFunc(Table, 0x54E420);
ObjectFunc(SkyBox_Chaos4_Load, 0x550930);
ObjectFunc(Obj_Chaos4, 0x550A30);
VoidFunc(j_ClampGlobalColorThing_Thing, 0x551510);
ObjectFunc(Chaos4_Main, 0x552960);
FunctionPointer(int, SetClip_Chaos6K, (int), 0x556D60);
FunctionPointer(void, SetClip_Chaos6S, (signed int a1), 0x556E40);
ObjectFunc(SkyBox_Chaos6_Load, 0x557610);
ObjectFunc(Obj_Chaos6, 0x557920);
ObjectFunc(Chaos6_Display, 0x558E30);
ObjectFunc(Chaos6_Delete, 0x559F20);
ObjectFunc(Chaos6_Main, 0x559FC0);
ObjectFunc(Smoke, 0x55A940);
VoidFunc(PerfectChaos_Load2ndBreathTexlist, 0x55D8B0);
ObjectFunc(Obj_Chaos7, 0x55DCD0);
ObjectFunc(SkyBox_PerfectChaos_Load, 0x55DDC0);
ObjectFunc(Sonic_SuperAura_Delete, 0x55E5B0);
ObjectFunc(Sonic_SuperAura_Display, 0x55E620);
ObjectFunc(Sonic_SuperAura_Main, 0x55E920);
FunctionPointer(signed int, InitSpinTrailModel, (EntityData1 *a1), 0x55F540);
ObjectFunc(SonicDashTrail_Delete, 0x55FAB0);
ObjectFunc(Sonic_SuperAura_Load, 0x55FAF0);
ObjectFunc(PerfectChaos_Delete, 0x55FEB0);
ObjectFunc(PerfectChaos_Display, 0x55FED0);
ObjectFunc(PerfectChaos_Main, 0x560260);
ObjectFunc(Chaos7Explosion_Display, 0x5632F0);
ObjectFunc(Chaos7Explosion_Main, 0x563370);
ObjectFunc(Chaos7Explosion_Init, 0x5633A0);
ObjectFunc(Chaos7Damage_Display, 0x5633C0);
ObjectFunc(Chaos7Damage_Slave_Main, 0x563450);
ObjectFunc(Chaos7Damage_Slave_Load, 0x563490);
ObjectFunc(Chaos7Damage_Master_Main, 0x563BA0);
ObjectFunc(Chaos7Damage_Master_Load, 0x563C50);
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
ObjectFunc(Obj_E101Beta, 0x566C00);
FunctionPointer(int, SetClip_E101R, (int), 0x568D20);
ObjectFunc(Obj_E101BetaMK2, 0x569040);
ObjectFunc(SkyBox_E101R_Load, 0x569060);
VoidFunc(Load_E101R_GC_NB, 0x5690D0);
FunctionPointer(void, E101Mk2_OceanDraw, (OceanData *v), 0x56CC30);
ObjectFunc(Obj_EggHornet, 0x571850);
ObjectFunc(SkyBox_EggHornet_Load, 0x571940);
FunctionPointer(int, PlayVoiceButCheckThing, (), 0x571990);
ObjectFunc(EggHornet_Main, 0x572010);
FunctionPointer(void, EggHornet_OceanDraw, (OceanData *a1), 0x572270);
ObjectFunc(EggHornet_Missile_Main, 0x574E00);
ObjectFunc(EggHornet_Missile_Loader_Main, 0x5750F0);
FunctionPointer(int, EggHornet_Missile_Rumble, (), 0x5751F0);
FunctionPointer(int, EggHornet_Something_Rumble, (), 0x575200);
ObjectFunc(SkyBox_EggWalker_Load, 0x575890);
ObjectFunc(Obj_EggWalker, 0x5758D0);
FunctionPointer(int, PlayEggmanVoice, (int), 0x5759C0);
ObjectFunc(EggWalker_Main, 0x576650);
ObjectFunc(OKazari, 0x577470);
ObjectFunc(SkyBox_EggViper_Load, 0x57C400);
ObjectFunc(Obj_EggViper, 0x57C460);
ObjectFunc(SetCurrentStageLight_EggViper, 0x57E560);
FunctionPointer(int, SetClip_ZERO, (int), 0x5850F0);
ObjectFunc(Obj_Zero, 0x585400);
ObjectFunc(SkyBox_Zero_Load, 0x585430);
VoidFunc(Load_EROBO_GC_NB, 0x5854A0);
FunctionPointer(void, Zero_OceanDraw, (OceanData *x), 0x587E10);
FunctionPointer(bool, MissionScreenStateIs2, (), 0x590620);
FunctionPointer(int, Release_MIS_C_TEXLIST, (), 0x590660);
VoidFunc(MissionCompleteScreen_Draw, 0x590690);
FunctionPointer(Sint32, MissionCompleteScreen, (), 0x590E60);
ObjectFunc(MissionStartClear_Delete, 0x591040);
ObjectFunc(MissionStartClear_Main, 0x591260);
VoidFunc(CountCompletedMissions, 0x591880);
FunctionPointer(int, Mission_Run, (), 0x5919C0);
VoidFunc(MissionSET_Clear, 0x591A20);
VoidFunc(MissionSET_Load, 0x591A70);
FunctionPointer(void, DeactivateMission, (char mission, char deleteobjects), 0x591C10);
FunctionPointer(ObjectMaster *, GetSetObjInstance, (ObjectMaster *caller, __int16 index), 0x592200);
FunctionPointer(_BOOL1, IsMissionSETObj, (void *a1), 0x592240);
FunctionPointer(_BOOL1, IsActiveMissionObject, (ObjectMaster *a1), 0x592260);
VoidFunc(LevelDestructor_MissionMode, 0x592370);
ObjectFunc(ClearMission, 0x5923C0);
ObjectFunc(CollectMissionItem, 0x592440);
ObjectFunc(Spinner_MissionStuff, 0x592520);
FunctionPointer(int, Mission_DrawCounter, (), 0x592A60);
FunctionPointer(int, Mission_DrawTimer, (), 0x592D50);
FunctionPointer(void, MissionSET_LoadCam, (int arg_0, int arg_4), 0x5931B0);
ObjectFunc(MissionTest_Display, 0x593290);
ObjectFunc(MissionTest_Main, 0x5932E0);
ObjectFunc(MissionTest_Load, 0x5933E0);
ObjectFunc(MissionStatue_Display, 0x593410);
ObjectFunc(MissionStatue_Delete, 0x5934C0);
ObjectFunc(MissionStatue_Main, 0x593510);
ObjectFunc(MissionStatue_Load, 0x593670);
ObjectFunc(MissionTimer_Main, 0x5936B0);
ObjectFunc(MissionTimer_Load, 0x5937B0);
ObjectFunc(MissionRobotFish_Main, 0x594D70);
ObjectFunc(MissionSign_Display, 0x594D90);
ObjectFunc(MissionSign_Main, 0x594E20);
ObjectFunc(MissionSign_Load, 0x594F00);
ObjectFunc(MissionMedal_Display, 0x594F40);
ObjectFunc(MissionMedal_Main, 0x594FC0);
ObjectFunc(MissionMedal_Load, 0x595210);
ObjectFunc(MissionFlag_Display, 0x595260);
ObjectFunc(MissionFlag_Main, 0x5952E0);
ObjectFunc(MissionFlag_Load, 0x595440);
ObjectFunc(MissionBalloon_Display, 0x595480);
ObjectFunc(MissionBalloon_Main, 0x595590);
ObjectFunc(MissionBalloon_Load, 0x595760);
ObjectFunc(MissionEndMark_Display, 0x5957D0);
ObjectFunc(MissionEndMark_Main, 0x595850);
ObjectFunc(MissionEndMark_Load, 0x595A40);
ObjectFunc(MissionStart_Main, 0x595B30);
ObjectFunc(MissionStart_Load, 0x595CF0);
ObjectFunc(SGene, 0x597660);
ObjectFunc(SBass, 0x5977F0);
ObjectFunc(SkyBox_SandHill_Load, 0x597AE0);
ObjectFunc(Obj_SandHill, 0x597C60);
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
FunctionPointer(float, SandHillScoreMultiplier_Draw, (float x, float y, float pri, __int16 n), 0x5990D0);
ObjectFunc(SandHillScoreMultiplier_Main, 0x5991A0);
FunctionPointer(void, RumbleWithinRange, (NJS_VECTOR *position, float range), 0x599FE0);
ObjectFunc(SkyBox_HotShelter_Load, 0x59A2A0);
ObjectFunc(Obj_HotShelter, 0x59A380);
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
ObjectFunc(DynamicCOL_DeleteObject, 0x59DBF0);
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
ObjectFunc(OGateSw_Display, 0x5A57E0);
ObjectFunc(OGateSw_Main, 0x5A5890);
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
ObjectFunc(PlayDestroyTargetVoices, 0x5ADE00);
ObjectFunc(PlayFinalEggDisposeIntrudersVoice, 0x5ADE90);
ObjectFunc(SkyBox_FinalEgg_Load, 0x5ADFE0);
ObjectFunc(Obj_FinalEgg, 0x5AE1A0);
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
ObjectFunc(Otatekan_Display, 0x5B4690);
ObjectFunc(Otatekan, 0x5B4840);
ObjectFunc(OElevator2, 0x5B4B30);
ObjectFunc(OHasigo, 0x5B4D00);
ObjectFunc(OEggKanban, 0x5B5100);
ObjectFunc(OStandLight_Display, 0x5B5150);
ObjectFunc(OStandLight_Main, 0x5B5200);
ObjectFunc(OStandLight, 0x5B5260);
ObjectFunc(OPinLamp, 0x5B5530);
ObjectFunc(OSideLamp, 0x5B5550);
ObjectFunc(OUpTarget2, 0x5B5E70);
ObjectFunc(OUpTarget1, 0x5B6590);
ObjectFunc(OTarget, 0x5B66E0);
ObjectFunc(FinalEggLevel2IsolationWalls, 0x5B6B30);
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
ObjectFunc(OSetstep_Display, 0x5BE920);
ObjectFunc(OSetstep_Init, 0x5BE9B0);
ObjectFunc(OContainer, 0x5BEBA0);
ObjectFunc(OSide_arm_fix, 0x5BED90);
ObjectFunc(OUp_arm_fix, 0x5BEFA0);
ObjectFunc(OSide_arm, 0x5BF9A0);
ObjectFunc(OUp_arm, 0x5C0260);
VoidFunc(Casino_Setup, 0x5C0960);
ObjectFunc(DrawBALLS, 0x5C0B70);
ObjectFunc(Casino_HandleSpawnPoints, 0x5C0CC0);
ObjectFunc(Obj_Casinopolis, 0x5C0F40);
ObjectFunc(RingInterval_Main, 0x5C1480);
FunctionPointer(void, AddRingsInterval_Delay5, (char count), 0x5C1500);
FunctionPointer(void, TakeRingsInterval1, (char count), 0x5C1530);
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
ObjectFunc(OTenss_Delete, 0x5CCF90);
ObjectFunc(OTenss_Display, 0x5CCFF0);
ObjectFunc(OTenss_Main, 0x5CD100);
ObjectFunc(OTenss_Load, 0x5CD2D0);
ObjectFunc(OTensc, 0x5CD2F0);
ObjectFunc(OShat1, 0x5CDA50);
ObjectFunc(OShat2, 0x5CDA60);
ObjectFunc(OMb_Delete, 0x5CDA80);
ObjectFunc(OMb_Display, 0x5CDAC0);
ObjectFunc(OMb_Main, 0x5CDB40);
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
ObjectFunc(ODmgsphere_Main, 0x5D39C0);
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
ObjectFunc(Carddisp_Display, 0x5D73F0);
ObjectFunc(Carddisp_Main, 0x5D7500);
ObjectFunc(Carddisp, 0x5D7510);
ObjectFunc(Card, 0x5D7760);
ObjectFunc(Idea_Display, 0x5D77D0);
ObjectFunc(Idea_Main, 0x5D78B0);
ObjectFunc(Idea_Init, 0x5D78E0);
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
ObjectFunc(PinballJackpot_Display, 0x5E1020);
ObjectFunc(PinballJackpot_Sprite_Main, 0x5E11A0);
ObjectFunc(PinballJackpot_Main, 0x5E12C0);
ObjectFunc(Obj_LostWorld, 0x5E18B0);
ObjectFunc(SkyBox_LostWorld_Load, 0x5E1FC0);
ObjectFunc(ORmirror_Display, 0x5E2380);
FunctionPointer(void, LostWorldFog_Fade, (float a1, float increment), 0x5E24B0);
ObjectFunc(LostWorldFogThing_Delete, 0x5E2530);
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
ObjectFunc(OSuimen_Switch_Display, 0x5E83C0);
ObjectFunc(OSuimen_Switch, 0x5E8410);
ObjectFunc(OSuimen_Display, 0x5E8520);
ObjectFunc(OSuimen_Main, 0x5E8580);
ObjectFunc(OSuimen, 0x5E86C0);
ObjectFunc(OTap, 0x5E8900);
ObjectFunc(ORlight, 0x5E8960);
ObjectFunc(OTswitch, 0x5E8DB0);
ObjectFunc(OTpanel_Delete, 0x5E8E20);
ObjectFunc(OTpanel_Display, 0x5E90C0);
ObjectFunc(OTpanel_Main, 0x5E92A0);
ObjectFunc(OTpanel_MainDebug, 0x5E9410);
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
FunctionPointer(void, SkyDeck_SimulateAltitude, (Uint16 act), 0x5ECA80);
FunctionPointer(void, SkyDeck_QueueDraw, (NJS_OBJECT *obj, NJS_TEX *uv, int uv_count), 0x5ED110);
ObjectFunc(SkyBox_SkyDeck_Display, 0x5ED1E0);
ObjectFunc(Radder_Display, 0x5EE330);
ObjectFunc(VariousSolidThings_Delete, 0x5EE520);
ObjectFunc(W_wall, 0x5EF2B0);
ObjectFunc(NextAct_Main, 0x5EF9D0);
ObjectFunc(Hangpole, 0x5EFB00);
ObjectFunc(Radder_Main, 0x5EFC50);
ObjectFunc(Fence_l, 0x5EFD90);
ObjectFunc(Fence_m, 0x5EFDE0);
ObjectFunc(Fence_s, 0x5EFE30);
ObjectFunc(Hang_c, 0x5F0040);
ObjectFunc(Trail, 0x5F00E0);
ObjectFunc(Homing, 0x5F02B0);
ObjectFunc(Obj_SkyDeck, 0x5F02E0);
ObjectFunc(SkyBox_SkyDeck, 0x5F0340);
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
FunctionPointer(void, SetClip_RedMountain, (signed int a1), 0x600700);
ObjectFunc(SkyBox_RedMountain_Load, 0x600FB0);
ObjectFunc(Obj_RedMountain, 0x601550);
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
ObjectFunc(OHataki_Display, 0x605FA0);
ObjectFunc(OHataki_Main, 0x606100);
ObjectFunc(OHataki_Load, 0x6063B0);
ObjectFunc(OLamp1_Main, 0x606820);
ObjectFunc(OLamp2_Main, 0x606840);
ObjectFunc(ODeka_Display, 0x606860);
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
ObjectFunc(OMtKusa_Display, 0x6082C0);
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
ObjectFunc(OBonehead_Display, 0x60E1E0);
ObjectFunc(OBonehead_Main, 0x60E410);
ObjectFunc(OBonestick_Main, 0x60E4D0);
ObjectFunc(O_untei1_Main, 0x60E8B0);
ObjectFunc(O_untei2_Main, 0x60E8F0);
ObjectFunc(O_untei3_Main, 0x60E930);
ObjectFunc(O_untei16_Main, 0x60E980);
ObjectFunc(O_komokosu_Main, 0x60EA70);
ObjectFunc(O_honoo_Main, 0x60EE00);
ObjectFunc(Obj_SpeedHighway, 0x610980);
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
ObjectFunc(OAntena_Display, 0x615D00);
ObjectFunc(OAntena_Main, 0x615E60);
ObjectFunc(OAntena, 0x615EB0);
ObjectFunc(OSuimen_Delete, 0x615F30);
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
ObjectFunc(OPit_Delete, 0x618690);
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
ObjectFunc(j_LandTable_MinimumRadius_Reset, 0x61CA80);
ObjectFunc(TimedMusicChange, 0x61CA90);
ObjectFunc(Obj_TwinklePark, 0x61D150);
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
ObjectFunc(OPit_Display, 0x620600);
ObjectFunc(OPit, 0x6207F0);
ObjectFunc(OPirates, 0x620AF0);
ObjectFunc(OGoRound, 0x621670);
ObjectFunc(OFloat, 0x621850);
ObjectFunc(OFlyer, 0x621BD0);
ObjectFunc(OFoothold, 0x621DB0);
ObjectFunc(OCatapult, 0x6223C0);
ObjectFunc(OBowling, 0x623450);
ObjectFunc(OBall_0, 0x623BB0);
ObjectFunc(OBarrel_Display, 0x623CC0);
ObjectFunc(OBarrel_Delete, 0x623D70);
ObjectFunc(OBarrel, 0x624020);
ObjectFunc(OShutter, 0x624380);
ObjectFunc(OMonitor, 0x6247C0);
ObjectFunc(Obj_HedgehogHammer, 0x624A70);
FunctionPointer(void, njDrawSprite3D_ADV01C_B, (NJS_SPRITE *sp, Int n, NJD_SPRITE attr), 0x6253F0);
ObjectFunc(MinigameMogura, 0x6268D0);
FunctionPointer(int, CheckSkyChaseActionThing, (), 0x626CC0);
ObjectFunc(Obj_SkyChase1, 0x626CE0);
ObjectFunc(Obj_SkyChase2, 0x627000);
ObjectFunc(Tornado_Delete, 0x627410);
ObjectFunc(Tornado_Display, 0x627430);
FunctionPointer(int, TornadoTarget_UsingLockOn, (), 0x627740);
ObjectFunc(Tornado_Main, 0x627EC0);
VoidFunc(SkyChaseInit, 0x6281A0);
ObjectFunc(HudScore_Display, 0x628330);
ObjectFunc(HudScore_Main, 0x6283D0);
ObjectFunc(TornadoHP_Draw, 0x628490);
ObjectFunc(TornadoHP_Main, 0x628740);
FunctionPointer(void, TornadoHP_Load, (int x, int y, signed int a3), 0x628890);
ObjectFunc(TornadoTarget_Display, 0x6288F0);
ObjectFunc(TornadoTarget_CalculateCenterPoint, 0x628D50);
ObjectFunc(TornadoTargetSprite_TargetLock_Display, 0x628DB0);
FunctionPointer(int, TornadoTarget_IDK, (), 0x628FB0);
ObjectFunc(TornadoTargetSprite_TargetLock_Delete, 0x629010);
ObjectFunc(TornadoTargetSprite_TargetLock_Main, 0x629060);
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
FunctionPointer(int, CheckIfCameraIsInHotel, (), 0x62EA30);
ObjectFunc(WelcomeToTwinklePark, 0x62EC60);
ObjectFunc(j_DisableFog, 0x62EFE0);
ObjectFunc(Obj_StationSquare, 0x62EFF0);
ObjectFunc(SkyBox_StationSquare_Load, 0x62F070);
ObjectFunc(SSNPC_Main, 0x62F450);
ObjectFunc(SSNPC_Load, 0x62F5A0);
VoidFunc(LoadSSNPCs, 0x62F5D0);
FunctionPointer(void, GetEntrance_SSquare, (EntityData1 *a1), 0x62F6D0);
ObjectFunc(BurgerShopStatue_Main, 0x630780);
ObjectFunc(OHotelFront_Display, 0x630A30);
ObjectFunc(OHotelFront_Main_AC, 0x630D90);
ObjectFunc(OHotelFront_Main_AB, 0x630DE0);
ObjectFunc(OHotelFront_Main_A, 0x630E70);
FunctionPointer(int, OHotelFront_Main_B, (ObjectMaster *a1), 0x630F10);
ObjectFunc(OHotelFront, 0x630FB0);
ObjectFunc(OHotelBack, 0x631030);
ObjectFunc(OHotelPool, 0x6310B0);
FunctionPointer(void, StationSquare_OceanDraw, (OceanData *x), 0x631140);
ObjectFunc(ORouka, 0x631480);
ObjectFunc(People_Main, 0x634980);
FunctionPointer(int, LoadSSMessages, (), 0x634DE0);
ObjectFunc(Cream_Display, 0x634F40);
ObjectFunc(Cream_Main, 0x635270);
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
ObjectFunc(Chao_AddToCollisionList, 0x637340);
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
ObjectFunc(Credits_StartMusic_Main, 0x6408C0);
ObjectFunc(Credits_Main_ForReal, 0x6408F0);
FunctionPointer(void, CreditsInfo_LoadTextures, (CreditsInfo *a1), 0x640D40);
FunctionPointer(void, CreditsInfo_FreeTextures, (CreditsInfo *a1), 0x640D90);
ObjectFunc(Credits_Main, 0x6411A0);
FunctionPointer(ObjectMaster *, Credits_Init, (CreditsList *a1, int a2), 0x6413E0);
ObjectFunc(EndBG_Display, 0x6414A0);
ObjectFunc(EndBG_Main, 0x641680);
FunctionPointer(ObjectMaster *, EndBG_Init, (signed int a2, CreditsInfo *a3, int a4, CreditsList *a5), 0x641910);
ObjectFunc(TutorialInstruction_Delete, 0x641E40);
ObjectFunc(TutorialPlayMusic_Main, 0x641E70);
ObjectFunc(TutorialInstruction_Main, 0x641EE0);
FunctionPointer(ObjectMaster *, TutorialInstruction_Load, (signed int a1, int a2), 0x6420F0);
FunctionPointer(void, DrawTiledBG, (__int16 *indices, int unused, float x, float y, float z, float hscale, float vscale), 0x642AE0);
ObjectFunc(TutorialInstructionOverlay_Main, 0x642E10);
ObjectFunc(TutorialInstructionOverlay_Display, 0x6430F0);
FunctionPointer(ObjectMaster *, TutorialInstructionOverlay_Load, (signed int a1, int a2, NJS_VECTOR *a3), 0x643450);
ObjectFunc(TutorialBackground_Main, 0x643580);
ObjectFunc(TutorialBackground_Display, 0x6436B0);
FunctionPointer(int, TutorialBackground_Load, (char a2, int, int), 0x643B20);
ObjectFunc(RecapBackground_Main, 0x643C90);
ObjectFunc(RecapBackground_Delete, 0x643D10);
FunctionPointer(void, LoadRecapBackground, (int index, int bgNum), 0x643D40);
FunctionPointer(int, start, (), 0x643F96);
FunctionPointer(void, j__free, (void *Memory), 0x6443E2);
FunctionPointer(__int32, j__atol, (const char *Str), 0x6453A2);
FunctionPointer(int, RunInitializationFunctions, (), 0x6466CA);
FunctionPointer(int, j_unknown_libname_27, (), 0x6487BE);
ObjectFunc(TailsVS_DrawWinLose, 0x6EF510);
ObjectFunc(j_RunObjectChildren, 0x6F2E40);
FunctionPointer(Sint32, PRSDec, (Uint8 *a1, Uint8 *a2), 0x6FC900);
VoidFunc(LoadGGMenuPVM, 0x6FE010);
VoidFunc(FreeGGMenuPVM, 0x6FE0A0);
StdcallFunctionPointer(void, njDrawTextureMemList_, (NJS_TEXTURE_VTX *a1, signed int a2, int a3, int a4), 0x701750);
VoidFunc(j_CopyRawControllers, 0x703430);
VoidFunc(j_j_CopyRawControllers, 0x703440);
FunctionPointer(void, SetNextChaoStage, (SADXChaoStage stage), 0x715110);
VoidFunc(GoToNextChaoStage, 0x715130);
FunctionPointer(SADXChaoStage, GetCurrentChaoStage, (), 0x715140);
FunctionPointer(SADXChaoStage, GetLastChaoStage, (), 0x715150);
FunctionPointer(SADXChaoStage, GetNextChaoStage, (), 0x715160);
FunctionPointer(bool, IsNextChaoStageReady, (), 0x715170);
VoidFunc(ChaoMain_Constructor, 0x715440);
FunctionPointer(int, ChaoMain_Destructor, (), 0x715540);
VoidFunc(LoadNextChaoStage, 0x715580);
VoidFunc(ChaoMain_Epilog, 0x715770);
ObjectFunc(ChaoGarden, 0x715820);
VoidFunc(ChaoMain_Prolog, 0x715AE0);
VoidFunc(LoadHeldChao, 0x716200);
ObjectFunc(GBAManager_Main, 0x716850);
ObjectFunc(GBAManagerDestruct, 0x716A60);
FunctionPointer(int, GBAManagerCreate, (), 0x716BA0);
FunctionPointer(Uint8 *, GetChaoSaveAddress, (), 0x716FA0);
VoidFunc(SetupChaoSaveFile, 0x7172D0);
FunctionPointer(Uint8 *, j_GetChaoSaveAddress, (), 0x717450);
VoidFunc(Chao_LoadTrees, 0x717BA0);
VoidFunc(LoadHeldItem, 0x717C80);
FunctionPointer(int, LoadChaoPVPs, (), 0x717E90);
FunctionPointer(void, LoadChaoTexlist, (const char *PVMName, NJS_TEXLIST *TexListPtr, unsigned __int16 a3), 0x718880);
FunctionPointer(void, FreeChaoTexlist, (unsigned __int16 a1), 0x7188E0);
VoidFunc(FreeChaoTexlists, 0x718940);
VoidFunc(FreeChaoTextures, 0x718960);
VoidFunc(ResetModules, 0x718990);
FunctionPointer(void, FreeModule, (int index), 0x7189D0);
FunctionPointer(void, LoadGameDLL, (const char *name, int index), 0x718A20);
VoidFunc(LoadMarket, 0x718BB0);
FunctionPointer(int, ChaoGardenFunc_BlackMarket, (), 0x718BF0);
VoidFunc(ChaoGardenFunc_MysticRuins, 0x718C10);
ObjectFunc(ChaoStgGarden02MR_Main, 0x718C30);
ObjectFunc(ChaoStgGarden02MR_Display, 0x718CC0);
ObjectFunc(ChaoStgGarden02MR_Delete, 0x718CD0);
ObjectFunc(ChaoStgGarden02MR_Load, 0x718D00);
VoidFunc(LoadMRGarden, 0x718E90);
VoidFunc(ChaoGardenFunc_EggCarrier, 0x718FB0);
ObjectFunc(ChaoStgGarden01EC_Main, 0x718FD0);
ObjectFunc(ChaoStgGarden01EC_Delete, 0x719010);
ObjectFunc(ChaoStgGarden01EC_Load, 0x719040);
VoidFunc(LoadECGarden, 0x7191D0);
FunctionPointer(int, ChaoGardenFunc_StationSquare, (), 0x719210);
ObjectFunc(ChaoStgGarden00SS_Main, 0x719230);
ObjectFunc(ChaoStgGarden00SS_Delete, 0x7192E0);
ObjectFunc(ChaoStgGarden00SS_Load, 0x719320);
VoidFunc(LoadSSGarden, 0x719600);
ObjectFunc(ChaoStgEntrance_Main, 0x719880);
VoidFunc(LoadRaceEntry, 0x7199B0);
FunctionPointer(int, ChaoGardenFunc_RaceEntry, (), 0x7199E0);
FunctionPointer(int, ChaoGardenFunc_Transporter, (), 0x719A00);
VoidFunc(LoadChaoNamingMachine, 0x719C40);
ObjectFunc(ChaoStgRace_Delete, 0x719C70);
ObjectFunc(ChaoStgRace_Main, 0x719CA0);
ObjectFunc(ChaoStgRace_Init, 0x719D00);
VoidFunc(LoadChaoRace, 0x719DB0);
FunctionPointer(int, ChaoGardenFunc_Race, (), 0x719DF0);
FunctionPointer(ObjectMaster *, GetChaoObject, (unsigned __int16 zero, unsigned __int16 index), 0x71A1F0);
FunctionPointer(Sint32, AddToGlobalChaoThingMaybe, (unsigned __int16 index, ObjectMaster *obj, __int16 a3, ChaoData *data), 0x71A600);
ObjectFunc(ChaoManager_Main, 0x71A720);
ObjectFunc(ChaoManager_Display, 0x71A780);
ObjectFunc(ChaoManager_Delete, 0x71A7B0);
VoidFunc(ChaoManager_Load, 0x71A7C0);
FunctionPointer(Sint32, _AddToGlobalChaoThingMaybe, (unsigned __int16 index, ObjectMaster *obj, __int16 a3), 0x71A840);
ObjectFunc(al_confirmsave__FDest, 0x71A910);
ObjectFunc(al_confirmsave, 0x71A980);
FunctionPointer(void, al_confirmsave_load, (int a1), 0x71AB20);
VoidFunc(al_confirmsave_load_zero, 0x71AB70);
ObjectFunc(al_confirmload__FDest, 0x71AB90);
ObjectFunc(al_confirmload, 0x71AC10);
VoidFunc(al_confirmload_load, 0x71AE50);
ObjectFunc(ChaoDX_Message_PlayerAction_Delete, 0x71B1D0);
ObjectFunc(ChaoDX_Message_PlayerAction_Display, 0x71B210);
ObjectFunc(ChaoDX_Message_PlayerAction_Main, 0x71B2B0);
VoidFunc(ChaoDX_Message_PlayerAction_Load, 0x71B3B0);
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
FunctionPointer(void, Chao_IncrementTypeStat, (ChaoDNA *a1, ChaoDataBase *a2), 0x71DF20);
ObjectFunc(Chao_Main, 0x7202D0);
ObjectFunc(Chao_Delete, 0x720390);
ObjectFunc(Chao_Display, 0x7204B0);
FunctionPointer(ObjectMaster *, CreateChao, (ChaoData *chaoData, int a2, void *buffer, NJS_VECTOR *position, Angle angle), 0x720670);
VoidFunc(CnkDisableSpecular, 0x720A10);
VoidFunc(CnkRestoreSpecular, 0x720A40);
ObjectFunc(Chao_Tree_Display, 0x721100);
ObjectFunc(Chao_Tree_Delete, 0x721180);
ObjectFunc(Chao_Tree_Main, 0x7212A0);
FunctionPointer(ObjectMaster *, Chao_CreateTree, (NJS_VECTOR *a1, int a2), 0x721800);
VoidFunc(LoadAnimalPVMs, 0x722520);
VoidFunc(FreeAnimalTexLists, 0x722550);
FunctionPointer(void, DisplayDebugShape, (int a1, int a2, int a3, int a4, Uint32 color, float depth_maybe), 0x723B70);
FunctionPointer(bool, ScaleObjectMaster_XYZ, (ObjectMaster *obj, float x, float y, float z), 0x723C50);
FunctionPointer(bool, ScaleObjectMaster_XZ, (ObjectMaster *obj, float x, float z), 0x723E60);
FunctionPointer(int, Load_al_stg_kinder_ad_tex, (), 0x724EA0);
FunctionPointer(Sint32, SetTexlist_al_stg_kinder_ad_tex, (), 0x724ED0);
FunctionPointer(int, j_GetMarketRings, (), 0x7257E0);
FunctionPointer(int, ChaoStgGarden02MR_Display_0, (), 0x728E20);
ObjectFunc(Chao_Transport_Display, 0x729250);
ObjectFunc(Chao_Transport_Main, 0x7293E0);
FunctionPointer(void, LoadChaoTransporter, (ObjectMaster *parent, int destination, float xpos, float ypos, float zpos, int yrot), 0x729550);
ObjectFunc(Chao_Name_Machine_Main, 0x729610);
ObjectFunc(Chao_Name_Machine_Delete, 0x729D80);
ObjectFunc(Chao_Name_Machine_Display, 0x729DB0);
ObjectFunc(Chao_Name_Machine_Load, 0x729EE0);
FunctionPointer(void, LoadChaoNameMachine, (NJS_VECTOR *position, int yrotation), 0x729F40);
VoidFunc(ChaoGardenMR_SetLandTable_Day, 0x72A790);
VoidFunc(Print_ChaoStgGarden02MR_Daytime_epilog, 0x72A810);
VoidFunc(ChaoGardenMR_SetLandTable_Evening, 0x72A820);
VoidFunc(Print_ChaoStgGarden02MR_Evening_epilog, 0x72A8A0);
VoidFunc(ChaoGardenMR_SetLandTable_Night, 0x72A8B0);
VoidFunc(Print_ChaoStgGarden02MR_Night_epilog, 0x72A930);
ObjectFunc(ChaoRaceDoor_Display, 0x72A940);
ObjectFunc(ChaoRaceDoor_MainD, 0x72A9E0);
ObjectFunc(ChaoRaceDoor_MainC, 0x72AA60);
ObjectFunc(ChaoRaceDoor_MainB, 0x72AAA0);
ObjectFunc(ChaoRaceDoor_Main, 0x72AB10);
ObjectFunc(ChaoRaceDoor_Load, 0x72AB50);
ObjectFunc(LoadChaoRaceDoor, 0x72AB80);
ObjectFunc(SSGardenExit_Display, 0x72AC00);
ObjectFunc(Chao_Station_Door, 0x72AD60);
FunctionPointer(int, SSGardenExit_Main, (ObjectMaster *a1), 0x72ADD0);
ObjectFunc(SSGardenExit_Load, 0x72AE10);
ObjectFunc(LoadSSGardenExit, 0x72AE40);
ObjectFunc(Chao_Market_Door_Display, 0x72AE70);
ObjectFunc(Chao_Market_Door, 0x72AEE0);
ObjectFunc(LoadChaoMarketDoorChild, 0x72AFD0);
VoidFunc(LoadButtonIndicators, 0x72B330);
FunctionPointer(void *, LoadChaoMessageFile, (const char *filename, int language), 0x72C060);
FunctionPointer(void, j__HeapFree, (LPVOID lpMem), 0x72C110);
FunctionPointer(void *, LoadChaoMessageFile2, (const char *filename), 0x72C180);
FunctionPointer(void, DrawOverlaySprite, (NJS_SPRITE *a1, Int n, float pri), 0x72CC70);
FunctionPointer(void, ProgressChaoEvolution, (ObjectMaster *a1, float a2), 0x730870);
FunctionPointer(void, IncrementPowerRun, (ObjectMaster *a1, float increment), 0x7308D0);
FunctionPointer(void, IncrementFlySwim, (ObjectMaster *a1, float increment), 0x730930);
FunctionPointer(void, IncrementAlignment, (ObjectMaster *a1, float increment), 0x730990);
FunctionPointer(void, SetupChaoAppearance, (ChaoDataBase *a1, char *a2), 0x730B30);
VoidFunc(EnableChao, 0x731CD0);
VoidFunc(DisableChao, 0x731CE0);
FunctionPointer(int, IsChaoEnabled, (), 0x731CF0);
ObjectFunc(ChaoDebug_MenuBack, 0x731DE0);
FunctionPointer(void, SetChaoDebugFunction_Disabled, (void (__cdecl *object_function)(ObjectMaster *), const char *name), 0x731E00);
FunctionPointer(void, SetChaoDebugFunction_Enabled, (void (__cdecl *object_function)(ObjectMaster *), const char *name), 0x731E40);
VoidFunc(SetChaoDebugFunctions, 0x731E80);
FunctionPointer(void, DrawChaoHud, (ChaoHudThing *a1, Uint32 color), 0x732850);
FunctionPointer(void, DrawChaoHudArray, (ChaoHudThing *array, int count, Uint32 color), 0x732940);
ObjectFunc(SetDefaultChaoAnimalParts, 0x734930);
ObjectFunc(LoadChaoMotionTable, 0x734EC0);
ObjectFunc(ChaoCocoon_Display, 0x73C670);
ObjectFunc(ChaoCocoon_Delete, 0x73C7A0);
ObjectFunc(ChaoCocoon_Display_, 0x73C820);
FunctionPointer(void, DrawChao, (ObjectMaster *a1, ChunkObjectPointer *a2), 0x73E730);
FunctionPointer(ChaoData2 *, AllocateChaoData2, (ObjectMaster *chao), 0x73FE70);
ObjectFunc(Chao_CopyPositionToOtherPosition, 0x740050);
FunctionPointer(float, ChaoDistanceFromPlayer, (ObjectMaster *a1, int n), 0x7401A0);
ObjectFunc(FreeSomeChaoData, 0x742060);
FunctionPointer(signed int, ResetChao, (ObjectMaster *chao, ChaoType type), 0x7420C0);
FunctionPointer(int, GetChaoBondIndexFromCharID, (), 0x742140);
ObjectFunc(ConvertBattleCharBondsToDX, 0x7421A0);
FunctionPointer(char, GetChaoCharacterBond, (ObjectMaster *a1), 0x742220);
ObjectFunc(ChaoDebug_LIKE, 0x7423E0);
ObjectFunc(ChaoEgg_Main, 0x7435F0);
ObjectFunc(ChaoEgg_Display, 0x7436F0);
ObjectFunc(ChaoEgg_Delete, 0x7438E0);
FunctionPointer(ObjectMaster *, CreateChaoEgg, (ChaoDNA *dna, ChaoDataBase *data, int n, NJS_VECTOR *a4, int a5), 0x743960);
ObjectFunc(MRChaoGarden_AnimateWaterfall_Main, 0x7459F0);
FunctionPointer(void, MRChaoGarden_AnimateWaterfall_Load, (NJS_TEX *uvs, int uvCount), 0x745A20);
ThiscallFunctionPointer(void, SetChaoName, (void *data), 0x74B2B0);
ObjectFunc(ChaoDebug_SHAPE, 0x757580);
ObjectFunc(ChaoDebug_ENV_MAP, 0x757B70);
ObjectFunc(ChaoDebug_COLOR, 0x757C30);
ObjectFunc(ChaoDebug_PARTS_DX, 0x757CD0);
ObjectFunc(ChaoDebug_MINIMAL_FLAG_DX, 0x757F00);
ObjectFunc(ChaoDebug_PARTS_BTL, 0x7580A0);
ObjectFunc(ChaoDebug_MINIMAL_FLAG_BTL, 0x7581A0);
ObjectFunc(ChaoDebug_MEDAL, 0x758370);
ObjectFunc(ChaoDebug_OBAKE, 0x7584A0);
ObjectFunc(ChaoDebug_BEHAVIOR, 0x758640);
ObjectFunc(ChaoDebug_GROUP, 0x758BE0);
ObjectFunc(ChaoDebug_STORY_FLAG, 0x758DC0);
ObjectFunc(ChaoDebug_ART_FLAG, 0x758F40);
ObjectFunc(ChaoDebug_DANCE_FLAG, 0x759080);
ObjectFunc(ChaoDebug_SONG_FLAG, 0x7591C0);
ObjectFunc(ChaoDebug_INST_FLAG, 0x759300);
ObjectFunc(ChaoDebug_LANDMARK, 0x759460);
ObjectFunc(ChaoDebug_MOTION, 0x7594C0);
ObjectFunc(ChaoDebug_LIFE, 0x759590);
ObjectFunc(ChaoDebug_SE, 0x759620);
ObjectFunc(ChaoDebug_VOICE, 0x759760);
ObjectFunc(ChaoDebug_VOICE2, 0x7598A0);
ObjectFunc(ChaoDebug_JINGLE, 0x7599E0);
ObjectFunc(ChaoDebug_SKILL, 0x759B80);
ObjectFunc(ChaoDebug_PARAMETER, 0x759DC0);
VoidFunc(j_nullsub_1, 0x76DAB0);
FunctionPointer(void, njDrawPolygon, (NJS_POLYGON_VTX *polygon, Int count, Int trans), 0x77DBC0);
FunctionPointer(void, njDrawTextureMemList, (NJS_TEXTURE_VTX *a1, Int count, Uint32 gbix, Int flag), 0x77DC70);
FunctionPointer(void, SetOceanAlphaModeAndFVF, (int a1), 0x77DCA0);
FunctionPointer(void, Direct3D_DrawFVF_H, (FVFStruct_H_B *data, signed int count), 0x77DD00);
VoidFunc(j_nullsub_2, 0x77DD80);
FunctionPointer(void, Direct3D_EnableHudAlpha, (bool enable), 0x77DD90);
VoidFunc(j_Direct3D_TextureFilterLinear, 0x77DDE0);
FunctionPointer(void, SetHudColorAndTextureNum, (int n, NJS_COLOR color), 0x77DDF0);
FunctionPointer(void, Direct3D_DrawQuad, (NJS_QUAD_TEXTURE_EX *quad), 0x77DE10);
FunctionPointer(void, njDrawLine2D, (NJS_POINT2COL *p, Int n, Float pri, Uint32 attr), 0x77DF40);
FunctionPointer(void, njDrawCircle2D, (NJS_POINT2COL *p, Int n, Float pri, Uint32 attr), 0x77DFC0);
FunctionPointer(void, njDrawSprite2D_DrawNow, (NJS_SPRITE *sp, Int n, Float pri, NJD_SPRITE attr), 0x77E050);
FunctionPointer(void, njDrawSprite3D_DrawNow, (NJS_SPRITE *sp, int n, NJD_SPRITE attr), 0x77E390);
FunctionPointer(void, njDrawLine3D, (NJS_POINT3COL *p, Int n, NJD_DRAW attr), 0x77E820);
FunctionPointer(void, DrawRectPoints, (NJS_POINT2 *points, float scale), 0x77E970);
FunctionPointer(void, njDrawTriangle2D, (NJS_POINT2COL *p, Int n, Float pri, NJD_DRAW attr), 0x77E9F0);
FunctionPointer(void, DrawSomeTriangleFanThing, (NJS_POINT3COL *a1, int a2, int a3), 0x77EAD0);
FunctionPointer(void, njColorBlendingMode_, (NJD_COLOR_TARGET target, NJD_COLOR_BLENDING mode), 0x77EC60);
FunctionPointer(void, Direct3D_EnableAlpha, (BOOL enable), 0x77ECE0);
FunctionPointer(void, Direct3D_SetZFunc, (Uint8 index), 0x77ED00);
FunctionPointer(void, Direct3D_EnableZWrite, (DWORD enable), 0x77ED20);
FunctionPointer(void, j_DeltaTime_Multiplier, (int a1), 0x77ED40);
FunctionPointer(void, Direct3D_SetCullMode, (int a1), 0x77ED50);
FunctionPointer(void, njDrawModel_SADX, (NJS_MODEL_SADX *a1), 0x77EDA0);
FunctionPointer(void, DrawModel_Callback, (NJS_OBJECT *a1), 0x77EF50);
FunctionPointer(void, DrawModel_ResetRenderFlags, (NJS_MODEL_SADX *a1), 0x77EF70);
VoidFunc(j_DirectInput_ReleaseDevice, 0x77EF90);
VoidFunc(PollKeyboard_, 0x77F090);
VoidFunc(ReleaseKeyboardDevice, 0x77F0F0);
FunctionPointer(void, njGetTexSurface_Maybe, (NJS_TEXMEMLIST *a1, Uint8 *data), 0x77F2F0);
ThiscallFunctionPointer(Sint32, Direct3D_SetTexList, (NJS_TEXLIST *__this), 0x77F3D0);
FastcallFunctionPointer(Sint32, njSetTextureNum_, (int num), 0x77F440);
FunctionPointer(signed int, AddExtension, (char *destination, const char *source, char *extension), 0x77F470);
FunctionPointer(NJS_TEXMEMLIST *, GetCachedTexture, (int gbix), 0x77F5B0);
FunctionPointer(void, SetGlobalTextureArray, (NJS_TEXMEMLIST *textureList, int length), 0x77F610);
FunctionPointer(Sint32, njLoadPaletteFile, (const char *filename, NJS_TEXPALETTE *pl), 0x77F700);
FunctionPointer(NJS_TEXMEMLIST *, TexMemList_PixelFormat, (NJS_TEXINFO *texinfo, int gbix), 0x77F7F0);
FunctionPointer(void, LoadPVM_ReadsGlobalIndex, (NJS_TEXMEMLIST *tex), 0x77F910);
ThiscallFunctionPointer(Sint32, njSetTextureNumG, (Uint32 gbix), 0x77F9D0);
ThiscallFunctionPointer(signed int, j_njSetTextureNumG, (int gbix), 0x77FA60);
FunctionPointer(Sint32, njReleaseTexture_, (NJS_TEXLIST *texlist), 0x77FA70);
VoidFunc(njReleaseTextureAll, 0x77FB30);
FunctionPointer(NJS_TEXMEMLIST *, LoadPVR, (void *data, int gbix), 0x77FBD0);
FunctionPointer(Sint32, njLoadTexture, (NJS_TEXLIST *texlist), 0x77FC80);
FunctionPointer(Sint32, njLoadTexturePvmMemory, (Uint8 *addr, NJS_TEXLIST *texList), 0x77FD80);
FunctionPointer(Sint32, njLoadTexturePvmFile, (const char *filename, NJS_TEXLIST *texList), 0x77FEB0);
FunctionPointer(void, njControl3D, (NJD_CONTROL_3D a1), 0x77FF60);
FunctionPointer(void, njSetConstantAttr, (Uint32 and_attr, Uint32 or_attr), 0x77FF70);
FunctionPointer(void, njSetConstantMaterial, (NJS_ARGB *color), 0x77FF90);
VoidFunc(set_dword_3CE712C_0, 0x780090);
StdcallFunctionPointer(BOOL, QueryPerformanceCounter_, (LARGE_INTEGER *lpPerformanceCount), 0x780140);
FastcallFunctionPointer(void, njInitMatrix, (NJS_MATRIX *matrixStack, Sint32 n, Int flag), 0x780180);
FunctionPointer(void, InitPolyBuffers_, (void *a1, int count), 0x780850);
VoidFunc(InitDebugOutput, 0x780870);
FunctionPointer(void, SetDebugFontSize, (unsigned __int16 a1), 0x7808C0);
FunctionPointer(void, SetDebugFontColor, (int color), 0x7808E0);
FunctionPointer(void, DisplayDebugString, (signed int position, const char *text), 0x7808F0);
FunctionPointer(void, PrintDebugNumber, (signed int position, int value, signed int numdigits), 0x780970);
FunctionPointer(void, DisplayDebugFloat, (int position, float value, signed int percision), 0x780AC0);
FunctionPointer(void, DisplayDebugStringFormatted, (signed int position, const char *text, ...), 0x780B30);
VoidFunc(Direct3D_NextScene, 0x780BE0);
FunctionPointer(void, SetNjScreenToPreset, (int a1, int a2, int frame_multiplier), 0x780C30);
FunctionPointer(int, HeapAllocateThing, (DWORD dwBytes), 0x780F60);
FunctionPointer(LPVOID, _HeapAlloc, (int count, int size), 0x780F90);
FunctionPointer(void, _HeapFree, (LPVOID lpMem), 0x780FB0);
VoidFunc(SonicLibraryFinalize, 0x780FD0);
FunctionPointer(int, j_CompareOSVersionInfoForSomeReason, (), 0x781010);
VoidFunc(j_Direct3D_EndScene, 0x781020);
VoidFunc(j_Direct3D_Present, 0x781030);
VoidFunc(j_Direct3D_Clear, 0x781040);
VoidFunc(j_Direct3D_BeginScene, 0x781050);
VoidFunc(j_j_DeltaSleep, 0x781060);
FunctionPointer(void, Direct3D_SetProjectionMatrix_, (D3DMATRIX *matrix), 0x781250);
ThiscallFunctionPointer(void, SetStartupProjection, (D3DMATRIX *__this), 0x781570);
FunctionPointer(void, njSetScreenDist, (int bams), 0x7815C0);
FunctionPointer(void, SetDrawDistance, (float min, float max), 0x7815E0);
FastcallFunctionPointer(void, njCalcPoint, (NJS_MATRIX_CONST_PTR m, const NJS_VECTOR *vs, NJS_VECTOR *vd), 0x781670);
VoidFunc(njPopMatrixEx, 0x7816E0);
ThiscallFunctionPointer(void, njScaleEx, (const NJS_VECTOR *Scale), 0x781700);
FastcallFunctionPointer(void, njRotateXYZ, (NJS_MATRIX_PTR m, Angle angx, Angle angy, Angle angz), 0x781770);
FastcallFunctionPointer(void, njRotateZYX, (NJS_MATRIX_PTR m, Angle angx, Angle angy, Angle angz), 0x7819C0);
ThiscallFunctionPointer(void, njTranslateEx, (const NJS_VECTOR *v), 0x781C10);
VoidFunc(njPushMatrixEx, 0x781C80);
VoidFunc(CopyConstantMaterial, 0x781CC0);
VoidFunc(ResetRenderFlags, 0x781DF0);
FastcallFunctionPointer(void, DrawModel_Callback_, (NJS_OBJECT *obj, void (__cdecl *function)(NJS_MODEL_SADX *)), 0x781E20);
ThiscallFunctionPointer(int, GetConstantMaterialBGRA, (NJS_COLOR *m), 0x781EC0);
FunctionPointer(void, polybuff_basic_strip, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x782030);
FunctionPointer(void, polybuff_basic_tri, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x782170);
FunctionPointer(void, polybuff_basic_quad, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x782200);
FunctionPointer(void, polybuff_uv_strip, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x782310);
FunctionPointer(void, polybuff_uv_tri, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x7824F0);
FunctionPointer(void, polybuff_uv_quad, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x7825C0);
FunctionPointer(void, polybuff_vcolor_strip, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x782730);
FunctionPointer(void, polybuff_vcolor_tri, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x782940);
FunctionPointer(void, polybuff_vcolor_quad, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x782A10);
FunctionPointer(void, polybuff_vcolor_uv_strip, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x782B80);
FunctionPointer(void, polybuff_vcolor_uv_tri, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x782EF0);
FunctionPointer(void, polybuff_vcolor_uv_quad, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x783060);
FunctionPointer(void, SetDrawModelMaterialCallback, (void (__cdecl *a1)()), 0x783260);
FunctionPointer(void, polybuff_normal_strip, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x783270);
FunctionPointer(void, polybuff_normal_tri, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x783410);
FunctionPointer(void, polybuff_normal_quad, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x7834C0);
FunctionPointer(void, polybuff_normal_uv_strip, (NJS_MESHSET *set, NJS_VECTOR *normals, NJS_POINT3 *points), 0x7835F0);
FunctionPointer(void, polybuff_normal_uv_tri, (NJS_MESHSET *a1, NJS_VECTOR *a2, NJS_POINT3 *a3), 0x783830);
FunctionPointer(void, polybuff_normal_uv_quad, (NJS_MESHSET *a1, NJS_VECTOR *a2, NJS_POINT3 *a3), 0x783920);
FunctionPointer(void, polybuff_normal_vcolor_strip, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x783AC0);
FunctionPointer(void, polybuff_normal_vcolor_tri, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x783D90);
FunctionPointer(void, polybuff_normal_vcolor_quad, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x783EC0);
FunctionPointer(void, polybuff_normal_vcolor_uv_strip, (NJS_MESHSET_SADX *a1, NJS_POINT3 *points, NJS_VECTOR *normals), 0x784060);
FunctionPointer(void, polybuff_normal_vcolor_uv_tri, (NJS_MESHSET *a1, NJS_VECTOR *a2, NJS_POINT3 *a3), 0x784480);
FunctionPointer(void, polybuff_normal_vcolor_uv_quad, (NJS_MESHSET *a1, NJS_VECTOR *a2, NJS_POINT3 *a3), 0x784620);
ThiscallFunctionPointer(void, Direct3D_ParseMaterial, (NJS_MATERIAL *material), 0x784850);
FunctionPointer(void, njDrawModel_SADX_Dynamic, (NJS_MODEL_SADX *callback), 0x784AE0);
ThiscallFunctionPointer(void, njGetMatrix, (NJS_MATRIX_PTR m), 0x784BC0);
ThiscallFunctionPointer(void, njTranslate, (NJS_MATRIX_PTR m, Float x, Float y, Float z), 0x784BE0);
ThiscallFunctionPointer(void, njScale, (NJS_MATRIX_PTR m, Float sx, Float sy, Float sz), 0x784C70);
FastcallFunctionPointer(void, njSetMatrix, (NJS_MATRIX_PTR md, NJS_MATRIX_CONST_PTR ms), 0x784D00);
FunctionPointer(void, njTranslateV_, (NJS_MATRIX_PTR m, const NJS_VECTOR *vd), 0x784D20);
FunctionPointer(void, njTranslate_, (NJS_MATRIX_PTR m, float x, float y, float z), 0x784D30);
FunctionPointer(void, njUnitMatrix_, (NJS_MATRIX_PTR a2), 0x784D50);
FastcallFunctionPointer(void, njCalcVector, (NJS_MATRIX_PTR m, const NJS_VECTOR *vs, NJS_VECTOR *vd), 0x784D60);
FastcallFunctionPointer(void, njRotateY, (NJS_MATRIX_PTR m, Angle ang), 0x784E20);
FastcallFunctionPointer(void, njRotateX, (NJS_MATRIX_PTR m, Angle ang), 0x784EE0);
FastcallFunctionPointer(void, njRotateZ, (NJS_MATRIX_PTR m, Angle ang), 0x784FB0);
FastcallFunctionPointer(void, njTranslateV, (NJS_MATRIX_PTR m, const NJS_VECTOR *v), 0x785070);
ThiscallFunctionPointer(void, njPushMatrix, (NJS_MATRIX_PTR m), 0x7850F0);
ThiscallFunctionPointer(void, njPopMatrix, (Uint32 n), 0x785140);
FastcallFunctionPointer(void, njAddVector, (NJS_VECTOR *vd, const NJS_VECTOR *vs), 0x785170);
FunctionPointer(Sint32, pdVibMxStart, (Uint32 port, const PDS_VIBPARAM *a2), 0x785280);
FunctionPointer(Sint32, pdVibMxStop, (Uint32 port), 0x785330);
FunctionPointer(void, meshset_basic_strip, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x785440);
FunctionPointer(void, meshset_basic_tri, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x785670);
FunctionPointer(void, meshset_basic_quad, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x785760);
FunctionPointer(void, meshset_uv_strip, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x785940);
FunctionPointer(void, meshset_uv_tri, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x785C70);
FunctionPointer(void, meshset_uv_quad, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x785DE0);
FunctionPointer(void, meshset_normal_strip, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x7860A0);
FunctionPointer(void, meshset_normal_tri, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x786390);
FunctionPointer(void, meshset_normal_quad, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x7864D0);
FunctionPointer(void, meshset_uv_normal_strip, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x786700);
FunctionPointer(void, meshset_uv_normal_tri, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x786B40);
FunctionPointer(void, meshset_uv_normal_quad, (NJS_MESHSET_SADX *meshset, NJS_POINT3 *points, NJS_VECTOR *normals), 0x786D00);
FunctionPointer(void, InitLandTableMeshSet, (NJS_MODEL_SADX *model, NJS_MESHSET_SADX *meshset), 0x787010);
FunctionPointer(void, FreeLandTableModel, (NJS_MODEL_SADX *a1), 0x7870A0);
FunctionPointer(void, FreeLandTableObject, (NJS_OBJECT *a1), 0x787100);
FunctionPointer(void, InitLandTableObject, (NJS_OBJECT *), 0x787140);
FastcallFunctionPointer(void, njSetTextureInfo, (NJS_TEXINFO *info, Uint16 *tex, Sint32 Type, Sint32 nWidth, Sint32 nHeight), 0x7871A0);
FastcallFunctionPointer(void, njSetTextureNameEx, (NJS_TEXNAME *texname, void *filename, void *texaddr, NJD_TEXATTR attr), 0x7871C0);
StdcallFunctionPointer(float, squareroot, (float), 0x7871F0);
VoidFunc(njEnableFog, 0x787220);
VoidFunc(njDisableFog, 0x787230);
FunctionPointer(void, njSetFogColor, (Uint32 c), 0x787240);
FunctionPointer(void, njSetFogTable, (float *fogtable), 0x787250);
FunctionPointer(void, njGenerateFogTable3, (float *fog, float n, float f), 0x787260);
ThiscallFunctionPointer(void, njUnitMatrix, (NJS_MATRIX_PTR m), 0x7875F0);
FastcallFunctionPointer(void, njSubVector, (NJS_VECTOR *vd, const NJS_VECTOR *vs), 0x787610);
FastcallFunctionPointer(Bool, njInvertMatrix, (NJS_MATRIX_PTR m), 0x787630);
FastcallFunctionPointer(float, njScalor, (const NJS_VECTOR *v), 0x787CC0);
FastcallFunctionPointer(float, njInnerProduct, (const NJS_VECTOR *v1, const NJS_VECTOR *v2), 0x787CF0);
FastcallFunctionPointer(float, njScalor2, (NJS_VECTOR *v), 0x787E10);
ThiscallFunctionPointer(Float, njTan, (Angle n), 0x787E30);
FunctionPointer(void, SetupScreen, (NJS_SCREEN *m), 0x788240);
FastcallFunctionPointer(void, njSetPvmTextureList, (NJS_TEXLIST *texlist, NJS_TEXNAME *texNames, char *fileNames, int length), 0x788260);
FastcallFunctionPointer(Float, njSin, (Angle n), 0x788290);
FastcallFunctionPointer(Float, njCos, (Angle n), 0x7883C0);
FastcallFunctionPointer(void, njGetTranslation, (NJS_MATRIX_PTR m, NJS_VECTOR *p), 0x7884F0);
FunctionPointer(void, njGetCameraMotion, (NJS_CAMERA *a1, NJS_MOTION *a2, NJS_CMOTION_DATA *a3, float a4), 0x788510);
FastcallFunctionPointer(void, njProjectScreen, (NJS_MATRIX_CONST_PTR m, const NJS_VECTOR *p3, NJS_POINT2 *p2), 0x788700);
FastcallFunctionPointer(void, njScaleV, (NJS_MATRIX_PTR m, const NJS_VECTOR *v), 0x788A50);
FastcallFunctionPointer(void, njMultiMatrix, (NJS_MATRIX_PTR md, NJS_MATRIX_CONST_PTR ms), 0x788AD0);
FastcallFunctionPointer(void, njRotate, (NJS_MATRIX_PTR m, const NJS_VECTOR *v, Angle ang), 0x788DF0);
FastcallFunctionPointer(void, njProject, (NJS_MATRIX *m, const NJS_PLANE *pl), 0x7893C0);
ThiscallFunctionPointer(void, njUnitTransPortion, (NJS_MATRIX_PTR _this), 0x789670);
FastcallFunctionPointer(void, TransformChunkObject, (NJS_CNK_OBJECT *a1), 0x789770);
FunctionPointer(void, DrawChunkObject, (NJS_CNK_OBJECT *a1), 0x789830);
VoidFunc(QueryPerformance, 0x789960);
FunctionPointer(void, DeltaTime_Multiplier, (int a1), 0x7899A0);
VoidFunc(FrameLimit, 0x7899E0);
VoidFunc(HandleWindowMessages, 0x789BD0);
FunctionPointer(BOOL, FocusWindow, (), 0x789C50);
StdcallFunctionPointer(WPARAM, InstanceMutex_Create, (HINSTANCE hinst, int a2, int a3, int hCmdShow), 0x789CB0);
ThiscallFunctionPointer(void, InstanceMutex_CloseA, (void *_this), 0x789D30);
ThiscallFunctionPointer(void, InstanceMutex_CloseB, (void *_this), 0x789D70);
FunctionPointer(int, Exit, (), 0x789DD0);
StdcallFunctionPointer(LRESULT, WndProc, (HWND handle, UINT Msg, WPARAM wParam, LPARAM lParam), 0x789DE0);
VoidFunc(Direct3D_SetChunkModelRenderState, 0x78A010);
VoidFunc(Direct3D_UnsetChunkModelRenderState, 0x78A0C0);
VoidFunc(EnableChunkMaterialFlags, 0x78A2D0);
VoidFunc(DisableChunkMaterialFlags, 0x78A2E0);
FunctionPointer(void, SetChunkMaterialFlags, (Uint32 flag), 0x78A2F0);
FunctionPointer(void, SetChunkTextureIndexA, (Uint32 a1), 0x78A300);
FunctionPointer(void, SetChunkTextureIndexB, (Uint32 index), 0x78A310);
FunctionPointer(Sint32, njCnkDrawModel, (NJS_CNK_MODEL *model), 0x78AA10);
FunctionPointer(void, njCnkDrawObject_, (NJS_CNK_OBJECT *a1), 0x78AA90);
FunctionPointer(void, njCnkDrawObject, (NJS_CNK_OBJECT *a1), 0x78AB80);
FunctionPointer(__int16, GetChunkTextureIndex, (NJS_CNK_MODEL *a1), 0x78ABE0);
FunctionPointer(void, GetChaoHatModel, (NJS_CNK_MODEL *a1, int a2), 0x78AF10);
FunctionPointer(void, DisplayDebugShape_, (int color, float a2, float a3, float a4, float a5, float depth_maybe), 0x78B4C0);
FastcallFunctionPointer(void, njSubMatrix, (NJS_MATRIX_PTR md, NJS_MATRIX_CONST_PTR ms), 0x78B6E0);
VoidFunc(Direct3D_TextureFilterPoint, 0x78B780);
VoidFunc(Direct3D_TextureFilterLinear, 0x78B800);
VoidFunc(Direct3D_DiffuseSourceVertexColor, 0x78B840);
VoidFunc(Direct3D_BeginScene, 0x78B880);
VoidFunc(Direct3D_EndScene, 0x78B960);
VoidFunc(Direct3D_Present, 0x78BA30);
VoidFunc(Direct3D_Clear, 0x78BA50);
FunctionPointer(int, CompareOSVersionInfoForSomeReason, (), 0x78BAB0);
VoidFunc(j_DeltaSleep, 0x78BAC0);
FunctionPointer(HRESULT, Direct3D_SetNullTexture, (), 0x78BBA0);
FastcallFunctionPointer(void, ChunkTextureFlip, (__int16 a1), 0x78BBD0);
ThiscallFunctionPointer(HRESULT, Direct3D_SetNullTextureStage, (DWORD stage), 0x78CB40);
ThiscallFunctionPointer(void, njReleaseTextureLow, (NJS_TEXMEMLIST *texmemlist), 0x78CD50);
FunctionPointer(void, Direct3D_ProbablyAppliesPalette, (NJS_TEXMEMLIST *), 0x78CDC0);
ThiscallFunctionPointer(int, Direct3D_SetNJSTexture, (NJS_TEXMEMLIST *t), 0x78CF20);
ThiscallFunctionPointer(void, DoSomethingWithPalette, (NJS_TEXPALETTE *p), 0x78CF80);
FastcallFunctionPointer(void, Direct3D_SetTextureNum, (int index), 0x78D140);
ThiscallFunctionPointer(void, Direct3D_SetHudColor, (NJS_COLOR color), 0x78D3E0);
ThiscallFunctionPointer(void, njDrawLine3D_Direct3D, (NJS_POINT3COL *_this), 0x78D3F0);
FunctionPointer(DWORD, DeviceCaps_CheckTexCoordCaps, (), 0x78DB80);
FastcallFunctionPointer(void, njSetCnkBlendMode, (Uint32 attr), 0x78DB90);
StdcallFunctionPointer(float, DoWeirdProjectionThings, (float a1), 0x78DBE0);
FastcallFunctionPointer(int, Direct3D_EnableLighting, (unsigned __int8 a1), 0x78E700);
FunctionPointer(int, InitPolyBuffers, (int _32, int count, void *a3), 0x78E720);
VoidFunc(FreePolyBuffers, 0x78E7A0);
FastcallFunctionPointer(void, DrawRect_TextureVertexTriangleStrip, (NJS_TEXTURE_VTX *points, signed int count), 0x78E7F0);
ThiscallFunctionPointer(void, njDrawLine2D_Direct3D, (NJS_POINT2COL *_this, float depth), 0x78E8C0);
ThiscallFunctionPointer(void, njDrawTriangle2D_List, (NJS_POINT2COL *_this, float depth), 0x78E960);
ThiscallFunctionPointer(void, Direct3D_DrawTriangleFan2D, (NJS_POINT2COL *_this, float depth), 0x78EA10);
ThiscallFunctionPointer(void, njDrawTriangle2D_Strip, (NJS_POINT2COL *_this, float depth), 0x78EAB0);
ThiscallFunctionPointer(void, ParseNjControl3D, (Sint16 *_this), 0x78EB50);
StdcallFunctionPointer(void, Direct3D_SetProjectionMatrix, (float hfov, float NearPlane, float FarPlane), 0x791170);
VoidFunc(Direct3D_SetViewportAndTransform, 0x7912E0);
VoidFunc(InitDirect3D8, 0x791610);
VoidFunc(Direct3D_SetDefaultRenderState, 0x791860);
ThiscallFunctionPointer(void, njAlphaMode, (Int mode), 0x791940);
ThiscallFunctionPointer(void, njTextureShadingMode, (NJD_TEX_SHADING mode), 0x791990);
VoidFunc(ProjectToWorldSpace, 0x791A50);
VoidFunc(Direct3D_SetWorldTransform, 0x791AB0);
VoidFunc(Direct3D_ResetWorldTransform, 0x791AD0);
VoidFunc(PollKeyboard, 0x791BD0);
FunctionPointer(void, CreateMouseDevice, (int a1, int a2, int a3, int a4), 0x791D60);
VoidFunc(DirectInput_ReleaseDevice, 0x791F40);
VoidFunc(DirectInput_Release, 0x7920D0);
FunctionPointer(void, DirectInput_Init, (ControllerData *a1, int a2), 0x7928E0);
FunctionPointer(Uint8 *, LoadPVx, (const char *str), 0x7929D0);
FunctionPointer(void, j__HeapFree_0, (LPVOID lpMem), 0x792A70);
ThiscallFunctionPointer(Uint8 *, LoadPVM_Seek, (void *data), 0x792A90);
FastcallFunctionPointer(void, njRotateEx, (const Angle *ang, Sint32 use_zxy), 0x7931A0);
FunctionPointer(int, QueueDebugMessage, (), 0x793A80);
ThiscallFunctionPointer(void, ProbablyDrawDebugText, (int a1), 0x793CC0);
VoidFunc(InitDirect3D8_, 0x793D80);
VoidFunc(Direct3D_EndScene_Release, 0x793D90);
FunctionPointer(signed int, GetTimeMaybe, (time_t *a1), 0x793DA0);
FunctionPointer(char *, GetWindowClassName, (), 0x793F60);
VoidFunc(UseHighPrecisionZBuffer, 0x794050);
FunctionPointer(Sint32, Direct3D_SetupPresentParameters, (D3DFORMAT *backBufferFormat, D3DFORMAT *depthBufferFormat), 0x794060);
VoidFunc(Direct3D_CheckAdapters, 0x7941D0);
VoidFunc(SetDisplayAdapterMaybe, 0x7943C0);
VoidFunc(Direct3D_SetDefaultTextureStageState, 0x7943F0);
VoidFunc(Direct3D_EnvironmentMap, 0x7944B0);
VoidFunc(Direct3D_ResetTextureTransform, 0x794500);
ThiscallFunctionPointer(void, MeshSetBuffer_Allocate, (NJS_MESHSET_SADX *_this), 0x794840);
ThiscallFunctionPointer(void, MeshSetBuffer_Free, (NJS_MESHSET_SADX *_this), 0x7948C0);
VoidFunc(Direct3D_EnableFog, 0x794950);
VoidFunc(Direct3D_DisableFog, 0x794980);
FunctionPointer(void, njSetFogColor_, (Uint32 c), 0x7949B0);
VoidFunc(Direct3D_ApplyFogTable, 0x7949F0);
FunctionPointer(void, SetFogDensityMaybe, (unsigned __int16 n), 0x794DB0);
FunctionPointer(void, njSetFogTable_, (const float *fogtable), 0x794E40);
VoidFunc(Direct3D_ConfigureFog, 0x794EC0);
FastcallFunctionPointer(float, njDetMatrix, (NJS_MATRIX_PTR m), 0x795000);
FunctionPointer(int, GetD3DCapThing, (int), 0x795930);
FunctionPointer(void, Set_FVFG_NormalX, (float a1), 0x795AB0);
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
VoidFunc(j_StopMusic, 0x79E1F0);
FunctionPointer(void, QueueSound_V, (int id, EntityData1 *entity, NJS_VECTOR *a3, NJS_VECTOR *a4), 0x79E450);
ObjectFunc(SpringH_Display, 0x79F650);
ObjectFunc(SpringH_Main, 0x79F710);
ObjectFunc(SpringH_Load, 0x79F860);
ObjectFunc(BoaBoa_Main, 0x7A0330);
ObjectFunc(RhinoTank_Display, 0x7A1120);
ObjectFunc(RhinoTank_Main, 0x7A1380);
ObjectFunc(TikalHint_Main_1, 0x7A15A0);
ObjectFunc(TikalHint_Main_2, 0x7A1610);
ObjectFunc(TikalHint_Main_3, 0x7A1770);
ObjectFunc(TikalHint_Main_4, 0x7A17F0);
ObjectFunc(TikalHint_Delete, 0x7A1910);
ObjectFunc(TikalHint_Load, 0x7A1AA0);
ObjectFunc(TikalHint_Main_5, 0x7A1C90);
ObjectFunc(TikalHint_Main, 0x7A1CD0);
ObjectFunc(Balloon_Display, 0x7A1E10);
ObjectFunc(Balloon_Main, 0x7A21C0);
FunctionPointer(float, DashHoop_LookAtDestination, (ObjectMaster *a1), 0x7A2280);
ObjectFunc(DashHoop_Display, 0x7A2470);
FunctionPointer(bool, DashHoop_LoadChild, (ObjectMaster *a1), 0x7A2660);
ObjectFunc(DashHoop_Main, 0x7A26F0);
ObjectFunc(Weed_Main, 0x7A2B60);
ObjectFunc(Spikes_Main, 0x7A30E0);
ObjectFunc(FallingSpikeBall_Main, 0x7A3A50);
ObjectFunc(FallingSpikeBall_Load, 0x7A3E50);
ObjectFunc(SwingSpikeBall_Load, 0x7A4260);
ObjectFunc(DashPanel_Display, 0x7A4360);
ObjectFunc(DashPanel_Main, 0x7A4450);
ObjectFunc(Spring_Display, 0x7A4980);
ObjectFunc(SpringB_Display, 0x7A4AE0);
ObjectFunc(Spring_Main, 0x7A4C30);
ObjectFunc(SpringB_Main, 0x7A4E50);
ObjectFunc(Froggy_Display, 0x7A6BB0);
ObjectFunc(Froggy_Main, 0x7A7AD0);
ObjectFunc(Bubbles_Main, 0x7A8A60);
ObjectFunc(Bubble_Main, 0x7A8E50);
ObjectFunc(KnucklesHint_Main, 0x7A9140);
ObjectFunc(HintMonitor_Display, 0x7A9460);
ObjectFunc(HintMonitor_Delete, 0x7A9620);
ObjectFunc(HintMonitor_Main, 0x7A9B40);
ObjectFunc(HintBox, 0x7A9C60);
ObjectFunc(Sweep_Display, 0x7AA3D0);
ObjectFunc(Sweep_Main, 0x7AA870);
ObjectFunc(Sweep_Load, 0x7AA960);
FunctionPointer(void, InitOceanData, (int n), 0x7AAAA0);
ObjectFunc(RingLine_Main, 0x7ABE90);
ObjectFunc(RingLineV_Main, 0x7AC4F0);
ObjectFunc(OSampleS, 0x7AC7B0);
ObjectFunc(OSampleM, 0x7AC810);
ObjectFunc(OSampleL, 0x7AC870);
ObjectFunc(OSampleSw, 0x7AC8D0);
FunctionPointer(Sint32, ProcessNBFile, (void *a1), 0x7B0260);
ObjectFunc(j_ClipSetObject, 0x7B0630);
ObjectFunc(Zero_Main, 0x7B0640);
ObjectFunc(Zero_Load, 0x7B0A70);
ObjectFunc(CScenechanger, 0x7B0C80);
ObjectFunc(OClrobj, 0x7B0DA0);
ObjectFunc(OCartStopper, 0x7B0EC0);
ObjectFunc(j_ObjectSelectedDebug, 0x7B14F0);
ObjectFunc(EBuyon, 0x7B2E00);
ObjectFunc(Tikal_Delete, 0x7B32C0);
ObjectFunc(Tikal_Display, 0x7B33A0);
ObjectFunc(Tikal_Main, 0x7B40C0);
ObjectFunc(Eggman_Delete, 0x7B4420);
ObjectFunc(Eggman_Main, 0x7B4EF0);
FunctionPointer(void, DebugSetMute_wrapper, (int a1), 0x7B7798);
VoidFunc(InitKnucklesWeldInfo, 0x7C94D0);
VoidFunc(InitSonicAnimData, 0x7CEC90);
VoidFunc(Sonic_SetLeftStretchyFeet, 0x7D0A90);
VoidFunc(Sonic_SetRightStretchyFeet, 0x7D0AC0);
VoidFunc(InitSonicWeldInfo, 0x7D0B50);
VoidFunc(InitEC0NPCData, 0x7D2720);
VoidFunc(InitEC3NPCData, 0x7D2CF0);
VoidFunc(InitSSNPCData, 0x7D5810);
VoidFunc(al_xy_init, 0x7DAE80);

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

// void __usercall(QueuedModelNode *node@<esi>, float pri, int idk)
static const void *const AddToQueuePtr = (void*)0x403E80;
static inline void AddToQueue(QueuedModelNode *node, float pri, int idk)
{
	__asm
	{
		push [idk]
		push [pri]
		mov esi, [node]
		call AddToQueuePtr
		add esp, 8
	}
}

// QueuedModelNode *__usercall@<eax>(__int16 texnum@<ax>, int size, QueuedModelType type, QueuedModelFlagsB flags)
static const void *const AllocateQueuedModelPtr = (void*)0x403F60;
static inline QueuedModelNode * AllocateQueuedModel(__int16 texnum, int _size, QueuedModelType _type, QueuedModelFlagsB flags)
{
	QueuedModelNode * result;
	__asm
	{
		push dword ptr [flags]
		push dword ptr [_type]
		push [_size]
		mov ax, [texnum]
		call AllocateQueuedModelPtr
		add esp, 12
		mov result, eax
	}
	return result;
}

// signed int __usercall@<eax>(QueuedModelLineB *q@<ebx>, NJS_POINT2COL *p, int count, float pri, NJD_DRAW attr)
static const void *const Draw2DLinesMaybe_InitPtr = (void*)0x404030;
static inline signed int Draw2DLinesMaybe_Init(QueuedModelLineB *q, NJS_POINT2COL *p, int count, float pri, NJD_DRAW attr)
{
	signed int result;
	__asm
	{
		push [attr]
		push [pri]
		push [count]
		push [p]
		mov ebx, [q]
		call Draw2DLinesMaybe_InitPtr
		add esp, 16
		mov result, eax
	}
	return result;
}

// void __usercall(NJS_ACTION *action@<eax>, float frameNumber, QueuedModelFlagsB flags, float scale, void (__cdecl *callback)(NJS_MODEL_SADX *, int, int))
static const void *const DisplayAnimationFramePtr = (void*)0x4053D0;
static inline void DisplayAnimationFrame(NJS_ACTION *action, float frameNumber, QueuedModelFlagsB flags, float scale, void (__cdecl *callback)(NJS_MODEL_SADX *, int, int))
{
	__asm
	{
		push [callback]
		push [scale]
		push dword ptr [flags]
		push [frameNumber]
		mov eax, [action]
		call DisplayAnimationFramePtr
		add esp, 16
	}
}

// void __usercall(int *act@<edi>, int *level@<ebx>)
static const void *const TimeOfDayIdPtr = (void*)0x40A420;
static inline void TimeOfDayId(int *act, int *level)
{
	__asm
	{
		mov ebx, [level]
		mov edi, [act]
		call TimeOfDayIdPtr
	}
}

// PaletteLight *__usercall@<eax>(int level, int act, int type@<eax>)
static const void *const GetLSPalettePtr = (void*)0x40AA30;
static inline PaletteLight * GetLSPalette(int level, int act, int _type)
{
	PaletteLight * result;
	__asm
	{
		mov eax, [_type]
		push [act]
		push [level]
		call GetLSPalettePtr
		add esp, 8
		mov result, eax
	}
	return result;
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

// void __usercall(EntityData1 *data1@<ecx>, EntityData1 *target@<edx>, float distance)
static const void *const UpdateTargetsPtr = (void*)0x418BE0;
static inline void UpdateTargets(EntityData1 *data1, EntityData1 *target, float distance)
{
	__asm
	{
		push [distance]
		mov edx, [target]
		mov ecx, [data1]
		call UpdateTargetsPtr
		add esp, 4
	}
}

// signed int __usercall@<eax>(EntityData1 *a@<edi>, EntityData1 *b@<esi>)
static const void *const CheckCollidePtr = (void*)0x418D00;
static inline signed int CheckCollide(EntityData1 *a, EntityData1 *b)
{
	signed int result;
	__asm
	{
		mov esi, [b]
		mov edi, [a]
		call CheckCollidePtr
		mov result, eax
	}
	return result;
}

// void __usercall(int act@<ecx>, const char *FileName)
static const void *const LoadCamFilePtr = (void*)0x4224F0;
static inline void LoadCamFile(int act, const char *FileName)
{
	__asm
	{
		push [FileName]
		mov ecx, [act]
		call LoadCamFilePtr
		add esp, 4
	}
}

// void __usercall(int act@<ecx>, const char *FileName)
static const void *const LoadSetFilePtr = (void*)0x422930;
static inline void LoadSetFile(int act, const char *FileName)
{
	__asm
	{
		push [FileName]
		mov ecx, [act]
		call LoadSetFilePtr
		add esp, 4
	}
}

// Sint32 __usercall@<eax>(int a1@<edi>)
static const void *const SoundQueue_GetFreeIndexPtr = (void*)0x423D10;
static inline Sint32 SoundQueue_GetFreeIndex(int a1)
{
	Sint32 result;
	__asm
	{
		mov edi, [a1]
		call SoundQueue_GetFreeIndexPtr
		mov result, eax
	}
	return result;
}

// int __usercall@<eax>(int a1@<edi>, void *a2@<esi>)
static const void *const SoundQueue_GetOtherThingPtr = (void*)0x423EB0;
static inline int SoundQueue_GetOtherThing(int a1, void *a2)
{
	int result;
	__asm
	{
		mov esi, [a2]
		mov edi, [a1]
		call SoundQueue_GetOtherThingPtr
		mov result, eax
	}
	return result;
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

// bool __usercall@<eax>(int level@<eax>, int character@<ecx>, int mission@<edx>)
static const void *const CheckMissionRequirementsSubgamePtr = (void*)0x4282D0;
static inline bool CheckMissionRequirementsSubgame(int level, int character, int mission)
{
	bool result;
	__asm
	{
		mov edx, [mission]
		mov ecx, [character]
		mov eax, [level]
		call CheckMissionRequirementsSubgamePtr
		mov result, al
	}
	return result;
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

// void __usercall(PVMEntry *pvm@<eax>)
static const void *const EV_LoadPvmPtr = (void*)0x42F940;
static inline void EV_LoadPvm(PVMEntry *pvm)
{
	__asm
	{
		mov eax, [pvm]
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

// int __usercall@<ecx>(_DWORD@<eax>)
static const void *const SetScreenTextureUVsPtr = (void*)0x433060;
static inline int SetScreenTextureUVs(_DWORD)
{
	int result;
	__asm
	{
		mov eax, [_DWORD]
		call SetScreenTextureUVsPtr
		mov result, ecx
	}
	return result;
}

// Bool __usercall@<eax>(EntityData1 *a1@<eax>)
static const void *const Camera_RunFreeCamPtr = (void*)0x434D80;
static inline Bool Camera_RunFreeCam(EntityData1 *a1)
{
	Bool result;
	__asm
	{
		mov eax, [a1]
		call Camera_RunFreeCamPtr
		mov result, eax
	}
	return result;
}

// void __usercall(EntityData1 *eax0@<eax>)
static const void *const Camera_Display_Ptr = (void*)0x435600;
static inline void Camera_Display_(EntityData1 *eax0)
{
	__asm
	{
		mov eax, [eax0]
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

// void __usercall(NJS_OBJECT *a1@<edi>, COL *a2)
static const void *const DrawLandTableObjectPtr = (void*)0x43A570;
static inline void DrawLandTableObject(NJS_OBJECT *a1, COL *a2)
{
	__asm
	{
		push [a2]
		mov edi, [a1]
		call DrawLandTableObjectPtr
		add esp, 4
	}
}

// void __usercall(CharObj2 *eax0@<eax>, EntityData2 *a2@<ebx>, EntityData1 *a3@<edi>)
static const void *const FollowSplinePtr = (void*)0x43C130;
static inline void FollowSpline(CharObj2 *eax0, EntityData2 *a2, EntityData1 *a3)
{
	__asm
	{
		mov edi, [a3]
		mov ebx, [a2]
		mov eax, [eax0]
		call FollowSplinePtr
	}
}

// CharObj2 *__usercall@<eax>(CharObj2 *result@<eax>, EntityData1 *a2@<ecx>, ColFlags colFlags@<ebx>, EntityData2 *a4@<esi>)
static const void *const HandleSurfaceFlagsPtr = (void*)0x43CB90;
static inline CharObj2* HandleSurfaceFlags(CharObj2 *result, EntityData1 *a2, ColFlags colFlags, EntityData2 *a4)
{
	CharObj2* result_;
	__asm
	{
		mov esi, [a4]
		mov ebx, [colFlags]
		mov ecx, [a2]
		mov eax, [result]
		call HandleSurfaceFlagsPtr
		mov result_, eax
	}
	return result_;
}

// void __usercall(ObjectMaster *eax0@<eax>, int pid)
static const void *const PlayCharacterDeathSoundPtr = (void*)0x446AF0;
static inline void PlayCharacterDeathSound(ObjectMaster *eax0, int pid)
{
	__asm
	{
		push [pid]
		mov eax, [eax0]
		call PlayCharacterDeathSoundPtr
		add esp, 4
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

// void __usercall(EntityData2 *a1@<ebx>, CharObj2 *a2@<edi>, EntityData1 *a3@<esi>)
static const void *const Tails_LoadAttackEffectPtr = (void*)0x45B180;
static inline void Tails_LoadAttackEffect(EntityData2 *a1, CharObj2 *a2, EntityData1 *a3)
{
	__asm
	{
		mov esi, [a3]
		mov edi, [a2]
		mov ebx, [a1]
		call Tails_LoadAttackEffectPtr
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

// Bool __usercall@<eax>(NJS_VECTOR *from@<ecx>, float x, float y, float z, float range)
static const void *const ObjectInRangePtr = (void*)0x46B750;
static inline Bool ObjectInRange(NJS_VECTOR *from, float x, float y, float z, float range)
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

// void __usercall(CharObj2 *a1@<eax>)
static const void *const Knuckles_UpgradesPtr = (void*)0x4726A0;
static inline void Knuckles_Upgrades(CharObj2 *a1)
{
	__asm
	{
		mov eax, [a1]
		call Knuckles_UpgradesPtr
	}
}

// void __usercall(EntityData1 *a1@<ebx>, CharObj2 *a2@<esi>)
static const void *const Knuckles_Something_APtr = (void*)0x472D60;
static inline void Knuckles_Something_A(EntityData1 *a1, CharObj2 *a2)
{
	__asm
	{
		mov esi, [a2]
		mov ebx, [a1]
		call Knuckles_Something_APtr
	}
}

// void __usercall(CharObj2 *a1@<ecx>, EntityData2 *a2@<edx>, EntityData1 *a3)
static const void *const Knuckles_Something_BPtr = (void*)0x472F80;
static inline void Knuckles_Something_B(CharObj2 *a1, EntityData2 *a2, EntityData1 *a3)
{
	__asm
	{
		push [a3]
		mov edx, [a2]
		mov ecx, [a1]
		call Knuckles_Something_BPtr
		add esp, 4
	}
}

// void __usercall(EntityData1 *data1@<edi>, CharObj2 *a2@<esi>, EntityData2 *a3)
static const void *const Knuckles_Something_CPtr = (void*)0x4730D0;
static inline void Knuckles_Something_C(EntityData1 *data1, CharObj2 *a2, EntityData2 *a3)
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

// void __usercall(EntityData1 *a1@<edi>, CharObj2 *a2@<esi>, EntityData2 *a3)
static const void *const Knuckles_Something_DPtr = (void*)0x473350;
static inline void Knuckles_Something_D(EntityData1 *a1, CharObj2 *a2, EntityData2 *a3)
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

// void __usercall(ObjectMaster *a1@<eax>)
static const void *const TailsAI_WriteInputPtr = (void*)0x47E290;
static inline void TailsAI_WriteInput(ObjectMaster *a1)
{
	__asm
	{
		mov eax, [a1]
		call TailsAI_WriteInputPtr
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

// Bool __usercall@<eax>(EntityData1 *a1@<esi>, CharObj2 *a2@<edi>)
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

// Bool __usercall@<eax>(CharObj2 *a1@<edi>, EntityData1 *a2@<esi>)
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

// void __usercall(CharObj2 *a1@<esi>)
static const void *const Sonic_MorphStretchyFeetPtr = (void*)0x493500;
static inline void Sonic_MorphStretchyFeet(CharObj2 *a1)
{
	__asm
	{
		mov esi, [a1]
		call Sonic_MorphStretchyFeetPtr
	}
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

// Sint32 __usercall@<eax>(NJS_OBJECT **a1@<edi>, char a2)
static const void *const CollisionCrashThing_LoadPtr = (void*)0x495860;
static inline Sint32 CollisionCrashThing_Load(NJS_OBJECT **a1, char a2)
{
	Sint32 result;
	__asm
	{
		push dword ptr [a2]
		mov edi, [a1]
		call CollisionCrashThing_LoadPtr
		add esp, 4
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

// void __usercall(CharObj2 *a1@<esi>)
static const void *const Sonic_JumpPadAniPtr = (void*)0x495C80;
static inline void Sonic_JumpPadAni(CharObj2 *a1)
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

// void __usercall(ObjectMaster *a1@<eax>)
static const void *const CharBubble_Init_Ptr = (void*)0x4A2AE0;
static inline void CharBubble_Init_(ObjectMaster *a1)
{
	__asm
	{
		mov eax, [a1]
		call CharBubble_Init_Ptr
	}
}

// void __usercall(NJS_VECTOR *pos@<edi>, NJS_SPRITE *_sp@<esi>, NJS_ARGB *color, Int n, float scale)
static const void *const DrawParticlePtr = (void*)0x4A3530;
static inline void DrawParticle(NJS_VECTOR *pos, NJS_SPRITE *_sp, NJS_ARGB *color, Int n, float scale)
{
	__asm
	{
		push [scale]
		push [n]
		push [color]
		mov esi, [_sp]
		mov edi, [pos]
		call DrawParticlePtr
		add esp, 12
	}
}

// void __usercall(NJS_VECTOR *a1@<eax>, ObjectMaster *a2@<edi>, float a3, int a4)
static const void *const DrawParticleSomeMoreProbablyPtr = (void*)0x4A3800;
static inline void DrawParticleSomeMoreProbably(NJS_VECTOR *a1, ObjectMaster *a2, float a3, int a4)
{
	__asm
	{
		push [a4]
		push [a3]
		mov edi, [a2]
		mov eax, [a1]
		call DrawParticleSomeMoreProbablyPtr
		add esp, 8
	}
}

// void __usercall(EntityData1 *a1@<eax>, ObjectData2 *a2@<esi>)
static const void *const Leon_Display_B_BPtr = (void*)0x4A6D90;
static inline void Leon_Display_B_B(EntityData1 *a1, ObjectData2 *a2)
{
	__asm
	{
		mov esi, [a2]
		mov eax, [a1]
		call Leon_Display_B_BPtr
	}
}

// void __usercall(EntityData1 *a1@<eax>, ObjectData2 *a2@<esi>)
static const void *const Leon_Display_A_BPtr = (void*)0x4A7610;
static inline void Leon_Display_A_B(EntityData1 *a1, ObjectData2 *a2)
{
	__asm
	{
		mov esi, [a2]
		mov eax, [a1]
		call Leon_Display_A_BPtr
	}
}

// int __usercall@<eax>(ObjectMaster *a1@<eax>)
static const void *const Leon_DebugPtr = (void*)0x4A7D20;
static inline int Leon_Debug(ObjectMaster *a1)
{
	int result;
	__asm
	{
		mov eax, [a1]
		call Leon_DebugPtr
		mov result, eax
	}
	return result;
}

// void __usercall(ObjectData2 *a1@<eax>, EntityData1 *a2@<ecx>)
static const void *const Leon_CheckPlayerDistancePtr = (void*)0x4A8040;
static inline void Leon_CheckPlayerDistance(ObjectData2 *a1, EntityData1 *a2)
{
	__asm
	{
		mov ecx, [a2]
		mov eax, [a1]
		call Leon_CheckPlayerDistancePtr
	}
}

// void __usercall(ObjectData2 *a1@<eax>, EntityData1 *a2@<ecx>)
static const void *const Leon_CheckVisiblePtr = (void*)0x4A8090;
static inline void Leon_CheckVisible(ObjectData2 *a1, EntityData1 *a2)
{
	__asm
	{
		mov ecx, [a2]
		mov eax, [a1]
		call Leon_CheckVisiblePtr
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

// void __usercall(EntityData1 *data1@<ebx>, ObjectData2 *data2)
static const void *const EPolice_DoSomethingPtr = (void*)0x4B2E90;
static inline void EPolice_DoSomething(EntityData1 *data1, ObjectData2 *data2)
{
	__asm
	{
		push [data2]
		mov ebx, [data1]
		call EPolice_DoSomethingPtr
		add esp, 4
	}
}

// ObjectMaster *__usercall@<eax>(NJS_VECTOR *a1@<edi>)
static const void *const EmblemCollected_LoadPtr = (void*)0x4B48E0;
static inline ObjectMaster * EmblemCollected_Load(NJS_VECTOR *a1)
{
	ObjectMaster * result;
	__asm
	{
		mov edi, [a1]
		call EmblemCollected_LoadPtr
		mov result, eax
	}
	return result;
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
static const void *const RocketH_InitPtr = (void*)0x4C9780;
static inline void RocketH_Init(ObjectMaster *a1)
{
	__asm
	{
		mov edi, [a1]
		call RocketH_InitPtr
	}
}

// void __usercall(ObjectMaster *a1@<eax>)
static const void *const RocketV_InitPtr = (void*)0x4C9830;
static inline void RocketV_Init(ObjectMaster *a1)
{
	__asm
	{
		mov eax, [a1]
		call RocketV_InitPtr
	}
}

// void __usercall(ObjectMaster *obj@<eax>, int type)
static const void *const DrawColObjPtr = (void*)0x4D42A0;
static inline void DrawColObj(ObjectMaster *obj, int _type)
{
	__asm
	{
		push [_type]
		mov eax, [obj]
		call DrawColObjPtr
		add esp, 4
	}
}

// signed int __usercall@<eax>(CollisionData *a1@<edi>, EntityData1 *esi0@<esi>)
static const void *const NoWater_IsPlayerInRangePtr = (void*)0x4D4C50;
static inline signed int NoWater_IsPlayerInRange(CollisionData *a1, EntityData1 *esi0)
{
	signed int result;
	__asm
	{
		mov esi, [esi0]
		mov edi, [a1]
		call NoWater_IsPlayerInRangePtr
		mov result, eax
	}
	return result;
}

// float __usercall@<st0>(int a1@<eax>, float x, float y, float pri)
static const void *const TwinkleCircuit_DrawCurrentLapPtr = (void*)0x4DC7D0;
static inline float TwinkleCircuit_DrawCurrentLap(int a1, float x, float y, float pri)
{
	float result;
	__asm
	{
		push [pri]
		push [y]
		push [x]
		mov eax, [a1]
		call TwinkleCircuit_DrawCurrentLapPtr
		add esp, 12
		fstp result
	}
	return result;
}

// void __usercall(int some_flags@<esi>)
static const void *const Obj_Icecap_DoColFlagThingsPtr = (void*)0x4E91C0;
static inline void Obj_Icecap_DoColFlagThings(int some_flags)
{
	__asm
	{
		mov esi, [some_flags]
		call Obj_Icecap_DoColFlagThingsPtr
	}
}

// void __usercall(ObjectMaster *obj@<eax>, NJS_OBJECT *node@<edi>, int a3)
static const void *const YASI0_Display_2Ptr = (void*)0x500CD0;
static inline void YASI0_Display_2(ObjectMaster *obj, NJS_OBJECT *node, int a3)
{
	__asm
	{
		push [a3]
		mov edi, [node]
		mov eax, [obj]
		call YASI0_Display_2Ptr
		add esp, 4
	}
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

// void __usercall(int a1@<esi>)
static const void *const CharacterSelectMenu_Display_BPtr = (void*)0x5119F0;
static inline void CharacterSelectMenu_Display_B(int a1)
{
	__asm
	{
		mov esi, [a1]
		call CharacterSelectMenu_Display_BPtr
	}
}

// void __usercall(int a1@<edi>)
static const void *const CharacterSelectMenu_Display_CPtr = (void*)0x511E10;
static inline void CharacterSelectMenu_Display_C(int a1)
{
	__asm
	{
		mov edi, [a1]
		call CharacterSelectMenu_Display_CPtr
	}
}

// int __usercall@<eax>(int a1@<esi>)
static const void *const CharacterSelectMenu_Display_DPtr = (void*)0x511E90;
static inline int CharacterSelectMenu_Display_D(int a1)
{
	int result;
	__asm
	{
		mov esi, [a1]
		call CharacterSelectMenu_Display_DPtr
		mov result, eax
	}
	return result;
}

// void __usercall(int a1@<esi>)
static const void *const CharacterSelectMenu_Display_EPtr = (void*)0x512110;
static inline void CharacterSelectMenu_Display_E(int a1)
{
	__asm
	{
		mov esi, [a1]
		call CharacterSelectMenu_Display_EPtr
	}
}

// void __usercall(ObjectMaster *a1@<eax>)
static const void *const CharSel_LoadSubObjectThingPtr = (void*)0x512AF0;
static inline void CharSel_LoadSubObjectThing(ObjectMaster *a1)
{
	__asm
	{
		mov eax, [a1]
		call CharSel_LoadSubObjectThingPtr
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

// void __usercall(EntityData1 *a2@<esi>)
static const void *const OSkydeck_Display_Ptr = (void*)0x51DE50;
static inline void OSkydeck_Display_(EntityData1 *a2)
{
	__asm
	{
		mov esi, [a2]
		call OSkydeck_Display_Ptr
	}
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

// void __usercall(ObjectMaster *obj@<eax>)
static const void *const SkyBox_MysticRuins_TimeOfDayLightDirectionPtr = (void*)0x52FB30;
static inline void SkyBox_MysticRuins_TimeOfDayLightDirection(ObjectMaster *obj)
{
	__asm
	{
		mov eax, [obj]
		call SkyBox_MysticRuins_TimeOfDayLightDirectionPtr
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

// void __usercall(NJS_OBJECT **a1@<ebx>, ObjectMaster *a2)
static const void *const OTenss_InitPtr = (void*)0x5CD200;
static inline void OTenss_Init(NJS_OBJECT **a1, ObjectMaster *a2)
{
	__asm
	{
		push [a2]
		mov ebx, [a1]
		call OTenss_InitPtr
		add esp, 4
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

// void __usercall(ObjectMaster *a1@<eax>)
static const void *const Obj_SkyDeck_ThingPtr = (void*)0x5EF6B0;
static inline void Obj_SkyDeck_Thing(ObjectMaster *a1)
{
	__asm
	{
		mov eax, [a1]
		call Obj_SkyDeck_ThingPtr
	}
}

// void __usercall(int n@<eax>)
static const void *const SkyDeckSkyThingRandomRotationPtr = (void*)0x5F2130;
static inline void SkyDeckSkyThingRandomRotation(int n)
{
	__asm
	{
		mov eax, [n]
		call SkyDeckSkyThingRandomRotationPtr
	}
}

// void __usercall(ObjectMaster *a1@<eax>)
static const void *const OMtKusa_ThingPtr = (void*)0x608150;
static inline void OMtKusa_Thing(ObjectMaster *a1)
{
	__asm
	{
		mov eax, [a1]
		call OMtKusa_ThingPtr
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

// void __usercall(ObjectMaster *a1@<eax>)
static const void *const TornadoTarget_MoveTargetWithinBoundsPtr = (void*)0x628970;
static inline void TornadoTarget_MoveTargetWithinBounds(ObjectMaster *a1)
{
	__asm
	{
		mov eax, [a1]
		call TornadoTarget_MoveTargetWithinBoundsPtr
	}
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

// void __usercall(ObjectMaster *a1@<eax>)
static const void *const Obj_StationSquare_SetOceanStuffPtr = (void*)0x62EEB0;
static inline void Obj_StationSquare_SetOceanStuff(ObjectMaster *a1)
{
	__asm
	{
		mov eax, [a1]
		call Obj_StationSquare_SetOceanStuffPtr
	}
}

// void __usercall(ChaoStatIndex index@<eax>, ChaoDataBase *data@<edi>, ChaoDNA *dna@<esi>)
static const void *const Chao_IncrementStatIndexPtr = (void*)0x71DEF0;
static inline void Chao_IncrementStatIndex(ChaoStatIndex index, ChaoDataBase *data, ChaoDNA *dna)
{
	__asm
	{
		mov esi, [dna]
		mov edi, [data]
		mov eax, [index]
		call Chao_IncrementStatIndexPtr
	}
}

// void __usercall(ChaoHudThing *a1@<eax>)
static const void *const DrawChaoHud_WhitePtr = (void*)0x7448B0;
static inline void DrawChaoHud_White(ChaoHudThing *a1)
{
	__asm
	{
		mov eax, [a1]
		call DrawChaoHud_WhitePtr
	}
}

// void __usercall(int count@<eax>, NJS_POINT3COL *a2@<edx>)
static const void *const Point3ColToVertexBufferPtr = (void*)0x77E880;
static inline void Point3ColToVertexBuffer(int count, NJS_POINT3COL *a2)
{
	__asm
	{
		mov edx, [a2]
		mov eax, [count]
		call Point3ColToVertexBufferPtr
	}
}

// void __usercall(int count@<eax>, NJS_POINT3COL *p2col@<ecx>)
static const void *const Point3ColToVertexBuffer_UVPtr = (void*)0x77E8C0;
static inline void Point3ColToVertexBuffer_UV(int count, NJS_POINT3COL *p2col)
{
	__asm
	{
		mov ecx, [p2col]
		mov eax, [count]
		call Point3ColToVertexBuffer_UVPtr
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

// void __usercall(NJS_CNK_MODEL *a1)
static const void *const DrawCnkModelPtr = (void*)0x78AB50;
static inline void DrawCnkModel(NJS_CNK_MODEL *a1)
{
	__asm
	{
		push [a1]
		call DrawCnkModelPtr
		add esp, 4
	}
}

// void __usercall(int texture_index@<edx>, int texture_stage@<ecx>)
static const void *const Direct3D_SetTextureToStagePtr = (void*)0x78D380;
static inline void Direct3D_SetTextureToStage(int texture_index, int texture_stage)
{
	__asm
	{
		mov ecx, [texture_stage]
		mov edx, [texture_index]
		call Direct3D_SetTextureToStagePtr
	}
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

// void __usercall(NJS_VECTOR *a2@<ebx>, EntityData1 *a3@<esi>, signed int index)
static const void *const CalcRingPosLinePtr = (void*)0x79B140;
static inline void CalcRingPosLine(NJS_VECTOR *a2, EntityData1 *a3, signed int index)
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

// void __usercall(NJS_VECTOR *position@<ebx>, EntityData1 *parent@<esi>, signed int index)
static const void *const CalcRingPosCirclePtr = (void*)0x79B210;
static inline void CalcRingPosCircle(NJS_VECTOR *position, EntityData1 *parent, signed int index)
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

// void __usercall(EntityData1 *player@<eax>, ObjectData2 *a2@<ecx>, EntityData1 *me@<esi>)
static const void *const RhinoTank_DoSomethingPtr = (void*)0x7A0790;
static inline void RhinoTank_DoSomething(EntityData1 *player, ObjectData2 *a2, EntityData1 *me)
{
	__asm
	{
		mov esi, [me]
		mov ecx, [a2]
		mov eax, [player]
		call RhinoTank_DoSomethingPtr
	}
}

// void __usercall(EntityData1 *a1@<ebx>, ObjectData2 *a2@<edi>)
static const void *const RhinoTank_ScrollTreadUVsPtr = (void*)0x7A0CA0;
static inline void RhinoTank_ScrollTreadUVs(EntityData1 *a1, ObjectData2 *a2)
{
	__asm
	{
		mov edi, [a2]
		mov ebx, [a1]
		call RhinoTank_ScrollTreadUVsPtr
	}
}

// void __usercall(ObjectData2 *a1@<eax>, EntityData1 *a2@<ecx>)
static const void *const RhinoTank_AnimatePtr = (void*)0x7A0F70;
static inline void RhinoTank_Animate(ObjectData2 *a1, EntityData1 *a2)
{
	__asm
	{
		mov ecx, [a2]
		mov eax, [a1]
		call RhinoTank_AnimatePtr
	}
}

// void __usercall(EntityData1 *entity@<esi>)
static const void *const RhinoTank_DebugSelectedPtr = (void*)0x7A1200;
static inline void RhinoTank_DebugSelected(EntityData1 *entity)
{
	__asm
	{
		mov esi, [entity]
		call RhinoTank_DebugSelectedPtr
	}
}

// void __usercall(ObjectData2 *a1@<eax>, EntityData1 *a2@<ecx>)
static const void *const RhinoTank_CheckDeadPtr = (void*)0x7A12E0;
static inline void RhinoTank_CheckDead(ObjectData2 *a1, EntityData1 *a2)
{
	__asm
	{
		mov ecx, [a2]
		mov eax, [a1]
		call RhinoTank_CheckDeadPtr
	}
}

// void __usercall(unsigned int a1@<eax>, unsigned int a2@<edx>, int a3@<esi>)
static const void *const NBFile_ObjectPtr = (void*)0x7B0000;
static inline void NBFile_Object(unsigned int a1, unsigned int a2, int a3)
{
	__asm
	{
		mov esi, [a3]
		mov edx, [a2]
		mov eax, [a1]
		call NBFile_ObjectPtr
	}
}

// void __usercall(NBChunk *chunk@<eax>, NBChunk *next@<ebx>)
static const void *const NBFile_AnimationPtr = (void*)0x7B0150;
static inline void NBFile_Animation(NBChunk *chunk, NBChunk *next)
{
	__asm
	{
		mov ebx, [next]
		mov eax, [chunk]
		call NBFile_AnimationPtr
	}
}

// void __usercall(NBChunk *chunk@<eax>, NBChunk *next@<ebx>)
static const void *const NBFile_ModelPtr = (void*)0x7B01A0;
static inline void NBFile_Model(NBChunk *chunk, NBChunk *next)
{
	__asm
	{
		mov ebx, [next]
		mov eax, [chunk]
		call NBFile_ModelPtr
	}
}

// void __usercall(ObjectMaster *a1@<eax>)
static const void *const Zero_DebugSelectedPtr = (void*)0x7B04A0;
static inline void Zero_DebugSelected(ObjectMaster *a1)
{
	__asm
	{
		mov eax, [a1]
		call Zero_DebugSelectedPtr
	}
}

#endif /* SADXMODLOADER_SADXFUNCTIONS_H */
