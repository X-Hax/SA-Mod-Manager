#pragma once
#include <SADXModLoader.h>
#include "DreamPad.h"

namespace rumble
{
	//FunctionPointer(Sint32, pdVibMxStart, (Uint32 port, const PDS_VIBPARAM *a2), 0x00785280);
	FunctionPointer(Sint32, pdVibMxStop, (Uint32 port), 0x00785330);

	Sint32 __cdecl pdVibMxStop_r(Uint32 port);
	void __cdecl Rumble_Load_hook(Uint32 port, Uint32 time, Motor motor);
	void __cdecl RumbleA_r(Uint32 port, Uint32 time);
	void __cdecl RumbleB_r(Uint32 port, Uint32 time, int a3, int a4);
	void default_rumble();
}