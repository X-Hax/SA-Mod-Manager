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

// SADX Functions
FunctionPointer(void, PrintDebug, (const char *Format, ...), 0x401000);
VoidFunc(CheckSettings, 0x4025B0);
FunctionPointer(void, SetTextures, (NJS_TEXLIST *texturelist), 0x403070);
FunctionPointer(bool, CheckModelDistance, (Vector3 *center, float radius), 0x403330);
FunctionPointer(bool, CheckModelDistance2, (float x, float y, float z, float a4), 0x406F70);
FunctionPointer(void, ProcessModelNode, (NJS_OBJECT *obj, int a2, float a3), 0x4074A0);
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
FunctionPointer(int, SetCharacter, (int16_t character), 0x4144D0);
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
FunctionPointer(void, SetNextLevel, (uint16_t level), 0x414B00);
FunctionPointer(bool, IsGamePaused, (), 0x414D70);
VoidFunc(LoadLevelResults, 0x415540);
VoidFunc(LoadCharacter, 0x4157C0);
FunctionPointer(void, UnloadCharTextures, (int Character), 0x420E90);
FunctionPointer(int, UnloadLevelTextures, (int16_t levelact), 0x421040);
FunctionPointer(int, LoadPVM, (char *PVMName, NJS_TEXLIST *TexList), 0x421180);
VoidFunc(LoadObjectTextures, 0x4213A0);
FunctionPointer(void, LoadLevelTextures, (int16_t level), 0x4215B0);
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
FunctionPointer(void, PlayJingle, (int song), 0x00425860);
FunctionPointer(void, PlayVoice, (int a1), 0x425710);
ObjectFunc(SoundManager_Load, 0x425830);
FunctionPointer(bool, GetDebugMode, (), 0x4258F0);
VoidFunc(Set0Rings, 0x425AB0);
FunctionPointer(int, GetMarketRings, (), 0x425AC0);
FunctionPointer(int, ResetLives, (), 0x425AF0);
FunctionPointer(void, GiveLives, (int16_t lives), 0x425B60);
FunctionPointer(void, AddRings, (int16_t rings), 0x425BE0);
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
FunctionPointer(NJS_OBJECT *, ProcessAnimatedModelNode, (NJS_OBJECT *a1, NJS_MDATA3 *a2), 0x4B7D00);
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
FunctionPointer(void, SetDebugFontColor, (int a1), 0x7808E0);
FunctionPointer(void, SetDebugFontSize, (unsigned short size), 0x007808C0);
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
FunctionPointer(void, RenderSA2Model, (NJS_OBJECT *a1), 0x78AB80);
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
FastcallFunctionPointer(double, BAMSsin, (int bams), 0x00788290);
FastcallFunctionPointer(double, BAMScos, (int bams), 0x007883C0);
ThiscallFunctionPointer(double, BAMStan, (int bams), 0x00787E30);

#ifdef _MSC_VER

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

// void __usercall(int16_t a1)
static const void *const StartLevelCutscenePtr = (void*)0x413C90;
static inline void StartLevelCutscene(int16_t a1)
{
	__asm
	{
		push [a1]
		call StartLevelCutscenePtr
		add esp, 4
	}
}

// signed int __usercall<eax>(int a1<ebp>, char *a2, NJS_TEXLIST *a3)
static const void *const LoadPvmMEM2Ptr = (void*)0x421260;
static inline signed int LoadPvmMEM2(int a1, char *a2, NJS_TEXLIST *a3)
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

// uint16_t __usercall<ax>(byte *a2<esi>)
static const void *const CalcSaveChecksumPtr = (void*)0x42CF90;
static inline uint16_t CalcSaveChecksum(uint8_t *a2)
{
	uint16_t result;
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

// void __usercall(ObjectMaster *this<eax>, int16_t type)
static const void *const DrawColObjPtr = (void*)0x4D42A0;
static inline void DrawColObj(ObjectMaster *obj, int16_t _type)
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

#else
#warning Inline functions have not been ported to gcc asm yet.
#endif

#endif /* SADXMODLOADER_SADXFUNCTIONS_H */
