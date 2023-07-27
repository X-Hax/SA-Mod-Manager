#include "stdafx.h"
#include "DreamPad.h"
#include "input.h"
#include "minmax.h"
#include "KeyboardMouse.h"
#include <map>

#if defined (_MSC_VER)
#undef max
#undef min
#define NOMINMAX
#endif


ControllerData KeyboardMouse::pad = {};
float          KeyboardMouse::normalized_l_ = 0.0f;
float          KeyboardMouse::normalized_r_ = 0.0f;
bool           KeyboardMouse::mouse_active = false;
bool           KeyboardMouse::wheel_active = false;
bool           KeyboardMouse::left_button = false;
bool           KeyboardMouse::right_button = false;
bool           KeyboardMouse::half_press = false;
bool           KeyboardMouse::e_held = false;
NJS_POINT2I    KeyboardMouse::cursor = {};
KeyboardStick  KeyboardMouse::sticks[2] = {};
Sint16         KeyboardMouse::mouse_x = 0;
Sint16         KeyboardMouse::mouse_y = 0;
Sint16         KeyboardMouse::wheel_delta = 0;
WNDPROC        KeyboardMouse::lpPrevWndFunc = nullptr;

std::map<int, int>SADX2004Keys = {
	{ VK_ESCAPE, 41 },
	{ VK_F1, 58 },
	{ VK_F2, 59 },
	{ VK_F3, 60 },
	{ VK_F4, 61 },
	{ VK_F5, 62 },
	{ VK_F6, 63 },
	{ VK_F7, 64 },
	{ VK_F8, 65 },
	{ VK_F9, 66 },
	{ VK_F10, 67 },
	{ VK_F11, 68 },
	{ VK_F12, 69 },
	{ VK_SNAPSHOT, 70 },
	{ VK_SCROLL, 71 },
	{ VK_PAUSE, 72 },
	{ 0xC0, 99 }, // ~ (doesn't exist in vanilla SADX)
	{ 0x31, 30 }, // 1
	{ 0x32, 31 }, // 2
	{ 0x33, 32 }, // 3
	{ 0x34, 33 }, // 4
	{ 0x35, 34 }, // 5
	{ 0x36, 35 }, // 6
	{ 0x37, 36 }, // 7
	{ 0x38, 37 }, // 8
	{ 0x39, 38 }, // 9
	{ 0x30, 39 }, // 0
	{ VK_OEM_MINUS, 45 }, // -
	{ 0xBB, 46 }, // =
	{ VK_BACK, 42 }, // Backspace
	{ VK_INSERT, 73 },
	{ VK_HOME, 74 },
	{ VK_PRIOR, 75 }, // Page Up
	{ VK_NUMLOCK, 83 }, // Num Lock
	{ 0x6F, 84 }, // Num slash
	{ 0x6A, 85 }, // Num asterisk
	{ 0x6D, 86 }, // Num -
	{ VK_TAB, 43 },
	{ 0x51, 20 }, // Q
	{ 0x57, 26 }, // W
	{ 0x45, 8 }, // E
	{ 0x52, 21 }, // R
	{ 0x54, 23 }, // T
	{ 0x59, 28 }, // Y
	{ 0x55, 24 }, // U
	{ 0x49, 12 }, // I
	{ 0x4F, 18 }, // O
	{ 0x50, 19 }, // P
	{ 219, 100 }, // [ (doesn't exist in vanilla SADX)
	{ 221, 101 }, // ] (doesn't exist in vanilla SADX)
	{ 220, 102 }, // Backslash (doesn't exist in vanilla SADX)
	{ VK_DELETE, 76 },
	{ VK_END, 77 },
	{ VK_NEXT, 78 }, // Page Down
	{ VK_NUMPAD7, 95 }, // Num 7 (Home)
	{ VK_NUMPAD8, 96 }, // Num 8 (Up)
	{ VK_NUMPAD9, 97 }, // Num 9 (Page Up)
	{ VK_ADD, 87 }, // Num Plus
	{ VK_CAPITAL, 103 }, // Caps Lock (doesn't exist in vanilla SADX)
	{ 0x41, 4 }, // A
	{ 0x53, 22 }, // S
	{ 0x44, 7 }, // D
	{ 0x46, 9 }, // F
	{ 0x47, 10 }, // G
	{ 0x48, 11 }, // H
	{ 0x4A, 13 }, // J
	{ 0x4B, 14 }, // K
	{ 0x4C, 15 }, // L
	{ VK_OEM_1, 51 }, // Semicolon
	{ VK_OEM_7, 52 }, // '
	{ VK_RETURN, 40 }, // Enter
	{ VK_NUMPAD4, 92 }, // Num 4 (Left)
	{ VK_NUMPAD5, 93 }, // Num 5
	{ VK_NUMPAD6, 94 }, // Num 6 (Right)
	{ VK_LSHIFT, 104 }, // Left Shift (doesn't exist in vanilla SADX)
	{ 0x5A, 29 }, // Z
	{ 0x58, 27 }, // X
	{ 0x43, 6 }, // C
	{ 0x56, 25 }, // V
	{ 0x42, 5 }, // B
	{ 0x4E, 17 }, // N
	{ 0x4D, 16 }, // M
	{ VK_OEM_COMMA, 54 },
	{ VK_OEM_PERIOD, 55 },
	{ VK_OEM_2, 56 }, // Slash
	{ VK_RSHIFT, 105 }, // Right Shift (doesn't exist in vanilla SADX)
	{ VK_UP, 82 }, // Up
	{ VK_NUMPAD1, 89 }, // Num 1 (End)
	{ VK_NUMPAD2, 90 }, // Num 2 (Down)
	{ VK_NUMPAD3, 91 }, // Num 3 (Page Down)
	{ VK_LCONTROL, 106 }, // Left Control (doesn't exist in vanilla SADX)
	{ VK_LMENU, 107 }, // Left Alt (doesn't exist in vanilla SADX)
	{ VK_SPACE, 44 },
	{ VK_RMENU, 108 }, // Right Alt (doesn't exist in vanilla SADX)
	{ VK_APPS, 109 }, // Menu (doesn't exist in vanilla SADX)
	{ VK_RCONTROL, 110 }, // Right Control (doesn't exist in vanilla SADX)
	{ VK_LEFT, 80 },
	{ VK_DOWN, 81 },
	{ VK_RIGHT, 79 },
	{ VK_NUMPAD0, 98 },
	{ 110, 111 }, // Numpad Delete (doesn't exist in vanilla SADX)
	{ 256, 112 }, // Numpad Enter (doesn't exist in vanilla SADX)};
};

void KeyboardMouse::clear_sadx_keys()
{
	for (int i = 0; i < 6; i++)
	{
		KeyboardInputPointer->indices[i] = 0;
	}
}

void KeyboardMouse::update_sadx_key(Uint32 key, bool down)
{
	auto it = SADX2004Keys.find(key);
	// No equivalent Windows -> SADX mapping available
	if (it == SADX2004Keys.end())
	{
		return;
	}
	// Update vanilla keyboard with up to 6 keys
	for (int i = 0; i < 6; i++)
	{
		if (down)
		{
			if (KeyboardInputPointer->indices[i] == 0)
			{
				KeyboardInputPointer->indices[i] = it->second;
				break;
			}
		}
		else
		{
			if (KeyboardInputPointer->indices[i] == it->second)
			{
				KeyboardInputPointer->indices[i] = 0;
			}
		}
	}
}

inline void set_button(Uint32& i, Uint32 value, bool down)
{
	down ? i |= value : i &= ~value;
}

LRESULT __stdcall poll_keyboard_mouse(HWND handle, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return KeyboardMouse::read_window_message(handle, Msg, wParam, lParam);
}

inline void normalize(const NJS_POINT2I& src, float* magnitude, short* out_x, short* out_y)
{
	constexpr auto short_max = std::numeric_limits<short>::max();
	auto x = static_cast<float>(clamp<short>(src.x, -short_max, short_max));
	auto y = static_cast<float>(clamp<short>(src.y, -short_max, short_max));
	float m = sqrt(x * x + y * y);

	if (m < FLT_EPSILON)
	{
		x = 0.0f;
		y = 0.0f;
	}
	else
	{
		x = x / m;
		y = y / m;
	}

	*magnitude = min(1.0f, m / static_cast<float>(short_max));

	*out_x = static_cast<short>(127 * x);
	*out_y = static_cast<short>(127 * y);
}

// TODO: framerate-independent interpolation
void KeyboardStick::update()
{
#define INTERPOLATE

	const auto horizontal = directions & (Buttons_Left | Buttons_Right);

	if (horizontal == Buttons_Left)
	{
#ifdef INTERPOLATE
		x = max(x - amount, -static_cast<int>(std::numeric_limits<short>::max()));
#else
		x = -static_cast<int>(std::numeric_limits<short>::max());
#endif
	}
	else if (horizontal == Buttons_Right)
	{
#ifdef INTERPOLATE
		x = min(x + amount, static_cast<int>(std::numeric_limits<short>::max()));
#else
		x = static_cast<int>(std::numeric_limits<short>::max());
#endif
	}
	else
	{
#ifdef INTERPOLATE
		if (x < 0)
		{
			x = min(x + amount, 0);
		}
		else if (x > 0)
		{
			x = max(x - amount, 0);
		}
#else
		x = 0;
#endif
	}

	const auto vertical = directions & (Buttons_Up | Buttons_Down);

	if (vertical == Buttons_Up)
	{
#ifdef INTERPOLATE
		y = max(y - amount, -static_cast<int>(std::numeric_limits<short>::max()));
#else
		y = -static_cast<int>(std::numeric_limits<short>::max());
#endif
	}
	else if (vertical == Buttons_Down)
	{
#ifdef INTERPOLATE
		y = min(y + amount, static_cast<int>(std::numeric_limits<short>::max()));
#else
		y = static_cast<int>(std::numeric_limits<short>::max());
#endif
	}
	else
	{
#ifdef INTERPOLATE
		if (y < 0)
		{
			y = min(y + amount, 0);
		}
		else if (y > 0)
		{
			y = max(y - amount, 0);
		}
#else
		y = 0;
#endif
	}
}

void KeyboardMouse::poll()
{
	hook_wnd_proc();

	sticks[0].update();
	sticks[1].update();
	NJS_POINT2I stick;

	if (sticks[0].x || sticks[0].y)
	{
		reset_cursor();
		stick = static_cast<NJS_POINT2I>(sticks[0]);
	}
	else
	{
		stick = cursor;
	}

	normalize(stick, &normalized_l_, &pad.LeftStickX, &pad.LeftStickY);
	normalize(sticks[1], &normalized_r_, &pad.RightStickX, &pad.RightStickY);

	if (half_press)
	{
		pad.LeftStickX /= 2;
		pad.LeftStickY /= 2;
		pad.RightStickX /= 2;
		pad.RightStickY /= 2;
		normalized_l_ /= 2.0f;
		normalized_r_ /= 2.0f;
	}

	if (wheel_active)
	{
		if (wheel_delta > 0)
		{
			set_button(pad.HeldButtons, Buttons_R, true);
			set_button(pad.HeldButtons, Buttons_L, false);
			wheel_delta--;
		}

		else if (wheel_delta < 0)
		{
			set_button(pad.HeldButtons, Buttons_L, true);
			set_button(pad.HeldButtons, Buttons_R, false);
			wheel_delta++;
		}

		else if (wheel_delta == 0)
		{
			wheel_active = false;
			set_button(pad.HeldButtons, Buttons_L, false);
			set_button(pad.HeldButtons, Buttons_R, false);
		}
	}

	DreamPad::update_buttons(pad, pad.HeldButtons);

	constexpr auto uchar_max = std::numeric_limits<uchar>::max();

	pad.LTriggerPressure = !!(pad.HeldButtons & Buttons_L) ? uchar_max : 0;
	pad.RTriggerPressure = !!(pad.HeldButtons & Buttons_R) ? uchar_max : 0;
	if (e_held) input::e_held = true;
	KeyboardUpdate();
}

void KeyboardMouse::update_keyboard_buttons(Uint32 key, bool down)
{
	// Update vanilla SADX array
	update_sadx_key(key, down);
	// Half press
	if (key == input::keys.Button_RightStick) half_press = down;
	// Center camera
	else if (key == input::keys.Button_LeftStick) e_held = down;
	// Buttons
	else if (key == input::keys.Button_A) set_button(pad.HeldButtons, Buttons_A, down);
	else if (key == input::keys.Button_B) set_button(pad.HeldButtons, Buttons_B, down);
	else if (key == input::keys.Button_X) set_button(pad.HeldButtons, Buttons_X, down);
	else if (key == input::keys.Button_Y) set_button(pad.HeldButtons, Buttons_Y, down);
	else if (key == input::keys.Button_RightShoulder) set_button(pad.HeldButtons, Buttons_Z, down);
	else if (key == input::keys.Button_LeftShoulder) set_button(pad.HeldButtons, Buttons_C, down);
	else if (key == input::keys.Button_Back) set_button(pad.HeldButtons, Buttons_D, down);
	else if (key == input::keys.Button_Start) set_button(pad.HeldButtons, Buttons_Start, down);
	// Triggers
	else if (key == input::keys.LT) set_button(pad.HeldButtons, Buttons_L, down);
	else if (key == input::keys.RT) set_button(pad.HeldButtons, Buttons_R, down);
	// D-Pad
	else if (key == input::keys.DPad_Up) set_button(pad.HeldButtons, Buttons_Up, down);
	else if (key == input::keys.DPad_Down) set_button(pad.HeldButtons, Buttons_Down, down);
	else if (key == input::keys.DPad_Left) set_button(pad.HeldButtons, Buttons_Left, down);
	else if (key == input::keys.DPad_Right) set_button(pad.HeldButtons, Buttons_Right, down);
	// Left stick
	else if (key == input::keys.Analog1_Up) set_button(sticks[0].directions, Buttons_Up, down);
	else if (key == input::keys.Analog1_Down) set_button(sticks[0].directions, Buttons_Down, down);
	else if (key == input::keys.Analog1_Left) set_button(sticks[0].directions, Buttons_Left, down);
	else if (key == input::keys.Analog1_Right) set_button(sticks[0].directions, Buttons_Right, down);
	// Right stick
	else if (key == input::keys.Analog2_Up) set_button(sticks[1].directions, Buttons_Up, down);
	else if (key == input::keys.Analog2_Down) set_button(sticks[1].directions, Buttons_Down, down);
	else if (key == input::keys.Analog2_Left) set_button(sticks[1].directions, Buttons_Left, down);
	else if (key == input::keys.Analog2_Right) set_button(sticks[1].directions, Buttons_Right, down);
}

void KeyboardMouse::update_cursor(Sint32 xrel, Sint32 yrel)
{
	if (!mouse_active || input::disable_mouse)
	{
		return;
	}

	CursorX = clamp(CursorX + xrel, -200, 200);
	CursorY = clamp(CursorY + yrel, -200, 200);

	auto& x = CursorX;
	auto& y = CursorY;

	auto m = x * x + y * y;

	if (m <= 625)
	{
		CursorMagnitude = 0;
		return;
	}

	CursorMagnitude = m / 361;

	if (CursorMagnitude >= 1)
	{
		if (CursorMagnitude > 120)
		{
			CursorMagnitude = 127;
		}
	}
	else
	{
		CursorMagnitude = 1;
	}

	njPushMatrix(reinterpret_cast<NJS_MATRIX_PTR>(0x0389D650));
	njRotateZ(nullptr, NJM_RAD_ANG(atan2(x, y)));

	NJS_VECTOR v = { 0.0f, static_cast<float>(CursorMagnitude) * 1.2f, 0.0f };
	njCalcVector(nullptr, &v, &v);

	CursorCos = static_cast<int>(v.x);
	CursorSin = static_cast<int>(v.y);

	constexpr auto short_max = static_cast<int>(std::numeric_limits<short>::max());

	auto& p = cursor;
	p.x = static_cast<Sint16>(clamp(static_cast<int>(-v.x / 128.0f * short_max), -short_max, short_max));
	p.y = static_cast<Sint16>(clamp(static_cast<int>(v.y / 128.0f * short_max), -short_max, short_max));

	njPopMatrix(1);
}

void KeyboardMouse::update_wheel(WPARAM wParam)
{
	if (input::disable_mouse)
	{
		return;
	}
	if ((int)wParam > 0)
	{
		wheel_delta = -15;
		wheel_active = true;
	}
	else if ((int)wParam < 0)
	{
		wheel_delta = 15;
		wheel_active = true;
	}
}

void KeyboardMouse::reset_cursor()
{
	CursorMagnitude = 0;
	CursorCos = 0;
	CursorSin = 0;
	CursorX = 0;
	CursorY = 0;
	cursor = {};
	mouse_active = false;
}

void KeyboardMouse::update_mouse_buttons(Uint32 button, bool down)
{
	if (input::disable_mouse) return;
	bool last_rmb = right_button;

	switch (button)
	{
	case VK_LBUTTON:
		left_button = down;

		if (!down && !MouseMode)
		{
			reset_cursor();
		}

		mouse_active = down;
		break;

	case VK_RBUTTON:
		right_button = down;
		break;

	case VK_MBUTTON:
		set_button(pad.HeldButtons, Buttons_Start, down);
		break;

	default:
		break;
	}

	if (left_button)
	{
		set_button(pad.HeldButtons, Buttons_B, right_button && right_button == last_rmb);
		set_button(pad.HeldButtons, Buttons_A, right_button && right_button != last_rmb);
	}
	else
	{
		set_button(pad.HeldButtons, Buttons_A, false);
		set_button(pad.HeldButtons, Buttons_B, right_button);
	}
}

WPARAM MapLeftRightKeys(WPARAM vk, LPARAM lParam)
{
	// Solution from https://stackoverflow.com/questions/5681284/how-do-i-distinguish-between-left-and-right-keys-ctrl-and-alt
	WPARAM new_vk = vk;
	UINT scancode = (lParam & 0x00ff0000) >> 16;
	int extended = (lParam & 0x01000000) != 0;

	switch (vk) {
	case VK_RETURN:
		if (lParam & 0x01000000) new_vk = 256;
		else new_vk = vk;
		break;
	case VK_SHIFT:
		new_vk = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);
		break;
	case VK_CONTROL:
		new_vk = extended ? VK_RCONTROL : VK_LCONTROL;
		break;
	case VK_MENU:
		new_vk = extended ? VK_RMENU : VK_LMENU;
		break;
	default:
		new_vk = vk;
		break;
	}
	return new_vk;
}

LRESULT KeyboardMouse::read_window_message(HWND handle, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_KILLFOCUS:
		sticks[0].directions = 0;
		sticks[1].directions = 0;
		pad.HeldButtons = 0;
		pad.LeftStickX = 0;
		pad.LeftStickY = 0;
		pad.RightStickX = 0;
		pad.RightStickY = 0;
		reset_cursor();
		clear_sadx_keys();
		break;

	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		update_mouse_buttons(VK_LBUTTON, Msg == WM_LBUTTONDOWN);
		break;

	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
		update_mouse_buttons(VK_RBUTTON, Msg == WM_RBUTTONDOWN);
		break;

	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
		update_mouse_buttons(VK_MBUTTON, Msg == WM_MBUTTONDOWN);
		break;

	case WM_MOUSEMOVE:
	{
		auto x = static_cast<short>(lParam & 0xFFFF);
		auto y = static_cast<short>(lParam >> 16);
		if (!input::disable_mouse)
		{
			update_cursor(x - mouse_x, y - mouse_y);
			mouse_x = x;
			mouse_y = y;
		}
		break;
	}

	case WM_MOUSEWHEEL:
		update_wheel(wParam);
		break;

	case WM_SYSKEYUP:
		update_keyboard_buttons(MapLeftRightKeys(wParam, lParam), Msg == WM_KEYDOWN || Msg == WM_SYSKEYDOWN);
		break;

	case WM_SYSKEYDOWN:
		if (wParam == VK_F2 && !(lParam & 0x40000000) && GameMode != 1 && GameMode != 8)
		{
			if (input::debug) PrintDebug("Soft reset\n");
			SoftResetByte = 1;
		}
		update_keyboard_buttons(MapLeftRightKeys(wParam, lParam), Msg == WM_KEYDOWN || Msg == WM_SYSKEYDOWN);
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
		update_keyboard_buttons(MapLeftRightKeys(wParam, lParam), Msg == WM_KEYDOWN || Msg == WM_SYSKEYDOWN);
		break;

	default:
		break;
	}

	return CallWindowProc(lpPrevWndFunc, handle, Msg, wParam, lParam);
}

void KeyboardMouse::hook_wnd_proc()
{
	if (lpPrevWndFunc == nullptr)
	{
		lpPrevWndFunc = reinterpret_cast<WNDPROC>(SetWindowLong(hWnd, GWL_WNDPROC, reinterpret_cast<LONG>(poll_keyboard_mouse)));
	}
}
