#include "stdafx.h"
#include <Windows.h>
#include <direct.h>	// for _getcwd

#include <string>
#include <sstream>	// because

#include "SDL.h"

#include <SADXModLoader.h>
#include <IniFile.hpp>

#include "typedefs.h"
#include "input.h"
#include "rumble.h"
#include "minmax.h"

static void* RumbleA_ptr = reinterpret_cast<void*>(0x004BCBC0);
static void* RumbleB_ptr = reinterpret_cast<void*>(0x004BCC10);
static void* UpdateControllers_ptr = reinterpret_cast<void*>(0x0040F460);
static void* AnalogHook_ptr = reinterpret_cast<void*>(0x0040F343);
static void* InitRawControllers_ptr = reinterpret_cast<void*>(0x0040F451); // End of function (hook)

KeyboardInput SADXKeyboard = { 0, 0, { 0, 0, 0, 0, 0, 0 }, nullptr };

PointerInfo jumps[] = {
	{ rumble::pdVibMxStop, rumble::pdVibMxStop_r },
	{ RumbleA_ptr, rumble::RumbleA_r },
	{ RumbleB_ptr, rumble::RumbleB_r },
	{ AnalogHook_ptr, input::WriteAnalogs_hook },
	{ InitRawControllers_ptr, input::InitRawControllers_hook },
	{ EnableController, input::EnableController_r },
	{ DisableController, input::DisableController_r },
	{ reinterpret_cast<void*>(0x0042D52D), rumble::default_rumble },
	// Used to skip over the standard controller update function.
	// This has no effect on the OnInput hook.
	{ UpdateControllers_ptr, reinterpret_cast<void*>(0x0040FDB3) }
};

static std::string build_mod_path(const char* modpath, const char* path)
{
	std::stringstream result;
	char workingdir[FILENAME_MAX]{};

	result << _getcwd(workingdir, FILENAME_MAX) << "\\" << modpath << "\\" << path;

	return result.str();
}

int GetEKey(int index)
{
	if (input::e_held && IsFreeCameraAllowed())
	{
		input::e_held = false;
		return 1;
	}
	return 0;
}

void CreateSADXKeyboard(KeyboardInput* ptr, int length)
{
	KeyboardInputPointer = &SADXKeyboard;
}

void SDL2_OnInput()
{
	input::poll_controllers();

	// Soft reset
	if (ControllerPointers[0]->HeldButtons == (Buttons_A | Buttons_B | Buttons_X | Buttons_Y) && ControllerPointers[0]->ReleasedButtons == Buttons_Start && GameMode != 1 && GameMode != 8)
	{
		if (input::debug) 
			PrintDebug("Soft reset\n");

		SoftResetByte = 1;
	}

	// Demo playback
	if (DemoPlaying && Demo_Enabled && CurrentDemoCutsceneID < 0 && !input::demo) input::demo = true; //Ignore everything but the Start button
	if (input::demo)
	{
		DemoControllerData* demo_memory = (DemoControllerData*)HeapThing;
		if (input::debug) PrintDebug("Demo frame:%d\n", Demo_Frame);
		if (GameState == 15)
		{

			if (input::raw_input[0].HeldButtons & Buttons_Start || Demo_Frame > Demo_MaxFrame || demo_memory[Demo_Frame].HeldButtons == -1)
			{
				Demo_Enabled = 0;
				StartLevelCutscene(6);
				input::demo = false;
				Demo_Frame = 0;
			}
			ControllerPointers[0]->HeldButtons = demo_memory[Demo_Frame].HeldButtons;
			ControllerPointers[0]->LTriggerPressure = demo_memory[Demo_Frame].LTrigger;
			ControllerPointers[0]->RTriggerPressure = demo_memory[Demo_Frame].RTrigger;
			ControllerPointers[0]->LeftStickX = demo_memory[Demo_Frame].StickX;
			ControllerPointers[0]->LeftStickY = demo_memory[Demo_Frame].StickY;
			ControllerPointers[0]->RightStickX = 0;
			ControllerPointers[0]->RightStickY = 0;
			ControllerPointers[0]->NotHeldButtons = demo_memory[Demo_Frame].NotHeldButtons;
			ControllerPointers[0]->PressedButtons = demo_memory[Demo_Frame].PressedButtons;
			ControllerPointers[0]->ReleasedButtons = demo_memory[Demo_Frame].ReleasedButtons;
		}
		else
		{
			input::raw_input[0].LeftStickX = 0;
			input::raw_input[0].LeftStickY = 0;
			input::raw_input[0].HeldButtons = 0;
			input::raw_input[0].LTriggerPressure = 0;
			input::raw_input[0].RTriggerPressure = 0;
			input::raw_input[0].LeftStickX = 0;
			input::raw_input[0].LeftStickY = 0;
			input::raw_input[0].RightStickX = 0;
			input::raw_input[0].RightStickY = 0;
			input::raw_input[0].NotHeldButtons = -1;
			input::raw_input[0].PressedButtons = 0;
			input::raw_input[0].ReleasedButtons = 0;
		}
	}

}

void SDL2_Init()
{
	const char* path = "SDL2\\";

	if (GetModuleHandleA("sadx-input-mod") != nullptr)
	{
		return;
	}

	std::string dll = build_mod_path(path, "SDL2.dll");

	const auto handle = LoadLibraryA(dll.c_str());

	if (handle == nullptr)
	{
		PrintDebug("[Input] Unable to load SDL2.dll.\n");

		MessageBoxA(nullptr, "Error loading SDL. See debug message for details.",
			"SDL Load Error", MB_OK | MB_ICONERROR);

		return;
	}

	int init;
	if ((init = SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC | SDL_INIT_EVENTS)) != 0)
	{
		PrintDebug("[Input] Unable to initialize SDL. Error code: %i\n", init);
		MessageBoxA(nullptr, "Error initializing SDL. See debug message for details.",
			"SDL Init Error", MB_OK | MB_ICONERROR);
		return;
	}

	// Replace function to get the E key for centering camera on character
	WriteCall(reinterpret_cast<void*>(0x00437547), GetEKey);
	// Replace call to CreateKeyboardDevice to set up SADX keyboard pointer
	WriteCall(reinterpret_cast<void*>(0x0077F0D7), CreateSADXKeyboard);
	// Disable call to CreateMouseDevice
	WriteData<5>(reinterpret_cast<void*>(0x0077F03E), 0x90i8);
	// Disable call to DirectInput_Init
	WriteData<5>(reinterpret_cast<void*>(0x0077F205), 0x90i8);

	// EnableControl
	WriteData(reinterpret_cast<bool**>(0x40EF80), &input::controller_enabled[0]);
	WriteData(reinterpret_cast<bool**>(0x40EF86), &input::controller_enabled[1]);
	WriteData(reinterpret_cast<bool**>(0x40EF90), input::controller_enabled);

	// DisableControl
	WriteData(reinterpret_cast<bool**>(0x40EFB0), &input::controller_enabled[0]);
	WriteData(reinterpret_cast<bool**>(0x40EFB6), &input::controller_enabled[1]);
	WriteData(reinterpret_cast<bool**>(0x40EFC0), input::controller_enabled);

	// IsControllerEnabled
	WriteData(reinterpret_cast<bool**>(0x40EFD8), input::controller_enabled);

	// Control
	WriteData(reinterpret_cast<bool**>(0x40FE0D), input::controller_enabled);
	WriteData(reinterpret_cast<bool**>(0x40FE2F), &input::controller_enabled[1]);

	// WriteAnalogs
	WriteData(reinterpret_cast<bool**>(0x40F30C), input::controller_enabled);

	input::controller_enabled[0] = true;
	input::controller_enabled[1] = true;

	std::string dbpath = build_mod_path(path, "gamecontrollerdb.txt");

	if (FileExists(dbpath))
	{
		int result = SDL_GameControllerAddMappingsFromFile(dbpath.c_str());

		if (result == -1)
		{
			PrintDebug("[Input] Error loading gamecontrollerdb: %s\n", SDL_GetError());
		}
		else
		{
			PrintDebug("[Input] Controller mappings loaded: %i\n", result);
		}
	}

	const std::string config_path = build_mod_path(path, "config.ini");

#ifdef _DEBUG
	const bool debug_default = true;
#else
	const bool debug_default = false;
#endif

	IniFile config(config_path);
	ushort KeyboardPlayer = max(0, min(7, config.getInt("Config", "KeyboardPlayer", 0)));
	input::debug = config.getBool("Config", "Debug", debug_default);
	input::disable_mouse = config.getBool("Config", "DisableMouse", true);
	// Keyboard mappings
	input::keys.Analog1_Up = config.getInt("Keyboard", "-lefty", 38);
	input::keys.Analog1_Down = config.getInt("Keyboard", "+lefty", 40);
	input::keys.Analog1_Left = config.getInt("Keyboard", "-leftx", 37);
	input::keys.Analog1_Right = config.getInt("Keyboard", "+leftx", 39);
	input::keys.Analog2_Up = config.getInt("Keyboard", "-righty", 73);
	input::keys.Analog2_Down = config.getInt("Keyboard", "+righty", 77);
	input::keys.Analog2_Left = config.getInt("Keyboard", "-rightx", 74);
	input::keys.Analog2_Right = config.getInt("Keyboard", "+rightx", 76);
	input::keys.Button_A = config.getInt("Keyboard", "a", 88);
	input::keys.Button_B = config.getInt("Keyboard", "b", 90);
	input::keys.Button_X = config.getInt("Keyboard", "x", 65);
	input::keys.Button_Y = config.getInt("Keyboard", "y", 83);
	input::keys.Button_Start = config.getInt("Keyboard", "start", 13);
	input::keys.Button_Back = config.getInt("Keyboard", "back", 86);
	input::keys.LT = config.getInt("Keyboard", "lefttrigger", 81);
	input::keys.RT = config.getInt("Keyboard", "righttrigger", 87);
	input::keys.Button_LeftShoulder = config.getInt("Keyboard", "leftshoulder", 67);
	input::keys.Button_RightShoulder = config.getInt("Keyboard", "rightshoulder", 66);
	input::keys.Button_LeftStick = config.getInt("Keyboard", "leftstick", 69);
	input::keys.Button_RightStick = config.getInt("Keyboard", "rightstick", 160);
	input::keys.DPad_Up = config.getInt("Keyboard", "dpup", 104);
	input::keys.DPad_Down = config.getInt("Keyboard", "dpdown", 98);
	input::keys.DPad_Left = config.getInt("Keyboard", "dpleft", 100);
	input::keys.DPad_Right = config.getInt("Keyboard", "dpright", 102);
	// This defaults RadialR to enabled if smooth-cam is detected.
	const bool smooth_cam = GetModuleHandleA("smooth-cam.dll") != nullptr;

	for (ushort i = 0; i < GAMEPAD_COUNT; i++)
	{
		DreamPad::Settings& settings = DreamPad::controllers[i].settings;

		const std::string section = "Controller " + std::to_string(i + 1);

		const int deadzone_l = config.getInt(section, "DeadzoneL", GAMEPAD_LEFT_THUMB_DEADZONE);
		const int deadzone_r = config.getInt(section, "DeadzoneR", GAMEPAD_RIGHT_THUMB_DEADZONE);

		settings.set_deadzone_l(deadzone_l);
		settings.set_deadzone_r(deadzone_r);

		settings.radial_l = config.getBool(section, "RadialL", true);
		settings.radial_r = config.getBool(section, "RadialR", smooth_cam);

		settings.trigger_threshold = config.getInt(section, "TriggerThreshold", GAMEPAD_TRIGGER_THRESHOLD);

		settings.rumble_factor = clamp(config.getFloat(section, "RumbleFactor", 1.0f), 0.0f, 1.0f);

		settings.mega_rumble = config.getBool(section, "MegaRumble", false);
		settings.rumble_min_time = static_cast<ushort>(config.getInt(section, "RumbleMinTime", 0));

		settings.allow_keyboard = (i == KeyboardPlayer);

		if (input::debug)
		{
			PrintDebug("[Input] Deadzones for P%d (L/R/T): %05d / %05d / %05d\n", (i + 1),
				settings.deadzone_l, settings.deadzone_r, settings.trigger_threshold);
		}
	}

	PrintDebug("[Input] Initialization complete.\n");
}

void SDL2_OnExit()
{
	for (auto& i : DreamPad::controllers)
	{
		i.close();
	}
}
