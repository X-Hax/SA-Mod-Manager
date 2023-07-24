#pragma once

#include <ninja.h>
#include <SADXStructs.h>


struct KeyboardKey
{
	char held;
	char old;
	char pressed;
};

struct KeyboardStick : NJS_POINT2I
{
	Uint32 directions;
	static constexpr auto amount = 8192;

	void update();
};

class KeyboardMouse
{
public:
	static const ControllerData& dreamcast_data()
	{
		return pad;
	}
	static float normalized_l()
	{
		return normalized_l_;
	}
	static float normalized_r()
	{
		return normalized_r_;
	}

	static void poll();
	static void update_keyboard_buttons(Uint32 key, bool down);
	static void update_cursor(Sint32 xrel, Sint32 yrel);
	static void update_wheel(WPARAM wParam);
	static void reset_cursor();
	static void update_mouse_buttons(Uint32 button, bool down);
	static LRESULT read_window_message(HWND handle, UINT Msg, WPARAM wParam, LPARAM lParam);
	static void hook_wnd_proc();
	static void clear_sadx_keys();
	static void update_sadx_key(Uint32 key, bool down);

private:
	static ControllerData pad;
	static float normalized_l_, normalized_r_;
	static bool mouse_active;
	static bool wheel_active;
	static bool left_button;
	static bool right_button;
	static bool half_press;
	static bool e_held;
	static NJS_POINT2I cursor;
	static KeyboardStick sticks[2];
	static Sint16 mouse_x;
	static Sint16 mouse_y;
	static Sint16 wheel_delta;
	static WNDPROC lpPrevWndFunc;
};
