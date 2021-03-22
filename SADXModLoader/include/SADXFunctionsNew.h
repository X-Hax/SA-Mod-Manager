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

// Ninja draw function
FunctionPointer(void, njDrawModel_, (NJS_MODEL_SADX* model), 0x784AE0); // Offloads to polybuff drawing functions
FunctionPointer(void, njDirectDrawModel, (NJS_MODEL_SADX* model), 0x77EDA0);

// Direct draw functions
FunctionPointer(void, njDrawModel, (NJS_MODEL_SADX* model), 0x77EF70);
FunctionPointer(void, njDrawObject, (NJS_OBJECT* object), 0x77EF50);
FunctionPointer(void, njActionOld, (NJS_ACTION* action, float frame), 0x789560);

// Easy draw functions
FunctionPointer(int, njEasyDrawModel, (NJS_MODEL* model), 0x4084F0);
FunctionPointer(void, njEasyDrawObject, (NJS_OBJECT* object), 0x40A100);
FunctionPointer(void, njEasyDrawMotion, (NJS_OBJECT* object, NJS_MOTION* motion, float frame), 0x406FA0);
FunctionPointer(void, njEasyDrawShapeMotion, (NJS_OBJECT* object, NJS_MOTION* motion, NJS_MOTION* shape, float frame), 0x406FE0);

// Simple draw functions
FunctionPointer(int, dsSimpleDrawModel, (NJS_MODEL_SADX* model), 0x401430);
FunctionPointer(void, dsDrawModel, (NJS_MODEL_SADX* model), 0x407970);
FunctionPointer(void, dsDrawObject, (NJS_OBJECT* object), 0x408530);
FunctionPointer(void, dsDrawModel_S, (NJS_MODEL_SADX* model), 0x40A140);
FunctionPointer(void, ds_DrawModelClip, (NJS_MODEL_SADX* model, float scale), 0x407A00);
FunctionPointer(void, ds_DrawObjectClip, (NJS_OBJECT* object, float scale), 0x4085A0);
FunctionPointer(void, dsDrawMotion, (NJS_OBJECT* object, NJS_MOTION* motion, float frame), 0x407040);
FunctionPointer(void, dsDrawMotionClip , (NJS_OBJECT* object, NJS_MOTION* motion, float frame, float scale), 0x405370);
FunctionPointer(void, dsDrawMotionLink, (NJS_OBJECT* object, NJS_MOTION_LINK* motionLink, float frame), 0x406C20);
FunctionPointer(void, dsActionLink, (NJS_ACTION_LINK* actionLink, float frame), 0x406EC0);
FunctionPointer(void, ds_ActionClip, (NJS_ACTION* action, float frame, float scale), 0x405450);
FunctionPointer(void, dsDrawShapeMotion, (NJS_OBJECT* object, NJS_MOTION* motion, NJS_MOTION* shape, float frame), 0x406030);

// Regular draw functions
FunctionPointer(void, __DrawModel, (NJS_MODEL_SADX* model), 0x403470);
FunctionPointer(int, DrawObject, (NJS_OBJECT* model), 0x4034B0);
FunctionPointer(void, DrawObjectClip, (NJS_OBJECT* object, float scale), 0x4037F0);
FunctionPointer(void, DrawModelEx, (NJS_MODEL_SADX* model, int queueFlags), 0x407BB0);
FunctionPointer(void, DrawObjectClipEx, (NJS_OBJECT* object, int queueFlags, float scale), 0x409080);
FunctionPointer(void, DrawModelMS, (NJS_MODEL_SADX* model, int queueFlags), 0x407CF0);
FunctionPointer(void, DrawModelMesh, (NJS_MODEL_SADX* model, int queueFlags), 0x407FC0);
FunctionPointer(void, DrawObjectMS, (NJS_OBJECT* object, int queueFlags, float scale), 0x409550);
FunctionPointer(void, DrawObjectClipMesh, (NJS_OBJECT* object, int queueFlags, float scale), 0x409A20);
// void __usercall(NJS_ACTION *action@<eax>, float frameNumber, QueuedModelFlagsB flags, float scale, void (__cdecl *callback)(NJS_MODEL_SADX *, int, int))
static const void* const DrawActionPtr = (void*)0x4053D0;
static inline void DrawAction(NJS_ACTION* action, float frame, int queueFlags, float scale, void(__cdecl* callback)(NJS_MODEL_SADX*, int, int))
{
	__asm
	{
		push[callback]
		push[scale]
		push dword ptr[queueFlags]
		push[frame]
		mov eax, [action]
		call DrawActionPtr
		add esp, 16
	}
}

// Missing usercall functions: DrawMotion at 4052F0, DrawMotionLink at 4069A0, DrawShapeMotion at 405EF0

// Variable draw functions (picks between Simple and Late)
FunctionPointer(void, lateDrawModel, (NJS_MODEL_SADX* model, int queueFlags), 0x4048E0);
FunctionPointer(void, lateDrawObject, (NJS_OBJECT* object, int queueFlags, float scale), 0x4074A0);
FunctionPointer(void, late_Action, (NJS_ACTION* action, float frame, int queueFlags, float scale), 0x405490);

// Late draw functions: Model
FunctionPointer(void, late_DrawModel, (NJS_MODEL_SADX* model, int queueFlags), 0x4078F0);
FunctionPointer(void, late_DrawModelClip, (NJS_MODEL_SADX* model, int queueFlags, float scale), 0x407870);
FunctionPointer(void, late_DrawModelEx, (NJS_MODEL_SADX* model, int queueFlags), 0x409450);
FunctionPointer(void, late_DrawModelClipEx, (NJS_MODEL_SADX* model, int queueFlags, float scale), 0x4094D0);
FunctionPointer(void, late_DrawModelMS, (NJS_MODEL_SADX* model, int queueFlags), 0x409920);
FunctionPointer(void, late_DrawModelClipMS, (NJS_MODEL_SADX* model, int queueFlags, float scale), 0x4099A0);
FunctionPointer(void, late_DrawModelMesh, (NJS_MODEL_SADX* model, int queueFlags), 0x409DF0);
FunctionPointer(void, late_DrawModelClipMesh, (NJS_MODEL_SADX* model, int queueFlags, float scale), 0x409E70);
FunctionPointer(void, late_DrawShadowModel, (NJS_MODEL_SADX* model, float scale), 0x407B00);

// Late draw functions: Object
FunctionPointer(void, late_DrawObject, (NJS_OBJECT* object, int queueFlags), 0x407B70);
FunctionPointer(void, late_DrawObjectClip, (NJS_OBJECT* object, int queueFlags, float scale), 0x407B40);
FunctionPointer(void, late_DrawObjectClipEx, (NJS_OBJECT* object, int queueFlags, float scale), 0x40A170);
FunctionPointer(void, late_DrawObjectEx, (NJS_OBJECT* object, int queueFlags), 0x40A1A0);
FunctionPointer(void, late_DrawObjectClipMS, (NJS_OBJECT* object, int queueFlags, float scale), 0x40A1E0);
FunctionPointer(void, late_DrawObjectMesh, (NJS_OBJECT* object, int queueFlags), 0x40A210);
FunctionPointer(void, late_DrawObjectClipMesh, (NJS_OBJECT* object, int queueFlags, float scale), 0x40A250);
FunctionPointer(void, late_DrawShadowObject, (NJS_OBJECT* object, float scale), 0x408690);

// Late draw functions: Motion
FunctionPointer(void, late_DrawMotionClip, (NJS_OBJECT* object, NJS_MOTION* motion, float frame, int queueFlags, float scale), 0x4053A0);
FunctionPointer(void, late_DrawMotionClipEx, (NJS_OBJECT* object, NJS_MOTION* motion, float frame, int queueFlags, float scale), 0x4082D0);
FunctionPointer(void, late_DrawMotionClipMesh, (NJS_OBJECT* object, NJS_MOTION* motion, float frame, int queueFlags, float scale), 0x408300);

// Late draw functions: Action
FunctionPointer(void, late_ActionEx, (NJS_ACTION* action, float frame, int queueFlags), 0x408330);
FunctionPointer(void, late_ActionClipEx, (NJS_ACTION* action, float frame, int queueFlags, float scale), 0x408350);
FunctionPointer(void, late_ActionMS, (NJS_ACTION* action, float frame, int queueFlags), 0x408380);
FunctionPointer(void, late_ActionClipMS, (NJS_ACTION* action, float frame, int queueFlags, float scale), 0x4083A0);
FunctionPointer(void, late_ActionMesh, (NJS_ACTION* action, float frame, int queueFlags), 0x4083D0);
FunctionPointer(void, late_ActionClipMesh, (NJS_ACTION* action, float frame, int queueFlags, float scale), 0x4083F0);

// Late draw functions: ActionLink
FunctionPointer(void, late_ActionLink, (NJS_ACTION_LINK* actionLink, float frame, int queueFlags), 0x406EF5);
FunctionPointer(void, late_ActionLinkEx, (NJS_ACTION_LINK* actionLink, float frame, int queueFlags), 0x4084B0);
FunctionPointer(void, late_ActionLinkMesh, (NJS_ACTION_LINK* actionLink, float frame, int queueFlags), 0x4084D0);

// Late draw functions: MotionLink
FunctionPointer(void, late_DrawMotionLinkEx, (NJS_OBJECT* object, NJS_MOTION_LINK* motionLink, float frame, int queueFlags), 0x408480);

// Late draw functions: Shape motion
FunctionPointer(void, late_DrawShapeMotionEx, (NJS_OBJECT* object, NJS_MOTION* motion, NJS_MOTION* shape, float frame, int queueFlags), 0x408420);
FunctionPointer(void, late_DrawShapeMotionMesh, (NJS_OBJECT* object, NJS_MOTION* motion, NJS_MOTION* shape, float frame, int queueFlags), 0x408450);

// Chaos draw functions
FunctionPointer(void, CHAOS_DrawModel, (NJS_MODEL_SADX* model), 0x409EF0);
FunctionPointer(void, CHAOS_DrawObject, (NJS_OBJECT* object), 0x40A280);
FunctionPointer(void, CHAOS_Action, (NJS_ACTION* action, float frame), 0x409FB0);
FunctionPointer(void, CHAOS_DrawShapeMotion, (NJS_OBJECT* object, NJS_MOTION* motion, NJS_MOTION* shape, float frame), 0x40A050);

#endif /* SADXMODLOADER_SADXFUNCTIONSNEW_H */
