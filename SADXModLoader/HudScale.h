#pragma once
#include "SADXModLoader.h"
void SetupHudScale();
void __cdecl Draw2DSpriteHax(NJS_SPRITE *sp, Int n, Float pri, Uint32 attr, char zfunc_type);
void __cdecl SaveScale();
void __cdecl RestoreScale();
void __cdecl ScaleA();
void __cdecl ScaleB();
