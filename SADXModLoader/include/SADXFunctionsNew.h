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

#endif /* SADXMODLOADER_SADXFUNCTIONSNEW_H */
