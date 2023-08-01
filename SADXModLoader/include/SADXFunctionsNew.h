/**
 * SADX Mod Loader.
 * SADX functions based on symbols.
 */

#ifndef SADXMODLOADER_SADXFUNCTIONSNEW_H
#define SADXMODLOADER_SADXFUNCTIONSNEW_H

#include <stdint.h>
#include "MemAccess.h"
#include "d3d8types.h"
#include "SADXStructsNew.h"
#include "SADXEnumsNew.h"

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

#define TaskFunc(NAME, ADDRESS) FunctionPointer(void,NAME,(task* tp),ADDRESS)
#define CamAdjustFunc(NAME, ADDRESS) FunctionPointer(void,NAME,(taskwk* twp,taskwk* ptwp,_OBJ_ADJUSTPARAM* adjwp),ADDRESS)
#define CamFunc(NAME, ADDRESS) FunctionPointer(void,NAME,(_OBJ_CAMERAPARAM* pParam),ADDRESS)

// General
FunctionPointer(int, InitGame, (), 0x413C00);
FunctionPointer(bool, ChkPause, (), 0x414D70); // Check if the game is paused
FunctionPointer(Bool, ChkGameMode, (), 0x414D90); // Check if in game
FunctionPointer(void*, CAlloc, (int count, int size), 0x0040B2D0); // Allocate memory
VoidFunc(Clear, 0x0040BF30);
VoidFunc(Reset, 0x0040BF40);
TaskFunc(LoopTaskC, 0x40B420); // Run all the children of a task
VoidFunc(DisplayTask, 0x40B540); // Call display function of all active objects
TaskFunc(DestroyTask, 0x40B570);
TaskFunc(FreeTask, 0x40B6C0);
TaskFunc(FreeTaskC, 0x40B7E0); // Free all the children of a task
FunctionPointer(task*, CreateElementalTask, (unsigned __int16 im, int level, void(__cdecl* exec)(task*)), 0x40B860);
FunctionPointer(task*, CreateChildTask, (unsigned __int16 im, void(__cdecl* exec)(task*), task* tp), 0x40B940);
VoidFunc(SetRoundMaster, 0x4143C0); // Load level task
VoidFunc(SetScrollTask, 0x414420); // Load skybox task
FunctionPointer(int, GetStageNumber, (), 0x414650); // Get stage and act number
FunctionPointer(void, AdvanceAct, (__int16 Gap), 0x415980); // Advance act number
FunctionPointer(signed int, NeonuLoadTexture, (NJS_TEXLIST* pTexlist), 0x4228E0);
FunctionPointer(signed int, late_ReleaseTexture, (NJS_TEXLIST* texlist), 0x00403290);
FunctionPointer(void, texLoadTexturePvmFile, (const char* filename, NJS_TEXLIST* texlist), 0x421180);
VoidFunc(ADX_Close, 0x425670); // Stop bgm
FunctionPointer(void, BGM_Play, (int song), 0x00425690);
VoidFunc(WakeTimer, 0x426030);
VoidFunc(SleepTimer, 0x426040);
VoidFunc(AdvanceTime, 0x426075);
FunctionPointer(void, SetViewAngle, (int new_view_angle), 0x437240);
FunctionPointer(void, LandChangeStage, (char Gap), 0x43A460); // Release landtable and request act change
FunctionPointer(void, AddCameraStage, (__int16 Gap), 0x434680); // Release cameras and request act change
VoidFunc(DrawMotionGround, 0x0043A810); // Draw landtable animations
FunctionPointer(void, LandChangeLandTable, (_OBJ_LANDTABLE* pOLT), 0x43A4C0); // Set Chao landtable
VoidFunc(InitFreeCamera, 0x434870);
FunctionPointer(Angle, AdjustAngle, (Angle ang0, Angle ang1, Angle dang), 0x438350); // Slowly adjust ang0 to ang1 at dang speed
FunctionPointer(Angle, SubAngle, (Angle ang0, Angle ang1), 0x4383A0); // Difference
FunctionPointer(Angle, DiffAngle, (Angle ang0, Angle ang1), 0x4383B0); // Positive difference
FunctionPointer(Angle, CreateNinjaAngle, (Angle ang0, Angle ang1), 0x4383D0);
FunctionPointer(Angle, VectorAngle, (NJS_POINT3* v1, NJS_POINT3* v2, NJS_POINT3* vn), 0x438410); // Calculate Y rotation between two points; "vn" is ground normal to reverse the angle if upside down (optional)
FunctionPointer(int, MSetPositionWIgnoreAttribute, (NJS_POINT3* p, NJS_POINT3* v, Angle3* a, int attrmask, float r), 0x439620); // Move position "p" with direction "v" and angle "a"; outputs in "v" and "a"; handles intersection with geo collisions in radius "r" except those whose attributes match attrmask; returns bitmask (1: touched a collision, 2: touched a collision upside down).
FunctionPointer(BOOL, MSetPosition, (NJS_POINT3* p, NJS_POINT3* v, Angle3* a, float r), 0x43A030); // Move position "p" with direction "v" and angle "a"; outputs in "v" and "a"; handles intersection with any geometry collision in radius "r"; returns whether it touched a collision or not.
FunctionPointer(BOOL, MSetPositionIgnoreAttribute, (NJS_POINT3* p, NJS_POINT3* v, Angle3* a, int attrmask, float r), 0x43A060); // Move position "p" with direction "v" and angle "a"; outputs in "v" and "a"; handles intersection with geo collisions in radius "r" except those whose attributes match attrmask; returns whether it touched a collision or not.
FunctionPointer(int, CheckCollisionP, (NJS_POINT3* vp, float d), 0x441840); // Check if a player is in a sphere, returns 0 or player id + 1
FunctionPointer(int, CheckCollisionCylinderP, (NJS_POINT3* vp, float r, float h), 0x4418D0); // Check if a player is in a cylinder, returns 0 or player id + 1
FunctionPointer(void, AddSetStage, (char Gap), 0x46BF70); // Release objects and request act change
FunctionPointer(BOOL, GetZxShadowOnFDPolygon, (zxsdwstr* carry, NJS_OBJECT* object), 0x456510); // Get vertical intersectio(s) of a point with a model, used for deathzones
FunctionPointer(void, SetChangeGameMode, (__int16 mode), 0x413C90);
FunctionPointer(void, SetPlayerNumber, (Uint16 pno), 0x4144D0);
FunctionPointer(void, SetStageNumber, (Sint8 level, Sint8 act), 0x414570);
FunctionPointer(void, AddNumPlayer, (__int16 lives), 0x425B60); // AddLives
FunctionPointer(void, AddNumRing, (__int16 amount), 0x425BE0);
FunctionPointer(__int16, GetNumRing, (), 0x425CC0);
FunctionPointer(int, ResetNumRing, (), 0x425AB0);
FunctionPointer(int, ResetNumPlayer, (), 0x425AF0); // ResetLives
FunctionPointer(int, GetNumPlayer, (), 0x425FE0); // GetLives
FunctionPointer(Characters, GetPlayerNumber, (), 0x4144E0); // Player ID
FunctionPointer(BOOL, CheckItemExplosion, (NJS_POINT3* pos), 0x4C0550); // Check if position is within itembox bomb range
FunctionPointer(void, CalcAdvanceAsPossible, (NJS_POINT3* src, NJS_POINT3* dst, float dist, NJS_POINT3* ans), 0x4BA860); // Get a point (in ans) on line from src to dst, dist as percentage
FunctionPointer(void, VibShot, (int pno, int Time), 0x4BCBC0); // RumbleA
FunctionPointer(void, VibConvergence, (int pno, int Power, int Freq, int Time), 0x4BCC10); // RumbleB
FunctionPointer(BOOL, CheckContinueData, (), 0x44ED70); // Check if a checkpoint has been taken
FunctionPointer(void, updateContinueData, (NJS_POINT3* pos, Angle3* ang), 0x44EE70); // Set checkpoint taken
VoidFunc(SetFinishAction, 0x415540); // LoadLevelResults
VoidFunc(StopBlowingWind, 0x4C68D0); // Stops Sonic quill jittering
FunctionPointer(BOOL, GetMiClearStatus, (), 0x590650);
DataPointer(int, MaskBlock, 0x3B36D48); // Show landtable parts (_OBJ_LANDENTRY) with corresponding "blockbit" flag
FunctionPointer(void, DrawSNumbers, (_SC_NUMBERS* pscn), 0x427BB0); // Draw a number on screen using the HUD font
FunctionPointer(void, CharColliOn, (taskwk* twp), 0x4BD180); // Enable character collisions
FunctionPointer(void, CharColliOff, (taskwk* twp), 0x4BD1B0); // Disable character collisions
FunctionPointer(void, Disp_8, (task* tp), 0x004B9290); // Ripple display function
FunctionPointer(void, bbl_ripple_draw, (task* tp), 0x007A81A0); // Ripple display function (bubble version)
FunctionPointer(void, PSetRippleEffect, (int pno, NJS_POINT3* pp), 0x004407C0); // Ripple effect created by player
FunctionPointer(void, CreateWaterripple, (NJS_POINT3* pos, NJS_POINT3* vec, float scl), 0x004B9430);
FunctionPointer(void, ModelTurnWhite, (taskwk* twp, __int16 BkNumber), 0x00412A20); // Makes a model "light up" when the character approaches it
FunctionPointer(void, _gjBeforeRender, (), 0x0078B880);
FunctionPointer(void, stHWSetVSync, (int a1), 0x007899A0); // Sets the delta time multiplier
FunctionPointer(void, njSetWaitVsyncCount, (int a1), 0x0077ED40); // Sets the delta time multiplier (calls stHWSetVSync)
FunctionPointer(void, dsInitInt, (int mode, int frameIncrement), 0x00411E30); // Sets the framerate limit mode
FunctionPointer(float, GetHighestFloatAbs, (float a1, float a2, float a3), 0x0049CC70); // Gets the (absolute) highest value; inlined in SADX X360
VoidFunc(ADXTaskInit, 0x004258B0); // Initializes the music manager
FunctionPointer(void, SetEntranceNumber, (char number), 0x004147F0); // Set level entrance (return area)
FunctionPointer(void, ChangeStageWithFadeOut, (unsigned __int8 level, unsigned __int8 act), 0x004145D0); // Go to a specific level with fadeout
FunctionPointer(void, TH_Wait, (), 0x004014B0); // Thread-related, Called in MSG_Cls
FunctionPointer(playerwk*, GetPlayerWorkPtr, (char pno), 0x441B00);
FunctionPointer(void, SetTableBg_mr, (int clipLevel), 0x52F400);
FunctionPointer(void, SetTableBg_chaos7, (int c_level), 0x55D690);
FunctionPointer(void, String_IniEasy, (String* ___this, const NJS_POINT3* v0, const NJS_POINT3* vN), 0x4BF860);
FunctionPointer(void, String_Exe, (String* ___this, const NJS_POINT3* v0, const NJS_POINT3* vN, Sint32 mode), 0x4BFCA0);
FunctionPointer(void, String_Dsp, (String* ___this), 0x4BFD40);
FunctionPointer(void, ERobStart, (erctrlstr* cmd), 0x4B3EB0);
FunctionPointer(OCMDATA*, OCMRegister, (taskwk* otwp), 0x5FE2F0);
FunctionPointer(Bool, OCMunregister, (OCMDATA* ocmdata), 0x5FE360);
FunctionPointer(OCMDATA*, OCMsearchRideobj, (taskwk* otwp), 0x5FE380);
VoidFunc(calcvsyncsyoriochi, 0x413920);

// Debug
FunctionPointer(void, njPrintColor, (int color), 0x007808E0); // Sets debug font color
FunctionPointer(void, njPrintC, (signed int position, const char* text), 0x7808F0); // Prints a string
FunctionPointer(void, njPrintD, (signed int position, int value, signed int numdigits), 0x780970); // Prints a decimal number
FunctionPointer(void, njPrintF, (int position, float value, signed int precision), 0x780AC0); // Prints a float
FunctionPointer(void, njPrint, (signed int position, const char* text, ...), 0x780B30); // Prints a formatted string
FunctionPointer(void, njPrintSize, (unsigned __int16 size), 0x7808C0); // Sets debug font size

static const void* const isTextureNGPtr = (void*)0x403250;
static inline BOOL isTextureNG(NJS_TEXLIST* tl) // Check if the texlist is valid
{
	BOOL result;
	__asm
	{
		mov eax, [tl]
		call isTextureNGPtr
		mov result, eax
	}
	return result;
}

// Turn vector to angle ZXY
static const void* const calcModerateVectorPtr = (void*)0x465F00;
static inline void calcModerateVector(NJS_POINT3* vec, Angle3* ang)
{
	__asm
	{
		mov esi, [ang]
		mov edi, [vec]
		call calcModerateVectorPtr
	}
}

// Maths
StdcallFunctionPointer(Float, njSqrt, (Float n), 0x7871F0);
FastcallFunctionPointer(Float, njDistanceL2PL, (NJS_LINE* l, NJS_PLANE* pl, NJS_POINT3* cp), 0x788640);

// Rendering-related
FunctionPointer(void, njSetZCompare, (unsigned __int8 mode), 0x077ED00); // Direct3D_SetZFunc
FunctionPointer(void, njSetZUpdateMode, (int enable), 0x77ED20); // Direct3D_EnableZWrite
VoidFunc(npSetZCompare, 0x401420); // Direct3D_ResetZFunc
FunctionPointer(void, njDrawTexture3DExStart, (int mode), 0x77DCA0); // SetOceanAlphaModeAndFVF
FunctionPointer(void, njDrawTexture3DExSetData, (NJS_TEXTURE_VTX* p, int count), 0x77DD00); // Direct3D_DrawFVF_H
FunctionPointer(void, ___njSetBackColor, (uint32_t c0, uint32_t c1, uint32_t c2), 0x402F10); // Set background color
FunctionPointer(void, ___njSetConstantMaterial, (NJS_ARGB* a1), 0x402F40);
FunctionPointer(void, ___njClipZ, (float nearZ, float farZ), 0x403180); // Clip distance (or draw distance)
VoidFunc(___njFogEnable, 0x411AF0);
VoidFunc(___njFogDisable, 0x411B40);
FunctionPointer(void, ___dsSetPalette, (int no), 0x412420);
FunctionPointer(void, SetMaterial, (float a, float r, float g, float b), 0x4128A0);
VoidFunc(SetMatMatMaterial, 0x4128E0);
VoidFunc(ResetMaterial, 0x4128F0);
VoidFunc(ResetRenderingParameter, 0x7AF430);
VoidFunc(ghDefaultBlendingMode, 0x433170);
VoidFunc(SaveConstantAttr, 0x439520);
VoidFunc(LoadConstantAttr, 0x439540);
VoidFunc(late_exec, 0x4086F0); // Draws queued models
VoidFunc(njWaitVSync, 0x780BE0); // Wait loop
FunctionPointer(void, OnConstantAttr, (NJD_FLAG _and, NJD_FLAG _or), 0x439560);
FunctionPointer(void, OffConstantAttr, (NJD_FLAG _and, NJD_FLAG _or), 0x439590);
FunctionPointer(void, njSetQuadTexture, (int n, NJS_COLOR color), 0x0077DDF0);

// Lighting
FunctionPointer(void, late_SetFunc, (void(__cdecl* func)(void*), void* data, float depth, int late_flags), 0x404840); // DrawModelCallback_Queue
FunctionPointer(void, lig_setLight, (NJS_POINT3* v, float r, float g, float b, float spe, float dif, float amb), 0x412740); // Converts legacy NJS_LIGHT data to palette generation data
FastcallFunctionPointer(void, lig_calcPalette, (int no), 0x411F50); // Generates palettes for a specified light type
FunctionPointer(void, lig_fillOffsetPalette, (int no, unsigned int spe, int num), 0x412180); // Fills the current specular palette with a specicied color
FunctionPointer(void, lig_cpyPalette, (int dstNo, int srcNo), 0x412210); // Copies one palette to another
FunctionPointer(void, lig_supplementPalette, (int no, int src1, int src2, float fmix1), 0x412280); // Mixes two palettes
FunctionPointer(void, lig_convHalfBrightPalette, (int no, float rate), 0x4123C0); // Multiplies the palette's colors by a given value (definition may be wrong)
FunctionPointer(void, dsScaleLight, (float a), 0x411E90); // Normal scaling for some objects
FunctionPointer(void, dsReScaleLight, (), 0x411EF0); // Restore scaled normals
FunctionPointer(void, lig_setGjPaletteNo___, (int no), 0x412160);
FunctionPointer(void, lig_resetGjPaletteNo___, (signed int no), 0x412400);

// Input
VoidFunc(PadReadOn, 0x40EF40); // EnableControl
VoidFunc(PadReadOff, 0x40EF50); // DisableControl
FunctionPointer(void, PadReadOnP, (unsigned __int8 pno), 0x40EF70); // EnableController
FunctionPointer(void, PadReadOffP, (unsigned __int8 pno), 0x40EFA0); // DisableController
FunctionPointer(BOOL, CheckPadReadModeP, (unsigned __int8 pno), 0x40EFD0); // IsControllerEnabled
FunctionPointer(void, input_padUpdate, (), 0x0040F460); // Poll input
FunctionPointer(void, GetSwitchData, (), 0x0040FDC0); // Control
FunctionPointer(int, KeyCtrlGetOn, (), 0x0040EF30); // Get pressed state of system key

// Player
FunctionPointer(void, LoadPlayerMotionData, (int curChar), 0x422680);
FunctionPointer(void, SetPlayerInitialPosition, (taskwk* twp), 0x414810);
FunctionPointer(void, PConvertVector_G2P, (taskwk* a1, NJS_POINT3* a2), 0x43EC00);
FunctionPointer(void, PConvertVector_P2G, (taskwk* a1, NJS_POINT3* a2), 0x43EC90);
FunctionPointer(void, PResetPosition, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x43EE70);
FunctionPointer(void, PJoinVertexes, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x43FA90);
FunctionPointer(void, PSetCrashEffect, (taskwk* a1), 0x440790);
FunctionPointer(void, PSetSplashEffect, (taskwk* a1), 0x440890); // Creates a water splash
FunctionPointer(void, KillHimP, (int pno), 0x440CD0); // Kill player (for damage)
FunctionPointer(void, RunAlongPathP, (Uint8 pno, pathtag* pp), 0x440E20);
FunctionPointer(void, HangDownFromPathP, (Uint8 pno, pathtag* pp, Sint32 point), 0x440E90);
FunctionPointer(void, SetAutoPilotForBreak, (Sint8 pno), 0x440F70);
FunctionPointer(void, CancelAutoPilotP, (Sint8 pno), 0x440FA0);
FunctionPointer(void, SetLookingAngleP, (Sint8 pno, Angle3* ang), 0x441040);
FunctionPointer(void, CancelLookingAtP, (Sint8 pno), 0x441080);
FunctionPointer(void, HoldOnPillarP, (Sint8 pno, task* ttp), 0x4411D0);
FunctionPointer(void, HoldOnIcicleP, (Sint8 pno, task* ttp), 0x441210);
FunctionPointer(void, SetInputP, (uint8_t pno, int8_t mode), 0x441260);
FunctionPointer(void, SetVelocityP, (uint8_t pno, float x, float y, float z), 0x441280);
FunctionPointer(void, SetSpringVelocityP, (Uint8 pnum, Float x, Float y, Float z), 0x441370);
FunctionPointer(void, SetTrampolineVelocityP, (Sint8 pno, Sint8 time, Float x, Float y, Float z), 0x441390);
FunctionPointer(void, SetVelocityAndRotationAndNoconTimeP, (int pno, NJS_POINT3* v, Angle3* ang, int tm), 0x441490);
FunctionPointer(void, SetVelocityAndRotationAndNoconTimeWithSpinDashP, (unsigned __int8 pno, NJS_POINT3* v, Angle3* ang, __int16 tm), 0x441540);
FunctionPointer(void, SetRotationP, (uint8_t pno, Angle angx, Angle angy, Angle angz), 0x4415F0);
FunctionPointer(void, SetAccelerationP, (unsigned __int8 pno, float x, float y, float z), 0x441750);
FunctionPointer(void, SetPositionP, (uint8_t pno, float x, float y, float z), 0x441780);
FunctionPointer(void, ChangeModePlaceWithTrampolineP, (Sint8 pno, Sint8 time), 0x4417F0);
FunctionPointer(void, GetOutOfCartP, (int pno, float x, float y, float z), 0x441820);
FunctionPointer(Bool, GetPlayerPosition, (unsigned __int8 pno, __int8 frame, NJS_POINT3* pos, Angle3* ang), 0x4419C0); // Get player position and/or angle a number of frames ago
FunctionPointer(Bool, SetPlayerPosition, (unsigned __int8 pno, __int8 frame, NJS_POINT3* pos, Angle3* ang), 0x441A40); // Set player position and/or angle a number of frames ago
FunctionPointer(task*, GetPlayerTaskPointer, (Sint8 pno), 0x441AC0);
FunctionPointer(void, GetThunderBarrierP, (char character), 0x441E30);
FunctionPointer(void, GetBarrierP, (char character), 0x441EA0);
FunctionPointer(void, GetInvincibleBodyP, (int character), 0x441F10);
FunctionPointer(void, PInitialize, (int num, task* tp), 0x442750);
FunctionPointer(bool, PCheckBreak, (taskwk* twp), 0x4429C0);
FunctionPointer(void, PGetAccelerationPushPull, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x442B50);
FunctionPointer(void, PGetFriction, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x4432E0);
FunctionPointer(void, PGetInertia, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x443650);
FunctionPointer(void, PGetGravity, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x443800);
FunctionPointer(void, PResetAngle, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x443AD0);
FunctionPointer(void, SetUserGravityXZ, (Angle angx, Angle angz), 0x43B4C0);
FunctionPointer(signed int, PCheckHoldObject, (taskwk* a1), 0x43B9C0);
FunctionPointer(void, PClearSpeed, (motionwk2* mwp, playerwk* pwp), 0x43C550);
FunctionPointer(void, PSetPosition1D, (taskwk* data1, motionwk2* data2, playerwk* co2), 0x43E100);
FunctionPointer(void, PGetSpeed, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x443F50);
FunctionPointer(void, PGetPushSpeed, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x4442C0);
FunctionPointer(void, KillHimByFallingDownP, (int pno), 0x446AD0);
FunctionPointer(void, SetParabolicMotionP, (int playerNum, float a2, NJS_VECTOR* a3), 0x446D90);
FunctionPointer(void, SetAscendPowerP, (uint8_t pno, float x, float y, float z), 0x446E90);
FunctionPointer(void, PGetBreak, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x448E50);
FunctionPointer(void, PGetAccelerationAir, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x44B9C0);
FunctionPointer(void, PGetRotation, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x44BB60);
FunctionPointer(void, PGetAcceleration, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x44C270);
FunctionPointer(int, PSetPosition, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x44CDF0);
FunctionPointer(void, PSetMotion, (mtnjvwk* mjp), 0x44A800);
FunctionPointer(void, DamegeRingScatter, (char pno), 0x4506F0); // Kill or hurt player with ring scattering
FunctionPointer(void, CharacterShadow, (taskwk* twp, shadowwk* swp), 0x49F1A0);
FunctionPointer(void, MakeParabolaInitSpeed, (sParabola* para), 0x4BD2D0); // Initialize parabola with start pos, end pos and time; sets movement in para->speed
FunctionPointer(Bool, ChkParabolaEnd, (sParabola* para), 0x4BD380); // Run parabola (set movement in para->speed), returns true if finished
FunctionPointer(void, PGetAcceleration1D, (taskwk* data1, motionwk2* data2, playerwk* co2), 0x442A50);
FunctionPointer(void, PGetSpeed1D, (taskwk* data1, motionwk2* data2, playerwk* co2), 0x4441B0);
FunctionPointer(int, PResetAccelerationAir, (taskwk* a1, motionwk2* a2, playerwk* a3), 0x44BD70);

// Characters
TaskFunc(SonicTheHedgehog, 0x49A9B0);
TaskFunc(SonicDisplay, 0x4948C0);
TaskFunc(SonicDestruct, 0x494860);
TaskFunc(MilesTalesPrower, 0x461700);
TaskFunc(MilesDisplay, 0x460C40);
TaskFunc(MilesDestruct, 0x45BEA0);
TaskFunc(KnucklesTheEchidna, 0x47A770);
TaskFunc(KnucklesDisplay, 0x4721B0);
TaskFunc(KnucklesDestruct, 0x472180);
TaskFunc(AmyRose, 0x48ABF0);
TaskFunc(AmyDisplay, 0x487260);
TaskFunc(AmyDestruct, 0x484360);
TaskFunc(E102, 0x483430);
TaskFunc(E102Display, 0x47FD50);
TaskFunc(E102Destruct, 0x47EED0);
TaskFunc(BigDestruct, 0x48B730);
TaskFunc(BigDisplay, 0x48B8B0);
TaskFunc(BigTheCat, 0x490A00);
TaskFunc(Eggman, 0x7B4EF0);
TaskFunc(Tikal, 0x7B40C0);
TaskFunc(EggrobForEvent0, 0x4D3E00);

static const void* const KnucklesCheckInputPtr = (void*)0x476970;
static inline signed int KnucklesCheckInput(taskwk* twp, motionwk2* mwp, playerwk* pwp)

{
	signed int result;
	__asm
	{
		push[mwp]
		mov esi, [twp]
		mov edi, [pwp]
		call KnucklesCheckInputPtr
		add esp, 4
		mov result, eax
	}
	return result;
}

static const void* const KnucklesCheckJumpPtr = (void*)0x476830;
static inline signed int KnucklesCheckJump(taskwk* twp, playerwk* pwp)
{
	signed int result;
	__asm
	{
		mov ecx, [twp]
		mov eax, [pwp]
		call KnucklesCheckJumpPtr
		mov result, eax
	}
	return result;
}

static const void* const KnucklesCheckStopPtr = (void*)0x477970;
static inline signed int KnucklesCheckStop(taskwk* twp, playerwk* pwp)
{
	signed int result;
	__asm
	{
		mov ecx, [twp]
		mov eax, [pwp]
		call KnucklesCheckStopPtr
		mov result, eax
	}
	return result;
}

static const void* const BigGetFloatPtr = (void*)0x48C610;
static inline void BigGetFloat(taskwk* twp, playerwk* pwp)
{
	__asm
	{
		mov esi, [twp]
		mov eax, [pwp]
		call BigGetFloatPtr
	}
}

// CCL (shape collision)
FunctionPointer(void, CCL_ClearInfo, (taskwk* twp), 0x418B60);
FunctionPointer(void, CCL_CalcRange, (taskwk* twp), 0x41BAC0);
FunctionPointer(void, EntryColliList, (taskwk* twp), 0x41C280);
FunctionPointer(void, FreeColliWork, (taskwk* twp), 0x41C4E0);
FunctionPointer(void, SET_COLLI_RANGE, (colliwk* cwp, float range), 0x41C530);
FunctionPointer(void, CCL_Init, (task* tp, CCL_INFO* info, int nbInfo, unsigned __int8 id), 0x41CAF0);
VoidFunc(CCL_ClearSearch, 0x41B9D0);
FunctionPointer(c_colli_hit_info*, CCL_IsHitKindWithNumEx, (taskwk* twp, uint8_t kind), 0x41BF30);
FunctionPointer(c_colli_hit_info*, CCL_IsHitPlayerEx, (taskwk* twp), 0x41C6E0);
FunctionPointer(c_colli_hit_info*, CCL_IsHitBulletEx, (taskwk* twp), 0x41C750);
FunctionPointer(taskwk*, CCL_IsHitKindWithNum, (taskwk* twp, uint8_t kind), 0x41C910); // Check if twp collides with a collision holding a specific "kind" identifier (see CCL_INFO), returns the object's taskwk or 0
FunctionPointer(taskwk*, CCL_IsHitKindWithNum2, (taskwk* twp, uint8_t kind), 0x41C930); // Same as CCL_IsHitKindWithNum2 but with unnecessary safe checks.
FunctionPointer(taskwk*, CCL_IsHitPlayer, (taskwk* twp), 0x41CBC0); // Check if a player collides with twp, returns the player's taskwk or 0
FunctionPointer(taskwk*, CCL_IsHitBullet, (taskwk* twp), 0x41CBE0); // Check if a projectile collides with twp, returns the projectile's taskwk or 0
FunctionPointer(taskwk*, CCL_IsHitPlayerWithNum, (taskwk* twp, Sint32 info_num), 0x41CC60);
FastcallFunctionPointer(BOOL, njCollisionCheckSS, (float* p1, float* p2), 0x789360); // Check if two spheres collide, arguments are two array of 4 floats for x, y, z and radius

// Geometry collision
FunctionPointer(void, ListGroundForCollision, (Float xPosition, Float yPosition, Float zPosition, Float fRadius), 0x43ACD0);
FunctionPointer(void, RegisterCollisionEntry, (int slAttribute, task* pTask, NJS_OBJECT* pObject), 0x43B2E0);
FunctionPointer(void, WithdrawCollisionEntry, (task* pTask, NJS_OBJECT* pObject), 0x43B380);
FunctionPointer(void, ReleaseMobileLandObject, (NJS_OBJECT* pObjLandObject), 0x43B450);
FunctionPointer(NJS_OBJECT*, GetMobileLandObject, (), 0x43B400);
FunctionPointer(void, SetMleriRangeRad, (float range), 0x43B6E0); // Set minimum collision lookup radius
FunctionPointer(bool, CheckPlayerRideOnMobileLandObjectP, (int pno, task* ttp), 0x441C30);
FunctionPointer(NJS_OBJECT*, SetModelCollision, (NJS_OBJECT* OriginalObj, task* tp, Sint32 Flag), 0x49D6C0); // Creates dynamic collision from Task
VoidFunc(ResetMleriRangeRad, 0x43B6F0); // Reset minimum collision lookup radius
VoidFunc(MakeLandCollLandEntryRangeIn, 0x43AEF0);
VoidFunc(MakeLandCollLandEntryALL, 0x43B580);
VoidFunc(CL_ColPolListUpModeSetAll, 0x4511C0); // Process all collisions instead of only those near the player, used for chao gardens.

// Path
FunctionPointer(int, CheckPlayerRideOnPath, (pathtag* pathtagp), 0x440ED0); // Check if P1 or P2 is on the specified path; returns bitfield.
FunctionPointer(int, GetStatusOnPath, (pathtag* tag, pathinfo* pi), 0x49C330); // Get position, angle and normals at "pi->onpathpos" distance from path in "pi".
FunctionPointer(BOOL, SCPathOnposToPntnmb, (pathtag* tag, float onpos, int* pntnmb), 0x49C5E0); // Converts distance on path to point number, FALSE if outside of range
FunctionPointer(BOOL, SCPathPntnmbToOnpos, (pathtag* tag, unsigned int pntnmb, float* onpos), 0x49C630); // Converts path point number to distance on path, FALSE if point outside of range.
FunctionPointer(float, SCPathPntNearToOnpos, (pathtag* tag, NJS_POINT3* pnt, NJS_POINT3* onpnt3, float* onpos), 0x49C670); // Get nearest point (in "onpnt3") and distance from path (in "onpos") from point "pnt", returns distance between the two points.
FunctionPointer(float, RunWithSeeingPathP, (uint8_t pno, pathtag* pp), 0x440E50);
FunctionPointer(void, LadderingPathP, (uint8_t pno, pathtag* pp, int point, Angle3* ang), 0x446C80);
FunctionPointer(BOOL, InitPathWork, (), 0x49C870);
VoidFunc(SetPathWork, 0x49C1A0);
TaskFunc(ManagePathWork, 0x49C820);
TaskFunc(Chaos7PathWork, 0x55E190);          // Path task for Perfect Chaos attack paths
TaskFunc(PathworkGoneWithTheWind, 0x4DF020); // Path task for wind paths
TaskFunc(PathworkSeeingPath, 0x4BB1F0);      // Path task for guiding paths (like loops)
TaskFunc(ObjectHeli1, 0x613ED0);             // Path task for Speed Highway's helicopter
TaskFunc(ObjectWater, 0x5E3830);             // Path task for Lost World's waterfall
TaskFunc(CamHw1Curve1, 0x6133E0);            // Path task for camera guiding paths (curve)
TaskFunc(CamHw1Tube1, 0x613400);             // Path task for camera guiding paths (tube)
TaskFunc(CamHw1Spiral1, 0x613420);           // Path task for camera guiding paths (spiral)
TaskFunc(CamHw1Hw14, 0x613420);              // Path task for camera guiding paths
TaskFunc(CamHw1Hw15, 0x613460);              // Path task for camera guiding paths

// Enemy
FunctionPointer(void, AddEnemyScore, (int add), 0x425C70);
FunctionPointer(enemywk*, EnemyInitialize, (task* tp, taskwk* twp), 0x4CC990); // Allocates an enemywk
FunctionPointer(char, EnemySearchPlayer, (taskwk* twp, enemywk* ewp), 0x4CCA80); // BOOL8: check if there is a player in ewp field of view
FunctionPointer(void, EnemyGetShadow, (taskwk* twp, enemywk* ewp), 0x4CCB50); // Gets shadow data in ewp->shadow
FunctionPointer(void, EnemyCheckWall, (taskwk* twp, enemywk* ewp), 0x4CCC30); // Turn twp->ang.y if wall detected (by ewp->bound_add_angle amount)
FunctionPointer(void, EnemyPreserveHomePosition, (taskwk* twp, enemywk* ewp), 0x4CD370); // Store twp->pos in ewp->home
FunctionPointer(void, EnemyPreservePreviousPosition, (taskwk* twp, enemywk* ewp), 0x4CD390); // Store twp->pos in ewp->pre
FunctionPointer(void, EnemyBackToPreviousPosition, (taskwk* twp, enemywk* ewp), 0x4CD3B0); // Set twp->pos to ewp->pre
FunctionPointer(float, EnemyDist2FromAim, (taskwk* twp, enemywk* ewp), 0x4CD3D0); // Get distance between twp->pos to ewp->aim (no sqrt)
FunctionPointer(Angle, EnemyCalcAimAngle, (taskwk* twp, enemywk* ewp), 0x4CD410); // Get angle between twp-pos and ewp->aim
FunctionPointer(void, EnemyTurnToAim, (taskwk* twp, enemywk* ewp), 0x4CD460); // Turn twp->ang.y to ewp->aim at ewp->angy_spd speed
FunctionPointer(float, EnemyDistFromHome, (taskwk* twp, enemywk* ewp), 0x4CD4D0); // Get distance between twp->pos and ewp->home
FunctionPointer(float, EnemyDist2FromHome, (taskwk* twp, enemywk* ewp), 0x4CD510); // Get distance between twp->pos and ewp->home (no sqrt)
FunctionPointer(Angle, EnemyCalcHomeAngle, (taskwk* twp, enemywk* ewp), 0x4CD550); // Get angle between twp->pos and ewp->home
FunctionPointer(void, EnemyTurnToHome, (taskwk* twp, enemywk* ewp), 0x4CD5A0); // Turn twp->ang.y to ewp->home at ewp->angy_spd speed
FunctionPointer(float, EnemyDist2FromPlayer, (taskwk* twp, int num), 0x4CD610); // Get distance between twp->pos and player pos (no sqrt)
FunctionPointer(Angle, EnemyCalcPlayerAngle, (taskwk* twp, enemywk* ewp, int pnum), 0x4CD670); // Get angle between twp->pos and player pos
FunctionPointer(void, EnemyTurnToPlayer, (taskwk* twp, enemywk* ewp, unsigned __int8 pnum), 0x4CD6F0); // Turn twp->ang.y to player pos at ewp->angy_spd speed
FunctionPointer(BOOL, EnemyCheckFrameIn, (NJS_POINT3* pos), 0x4CD730); // Check if position is visible on screen
FunctionPointer(void, EnemyAimAroundPlayer, (taskwk* twp, enemywk* ewp, int pnum, int aim_num), 0x4CD7A0); // Set ewp->aim to one of 4 hardcoded points around player position
FunctionPointer(void, CreateExpParts, (taskwk* twp, NJS_MODEL** model, NJS_TEXLIST* tex), 0x4CDCB0); // Simulates an explosion from a null-terminated list of models
FunctionPointer(void, CreateExpSpring, (taskwk* twp, unsigned __int8 num), 0x4CDE20);
FunctionPointer(void, EnemyBuyoScale, (taskwk* twp, enemywk* ewp), 0x4CDF60);
FunctionPointer(void, EnemyBumpPlayer, (char pnum), 0x4CDFE0);
FunctionPointer(BOOL, EnemyCheckDamage, (taskwk* twp, enemywk* ewp), 0x4CE030); // Check if enemy is damaged, sets some damage type flags in ewp->flag
FunctionPointer(void, EnemyCheckFloor, (taskwk* twp, enemywk* ewp), 0x4CE100); // Check ground status and draw shadow
FunctionPointer(void, EnemyCheckGroundCollision, (taskwk* twp, enemywk* ewp), 0x4CE370); // Main enemy collision and shadow routine
TaskFunc(UniDestructor, 0x4E21D0); // Destructor task for every enemy, does nothing
FunctionPointer(BOOL, CalcMMMatrix, (NJS_MATRIX_PTR impmat, NJS_ACTION* actptr, float mtnfrm, unsigned int srcnmb, NJS_MATRIX_PTR ansmat), 0x4B81F0); // Get matrix (in ansmat) of a node (srcnmb) of an animated model using impmat as base matrix; stores all matrices in a buffer for GetMMMatrix
FunctionPointer(BOOL, GetMMMatrix, (unsigned int srcnmb, NJS_MATRIX_PTR ansmat), 0x4B82D0); // Get stored matrix (in ansmat) of node (srcnmb), use this once CalcMMMatrix has been called to speed it up
FunctionPointer(void, clrObjFlags, (NJS_OBJECT** obj, unsigned int flag), 0x4399D0); // Set a flag in the evalflag of an object
FunctionPointer(void, setObjFlags, (NJS_OBJECT** obj, unsigned int flag), 0x4399A0); // Unset a flag from the evalflag of an object
FunctionPointer(void, CreateFire, (NJS_VECTOR* pos, NJS_VECTOR* velo, float scl), 0x004CB060); // Creates Unidus fire particles

static const void* const calcAimPosPtr = (void*)0x7B1720;
static inline void calcAimPos(taskwk* twp, enemywk* ewp)
{
	__asm
	{
		mov esi, [ewp]
		mov edi, [twp]
		call calcAimPosPtr
	}
}

// Boss
FunctionPointer(bosswk*, BInitialize, (taskwk* twp, unsigned int size), 0x4BD420); // Allocates a bosswk, size is customizable (minimum should be 56)
FunctionPointer(void, BSetMotion, (taskwk* twp, bosswk* bwp), 0x4BE220);
FunctionPointer(void, BJoinVertexes, (taskwk* twp, bosswk* bwp), 0x4BDC50);
FunctionPointer(void, SetDisplayBossName, (const char* str, int xpos, int ypos, int time), 0x4B36D0);
FunctionPointer(void, LoadLifeGauge, (signed int w, signed int h, signed int health), 0x4B3CC0);
FunctionPointer(BOOL, ChkE104ColliLandXZ, (NJS_POINT3* pPos, float fDistChk, float* pfGroundY, NJS_POINT3* pVec), 0x568AF0); // Ground collision logic for E104
FunctionPointer(task*, SetCircleLimit, (NJS_POINT3* pos, NJS_POINT3* center, float radius), 0x7AF3E0); // Creates an object that limits a position into a circle

static const void* const BSetMotion_NextPtr = (void*)0x4BE170;
static inline void BSetMotion_Next(bosswk* bwp, int patno)
{
	__asm
	{
		mov ecx, [patno]
		mov eax, [bwp]
		call BSetMotion_NextPtr
	}
}

// Level
VoidFunc(SetTableBG_Chaos0, 0x545CE0);
VoidFunc(SetTableBG_Chaos2, 0x54A540);
VoidFunc(SetTableBG_Chaos4, 0x550840);
VoidFunc(SetTableBG_Chaos6, 0x5577E0);
VoidFunc(SetTableBG_E101, 0x566AF0);

static const void* const RdHighwayInitPtr = (void*)0x60FF80;
static inline void RdHighwayInit(task* tp)
{
	__asm
	{
		mov edi, [tp]
		call RdHighwayInitPtr
	}
}

static const void* const RdSnowCheckEnterIceCavePtr = (void*)0x4E9260;
static inline void RdSnowCheckEnterIceCave(task* tp)
{
	__asm
	{
		mov eax, [tp]
		call RdSnowCheckEnterIceCavePtr
	}
}

static const void* const RdSnowCheckExitIceCavePtr = (void*)0x4E9380;
static inline void RdSnowCheckExitIceCave(task* tp)
{
	__asm
	{
		mov eax, [tp]
		call RdSnowCheckExitIceCavePtr
	}
}

static const void* const RdSnowInitPtr = (void*)0x4E9780;
static inline void RdSnowInit(task* tp)
{
	__asm
	{
		mov eax, [tp]
		call RdSnowInitPtr
	}
}

static const void* const RdMountainInitPtr = (void*)0x601390;
static inline void RdMountainInit(task* tp)
{
	__asm
	{
		mov esi, [tp]
		call RdMountainInitPtr
	}
}

// Miren functions (utilities)
FunctionPointer(void, MirenInitTask, (task* task_p, const TaskInfo* info_p, void* param_p), 0x796B30); // Create a task with information from TaskInfo
FunctionPointer(void, MirenSetTask, (int level, const TaskInfo* info_p, void* param_p), 0x796B90); // Edit a task with information from TaskInfo
FunctionPointer(void, MirenSetChildTask, (task* parent_p, const TaskInfo* info_p, void* param_p), 0x796BD0); // Creates a child task with information from TaskInfo
FunctionPointer(void, MirenGameChangeAct, (int actDiff, int bgmId), 0x796C10); // Draw collisions of a colliwk (debug)
FunctionPointer(void, MirenSoundPlayOneShotSE, (int se, const NJS_POINT3* pos_p, const VolumeInfo* info_p), 0x79E400); // Creates a simple spatial sound with volume setting
FunctionPointer(void, MirenObjInitCharColli, (task* task_p, const ObjectInfo* objInfo_p, const ObjCmnInfo* cmnInfo_p), 0x79E510); // A wrapper for CCL_Init
FunctionPointer(void, MirenObjClearForceWork, (forcewk* forceWk_p), 0x79E540); // Clears a forcewk
FunctionPointer(void, MirenObjEntryMobileLandObject, (task* task_p, obj* obj_p, int attr, float scale), 0x79E580); // Sets unknown collision flags based on model radius * scale then calls RegisterCollisionEntry
FunctionPointer(bool, MirenObjCheckCollisionP, (taskwk* taskWk_p, float dist), 0x79E600); // Set 0x100 flag if player in range, enables/disables mobile land collision
FunctionPointer(void, MirenObjFreeMobileLandObject, (task* task_p, NJS_OBJECT* obj_p), 0x79E630); // Removes a mobile land collision.
FunctionPointer(void, MirenObjFreeFall, (NJS_POINT3* pos_p, NJS_POINT3* spd_p, float yOfs, const NJS_POINT3* slowRate_p), 0x79E650);
FunctionPointer(void, MirenObjDrawCharColliObjectNoScale, (NJS_OBJECT* obj_p, taskwk* taskWk_p, NJS_TEXLIST* texList_p), 0x79E820);
FunctionPointer(NJS_OBJECT*, MirenObjAllocMobileLandObject, (const NJS_OBJECT* orgObj_p), 0x79E8B0); // Gets mobile land object entry (with GetMobileLandObject) and copy information from orgObj
FunctionPointer(void, MirenObjSetObjectMatrix, (NJS_OBJECT* obj_p, const taskwk* taskWk_p, float scale), 0x79E8F0); // Copy position, rotation and scale to obj_p
FunctionPointer(void, MirenObjDrawMobileLandObject, (NJS_OBJECT* obj_p, taskwk* taskWk_p, float scale, NJS_TEXLIST* texList_p), 0x79E930);
FunctionPointer(void, MirenObjEditDrawCollisionInfo, (const CCL_INFO* info_p, const taskwk* taskWk_p), 0x79F450);
FunctionPointer(void, MirenObjEditDrawCollision, (const colliwk* colliWk_p), 0x79F4D0); // Draw collisions of a colliwk (debug)

// Object
FunctionPointer(BOOL, dsCheckViewV, (NJS_POINT3* ft, float radius), 0x403330); // IsVisible
FunctionPointer(BOOL, SetRegularTexture, (), 0x420F90); // Set regular object texlist
FunctionPointer(BOOL, CheckObjectTexture, (), 0x420FB0); // Check if the first level object texlist exists
FunctionPointer(BOOL, SetObjectTexture, (), 0x420FC0); // Set first level object texlist
FunctionPointer(Bool, CheckEditMode, (), 0x4258F0);
FunctionPointer(int, GetTheNearestPlayerNumber, (NJS_POINT3* pos), 0x441B70);
FunctionPointer(BOOL, CheckRangeWithR, (task* tp, Float fRange), 0x46BFA0);
FunctionPointer(BOOL, CheckRangeOutWithR, (task* tp, float fRange), 0x46C010);
TaskFunc(SetBroken, 0x46C0F0);   // Set flag that objects check with CheckBroken to not respawn
TaskFunc(SetNoRevive, 0x46C100); // Tell the game to not respawn the object upon restart
FunctionPointer(BOOL, CheckBroken, (task* tp), 0x46C110); // Check no respawn flag
TaskFunc(SetContinue, 0x46C120);
TaskFunc(Dead, 0x46C130); // Set no respawn flag
TaskFunc(DeadOut, 0x46C150); // Destroy object and set no respawn flag
FunctionPointer(BOOL, CheckRange, (task* tp), 0x46C330);
FunctionPointer(BOOL, CheckRangeOut, (task* tp), 0x46C360);
FunctionPointer(BOOL, CheckRangeOut_L, (task* tp), 0x46C390);
FunctionPointer(BOOL, ObjectMovableInitialize, (taskwk* twp, motionwk* mwp, unsigned int mode), 0x49CDA0); // Modes: 4 is regular, 5 is hold in place, 6 is Big's huge objects, 7 is swingable
FunctionPointer(void, ObjectMovableSRegularExecute, (task* tp), 0x49D730); // Run small held object physics
FunctionPointer(void, ObjectMovableMRegularExecute, (task* tp), 0x49DA30); // Run heavy held object physics
FunctionPointer(void, SetHoldingItemIDP, (uint8_t pno, int itemID), 0x442010); // Set an identifier for the current held object
FunctionPointer(int, GetHoldingItemIDP, (uint8_t pno), 0x442030); // Get identifier for the current held object
FunctionPointer(void, ObjectSetupInput, (taskwk* twp, motionwk* mwp), 0x49CE60); // Reset acceleration and flags
TaskFunc(ObjectNormal, 0x49DDF0); // Object task that draws an object in twp->counter with level texlist or twp->value texlist
TaskFunc(ObjectNormal_LE, 0x49DEF0); // Same as ObjectNormal with different D3D Z Func
TaskFunc(ObjectCCollision, 0x49DF70); // Same as ObjectNormal but with collision
TaskFunc(ObjectScaleCCollision, 0x49DFF0); // Same as ObjectCCollision but with scaling
TaskFunc(ObjectGCollision, 0x49E170); // Same as ObjectNormal but with geometry collision in twp->timer
TaskFunc(ObjectScaleDisplay, 0x49D220); // Same as ObjectNormalDisplay with different D3D Z Func
TaskFunc(ObjectNormalDisplay_LE, 0x49D24B); // Same as ObjectNormalDisplay with different D3D Z Func
TaskFunc(ObjectNormalDisplay, 0x49DDF0); // Display task that draws an object in twp->counter using regular pos/ang values
TaskFunc(ObjectNormalExit, 0x49D050); // Red Mountain and other level objects delete function
FunctionPointer(float, GetShadowPos, (float x, float y, float z, Angle3* ang), 0x49E920); // Get Y position and angle of ground below
FunctionPointer(float, GetShadowPosOnWalter, (float x, float y, float z, Angle3* ang), 0x49EAD0); // Get Y position and angle of ground and water below
FunctionPointer(BOOL, GetShadowPosXYZ, (xyyzzxsdwstr* answer), 0x49F450);
FunctionPointer(BOOL, GetShadowPosXYZonWater, (xyyzzxsdwstr* answer), 0x49F720);
FunctionPointer(void, CreateAnimal, (int e_num, float x, float y, float z), 0x4BE610);
FunctionPointer(void, CreateFlash, (taskwk* twp, float scl), 0x4CAF30); // Create an explosion effect at twp position
FunctionPointer(void, CreateFlash2, (float x, float y, float z, float scl), 0x4CAF80); // Create an explosion effect
TaskFunc(E102KillCursor, 0x4CEFE0); // Remove gamma target on task
FunctionPointer(BOOL, Knuckles_KakeraGame_Set_PutEme, (uint8_t emeid, NJS_POINT3* emepos), 0x477D90); // Give emerald
FunctionPointer(BOOL, Knuckles_KakeraGame_Set_CheckEme, (uint8_t emeid, NJS_POINT3* emepos), 0x476660); // Check if emerald exists and signal position to radar
FunctionPointer(void, CreateSmoke, (NJS_POINT3* pos, NJS_POINT3* velo, Float scl), 0x4B9820); // Create smoke effect
FunctionPointer(void, CreateSmoke2, (particle_info* effect), 0x4B98E0); // Create smoke effect with custom data
FunctionPointer(void, CreateSnow, (NJS_POINT3* pos, NJS_POINT3* velo, Float scl), 0x4B9B10);
FunctionPointer(void, CreateBomb, (NJS_POINT3* pos, Float scl), 0x4CACF0); // Create bomb effect
FunctionPointer(void, ef_speed, (taskwk* twp), 0x4D6BF0);
FunctionPointer(void, ef_5ring, (taskwk* twp), 0x4D6C50);
FunctionPointer(void, ef_10ring, (taskwk* twp), 0x4D6C90);
FunctionPointer(void, ef_random_ring, (taskwk* twp), 0x4D6CD0);
FunctionPointer(void, ef_1up, (taskwk* twp), 0x4D6D40);
FunctionPointer(void, ef_muteki, (taskwk* twp), 0x4D6D80);
FunctionPointer(void, ef_baria, (taskwk* twp), 0x4D6DC0);
FunctionPointer(void, ef_explosion, (taskwk* twp), 0x4D6E00);
FunctionPointer(void, ef_th_baria, (taskwk* twp), 0x4D6E40);
FunctionPointer(void, DrawCustomObject, (NJS_OBJECT* top_object, CUSTOM_OBJ* custom), 0x4BA5D0);
FunctionPointer(void, DrawCustomObject_wz, (NJS_OBJECT* top_object, CUSTOM_OBJ* custom), 0x4BA710);
TaskFunc(Draw, 0x004D6810); // Draws the item box
TaskFunc(DrawInWater, 0x004D6990); // Draws the underwater item box
TaskFunc(Draw_Break, 0x004D6B20); // Draws the destroyed item box
TaskFunc(Dead_ItemBox, 0x004D6BA0); // Deletes the item box
TaskFunc(ObjectSkydeck_wings_End, 0x004FB290);

// Object task functions
TaskFunc(CameraDisplay, 0x4370F0);
TaskFunc(CameraPause, 0x4373D0);
TaskFunc(Camera, 0x438090);
TaskFunc(ObjectSavePoint, 0x44F540); // Checkpoint
TaskFunc(RingDoneDisplayer, 0x44FC80);
TaskFunc(Ring, 0x450370);
TaskFunc(BigDirectAhead, 0x48E2E0);
TaskFunc(EnemyLeon, 0x4A85C0);
TaskFunc(EnemySaru, 0x4AD140); // Kiki
TaskFunc(EnemyUniA, 0x4AF190);
TaskFunc(EnemyUniB, 0x4AF500);
TaskFunc(EnemyUniC, 0x4AF860);
TaskFunc(EnemySpinnaA, 0x4B0DF0);
TaskFunc(EnemySpinnaB, 0x4B0F40);
TaskFunc(EnemySpinnaC, 0x4B1090);
TaskFunc(PoliceDisplayer, 0x4B2710);
TaskFunc(PoliceExecutor, 0x4B30E0);
TaskFunc(EnemyPolice, 0x4B3210);
TaskFunc(ObjectJumpPanel, 0x4B8DC0);
TaskFunc(TBarrierDisp, 0x4B9D90);
TaskFunc(ThunderB, 0x4BA100);
TaskFunc(ObjectItemboxAir, 0x4C07D0);
TaskFunc(ObjectRocketBaseH, 0x4CA530);
TaskFunc(ObjectRocketBaseHS, 0x4CA660);
TaskFunc(ObjectRocketBaseV, 0x4CA7D0);
TaskFunc(ObjectRocketBaseVS, 0x4CA900);
TaskFunc(ObjectSwitch, 0x4CBA80);
TaskFunc(Object_Dynamite, 0x4CBDE0);
TaskFunc(ObjectItemBox, 0x4D6F10);
TaskFunc(ColliSphere, 0x4D4700);
TaskFunc(ColliCylinder, 0x4D4770);
TaskFunc(ColliCube, 0x4D47E0);
TaskFunc(ColliWall, 0x4D4850);
TaskFunc(ColliOttottoRange, 0x4D4B70);
TaskFunc(ObjectHintbox, 0x007A9C60); // Hint monitor
TaskFunc(BossChaos0, 0x548640);
TaskFunc(Chaos2Column, 0x54AC80);
TaskFunc(EggCarrierCloud_c6, 0x557690);
TaskFunc(B_Destructor, 0x59DBF0); // Standard destroy for geometry collisions (stored in twp->counter)
TaskFunc(EnemyMountainE104, 0x605B40); // Epsilon
TaskFunc(mt_gdcontrol, 0x600890); // Delayed bgm load task
TaskFunc(execTPCoaster, 0x61D6E0);
TaskFunc(dispAxelPanel, 0x7A4360);
TaskFunc(ObjectAxelPanel, 0x7A4450);
TaskFunc(DrawSpring, 0x7A4980);
TaskFunc(DrawSpringB, 0x7A4AE0);
TaskFunc(ObjectIronBallJG, 0x7A3E50);
TaskFunc(ObjectIronBall, 0x7A4260);
TaskFunc(ObjectSpring, 0x7A4C30);
TaskFunc(ObjectSpringB, 0x7A4E50);
TaskFunc(ObjectLinering, 0x7ABE90);
TaskFunc(ObjectLineringV, 0x7AC4F0);
TaskFunc(ObjectToge, 0x7A30E0); // Spikes
TaskFunc(CallObjectEmeraldDisplay, 0x4A2FB0); // Emerald piece disp
TaskFunc(ObjectEmeraldPRegular, 0x4A2FD0); // Emerald piece exec
TaskFunc(ObjectEmeraldP, 0x4A3420); // Emerald piece
TaskFunc(ObjectReleaseBox, 0x46B170); // Capsule
TaskFunc(Object_Mountain_Zako, 0x603F80);
TaskFunc(Object_Mountain_ZakoB, 0x603FF0);
TaskFunc(Object_Mountain_Blow, 0x605C50);
TaskFunc(Object_Mountain_Blow2, 0x605D40);
TaskFunc(ObjectMountainSakusaku, 0x605E40);
TaskFunc(ObjectMountainSaku, 0x605DF0);
TaskFunc(ObjectMountainTuribasi1, 0x6080D0);
TaskFunc(ObjectMountainTuribasi2, 0x6080F0);
TaskFunc(ObjectMountainTuribasi3, 0x608110);
TaskFunc(ObjectMountainTuribasi4, 0x608130);
TaskFunc(cartDisplay, 0x796CE0);
TaskFunc(EnemyCart, 0x79A9E0);
TaskFunc(ObjectGroupRing, 0x79B2F0);
TaskFunc(EnemySnake, 0x7A0330);
TaskFunc(EnemySai, 0x7A1380); // Rhinotank
TaskFunc(ObjectTikalHint, 0x7A1AA0);
TaskFunc(ObjectCmnKusa, 0x7A2B60); // Weed that Tails can cut
TaskFunc(ObjectCmnDushRing, 0x7A26F0); // Dash hoop
TaskFunc(ObjectBaloon, 0x7A21C0);

// Cart
FunctionPointer(CartDispTime, CartStateCentiSecToDispTime, (int sec100), 0x4DBBA0);   // Converts ms to min:sec:ms, accepts negative time
FunctionPointer(void, CartInitLetter, (), 0x4DC740);                                  // Init cart letter sprites
FunctionPointer(void, cartTopographicalCollision, (task* tp, taskwk* twp), 0x799380); // Ground collision logic for carts

static const void* const setCartDirectionPtr = (void*)0x798050;
static inline void setCartDirection(taskwk* twp, NJS_POINT3* vec)
{
	__asm
	{
		push[vec]
		mov esi, [twp]
		call setCartDirectionPtr
		add esp, 4
	}
}

static const void* const cartExplosionPtr = (void*)0x797300;
static inline void cartExplosion(taskwk* twp)
{
	__asm
	{
		mov edi, [twp]
		call cartExplosionPtr
	}
}

static const void* const cartCheckPassPtr = (void*)0x7980C0;
static inline void cartCheckPass(taskwk* twp)
{
	__asm
	{
		mov ecx, [twp]
		call cartCheckPassPtr
	}
}

static const void* const cartSpdControlSonicOnTheCartPtr = (void*)0x798E40;
static inline void cartSpdControlSonicOnTheCart(taskwk* twp)
{
	__asm
	{
		mov ebx, [twp]
		call cartSpdControlSonicOnTheCartPtr
	}
}

static const void* const setupCartStagePtr = (void*)0x7981F0;
static inline void setupCartStage(task* tp)
{
	__asm
	{
		mov eax, [tp]
		call setupCartStagePtr
	}
}

static const void* const cartSELoopPtr = (void*)0x798170;
static inline void cartSELoop(int se_no, task* tp)
{
	__asm
	{
		mov edi, [tp]
		mov ebx, [se_no]
		call cartSELoopPtr
	}
}

static const void* const cartThinkPtr = (void*)0x79A8E0;
static inline void cartThink(taskwk* twp, task* tp)
{
	__asm
	{
		mov ecx, [tp]
		mov eax, [twp]
		call cartThinkPtr
	}
}

static const void* const cartSpdForceOfNaturePtr = (void*)0x799EB0;
static inline void cartSpdForceOfNature(taskwk* twp)
{
	__asm
	{
		mov edx, [twp]
		call cartSpdForceOfNaturePtr
	}
}

static const void* const cartShadowPosPtr = (void*)0x7977C0;
static inline void cartShadowPos(taskwk* twp)
{
	__asm
	{
		mov eax, [twp]
		call cartShadowPosPtr
	}
}

// Sound (note: "tone" is sound id)
FunctionPointer(BOOL, Get3Dmode, (), 0x40FF40); // Get3DSoundEnabled
FunctionPointer(signed int, dsPlay_oneshot, (int tone, int id, int pri, int volofs), 0x423D70);
FunctionPointer(signed int, dsPlay_iloop, (int tone, int id, int pri, int volofs), 0x423E20);
FunctionPointer(signed int, dsPlay_timer, (int tone, int id, int pri, int volofs, int timer), 0x423F50);
FunctionPointer(signed int, dsPlay_timer_v, (int tone, int id, int pri, int volofs, int timer, float x, float y, float z), 0x424000);
FunctionPointer(signed int, dsPlay_timer_vq, (int tone, int id, int pri, int volofs, int timer, float x, float y, float z, float rad), 0x424100);
FunctionPointer(void, dsStop_num, (int tone), 0x424210); // stop sounds by sound id
FunctionPointer(void, dsStop_id, (int id), 0x424240); // stop sounds using identifier "id"
FunctionPointer(void, dsVolume_id, (int id, int vol), 0x424270); // change volume of sounds using identifier "id"
FunctionPointer(void, dsPan_id, (int id, int pan), 0x4242B0); // change panning of sounds using identifier "id"
FunctionPointer(void, dsPitch_id, (int id, int pitch), 0x4242F0); // change pitch of sounds using identifier "id"
VoidFunc(dsPause_all, 0x424320);
VoidFunc(dsRelease_all, 0x424380);
VoidFunc(dsPauseSndOnly, 0x424400);
VoidFunc(dsStop_all, 0x424460);
VoidFunc(TempEraseSound, 0x424830);
FunctionPointer(void, dsPlay_oneshot_Dolby, (int tone, int id, int pri, int volofs, int time, taskwk* pTaskwk), 0x424880);
FunctionPointer(void, dsPlay_Dolby_time, (int tone, int id, int pri, int volofs, int time, taskwk* pTaskwk), 0x424920);
FunctionPointer(void, dsPlay_Dolby_timer_vq, (int tone, int id, int pri, int volofs, int timer, float rad, taskwk* pTaskwk), 0x4249E0);
FunctionPointer(int, dsPlay_oneshot_v, (int tone, int id, int pri, int volofs, float x, float y, float z), 0x424FC0);
VoidFunc(voicevolresume, 0x40CD10);
FunctionPointer(void, SetBankDir, (signed int soundlist), 0x4238E0);
VoidFunc(BGM_Stop, 0x4256B0); //Stop BGM
FunctionPointer(MDHANDLE*, MDHeaderOpen, (const char* fname, Sint32 flag), 0x4B4D10); // Open sound bank
FunctionPointer(void, MDHeaderClose, (MDHANDLE* mdh), 0x4B4F50); // Close sound bank
FunctionPointer(BOOL, PlayPCM, (int ch), 0x410050);
FunctionPointer(void, StopPCM, (int ch), 0x410100);
FunctionPointer(void, SetVolumePCM, (int ch, int volume), 0x4101A0);
FunctionPointer(void, SetFreqencyPCM, (int ch, float freqency), 0x410200);
FunctionPointer(void, SetPanPCM, (int ch, int pan), 0x410260);
FunctionPointer(void, Set3DPositionPCM, (int ch, float x, float y, float z), 0x4102C0);
FunctionPointer(BOOL, Set3DMinMaxPCM, (int ch, float _min, float _max), 0x4103B0);
FunctionPointer(BOOL, LoadPCM, (int ch, void* wavememory, int wavesize, int loopflag), 0x4106F0);
FunctionPointer(BOOL, Load3DPCM, (int ch, void* wavememory, int wavesize, int loopflag), 0x410970);
FunctionPointer(void, IsndPan, (int pan, int handleno), 0x423C80);
FunctionPointer(void, IsndVolume, (int vol, int handleno), 0x423C20);
FunctionPointer(void, IsndPitch, (int pitch, int handleno), 0x423CC0);
FunctionPointer(BOOL, dsEVboss, (int tone), 0x4246C0);
VoidFunc(wmapause, 0x40D060);
VoidFunc(wmaresume, 0x40D0A0);
VoidFunc(dsSoundServer, 0x4250D0);

// Camera
FunctionPointer(void, CameraSetEventCameraFunc, (CamFuncPtr func, Sint8 ucAdjustType, Sint8 scCameraDirect), 0x437D20); // Creates an event camera with custom script, see CDM enum for direct mode
FunctionPointer(void, CameraSetEventCamera, (Sint16 ssCameraMode, Sint8 ucAdjustType), 0x437BF0); // Creates an event camera, see CAMMD and CAMADJ enums
FunctionPointer(Bool, IsEventCamera, (), 0x436520); // If the current active camera is an event camera
FunctionPointer(Bool, IsCompulsionCamera, (), 0x436530); // If the current active camera is a compulsion camera
VoidFunc(CameraReleaseEventCamera, 0x436140); // Release active event camera
VoidFunc(CameraReleaseCollisionCamera, 0x436400); // Release active collision camera
FunctionPointer(void, CamcontSetCameraCAMSTATUS, (taskwk* pTaskWork), 0x435C30); // Copy position (campos) and angle (ang) from control to task; automatically called after camera script if direct mode is CDM_CAMSTATUS
FunctionPointer(void, CamcontSetCameraLOOKAT, (taskwk* pTaskWork), 0x435C70); // Copy position (campos) from control & calculate angle (ang) to target (tgtpos); automatically called after camera script if direct mode is CDM_LOOKAT
FunctionPointer(void, CamcontSetCameraTGTOFST, (taskwk* pTaskWork), 0x435D10); // Copy angle (ang) from control & calculate position using angle (ang) and distance (tgtdist); automatically called after camera script if direct mode is CDM_TGTOFST
FunctionPointer(void, SetAdjustMode, (Sint32 AdjustType), 0x436560); // Change current adjust mode (post-process script)
FunctionPointer(Sint32, GetAdjustMode, (), 0x436590); // Get current adjust mode (post-process script)
FunctionPointer(Sint32, GetCameraMode, (), 0x4365A0); // Get current camera mode (script)
FunctionPointer(void, SetCameraMode, (Sint32 mode), 0x4365B0); // Set current camera mode (script)
FunctionPointer(void, ChangeCamsetMode, (Sint8 mode), 0x4367A0); // Set current direct mode (see CDM enum)
FunctionPointer(Sint32, CameraAdditionalCollision, (NJS_POINT3* pos), 0x437E90); // Run collision against sphere/box collision cameras in the camera layout
FunctionPointer(Sint32, CameraAdditionalPlane, (NJS_POINT3* src, NJS_POINT3* pos), 0x437F20); // Run collision against plane collision cameras in the camera layout
FunctionPointer(void, CameraPositionSmooth, (NJS_POINT3* last, NJS_POINT3* pos), 0x436C60); // Intended to be a smooth lerp, but broken
FunctionPointer(void, SetStartCameraMode, (Sint8 mode), 0x436C60); // Set the starting camera mode (when the camera is initialized)
FunctionPointer(void, SetDefaultNormalCameraMode, (Sint8 mode, Sint8 adjust), 0x4345D0); // Set the default camera mode (no camera? or after an event camera release)
FunctionPointer(void, ResetCameraTimer, (), 0x436550);
CamFunc(CameraFollow, 0x462E90);
CamFunc(CameraKnuckle, 0x469590);
CamFunc(CameraMagonote, 0x463360);
CamFunc(CameraSonic, 0x46A760);
CamFunc(CameraAshland, 0x46A350);
CamFunc(CameraFishing, 0x466BB0);
CamFunc(CameraFixed, 0x4674B0);
CamFunc(CameraKlamath, 0x46A0A0);
CamFunc(CameraLine, 0x4648D0);
CamFunc(CameraNewFollow, 0x467520);
CamFunc(CameraPoint, 0x469E90);
CamFunc(CameraSonicP, 0x46A700);
CamFunc(CameraAdvertise, 0x463000);
CamFunc(CameraBack, 0x629730);
CamFunc(CameraBack2, 0x6294B0);
CamFunc(CameraBuilding, 0x463BD0);
CamFunc(CameraCart, 0x46A490);
CamFunc(CameraChaos, 0x4643B0);
CamFunc(CameraChaosPole, 0x464520);
CamFunc(CameraChaosStageInit, 0x4646F0);
CamFunc(CameraChaosStd, 0x464590);
CamFunc(CameraChaosWall, 0x464580);
CamFunc(CameraStay, 0x464DF0);
CamFunc(CameraE101R, 0x56CBF0);
CamFunc(CameraE103, 0x467990);
CamFunc(CameraEgm3, 0x57D6B0);
CamFunc(CameraFollowG, 0x467740);
CamFunc(CameraLR, 0x4627B0);
CamFunc(CameraEventPoint, 0x464A00);
CamFunc(PathCamera1, 0x4653E0);
CamFunc(PathCamera2Core, 0x465E20);
CamFunc(CameraRuinWaka1, 0x4676C0);
CamFunc(CameraSnowboard, 0x468A20);
CamFunc(CameraSurvey, 0x465F50);
CamFunc(CameraTaiho, 0x469350);
CamFunc(CameraTornadeInside, 0x463860);
CamFunc(CameraTwoHares, 0x466CE0);
CamFunc(CameraLeave, 0x467680);
CamFunc(CameraAvoid, 0x467B70);
CamFunc(CameraModeEditor, 0x463050);
CamFunc(CameraGuriGuri, 0x4631E0);
CamFunc(CameraScanPath, 0x463970);
CamFunc(CameraSurvey_BackKos, 0x466240);
CamFunc(CameraFishingCatch, 0x46E4C0);
CamFunc(CameraLureAndFish, 0x46E9A0);
CamAdjustFunc(AdjustNone, 0x467D80);
CamAdjustFunc(AdjustNormal, 0x467DC0);
CamAdjustFunc(AdjustSlow, 0x468000);
CamAdjustFunc(AdjustThreePoint, 0x469D10);
CamAdjustFunc(AdjustForFreeCamera, 0x468800);

static const void* const CameraViewSetPtr = (void*)0x435600;
// Apply camera view (view matrix, projection matrix, etc.)
static inline void CameraViewSet(taskwk* twp) 
{
	__asm
	{
		mov eax, [twp]
		call CameraViewSetPtr
	}
}

static const void* const CameraCollisitonCheckPtr = (void*)0x462B40;
// Run collision with environement, should be used in camera scripts
static inline void CameraCollisitonCheck(NJS_POINT3* src, NJS_POINT3* dst)
{
	__asm
	{
		mov edi, [dst]
		mov esi, [src]
		call CameraCollisitonCheckPtr
	}
}

static const void* const CameraCollisitonCheckAdjPtr = (void*)0x462CF0;
// Run collision with environement, should be used in camera adjusts (post-process scripts)
static inline void CameraCollisitonCheckAdj(NJS_POINT3* src, NJS_POINT3* dst)
{
	__asm
	{
		mov edi, [dst]
		mov esi, [src]
		call CameraCollisitonCheckAdjPtr
	}
}

// NJD_CONTROL_3D functions
VoidFunc(SaveControl3D, 0x4395C0); // Backs up NJD_CONTROL_3D flags
VoidFunc(LoadControl3D, 0x4395D0); // Restores NJD_CONTROL_3D flags
FunctionPointer(void, OnControl3D, (NJD_CONTROL_3D flag), 0x4395E0); // Adds an NJD_CONTROL_3D flag
FunctionPointer(void, OffControl3D, (NJD_CONTROL_3D flag), 0x439600); // Removes an NJD_CONTROL_3D flag

// Ninja draw functions
FunctionPointer(void, njDrawModel_, (NJS_MODEL_SADX* mdl), 0x784AE0); // Offloads to polybuff drawing functions
FunctionPointer(void, njDirectDrawModel, (NJS_MODEL_SADX* mdl), 0x77EDA0);
FunctionPointer(void, njDrawPolygon2D, (NJS_POINT2COL* p, int count, float pri, NJD_DRAW attr), 0x77DFC0);
FunctionPointer(void, njDrawPolygon3D, (NJS_POINT3COL* p, int n, int atr), 0x77EAD0);
FunctionPointer(void, njDrawSprite2D, (NJS_SPRITE* sp, int n, float pri, NJD_SPRITE attr), 0x77E050);
FunctionPointer(void, njDrawSprite3D, (NJS_SPRITE* sp, int n, NJD_SPRITE attr), 0x77E390);
FunctionPointer(void, njDrawLine3D, (NJS_POINT3COL* p, int n, NJD_DRAW attr), 0x77E820);
FunctionPointer(void, njDrawLine2D, (NJS_POINT2COL* p, int n, float pri, NJD_DRAW attr), 0x77DF40);
FunctionPointer(void, njDrawTriangle3D, (NJS_POINT3COL* p, int n, NJD_DRAW atr), 0x77EBA0);
FunctionPointer(void, njDrawTexture, (NJS_TEXTURE_VTX* p, Int count, Uint32 gbix, Int flag), 0x77DC70);
FunctionPointer(void, njDrawQuadTextureEx, (NJS_QUAD_TEXTURE_EX* p), 0x77DE10);

// Direct draw functions
FunctionPointer(void, njDrawModel, (NJS_MODEL_SADX* mdl), 0x77EF70);
FunctionPointer(void, njDrawObject, (NJS_OBJECT* object), 0x77EF50);
FunctionPointer(void, njActionOld, (NJS_ACTION* action, float frame), 0x789560);

// Easy draw functions
FunctionPointer(int, njEasyDrawModel, (NJS_MODEL* model), 0x4084F0);
FunctionPointer(void, njEasyDrawObject, (NJS_OBJECT* object), 0x40A100);
FunctionPointer(void, njEasyDrawMotion, (NJS_OBJECT* object, NJS_MOTION* motion, float frame), 0x406FA0);
FunctionPointer(void, njEasyDrawShapeMotion, (NJS_OBJECT* object, NJS_MOTION* motion, NJS_MOTION* shape, float frame), 0x406FE0);

// Simple draw functions
FunctionPointer(int, dsSimpleDrawModel, (NJS_MODEL_SADX* mdl), 0x401430);
FunctionPointer(void, dsDrawModel, (NJS_MODEL_SADX* mdl), 0x407970);
FunctionPointer(void, dsDrawObject, (NJS_OBJECT* object), 0x408530);
FunctionPointer(void, dsDrawModel_S, (NJS_MODEL_SADX* mdl), 0x40A140);
FunctionPointer(void, ds_DrawModelClip, (NJS_MODEL_SADX* mdl, float clipScl), 0x407A00);
FunctionPointer(void, ds_DrawObjectClip, (NJS_OBJECT* object, float clipScl), 0x4085A0);
FunctionPointer(void, dsDrawMotion, (NJS_OBJECT* object, NJS_MOTION* motion, float frame), 0x407040);
FunctionPointer(void, dsDrawMotionClip , (NJS_OBJECT* object, NJS_MOTION* motion, float frame, float clipScl), 0x405370);
FunctionPointer(void, dsDrawMotionLink, (NJS_OBJECT* object, NJS_MOTION_LINK* motionLink, float frame), 0x406C20);
FunctionPointer(void, dsActionLink, (NJS_ACTION_LINK* actionLink, float frame), 0x406EC0);
FunctionPointer(void, ds_ActionClip, (NJS_ACTION* action, float frame, float clipScl), 0x405450);
FunctionPointer(void, ds_ActionClipNoScale, (NJS_ACTION* action, float frame), 0x407020); // Same as ds_ActionClip but with scale 1.0; inlined in X360
FunctionPointer(void, dsDrawShapeMotion, (NJS_OBJECT* object, NJS_MOTION* motion, NJS_MOTION* shape, float frame), 0x406030);
FunctionPointer(void, ds_DrawBoxFill2D, (float left, float top, float right, float bottom, float pri, unsigned int argb), 0x4071C0);
FunctionPointer(void, dsDrawSprite2D, (NJS_SPRITE* sp, int n, float pri, NJD_SPRITE attr), 0x4070A0);
FunctionPointer(void, dsDrawSprite3D, (NJS_SPRITE* sp, int n, NJD_SPRITE attr), 0x407130);
FunctionPointer(void, put3Dsprite, (NJS_SPRITE* sp, Int n, NJD_SPRITE attr), 0x527930); // Draws Hedgehog Hammer score

// Regular draw functions
FunctionPointer(void, __DrawModel, (NJS_MODEL_SADX* mdl), 0x403470);
FunctionPointer(int, DrawObject, (NJS_OBJECT* model), 0x4034B0);
FunctionPointer(void, DrawObjectClip, (NJS_OBJECT* object, float clipScl), 0x4037F0);
FunctionPointer(void, DrawModelEx, (NJS_MODEL_SADX* mdl, LATE flgs), 0x407BB0);
FunctionPointer(void, DrawObjectClipEx, (NJS_OBJECT* object, LATE flgs, float clipScl), 0x409080);
FunctionPointer(void, DrawModelMS, (NJS_MODEL_SADX* mdl, LATE flgs), 0x407CF0);
FunctionPointer(void, DrawModelMesh, (NJS_MODEL_SADX* mdl, LATE flgs), 0x407FC0);
FunctionPointer(void, DrawObjectMS, (NJS_OBJECT* object, LATE flgs, float clipScl), 0x409550);
FunctionPointer(void, DrawObjectClipMesh, (NJS_OBJECT* object, LATE flgs, float clipScl), 0x409A20);

// void __usercall(NJS_ACTION* action@<eax>, float frame, LATE flgs, float clpScl, void* drwMdlFnc)
static const void* const DrawActionPtr = (void*)0x4053D0;
static inline void DrawAction(NJS_ACTION* action, float frame, LATE flgs, float clpScl, void* drwMdlFnc)
{
	__asm
	{
		push[drwMdlFnc]
		push[clpScl]
		push dword ptr[flgs]
		push[frame]
		mov eax, [action]
		call DrawActionPtr
		add esp, 16
	}
}

// void __usercall(obj *object@<edi>, NJS_MOTION *motion@<eax>, float frame, LATE flgs, float clpScl, void *drwMdlFnc)
static const void* const DrawMotionPtr = (void*)0x4052F0;
static inline void DrawMotion(NJS_OBJECT* object, NJS_MOTION* motion, float frame, LATE flgs, float clpScl, void* drwMdlFnc)
{
	__asm
	{
		push[drwMdlFnc]
		push[clpScl]
		push dword ptr[flgs]
		push[frame]
		mov eax, [motion]
		mov edi, [object]
		call DrawMotionPtr
		add esp, 16
	}
}

// void __usercall(obj *object, NJS_MOTION_LINK *motionlink@<eax>, float frame, LATE flgs, float clpScl, void *drwMdlFnc)
static const void* const DrawMotionLinkPtr = (void*)0x4069A0;
static inline void DrawMotionLink(NJS_OBJECT* object, NJS_MOTION_LINK* motionlink, float frame, LATE flgs, float clpScl, void* drwMdlFnc)
{
	__asm
	{
		push[drwMdlFnc]
		push[clpScl]
		push dword ptr[flgs]
		push[frame]
		push[object]
		mov eax, [motionlink]
		call DrawMotionLinkPtr
		add esp, 20
	}
}

// void __usercall(obj *object, NJS_MOTION *motion@<eax>, NJS_MOTION *shape@<edi>, float frame, LATE flgs, float clpScl, void *drawModel)
static const void* const DrawShapeMotionPtr = (void*)0x405EF0;
static inline void DrawShapeMotion(NJS_OBJECT* object, NJS_MOTION* motion, NJS_MOTION* shape, float frame, LATE flgs, float clpScl, void* drawModel)
{
	__asm
	{
		push[drawModel]
		push[clpScl]
		push dword ptr[flgs]
		push[frame]
		push[object]
		mov edi, [shape]
		mov eax, [motion]
		call DrawShapeMotionPtr
		add esp, 20
	}
}

// Quaternion functions
FunctionPointer(void, njInterpolateQuaternion, (NJS_QUATERNION* q1, NJS_QUATERNION* q2, Float t, NJS_QUATERNION* qd), 0x793790);
FunctionPointer(void, njZXYAngleToQuaternion, (Angle3* ang, NJS_QUATERNION* q), 0x793900);
FunctionPointer(void, njXYZAngleToQuaternion, (Angle3* ang, NJS_QUATERNION* q), 0x7939C0);

// Variable draw functions (picks between Simple and Late)
FunctionPointer(void, lateDrawModel, (NJS_MODEL_SADX* mdl, LATE flgs), 0x4048E0);
FunctionPointer(void, lateDrawObject, (NJS_OBJECT* object, LATE flgs, float clipScl), 0x4074A0);

// Late draw functions: Model
FunctionPointer(void, late_DrawModel, (NJS_MODEL_SADX* mdl, LATE flgs), 0x4078F0);
FunctionPointer(void, late_DrawModelClip, (NJS_MODEL_SADX* mdl, LATE flgs, float clipScl), 0x407870);
FunctionPointer(void, late_DrawModelEx, (NJS_MODEL_SADX* mdl, LATE flgs), 0x409450);
FunctionPointer(void, late_DrawModelClipEx, (NJS_MODEL_SADX* mdl, LATE flgs, float clipScl), 0x4094D0);
FunctionPointer(void, late_DrawModelMS, (NJS_MODEL_SADX* mdl, LATE flgs), 0x409920);
FunctionPointer(void, late_DrawModelClipMS, (NJS_MODEL_SADX* mdl, LATE flgs, float clipScl), 0x4099A0);
FunctionPointer(void, late_DrawModelMesh, (NJS_MODEL_SADX* mdl, LATE flgs), 0x409DF0);
FunctionPointer(void, late_DrawModelClipMesh, (NJS_MODEL_SADX* mdl, LATE flgs, float clipScl), 0x409E70);
FunctionPointer(void, late_DrawShadowModel, (NJS_MODEL_SADX* mdl, float clipScl), 0x407B00);

// Late draw functions: Object
FunctionPointer(void, late_DrawObject, (NJS_OBJECT* object, LATE flgs), 0x407B70);
FunctionPointer(void, late_DrawObjectClip, (NJS_OBJECT* object, LATE flgs, float clipScl), 0x407B40);
FunctionPointer(void, late_DrawObjectEx, (NJS_OBJECT* object, LATE flgs), 0x40A1A0);
FunctionPointer(void, late_DrawObjectClipEx, (NJS_OBJECT* object, LATE flgs, float clipScl), 0x40A170);
// late_DrawObjectMS does not exist
FunctionPointer(void, late_DrawObjectClipMS, (NJS_OBJECT* object, LATE flgs, float clipScl), 0x40A1E0);
FunctionPointer(void, late_DrawObjectMesh, (NJS_OBJECT* object, LATE flgs), 0x40A210);
FunctionPointer(void, late_DrawObjectClipMesh, (NJS_OBJECT* object, LATE flgs, float clipScl), 0x40A250);
FunctionPointer(void, late_DrawShadowObject, (NJS_OBJECT* object, float clipScl), 0x408690);

// Late draw functions: Motion
FunctionPointer(void, late_DrawMotionClip, (NJS_OBJECT* object, NJS_MOTION* motion, float frame, LATE flgs, float clipScl), 0x4053A0);
FunctionPointer(void, late_DrawMotionClipEx, (NJS_OBJECT* object, NJS_MOTION* motion, float frame, LATE flgs, float clipScl), 0x4082D0);
FunctionPointer(void, late_DrawMotionClipMesh, (NJS_OBJECT* object, NJS_MOTION* motion, float frame, LATE flgs, float clipScl), 0x408300);

// Late draw functions: Action
FunctionPointer(void, late_Action, (NJS_ACTION* action, float frame, LATE flgs), 0x405470);
FunctionPointer(void, late_ActionClip, (NJS_ACTION* action, float frame, LATE flgs, float clipScl), 0x405490);
FunctionPointer(void, late_ActionEx, (NJS_ACTION* action, float frame, LATE flgs), 0x408330);	
FunctionPointer(void, late_ActionClipEx, (NJS_ACTION* action, float frame, LATE flgs, float clipScl), 0x408350);
FunctionPointer(void, late_ActionMS, (NJS_ACTION* action, float frame, LATE flgs), 0x408380);
FunctionPointer(void, late_ActionClipMS, (NJS_ACTION* action, float frame, LATE flgs, float clipScl), 0x4083A0);
FunctionPointer(void, late_ActionMesh, (NJS_ACTION* action, float frame, LATE flgs), 0x4083D0);
FunctionPointer(void, late_ActionClipMesh, (NJS_ACTION* action, float frame, LATE flgs, float clipScl), 0x4083F0);

// Late draw functions: ActionLink
FunctionPointer(void, late_ActionLink, (NJS_ACTION_LINK* actionLink, float frame, LATE flgs), 0x406EF5);
FunctionPointer(void, late_ActionLinkEx, (NJS_ACTION_LINK* actionLink, float frame, LATE flgs), 0x4084B0);
FunctionPointer(void, late_ActionLinkMesh, (NJS_ACTION_LINK* actionLink, float frame, LATE flgs), 0x4084D0);

// Late draw functions: MotionLink
FunctionPointer(void, late_DrawMotionLinkEx, (NJS_OBJECT* object, NJS_MOTION_LINK* motionLink, float frame, LATE flgs), 0x408480);

// Late draw functions: Shape motion
FunctionPointer(void, late_DrawShapeMotionEx, (NJS_OBJECT* object, NJS_MOTION* motion, NJS_MOTION* shape, float frame, LATE flgs), 0x408420);
FunctionPointer(void, late_DrawShapeMotionMesh, (NJS_OBJECT* object, NJS_MOTION* motion, NJS_MOTION* shape, float frame, LATE flgs), 0x408450);

// Late draw functions: Other functions
FunctionPointer(void, late_DrawTriangle3D, (NJS_POINT3COL* p, int n, NJD_DRAW atr, LATE flgs), 0x4043D0);
FunctionPointer(void, late_DrawPolygon2D, (NJS_POINT2COL* p, int n, float depth, NJD_DRAW atr, LATE flgs), 0x404490);
FunctionPointer(void, late_DrawPolygon3D, (NJS_POINT3COL* p, int n, NJD_DRAW atr, LATE flgs), 0x4045A0);
FunctionPointer(void, late_DrawLine3D, (NJS_POINT3COL* p, int n, NJD_DRAW attr, LATE flags), 0x404310);
FunctionPointer(void, late_DrawBoxFill2D, (float x, float y, float x2, float y2, float pri, int argb, LATE flgs), 0x4073B0);
FunctionPointer(void, late_DrawSprite2D, (NJS_SPRITE* sp, int n, float pri, NJD_SPRITE atr, LATE flgs), 0x404660);
FunctionPointer(void, late_DrawSprite3D, (NJS_SPRITE* sp, int n, NJD_SPRITE atr, LATE flgs), 0x404760);
FunctionPointer(void, ___njDrawSprite3D, (NJS_SPRITE* _sp, Int n, NJD_SPRITE attr), 0x407070); // Uses late_DrawSprite3D, disables Z write with NJD_SPRITE_ALPHA
FunctionPointer(void, saDrawSprite3D, (NJS_SPRITE* _sp, Int n, NJD_SPRITE attr), 0x4072F0); // No Z write, uses sa_s3_z_ofs for depth
FunctionPointer(void, ___SAnjDrawPolygon2D, (NJS_POINT2COL* p, int n, float pri, unsigned int attr), 0x4010D0);
FunctionPointer(void, __njDrawPolygon2D, (NJS_POINT2COL* p, int n, float pri, NJD_DRAW atr), 0x403C60);

// Chaos draw functions
FunctionPointer(void, CHAOS_DrawModel, (NJS_MODEL_SADX* mdl), 0x409EF0);
FunctionPointer(void, CHAOS_DrawObject, (NJS_OBJECT* object), 0x40A280);
FunctionPointer(void, CHAOS_Action, (NJS_ACTION* action, float frame), 0x409FB0);
FunctionPointer(void, CHAOS_DrawShapeMotion, (NJS_OBJECT* object, NJS_MOTION* motion, NJS_MOTION* shape, float frame), 0x40A050);

// Demos
FunctionPointer(void, dsGoDemoLoop, (), 0x0042C820); // Sets up demo stuff
FunctionPointer(int, dsReturnCheck, (), 0x0042CAA0); // Runs after demos

// FMVs
FunctionPointer(int, movie, (int movie_num), 0x005130A0);
FunctionPointer(bool, MsMovie, (int n), 0x0042F020); // Plays videos
FunctionPointer(int, CheckMovieStatus, (), 0x00513D50);
FunctionPointer(int, GetNJTexture, (), 0x00513990);
FunctionPointer(void, DrawMovieTex, (int width, int height), 0x005139F0);
FunctionPointer(void, DrawFadePoly, (char fade), 0x00512D30);
FunctionPointer(signed int, FadeControl, (int video_id), 0x00512DF0);
FunctionPointer(__int64, syTmrGetCount, (), 0x007800E0);
VoidFunc(StartDShowTextureRenderer, 0x513850);
VoidFunc(EndDShowTextureRenderer, 0x00513C50);
VoidFunc(PlayDShowTextureRenderer, 0x00513D30);
VoidFunc(saito_init, 0x005136E0);
VoidFunc(saito_exit, 0x00513840);

// Event audio
FunctionPointer(void, voicevolchange, (int vol), 0x40CCF0);
FunctionPointer(void, EventSe_Init, (int size), 0x64FC80);
VoidFunc(EventSe_Close, 0x64FCB0);
FunctionPointer(void, EventSe_Play, (int id, int se, int frame), 0x64FD00);
FunctionPointer(void, EventSe_Stop, (int id), 0x64FD40);
FunctionPointer(void, EventSe_Volume, (int id, int volume, int frame), 0x64FD80);
FunctionPointer(void, EventSe_Pan, (int id, int pan, int frame), 0x64FDB0);
FunctionPointer(void, EventSe_Pitch, (int id, int pitch, int frame), 0x64FDE0);
FunctionPointer(void, EventSe_Oneshot, (int se, int volume, int pan, int pitch), 0x64FE10);

// Event message
FunctionPointer(void, MSG_LoadTexture, (MSGC* msgc), 0x40D290);
FunctionPointer(void, MSG_LoadTexture2, (MSGC* msgc), 0x40D2A0);
FunctionPointer(void, MSG_Close, (MSGC* msgc), 0x40D450);
FunctionPointer(void, MSG_Disp, (MSGC* msgc), 0x40D490);
FunctionPointer(void, MSG_Cls, (MSGC* msgc), 0x40D850);
FunctionPointer(void, NH_MSG_Open, (MSGC* msgc, __int16 x, __int16 y, int width, int height, int globalindex, void* buf), 0x40E1E0);
FunctionPointer(void, NH_MSG_Close, (MSGC* msgc), 0x40E2C0);
FunctionPointer(void, MSG_Open, (MSGC* msgc, int x, int y, int width, int height, unsigned int globalindex), 0x40E430);
FunctionPointer(void, MSG_Puts, (MSGC* msgc, const char* str), 0x40E570);
FunctionPointer(void, EV_Msg, (const char* str), 0x42FB20); //Event Library: Open a textbox. Note: Doubles as EV_Wait(20).
VoidFunc(EV_MsgClose, 0x42FBB0); //Event Library: Close a textbox.  Note: Doubles as EV_Wait(20).
VoidFunc(EV_MsgCls, 0x42FC20); //Event Library: Close text but don't close the textbox.
FunctionPointer(void, EV_MsgW, (int time, char* str), 0x431640); //Event Library: Display a message and wait.
FunctionPointer(MSGC*, CreateMsgTexVtx, (NJS_TEXTURE_VTX* vtx, unsigned int color, float fade, float x, float y, float z, float w, float h), 0x6424A0);

// Event camera
VoidFunc(EV_CameraOn, 0x4303E0); //Event Library: Use Event Camera
VoidFunc(EV_CameraOff, 0x4304B0); //Event Library: Delete Event Camera
FunctionPointer(void, EV_CameraAng, (int mode, int time, int x, int y, int z), 0x430C90); //Event Library: Set camera rotation.
FunctionPointer(void, EV_CameraPos, (int mode, int time, float x, float y, float z), 0x430D00); //Event Library: Set camera position.
FunctionPointer(void, EV_CameraPosORY, (char mode, int time, task* tp, float x, float y, float z), 0x430D70); //Event Library: Set camera position while orbiting an object. Uncommon.
VoidFunc(EV_CameraTargetFree, 0x430E50); //Event Library: Clear CameraTarget commands.
FunctionPointer(void, EV_CameraTargetPos, (int mode, int time, float x, float y, float z, float az), 0x430E60); //Event Library: Make the camera look at a point.
FunctionPointer(void, EV_CameraTargetObj, (int mode, int time, task* tp, float x, float y, float z, int az), 0x430EE0); //Event Library: Make the camera look at an object from a position.
VoidFunc(EV_CameraChaseFree, 0x430F60); //Event Library: Clear CameraChase commands.
FunctionPointer(void, EV_CameraChase, (task* tp), 0x430F70); //Event Library: Make the camera follow an object.
/*Event Library: Make the camera follow an object while also running an animation (ROTMOVE).
* x0/y0/z0 = start angle
* x1/y1/z1 = end angle
* d0 = start distance
* d1 = end distance*/
FunctionPointer(void, EV_CameraChaseRM, (int mode, int time, task* tp, float height, int x0, int y0, int z0, float d0, int x1, int y1, int z1, float d1), 0x430FC0);
FunctionPointer(void, EV_CameraPerspective, (int mode, int time, int ang), 0x431060); //Event Library: Set FOV.
FunctionPointer(void, EV_CameraPath, (cpathtag* path, float speed), 0x4310A0); //Event Library: Make the camera follow a path.

// Event
VoidFunc(EV_CanselOn, 0x42F630); //Event Library: Enable event conditions. (Remove the HUD etc.)
VoidFunc(EV_Exec2, 0x431430); //Event Library: Setup to create an event. Place below EV_Load2.
FunctionPointer(void, EV_Load2, (int no), 0x42FA30); //Event Library: Load an event. This is what to call if wanting to play one, with Ev_Exec2 directly under it.
FunctionPointer(void, DisplayFade, (task* a1), 0x00412F70); // Event fade display function
FunctionPointer(void, SeqTaskFadeIn, (task* a1), 0x00412FE0); // Event fade-in main function
FunctionPointer(void, SeqTaskFadeOut, (task* a1), 0x00413060); // Event fade-out main function
FunctionPointer(BOOL, EV_CheckCansel, (), 0x42FB00); //Event Library: Check if any event is running. Uncommon: Only used in Chaos 0's level and for the Casinopolis doors in Tails' story.
FunctionPointer(void, EV_Wait, (int time), 0x4314D0); //Event Library: Wait X Frames.
FunctionPointer(void, EV_CreateObject, (task** tp, float px, float py, float pz, int ax, int ay, int az), 0x431670); //Event Library: Create a generic object.
FunctionPointer(void, EV_CreateObjectFunc, (task** tp, task* (*func)(void), float x, float y, float z, int rx, int ry, int rz), 0x4316C0); //Event Library: Create an object that has its own main/display functions.
FunctionPointer(void, EV_InitObject, (task* tp), 0x431730); //Event Library: Clear any existing Action/Path/Move/Face commands from a task.
FunctionPointer(void, EV_InitPlayer, (int n), 0x431780); //Event Library: Cleanup for the player before/after an event.
FunctionPointer(void, EV_WaitAction, (task* tp), 0x431810); //Event Library: Wait for a task to finish an Action command.
FunctionPointer(void, EV_WaitPath, (task* tp), 0x431840); //Event Library: Wait for a task to finish a Path command.
FunctionPointer(void, EV_PlayPad, (int no, PADREC_DATA_TAG* tag), 0x431870); //Event Library: Automated controller input
FunctionPointer(void, EV_WaitMove, (task* tp), 0x4318D0); //Event Library: Wait for a task to finish a Move command.
FunctionPointer(task*, EV_GetPlayer, (uint8_t no), 0x42FC40); //Event Library: Get task of Character in slot no.
FunctionPointer(void, EV_FreeObject, (task** tp), 0x42FC50); //Event Library: Delete an object.
FunctionPointer(void, EV_SetPos, (task* tp, float x, float y, float z), 0x42FC70); //Event Library: Set Position of a task.
FunctionPointer(void, EV_SetAng, (task* tp, int x, int y, int z), 0x42FCD0); //Event Library: Set Rotation of a task.
FunctionPointer(void, EV_SetMode, (task* tp, int mode), 0x42FD20); //Event Library: Set Mode (Action ID) of a task.
FunctionPointer(void, EV_SetShadow, (task* tp, float size), 0x42FD40); //Event Library: Set shadow size of a task.
FunctionPointer(void, EV_SetColli, (task* tp, float size), 0x42FD60); //Event Library: Set collision size of a task.
FunctionPointer(void, EV_CreatePlayer, (int n, void(__cdecl* func)(task*), float px, float py, float pz, int ax, int ay, int az), 0X42FD80); //Event Library: Create an NPC player character.
FunctionPointer(void, EV_RemovePlayer, (int n), 0x42FDE0); //Event Library: Remove an NPC player character.
/*Event Library: Play an animation.
* This is the game's primary method of displaying props such as Chaos Emeralds.
* mode: 0 (Don't loop) or 1 (Loop)
* linkframe: Interpolation frames between the previous Action and this one.
* linkframe only works with actions called by this or EV_SetMotion.
* Animations are queued.*/
FunctionPointer(void, EV_SetAction, (task* tp, NJS_ACTION* ap, NJS_TEXLIST* lp, float speed, int mode, int linkframe), 0x42FE00);
/*Event Library:Play an animation without a premade NJS_ACTION.
* Mainly used for objects with shared animations, such as NPCs and tribal Echidnas.*/
FunctionPointer(void, EV_SetMotion, (task* tp, NJS_OBJECT* op, NJS_MOTION* mp, NJS_TEXLIST* lp, float speed, int mode, int linkframe), 0x42FE20);
FunctionPointer(void, EV_ClrAction, (task* tp), 0x42FE40); //Event Library: Clear EV_SetAction/Motion commands.
FunctionPointer(void, EV_SetPath, (task* tp, EPATHTAG* path, float speed, int mode), 0x42FE60); //Event Library: Make an object follow a path.
FunctionPointer(void, EV_ClrPath, (task* tp), 0x42FE80); //Event Library: Clear EV_SetPath commands.
FunctionPointer(void, EV_LookFree, (task* tp), 0x42FFB0); //Event Library: Clear Look commands.
FunctionPointer(void, EV_LookObject, (task* tp, task* target, float x, float y, float z), 0x42FFD0); //Event Library: Focus a character's head on an object.
FunctionPointer(void, EV_LookPoint, (task* tp, float x, float y, float z), 0x430000); //Event Library: Focus a character's head on a point.
FunctionPointer(void, EV_LookAngle, (task* tp, int x, int y, int z), 0x430030); //Event Library: Rotate a character's head.
FunctionPointer(void, EV_MoveFree, (task* tp), 0x430060); //Event Library: Clear Move
FunctionPointer(void, EV_MovePoint, (task* tp, float x, float y, float z), 0x430080); //Event Library: Move a character to a point at a fixed speed.
FunctionPointer(void, EV_MovePoint2, (task* tp, float x, float y, float z, float s, float a), 0x4300D0); //Event Library: Move a character to a point at an adjustable speed and acceleration.
FunctionPointer(void, EV_MoveRotation, (task* tp, int x, int y, int z), 0x430120); //Event Library: Rotate a character.
FunctionPointer(void, AmyForEventHammerScaleIm, (uint8_t playernmb, float scale), 0x486EC0); //Event: Set the scale of Amy's Hammer.
FunctionPointer(void, stopObject, (task* tp), 0x6EBAB0); //Event: Stop moveObject on a task.
VoidFunc(stopObjectAll, 0x6EBB00); //Event: Stop moveObject on all tasks. Mainly used for cleanup at the end of events.
FunctionPointer(void, moveObject, (task* obj, float st_x, float st_y, float st_z, float end_x, float end_y, float end_z, int frame), 0x6EC2B0); //Event Move object: Position only
FunctionPointer(void, moveObjectOn, (task* tp, float x, float y, float z, int frame, task* dest_obj), 0x6EC6D0); //Event Move object: Position, relative to dest_obj.
/*Event Move object: Position + Angle, relative to dest_obj.
Only used once - Amy's Flicky in EV0063 (Twinkle Park).*/
FunctionPointer(void, moveObjectOnAngle, (task* obj, float far_x, float far_y, float far_z, int ang_x, int ang_y, int ang_z, int frame, task* dest_obj), 0x6EC790);
/*Move object: Position relative to dest_obj. Make obj face the same direction as dest_obj.
//Only used once - Froggy following Big in his ending, after the fishing sequence.*/
FunctionPointer(void, moveObjectOnRotate, (task* obj, float far_x, float far_y, float far_z, signed int frame, task* dest_obj), 0x6EC8B0);
/*Move object: Position relative to the right hand of a given player character.
Only used twice: EV0087 (Eggman holding a Chaos Emerald) and EV0066 (Amy holding a balloon)*/
FunctionPointer(void, moveObjectRightHand, (task* obj, char player_no, float far_x, float far_y, float far_z, int frame), 0x6ECA20);
FunctionPointer(void, addmotModel, (int model, NJS_ACTION* action, int mot_id), 0x6ECAC0); //Event: Add an animation to a model placed with createModel.
FunctionPointer(void, playModel, (int model_id, int mot_id, float mot_spd, int loop_num), 0x6ECAE0); //Event: Play animation (added with addmotModel) in a model placed with createModel.
FunctionPointer(void, stopModel, (int model_id), 0x6ECB20); //Event: Stop animation in a model placed with createModel.
FunctionPointer(task*, getobjModel, (int model_id), 0x6ECB40); //Event: Get a model placed with createModel.
FunctionPointer(void, deleteModel, (int model_id), 0x6ECB50); //Event: Delete a model placed with createModel.
FunctionPointer(void, createModel, (float pos_x, float pos_y, float pos_z, //Event: Place a model. Uncommon, mostly ignored in favor of EV_CreateObject.
	int ang_x, int ang_y, int ang_z, float scl_x, float scl_y, float scl_z,
	NJS_OBJECT* model, NJS_TEXLIST* texlist, int model_id), 0x6ECE70); 
FunctionPointer(void, createModelEC, (float pos_x, float pos_y, float pos_z,  //Event: Place a model. (Used Exclusively for low-poly Egg Carrier)
	int ang_x, int ang_y, int ang_z, float scl_x, float scl_y, float scl_z,
	NJS_OBJECT* model, NJS_TEXLIST* texlist, int model_id), 0x6ECF20);
VoidFunc(EV_PadOn, 0x42F610); //Enable Controls
VoidFunc(EV_PadOff, 0x42F620); //Disable Controls
/*Event Library: Play a face animation.
* Each letter in the string represents a different face animation.
* A->Z (Uppercase) a->e (lowercase)
* Common combos: "CDE" (Talking) "F" (Buggin' face)
* End string with 0 or use EV_ClrFace to reset the head.
* By default this only works for Sonic, Tails, Knuckles, Amy and Tikal.
* Big works too, but is severely broken and will probably crash the game.*/
FunctionPointer(void, EV_SetFace, (task* tp, const char* str), 0x4310D0);
FunctionPointer(void, EV_ClrFace, (task* tp), 0x4310F0); //Event Library: Reset a character's face.
VoidFunc(EV_SerifStop, 0x431110); //Event Library: Stop a voice clip.
VoidFunc(EV_SerifWait, 0x431900); //Event Library: Wait for a voice clip to end.
FunctionPointer(void, EV_SerifPlay, (int id), 0x431930); //Event Library: Play a voice clip
FunctionPointer(void, tikal_dispSwitch, (int sw), 0x4A3520); //Event: Turns off in-game Tikal orbs in events
FunctionPointer(void, EggrobForEvent0_RP_Set, (NJS_POINT3* tgt), 0x4D1740); //Event: Make Event ZERO attack a point.
FunctionPointer(void, EvSetTableBgMR, (), 0x52FB20); //Event: Auto-Adjust MR Act 1 Skybox to whatever ClipLevel is set to. Only used in one event (ZERO boarding Egg Carrier).
FunctionPointer(void, shadowOn, (char num, float pos_x, float pos_y, float pos_z, signed int bigInt, signed int smallInt, float scaleMax), 0x6A6680); //Event: Large shadow animation used by ZERO.
FunctionPointer(void, moveObjectAngle, (task* obj, //Event Move object: Position + Angle (Given as float). Only used once - by E101 Mk.II after shooting Gamma.
	float st_x, float st_y, float st_z,
	float end_x, float end_y, float end_z,
	float st_ang_x, float st_ang_y, float st_ang_z,
	float end_ang_x, float end_ang_y, float end_ang_z,
	signed int frame), 0x6EC3A0);
FunctionPointer(void, moveObjectAngle2, (task* obj, float st_x, float st_y, float st_z, //Event Move object: Position + Angle.
	float en_x, float en_y, float en_z, int st_ang_x, int st_ang_y, int st_ang_z,
	int en_ang_x, int en_ang_y, int en_ang_z, signed int frame), 0x6EC580);
FunctionPointer(task*, CTikalLight_Create, (float x, float y, float z), 0x6ED090); //Event Tikal Ball
FunctionPointer(void, effect_delete, (__int16 ef_num), 0x6ED490); //Event: Delete effect.
FunctionPointer(void, effect_create2, (task* obj, int ef_num, float ef_x, float ef_y, float ef_z, NJS_TEXLIST* ef_name), 0x6ED770); //Event: Effect generator
/*Event: Alternate effect generator. Only used before ev_ef_kiran (Shooting star) in Big's intro and Knuckles' ending.
Note: Its display function is broken. It assumes every sprite has 8 frames.*/
FunctionPointer(void, effect_create, (task* obj, __int16 ef_num, float ef_x, float ef_y, float ef_z, NJS_TEXLIST* ef_name), 0x6ED580);
FunctionPointer(void, effect_color, (__int16 ef_num, float col_a, float col_r, float col_g, float col_b), 0x6ED910); //Event: Set effect color.
FunctionPointer(void, effect_size_change, (int ef_num, float speed_x, float speed_y), 0x6ED950); //Event: Set effect scale. Uncommon, Only used at the end of Sonic's Ancient Light event.
FunctionPointer(void, effect_anim_speed, (__int16 ef_num, float anim_speed), 0x6ED980); //Event: Set effect animation speed. Only used for fire effect behind Chaos in Super Sonic's flashback.
VoidFunc(crushLightOff, 0x6ED9A0); //Event: Remove light beams.
FunctionPointer(void, crushLightOn, (float pos_x, float pos_y, float pos_z, int crushLightNum, int life,  //Chaos transformation light beams
	float start_width, float end_width, unsigned int start_col, unsigned int end_col), 0x6EDA20);
FunctionPointer(void, effect_scl, (int ef_num, float sx, float sy), 0x6ED8E0); //Event: Set effect scale. 
VoidFunc(DeleteChaos0, 0x6EDAB0); //Event: Delete Event Chaos 0.
FunctionPointer(void, Jump2Chaos0, (float pos_x, float pos_y, float pos_z, //Event: Make Event Chaos 0 Jump
	float jumph, signed int frame), 0x6EDAD0);
FunctionPointer(void, Jump3Chaos0, (float pos_x, float pos_y, float pos_z, //Event: Make Event Chaos 0 Jump (Alternate Animation) Only used in Knuckles' intro.
	float jumph, signed int frame), 0x6EDBA0);
FunctionPointer(void, MoveChaos0, (float pos_x, float pos_y, float pos_z, signed int frame), 0x6EDC70); //Event: Move Event Chaos 0. NOTE: He has no Walking animation, make sure he's in Puddle form.
FunctionPointer(task*, GetChaos0, (), 0x6EDCD0); //Event: Get Event Chaos 0's task.
VoidFunc(ToWaterChaos0, 0x6EDD00); //Event: Make Event Chaos 0 retreat into puddle.
VoidFunc(ToHumanChaos0, 0x6EDD20); //Event: Make Event Chaos 0 emerge from puddle.
FunctionPointer(void, SetAngleChaos0, (int ang_x, int ang_y, int ang_z), 0x6EDD60);
VoidFunc(SetWaterChaos0, 0x6EDD80); //Event: Set Event Chaos 0 as puddle form.
FunctionPointer(void, ChangeMotspd, (float mod_spd), 0x6EDCE0); //Event: Set Event Chaos 0's movement speed.
FunctionPointer(void, SetPositionChaos0, (float pos_x, float pos_y, float pos_z), 0x6EDD40); //Event: Set Event Chaos 0's Position.
FunctionPointer(void, SetSclChaos0, (float scl_x, float scl_y, float scl_z), 0x6EDDB0); //Event: Set Event Chaos 0's Scale.
VoidFunc(CngMotStand2Chaos0, 0x6EDDD0); //Event: Set Event Chaos 0 looking to left. Only used in Ending.
VoidFunc(CngMotStand4Chaos0, 0x6EDDE0); //Event: Set Event Chaos 0 looking forward. Only used in Ending.
VoidFunc(CngMotFlyChaos0, 0x6EDDF0); //Event Set Event Chaos 0 looking up. Only used in Ending.
FunctionPointer(void, CreateChaos0, (float pos_x, float pos_y, float pos_z, //Event: Create Chaos 0 Event Actor
	int ang_x, int ang_y, int ang_z, signed int cng_int), 0x6EE930); 
FunctionPointer(void, SetEffectRotaryEmerald, (int eme_num, NJS_TEXLIST* ef_name, float scale, float col_a, float col_r, float col_g, float col_b), 0x6EEA30); //Event: Create effect for RotaryEmerald (Emerald glowing etc.)
FunctionPointer(void, SplashRotaryEmerald, (float splash_spd), 0x6EEAC0); //Event: Scatter RotaryEmerald objects
VoidFunc(deleteRotaryEmeraldTask, 0x6EEE00); //Event: delete RotaryEmerald.
FunctionPointer(void, CreateRotaryEmerald, (float pos_x, float pos_y, float pos_z, //Event: Controller for Emeralds spinning in a circle. Can be given any 7 tasks.
	float r, int rot_spd, task* eme1_task_p, task* eme2_task_p, task* eme3_task_p,
	task* eme4_task_p, task* eme5_task_p, task* eme6_task_p, task* eme7_task_p), 0x6EEF40); 
FunctionPointer(void, EV_Alife_FaceChange, (task* tp, int texid), 0x6EEFD0); //Event: Make Tikal's Chao change facial expressions.
FunctionPointer(task*, EV_Alife, (), 0x6EF2C0); //Event: Tikal's Chao. Create with EV_CreateObjectFunc.
FunctionPointer(task*, COverlayCreate, (float s, float a, float r, float g, float b), 0x6EF480); //Event: Create Color Overlay
FunctionPointer(void, COverlaySetSpeed, (task* _this, float s), 0x6EF4C0); //Event: Set Overlay transition speed.
FunctionPointer(void, COverlaySetAlpha, (task* _this, float a), 0x6EF4D0); //Event: Set Overlay alpha. Use COverlaySetSpeed to control how fast it transitions.
FunctionPointer(void, COverlaySetColor, (task* _this, float r, float g, float b), 0x6EF4E0); //Event: Set Overlay color.
FunctionPointer(void, COverlaySetPriority, (task* _this, float p), 0x6EF500); //Event: Set Overlay distance.
FunctionPointer(task*, CIchimaie2_Create, (NJS_TEXLIST* texlistp, char mode), 0x6EF680); //Event: Create Image Overlay. Used for Flashbacks and Sonic's intro.
FunctionPointer(void, CIchimaie2_SetDstAlpha, (task* _this, float alpha, int frame), 0x6EF6F0); //Event: Set Image Overlay alpha and transition speed.
FunctionPointer(task*, CIchimaie2_SetPriority, (task* _this, float pri), 0x6EF710); //Event: Set Overlay distance.
FunctionPointer(task*, CIchimaie2_SetTextureId, (task* _this, int id), 0x6EF720); //Event: Set Overlay texid (from the list given to CIchimaie2_Create).
FunctionPointer(void, CIchimaie2_SetBlendingMode, (task* _this, int index, char mode), 0x6EF730); //Event: Set the blending mode for Image Overlay if transparent.
VoidFunc(deleteScanLine, 0x6EF750); //Event: Delete E102 scanline effect.
FunctionPointer(void, createScanLine, (unsigned __int8 a, unsigned __int8 r, unsigned __int8 g, unsigned __int8 b, int res), 0x6EF940); //Event: E102 1st Person Scanlines
FunctionPointer(task*, CSkyWalk_create2, (task* obj, float height), 0x6EF9C0); //Event: Create invisible floor for task obj at height y.
VoidFunc(Last1AHigh_Create, 0x6EFAF0); //Event: Create the Platform for Perfect Chaos Scenes.
VoidFunc(Last1AHigh_Delete, 0x6EFB10); //Event: Delete the Platform for Perfect Chaos Scenes.
VoidFunc(efWhiteOff, 0x6EFB30); //Event: Delete White fade effect.
FunctionPointer(void, efWhiteOn, (int in_int, int out_int), 0x6EFCA0); //Event: Automated Fade to White. Uncommon. Only used by Knuckles reacting to the Chaos 4>6 transformation.
FunctionPointer(void, efWhiteOn2, (int in_int, int white_int, int out_int), 0x6EFD00);  //Event: Automated Fade to white, mainly used by Tikal
FunctionPointer(void, StgChaos7CtrlOff, (int fexit), 0x6EFD70); //Event: Return Perfect Chaos to usual behavior.
VoidFunc(StgChaos7Howl1, 0x6EFDF0); //Event: Make Perfect Chaos roar.
FunctionPointer(void, StgChaos7Breath, (float length), 0x6EFE00); //Event: Make Perfect Chaos shoot his mouth laser.
FunctionPointer(void, StgChaos7SetPos, (float pos_x, float pos_y, float pos_z), 0x6EFE10); //Event: Set Perfect Chaos' Position.
FunctionPointer(void, StgChaos7SetAng, (int ang_x, int ang_y, int ang_z), 0x6EFE60); //Event: Set Perfect Chaos' Angle.
FunctionPointer(void, StgChaos7CtrlOn, (float pos_x, float pos_y, float pos_z, int ang_x, int ang_y, int ang_z, int rot_spd), 0x6EFEC0); //Event: Disable boss behavior for Perfect Chaos.
FunctionPointer(task*, SetEventBirdie0, (), 0x6F0070); //Event: Setup for Amy's Flicky.
FunctionPointer(task*, SetEventBirdie1, (), 0x6F00A0); //Event: Setup for E102's Flicky.
FunctionPointer(task*, SetEventBirdie2, (), 0x6F00D0); //Event: Setup for E101's Flicky.
FunctionPointer(void, create_eggmoble, (float x, float y, float z, int ax, int ay, int az), 0x6F0940); //Event: Create Eggmobile.
VoidFunc(delete_eggmoble, 0x6F09A0); //Event: Delete Eggmobile.
FunctionPointer(void, ChgEggMobleMod, (char mode), 0x6F09C0); //Event: Set Eggmobile hover type.
FunctionPointer(void, ChgEggMobleSMod, (char mode), 0x6F09E0);  //Event: Set Eggmobile jet type.
FunctionPointer(void, eggmoble_move_normal, (float x, float y, float z, signed int frame), 0x6F0A00); //Event: Move Eggmobile
FunctionPointer(void, eggmoble_move_rapid, (float x, float y, float z, signed int frame), 0x6F0A60);  //Event: Move Eggmobile (Faster)
FunctionPointer(void, eggmoble_turn, (signed int ax, signed int ay, signed int az, signed int frame, char efmode), 0x6F0AC0);  //Event: Rotate Eggmobile
FunctionPointer(void, eggmoble_moveandturn, (float x, float y, float z, signed int ax, signed int ay, signed int az, signed int frame, char efmode), 0x6F0B20);  //Event: Move and Rotate Eggmobile
FunctionPointer(void, seteggmobleparam, (float updown_rat, int updown_acc), 0x6F0BC0); //Event: Set the rate and acceleration of Eggmobile's hovering animation.
FunctionPointer(task*, GetEggMobleTask, (), 0x6F0BF0); //Event: Get Eggmobile's task.
FunctionPointer(task*, Create_SphereBomb, (float x, float y, float z, __int16 frame, float sphere_radius, float scl_spd), 0x6F0D10); //Event: Create Explosion Effect
FunctionPointer(void, ChgSphereBombMode, (task* tp, char mode), 0x6F0D90); //Event: Set mode of SphereBomb.
FunctionPointer(void, SetSphereBombParameter, (task* tp, float scl_reduce_acc, float vscroll_spd, float sphere_radious), 0x6F0DB0); //Event: Set parameters of SphereBomb.
VoidFunc(WaterPillarOff, 0x6F1000); //Event: Remove Chaos' Water Pillar.
FunctionPointer(void, WaterPillarOn, (float pos_x, float pos_y, float pos_z, float play_spd, float scale), 0x6F15A0); //Event: Super Sonic's Story Tornado 2 Chaos Pillar
FunctionPointer(void, effect2_delete, (__int16 ef_num), 0x6F17E0); //Event: Delete effect created with effect2_create.
FunctionPointer(void, effect2_create, (task* obj, __int16 ef_num, float ef_x, float ef_y, float ef_z, NJS_TEXLIST* ef_name, __int16 ef_frame), 0x6F18C0); //Event: Effect Generator. Uncommon, only used for Tikal sealing chaos.
FunctionPointer(void, effect2_scl, (__int16 ef_num, float sx, float sy), 0x6F1A40); //Event: Set scale of effect created with effect2_create.
FunctionPointer(void, effect2_color, (__int16 ef_num, float col_a, float col_r, float col_g, float col_b), 0x6F1A70); //Event: Color effect created with effect2_create.
FunctionPointer(void, effect2_angle, (__int16 ef_num, int ang_start, int ang_speed), 0x6F1AB0); //Event: Set Angle of effect created with effect2_create.
FunctionPointer(void, effect2_size_change, (__int16 ef_num, float speed_x, float speed_y), 0x6F1AE0); //Event: Scale effect (animated) created with effect2_create.
FunctionPointer(void, stopWaveCtrl, (int id), 0x6F1B80); //Event: Delete water ripple.
FunctionPointer(void, createWaveCtrl, (float x, float y, float z, float a4, float a5, int a6, int a7, int a8), 0x6F1D30); //Event: Water Ripple
FunctionPointer(task*, SetEventFrog, (), 0x6F1DE0); //Event: Froggy (w/ Tail). Create with EV_CreateObjectFunc.
FunctionPointer(void, chg_frogshape, (char shapeNo), 0x6F1F60); //Event: Set whether setfrog has the tail or not.
FunctionPointer(task*, set_shapefrog, (), 0x6F2330); //Froggy (w/o Tail). Create with EV_CreateObjectFunc.
VoidFunc(StgChaos6CtrlOff, 0x6F2360); //Event: Return Chaos 6 to usual behavior.
VoidFunc(StgChaos6Human2Water, 0x6F23E0); //Event: Make Chaos 6 retreat into puddle.
VoidFunc(StgChaos6Water2Human, 0x6F23F0); //Event: Make Chaos 6 emerge from puddle.
FunctionPointer(void, StgChaos6SetPos, (float pos_x, float pos_y, float pos_z), 0x6F2400); //Event: Set Chaos 6's Position.
FunctionPointer(void, StgChaos6SetAng, (int ang_x, int ang_y, int ang_z), 0x6F2450); //Event: Set Chaos 6's Angle
FunctionPointer(void, StgChaos6CtrlOn, (float pos_x, float pos_y, float pos_z, int ang_x, int ang_y, int ang_z, int rot_spd), 0x6F24B0); //Event: Disable boss behavior for Chaos 6.
VoidFunc(DeleteChaos4, 0x6F25A0); //Event: Delete Event Chaos 4.
FunctionPointer(void, ChangeMotionSpeedChaos4, (float mod_spd), 0x6F25C0); //Event: Set Event Chaos 4's movement speed.
FunctionPointer(void, CreateChaos4, (float pos_, float pos_y, float pos_z, int ang_x, int ang_y, int ang_z, signed int cng_int), 0x6F2AB0); //Event: Create Chaos 4 Event Actor
FunctionPointer(void, EvCreateKiran2, (NJS_POINT3* pos, NJS_POINT3* velo, float scl, float scl_spd), 0x6F2C40); //Event: Shooting Star Effect in Big's Intro/Knuckles' Outro.
FunctionPointer(void, Create_e102lightning, (float x, float y, float z, int childtask_num), 0x6F2F60); //Event: Broken E102 electrical effects.
VoidFunc(Delete_e102lightning, 0x6F2FB0); //Event: Delete E102 electrical effects.
FunctionPointer(void, sandStorm, (int frame, int alpha), 0x6F2FD0); //Event: Set E102 First Person Noise (Requires createBoot)
VoidFunc(deleteBoot, 0x6F3000);  //Event: Delete E102 Boot task.
FunctionPointer(void, createBoot, (int wait_frame, int sstorm_frame, int poweron_frame, int adjust_frame), 0x6F32E0);  //Event: E102 1st-person noise/overlay.
VoidFunc(CreateBootSleep, 0x6F3350); //Event: E102 Shut Down
FunctionPointer(task*, CObjSmoke_Create, (), 0x6F3450); //Event: Create smoke Effect
FunctionPointer(void, SetE102Effect, (float x, float y, float z), 0x6F3650); //Event: Dust particles for E102 taking liftoff while escaping the Egg Carrier.
FunctionPointer(void, ChgEffectMod, (char mode), 0x6F3690); //Event: Change mode for E102Effect.
VoidFunc(DelE102Effect, 0x6F36B0); //Event: Delete E102Effect.
VoidFunc(E101Factory_Create, 0x6F3700); //Event: Create E101 Egg Carrier Room (Spawns above main interior)
VoidFunc(E101Factory_Delete, 0x6F3720); //Event: Delete E101 Egg Carrier Room 
FunctionPointer(task*, CreateCaptureBeam, (float x, float y, float z, int ax, int ay, int az), 0x6F39C0); //Event: Eggman Capture Beam
FunctionPointer(void, light_delete, (task* tp), 0x6F3A20); //Event: Delete Capture Beam (Alt name: delete_capturebeam)
FunctionPointer(void, ChgCaptureMod, (task* tp, char mode), 0x6F3A40); //Event: Change mode for capture beam.
FunctionPointer(void, SetCaptureParam, (task* tp, float sclx, float scly, float sclz, float scl_large_spd, float scl_small_spd, signed int rot_spd), 0x6F3A60); //Event: Set parameters for capture beam.
FunctionPointer(task*, ev_e105, (), 0x6F3D10); //Event: E105 (Create with EV_CreateObjectFunc)
FunctionPointer(task*, ev_e104, (), 0x6F3F40); //Event: E104 (Create with EV_CreateObjectFunc)
FunctionPointer(task*, ev_e103, (), 0x6F4180); //Event: E103 (Create with EV_CreateObjectFunc)
FunctionPointer(task*, light_create, (float x, float y, float z), 0x6F43D0); //Event: Eggman's spotlights from fanfare cutscene.
FunctionPointer(void, light_turn, (task* tp, float x, float y, float z), 0x6F4400); //Event: Move Eggman's spotlights.
VoidFunc(frog_create, 0x6F44F0); //Event: Egg Carrier Screen Froggy
VoidFunc(frog_delete, 0x6F4530); //Event: Delete Egg Carrier Screen Froggy
VoidFunc(dark_on, 0x6F4600); //Event: Nonfunctional, original fanfare scene darkness effect.
VoidFunc(dark_off, 0x6F4620); //Event: Nonfunctional, delete original fanfare scene darkness effect.
FunctionPointer(task*, ev_e101, (), 0x6F49D0); //Event: E101 (Create with EV_CreateObjectFunc)
FunctionPointer(void, MemeCreate, (int num), 0x6F53B0); //Event: Emerald Shards flying into Master Emerald
VoidFunc(MemeDelete, 0x6F53D0); //Event: Delete Emerald Shards flying into Master Emerald
VoidFunc(DeleteChaos2, 0x6F53F0); //Event: Delete Event Chaos 2.
VoidFunc(ToHumanChaos2, 0x6F5410); //Event: Make Event Chaos 2 emerge from puddle.
FunctionPointer(void, ChangeMotionSpeedChaos2, (float mot_spd), 0x6F5430); //Set Event Chaos 6's Movement Speed.
FunctionPointer(void, SetPositionChaos2, (float pos_x, float pos_y, float pos_z), 0x6F5450);
FunctionPointer(void, SetAngleChaos2, (int ang_x, int ang_y, int ang_z), 0x6F5470);
VoidFunc(SetWaterChaos2, 0x6F5490); //Event: Set Event Chaos 2 as puddle form.
FunctionPointer(void, CreateChaos2, (float pos_x, float pos_y, float pos_z, int ang_x, int ang_y, int ang_z, signed int cng_int), 0x6F5A70); //Chaos 2 Event Actor Controls
FunctionPointer(void, throughplayer_on, (task* tp), 0x6F5B50); //Event: task ignores other tasks' collision.
FunctionPointer(void, throughplayer_off, (task* tp), 0x6F5B90); //Event: Turn off throughplayer effect.
VoidFunc(DeleteChaos1, 0x6F5BE0); //Event: Delete Chaos 1.
VoidFunc(ToWaterChaos1, 0x6F5C00); //Event: Make Chaos 1 retreat into puddle.
VoidFunc(ToHumanChaos1, 0x6F5C20); //Event: Make Chaos 1 emerge from puddle.
FunctionPointer(void, ChangeMotionSpeedChaos1, (float mot_spd), 0x6F5C40); //Set Chaos 1's Movement Speed.
FunctionPointer(void, SetPositionChaos1, (float pos_x, float pos_y, float pos_z), 0x6F5C60);  //Event: Set Chaos 1's Position.
VoidFunc(SetWaterChaos1, 0x6F5C80); //Event: Set Chaos 1 as puddle form.
FunctionPointer(void, CreateChaos1, (float pos_x, float pos_y, float pos_z, int ang_x,  //Event: Create Chaos 1 Event Actor
	int ang_y, int ang_z, int cng_int), 0x6F6120);
VoidFunc(StgChaos2CtrlOff, 0x6F6210); //Event: Return Chaos 2 to usual behavior.
VoidFunc(StgChaos2Human2Water, 0x6F6290); //Event: Make Chaos 2 retreat into puddle.
VoidFunc(StgChaos2Water2Human, 0x6F62A0); //Event: Make Chaos 2 emerge from puddle.
FunctionPointer(void, StgChaos2Action, (float ratio), 0x6F62B0); //Event: Make Chaos 2 roar.
FunctionPointer(void, StgChaos2BubbleAdd, (float num), 0x6F62C0); //Event: Set number of bubbles inside Chaos 2.
FunctionPointer(void, StgChaos2SetPos, (float pos_x, float pos_y, float pos_z), 0x6F62D0); //Event: Set Chaos 2's Position.
FunctionPointer(void, StgChaos2SetAng, (int ang_x, int ang_y, int ang_z), 0x6F6320); //Event: Set Chaos 2's Angle.
FunctionPointer(void, StgChaos2CtrlOn, (float pos_x, float pos_y, float pos_z, int ang_x, int ang_y, int ang_z, int rot_spd), 0x6F6380); //Event: Disable boss behavior for Chaos 2.
VoidFunc(CEcCloud_Stop, 0x6F6690); //Event: Stop Egg Carrier wind effect (from CEcCloud_Start).
FunctionPointer(void, CEcCloud_Start, (float scale, int num), 0x6F6810); //Egg Carrier wind effect.
FunctionPointer(task*, set_amy, (), 0x6F6B20); //Event: ZERO Carrying Amy. Create with EV_CreateObjectFunc.
FunctionPointer(void, setamyparam, (float spd), 0x6F6B40); //Event: Set animation speed for ZERO Carrying Amy. (set_amy)
FunctionPointer(task*, mghand_init, (float x, float y, float z, int ax, int ay, int az), 0x6F6F30); //Event: Eggman's claw (Or as the game calls it, "Magic Hand")
FunctionPointer(void, chgMghandMod, (task* obj, char mode), 0x6F6FC0); //Event: Set mode for Eggman's claw.
FunctionPointer(void, mghandsetspd, (task* obj, int angspd_x, int angspd_y), 0x6F6FE0);  //Event: Set animation speed for Eggman's claw.
VoidFunc(fadein_delete, 0x6F7000); //Event: Leftover from an old fade implementation in Amy's intro. Just deletes an unused task: task_fadein.
VoidFunc(DeleteEggmoble1, 0x6F7020); //Event: Delete Event Egg Hornet.
FunctionPointer(void, MoveEggmoble1, (float tar_x, float tar_y, float tar_z), 0x6F7040); //Event: Move Event Egg Hornet.
FunctionPointer(void, SetEggmoble1, (float pos_x, float pos_y, float pos_z, int ang_x, int ang_y, int ang_z), 0x6F7090); //Event: Set Event Egg Hornet's Position and Angle.
VoidFunc(JetNoneEggmoble1, 0x6F70D0); //Event: Event Egg Hornet Jet mode.
VoidFunc(JetNormalEggmoble1, 0x6F70E0); //Event: Event Egg Hornet Jet mode.
VoidFunc(JetLargeEggmoble1, 0x6F70F0); //Event: Event Egg Hornet Jet mode.
VoidFunc(JetMaxEggmoble1, 0x6F7100); //Event: Event Egg Hornet Jet mode.
FunctionPointer(void, ChangeSpeedEggmoble1, (float speed), 0x6F7110); //Event: Set Event Egg Hornet's flying speed.
FunctionPointer(void, CreateEggmoble1, (float x, float y, float z, int angx, int angy, int angz), 0x6F78A0); //Event: Egg Hornet Event Actor
FunctionPointer(void, EV_eggmoble2, (float x, float y, float z, int ax, int ay, int az), 0x6F7D80); //Event: Egg Walker Event Actor.
FunctionPointer(void, ChgEgg2Mod, (char mode), 0x6F7DF0); //Event: Set mode for Event Egg Walker.
VoidFunc(EV_deleggmoble2, 0x6F7E00); //Event: Delete Event Egg Walker.
FunctionPointer(void, ShadowEggmoble2, (float size, float height), 0x6F7E20); //Event: Set the size and height of the shadow below Event Egg Walker.
FunctionPointer(task*, SetEventObjectSSCar, (), 0x6F7FE0); //Event: Cars in Egg Walker Cutscene. Create with EV_CreateObjectFunc.
FunctionPointer(void, SetEggMissileParam, (task* tp, float scl), 0x6F8300); //Event: Set Event Missile' scl.
FunctionPointer(void, SetMissileEffect, (task* tp, char attr), 0x6F8320); //Event: Set active effect bit for Event Eggman Missile
FunctionPointer(task*, Create_EV_Eggmissile, (), 0x6F8610); //Event: Create Event Eggman Missile
FunctionPointer(task*, CSukiari_Create, (int color, NJS_POINT2* a2, int pointn), 0x6F8AB0); //Event: Eggman seeing the Purple Emerald screen effect.
FunctionPointer(void, CSukiari_Alpha, (task* a1, unsigned char alpha, int frame), 0x6F8B40); //Event: Set alpha for CSukiari.
FunctionPointer(task*, ev_trzangai, (), 0x6F9390); //Event: Crashed prototype plane. Create with EV_CreateObjectFunc.
FunctionPointer(task*, miles_with_ptr, (), 0x6F9570); //Event: Prototype plane in Tails' intro. Create with EV_CreateObjectFunc.
FunctionPointer(void, miles_motchg, (int num, float spd), 0x6F95A0); //Event: Set animation for miles_with_ptr.
VoidFunc(StgChaos0CtrlOff, 0x6F95D0);  //Event: Return Chaos 0 to usual behavior.
FunctionPointer(void, StgChaos0SetPos, (float pos_x, float pos_y, float pos_z), 0x6F9650); //Event: Set Chaos 0's Position.
FunctionPointer(void, StgChaos0SetAng, (int ang_x, int ang_y, int ang_z), 0x6F96A0); //Event: Set Chaos 0's Angle.
FunctionPointer(void, StgChaos0CtrlOn, (float pos_x, float pos_y, float pos_z, // //Event: Disable boss behavior for Chaos 0.
	int ang_x, int ang_y, int ang_z, int rot_spd), 0x6F9840);
FunctionPointer(task*, EV_zangai_ctrl, (float x, float y, float z, float sx, //Event: Egg Viper cutscene debris controller
	float sy, float sz, __int16 a7), 0x6F9B40); 
VoidFunc(EV_EggViperKesu, 0x7B5500); //Event: Egg Viper: Reset.
VoidFunc(EV_EggViperEggmanToujou, 0x7B5570); //Event: Egg Viper plays animation: Reveal cockpit.
VoidFunc(EV_EggViperEggmanShaberu, 0x7B5580); //Event: Egg Viper plays animation: Eggman talking (animated)
VoidFunc(EV_EggViperEggmanShaberiYame, 0x7B5590); //Event: Egg Viper plays animation: Eggman talking (static)
VoidFunc(EV_EggViperEggmanHikkomu, 0x7B55A0); //Event: Egg Viper plays animation: Eggman returns to cockpit.
VoidFunc(EV_EggViperJoushou, 0x7B55B0); //Event: Egg Viper plays animation: Close cockpit.
FunctionPointer(void, EV_EggViperArawaru, (float xpos, float ypos, float zpos, int yang), 0x7B5CF0); //Event: Set Egg Viper's Position.
FunctionPointer(task*, CreateEventObjectTR, (void), 0x6F89A0); //Event: Tornado 1. Create with EV_CreateObjectFunc.

// Story sequence
FunctionPointer(BOOL, SeqCheckFlag, (int no), 0x412D20);
FunctionPointer(void, SeqSetPlayer, (int no), 0x413380); // Current story section
FunctionPointer(SEQ_SECTIONTBL*, SeqGetSectionList, (int playerno), 0x44EAF0); // Current story section
FunctionPointer(void, SeqSetTime, (Sint8 time), 0x412C00); // Set time of dat
FunctionPointer(Sint8, SeqGetTime, (), 0x412C10); // Get time of day

// Menu and HUD
VoidFunc(DialogJimakuInit, 0x40BC80);
FunctionPointer(void, DialogJimakuPut, (const char* str), 0x40BD30);
FunctionPointer(char, GetDialogStat, (), 0x432550); // Get the current selected item of a dialog
FunctionPointer(BOOL, CloseDialog, (), 0x432580);
FunctionPointer(void, OpenDialogCsrLet, (const DialogPrmType* dp, char csr, char* dis_csr_ptr), 0x432D20); // Open a menu dialog; csr is current selected item, dis_csr_ptr is an array of disabled items
FunctionPointer(void, OpenDialog, (const DialogPrmType* dp), 0x432DB0); // Open a menu dialog (only one can run)
FunctionPointer(void, ghSetPvrTexMaterial, (unsigned int color), 0x432F40);
FunctionPointer(void, ghSetPvrTexVertexColor, (unsigned int c0, unsigned int c1, unsigned int c2, unsigned int c3), 0x432F90);
FunctionPointer(void, DrawTitleBack, (float x, float y, float z, float w, float h), 0x4334F0);
FunctionPointer(void, printMenu2, (PAUSE_TEXTURE_ID id, signed int pos_x, signed int pos_y, int flag), 0x4B62B0); // Draws a pause menu texture (flag is selection rectangle)
FunctionPointer(void, ghInitPvrTexture, (), 0x432EA0); // Resets global PVR texture settings
FunctionPointer(void, ghDrawPvrTexture, (int index, float x, float y, float z), 0x4338D0); // Draws a texture
FunctionPointer(void, ghDrawPvrTextureS, (int index, float x, float y, float z, float sx, float xy), 0x433F80); // Draws a texture with scale
FunctionPointer(void, ghDrawPvrTextureWH, (int index, float x, float y, float z, float centerX, float centerY), 0x433C90); // Draws a texture with width and height
FunctionPointer(void, ghDrawPvrTextureWHUV, (int index, float x, float y, float z, float w, float h, NJS_TEXTURE_VTX* uvset), 0x4335A0); // Draws a texture with width, height and UVs
FunctionPointer(void, ghSetPvrTexBaseColor, (unsigned int color), 0x433010);
FunctionPointer(void, DrawShadowWindow, (float x, float y, float z, float w, float h), 0x4343E0);
FunctionPointer(void, AdvaOpenDialogCsrLet, (DiaTypeEnum dialog_type, char csr, char* csrp), 0x5057D0); // Open a main menu dialog; csr is current selected item, dis_csr_ptr is an array of disabled items
FunctionPointer(void, AdvaOpenDialogQuick, (DiaTypeEnum dialog_type, char csr, char* csrp), 0x5057F0); // See AdvaOpenDialogCsrLet
FunctionPointer(void, CmnAdvaModeProcedure, (AdvaModeEnum mode), 0x505B40); // Request menu change
FunctionPointer(void, CharSelAdvaModeProcedure, (AdvaModeEnum mode), 0x505E60);
FunctionPointer(void, AvaLoadTexForEachMode, (int mode), 0x506010); // Load textures for current menu
VoidFunc(AvaReleaseTexForEachMode, 0x506040); // Release textures from current menu
FunctionPointer(BOOL, GetEnableTrialActNumAll, (), 0x5061F0);
FunctionPointer(BOOL, GetEnableMiniGameNumAll, (), 0x506210);
FunctionPointer(BOOL, AvaGetMissionEnable, (), 0x506410);
FunctionPointer(BOOL, GetEachCharEnable, (Sint32 chnum), 0x506630);
FunctionPointer(BOOL, GetEachCharEnableForTrial, (Sint32 chnum), 0x506700);
FunctionPointer(BOOL, AvaGetTrialEnable, (), 0x506780);
FunctionPointer(int, GetFadeOutColFromT, (float t), 0x506E10); // Return fade out colour from 0-1 range float
FunctionPointer(int, GetFadeInColFromT, (float t), 0x506E40); // Return fade in colour from 0-1 range float
FunctionPointer(void, DrawSkyBg, (float z), 0x507BB0);
FunctionPointer(void, EvBossDispHitPoint, (__HPPOS* hppos), 0x4B62B0); // Displays hit count for character minibosses
TaskFunc(trial_act_sel_exec, 0x50B220);
TaskFunc(trial_act_sel_disp, 0x50B410);
TaskFunc(title_new_exec, 0x5101A0);
TaskFunc(file_sel_disp, 0x00505550); // File Select display function
FunctionPointer(void, DisplayTitleNew_3, (float baseZ), 0x0050BA90); // Main menu display function
FunctionPointer(int, BetweenCol, (unsigned int src, unsigned int dst, float avg), 0x004319D0); // Blends two colors together
FunctionPointer(void, ghFontPuts, (const char* text, float x, float y, float scale), 0x00420C50); // Draws a string with Ascii16x16FontInfo
FunctionPointer(AvaCharEnum, GetAdvertisePlayerNumber, (), 0x0042CDE0); // Get current character in menus
FunctionPointer(void, DrawSquareC, (Uint32 col, Float x, Float y, Float z, Float magx, Float magy), 0x431CB0);
FunctionPointer(void, DrawDlgCsrSqr, (Uint8 col_alpha, Float x, Float y, Float z, Float magx, Float magy), 0x431BB0);

// TGS Menus
VoidFunc(DisplaySelectingStage_ss, 0x0042DE70);
VoidFunc(DisplaySelectingCharacter_ss, 0x0042DDE0);
VoidFunc(DisplaySelectingSequence, 0x0042DF30);
VoidFunc(DisplayForwardArrow, 0x0042E010);
VoidFunc(DisplayBackwardArrow, 0x0042DFE0);

// Level objects: Windy Valley
FunctionPointer(void, ObjectWatageDisplay, (task* tp), 0x004DFA60); // Dandelion

// Level objects: Twinkle Park
FunctionPointer(void, dispTPRoof, (task* tp), 0x5F7370); // Renders various Twinkle Park objects using the pointer in the task struct

// Level objects: Red Mountain
FunctionPointer(void, Object_Mountain_CL_sub, (task* tp, NJS_OBJECT* pObj), 0x600BF0); // Draws Red Mountain clouds
FunctionPointer(void, SetClip_0500, (int ClipLevel), 0x6007E0); //Clip level for Red Mountain

// Level objects: Sky Deck
FunctionPointer(void, ObjectSkydeck_Shadow_Display, (task* tsk, NJS_OBJECT* obj), 0x005ED790);

// Level objects: Casinopolis
FunctionPointer(void, ObjCasino_SetTexAnimInfo, (TEXANIMINFO* paniminfo, int cnt), 0x005DD900); // Animates textures using TEXANIMINFO
FunctionPointer(void, ObjCasino_ResetTexAnimInfo, (TEXANIMINFO* paniminfo, int cnt), 0x005DD920); // Resets texture animation
FunctionPointer(Sint32, SetObjectTexture2, (int tex), 0x005C09D0); // Sets OBJ_CASINO or OBJ_CASINO9 texlist
FunctionPointer(void, ObjCasino_MobileObj, (task* tp, NJS_POINT3* vec, int angY), 0x005DD4C0); // Dynamic collision for Casino gears

// Level objects: Station Square
FunctionPointer(void, LampDisp, (task* a1), 0x0063A930); // Draws the street light in Station Square (night version)
FunctionPointer(int, checkInSsHodel, (), 0x0062EA30); // Checks if the camera is inside Station Square hotel (lol)
FunctionPointer(void, SetPositionS_WhenHeGetShoes, (task* tsk), 0x006303D0); // Sets Sonic's position in the sewers when he gets the Light Speed Dash shoes
VoidFunc(SSChangeTimetex, 0x00633690); // Changes Station Square textures depending on the time of day

// Chao
FunctionPointer(void, AL_ChangeStageLater, (eCHAO_STAGE_NUMBER NextChaoStage), 0x00715700); // Goes to the specified Chao stage
FunctionPointer(void, AL_LoadTex, (const char* filename, NJS_TEXLIST* pTexlist, unsigned __int16 lev), 0x00718880);
FunctionPointer(void, AL_SetChaoMenuTexInfo, (NJS_TEXLIST* pTexlist, unsigned int WindowTexNum, unsigned int ParamTexNum, unsigned int PortTexNum), 0x0072CC00);
FunctionPointer(int, AL_LoadAnotherGardenInfoStart, (), 0x00717160);
FunctionPointer(void, ALCAM_CreateCameraManager, (), 0x72A750);
FunctionPointer(char, CameraSetCollisionCameraFunc, (void(__cdecl* fnCamera)(_OBJ_CAMERAPARAM*), unsigned __int8 ucAdjustType, char scCameraDirect), 0x00436210);
FunctionPointer(_camcontwk**, AL_EntCameraFunc, (), 0x0072CCF0);
FunctionPointer(void, ALO_OdekakeMachineCreate, (NJS_POINT3* pPos, int angy), 0x00729F40);
FunctionPointer(void, ALO_WarpCreate, (task* parent_tp, int aim, float posx, float posy, float posz, int roty), 0x00729550);
FunctionPointer(void, AL_ExitToStage, (int stage, int act), 0x715730);
FunctionPointer(void, AL_CalcMotionMartix, (al_object* pObject), 0x00765010);
FunctionPointer(void, AL_InitCalcMotionMatrix, (task* tp), 0x00764B10);
FunctionPointer(int, ALR_GetBodyPosition, (NJS_VECTOR* vector, taskwk* twp, alifewk* awp, int joint), 0x00754D00); // Calculates SA1 Chao node positions (broken in DX)
VoidFunc(ALW_Create, 0x0071A7C0);
VoidFunc(AL_EntCameraSet, 0x0072CD70);
VoidFunc(AlEntAnimArrayInit, 0x0072CC30);
VoidFunc(AL_GBAManagerWakeup, 0x00716A90);
VoidFunc(AL_JoyInputStart, 0x0072CBC0);
VoidFunc(AL_EntranceMenuCreate, 0x0072CB40);
VoidFunc(AL_PlayerDisable, 0x007153F0);
VoidFunc(ALO_EntDoorCreate, 0x0072C2E0);
VoidFunc(_al_stage_prolog, 0x00715AE0);
VoidFunc(_alg_blackmarket_prolog, 0x00718BB0);
VoidFunc(_alg_garden00_ss_prolog, 0x00719600);
VoidFunc(_alg_garden01_ec_prolog, 0x007191D0);
VoidFunc(_alg_garden02_mr_prolog, 0x00718E90);
VoidFunc(_alg_garden02_mr_daytime_prolog, 0x0072A790);
VoidFunc(_alg_garden02_mr_evening_prolog, 0x0072A820);
VoidFunc(_alg_garden02_mr_night_prolog, 0x0072A8B0);
VoidFunc(_alg_entrance_prolog, 0x007199B0);
VoidFunc(_al_odekake_prolog, 0x00719C40);
VoidFunc(_alg_race_prolog, 0x00719DB0);
TaskFunc(AL_Garden00Master, 0x00719320);
TaskFunc(AL_Garden01Master, 0x00719040);
TaskFunc(AL_Garden02Master, 0x00718D00);
TaskFunc(AL_EntranceMaster, 0x00719880);
TaskFunc(AL_RaceMaster, 0x00719D00);
FunctionPointer(void, AL_EntranceMasterExec, (task* a1), 0x7197E0);
FunctionPointer(void, AL_EntranceMasterDest, (task* a1), 0x7197C0);
TaskFunc(ObjectAlrCracker, 0x0071BEA0);
VoidFunc(ObjectAlrCracker_Init, 0x0071BF20);
TaskFunc(ObjectALGardenDoor, 0x0072AB50);
FunctionPointer(void, AL_SetMotionLink, (task* a1, int a2), 0x00734F00);
FunctionPointer(void, AL_SetBehavior, (task* tp, void(__cdecl* Func)(task*)), 0x0071EF10);
FunctionPointer(void, AL_CreateNormalCameraTask, (), 0x72A570);
FunctionPointer(void, AlrObjSetTask, (TaskInfo* a2, void* a3), 0x00752E90);
FunctionPointer(eCHAO_STAGE_NUMBER, AL_GetStageNumber, (), 0x00715140);
TaskFunc(ChaoSelectWindowTDisplayer, 0x00769320);
FunctionPointer(void, AL_DrawTreeSub, (TREE_WORK* pTree, NJS_OBJECT* pObject), 0x00720DF0); // Chao Tree display function

#endif /* SADXMODLOADER_SADXFUNCTIONSNEW_H */