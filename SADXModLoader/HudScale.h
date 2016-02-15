#pragma once
#include "SADXModLoader.h"

void SetupHudScale();

void __cdecl Draw2DSpriteHax(NJS_SPRITE *sp, Int n, Float pri, Uint32 attr, char zfunc_type);
void __cdecl ScalePush(bool center_screen = false);
void __cdecl ScalePop();
void __cdecl ScaleA();
void __cdecl ScaleB();
void __cdecl ScaleStageMission(ObjectMaster* _this);
short __cdecl ScalePauseMenu();
