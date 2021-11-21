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

// SADX Functions
FunctionPointer(task*, CreateChildTask, (unsigned __int16 im, TaskFuncPtr exec, task* tp), 0x40B940);
FunctionPointer(task*, CreateElementalTask, (unsigned __int16 im, int level, TaskFuncPtr exec), 0x40B860);
TaskFunc(DestroyTask, 0x40B570);
TaskFunc(FreeTask, 0x40B6C0);
TaskFunc(B_Destructor, 0x59DBF0);
TaskFunc(LoopTaskC, 0x40B420); // Run all the children of a task
FunctionPointer(void, PGetRotation, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x44BB60);
FunctionPointer(void, PGetAcceleration, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x44C270);
FunctionPointer(void, PGetSpeed, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x443F50);
FunctionPointer(int, PSetPosition, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x44CDF0);
FunctionPointer(void, PSetCrashEffect, (taskwk* twp), 0x440790);
FunctionPointer(void, PConvertVector_P2G, (taskwk* twp, NJS_POINT3* vp), 0x43EC90);
FunctionPointer(void, PConvertVector_G2P, (taskwk* twp, NJS_POINT3* vp), 0x43EC00);
FunctionPointer(void, PResetAngle, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x443AD0);
FunctionPointer(void, PGetInertia, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x443650);
FunctionPointer(void, PGetGravity, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x443800);
FunctionPointer(void, PGetAccelerationAir, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x44B9C0);
FunctionPointer(void, Knux_RunsActions, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x478020);
FunctionPointer(bool, PCheckBreak, (taskwk* twp), 0x4429C0);
FunctionPointer(BOOL, CheckRangeOutWithR, (task* tp, float fRange), 0x46C010);
FunctionPointer(BOOL, CheckRangeOut, (task* tp), 0x46C360);
FunctionPointer(BOOL, CheckRangeOut_L, (task* tp), 0x46C390);
FunctionPointer(void, CharacterShadow, (taskwk* twp, shadowwk* swp), 0x49F1A0);
FunctionPointer(void, PJoinVertexes, (taskwk* twp, motionwk2* mwp, playerwk* pwp), 0x43FA90);
FunctionPointer(void, PInitialize, (int num, task* tp), 0x442750);
FunctionPointer(BOOL, SeqCheckFlag, (int no), 0x412D20);
FunctionPointer(int, CheckCollisionCylinderP, (NJS_POINT3* vp, float r, float h), 0x4418D0); // Check if player is in non rotated cylinder, returns 0 or player id + 1.
FunctionPointer(void, SetInputP, (uint8_t pno, int8_t mode), 0x441260);
FunctionPointer(void, SetPositionP, (uint8_t pno, float x, float y, float z), 0x441780);
FunctionPointer(void, SetRotationP, (uint8_t pno, Angle angx, Angle angy, Angle angz), 0x4415F0);
FunctionPointer(void, LadderingPathP, (uint8_t pno, pathtag* pp, int point, Angle3* ang), 0x446C80);
VoidFunc(InitFreeCamera, 0x434870);

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

// Bool __usercall@<eax>(NJS_TEXLIST *tl@<eax>)
static const void* const isTextureNGPtr = (void*)0x403250;
static inline BOOL isTextureNG(NJS_TEXLIST* tl)
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

// CCL (shape collision) functions
FunctionPointer(void, CCL_ClearInfo, (taskwk* twp), 0x418B60);
FunctionPointer(void, CCL_CalcRange, (taskwk* twp), 0x41BAC0);
FunctionPointer(void, EntryColliList, (taskwk* twp), 0x41C280);
FunctionPointer(void, FreeColliWork, (taskwk* twp), 0x41C4E0);
FunctionPointer(void, CCL_Init, (task* tp, CCL_INFO* info, int nbInfo, unsigned __int8 id), 0x41CAF0);

// Geometry collision functions
FunctionPointer(bool, CheckPlayerRideOnMobileLandObjectP, (int pno, task* ttp), 0x441C30);
FunctionPointer(void, RegisterCollisionEntry, (int slAttribute, task* pTask, obj* pObject), 0x43B2E0);
FunctionPointer(void, WithdrawCollisionEntry, (task* pTask, obj* pObject), 0x43B380);
FunctionPointer(void, ReleaseMobileLandObject, (obj* pObjLandObject), 0x43B450);
FunctionPointer(obj*, GetMobileLandObject, (), 0x43B400);

// Path functions
FunctionPointer(int, CheckPlayerRideOnPath, (pathtag* pathtagp), 0x440ED0); // Check if P1 or P2 is on the specified path; returns bitfield.
FunctionPointer(int, GetStatusOnPath, (pathtag* tag, pathinfo* pi), 0x49C330); // Get position, angle and normals at "pi->onpathpos" distance from path in "pi".
FunctionPointer(BOOL, SCPathPntnmbToOnpos, (pathtag* tag, unsigned int pntnmb, float* onpos), 0x49C630); // Converts path point number to distance on path, returns 0 if point outside of range.
FunctionPointer(float, SCPathPntNearToOnpos, (pathtag* tag, NJS_POINT3* pnt, NJS_POINT3* onpnt3, float* onpos), 0x49C670); // Get nearest point (in "onpnt3") and distance from path (in "onpos") from point "pnt", returns distance between the two points.
FunctionPointer(float, RunWithSeeingPathP, (uint8_t pno, pathtag* pp), 0x440E50);
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

// Camera Functions
FunctionPointer(void, CameraSetEventCameraFunc, (CamFuncPtr func, int8_t ucAdjustType, int8_t scCameraDirect), 0x437D20);
FunctionPointer(void, CameraSetEventCamera, (int16_t ssCameraMode, int8_t ucAdjustType), 0x437BF0);
FunctionPointer(BOOL, IsEventCamera, (), 0x436520);
FunctionPointer(BOOL, IsCompulsionCamera, (), 0x436530);
VoidFunc(CameraReleaseEventCamera, 0x436140);
VoidFunc(CameraReleaseCollisionCamera, 0x436400);
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
CamFunc(CameraE101R, 0x56CBF0);
CamFunc(CameraE103, 0x467990);
CamFunc(CameraEgm3, 0x57D6B0);
CamFunc(CameraFollowG, 0x467740);
CamFunc(CameraLR, 0x4627B0);
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
CamAdjustFunc(AdjustNone, 0x467D80);
CamAdjustFunc(AdjustNormal, 0x467DC0);
CamAdjustFunc(AdjustSlow, 0x468000);
CamAdjustFunc(AdjustThreePoint, 0x469D10);
CamAdjustFunc(AdjustForFreeCamera, 0x468800);

// Ninja draw function
FunctionPointer(void, njDrawModel_, (NJS_MODEL_SADX* mdl), 0x784AE0); // Offloads to polybuff drawing functions
FunctionPointer(void, njDirectDrawModel, (NJS_MODEL_SADX* mdl), 0x77EDA0);

// Direct draw functions
FunctionPointer(void, njDrawModel, (NJS_MODEL_SADX* mdl), 0x77EF70);
FunctionPointer(void, njDrawObject, (obj* object), 0x77EF50);
FunctionPointer(void, njActionOld, (NJS_ACTION* action, float frame), 0x789560);

// Easy draw functions
FunctionPointer(int, njEasyDrawModel, (NJS_MODEL* model), 0x4084F0);
FunctionPointer(void, njEasyDrawObject, (obj* object), 0x40A100);
FunctionPointer(void, njEasyDrawMotion, (obj* object, NJS_MOTION* motion, float frame), 0x406FA0);
FunctionPointer(void, njEasyDrawShapeMotion, (obj* object, NJS_MOTION* motion, NJS_MOTION* shape, float frame), 0x406FE0);

// Simple draw functions
FunctionPointer(int, dsSimpleDrawModel, (NJS_MODEL_SADX* mdl), 0x401430);
FunctionPointer(void, dsDrawModel, (NJS_MODEL_SADX* mdl), 0x407970);
FunctionPointer(void, dsDrawObject, (obj* object), 0x408530);
FunctionPointer(void, dsDrawModel_S, (NJS_MODEL_SADX* mdl), 0x40A140);
FunctionPointer(void, ds_DrawModelClip, (NJS_MODEL_SADX* mdl, float clipScl), 0x407A00);
FunctionPointer(void, ds_DrawObjectClip, (obj* object, float clipScl), 0x4085A0);
FunctionPointer(void, dsDrawMotion, (obj* object, NJS_MOTION* motion, float frame), 0x407040);
FunctionPointer(void, dsDrawMotionClip , (obj* object, NJS_MOTION* motion, float frame, float clipScl), 0x405370);
FunctionPointer(void, dsDrawMotionLink, (obj* object, NJS_MOTION_LINK* motionLink, float frame), 0x406C20);
FunctionPointer(void, dsActionLink, (NJS_ACTION_LINK* actionLink, float frame), 0x406EC0);
FunctionPointer(void, ds_ActionClip, (NJS_ACTION* action, float frame, float clipScl), 0x405450);
FunctionPointer(void, dsDrawShapeMotion, (obj* object, NJS_MOTION* motion, NJS_MOTION* shape, float frame), 0x406030);

// Regular draw functions
FunctionPointer(void, __DrawModel, (NJS_MODEL_SADX* mdl), 0x403470);
FunctionPointer(int, DrawObject, (obj* model), 0x4034B0);
FunctionPointer(void, DrawObjectClip, (obj* object, float clipScl), 0x4037F0);
FunctionPointer(void, DrawModelEx, (NJS_MODEL_SADX* mdl, int flgs), 0x407BB0);
FunctionPointer(void, DrawObjectClipEx, (obj* object, int flgs, float clipScl), 0x409080);
FunctionPointer(void, DrawModelMS, (NJS_MODEL_SADX* mdl, int flgs), 0x407CF0);
FunctionPointer(void, DrawModelMesh, (NJS_MODEL_SADX* mdl, int flgs), 0x407FC0);
FunctionPointer(void, DrawObjectMS, (obj* object, int flgs, float clipScl), 0x409550);
FunctionPointer(void, DrawObjectClipMesh, (obj* object, int flgs, float clipScl), 0x409A20);

// void __usercall(NJS_ACTION* action@<eax>, float frame, int flgs, float clpScl, void* drwMdlFnc)
static const void* const DrawActionPtr = (void*)0x4053D0;
static inline void DrawAction(NJS_ACTION* action, float frame, int flgs, float clpScl, void* drwMdlFnc)
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

// void __usercall(obj *object@<edi>, NJS_MOTION *motion@<eax>, float frame, int flgs, float clpScl, void *drwMdlFnc)
static const void* const DrawMotionPtr = (void*)0x4052F0;
static inline void DrawMotion(obj* object, NJS_MOTION* motion, float frame, int flgs, float clpScl, void* drwMdlFnc)
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

// void __usercall(obj *object, NJS_MOTION_LINK *motionlink@<eax>, float frame, int flgs, float clpScl, void *drwMdlFnc)
static const void* const DrawMotionLinkPtr = (void*)0x4069A0;
static inline void DrawMotionLink(obj* object, NJS_MOTION_LINK* motionlink, float frame, int flgs, float clpScl, void* drwMdlFnc)
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

// void __usercall(obj *object, NJS_MOTION *motion@<eax>, NJS_MOTION *shape@<edi>, float frame, int flgs, float clpScl, void *drawModel)
static const void* const DrawShapeMotionPtr = (void*)0x405EF0;
static inline void DrawShapeMotion(obj* object, NJS_MOTION* motion, NJS_MOTION* shape, float frame, int flgs, float clpScl, void* drawModel)
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

// Variable draw functions (picks between Simple and Late)
FunctionPointer(void, lateDrawModel, (NJS_MODEL_SADX* mdl, int flgs), 0x4048E0);
FunctionPointer(void, lateDrawObject, (obj* object, int flgs, float clipScl), 0x4074A0);
FunctionPointer(void, late_Action, (NJS_ACTION* action, float frame, int flgs, float clipScl), 0x405490);

// Late draw functions: Model
FunctionPointer(void, late_DrawModel, (NJS_MODEL_SADX* mdl, int flgs), 0x4078F0);
FunctionPointer(void, late_DrawModelClip, (NJS_MODEL_SADX* mdl, int flgs, float clipScl), 0x407870);
FunctionPointer(void, late_DrawModelEx, (NJS_MODEL_SADX* mdl, int flgs), 0x409450);
FunctionPointer(void, late_DrawModelClipEx, (NJS_MODEL_SADX* mdl, int flgs, float clipScl), 0x4094D0);
FunctionPointer(void, late_DrawModelMS, (NJS_MODEL_SADX* mdl, int flgs), 0x409920);
FunctionPointer(void, late_DrawModelClipMS, (NJS_MODEL_SADX* mdl, int flgs, float clipScl), 0x4099A0);
FunctionPointer(void, late_DrawModelMesh, (NJS_MODEL_SADX* mdl, int flgs), 0x409DF0);
FunctionPointer(void, late_DrawModelClipMesh, (NJS_MODEL_SADX* mdl, int flgs, float clipScl), 0x409E70);
FunctionPointer(void, late_DrawShadowModel, (NJS_MODEL_SADX* mdl, float clipScl), 0x407B00);

// Late draw functions: Object
FunctionPointer(void, late_DrawObject, (obj* object, int flgs), 0x407B70);
FunctionPointer(void, late_DrawObjectClip, (obj* object, int flgs, float clipScl), 0x407B40);
FunctionPointer(void, late_DrawObjectClipEx, (obj* object, int flgs, float clipScl), 0x40A170);
FunctionPointer(void, late_DrawObjectEx, (obj* object, int flgs), 0x40A1A0);
FunctionPointer(void, late_DrawObjectClipMS, (obj* object, int flgs, float clipScl), 0x40A1E0);
FunctionPointer(void, late_DrawObjectMesh, (obj* object, int flgs), 0x40A210);
FunctionPointer(void, late_DrawObjectClipMesh, (obj* object, int flgs, float clipScl), 0x40A250);
FunctionPointer(void, late_DrawShadowObject, (obj* object, float clipScl), 0x408690);

// Late draw functions: Motion
FunctionPointer(void, late_DrawMotionClip, (obj* object, NJS_MOTION* motion, float frame, int flgs, float clipScl), 0x4053A0);
FunctionPointer(void, late_DrawMotionClipEx, (obj* object, NJS_MOTION* motion, float frame, int flgs, float clipScl), 0x4082D0);
FunctionPointer(void, late_DrawMotionClipMesh, (obj* object, NJS_MOTION* motion, float frame, int flgs, float clipScl), 0x408300);

// Late draw functions: Action
FunctionPointer(void, late_ActionEx, (NJS_ACTION* action, float frame, int flgs), 0x408330);
FunctionPointer(void, late_ActionClipEx, (NJS_ACTION* action, float frame, int flgs, float clipScl), 0x408350);
FunctionPointer(void, late_ActionMS, (NJS_ACTION* action, float frame, int flgs), 0x408380);
FunctionPointer(void, late_ActionClipMS, (NJS_ACTION* action, float frame, int flgs, float clipScl), 0x4083A0);
FunctionPointer(void, late_ActionMesh, (NJS_ACTION* action, float frame, int flgs), 0x4083D0);
FunctionPointer(void, late_ActionClipMesh, (NJS_ACTION* action, float frame, int flgs, float clipScl), 0x4083F0);

// Late draw functions: ActionLink
FunctionPointer(void, late_ActionLink, (NJS_ACTION_LINK* actionLink, float frame, int flgs), 0x406EF5);
FunctionPointer(void, late_ActionLinkEx, (NJS_ACTION_LINK* actionLink, float frame, int flgs), 0x4084B0);
FunctionPointer(void, late_ActionLinkMesh, (NJS_ACTION_LINK* actionLink, float frame, int flgs), 0x4084D0);

// Late draw functions: MotionLink
FunctionPointer(void, late_DrawMotionLinkEx, (obj* object, NJS_MOTION_LINK* motionLink, float frame, int flgs), 0x408480);

// Late draw functions: Shape motion
FunctionPointer(void, late_DrawShapeMotionEx, (obj* object, NJS_MOTION* motion, NJS_MOTION* shape, float frame, int flgs), 0x408420);
FunctionPointer(void, late_DrawShapeMotionMesh, (obj* object, NJS_MOTION* motion, NJS_MOTION* shape, float frame, int flgs), 0x408450);

// Chaos draw functions
FunctionPointer(void, CHAOS_DrawModel, (NJS_MODEL_SADX* mdl), 0x409EF0);
FunctionPointer(void, CHAOS_DrawObject, (obj* object), 0x40A280);
FunctionPointer(void, CHAOS_Action, (NJS_ACTION* action, float frame), 0x409FB0);
FunctionPointer(void, CHAOS_DrawShapeMotion, (obj* object, NJS_MOTION* motion, NJS_MOTION* shape, float frame), 0x40A050);

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
FunctionPointer(void, EV_CreateObjectFunc, (task** tp, task* (*func)(void), float x, float y, float z, int rx, int ry, int rz), 0x4316C0);
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
FunctionPointer(void, EV_SetMotion, (task* tp, obj* op, NJS_MOTION* mp, NJS_TEXLIST* lp, float speed, int mode, int linkframe), 0x42FE20);
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
FunctionPointer(void, AmyForEventHammerScaleIm, (uint8_t playernmb, float scale), 0x486EC0);
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
FunctionPointer(void, createModelEC, (float pos_x, float pos_y, float pos_z, int ang_x, int ang_y, int ang_z, float scl_x, float scl_y, float scl_z, obj* model, NJS_TEXLIST* texlist, int model_id), 0x6ECF20);

#endif /* SADXMODLOADER_SADXFUNCTIONSNEW_H */
