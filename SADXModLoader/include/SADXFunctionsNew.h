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
		mov result, eax
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

// Bool __usercall@<eax>(NJS_TEXLIST *a1@<eax>)
static const void* const isTextureNGPtr = (void*)0x403250;
static inline Bool isTextureNG(NJS_TEXLIST* a1)
{
	Bool result;
	__asm
	{
		mov eax, [a1]
		call isTextureNGPtr
		mov result, eax
	}
	return result;
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

// Cutscene audio functions
FunctionPointer(void, voicevolchange, (int vol), 0x40CCF0);
FunctionPointer(void, EventSe_Play, (int id, int se, int frame), 0x64FD00);
FunctionPointer(void, EventSe_Stop, (int id), 0x64FD40);
FunctionPointer(void, EventSe_Volume, (int id, int volume, int frame), 0x64FD80);
FunctionPointer(void, EventSe_Pan, (int id, int pan, int frame), 0x64FDB0);
FunctionPointer(void, EventSe_Pitch, (int id, int pitch, int frame), 0x64FDE0);
FunctionPointer(void, EventSe_Oneshot, (int se, int volume, int pan, int pitch), 0x64FE10);

// Cutscene message
FunctionPointer(void, EV_Msg, (char* str), 0x42FB20);
VoidFunc(EV_MsgClose, 0x42FBB0);
VoidFunc(EV_MsgCls, 0x42FC20);
FunctionPointer(void, EV_MsgW, (int time, char* str), 0x431640);

// Cutscene Camera
VoidFunc(EV_CameraOn, 0x4303E0);
VoidFunc(EV_CameraOff, 0x4304B0);
FunctionPointer(void, EV_CameraAng, (int mode, int time, int x, int y, int z), 0x430C90);
FunctionPointer(void, EV_CameraPos, (int mode, int time, float x, float y, float z), 0x430D00);
FunctionPointer(void, EV_CameraPosORY, (char mode, int time, task* tp, float x, float y, float z), 0x430D70);
VoidFunc(EV_CameraTargetFree, 0x430E50);
FunctionPointer(void, EV_CameraTargetPos, (int mode, int time, float x, float y, float z, float az), 0x430E60);
FunctionPointer(void, EV_CameraTargetObj, (int mode, int time, task* tp, float x, float y, float z, int az), 0x430EE0);
VoidFunc(EV_CameraChaseFree, 0x430F60);
FunctionPointer(void, EV_CameraChase, (task* tp), 0x430F70);
FunctionPointer(void, EV_CameraChaseRM, (int mode, int time, task* tp, float height, int x0, int y0, int z0, float d0, int x1, int y1, int z1, float d1), 0x430FC0);
FunctionPointer(void, EV_CameraPerspective, (int mode, int time, int ang), 0x431060);
FunctionPointer(void, EV_CameraPath, (cpathtag* path, float speed), 0x4310A0);

// Cutscene functions
FunctionPointer(void, EV_CreateObject, (task** tp, float px, float py, float pz, int ax, int ay, int az), 0x431670);
FunctionPointer(void, EV_CreateObjectFunc, (task** a1, task* (*func)(void), float x, float y, float z, int rx, int ry, int rz), 0x4316C0);
FunctionPointer(void, EV_InitObject, (task* tp), 0x431730);
FunctionPointer(void, EV_InitPlayer, (int n), 0x431780);
FunctionPointer(void, EV_WaitAction, (task* tp), 0x431810);
FunctionPointer(void, EV_WaitPath, (task* tp), 0x431840);
FunctionPointer(void, EV_PlayPad, (int no, PADREC_DATA_TAG* tag), 0x431870);
FunctionPointer(void, EV_WaitMove, (task* tp), 0x4318D0);
FunctionPointer(void, EV_FreeObject, (task** tp), 0x42FC50);
FunctionPointer(void, EV_SetPos, (task* tp, float x, float y, float z), 0x42FC70);
FunctionPointer(void, EV_SetAng, (task* tp, int x, int y, int z), 0x42FCD0);
FunctionPointer(void, EV_SetMode, (task* tp, int mode), 0x42FD20);
FunctionPointer(void, EV_SetShadow, (task* tp, float size), 0x42FD40);
FunctionPointer(void, EV_SetColli, (task* tp, float size), 0x42FD60);
FunctionPointer(void, EV_CreatePlayer, (int n, void(__cdecl* func)(task*), float px, float py, float pz, int ax, int ay, int az), 0X42FD80);
FunctionPointer(void, EV_RemovePlayer, (int n), 0x42FDE0);
FunctionPointer(void, EV_SetAction, (task* tp, NJS_ACTION* ap, NJS_TEXLIST* lp, float speed, int mode, int linkframe), 0x42FE00);
FunctionPointer(void, EV_SetMotion, (task* tp, NJS_OBJECT* op, NJS_MOTION* mp, NJS_TEXLIST* lp, float speed, int mode, int linkframe), 0x42FE20);
FunctionPointer(void, EV_ClrAction, (task* tp), 0x42FE40);
FunctionPointer(void, EV_SetPath, (task* tp, EPATHTAG* path, float speed, int mode), 0x42FE60);
FunctionPointer(void, EV_ClrPath, (task* tp), 0x42FE80);
FunctionPointer(void, EV_LookFree, (task* tp), 0x42FFB0);
FunctionPointer(void, EV_LookObject, (task* tp, task* target, float x, float y, float z), 0x42FFD0);
FunctionPointer(void, EV_LookPoint, (task* tp, float x, float y, float z), 0x430000);
FunctionPointer(void, EV_LookAngle, (task* tp, int x, int y, int z), 0x430030);
FunctionPointer(void, EV_MoveFree, (task* tp), 0x430060);
FunctionPointer(void, EV_MovePoint, (task* tp, float x, float y, float z), 0x430080);
FunctionPointer(void, EV_MovePoint2, (task* tp, float x, float y, float z, float s, float a), 0x4300D0);
FunctionPointer(void, EV_MoveRotation, (task* tp, int x, int y, int z), 0x430120);
FunctionPointer(void, AmyForEventHammerScaleIm, (char playernmb, float scale), 0x486EC0);
FunctionPointer(void, stopObject, (task* tp), 0x6EBAB0);
VoidFunc(stopObjectAll, 0x6EBB00);
FunctionPointer(void, moveObject, (task* obj, float st_x, float st_y, float st_z, float end_x, float end_y, float end_z, int frame), 0x6EC2B0);
FunctionPointer(void, moveObjectOn, (task* tp, float x, float y, float z, int frame, task* dest_obj), 0x6EC6D0);
FunctionPointer(void, moveObjectOnAngle, (task* obj, float far_x, float far_y, float far_z, int ang_x, int ang_y, int ang_z, int frame, task* dest_obj), 0x6EC790);
FunctionPointer(void, moveObjectOnRotate, (task* obj, float far_x, float far_y, float far_z, signed int frame, task* dest_obj), 0x6EC8B0);
FunctionPointer(void, moveObjectRightHand, (task* obj, char player_no, float far_x, float far_y, float far_z, int frame), 0x6ECA20);
FunctionPointer(void, addmotModel, (int model, NJS_ACTION* action, int mot_id), 0x6ECAC0);
FunctionPointer(void, playModel, (int model_id, int mot_id, float mot_spd, int loop_num), 0x6ECAE0);
FunctionPointer(void, stopModel, (int model_id), 0x6ECB20);
FunctionPointer(task*, getobjModel, (int model_id), 0x6ECB40);
FunctionPointer(void, deleteModel, (int model_id), 0x6ECB50);
FunctionPointer(void, createModel, (float pos_x, float pos_y, float pos_z, int ang_x, int ang_y, int ang_z, float scl_x, float scl_y, float scl_z, obj* model, NJS_TEXLIST* texlist, int model_id), 0x6ECE70);
FunctionPointer(void, createModelEC, (float pos_x, float pos_y, float pos_z, int ang_x, int ang_y, int ang_z, float scl_x, float scl_y, float scl_z, NJS_OBJECT* model, NJS_TEXLIST* texlist, int model_id), 0x6ECF20);

// Story related functions
FunctionPointer(SEQ_SECTIONTBL*, SeqGetSectionList, (int playerno), 0x44EAF0); // Current story section

#endif /* SADXMODLOADER_SADXFUNCTIONSNEW_H */