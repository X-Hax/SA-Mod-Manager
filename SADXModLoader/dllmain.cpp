#include "stdafx.h"

#include <cassert>

#include <deque>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include "direct3d.h"

using std::deque;
using std::ifstream;
using std::string;
using std::wstring;
using std::unique_ptr;
using std::unordered_map;
using std::vector;

// Win32 headers.
#include <DbgHelp.h>
#include <Shlwapi.h>
#include <GdiPlus.h>
#include "resource.h"

#include "git.h"
#include "version.h"

#include "IniFile.hpp"
#include "CodeParser.hpp"
#include "MediaFns.hpp"
#include "TextConv.hpp"
#include "SADXModLoader.h"
#include "LandTableInfo.h"
#include "ModelInfo.h"
#include "AnimationFile.h"
#include "TextureReplacement.h"
#include "FileReplacement.h"
#include "FileSystem.h"
#include "Events.h"
#include "AutoMipmap.h"
#include "UiScale.h"
#include "FixFOV.h"
#include "EXEData.h"
#include "DLLData.h"

static HINSTANCE g_hinstDll = nullptr;

/**
 * Show an error message indicating that this isn't the 2004 US version.
 * This function also calls ExitProcess(1).
 */
static void ShowNon2004USError(void)
{
	MessageBox(nullptr, L"This copy of Sonic Adventure DX is not the 2004 US version.\n\n"
		L"Please obtain the EXE file from the 2004 US version and try again.",
		L"SADX Mod Loader", MB_ICONERROR);
	ExitProcess(1);
}

/**
 * Hook SADX's CreateFileA() import.
 */
static void HookCreateFileA(void)
{
	ULONG ulSize = 0;
	PROC pNewFunction;
	PROC pActualFunction;

	PCSTR pcszModName;

	// SADX module handle. (main executable)
	HMODULE hModule = GetModuleHandle(nullptr);
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc;

	pNewFunction = (PROC)MyCreateFileA;
	// Get the actual CreateFileA() using GetProcAddress().
	pActualFunction = GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "CreateFileA");
	assert(pActualFunction != nullptr);

	pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(
		hModule, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize);

	if (pImportDesc == nullptr)
		return;

	for (; pImportDesc->Name; pImportDesc++)
	{
		// get the module name
		pcszModName = (PCSTR)((PBYTE)hModule + pImportDesc->Name);

		// check if the module is kernel32.dll
		if (pcszModName != nullptr && _stricmp(pcszModName, "Kernel32.dll") == 0)
		{
			// get the module
			PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)((PBYTE)hModule + pImportDesc->FirstThunk);

			for (; pThunk->u1.Function; pThunk++)
			{
				PROC* ppfn = (PROC*)&pThunk->u1.Function;
				if (*ppfn == pActualFunction)
				{
					// Found CreateFileA().
					DWORD dwOldProtect = 0;
					VirtualProtect(ppfn, sizeof(pNewFunction), PAGE_WRITECOPY, &dwOldProtect);
					WriteData(ppfn, pNewFunction);
					VirtualProtect(ppfn, sizeof(pNewFunction), dwOldProtect, &dwOldProtect);
					// FIXME: Would it be listed multiple times?
					break;
				} // Function that we are looking for
			}
		}
	}
}

/**
 * Change write protection of the .rdata section.
 * @param protect True to protect; false to unprotect.
 */
static void SetRDataWriteProtection(bool protect)
{
	// Reference: https://stackoverflow.com/questions/22588151/how-to-find-data-segment-and-code-segment-range-in-program
	// TODO: Does this handle ASLR? (SADX doesn't use ASLR, though...)

	// SADX module handle. (main executable)
	HMODULE hModule = GetModuleHandle(nullptr);

	// Get the PE header.
	const IMAGE_NT_HEADERS *const pNtHdr = ImageNtHeader(hModule);
	// Section headers are located immediately after the PE header.
	const IMAGE_SECTION_HEADER *pSectionHdr = reinterpret_cast<const IMAGE_SECTION_HEADER*>(pNtHdr+1);

	// Find the .rdata section.
	for (unsigned int i = pNtHdr->FileHeader.NumberOfSections; i > 0; i--, pSectionHdr++)
	{
		if (strncmp(reinterpret_cast<const char*>(pSectionHdr->Name), ".rdata", sizeof(pSectionHdr->Name)) != 0)
			continue;

		// Found the .rdata section.
		// Verify that this matches SADX.
		if (pSectionHdr->VirtualAddress != 0x3DB000 ||
			pSectionHdr->Misc.VirtualSize != 0xB6B88)
		{
			// Not SADX, or the wrong version.
			ShowNon2004USError();
			ExitProcess(1);
		}

		const intptr_t vaddr = reinterpret_cast<intptr_t>(hModule) + pSectionHdr->VirtualAddress;
		DWORD flOldProtect;
		DWORD flNewProtect = (protect ? PAGE_READONLY : PAGE_WRITECOPY);
		VirtualProtect(reinterpret_cast<void*>(vaddr), pSectionHdr->Misc.VirtualSize, flNewProtect, &flOldProtect);
		return;
	}

	// .rdata section not found.
	ShowNon2004USError();
	ExitProcess(1);
}

struct message
{
	string text;
	uint32_t time;
};

static deque<message> msgqueue;

static const uint32_t fadecolors[] = {
	0xF7FFFFFF, 0xEEFFFFFF, 0xE6FFFFFF, 0xDDFFFFFF,
	0xD5FFFFFF, 0xCCFFFFFF, 0xC4FFFFFF, 0xBBFFFFFF,
	0xB3FFFFFF, 0xAAFFFFFF, 0xA2FFFFFF, 0x99FFFFFF,
	0x91FFFFFF, 0x88FFFFFF, 0x80FFFFFF, 0x77FFFFFF,
	0x6FFFFFFF, 0x66FFFFFF, 0x5EFFFFFF, 0x55FFFFFF,
	0x4DFFFFFF, 0x44FFFFFF, 0x3CFFFFFF, 0x33FFFFFF,
	0x2BFFFFFF, 0x22FFFFFF, 0x1AFFFFFF, 0x11FFFFFF,
	0x09FFFFFF, 0
};

// Code Parser.
static CodeParser codeParser;

static void __cdecl ProcessCodes()
{
	codeParser.processCodeList();
	RaiseEvents(modFrameEvents);

	const int numrows = (VerticalResolution / (int)DebugFontSize);
	int pos;
	if ((int)msgqueue.size() <= numrows - 1)
		pos = (numrows - 1) - (msgqueue.size() - 1);
	else
		pos = 0;
	if (msgqueue.size() > 0)
		for (deque<message>::iterator iter = msgqueue.begin();
			iter != msgqueue.end(); ++iter)
	{
		int c = -1;
		if (300 - iter->time < LengthOfArray(fadecolors))
			c = fadecolors[LengthOfArray(fadecolors) - (300 - iter->time) - 1];
		SetDebugFontColor((int)c);
		DisplayDebugString(pos++, (char *)iter->text.c_str());
		if (++iter->time >= 300)
		{
			msgqueue.pop_front();
			if (msgqueue.size() == 0)
				break;
			iter = msgqueue.begin();
		}
		if (pos == numrows)
			break;
	}
}


static bool dbgConsole, dbgScreen;
// File for logging debugging output.
static FILE *dbgFile = nullptr;

/**
 * SADX Debug Output function.
 * @param Format Format string.
 * @param ... Arguments.
 * @return Return value from vsnprintf().
 */
static int __cdecl SADXDebugOutput(const char *Format, ...)
{
	va_list ap;
	va_start(ap, Format);
	int result = vsnprintf(nullptr, 0, Format, ap) + 1;
	va_end(ap);
	char *buf = new char[result+1];
	va_start(ap, Format);
	result = vsnprintf(buf, result+1, Format, ap);
	va_end(ap);

	// Console output.
	if (dbgConsole)
	{
		// TODO: Convert from Shift-JIS to CP_ACP?
		fputs(buf, stdout);
		fflush(stdout);
	}

	// Screen output.
	if (dbgScreen)
	{
		message msg = { { buf }, 0 };
		// Remove trailing newlines if present.
		while (!msg.text.empty() &&
			(msg.text[msg.text.size()-1] == '\n' ||
			 msg.text[msg.text.size()-1] == '\r'))
		{
			msg.text.resize(msg.text.size()-1);
		}
		msgqueue.push_back(msg);
	}

	// File output.
	if (dbgFile)
	{
		// SADX prints text in Shift-JIS.
		// Convert it to UTF-8 before writing it to the debug file.
		char *utf8 = SJIStoUTF8(buf);
		if (utf8)
		{
			fputs(utf8, dbgFile);
			fflush(dbgFile);
			delete[] utf8;
		}
	}

	delete[] buf;
	return result;
}

enum windowmodes { windowed, fullscreen };
struct windowsize { int x; int y; int width; int height; };
struct windowdata { int x; int y; int width; int height; DWORD style; DWORD exStyle; };

// Used for borderless windowed mode.
// Defines the size of the outer-window which wraps the game window and draws the background.
static windowdata outerSizes[] = {
	{ CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, WS_CAPTION | WS_SYSMENU | WS_VISIBLE, 0 }, // windowed
	{ 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, WS_POPUP | WS_VISIBLE, WS_EX_APPWINDOW } // fullscreen
};

// Used for borderless windowed mode.
// Defines the size of the inner-window on which the game is rendered.
static windowsize innerSizes[2] = {};

static HWND        accelWindow = nullptr;
static windowmodes windowMode  = windowmodes::windowed;
static HACCEL      accelTable  = nullptr;

DataPointer(int, dword_3D08534, 0x3D08534);
static void __cdecl HandleWindowMessages_r()
{
	MSG Msg; // [sp+4h] [bp-1Ch]@1

	if (PeekMessageA(&Msg, nullptr, 0, 0, 1u))
	{
		do
		{
			if (!TranslateAccelerator(accelWindow, accelTable, &Msg))
			{
				TranslateMessage(&Msg);
				DispatchMessageA(&Msg);
			}
		} while (PeekMessageA(&Msg, nullptr, 0, 0, 1u));
		dword_3D08534 = Msg.wParam;
	}
	else
	{
		dword_3D08534 = Msg.wParam;
	}
}

static vector<RECT> screenBounds;
static Gdiplus::Bitmap* backgroundImage = nullptr;
static bool switchingWindowMode         = false;
static bool borderlessWindow            = false;
static bool scaleScreen                 = true;
static bool windowResize                = false;
static unsigned int screenNum           = 1;
static bool customWindowSize            = false;
static int customWindowWidth            = 640;
static int customWindowHeight           = 480;

static BOOL CALLBACK GetMonitorSize(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	screenBounds.push_back(*lprcMonitor);
	return TRUE;
}

static const uint8_t wndpatch[] = { 0xA1, 0x30, 0xFD, 0xD0, 0x03, 0xEB, 0x08 }; // mov eax,[hWnd] / jmp short 0xf
static int currentScreenSize[2];

static LRESULT CALLBACK WrapperWndProc(HWND wrapper, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_ACTIVATE:
			if ((LOWORD(wParam) != WA_INACTIVE && !lParam) || reinterpret_cast<HWND>(lParam) == WindowHandle)
			{
				SetFocus(WindowHandle);
				return 0;
			}
			break;

		case WM_CLOSE:
			// we also need to let SADX do cleanup
			SendMessageA(WindowHandle, WM_CLOSE, wParam, lParam);
			// what we do here is up to you: we can check if SADX decides to close, and if so, destroy ourselves, or something like that
			return 0;

		case WM_ERASEBKGND:
		{
			if (backgroundImage == nullptr || windowResize)
			{
				break;
			}

			Gdiplus::Graphics gfx((HDC)wParam);
			gfx.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);

			RECT rect;
			GetClientRect(wrapper, &rect);

			auto w = rect.right - rect.left;
			auto h = rect.bottom - rect.top;

			if (w == innerSizes[windowMode].width && h == innerSizes[windowMode].height)
			{
				break;
			}
			
			gfx.DrawImage(backgroundImage, 0, 0, w, h);
			return 0;
		}

		case WM_SIZE:
		{
			auto& inner = innerSizes[windowMode];

			if (windowResize)
			{
				inner.x      = 0;
				inner.y      = 0;
				inner.width  = LOWORD(lParam);
				inner.height = HIWORD(lParam);
			}

			// update the inner window (game view)
			SetWindowPos(WindowHandle, HWND_TOP, inner.x, inner.y, inner.width, inner.height, 0);
			break;
		}

		case WM_COMMAND:
		{
			if (wParam != MAKELONG(ID_FULLSCREEN, 1))
				break;

			switchingWindowMode = true;

			if (windowMode == windowed)
			{
				RECT rect;
				GetWindowRect(wrapper, &rect);

				outerSizes[windowMode].x       = rect.left;
				outerSizes[windowMode].y       = rect.top;
				outerSizes[windowMode].width   = rect.right - rect.left;
				outerSizes[windowMode].height  = rect.bottom - rect.top;
				outerSizes[windowMode].style   = GetWindowLongA(accelWindow, GWL_STYLE);
				outerSizes[windowMode].exStyle = GetWindowLongA(accelWindow, GWL_EXSTYLE);
			}

			windowMode = windowMode == windowed ? fullscreen : windowed;
			
			// update outer window (draws background)
			const auto& outer = outerSizes[windowMode];
			SetWindowLongA(accelWindow, GWL_STYLE, outer.style);
			SetWindowLongA(accelWindow, GWL_EXSTYLE, outer.exStyle);
			SetWindowPos(accelWindow, HWND_NOTOPMOST, outer.x, outer.y, outer.width, outer.height, SWP_FRAMECHANGED);

			switchingWindowMode = false;
			return 0;
		}

		case WM_ACTIVATEAPP:
			if (!switchingWindowMode)
			{
				WndProc_B(WindowHandle, uMsg, wParam, lParam);
			}

			if (windowMode == windowed)
			{
				while (ShowCursor(TRUE) < 0);
			}
			else
			{
				while (ShowCursor(FALSE) > 0);
			}

		default:
			break;
	}

	// alternatively we can return SendMe
	return DefWindowProcA(wrapper, uMsg, wParam, lParam);
}

static RECT   last_rect    = {};
static Uint32 last_width   = 0;
static Uint32 last_height  = 0;
static DWORD  last_style   = 0;
static DWORD  last_exStyle = 0;

static void enable_fullscreen_mode(HWND handle)
{
	IsWindowed = false;
	last_width = HorizontalResolution;
	last_height = VerticalResolution;

	GetWindowRect(handle, &last_rect);
	last_style = GetWindowLongA(handle, GWL_STYLE);
	last_exStyle = GetWindowLongA(handle, GWL_EXSTYLE);
	SetWindowLongA(handle, GWL_STYLE, WS_POPUP | WS_SYSMENU | WS_VISIBLE);
	while (ShowCursor(FALSE) > 0);
}

static void enable_windowed_mode(HWND handle)
{
	SetWindowLongA(handle, GWL_STYLE, last_style);
	SetWindowLongA(handle, GWL_EXSTYLE, last_exStyle);

	auto width = last_rect.right - last_rect.left;
	auto height = last_rect.bottom - last_rect.top;

	if (width <= 0 || height <= 0)
	{
		last_rect = {};
		last_rect.right = 640;
		last_rect.bottom = 480;
		AdjustWindowRectEx(&last_rect, last_style, false, last_exStyle);
		width = last_rect.right - last_rect.left;
		height = last_rect.bottom - last_rect.top;
	}

	SetWindowPos(handle, HWND_NOTOPMOST, last_rect.left, last_rect.top, width, height, 0);
	IsWindowed = true;
	while (ShowCursor(TRUE) < 0);
}

static LRESULT CALLBACK WndProc_Resizable(HWND handle, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
		default:
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_SIZE:
		{
			if (customWindowSize)
			{
				break;
			}

			if (!IsWindowed || Direct3D_Device == nullptr)
			{
				return 0;
			}

			int w = LOWORD(lParam);
			int h = HIWORD(lParam);

			if (!w || !h)
			{
				break;
			}

			direct3d::change_resolution(w, h);
			break;
		}

		case WM_COMMAND:
		{
			if (wParam != MAKELONG(ID_FULLSCREEN, 1))
			{
				break;
			}

			if (direct3d::is_windowed() && IsWindowed)
			{
				enable_fullscreen_mode(handle);

				const auto& rect = screenBounds[screenNum == 0 ? 0 : screenNum - 1];

				const auto w = rect.right - rect.left;
				const auto h = rect.bottom - rect.top;

				direct3d::change_resolution(w, h, false);
			}
			else
			{
				direct3d::change_resolution(last_width, last_height, true);
				enable_windowed_mode(handle);
			}

			return 0;
		}
	}

	return DefWindowProcA(handle, Msg, wParam, lParam);
}

LRESULT __stdcall WndProc_hook(HWND handle, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProcA(handle, Msg, wParam, lParam);
}

wstring borderimg = L"mods\\Border.png";
static void CreateSADXWindow_r(HINSTANCE hInstance, int nCmdShow)
{
	// Primary window class name.
	const char *const lpszClassName = GetWindowClassName();

	// Hook default return of SADX's window procedure to force it to return DefWindowProc
	WriteJump(reinterpret_cast<void*>(0x00789E48), WndProc_hook);

	// Primary window class for SADX.
	WNDCLASSA v8; // [sp+4h] [bp-28h]@1
	v8.style         = 0;
	v8.lpfnWndProc   = (windowResize ? WndProc_Resizable : WndProc);
	v8.cbClsExtra    = 0;
	v8.cbWndExtra    = 0;
	v8.hInstance     = hInstance;
	v8.hIcon         = LoadIconA(hInstance, MAKEINTRESOURCEA(101));
	v8.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	v8.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	v8.lpszMenuName  = nullptr;
	v8.lpszClassName = lpszClassName;
	if (!RegisterClassA(&v8))
		return;

	RECT windowRect;
	windowRect.top = 0;
	windowRect.left = 0;
	if (customWindowSize)
	{
		windowRect.right = customWindowWidth;
		windowRect.bottom = customWindowHeight;
	}
	else
	{
		windowRect.right = HorizontalResolution;
		windowRect.bottom = VerticalResolution;
	}

	if (borderlessWindow || IsWindowed)
	{
		currentScreenSize[0] = GetSystemMetrics(SM_CXSCREEN);
		currentScreenSize[1] = GetSystemMetrics(SM_CYSCREEN);
		WriteData((int **)0x79426E, &currentScreenSize[0]);
		WriteData((int **)0x79427A, &currentScreenSize[1]);
	}

	EnumDisplayMonitors(nullptr, nullptr, GetMonitorSize, 0);

	int screenX, screenY, screenW, screenH, wsX, wsY, wsW, wsH;
	if (screenNum > 0)
	{
		if (screenBounds.size() < screenNum)
			screenNum = 1;

		RECT screenSize = screenBounds[screenNum - 1];
		wsX = screenX = screenSize.left;
		wsY = screenY = screenSize.top;
		wsW = screenW = screenSize.right - screenSize.left;
		wsH = screenH = screenSize.bottom - screenSize.top;
	}
	else
	{
		screenX = GetSystemMetrics(SM_XVIRTUALSCREEN);
		screenY = GetSystemMetrics(SM_YVIRTUALSCREEN);
		screenW = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		screenH = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		wsX = 0;
		wsY = 0;
		wsW = GetSystemMetrics(SM_CXSCREEN);
		wsH = GetSystemMetrics(SM_CYSCREEN);
	}

	accelTable = LoadAcceleratorsA(g_hinstDll, MAKEINTRESOURCEA(IDR_ACCEL_WRAPPER_WINDOW));

	if (borderlessWindow)
	{
		if (windowResize)
			outerSizes[windowed].style |= WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX;

		AdjustWindowRectEx(&windowRect, outerSizes[windowed].style, false, 0);

		outerSizes[windowed].width = windowRect.right - windowRect.left;
		outerSizes[windowed].height = windowRect.bottom - windowRect.top;

		outerSizes[windowed].x = wsX + ((wsW - outerSizes[windowed].width) / 2);
		outerSizes[windowed].y = wsY + ((wsH - outerSizes[windowed].height) / 2);

		outerSizes[fullscreen].x = screenX;
		outerSizes[fullscreen].y = screenY;
		outerSizes[fullscreen].width = screenW;
		outerSizes[fullscreen].height = screenH;

		if (customWindowSize)
		{
			float num = min((float)customWindowWidth / (float)HorizontalResolution, (float)customWindowHeight / (float)VerticalResolution);
			innerSizes[windowed].width = (int)((float)HorizontalResolution * num);
			innerSizes[windowed].height = (int)((float)VerticalResolution * num);
			innerSizes[windowed].x = (customWindowWidth - innerSizes[windowed].width) / 2;
			innerSizes[windowed].y = (customWindowHeight - innerSizes[windowed].height) / 2;
		}
		else
		{
			innerSizes[windowed].width = HorizontalResolution;
			innerSizes[windowed].height = VerticalResolution;
			innerSizes[windowed].x = 0;
			innerSizes[windowed].y = 0;
		}

		if (scaleScreen)
		{
			float num = min((float)screenW / (float)HorizontalResolution, (float)screenH / (float)VerticalResolution);
			innerSizes[fullscreen].width = (int)((float)HorizontalResolution * num);
			innerSizes[fullscreen].height = (int)((float)VerticalResolution * num);
		}
		else
		{
			innerSizes[fullscreen].width = HorizontalResolution;
			innerSizes[fullscreen].height = VerticalResolution;
		}

		innerSizes[fullscreen].x = (screenW - innerSizes[fullscreen].width) / 2;
		innerSizes[fullscreen].y = (screenH - innerSizes[fullscreen].height) / 2;

		windowMode = IsWindowed ? windowed : fullscreen;

		if (!FileExists(borderimg))
			borderimg = L"mods\\Border_Default.png";
		if (FileExists(borderimg))
		{
			Gdiplus::GdiplusStartupInput gdiplusStartupInput;
			ULONG_PTR gdiplusToken;
			Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
			backgroundImage = Gdiplus::Bitmap::FromFile(borderimg.c_str());
		}

		// Register a window class for the wrapper window.
		WNDCLASSA w;

		w.style         = 0;
		w.lpfnWndProc   = WrapperWndProc;
		w.cbClsExtra    = 0;
		w.cbWndExtra    = 0;
		w.hInstance     = hInstance;
		w.hIcon         = LoadIconA(hInstance, MAKEINTRESOURCEA(101));
		w.hCursor       = LoadCursor(nullptr, IDC_ARROW);
		w.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		w.lpszMenuName  = nullptr;
		w.lpszClassName = "WrapperWindow";

		if (!RegisterClassA(&w))
			return;

		const auto& outerSize = outerSizes[windowMode];

		accelWindow = CreateWindowExA(outerSize.exStyle,
			"WrapperWindow",
			lpszClassName,
			outerSize.style,
			outerSize.x, outerSize.y, outerSize.width, outerSize.height,
			nullptr, nullptr, hInstance, nullptr);

		if (accelWindow == nullptr)
			return;

		const auto& innerSize = innerSizes[windowMode];

		WindowHandle = CreateWindowExA(0,
			lpszClassName,
			lpszClassName,
			WS_CHILD | WS_VISIBLE,
			innerSize.x, innerSize.y, innerSize.width, innerSize.height,
			accelWindow, nullptr, hInstance, nullptr);

		SetFocus(WindowHandle);
		ShowWindow(accelWindow, nCmdShow);
		UpdateWindow(accelWindow);
		SetForegroundWindow(accelWindow);

		IsWindowed = true;

		WriteData((void *)0x402C61, wndpatch);
	}
	else
	{
		DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE;
		DWORD dwExStyle = 0;

		if (windowResize)
		{
			dwStyle |= WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX;
		}

		AdjustWindowRectEx(&windowRect, dwStyle, false, dwExStyle);
		int w = windowRect.right - windowRect.left;
		int h = windowRect.bottom - windowRect.top;
		int x = wsX + ((wsW - w) / 2);
		int y = wsY + ((wsH - h) / 2);
		WindowHandle = CreateWindowExA(dwExStyle,
			lpszClassName,
			lpszClassName,
			dwStyle,
			x, y, w, h,
			nullptr, nullptr, hInstance, nullptr);

		if (!IsWindowed)
		{
			enable_fullscreen_mode(WindowHandle);
		}

		ShowWindow(WindowHandle, nCmdShow);
		UpdateWindow(WindowHandle);
		SetForegroundWindow(WindowHandle);

		accelWindow = WindowHandle;
	}

	// Hook the window message handler.
	WriteJump((void *)HandleWindowMessages, (void *)HandleWindowMessages_r);
}

static __declspec(naked) void CreateSADXWindow_asm()
{
	__asm
	{
		mov ebx, [esp+4]
		push ebx
		push eax
		call CreateSADXWindow_r
		add esp, 8
		retn
	}
}

static unordered_map<unsigned char, unordered_map<int, StartPosition> > StartPositions;
static void RegisterStartPosition(unsigned char character, const StartPosition &position)
{
	auto iter = StartPositions.find(character);
	if (iter == StartPositions.end())
	{
		// No start positions registered for this character.
		// Initialize it with the default start positions.
		const StartPosition *origlist;
		switch (character)
		{
		case Characters_Sonic:
			origlist = SonicStartArray;
			break;
		case Characters_Tails:
			origlist = TailsStartArray;
			break;
		case Characters_Knuckles:
			origlist = KnucklesStartArray;
			break;
		case Characters_Amy:
			origlist = AmyStartArray;
			break;
		case Characters_Gamma:
			origlist = GammaStartArray;
			break;
		case Characters_Big:
			origlist = BigStartArray;
			break;
		default:
			return;
		}
		unordered_map<int, StartPosition> &newlist = StartPositions[character];
		for (; origlist->LevelID != LevelIDs_Invalid; origlist++)
		{
			newlist[levelact(origlist->LevelID, origlist->ActID)] = *origlist;
		}
		// Update the start position for the specified level.
		newlist[levelact(position.LevelID, position.ActID)] = position;
	}
	else
	{
		// Start positions have already been registered.
		// Update the existing map.
		iter->second[levelact(position.LevelID, position.ActID)] = position;
	}
}

static void ClearStartPositionList(unsigned char character)
{
	switch (character)
	{
	case Characters_Sonic:
	case Characters_Tails:
	case Characters_Knuckles:
	case Characters_Amy:
	case Characters_Gamma:
	case Characters_Big:
		break;
	default:
		return;
	}
	StartPositions[character].clear();
}

static unordered_map<unsigned char, unordered_map<int, FieldStartPosition> > FieldStartPositions;
static void RegisterFieldStartPosition(unsigned char character, const FieldStartPosition &position)
{
	if (character >= Characters_MetalSonic) return;
	auto iter = FieldStartPositions.find(character);
	if (iter == FieldStartPositions.end())
	{
		// No field start positions registered for this character.
		// Initialize it with the default field start positions.
		const FieldStartPosition *origlist = StartPosList_FieldReturn[character];
		unordered_map<int, FieldStartPosition> &newlist = FieldStartPositions[character];
		for (; origlist->LevelID != LevelIDs_Invalid; origlist++)
		{
			newlist[levelact(origlist->LevelID, origlist->FieldID)] = *origlist;
		}
		// Update the field start position for the specified level.
		newlist[levelact(position.LevelID, position.FieldID)] = position;
	}
	else
	{
		// Field start positions have already been registered.
		// Update the existing map.
		iter->second[levelact(position.LevelID, position.FieldID)] = position;
	}
}

static void ClearFieldStartPositionList(unsigned char character)
{
	if (character >= Characters_MetalSonic) return;
	FieldStartPositions[character].clear();
}

static unordered_map<int, PathDataPtr> Paths;
static bool PathsInitialized;
static void RegisterPathList(const PathDataPtr &paths)
{
	if (!PathsInitialized)
	{
		const PathDataPtr *oldlist = PathDataPtrs;
		for (; oldlist->LevelAct != 0xFFFF; oldlist++)
		{
			Paths[oldlist->LevelAct] = *oldlist;
		}
		PathsInitialized = true;
	}
	Paths[paths.LevelAct] = paths;
}

static void ClearPathListList()
{
	Paths.clear();
	PathsInitialized = true;
}

static unordered_map<unsigned char, vector<PVMEntry> > CharacterPVMs;
static void RegisterCharacterPVM(unsigned char character, const PVMEntry &pvm)
{
	if (character > Characters_MetalSonic) return;
	auto iter = CharacterPVMs.find(character);
	if (iter == CharacterPVMs.end())
	{
		// Character PVM vector has not been created yet.
		// Initialize it with the texture list.
		const PVMEntry *origlist = CharacterPVMEntries[character];
		vector<PVMEntry> &newlist = CharacterPVMs[character];
		for (; origlist->TexList != nullptr; origlist++)
		{
			newlist.push_back(*origlist);
		}
		// Add the new PVM.
		newlist.push_back(pvm);
	}
	else
	{
		// Character PVM vector has been created.
		// Add the new texture.
		iter->second.push_back(pvm);
	}
}

static void ClearCharacterPVMList(unsigned char character)
{
	if (character > Characters_MetalSonic) return;
	CharacterPVMs[character].clear();
}

static vector<PVMEntry> CommonObjectPVMs;
static bool CommonObjectPVMsInitialized;
static void RegisterCommonObjectPVM(const PVMEntry &pvm)
{
	if (!CommonObjectPVMsInitialized)
	{
		const PVMEntry *oldlist = &OBJ_REGULAR_TEXLISTS[0];
		for (; oldlist->TexList != nullptr; oldlist++)
		{
			CommonObjectPVMs.push_back(*oldlist);
		}
		CommonObjectPVMsInitialized = true;
	}
	CommonObjectPVMs.push_back(pvm);
}

static void ClearCommonObjectPVMList()
{
	CommonObjectPVMs.clear();
	CommonObjectPVMsInitialized = true;
}

static unsigned char trialcharacters[] = { 0, 0xFFu, 1, 2, 0xFFu, 3, 5, 4, 6 };
static inline unsigned char gettrialcharacter(unsigned char character)
{
	if (character >= LengthOfArray(trialcharacters))
		return 0xFF;
	return trialcharacters[character];
}

static unordered_map<unsigned char, vector<TrialLevelListEntry> > _TrialLevels;
static void RegisterTrialLevel(unsigned char character, const TrialLevelListEntry &level)
{
	character = gettrialcharacter(character);
	if (character == 0xFF) return;
	auto iter = _TrialLevels.find(character);
	if (iter == _TrialLevels.end())
	{
		// Trial level vector has not been registered yet.
		// Initialize it with the standard trial level list.
		const TrialLevelList *const origlist = &TrialLevels[character];
		vector<TrialLevelListEntry> &newlist = _TrialLevels[character];
		newlist.resize(origlist->Count);
		memcpy(newlist.data(), origlist->Levels, sizeof(TrialLevelListEntry) * origlist->Count);
		// Add the new trial level.
		newlist.push_back(level);
	}
	else
	{
		// Trial level vector has already been created.
		// Add the new level.
		iter->second.push_back(level);
	}
}

static void ClearTrialLevelList(unsigned char character)
{
	character = gettrialcharacter(character);
	if (character == 0xFF) return;
	_TrialLevels[character].clear();
}

static unordered_map<unsigned char, vector<TrialLevelListEntry> > _TrialSubgames;
static void RegisterTrialSubgame(unsigned char character, const TrialLevelListEntry &level)
{
	character = gettrialcharacter(character);
	if (character == 0xFF) return;
	auto iter = _TrialSubgames.find(character);
	if (iter == _TrialSubgames.end())
	{
		// Trial subgame vector has not been registered yet.
		// Initialize it with the standard trial subgame list.
		const TrialLevelList *const origlist = &TrialSubgames[character];
		vector<TrialLevelListEntry> &newlist = _TrialSubgames[character];
		newlist.resize(origlist->Count);
		memcpy(newlist.data(), origlist->Levels, sizeof(TrialLevelListEntry) * origlist->Count);
		// Add the new trial subgame.
		newlist.push_back(level);
	}
	else
	{
		// Trial subgame vector has already been created.
		// Add the new subgame.
		iter->second.push_back(level);
	}
}

static void ClearTrialSubgameList(unsigned char character)
{
	character = gettrialcharacter(character);
	if (character == 0xFF) return;
	_TrialSubgames[character].clear();
}

static const char *mainsavepath = "SAVEDATA";
static const char *GetMainSavePath()
{
	return mainsavepath;
}

static const char *chaosavepath = "SAVEDATA";
static const char *GetChaoSavePath()
{
	return chaosavepath;
}

const char* __cdecl GetReplaceablePath(const char* path)
{
	return sadx_fileMap.replaceFile(path);
}

void _ReplaceFile(const char *src, const char *dst)
{
	sadx_fileMap.addReplaceFile(src, dst);
}

string windowtitle;
void SetWindowTitle(const char *title)
{
	if (WindowHandle)
		SetWindowTextA(WindowHandle, title);
	else
		windowtitle = title;
}

static vector<SoundList> _SoundLists;
int RegisterSoundList(const SoundList &list)
{
	if (_SoundLists.empty())
	{
		_SoundLists.resize(SoundLists_Length);
		memcpy(_SoundLists.data(), SoundLists, sizeof(SoundList) * SoundLists_Length);
	}
	_SoundLists.push_back(list);
	SoundLists_Cust = _SoundLists.data();
	SoundLists_Cust_Length = _SoundLists.size();
	return _SoundLists.size() - 1;
}

static const HelperFunctions helperFunctions =
{
	ModLoaderVer,
	&RegisterStartPosition,
	&ClearStartPositionList,
	&RegisterFieldStartPosition,
	&ClearFieldStartPositionList,
	&RegisterPathList,
	&ClearPathListList,
	&RegisterCharacterPVM,
	&ClearCharacterPVMList,
	&RegisterCommonObjectPVM,
	&ClearCommonObjectPVMList,
	&RegisterTrialLevel,
	&ClearTrialLevelList,
	&RegisterTrialSubgame,
	&ClearTrialSubgameList,
	&GetMainSavePath,
	&GetChaoSavePath,
	&GetReplaceablePath,
	&_ReplaceFile,
	&SetWindowTitle,
	&RegisterSoundList
};

static const char *const dlldatakeys[] = {
	"CHRMODELSData",
	"ADV00MODELSData",
	"ADV01MODELSData",
	"ADV01CMODELSData",
	"ADV02MODELSData",
	"ADV03MODELSData",
	"BOSSCHAOS0MODELSData",
	"CHAOSTGGARDEN02MR_DAYTIMEData",
	"CHAOSTGGARDEN02MR_EVENINGData",
	"CHAOSTGGARDEN02MR_NIGHTData"
};

void __cdecl WriteSaveFile_r()
{
	char v0; // bl@1
	SaveFileInfo *v2; // edi@8
	char v3[MAX_PATH]; // esi@8
	FILE *v5; // edi@20

	v0 = 1;
	*(char *)0x3ABDF7A = 0;
	*(char *)0x3B291B1 = 0;
	if (!*(int *)0x3B29198)
	{
		if (*(unsigned char *)0x3B291B2 > 4u)
		{
			*(int *)0x3B291A4 = 0;
			*(char *)0x3B291AD = 0;
			*(char *)0x3ABDF76 = 0;
			*(char *)0x3B22E1E = 0;
			*(char *)0x3B291B2 = 0;
		}
		CreateDirectoryA(mainsavepath, nullptr);
		if (!*(char *)0x3B291B0)
			SaveSave();
		if (*(char *)0x3B291B3)
		{
			*(char *)0x3B291B3 = 0;
			if ((unsigned __int8)*(char *)0x3B290E0 > 98u)
				return;
			v2 = SaveFiles->Next;
			snprintf(v3, MAX_PATH, "SonicDX%02d.snc", 1);
			if (v2)
				while (1)
				{
					if (CompareStringA(9u, 1u, v3, -1, v2->Filename, -1) == 2)
					{
						v2 = SaveFiles->Next;
						++v0;
						if ((unsigned __int8)v0 > 99u)
							return;
						snprintf(v3, MAX_PATH, "SonicDX%02d.snc", (unsigned __int8)v0);
					}
					else
						v2 = v2->Next;
					if (!v2)
						break;
				}
			if (*(char **)0x3B290DC != nullptr)
			{
				free(*(char **)0x3B290DC);
				*(char **)0x3B290DC = nullptr;
			}
			*(char **)0x3B290DC = (char *)malloc(0xEu);
			++*(char *)0x3B290E0;
			*(char *)0x3B290D8 = v0;
			snprintf(*(char **)0x3B290DC, 0xEu, "SonicDX%02d.snc", (unsigned __int8)v0);
			snprintf(v3, MAX_PATH, "./%s/SonicDX%02d.snc", mainsavepath, (unsigned __int8)v0);
		}
		else
		{
			lstrlenA(*(char **)0x3B290DC);
			snprintf(v3, MAX_PATH, "./%s/%s", mainsavepath, *(char **)0x3B290DC);
		}
		v5 = fopen(v3, "wb");
		fwrite((void *)0x3B2B3A8, sizeof(SaveFileData), 1, v5);
		*(char *)0x3B290E8 = 0;
		InputThing__Ctor();
		*(char *)0x3B291B2 = 0;
		*(int *)0x3B291A4 = 0;
		*(char *)0x3ABDF7A = 0;
		*(char *)0x3B291AD = 0;
		*(char *)0x3ABDF76 = 0;
		*(char *)0x3B22E1E = 0;
		fclose(v5);
		*(char *)0x3B291B1 = 1;
	}
}

int __cdecl FixEKey(int i)
{
	return IsCameraControlEnabled() && GetKey(i);
}

const auto loc_794566 = (void*)0x00794566;
void __declspec(naked) PolyBuff_Init_FixVBuffParams()
{
	__asm
	{
		push D3DPOOL_MANAGED
		push ecx
		push D3DUSAGE_WRITEONLY
		jmp loc_794566
	}
}

static void __cdecl InitMods()
{
	// Hook present function to handle device lost/reset states
	direct3d::init();

	FILE *f_ini = _wfopen(L"mods\\SADXModLoader.ini", L"r");
	if (!f_ini)
	{
		MessageBox(nullptr, L"mods\\SADXModLoader.ini could not be read!", L"SADX Mod Loader", MB_ICONWARNING);
		return;
	}
	unique_ptr<IniFile> ini(new IniFile(f_ini));
	fclose(f_ini);

	// Get sonic.exe's path and filename.
	wchar_t pathbuf[MAX_PATH];
	GetModuleFileName(nullptr, pathbuf, MAX_PATH);
	wstring exepath(pathbuf);
	wstring exefilename;
	string::size_type slash_pos = exepath.find_last_of(L"/\\");
	if (slash_pos != string::npos)
	{
		exefilename = exepath.substr(slash_pos + 1);
		if (slash_pos > 0)
			exepath = exepath.substr(0, slash_pos);
	}

	// Convert the EXE filename to lowercase.
	transform(exefilename.begin(), exefilename.end(), exefilename.begin(), ::towlower);

	// Process the main Mod Loader settings.
	const IniGroup *settings = ini->getGroup("");

	if (settings->getBool("DebugConsole"))
	{
		// Enable the debug console.
		// TODO: setvbuf()?
		AllocConsole();
		SetConsoleTitle(L"SADX Mod Loader output");
		freopen("CONOUT$", "wb", stdout);
		dbgConsole = true;
	}

	dbgScreen = settings->getBool("DebugScreen");
	if (settings->getBool("DebugFile"))
	{
		// Enable debug logging to a file.
		// dbgFile will be nullptr if the file couldn't be opened.
		dbgFile = _wfopen(L"mods\\SADXModLoader.log", L"a+");
	}

	// Is any debug method enabled?
	if (dbgConsole || dbgScreen || dbgFile)
	{
		WriteJump((void *)PrintDebug, (void *)SADXDebugOutput);
		PrintDebug("SADX Mod Loader v" VERSION_STRING " (API version %d), built " __TIMESTAMP__ "\n",
			ModLoaderVer);
#ifdef MODLOADER_GIT_VERSION
#ifdef MODLOADER_GIT_DESCRIBE
		PrintDebug("%s, %s\n", MODLOADER_GIT_VERSION, MODLOADER_GIT_DESCRIBE);
#else /* !MODLOADER_GIT_DESCRIBE */
		PrintDebug("%s\n", MODLOADER_GIT_VERSION);
#endif /* MODLOADER_GIT_DESCRIBE */
#endif /* MODLOADER_GIT_VERSION */
	}

	WriteJump((void *)0x789E50, CreateSADXWindow_asm); // override window creation function
	// Other various settings.
	if (settings->getBool("DisableCDCheck"))
		WriteJump((void *)0x402621, (void *)0x402664);

	// Custom resolution.
	WriteJump((void *)0x40297A, (void *)0x402A90);

	int hres = settings->getInt("HorizontalResolution", 640);
	if (hres > 0)
	{
		HorizontalResolution = hres;
		HorizontalStretch = HorizontalResolution / 640.0f;
	}

	int vres = settings->getInt("VerticalResolution", 480);
	if (vres > 0)
	{
		VerticalResolution = vres;
		VerticalStretch = VerticalResolution / 480.0f;
	}

	fov::initialize();

	borderlessWindow   = settings->getBool("WindowedFullscreen");
	scaleScreen        = settings->getBool("StretchFullscreen", true);
	screenNum          = settings->getInt("ScreenNum", 1);
	customWindowSize   = settings->getBool("CustomWindowSize");
	customWindowWidth  = settings->getInt("WindowWidth", 640);
	customWindowHeight = settings->getInt("WindowHeight", 480);
	windowResize       = settings->getBool("ResizableWindow") && !customWindowSize;

	if (!borderlessWindow)
	{
		vector<uint8_t> nop(5, 0x90);
		WriteData((void*)0x007943D0, nop.data(), nop.size());

		// SADX automatically corrects values greater than the number of adapters available.
		// DisplayAdapter is unsigned, so -1 will be greater than the number of adapters, and it will reset.
		DisplayAdapter = screenNum - 1;
	}

	// Causes significant performance drop on some systems.
	if (windowResize)
	{
		// MeshSetBuffer_CreateVertexBuffer: Change D3DPOOL_DEFAULT to D3DPOOL_MANAGED
		WriteData((char*)0x007853F3, (char)D3DPOOL_MANAGED);
		// MeshSetBuffer_CreateVertexBuffer: Remove D3DUSAGE_DYNAMIC
		WriteData((short*)0x007853F6, (short)D3DUSAGE_WRITEONLY);
		// PolyBuff_Init: Remove D3DUSAGE_DYNAMIC and set pool to D3DPOOL_MANAGED
		WriteJump((void*)0x0079455F, PolyBuff_Init_FixVBuffParams);
	}

	if (!settings->getBool("PauseWhenInactive", true))
		WriteData((uint8_t *)0x401914, (uint8_t)0xEBu);

	if (settings->getBool("AutoMipmap", true))
		mipmap::enable_auto_mipmaps();

	// Hijack a ton of functions in SADX.
	*(void **)0x38A5DB8 = (void *)0x38A5D94; // depth buffer fix
	WriteCall((void *)0x437547, FixEKey);
	WriteCall((void *)0x42544C, (void *)PlayMusicFile_r);
	WriteCall((void *)0x4254F4, (void *)PlayVoiceFile_r);
	WriteCall((void *)0x425569, (void *)PlayVoiceFile_r);
	WriteCall((void *)0x513187, (void *)PlayVideoFile_r);
	WriteJump((void *)0x40D1EA, (void *)WMPInit_r);
	WriteJump((void *)0x40CF50, (void *)WMPRestartMusic_r);
	WriteJump((void *)0x40D060, (void *)PauseSound_r);
	WriteJump((void *)0x40D0A0, (void *)ResumeSound_r);
	WriteJump((void *)0x40CFF0, (void *)WMPClose_r);
	WriteJump((void *)0x40D28A, (void *)WMPRelease_r);
	WriteJump(LoadSoundList, LoadSoundList_r);

	// Fixes N-sided polygons (Gamma's headlight) by using
	// triangle strip vertex buffer initializers.

	for (int i = 0; i < MeshSetInitFunctions_Length; ++i)
	{
		auto& a = MeshSetInitFunctions[i];
		a[NJD_MESHSET_N >> 14] = a[NJD_MESHSET_TRIMESH >> 14];
	}

	for (int i = 0; i < PolyBuffDraw_VertexColor_Length; ++i)
	{
		auto& a = PolyBuffDraw_VertexColor[i];
		a[NJD_MESHSET_N >> 14] = a[NJD_MESHSET_TRIMESH >> 14];
	}

	for (int i = 0; i < PolyBuffDraw_NoVertexColor_Length; ++i)
	{
		auto& a = PolyBuffDraw_NoVertexColor[i];
		a[NJD_MESHSET_N >> 14] = a[NJD_MESHSET_TRIMESH >> 14];
	}

	// Replaces half-pixel offset addition with subtraction
	WriteData((uint8_t*)0x0077DE1E, (uint8_t)0x25);
	WriteData((uint8_t*)0x0077DE33, (uint8_t)0x25);
	WriteData((uint8_t*)0x0078E822, (uint8_t)0x25);
	WriteData((uint8_t*)0x0078E83C, (uint8_t)0x25);
	WriteData((uint8_t*)0x0078E991, (uint8_t)0x25);
	WriteData((uint8_t*)0x0078E9AE, (uint8_t)0x25);
	WriteData((uint8_t*)0x0078EA41, (uint8_t)0x25);
	WriteData((uint8_t*)0x0078EA5E, (uint8_t)0x25);
	WriteData((uint8_t*)0x0078EAE1, (uint8_t)0x25);
	WriteData((uint8_t*)0x0078EAFE, (uint8_t)0x25);

	texpack::init();

	// Unprotect the .rdata section.
	SetRDataWriteProtection(false);

	// Enables GUI texture filtering (D3DTEXF_POINT -> D3DTEXF_LINEAR)
	if (settings->getBool("TextureFilter", true))
	{
		WriteData((uint8_t*)0x0078B7C4, (uint8_t)0x02);
		WriteData((uint8_t*)0x0078B7D8, (uint8_t)0x02);
		WriteData((uint8_t*)0x0078B7EC, (uint8_t)0x02);
	}

	direct3d::set_vsync(settings->getBool("EnableVsync", true));

	if (settings->getBool("ScaleHud", false))
	{
		uiscale::initialize();
	}

	int bgFill = settings->getInt("BackgroundFillMode", uiscale::FillMode::fill);
	if (bgFill >= 0 && bgFill <= 3)
	{
		uiscale::bg_fill = static_cast<uiscale::FillMode>(bgFill);
		uiscale::setup_background_scale();
	}

	int fmvFill = settings->getInt("FmvFillMode", uiscale::FillMode::fit);
	if (fmvFill >= 0 && fmvFill <= 3)
	{
		uiscale::fmv_fill = static_cast<uiscale::FillMode>(fmvFill);
		uiscale::setup_fmv_scale();
	}

	sadx_fileMap.scanSoundFolder("system\\sounddata\\bgm\\wma");
	sadx_fileMap.scanSoundFolder("system\\sounddata\\voice_jp\\wma");
	sadx_fileMap.scanSoundFolder("system\\sounddata\\voice_us\\wma");

	// Map of files to replace.
	// This is done with a second map instead of sadx_fileMap directly
	// in order to handle multiple mods.
	unordered_map<string, string> filereplaces;
	vector<std::pair<string, string>> fileswaps;

	vector<std::pair<ModInitFunc, string>> initfuncs;
	vector<std::pair<wstring, wstring>> errors;

	string _mainsavepath, _chaosavepath;

	// It's mod loading time!
	PrintDebug("Loading mods...\n");
	for (unsigned int i = 1; i <= 999; i++)
	{
		char key[8];
		snprintf(key, sizeof(key), "Mod%u", i);
		if (!settings->hasKey(key))
			break;

		const string mod_dirA = "mods\\" + settings->getString(key);
		const wstring mod_dir = L"mods\\" + settings->getWString(key);
		const wstring mod_inifile = mod_dir + L"\\mod.ini";
		FILE *f_mod_ini = _wfopen(mod_inifile.c_str(), L"r");
		if (!f_mod_ini)
		{
			PrintDebug("Could not open file mod.ini in \"mods\\%s\".\n", mod_dirA.c_str());
			errors.emplace_back(mod_dir, L"mod.ini missing");
			continue;
		}
		unique_ptr<IniFile> ini_mod(new IniFile(f_mod_ini));
		fclose(f_mod_ini);

		const IniGroup *const modinfo = ini_mod->getGroup("");
		const string mod_nameA = modinfo->getString("Name");
		const wstring mod_name = modinfo->getWString("Name");
		PrintDebug("%u. %s\n", i, mod_nameA.c_str());

		if (ini_mod->hasGroup("IgnoreFiles"))
		{
			const IniGroup *group = ini_mod->getGroup("IgnoreFiles");
			auto data = group->data();
			for (const auto& iter : *data)
			{
				sadx_fileMap.addIgnoreFile(iter.first, i);
				PrintDebug("Ignored file: %s\n", iter.first.c_str());
			}
		}

		if (ini_mod->hasGroup("ReplaceFiles"))
		{
			const IniGroup *group = ini_mod->getGroup("ReplaceFiles");
			auto data = group->data();
			for (const auto& iter : *data)
			{
				filereplaces[FileMap::normalizePath(iter.first)] =
					FileMap::normalizePath(iter.second);
			}
		}

		if (ini_mod->hasGroup("SwapFiles"))
		{
			const IniGroup *group = ini_mod->getGroup("SwapFiles");
			auto data = group->data();
			for (const auto& iter : *data)
			{
				fileswaps.emplace_back(FileMap::normalizePath(iter.first),
					FileMap::normalizePath(iter.second));
			}
		}

		// Check for SYSTEM replacements.
		// TODO: Convert to WString.
		const string modSysDirA = mod_dirA + "\\system";
		if (DirectoryExists(modSysDirA))
			sadx_fileMap.scanFolder(modSysDirA, i);

		const string modTexDir = mod_dirA + "\\textures";
		if (DirectoryExists(modTexDir))
			sadx_fileMap.scanTextureFolder(modTexDir, i);

		// Check if a custom EXE is required.
		if (modinfo->hasKeyNonEmpty("EXEFile"))
		{
			wstring modexe = modinfo->getWString("EXEFile");
			transform(modexe.begin(), modexe.end(), modexe.begin(), ::towlower);

			// Are we using the correct EXE?
			if (modexe != exefilename)
			{
				wchar_t msg[4096];
				swprintf(msg, LengthOfArray(msg),
					L"Mod \"%s\" should be run from \"%s\", but you are running \"%s\".\n\n"
					L"Continue anyway?", mod_name.c_str(), modexe.c_str(), exefilename.c_str());
				if (MessageBox(nullptr, msg, L"SADX Mod Loader", MB_ICONWARNING | MB_YESNO) == IDNO)
					ExitProcess(1);
			}
		}

		// Check if the mod has a DLL file.
		if (modinfo->hasKeyNonEmpty("DLLFile"))
		{
			// Prepend the mod directory.
			// TODO: SetDllDirectory().
			wstring dll_filename = mod_dir + L'\\' + modinfo->getWString("DLLFile");
			HMODULE module = LoadLibrary(dll_filename.c_str());
			if (module == nullptr)
			{
				DWORD error = GetLastError();
				LPWSTR buffer;
				size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					nullptr, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), (LPWSTR)&buffer, 0, nullptr);
				bool allocated = (size != 0);

				if (!allocated)
				{
					static const wchar_t fmterr[] = L"Unable to format error message.";
					buffer = const_cast<LPWSTR>(fmterr);
					size = LengthOfArray(fmterr)-1;
				}
				wstring message(buffer, size);
				if (allocated)
					LocalFree(buffer);

				const string dll_filenameA = UTF16toMBS(dll_filename, CP_ACP);
				const string messageA = UTF16toMBS(message, CP_ACP);
				PrintDebug("Failed loading mod DLL \"%s\": %s\n", dll_filenameA.c_str(), messageA.c_str());
				errors.emplace_back(mod_name, L"DLL error - " + message);
			}
			else
			{
				const ModInfo *info = (const ModInfo *)GetProcAddress(module, "SADXModInfo");
				if (info)
				{
					if (info->Patches)
					{
						for (int j = 0; j < info->PatchCount; j++)
							WriteData(info->Patches[j].address, info->Patches[j].data, info->Patches[j].datasize);
					}
					if (info->Jumps)
					{
						for (int j = 0; j < info->JumpCount; j++)
							WriteJump(info->Jumps[j].address, info->Jumps[j].data);
					}
					if (info->Calls)
					{
						for (int j = 0; j < info->CallCount; j++)
							WriteCall(info->Calls[j].address, info->Calls[j].data);
					}
					if (info->Pointers)
					{
						for (int j = 0; j < info->PointerCount; j++)
							WriteData((void **)info->Pointers[j].address, info->Pointers[j].data);
					}
					if (info->Init)
					{
						// TODO: Convert to Unicode later. (Will require an API bump.)
						initfuncs.emplace_back(info->Init, mod_dirA);
					}
					const ModInitFunc init = (const ModInitFunc)GetProcAddress(module, "Init");
					if (init)
					{
						initfuncs.emplace_back(init, mod_dirA);
					}
					const PatchList *const patches = (const PatchList *)GetProcAddress(module, "Patches");
					if (patches)
					{
						for (int j = 0; j < patches->Count; j++)
						{
							WriteData(patches->Patches[j].address, patches->Patches[j].data, patches->Patches[j].datasize);
						}
					}
					const PointerList *const jumps = (const PointerList *)GetProcAddress(module, "Jumps");
					if (jumps)
					{
						for (int j = 0; j < jumps->Count; j++)
						{
							WriteJump(jumps->Pointers[j].address, jumps->Pointers[j].data);
						}
					}
					const PointerList *const calls = (const PointerList *)GetProcAddress(module, "Calls");
					if (calls)
					{
						for (int j = 0; j < calls->Count; j++)
						{
							WriteCall(calls->Pointers[j].address, calls->Pointers[j].data);
						}
					}
					const PointerList *const pointers = (const PointerList *)GetProcAddress(module, "Pointers");
					if (pointers)
					{
						for (int j = 0; j < pointers->Count; j++)
						{
							WriteData((void **)pointers->Pointers[j].address, pointers->Pointers[j].data);
						}
					}

					RegisterEvent(modFrameEvents, module, "OnFrame");
					RegisterEvent(modInputEvents, module, "OnInput");
					RegisterEvent(modControlEvents, module, "OnControl");
					RegisterEvent(modExitEvents, module, "OnExit");
					RegisterEvent(modRenderDeviceLost, module, "OnRenderDeviceLost");
					RegisterEvent(modRenderDeviceReset, module, "OnRenderDeviceReset");

					auto customTextureLoad = reinterpret_cast<TextureLoadEvent>(GetProcAddress(module, "OnCustomTextureLoad"));
					if (customTextureLoad != nullptr)
						modCustomTextureLoadEvents.push_back(customTextureLoad);
				}
				else
				{
					const string dll_filenameA = UTF16toMBS(dll_filename, CP_ACP);
					PrintDebug("File \"%s\" is not a valid mod file.\n", dll_filenameA.c_str());
					errors.emplace_back(mod_name, L"Not a valid mod file.");
				}
			}
		}

		// Check if the mod has EXE data replacements.
		if (modinfo->hasKeyNonEmpty("EXEData"))
		{
			wchar_t filename[MAX_PATH];
			swprintf(filename, LengthOfArray(filename), L"%s\\%s",
				mod_dir.c_str(), modinfo->getWString("EXEData").c_str());
			ProcessEXEData(filename, mod_dir);
		}

		// Check if the mod has DLL data replacements.
		for (unsigned int j = 0; j < LengthOfArray(dlldatakeys); j++)
		{
			if (modinfo->hasKeyNonEmpty(dlldatakeys[j]))
			{
				wchar_t filename[MAX_PATH];
				swprintf(filename, LengthOfArray(filename), L"%s\\%s",
					mod_dir.c_str(), modinfo->getWString(dlldatakeys[j]).c_str());
				ProcessDLLData(filename, mod_dir);
			}
		}

		if (modinfo->getBool("RedirectMainSave"))
			_mainsavepath = mod_dirA + "\\SAVEDATA";

		if (modinfo->getBool("RedirectChaoSave"))
			_chaosavepath = mod_dirA + "\\SAVEDATA";

		if (modinfo->hasKeyNonEmpty("WindowTitle"))
			windowtitle = modinfo->getString("WindowTitle");

		if (modinfo->hasKeyNonEmpty("BorderImage"))
			borderimg = mod_dir + L'\\' + modinfo->getWString("BorderImage");
	}

	if (!errors.empty())
	{
		std::wstringstream message;
		message << L"The following mods didn't load correctly:" << std::endl;

		for (auto& i : errors)
		{
			message << std::endl << i.first << ": " << i.second;
		}

		MessageBox(nullptr, message.str().c_str(), L"Mods failed to load", MB_OK | MB_ICONERROR);

		// Clear the errors vector to free memory.
		errors.clear();
		errors.shrink_to_fit();
	}

	// Replace filenames. ("ReplaceFiles")
	for (const auto& filereplace : filereplaces)
	{
		sadx_fileMap.addReplaceFile(filereplace.first, filereplace.second);
	}
	for (const auto& fileswap : fileswaps)
	{
		sadx_fileMap.swapFiles(fileswap.first, fileswap.second);
	}

	for (auto& initfunc : initfuncs)
		initfunc.first(initfunc.second.c_str(), helperFunctions);

	for (const auto& i : StartPositions)
	{
		auto poslist = &i.second;
		StartPosition *newlist = new StartPosition[poslist->size() + 1];
		StartPosition *cur = newlist;
		for (const auto& j : *poslist)
			*cur++ = j.second;
		cur->LevelID = LevelIDs_Invalid;
		switch (i.first)
		{
		default:
		case Characters_Sonic:
			WriteData((StartPosition **)0x41491E, newlist);
			break;
		case Characters_Tails:
			WriteData((StartPosition **)0x414925, newlist);
			break;
		case Characters_Knuckles:
			WriteData((StartPosition **)0x41492C, newlist);
			break;
		case Characters_Amy:
			WriteData((StartPosition **)0x41493A, newlist);
			break;
		case Characters_Gamma:
			WriteData((StartPosition **)0x414941, newlist);
			break;
		case Characters_Big:
			WriteData((StartPosition **)0x414933, newlist);
			break;
		}
	}

	for (const auto& i : FieldStartPositions)
	{
		auto poslist = &i.second;
		FieldStartPosition *newlist = new FieldStartPosition[poslist->size() + 1];
		FieldStartPosition *cur = newlist;
		for (const auto& j : *poslist)
			*cur++ = j.second;
		cur->LevelID = LevelIDs_Invalid;
		StartPosList_FieldReturn[i.first] = newlist;
	}

	if (PathsInitialized)
	{
		PathDataPtr *newlist = new PathDataPtr[Paths.size() + 1];
		PathDataPtr *cur = newlist;
		for (const auto& path : Paths)
			*cur++ = path.second;
		cur->LevelAct = 0xFFFF;
		WriteData((PathDataPtr **)0x49C1A1, newlist);
		WriteData((PathDataPtr **)0x49C1AF, newlist);
	}

	for (const auto& pvm : CharacterPVMs)
	{
		const vector<PVMEntry> *pvmlist = &pvm.second;
		auto size = pvmlist->size();
		PVMEntry *newlist = new PVMEntry[size + 1];
		memcpy(newlist, pvmlist->data(), sizeof(PVMEntry) * size);
		newlist[size].TexList = nullptr;
		CharacterPVMEntries[pvm.first] = newlist;
	}

	if (CommonObjectPVMsInitialized)
	{
		auto size = CommonObjectPVMs.size();
		PVMEntry *newlist = new PVMEntry[size + 1];
		//PVMEntry *cur = newlist;
		memcpy(newlist, CommonObjectPVMs.data(), sizeof(PVMEntry) * size);
		newlist[size].TexList = nullptr;
		TexLists_ObjRegular[0] = newlist;
		TexLists_ObjRegular[1] = newlist;
	}

	for (const auto& level : _TrialLevels)
	{
		const vector<TrialLevelListEntry> *levellist = &level.second;
		auto size = levellist->size();
		TrialLevelListEntry *newlist = new TrialLevelListEntry[size];
		memcpy(newlist, levellist->data(), sizeof(TrialLevelListEntry) * size);
		TrialLevels[level.first].Levels = newlist;
		TrialLevels[level.first].Count = size;
	}

	for (const auto& subgame : _TrialSubgames)
	{
		const vector<TrialLevelListEntry> *levellist = &subgame.second;
		auto size = levellist->size();
		TrialLevelListEntry *newlist = new TrialLevelListEntry[size];
		memcpy(newlist, levellist->data(), sizeof(TrialLevelListEntry) * size);
		TrialSubgames[subgame.first].Levels = newlist;
		TrialSubgames[subgame.first].Count = size;
	}

	if (!_mainsavepath.empty())
	{
		char *buf = new char[_mainsavepath.size() + 1];
		strncpy(buf, _mainsavepath.c_str(), _mainsavepath.size() + 1);
		mainsavepath = buf;
		string tmp = "./" + _mainsavepath + "/%s";
		buf = new char[tmp.size() + 1];
		strncpy(buf, tmp.c_str(), tmp.size() + 1);
		WriteData((char **)0x421E4E, buf);
		WriteData((char **)0x421E6A, buf);
		WriteData((char **)0x421F07, buf);
		WriteData((char **)0x5050E5, buf);
		WriteData((char **)0x5051ED, buf);
		tmp = "./" + _mainsavepath + "/SonicDX??.snc";
		buf = new char[tmp.size() + 1];
		strncpy(buf, tmp.c_str(), tmp.size() + 1);
		WriteData((char **)0x5050AB, buf);
		WriteJump(WriteSaveFile, WriteSaveFile_r);
	}

	if (!_chaosavepath.empty())
	{
		char *buf = new char[_chaosavepath.size() + 1];
		strncpy(buf, _chaosavepath.c_str(), _chaosavepath.size() + 1);
		chaosavepath = buf;
		string tmp = "./" + _chaosavepath + "/SONICADVENTURE_DX_CHAOGARDEN.snc";
		buf = new char[tmp.size() + 1];
		strncpy(buf, tmp.c_str(), tmp.size() + 1);
		WriteData((char **)0x7163EF, buf);
		WriteData((char **)0x71AA6F, buf);
		WriteData((char **)0x71ACDB, buf);
		WriteData((char **)0x71ADC5, buf);
	}

	if (!windowtitle.empty())
	{
		char *buf = new char[windowtitle.size() + 1];
		strncpy(buf, windowtitle.c_str(), windowtitle.size() + 1);
		*(char**)0x892944 = buf;
	}

	PrintDebug("Finished loading mods\n");

	// Check for patches.
	ifstream patches_str("mods\\Patches.dat", ifstream::binary);
	if (patches_str.is_open())
	{
		CodeParser patchParser;
		static const char codemagic[6] = { 'c', 'o', 'd', 'e', 'v', '5' };
		char buf[sizeof(codemagic)];
		patches_str.read(buf, sizeof(buf));
		if (!memcmp(buf, codemagic, sizeof(codemagic)))
		{
			int codecount_header;
			patches_str.read((char*)&codecount_header, sizeof(codecount_header));
			PrintDebug("Loading %d patches...\n", codecount_header);
			patches_str.seekg(0);
			int codecount = patchParser.readCodes(patches_str);
			if (codecount >= 0)
			{
				PrintDebug("Loaded %d patches.\n", codecount);
				patchParser.processCodeList();
			}
			else
			{
				PrintDebug("ERROR loading patches: ");
				switch (codecount)
				{
				case -EINVAL:
					PrintDebug("Patch file is not in the correct format.\n");
					break;
				default:
					PrintDebug("%s\n", strerror(-codecount));
					break;
				}
			}
		}
		else
		{
			PrintDebug("Patch file is not in the correct format.\n");
		}
		patches_str.close();
	}

	// Check for codes.
	ifstream codes_str("mods\\Codes.dat", ifstream::binary);
	if (codes_str.is_open())
	{
		static const char codemagic[6] = { 'c', 'o', 'd', 'e', 'v', '5' };
		char buf[sizeof(codemagic)];
		codes_str.read(buf, sizeof(buf));
		if (!memcmp(buf, codemagic, sizeof(codemagic)))
		{
			int codecount_header;
			codes_str.read((char*)&codecount_header, sizeof(codecount_header));
			PrintDebug("Loading %d codes...\n", codecount_header);
			codes_str.seekg(0);
			int codecount = codeParser.readCodes(codes_str);
			if (codecount >= 0)
			{
				PrintDebug("Loaded %d codes.\n", codecount);
				codeParser.processCodeList();
			}
			else
			{
				PrintDebug("ERROR loading codes: ");
				switch (codecount)
				{
				case -EINVAL:
					PrintDebug("Code file is not in the correct format.\n");
					break;
				default:
					PrintDebug("%s\n", strerror(-codecount));
					break;
				}
			}
		}
		else
		{
			PrintDebug("Code file is not in the correct format.\n");
		}
		codes_str.close();
	}

	// Sets up code/event handling
	WriteJump((void*)0x00426063, (void*)ProcessCodes);
	WriteJump((void*)0x0040FDB3, (void*)OnInput);			// End of first chunk
	WriteJump((void*)0x0042F1C5, (void*)OnInput_MidJump);	// Cutscene stuff - Untested. Couldn't trigger ingame.
	WriteJump((void*)0x0042F1E9, (void*)OnInput);			// Cutscene stuff
	WriteJump((void*)0x0040FF00, (void*)OnControl);
}

DataPointer(HMODULE, chrmodelshandle, 0x3AB9170);
static void __cdecl LoadChrmodels(void)
{
	chrmodelshandle = LoadLibrary(L".\\system\\CHRMODELS_orig.dll");
	if (!chrmodelshandle)
	{
		MessageBox(nullptr, L"CHRMODELS_orig.dll could not be loaded!\n\n"
			L"SADX will now proceed to abruptly exit.",
			L"SADX Mod Loader", MB_ICONERROR);
		ExitProcess(1);
	}
	SetChrmodelsDLLHandle(chrmodelshandle);
	WriteCall((void *)0x402513, (void *)InitMods);
}

/**
 * DLL entry point.
 * @param hinstDll DLL instance.
 * @param fdwReason Reason for calling DllMain.
 * @param lpvReserved Reserved.
 */
BOOL APIENTRY DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpvReserved)
{
	// US version check.
	static const void* const verchk_addr = (void*)0x789E50;
	static const uint8_t verchk_data[] = { 0x83, 0xEC, 0x28, 0x57, 0x33 };

	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		g_hinstDll = hinstDll;
		HookCreateFileA();

		// Make sure this is the correct version of SADX.
		if (memcmp(verchk_data, verchk_addr, sizeof(verchk_data)) != 0)
		{
			ShowNon2004USError();
			ExitProcess(1);
		}

		WriteData((unsigned char*)0x401AE1, (unsigned char)0x90);
		WriteCall((void *)0x401AE2, (void *)LoadChrmodels);

#if !defined(_MSC_VER) || defined(_DLL)
		// Disable thread library calls, since we don't
		// care about thread attachments.
		// NOTE: On MSVC, don't do this if using the static CRT.
		// Reference: https://msdn.microsoft.com/en-us/library/windows/desktop/ms682579(v=vs.85).aspx
		DisableThreadLibraryCalls(hinstDll);
#endif /* !defined(_MSC_VER) || defined(_DLL) */
		break;

	case DLL_PROCESS_DETACH:
		// Make sure the log file is closed.
		if (dbgFile)
		{
			fclose(dbgFile);
			dbgFile = nullptr;
		}
		break;

	default:
		break;
	}

	return TRUE;
}
