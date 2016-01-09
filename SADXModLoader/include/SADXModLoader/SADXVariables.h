/**
 * SADX Mod Loader.
 * SADX variables.
 */

#ifndef SADXMODLOADER_SADXVARS_H
#define SADXMODLOADER_SADXVARS_H

#include "../ModLoader/MemAccess.h"
#include "../d3d8types.h"
#include "SADXStructs.h"

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
DataArray(PhysicsData_t, PhysicsArray, 0x9154E8, 8);
DataArray(PVMEntry *, CharSel_PVMNames, 0x10DB88C, 8);
DataPointer(int, GameMode, 0x3ABDC7C);
DataPointer(short, GameState, 0x03B22DE4);
DataPointer(NJS_VECTOR, Gravity, 0x3B0F0F8);
DataPointer(int, LevelFrameCount, 0x3B0F108);
DataPointer(int, FrameCounter, 0x3B0F13C);
DataPointer(int, FrameIncrement, 0x03B11178);
DataPointer(char, LastStoryFlag, 0x3B18DB4);
DataPointer(char, MetalSonicFlag, 0x3B18DB5);
DataPointer(short, CurrentCharacter, 0x3B22DC0);
DataPointer(short, CurrentLevel, 0x3B22DCC);
DataPointer(short, CurrentAct, 0x3B22DEC);
DataPointer(short, LastLevel, 0x03B22DD8);
DataPointer(short, LastAct, 0x03B22DD4);
DataPointer(short, NextLevel, 0x03B22DF0);
DataPointer(short, NextAct, 0x03B22E18);
DataPointer(short, ObjectPlacementDebug, 0x3B29D08);
DataPointer(RestartData, RestartLevel, 0x3B42F80);
DataArray(ObjectMaster*, PlayerPtrs, 0x03b42e10, 8);
DataArray(CharObj1*, CharObj1Ptrs, 0x03B42E10, 8);
DataArray(CharObj2*, CharObj2Ptrs, 0x03B3CDF0, 8);
DataPointer(short, Rings, 0x03B0F0E4);
DataPointer(int, MarketRings, 0x03B2C3FC);
DataPointer(int, CasinoRings, 0x03C74880);
DataPointer(char, Lives, 0x03B0EF34);
DataPointer(float, SkyChaseHealth, 0x03C82304);
DataPointer(char, HideHud, 0x03B0EF40);
DataPointer(int, CutscenePlaying, 0x3B2A2E4);
DataPointer(float,	DebugFontSize,	0x03CE7154);
DataPointer(int, DebugFontColor,	0x03CE715C);
DataPointer(Uint32, DisplayAdapter, 0x03D0FDFC);
// This is probably NJS_MATRIX and not D3DMATRIX, but they're equivalent in size so it doesn't matter.
DataPointer(D3DMATRIX, ProjectionMatrix, 0x03AAD0A0);
DataPointer(NJS_SCREEN, _nj_screen_, 0x03D0F9E0);
DataPointer(Angle, HorizontalFOV_BAMS, 0x03AB98EC);
DataPointer(Angle, LastHorizontalFOV_BAMS, 0x03B2CBB4);
DataPointer(NJS_SPRITE, VideoFrame, 0x03C600A4);
DataPointer(bool, LoadingFile, 0x3ABDF68);

// Raw input array. It's recommended you use ControllerPointers instead.
// This will prevent compatibility issues with mods that need to redirect input.
DataArray(ControllerData, ControllersRaw, 0x03B0E9C8, 4);
// Pointers to ControllersRaw. It's recommended that you use this over ControllersRaw directly.
DataArray(ControllerData*, ControllerPointers, 0x03B0E77C, 8);
DataArray(ControllerData, Controllers, 0x03B0E7F0, 8);
DataPointer(bool, RumbleEnabled, 0x00913B10);
// Indicates whether all controllers are enabled.
// It's recommended that you use the IsControlEnabled, EnableControl and DisableControl functions instead of
// using this directly to prevent compatibility issues with mods that redirect the variable.
DataPointer(bool, ControlEnabled, 0x00909FB0);
// Indicates whether the controller at the specified index is enabled.
// It's recommended you use the IsControllerEnabled, EnableController and DisableController functions instead of
// using this directly to prevent compatibility issues with mods that redirect the array.
DataArray(bool, ControllerEnabled, 0x00909FB4, 4);

// Buttons used for attacking.
DataPointer(int, AttackButtons, 0x00909F94);
// Buttons used for grabbing objects.
DataPointer(int, GrabButtons, 0x00909FA4);

#endif /* SADXMODLOADER_SADXVARS_H */
