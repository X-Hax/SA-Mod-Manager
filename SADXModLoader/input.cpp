#include "stdafx.h"

#include <SADXModLoader.h>

#include "minmax.h"
#include "typedefs.h"
#include "input.h"
#include "rumble.h"
#include "DreamPad.h"


struct AnalogThing
{
	Angle angle;
	float magnitude;
};

namespace input
{
	ControllerData raw_input[GAMEPAD_COUNT];
	bool controller_enabled[GAMEPAD_COUNT];
	bool debug = false;
	bool disable_mouse = true;
	bool e_held = false;
	bool demo = false;
	KeyboardMapping keys;
	inline void poll_sdl()
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			default:
				break;

			case SDL_JOYDEVICEADDED:
			{
				const int which = event.cdevice.which;

				for (auto& controller : DreamPad::controllers)
				{
					// Checking for both in cases like the DualShock 4 and (e.g.) DS4Windows where the controller might be
					// "connected" twice with the same ID. DreamPad::open automatically closes if already open.
					if (!controller.connected() || controller.controller_id() == which)
					{
						controller.open(which);
						break;
					}
				}
				break;
			}

			case SDL_JOYDEVICEREMOVED:
			{
				const int which = event.cdevice.which;

				for (auto& controller : DreamPad::controllers)
				{
					if (controller.controller_id() == which)
					{
						controller.close();
						break;
					}
				}
				break;
			}
			}
		}

		SDL_GameControllerUpdate();
	}

	void poll_controllers()
	{
		poll_sdl();
		KeyboardMouse::poll();

		for (uint i = 0; i < GAMEPAD_COUNT; i++)
		{
			DreamPad& dreampad = DreamPad::controllers[i];

			dreampad.poll();
			raw_input[i] = dreampad.dreamcast_data();

			// Compatibility for mods which use ControllersRaw directly.
			// This will only copy the first four controllers.
			if (i < ControllersRaw.size())
			{
				ControllersRaw[i] = raw_input[i];
			}

#ifdef EXTENDED_BUTTONS
			if (dreampad.e_held_pad()) input::e_held = true;
			if (debug && raw_input[i].HeldButtons & Buttons_C)
			{
				const ControllerData& pad = raw_input[i];
				Motor m = DreamPad::controllers[i].active_motor();

				DisplayDebugStringFormatted(NJM_LOCATION(0, 8 + (3 * i)), "P%d  B: %08X LT/RT: %03d/%03d V: %d%d", (i + 1),
					pad.HeldButtons, pad.LTriggerPressure, pad.RTriggerPressure, (m & Motor::large), (m & Motor::small_) >> 1);
				DisplayDebugStringFormatted(NJM_LOCATION(4, 9 + (3 * i)), "LS: %4d/%4d (%f) RS: %4d/%4d (%f)",
					pad.LeftStickX, pad.LeftStickY, dreampad.normalized_l(), pad.RightStickX, pad.RightStickY,
					dreampad.normalized_r());

				if (pad.HeldButtons & Buttons_Z)
				{
					const int pressed = pad.PressedButtons;
					if (pressed & Buttons_Up)
					{
						dreampad.settings.rumble_factor += 0.125f;
					}
					else if (pressed & Buttons_Down)
					{
						dreampad.settings.rumble_factor -= 0.125f;
					}
					else if (pressed & Buttons_Left)
					{
						rumble::RumbleA_r(i, 0);
					}
					else if (pressed & Buttons_Right)
					{
						rumble::RumbleB_r(i, 7, 59, 6);
					}

					DisplayDebugStringFormatted(NJM_LOCATION(4, 10 + (3 * i)),
						"Rumble factor (U/D): %f (L/R to test)", dreampad.settings.rumble_factor);
				}
			}
#endif
		}
	}

	// ReSharper disable once CppDeclaratorNeverUsed
	static void WriteAnalogs_c()
	{
		if (!ControlEnabled)
		{
			return;
		}

		for (uint i = 0; i < GAMEPAD_COUNT; i++)
		{
			if (!controller_enabled[i])
			{
				continue;
			}

			const DreamPad& dream_pad = DreamPad::controllers[i];

			if (dream_pad.connected() || (dream_pad.settings.allow_keyboard && !i))
			{
				const ControllerData& pad = dream_pad.dreamcast_data();
				// SADX's internal deadzone is 12 of 127. It doesn't set the relative forward direction
				// unless this is exceeded in WriteAnalogs(), so the analog shouldn't be set otherwise.
				if (abs(pad.LeftStickX) > 12 || abs(pad.LeftStickY) > 12)
				{
					NormalizedAnalogs[i].magnitude = dream_pad.normalized_l();
				}
			}
		}
	}

	void __declspec(naked) WriteAnalogs_hook()
	{
		__asm
		{
			call WriteAnalogs_c
			ret
		}
	}

	// ReSharper disable once CppDeclaratorNeverUsed
	static void redirect_raw_controllers()
	{
		for (uint i = 0; i < GAMEPAD_COUNT; i++)
		{
			ControllerPointers[i] = &raw_input[i];
		}
	}

	void __declspec(naked) InitRawControllers_hook()
	{
		__asm
		{
			call redirect_raw_controllers
			ret
		}
	}

	void EnableController_r(Uint8 index)
	{
		// default behavior 
		if (index > 1)
		{
			controller_enabled[0] = true;
			controller_enabled[1] = true;
		}

		if (index > GAMEPAD_COUNT)
		{
			for (Uint32 i = 0; i < min(index, static_cast<Uint8>(GAMEPAD_COUNT)); i++)
			{
				EnableController_r(i);
			}
		}
		else
		{
			controller_enabled[index] = true;
		}
	}

	void DisableController_r(Uint8 index)
	{
		// default behavior 
		if (index > 1)
		{
			controller_enabled[0] = false;
			controller_enabled[1] = false;
		}

		if (index > GAMEPAD_COUNT)
		{
			for (Uint32 i = 0; i < min(index, static_cast<Uint8>(GAMEPAD_COUNT)); i++)
			{
				DisableController_r(i);
			}
		}
		else
		{
			controller_enabled[index] = false;
		}
	}
}
