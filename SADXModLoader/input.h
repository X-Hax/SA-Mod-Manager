#pragma once
#include "DreamPad.h"

namespace input
{
	void poll_controllers();
	void WriteAnalogs_hook();
	void InitRawControllers_hook();
	void __cdecl EnableController_r(Uint8 index);
	void __cdecl DisableController_r(Uint8 index);

	extern ControllerData raw_input[GAMEPAD_COUNT];
	extern bool controller_enabled[GAMEPAD_COUNT];
	extern bool debug;
	extern bool disable_mouse;
	extern bool e_held;
	extern bool demo;
	extern KeyboardMapping keys;
}

void SDL2_Init();
void SDL2_OnExit();
void SDL2_OnInput();

extern bool isInputMod;