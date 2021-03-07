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

// SADX Functions
FunctionPointer(task*, CreateChildTask, (unsigned __int16 im, void(__cdecl* exec)(task*), task* tp), 0x40B940);
FunctionPointer(task*, CreateElementalTask, (unsigned __int16 im, int level, void(__cdecl* exec)(task*)), 0x40B860);
FunctionPointer(void, DestroyTask, (task* tp), 0x40B570);
FunctionPointer(void, FreeTask, (task* tp), 0x40B6C0);
FunctionPointer(void, RegisterCollisionEntry, (int slAttribute, task* pTask, obj* pObject), 0x43B2E0);
FunctionPointer(void, WithdrawCollisionEntry, (task* pTask, obj* pObject), 0x43B380);
FunctionPointer(void, B_Destructor, (task* tp), 0x59DBF0);
FunctionPointer(void, PlayerGetRotation, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x44BB60);
FunctionPointer(void, PlayerGetAcceleration, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x44C270);
FunctionPointer(void, PlayerGetSpeed, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x443F50);
FunctionPointer(int, PlayerSetPosition, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x44CDF0);
FunctionPointer(void, PSetCrashEffect, (taskwk* a1), 0x440790);
FunctionPointer(void, PConvertVector_P2G, (taskwk* a1, NJS_POINT3* a2), 0x43EC90);
FunctionPointer(void, PConvertVector_G2P, (taskwk* a1, NJS_POINT3* a2), 0x43EC00);
FunctionPointer(void, PlayerResetAngle, (taskwk* a1, motionwk2* a2, playerwk* a3), 0x443AD0);
FunctionPointer(void, PlayerGetInertia, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x443650);
FunctionPointer(void, PlayerGetGravity, (taskwk* a1, motionwk2* a2, playerwk* a3), 0x443800);
FunctionPointer(void, PlayerGetAccelerationAir, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x44B9C0);
FunctionPointer(void, Knux_RunsActions, (taskwk* a1, motionwk2* a2, playerwk* a3), 0x478020);
FunctionPointer(bool, Player_CheckBreak, (taskwk* a1), 0x4429C0);

static const void* const Knux_NActPtr = (void*)0x476970;
static inline signed int Knux_NAct(playerwk* a1, taskwk* a2, motionwk2* a3)
{
	signed int result;
	__asm
	{
		push[a3]
		mov esi, [a2]
		mov edi, [a1]
		call Knux_NActPtr
		add esp, 4
		mov result, edi
	}
	return result;
}

static const void* const Knux_CheckJumpPtr = (void*)0x476830;
static inline signed int Knux_CheckJump(playerwk* a1, taskwk* a2)
{
	signed int result;
	__asm
	{
		mov ecx, [a2]
		mov eax, [a1]
		call Knux_CheckJumpPtr
		mov result, eax
	}
	return result;
}

static const void* const KnuxCheckStopPtr = (void*)0x477970;
static inline signed int KnuxCheckStop(playerwk* a1, taskwk* a2)
{
	signed int result;
	__asm
	{
		mov ecx, [a2]
		mov eax, [a1]
		call KnuxCheckStopPtr
		add esp, 4
		mov result, eax
	}
	return result;
}

static const void* const Big_GetFloatPtr = (void*)0x48C610;
static inline void BigGetFloat(playerwk* a1, taskwk* a2)
{
	__asm
	{
		mov esi, [a2]
		mov eax, [a1]
		call Big_GetFloatPtr
	}
}

#endif /* SADXMODLOADER_SADXFUNCTIONSNEW_H */
