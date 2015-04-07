// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <cwctype>

#include <deque>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
using std::deque;
using std::ios_base;
using std::ifstream;
using std::string;
using std::wstring;
using std::unique_ptr;
using std::unordered_map;
using std::vector;

// Win32 headers.
#include <dbghelp.h>
#include <shlwapi.h>
#include <gdiplus.h>

#include "config.SADXModLoader.h"
#include "git.h"

#include "IniFile.hpp"
#include "CodeParser.hpp"
#include "FileMap.hpp"
#include "MediaFns.hpp"
#include "TextConv.hpp"
#include "SADXModLoader.h"
#include "..\libmodutils\LandTableInfo.h"
#include "..\libmodutils\ModelInfo.h"
#include "..\libmodutils\AnimationFile.h"

static HINSTANCE myhandle;
static HMODULE chrmodelshandle;
static FARPROC __stdcall MyGetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	return GetProcAddress((hModule == myhandle ? chrmodelshandle : hModule), lpProcName);
}

/**
 * Replace slash characters with backslashes.
 * @param c Character.
 * @return If c == '/', '\\'; otherwise, c.
 */
static inline int backslashes(int c)
{
	if (c == '/')
		return '\\';
	else
		return c;
}

// File replacement map.
// NOTE: Do NOT mark this as static.
// MediaFns.cpp needs to access the FileMap.
FileMap sadx_fileMap;

/**
 * CreateFileA() wrapper using _ReplaceFile().
 * @param lpFileName
 * @param dwDesiredAccess
 * @param dwShareMode
 * @param lpSecurityAttibutes
 * @param dwCreationDisposition
 * @param dwFlagsAndAttributes
 * @param hTemplateFile
 * @return
 */
static HANDLE __stdcall MyCreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	return CreateFileA(sadx_fileMap.replaceFile(lpFileName), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

static int __cdecl PlayVoiceFile_r(LPCSTR filename)
{
	filename = sadx_fileMap.replaceFile(filename);
	return PlayVoiceFile(filename);
}

/**
 * C wrapper to call sadx_fileMap.replaceFile() from asm.
 * @param lpFileName Filename.
 * @return Replaced filename, or original filename if not replaced by a mod.
 */ 
static const char *_ReplaceFile(const char *lpFileName)
{
	return sadx_fileMap.replaceFile(lpFileName);
}

static __declspec(naked) int PlayVideoFile_r()
{
	__asm
	{
		mov eax, [esp+4]
		push esi
		push eax
		call _ReplaceFile
		add esp, 4
		pop esi
		mov [esp+4], eax
		jmp PlayVideoFilePtr
	}
}

static void HookTheAPI()
{
	ULONG ulSize = 0;
	PROC pNewFunction = NULL;
	PROC pActualFunction = NULL;

	PSTR pszModName = NULL;

	HMODULE hModule = GetModuleHandle(NULL);
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc = NULL;

	pNewFunction = (PROC)MyGetProcAddress;
	PROC pNewCreateFile = (PROC)MyCreateFileA;
	pActualFunction = GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "GetProcAddress");
	PROC pActualCreateFile = GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "CreateFileA");

	pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR) ImageDirectoryEntryToData(
		hModule, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize);

	if(NULL != pImportDesc)
	{
		for (; pImportDesc->Name; pImportDesc++)
		{
			// get the module name
			pszModName = (PSTR) ((PBYTE) hModule + pImportDesc->Name);

			if(NULL != pszModName)
			{
				// check if the module is kernel32.dll
				if (lstrcmpiA(pszModName, "Kernel32.dll") == 0)
				{
					// get the module
					PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA) ((PBYTE) hModule + pImportDesc->FirstThunk);

					for (; pThunk->u1.Function; pThunk++) 
					{
						PROC* ppfn = (PROC*) &pThunk->u1.Function;
						if(*ppfn == pActualFunction)
						{
							DWORD dwOldProtect = 0;
							VirtualProtect(ppfn, sizeof(pNewFunction), PAGE_WRITECOPY,&dwOldProtect);
							WriteProcessMemory(GetCurrentProcess(), ppfn, &pNewFunction, sizeof(pNewFunction), NULL);
							VirtualProtect(ppfn, sizeof(pNewFunction), dwOldProtect,&dwOldProtect);
						} // Function that we are looking for
						else if (*ppfn == pActualCreateFile)
						{
							DWORD dwOldProtect = 0;
							VirtualProtect(ppfn, sizeof(pNewCreateFile), PAGE_WRITECOPY,&dwOldProtect);
							WriteProcessMemory(GetCurrentProcess(), ppfn, &pNewCreateFile, sizeof(pNewCreateFile), NULL);
							VirtualProtect(ppfn, sizeof(pNewCreateFile), dwOldProtect,&dwOldProtect);
						}
					}
				} // Compare module name
			} // Valid module name
		}
	}
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

static vector<ModFrameFunc> modFrameFuncs;

static void __cdecl ProcessCodes()
{
	codeParser.processCodeList();
	for (unsigned int i = 0; i < modFrameFuncs.size(); i++)
		modFrameFuncs[i]();
	const int numrows = (VerticalResolution / 12);
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
			SetDebugTextColor((int)c);
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
 * @param args Arguments.
 * @return Return value from vsnprintf().
 */
static int __cdecl SADXDebugOutput(const char *Format, ...)
{
	va_list ap;
	va_start(ap, Format);
	int result = vsnprintf(NULL, 0, Format, ap) + 1;
	va_end(ap);
	char *buf = new char[result];
	va_start(ap, Format);
	result = vsnprintf(buf, result, Format, ap);
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
		message msg = { buf };
		if (msg.text[msg.text.length() - 1] == '\n')
			msg.text = msg.text.substr(0, msg.text.length() - 1);
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

struct windowsize { int x; int y; int width; int height; };

struct windowdata { int x; int y; int width; int height; DWORD style; DWORD extendedstyle; };

windowdata windowsizes[] = {
	{ CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, WS_CAPTION | WS_SYSMENU, 0 }, // windowed
	{ 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, WS_POPUP, WS_EX_APPWINDOW } // fullscreen
};

enum windowmodes { windowed, fullscreen };

windowsize innersizes[2];

ACCEL accelerators[] = {
	{ FALT | FVIRTKEY, VK_RETURN, 0 }
};

WNDCLASS outerWindowClass;
HWND outerWindow;
windowmodes windowmode;
HACCEL accelTbl;

DataPointer(int, dword_3D08534, 0x3D08534);
static void __cdecl sub_789BD0()
{
	MSG v0; // [sp+4h] [bp-1Ch]@1

	if ( PeekMessageA(&v0, 0, 0, 0, 1u) )
	{
		do
		{
			if (!TranslateAccelerator(outerWindow, accelTbl, &v0))
			{
				TranslateMessage(&v0);
				DispatchMessageA(&v0);
			}
		}
		while ( PeekMessageA(&v0, 0, 0, 0, 1u) );
		dword_3D08534 = v0.wParam;
	}
	else
	{
		dword_3D08534 = v0.wParam;
	}
}

DataPointer(HINSTANCE, hInstance, 0x3D0FD34);
void CreateOuterWindow()
{
	windowdata *data = &windowsizes[windowmode];

	outerWindow = CreateWindowEx(data->extendedstyle,
		L"WrapperWindow",
		L"SonicAdventureDXPC",
		data->style,
		data->x, data->y, data->width, data->height,
		NULL, NULL, hInstance, NULL);
}

static Gdiplus::Bitmap *bgimg;
DataPointer(HWND, hWnd, 0x3D0FD30);
StdcallFunctionPointer(LRESULT, sub_401900, (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam), 0x401900);
static LRESULT CALLBACK WrapperWndProc(HWND wrapper, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CLOSE:
		// we also need to let SADX do cleanup
		SendMessage(hWnd, WM_CLOSE, wParam, lParam);
		// what we do here is up to you: we can check if SADX decides to close, and if so, destroy ourselves, or something like that
		return 0;
	case WM_ERASEBKGND:
		if (bgimg != nullptr)
		{
			Gdiplus::Graphics gfx((HDC)wParam);
			gfx.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
			gfx.DrawImage(bgimg, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
			return 0;
		}
	case WM_COMMAND:
		if (LOWORD(wParam) == 0)
		{
			if (windowmode == windowed)
			{
				RECT rect;
				GetWindowRect(wrapper, &rect);
				windowsizes[windowed].x = rect.left;
				windowsizes[windowed].y = rect.top;
			}
			windowmode = windowmode == windowed ? fullscreen : windowed;
			HWND oldwnd = outerWindow;
			CreateOuterWindow();
			SetParent(hWnd, outerWindow);
			windowsize *size = &innersizes[windowmode];
			SetWindowPos(hWnd, NULL, size->x, size->y, size->width, size->height, 0);
			DestroyWindow(oldwnd);
			SetFocus(hWnd);
			ShowWindow(outerWindow, SW_SHOW);
			UpdateWindow(outerWindow);
			SetForegroundWindow(outerWindow);
			return 0;
		}
		break;
	case WM_ACTIVATEAPP:
		sub_401900(hWnd, uMsg, wParam, lParam);
		if (windowmode == windowed)
			while (ShowCursor(TRUE) < 0);
		else
			while (ShowCursor(FALSE) > 0);
	default:
		// alternatively we can return SendMe
		return DefWindowProc(wrapper, uMsg, wParam, lParam);
	}
	/* unreachable */ return 0;
}

static bool windowedfullscreen = false;

uint8_t wndpatch[] = { 0xA1, 0x30, 0xFD, 0xD0, 0x03, 0xEB, 0x08 };

DataPointer(int, Windowed, 0x38A5DC4);
static void CreateSADXWindow(HINSTANCE _hInstance, int nCmdShow)
{
	WNDCLASSA v8; // [sp+4h] [bp-28h]@1

	v8.style = 0;
	v8.lpfnWndProc = (WNDPROC)0x789DE0;
	v8.cbClsExtra = 0;
	v8.cbWndExtra = 0;
	v8.hInstance = _hInstance;
	v8.hIcon = LoadIconA(_hInstance, MAKEINTRESOURCEA(101));
	v8.hCursor = LoadCursorA(0, MAKEINTRESOURCEA(0x7F00));
	v8.hbrBackground = (HBRUSH)GetStockObject(0);
	v8.lpszMenuName = 0;
	v8.lpszClassName = GetWindowClassName();
	if (!RegisterClassA(&v8))
		return;
	RECT wndsz = { 0, 0, HorizontalResolution, VerticalResolution };
	AdjustWindowRectEx(&wndsz, WS_CAPTION | WS_SYSMENU, false, 0);
	if (windowedfullscreen)
	{
		int scrnw = GetSystemMetrics(SM_CXSCREEN);
		int scrnh = GetSystemMetrics(SM_CYSCREEN);
		windowsizes[windowed].width = wndsz.right - wndsz.left;
		windowsizes[windowed].height = wndsz.bottom - wndsz.top;
		if (!Windowed)
		{
			windowsizes[windowed].x = (scrnw - windowsizes[windowed].width) / 2;
			windowsizes[windowed].y = (scrnh - windowsizes[windowed].height) / 2;
		}
		windowsizes[fullscreen].width = scrnw;
		windowsizes[fullscreen].height = scrnh;
		innersizes[windowed].x = 0;
		innersizes[windowed].y = 0;
		innersizes[windowed].width = HorizontalResolution;
		innersizes[windowed].height = VerticalResolution;
		float num = min((float)scrnw / (float)HorizontalResolution, (float)scrnh / (float)VerticalResolution);
		innersizes[fullscreen].width = (int)((float)HorizontalResolution * num);
		innersizes[fullscreen].height = (int)((float)VerticalResolution * num);
		innersizes[fullscreen].x = (scrnw - innersizes[fullscreen].width) / 2;
		innersizes[fullscreen].y = (scrnh - innersizes[fullscreen].height) / 2;

		windowmode = Windowed ? windowed : fullscreen;

		if (PathFileExists(L"mods\\Border.png"))
		{
			Gdiplus::GdiplusStartupInput gdiplusStartupInput;
			ULONG_PTR gdiplusToken;
			Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
			bgimg = Gdiplus::Bitmap::FromFile(L"mods\\Border.png");
		}
		WNDCLASS w;
		ZeroMemory(&w, sizeof (WNDCLASS));
		w.lpszClassName = TEXT("WrapperWindow");
		w.lpfnWndProc = WrapperWndProc;
		w.hInstance = _hInstance;
		w.hIcon = LoadIconA(_hInstance, MAKEINTRESOURCEA(101));
		w.hCursor = LoadCursorA(0, MAKEINTRESOURCEA(0x7F00));
		w.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		if (RegisterClass(&w) == 0)
			return;

		CreateOuterWindow();

		if (outerWindow == NULL)
			return;

		accelTbl = CreateAcceleratorTable(arrayptrandlength(accelerators));

		windowsize *size = &innersizes[windowmode];

		hWnd = CreateWindowExA(0, GetWindowClassName(), GetWindowClassName(), WS_CHILD | WS_VISIBLE,
			size->x, size->y, size->width, size->height, outerWindow, NULL, hInstance, 0);
		SetFocus(hWnd);
		ShowWindow(outerWindow, nCmdShow);
		UpdateWindow(outerWindow);
		SetForegroundWindow(outerWindow);
		Windowed = 1;
		WriteJump((void *)0x789BD0, (void *)sub_789BD0);
		WriteData((void *)0x402C61, wndpatch);
	}
	else
	{
		signed int v2; // eax@3
		DWORD v3; // esi@3
		if ( Windowed )
		{
			v3 = 0;
			v2 = WS_CAPTION | WS_SYSMENU;
		}
		else
		{
			v3 = WS_EX_TOPMOST | WS_EX_TOOLWINDOW;
			v2 = WS_CAPTION;
		}
		hWnd = CreateWindowExA(v3, GetWindowClassName(), GetWindowClassName(), v2, CW_USEDEFAULT, CW_USEDEFAULT, wndsz.right - wndsz.left, wndsz.bottom - wndsz.top, 0, NULL, hInstance, 0);
		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);
		SetForegroundWindow(hWnd);
	}
}

static __declspec(naked) void sub_789E50_r()
{
	__asm
	{
		mov ebx, [esp+4]
		push ebx
			push eax
			call CreateSADXWindow
			add esp, 8
			retn
	}
}

static unordered_map<unsigned char, unordered_map<int, StartPosition> > StartPositions;
static void RegisterStartPosition(unsigned char character, const StartPosition &position)
{
	auto iter = StartPositions.find(character);
	unordered_map<int, StartPosition> *newlist;
	if (iter == StartPositions.end())
	{
		const StartPosition *origlist;
		switch (character)
		{
		case Characters_Sonic:
			origlist = (StartPosition *)0x90A5C8;
			break;
		case Characters_Tails:
			origlist = (StartPosition *)0x90AB68;
			break;
		case Characters_Knuckles:
			origlist = (StartPosition *)0x90AEA0;
			break;
		case Characters_Amy:
			origlist = (StartPosition *)0x90B470;
			break;
		case Characters_Gamma:
			origlist = (StartPosition *)0x90B7A8;
			break;
		case Characters_Big:
			origlist = (StartPosition *)0x90B1B0;
			break;
		default:
			return;
		}
		StartPositions[character] = unordered_map<int, StartPosition>();
		newlist = &StartPositions[character];
		while (origlist->LevelID != LevelIDs_Invalid)
		{
			(*newlist)[levelact(origlist->LevelID, origlist->ActID)] = *origlist;
			origlist++;
		}
	}
	else
	{
		newlist = &iter->second;
	}
	(*newlist)[levelact(position.LevelID, position.ActID)] = position;
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
	StartPositions[character] = unordered_map<int, StartPosition>();
}

static unordered_map<unsigned char, unordered_map<int, FieldStartPosition> > FieldStartPositions;
static void RegisterFieldStartPosition(unsigned char character, const FieldStartPosition &position)
{
	if (character >= Characters_MetalSonic) return;
	auto iter = FieldStartPositions.find(character);
	unordered_map<int, FieldStartPosition> *newlist;
	if (iter == FieldStartPositions.end())
	{
		const FieldStartPosition *origlist = ((FieldStartPosition **)0x90BEFC)[character];
		FieldStartPositions[character] = unordered_map<int, FieldStartPosition>();
		newlist = &FieldStartPositions[character];
		while (origlist->LevelID != LevelIDs_Invalid)
		{
			(*newlist)[levelact(origlist->LevelID, origlist->FieldID)] = *origlist;
			origlist++;
		}
	}
	else
	{
		newlist = &iter->second;
	}
	(*newlist)[levelact(position.LevelID, position.FieldID)] = position;
}

static void ClearFieldStartPositionList(unsigned char character)
{
	if (character >= Characters_MetalSonic) return;
	FieldStartPositions[character] = unordered_map<int, FieldStartPosition>();
}

static unordered_map<int, PathDataPtr> Paths;
static bool PathsInitialized;
static void RegisterPathList(const PathDataPtr &paths)
{
	if (!PathsInitialized)
	{
		const PathDataPtr *oldlist = (PathDataPtr *)0x91A858;
		while (oldlist->LevelAct != 0xFFFF)
		{
			Paths[oldlist->LevelAct] = *oldlist;
			oldlist++;
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
	vector<PVMEntry> *newlist;
	if (iter == CharacterPVMs.end())
	{
		const PVMEntry *origlist = ((PVMEntry **)0x90ED54)[character];
		CharacterPVMs[character] = vector<PVMEntry>();
		newlist = &CharacterPVMs[character];
		while (origlist->TexList)
			newlist->push_back(*origlist++);
	}
	else
		newlist = &iter->second;
	newlist->push_back(pvm);
}

static void ClearCharacterPVMList(unsigned char character)
{
	if (character > Characters_MetalSonic) return;
	CharacterPVMs[character] = vector<PVMEntry>();
}

static vector<PVMEntry> CommonObjectPVMs;
static bool CommonObjectPVMsInitialized;
static void RegisterCommonObjectPVM(const PVMEntry &pvm)
{
	if (!CommonObjectPVMsInitialized)
	{
		const PVMEntry *oldlist = (PVMEntry *)0x90EC18;
		while (oldlist->TexList)
			CommonObjectPVMs.push_back(*oldlist++);
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
	vector<TrialLevelListEntry> *newlist;
	if (iter == _TrialLevels.end())
	{
		const TrialLevelList *origlist = &TrialLevels[character];
		_TrialLevels[character] = vector<TrialLevelListEntry>();
		newlist = &_TrialLevels[character];
		newlist->resize(origlist->Count);
		memcpy(newlist->data(), origlist->Levels, sizeof(TrialLevelListEntry) * origlist->Count);
	}
	else
		newlist = &iter->second;
	newlist->push_back(level);
}

static void ClearTrialLevelList(unsigned char character)
{
	character = gettrialcharacter(character);
	if (character == 0xFF) return;
	_TrialLevels[character] = vector<TrialLevelListEntry>();
}

static unordered_map<unsigned char, vector<TrialLevelListEntry> > _TrialSubgames;
static void RegisterTrialSubgame(unsigned char character, const TrialLevelListEntry &level)
{
	character = gettrialcharacter(character);
	if (character == 0xFF) return;
	auto iter = _TrialSubgames.find(character);
	vector<TrialLevelListEntry> *newlist;
	if (iter == _TrialSubgames.end())
	{
		const TrialLevelList *origlist = &TrialSubgames[character];
		_TrialSubgames[character] = vector<TrialLevelListEntry>();
		newlist = &_TrialSubgames[character];
		newlist->resize(origlist->Count);
		memcpy(newlist->data(), origlist->Levels, sizeof(TrialLevelListEntry) * origlist->Count);
	}
	else
		newlist = &iter->second;
	newlist->push_back(level);
}

static void ClearTrialSubgameList(unsigned char character)
{
	character = gettrialcharacter(character);
	if (character == 0xFF) return;
	_TrialSubgames[character] = vector<TrialLevelListEntry>();
}

static const HelperFunctions helperFunctions =
{
	ModLoaderVer,
	RegisterStartPosition,
	ClearStartPositionList,
	RegisterFieldStartPosition,
	ClearFieldStartPositionList,
	RegisterPathList,
	ClearPathListList,
	RegisterCharacterPVM,
	ClearCharacterPVMList,
	RegisterCommonObjectPVM,
	ClearCommonObjectPVMList,
	RegisterTrialLevel,
	ClearTrialLevelList,
	RegisterTrialSubgame,
	ClearTrialSubgameList
};

static vector<string> &split(const string &s, char delim, vector<string> &elems) {
    std::stringstream ss(s);
    string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


static vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

static string trim(const string &s)
{
	auto st = s.find_first_not_of(' ');
	if (st == string::npos)
		st = 0;
	auto ed = s.find_last_not_of(' ');
	if (ed == string::npos)
		ed = s.size() - 1;
	return s.substr(st, (ed + 1) - st);
}

template<typename T>
static inline T *arrcpy(T *dst, const T *src, size_t cnt)
{
	return (T *)memcpy(dst, src, cnt * sizeof(T));
}

template<typename T>
static inline void clrmem(T *mem)
{
	ZeroMemory(mem, sizeof(T));
}

static const unordered_map<string, uint8_t> levelidsnamemap = {
	{ "hedgehoghammer", LevelIDs_HedgehogHammer },
	{ "emeraldcoast", LevelIDs_EmeraldCoast },
	{ "windyvalley", LevelIDs_WindyValley },
	{ "twinklepark", LevelIDs_TwinklePark },
	{ "speedhighway", LevelIDs_SpeedHighway },
	{ "redmountain", LevelIDs_RedMountain },
	{ "skydeck", LevelIDs_SkyDeck },
	{ "lostworld", LevelIDs_LostWorld },
	{ "icecap", LevelIDs_IceCap },
	{ "casinopolis", LevelIDs_Casinopolis },
	{ "finalegg", LevelIDs_FinalEgg },
	{ "hotshelter", LevelIDs_HotShelter },
	{ "chaos0", LevelIDs_Chaos0 },
	{ "chaos2", LevelIDs_Chaos2 },
	{ "chaos4", LevelIDs_Chaos4 },
	{ "chaos6", LevelIDs_Chaos6 },
	{ "perfectchaos", LevelIDs_PerfectChaos },
	{ "egghornet", LevelIDs_EggHornet },
	{ "eggwalker", LevelIDs_EggWalker },
	{ "eggviper", LevelIDs_EggViper },
	{ "zero", LevelIDs_Zero },
	{ "e101", LevelIDs_E101 },
	{ "e101r", LevelIDs_E101R },
	{ "stationsquare", LevelIDs_StationSquare },
	{ "eggcarrieroutside", LevelIDs_EggCarrierOutside },
	{ "eggcarrierinside", LevelIDs_EggCarrierInside },
	{ "mysticruins", LevelIDs_MysticRuins },
	{ "past", LevelIDs_Past },
	{ "twinklecircuit", LevelIDs_TwinkleCircuit },
	{ "skychase1", LevelIDs_SkyChase1 },
	{ "skychase2", LevelIDs_SkyChase2 },
	{ "sandhill", LevelIDs_SandHill },
	{ "ssgarden", LevelIDs_SSGarden },
	{ "ecgarden", LevelIDs_ECGarden },
	{ "mrgarden", LevelIDs_MRGarden },
	{ "chaorace", LevelIDs_ChaoRace },
	{ "invalid", LevelIDs_Invalid }
};

static uint8_t ParseLevelID(const string &str)
{
	string str2 = trim(str);
	transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
	auto lv = levelidsnamemap.find(str2);
	if (lv != levelidsnamemap.end())
		return lv->second;
	else
		return (uint8_t)strtol(str.c_str(), nullptr, 10);
}

static uint16_t ParseLevelAndActID(const string &str)
{
	if (str.size() == 4)
	{
		const char *cstr = str.c_str();
		char buf[3];
		buf[2] = 0;
		memcpy(buf, cstr, 2);
		uint16_t result = (uint16_t)(strtol(buf, nullptr, 10) << 8);
		memcpy(buf, cstr + 2, 2);
		return result | (uint16_t)strtol(buf, nullptr, 10);
	}
	else
	{
		vector<string> strs = split(str, ' ');
		return (uint16_t)((ParseLevelID(strs[0]) << 8) | strtol(strs[1].c_str(), nullptr, 10));
	}
}

static const unordered_map<string, uint8_t> charflagsnamemap = {
	{ "sonic", CharacterFlags_Sonic },
	{ "eggman", CharacterFlags_Eggman },
	{ "tails", CharacterFlags_Tails },
	{ "knuckles", CharacterFlags_Knuckles },
	{ "tikal", CharacterFlags_Tikal },
	{ "amy", CharacterFlags_Amy },
	{ "gamma", CharacterFlags_Gamma },
	{ "big", CharacterFlags_Big }
};

static uint8_t ParseCharacterFlags(const string &str)
{
	vector<string> strflags = split(str, ',');
	uint8_t flag = 0;
	for (auto iter = strflags.cbegin(); iter != strflags.cend(); iter++)
	{
		string str = trim(*iter);
		transform(str.begin(), str.end(), str.begin(), ::tolower);
		auto ch = charflagsnamemap.find(str);
		if (ch != charflagsnamemap.end())
			flag |= ch->second;
	}
	return flag;
}

static const unordered_map<string, uint8_t> languagesnamemap = {
	{ "japanese", Languages_Japanese },
	{ "english", Languages_English },
	{ "french", Languages_French },
	{ "spanish", Languages_Spanish },
	{ "german", Languages_German }
};

static uint8_t ParseLanguage(const string &str)
{
	string str2 = trim(str);
	transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
	auto lv = languagesnamemap.find(str2);
	if (lv != languagesnamemap.end())
		return lv->second;
	return Languages_Japanese;
}

static string DecodeUTF8(const string &str, int language)
{
	if (language <= Languages_English)
		return UTF8toSJIS(str);
	else
		return UTF8to1252(str);
}

static string UnescapeNewlines(const string &str)
{
	string result;
	result.reserve(str.size());
	for (unsigned int c = 0; c < str.size(); c++)
		switch (str[c])
		{
			case '\\': // escape character
				if (c + 1 == str.size())
				{
					result.push_back(str[c]);
					continue;
				}
				switch (str[++c])
				{
				case 'n': // line feed
					result.push_back('\n');
					break;
				case 'r': // carriage return
					result.push_back('\r');
					break;
				default: // literal character
					result.push_back(str[c]);
					break;
				}
				break;
			default:
				result.push_back(str[c]);
				break;
		}
	return result;
}

static void ParseVertex(const string &str, Vertex &vert)
{
	vector<string> vals = split(str, ',');
	vert.x = (float)strtod(vals[0].c_str(), nullptr);
	vert.y = (float)strtod(vals[1].c_str(), nullptr);
	vert.z = (float)strtod(vals[2].c_str(), nullptr);
}

static void ParseRotation(const string str, Rotation &rot)
{
	vector<string> vals = split(str, ',');
	rot.x = (int)strtol(vals[0].c_str(), nullptr, 16);
	rot.y = (int)strtol(vals[1].c_str(), nullptr, 16);
	rot.z = (int)strtol(vals[2].c_str(), nullptr, 16);
}

template<typename T>
static void ProcessPointerList(const string &list, T *item)
{
	vector<string> ptrs = split(list, ',');
	for (unsigned int i = 0; i < ptrs.size(); i++)
		*(T **)(strtol(ptrs[i].c_str(), nullptr, 16) + 0x400000) = item;
}

static void ProcessLandTableINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	LandTable *landtable = (new LandTableInfo(mod_dir + L'\\' + group->getWString("filename")))->getlandtable();
	ProcessPointerList(group->getString("pointer"), landtable);
}

static unordered_map<string, NJS_OBJECT *> inimodels;
static void ProcessModelINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	ModelInfo *mdlinf = new ModelInfo(mod_dir + L'\\' + group->getWString("filename"));
	NJS_OBJECT *model = mdlinf->getmodel();
	inimodels[mdlinf->getlabel(model)] = model;
	ProcessPointerList(group->getString("pointer"), model);
}

static void ProcessActionINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	NJS_ACTION *action = new NJS_ACTION;
	action->motion = (new AnimationFile(mod_dir + L'\\' + group->getWString("filename")))->getmotion();
	action->object = inimodels.find(group->getString("model"))->second;
	ProcessPointerList(group->getString("pointer"), action);
}

static void ProcessAnimationINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	NJS_MOTION *animation = (new AnimationFile(mod_dir + L'\\' + group->getWString("filename")))->getmotion();
	ProcessPointerList(group->getString("pointer"), animation);
}

static void ProcessObjListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	const IniFile *objlistdata = new IniFile(mod_dir + L'\\' + group->getWString("filename"));
	vector<ObjectListEntry> objs;
	for (int i = 0; i < 999; i++)
	{
		char key[4];
		_snprintf(key, sizeof(key), "%d", i);
		if (!objlistdata->hasGroup(key)) break;
		const IniGroup *objdata = objlistdata->getGroup(key);
		ObjectListEntry entry;
		entry.Arg1 = objdata->getInt("Arg1");
		entry.Arg2 = objdata->getInt("Arg2");
		entry.UseDistance = objdata->getInt("Flags");
		entry.Distance = objdata->getFloat("Distance");
		entry.LoadSub = (ObjectFuncPtr)objdata->getIntRadix("Code", 16);
		entry.Name = UTF8toSJIS(objdata->getString("Name").c_str());
		objs.push_back(entry);
	}
	delete objlistdata;
	ObjectList *list = new ObjectList;
	list->Count = objs.size();
	list->List = new ObjectListEntry[list->Count];
	arrcpy(list->List, objs.data(), list->Count);
	ProcessPointerList(group->getString("pointer"), list);
}

static void ProcessStartPosINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	const IniFile *startposdata = new IniFile(mod_dir + L'\\' + group->getWString("filename"));
	vector<StartPosition> poss;
	for (auto iter = startposdata->cbegin(); iter != startposdata->cend(); iter++)
	{
		StartPosition pos;
		uint16_t levelact = ParseLevelAndActID(iter->first);
		pos.LevelID = (int16_t)(levelact >> 8);
		pos.ActID = (int16_t)(levelact & 0xFF);
		ParseVertex(iter->second->getString("Position", "0,0,0"), pos.Position);
		pos.YRot = iter->second->getIntRadix("YRotation", 16);
		poss.push_back(pos);
	}
	delete startposdata;
	auto numents = poss.size();
	StartPosition *list = new StartPosition[numents + 1];
	arrcpy(list, poss.data(), numents);
	clrmem(&list[numents]);
	list[numents].LevelID = LevelIDs_Invalid;
	ProcessPointerList(group->getString("pointer"), list);
}

static vector<PVMEntry> ProcessTexListINI_Internal(const IniFile *texlistdata)
{
	vector<PVMEntry> texs;
	for (int i = 0; i < 999; i++)
	{
		char key[4];
		_snprintf(key, sizeof(key), "%d", i);
		if (!texlistdata->hasGroup(key)) break;
		const IniGroup *pvmdata = texlistdata->getGroup(key);
		PVMEntry entry;
		entry.Name = strdup(pvmdata->getString("Name").c_str());
		entry.TexList = (NJS_TEXLIST *)pvmdata->getIntRadix("Textures", 16);
		texs.push_back(entry);
	}
	return texs;
}

static void ProcessTexListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	const IniFile *texlistdata = new IniFile(mod_dir + L'\\' + group->getWString("filename"));
	vector<PVMEntry> texs = ProcessTexListINI_Internal(texlistdata);
	delete texlistdata;
	auto numents = texs.size();
	PVMEntry *list = new PVMEntry[numents + 1];
	arrcpy(list, texs.data(), numents);
	clrmem(&list[numents]);
	ProcessPointerList(group->getString("pointer"), list);
}

static void ProcessLevelTexListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	const IniFile *texlistdata = new IniFile(mod_dir + L'\\' + group->getWString("filename"));
	vector<PVMEntry> texs = ProcessTexListINI_Internal(texlistdata);
	auto numents = texs.size();
	PVMEntry *list = new PVMEntry[numents];
	arrcpy(list, texs.data(), numents);
	LevelPVMList *lvl = new LevelPVMList;
	lvl->Level = (int16_t)ParseLevelAndActID(texlistdata->getString("", "Level", "0000"));
	delete texlistdata;
	lvl->NumTextures = (int16_t)numents;
	lvl->PVMList = list;
	ProcessPointerList(group->getString("pointer"), lvl);
}

static void ProcessTrialLevelListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	ifstream fstr(mod_dir + L'\\' + group->getWString("filename"));
	vector<TrialLevelListEntry> lvls;
	while (fstr.good())
	{
		string str;
		getline(fstr, str);
		if (str.size() > 0)
		{
			TrialLevelListEntry ent;
			uint16_t lvl = ParseLevelAndActID(str);
			ent.Level = (char)(lvl >> 8);
			ent.Act = (char)lvl;
			lvls.push_back(ent);
		}
	}
	fstr.close();
	auto numents = lvls.size();
	TrialLevelList *list = new TrialLevelList;
	list->Levels = new TrialLevelListEntry[numents];
	arrcpy(list->Levels, lvls.data(), numents);
	list->Count = (int)numents;
	ProcessPointerList(group->getString("pointer"), list);
}

static void ProcessBossLevelListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	ifstream fstr(mod_dir + L'\\' + group->getWString("filename"));
	vector<uint16_t> lvls;
	while (fstr.good())
	{
		string str;
		getline(fstr, str);
		if (str.size() > 0)
			lvls.push_back(ParseLevelAndActID(str));
	}
	fstr.close();
	auto numents = lvls.size();
	uint16_t *list = new uint16_t[numents + 1];
	arrcpy(list, lvls.data(), numents);
	list[numents] = levelact(LevelIDs_Invalid, 0);
	ProcessPointerList(group->getString("pointer"), list);
}

static void ProcessFieldStartPosINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	const IniFile *startposdata = new IniFile(mod_dir + L'\\' + group->getWString("filename"));
	vector<FieldStartPosition> poss;
	for (auto iter = startposdata->cbegin(); iter != startposdata->cend(); iter++)
	{
		FieldStartPosition pos = { ParseLevelID(iter->first) };
		pos.FieldID = ParseLevelID(iter->second->getString("Field", "Invalid"));
		ParseVertex(iter->second->getString("Position", "0,0,0"), pos.Position);
		pos.YRot = iter->second->getIntRadix("YRotation", 16);
		poss.push_back(pos);
	}
	delete startposdata;
	auto numents = poss.size();
	FieldStartPosition *list = new FieldStartPosition[numents + 1];
	arrcpy(list, poss.data(), numents);
	clrmem(&list[numents]);
	list[numents].LevelID = LevelIDs_Invalid;
	ProcessPointerList(group->getString("pointer"), list);
}

static void ProcessSoundTestListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	const IniFile *inidata = new IniFile(mod_dir + L'\\' + group->getWString("filename"));
	vector<SoundTestEntry> sounds;
	for (int i = 0; i < 999; i++)
	{
		char key[4];
		_snprintf(key, sizeof(key), "%d", i);
		if (!inidata->hasGroup(key)) break;
		const IniGroup *snddata = inidata->getGroup(key);
		SoundTestEntry entry;
		entry.Name = UTF8toSJIS(snddata->getString("Title").c_str());
		entry.ID = snddata->getInt("Track");
		sounds.push_back(entry);
	}
	delete inidata;
	auto numents = sounds.size();
	SoundTestCategory *cat = new SoundTestCategory;
	cat->Entries = new SoundTestEntry[numents];
	arrcpy(cat->Entries, sounds.data(), numents);
	cat->Count = (int)numents;
	ProcessPointerList(group->getString("pointer"), cat);
}

static void ProcessMusicListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	const IniFile *inidata = new IniFile(mod_dir + L'\\' + group->getWString("filename"));
	vector<MusicInfo> songs;
	for (int i = 0; i < 999; i++)
	{
		char key[4];
		_snprintf(key, sizeof(key), "%d", i);
		if (!inidata->hasGroup(key)) break;
		const IniGroup *musdata = inidata->getGroup(key);
		MusicInfo entry;
		entry.Name = strdup(musdata->getString("Filename").c_str());
		entry.Loop = (int)musdata->getBool("Loop");
		songs.push_back(entry);
	}
	delete inidata;
	auto numents = songs.size();
	MusicInfo *list = new MusicInfo[numents];
	arrcpy(list, songs.data(), numents);
	ProcessPointerList(group->getString("pointer"), list);
}

static void ProcessSoundListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	const IniFile *inidata = new IniFile(mod_dir + L'\\' + group->getWString("filename"));
	vector<SoundFileInfo> sounds;
	for (int i = 0; i < 999; i++)
	{
		char key[4];
		_snprintf(key, sizeof(key), "%d", i);
		if (!inidata->hasGroup(key)) break;
		const IniGroup *snddata = inidata->getGroup(key);
		SoundFileInfo entry;
		entry.Bank = snddata->getInt("Bank");
		entry.Filename = strdup(snddata->getString("Filename").c_str());
		sounds.push_back(entry);
	}
	delete inidata;
	auto numents = sounds.size();
	SoundList *list = new SoundList;
	list->List = new SoundFileInfo[numents];
	arrcpy(list->List, sounds.data(), numents);
	list->Count = (int)numents;
	ProcessPointerList(group->getString("pointer"), list);
}

static vector<char *> ProcessStringArrayINI_Internal(const wstring &filename, uint8_t language)
{
	ifstream fstr(filename);
	vector<char *> strs;
	while (fstr.good())
	{
		string str;
		getline(fstr, str);
		str = DecodeUTF8(UnescapeNewlines(str), language);
		strs.push_back(strdup(str.c_str()));
	}
	fstr.close();
	return strs;
}

static void ProcessStringArrayINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	vector<char *> strs = ProcessStringArrayINI_Internal(mod_dir + L'\\' + group->getWString("filename"),
		ParseLanguage(group->getString("language")));
	auto numents = strs.size();
	char **list = new char *[numents];
	arrcpy(list, strs.data(), numents);
	ProcessPointerList(group->getString("pointer"), list);
}

static void ProcessNextLevelListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	const IniFile *inidata = new IniFile(mod_dir + L'\\' + group->getWString("filename"));
	vector<NextLevelData> ents;
	for (int i = 0; i < 999; i++)
	{
		char key[4];
		_snprintf(key, sizeof(key), "%d", i);
		if (!inidata->hasGroup(key)) break;
		const IniGroup *entdata = inidata->getGroup(key);
		NextLevelData entry;
		entry.CGMovie = (char)entdata->getInt("CGMovie");
		entry.CurrentLevel = (char)ParseLevelID(entdata->getString("Level"));
		entry.NextLevelAdventure = (char)ParseLevelID(entdata->getString("NextLevel"));
		entry.NextActAdventure = (char)entdata->getInt("NextAct");
		entry.StartPointAdventure = (char)entdata->getInt("StartPos");
		entry.AltNextLevel = (char)ParseLevelID(entdata->getString("AltNextLevel"));
		entry.AltNextAct = (char)entdata->getInt("AltNextAct");
		entry.AltStartPoint = (char)entdata->getInt("AltStartPos");
		ents.push_back(entry);
	}
	delete inidata;
	auto numents = ents.size();
	NextLevelData *list = new NextLevelData[numents + 1];
	arrcpy(list, ents.data(), numents);
	clrmem(&list[numents]);
	list[numents].CurrentLevel = -1;
	ProcessPointerList(group->getString("pointer"), list);
}

static const wstring languagenames[] = { L"Japanese", L"English", L"French", L"Spanish", L"German" };
static void ProcessCutsceneTextINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename")) return;
	char ***addr = (char ***)group->getIntRadix("address", 16);
	if (addr == nullptr) return;
	wstring pathbase = mod_dir + L'\\' + group->getWString("filename") + L'\\';
	for (unsigned int i = 0; i < LengthOfArray(languagenames); i++)
	{
		vector<char *> strs = ProcessStringArrayINI_Internal(pathbase + languagenames[i] + L".txt", i);
		auto numents = strs.size();
		char **list = new char *[numents];
		arrcpy(list, strs.data(), numents);
		*addr++ = list;
	}
}

static void ProcessRecapScreenINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename")) return;
	int length = group->getInt("length");
	RecapScreen **addr = (RecapScreen **)group->getIntRadix("address", 16);
	if (addr == nullptr) return;
	wstring pathbase = mod_dir + L'\\' + group->getWString("filename") + L'\\';
	for (unsigned int l = 0; l < LengthOfArray(languagenames); l++)
	{
		RecapScreen *list = new RecapScreen[length];
		for (int i = 0; i < length; i++)
		{
			wchar_t buf[4];
			swprintf_s(buf, L"%d", i);
			const IniFile *inidata = new IniFile(pathbase + buf + L'\\' + languagenames[i] + L".ini");
			vector<string> strs = split(inidata->getString("", "Text"), '\n');
			auto numents = strs.size();
			list[i].TextData = new char *[numents];
			for (unsigned int j = 0; j < numents; j++)
				list[i].TextData[j] = strdup(DecodeUTF8(strs[j], l).c_str());
			list[i].LineCount = (int)numents;
			list[i].Speed = inidata->getFloat("", "Speed", 1);
			delete inidata;
		}
		*addr++ = list;
	}
}

static void ProcessNPCTextINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename")) return;
	int length = group->getInt("length");
	HintText_Entry **addr = (HintText_Entry **)group->getIntRadix("address", 16);
	if (addr == nullptr) return;
	wstring pathbase = mod_dir + L'\\' + group->getWString("filename") + L'\\';
	for (unsigned int l = 0; l < LengthOfArray(languagenames); l++)
	{
		HintText_Entry *list = new HintText_Entry[length];
		for (int i = 0; i < length; i++)
		{
			wchar_t buf[4];
			swprintf_s(buf, L"%d", i);
			const IniFile *inidata = new IniFile(pathbase + buf + L'\\' + languagenames[i] + L".ini");
			vector<int16_t> props;
			vector<HintText_Text> text;
			for (int j = 0; j < 999; j++)
			{
				char buf[4];
				_snprintf(buf, LengthOfArray(buf), "%d", j);
				if (!inidata->hasGroup(buf)) break;
				if (props.size() > 0)
					props.push_back(NPCTextControl_NewGroup);
				const IniGroup *entdata = inidata->getGroup(buf);
				if (entdata->hasKeyNonEmpty("EventFlags"))
				{
					vector<string> strs = split(entdata->getString("EventFlags"), ',');
					for (unsigned int k = 0; k < strs.size(); k++)
					{
						props.push_back(NPCTextControl_EventFlag);
						props.push_back((int16_t)strtol(strs[i].c_str(), nullptr, 10));
					}
				}
				if (entdata->hasKeyNonEmpty("NPCFlags"))
				{
					vector<string> strs = split(entdata->getString("NPCFlags"), ',');
					for (unsigned int k = 0; k < strs.size(); k++)
					{
						props.push_back(NPCTextControl_NPCFlag);
						props.push_back((int16_t)strtol(strs[i].c_str(), nullptr, 10));
					}
				}
				if (entdata->hasKeyNonEmpty("Character"))
				{
					props.push_back(NPCTextControl_Character);
					props.push_back((int16_t)ParseCharacterFlags(entdata->getString("Character")));
				}
				if (entdata->hasKeyNonEmpty("Voice"))
				{
					props.push_back(NPCTextControl_Voice);
					props.push_back((int16_t)entdata->getInt("Voice"));
				}
				if (entdata->hasKeyNonEmpty("SetEventFlag"))
				{
					props.push_back(NPCTextControl_SetEventFlag);
					props.push_back((int16_t)entdata->getInt("SetEventFlag"));
				}
				string linekey = buf;
				linekey += ".Lines[%d]";
				char *buf2 = new char[linekey.size() + 2];
				bool hasText = false;
				for (int k = 0; k < 999; k++)
				{
					_snprintf(buf2, linekey.size() + 2, linekey.c_str(), k);
					if (!inidata->hasGroup(buf2)) break;
					hasText = true;
					entdata = inidata->getGroup(buf2);
					HintText_Text entry;
					entry.Message = strdup(DecodeUTF8(entdata->getString("Line"), l).c_str());
					entry.Time = entdata->getInt("Time");
					text.push_back(entry);
				}
				delete[] buf2;
				delete inidata;
				if (hasText)
				{
					HintText_Text t = { };
					text.push_back(t);
				}
			}
			if (props.size() > 0)
			{
				props.push_back(NPCTextControl_End);
				list[i].Properties = new int16_t[props.size()];
				arrcpy(list[i].Properties, props.data(), props.size());
			}
			else
				list[i].Properties = nullptr;
			if (text.size() > 0)
			{
				list[i].Text = new HintText_Text[text.size()];
				arrcpy(list[i].Text, text.data(), text.size());
			}
			else
				list[i].Text = nullptr;
		}
		*addr++ = list;
	}
}

static void ProcessLevelClearFlagListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	ifstream fstr(mod_dir + L'\\' + group->getWString("filename"));
	vector<LevelClearFlagData> lvls;
	while (fstr.good())
	{
		string str;
		getline(fstr, str);
		if (str.size() > 0)
		{
			LevelClearFlagData ent;
			vector<string> parts = split(str, ' ');
			ent.Level = ParseLevelID(parts[0]);
			ent.FlagOffset = (int16_t)strtol(parts[1].c_str(), nullptr, 16);
			lvls.push_back(ent);
		}
	}
	fstr.close();
	auto numents = lvls.size();
	LevelClearFlagData *list = new LevelClearFlagData[numents + 1];
	arrcpy(list, lvls.data(), numents);
	list[numents].Level = -1;
	ProcessPointerList(group->getString("pointer"), list);
}

static void ProcessDeathZoneINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wstring dzinipath = mod_dir + L'\\' + group->getWString("filename");
	IniFile *dzdata = new IniFile(dzinipath);
	wchar_t *buf = new wchar_t[dzinipath.size() + 1];
	wcsncpy(buf, dzinipath.c_str(), dzinipath.size());
	PathRemoveFileSpec(buf);
	wstring dzpath = buf;
	delete[] buf;
	vector<DeathZone> deathzones;
	for (int i = 0; i < 999; i++)
	{
		char key[4];
		_snprintf(key, sizeof(key), "%d", i);
		if (!dzdata->hasGroup(key)) break;
		uint8_t flag = ParseCharacterFlags(dzdata->getString(key, "Flags"));
		wchar_t wkey[4];
		_snwprintf(wkey, 4, L"%d", i);
		ModelInfo *dzmdl = new ModelInfo(dzpath + L"\\" + wkey + L".sa1mdl");
		DeathZone dz = { flag, dzmdl->getmodel() };
		deathzones.push_back(dz);
	}
	delete dzdata;
	DeathZone *newlist = new DeathZone[deathzones.size() + 1];
	arrcpy(newlist, deathzones.data(), deathzones.size());
	clrmem(&newlist[deathzones.size()]);
	ProcessPointerList(group->getString("pointer"), newlist);
}

static void ProcessSkyboxScaleINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename")) return;
	int count = group->getInt("count");
	SkyboxScale **addr = (SkyboxScale **)group->getIntRadix("address", 16);
	if (addr == nullptr) return;
	const IniFile *inidata = new IniFile(mod_dir + L'\\' + group->getWString("filename"));
	for (int i = 0; i < count; i++)
	{
		char key[4];
		_snprintf(key, sizeof(key), "%d", i);
		if (!inidata->hasGroup(key))
		{
			*addr++ = nullptr;
			continue;
		}
		const IniGroup *entdata = inidata->getGroup(key);
		SkyboxScale *entry = new SkyboxScale;
		ParseVertex(entdata->getString("Far", "1,1,1"), entry->Far);
		ParseVertex(entdata->getString("Normal", "1,1,1"), entry->Normal);
		ParseVertex(entdata->getString("Near", "1,1,1"), entry->Near);
		*addr++ = entry;
	}
	delete inidata;
}

static void ProcessLevelPathListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wstring inipath = mod_dir + L'\\' + group->getWString("filename") + L'\\';
	vector<PathDataPtr> pathlist;
	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFileEx(inipath.c_str(), FindExInfoStandard, &data, FindExSearchLimitToDirectories, nullptr, 0);
	if (hFind == INVALID_HANDLE_VALUE) return;
	do
	{
		if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) continue;
		uint16_t levelact;
		try
		{
			levelact = ParseLevelAndActID(UTF16toMBS(wstring(data.cFileName), CP_UTF8));
		}
		catch (...)
		{
			continue;
		}
		wstring levelpath = inipath + data.cFileName + L"\\%d.ini";
		wchar_t *buf = new wchar_t[levelpath.size() + 2];
		vector<LoopHead *> paths;
		for (int i = 0; i < 999; i++)
		{
			_snwprintf(buf, levelpath.size() + 2, levelpath.c_str(), i);
			if (!PathFileExists(buf)) break;
			const IniFile *inidata = new IniFile(buf);
			const IniGroup *entdata;
			vector<Loop> points;
			char buf2[4];
			for (int j = 0; j < 999; j++)
			{
				_snprintf(buf2, LengthOfArray(buf2), "%d", j);
				if (!inidata->hasGroup(buf2)) break;
				entdata = inidata->getGroup(buf2);
				Loop point;
				point.Ang_X = (int16_t)entdata->getIntRadix("XRotation", 16);
				point.Ang_Y = (int16_t)entdata->getIntRadix("YRotation", 16);
				point.Dist = entdata->getFloat("Distance");
				ParseVertex(entdata->getString("Position", "0,0,0"), point.Position);
				points.push_back(point);
			}
			entdata = inidata->getGroup("");
			LoopHead *path = new LoopHead;
			path->Unknown_0 = (int16_t)entdata->getInt("Unknown");
			path->Count = (int16_t)points.size();
			path->TotalDist = entdata->getFloat("TotalDistance");
			path->LoopList = new Loop[path->Count];
			arrcpy(path->LoopList, points.data(), path->Count);
			path->Object = (ObjectFuncPtr)entdata->getIntRadix("Code", 16);
			paths.push_back(path);
			delete inidata;
		}
		delete[] buf;
		auto numents = paths.size();
		PathDataPtr ptr;
		ptr.LevelAct = levelact;
		ptr.PathList = new LoopHead *[numents + 1];
		arrcpy(ptr.PathList, paths.data(), numents);
		ptr.PathList[numents] = nullptr;
		pathlist.push_back(ptr);
	}
	while (FindNextFile(hFind, &data));
	FindClose(hFind);
	PathDataPtr *newlist = new PathDataPtr[pathlist.size() + 1];
	arrcpy(newlist, pathlist.data(), pathlist.size());
	clrmem(&newlist[pathlist.size()]);
	newlist[pathlist.size()].LevelAct = -1;
	ProcessPointerList(group->getString("pointer"), newlist);
}

static void ProcessStageLightDataListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	const IniFile *inidata = new IniFile(mod_dir + L'\\' + group->getWString("filename"));
	vector<StageLightData> ents;
	for (int i = 0; i < 999; i++)
	{
		char key[4];
		_snprintf(key, sizeof(key), "%d", i);
		if (!inidata->hasGroup(key)) break;
		const IniGroup *entdata = inidata->getGroup(key);
		StageLightData entry;
		entry.level = (char)ParseLevelID(entdata->getString("Level"));
		entry.act = (char)entdata->getInt("Act");
		entry.light_num = (char)entdata->getInt("LightNum");
		entry.use_yxz = (char)entdata->getBool("UseDirection");
		ParseVertex(entdata->getString("Direction"), entry.xyz);
		entry.dif = entdata->getFloat("Dif");
		entry.mutliplier = entdata->getFloat("Multiplier");
		ParseVertex(entdata->getString("RGB"), *(Vertex *)entry.rgb);
		ParseVertex(entdata->getString("AmbientRGB"), *(Vertex *)entry.amb_rgb);
		ents.push_back(entry);
	}
	delete inidata;
	auto numents = ents.size();
	StageLightData *list = new StageLightData[numents + 1];
	arrcpy(list, ents.data(), numents);
	clrmem(&list[numents]);
	list[numents].level = -1;
	ProcessPointerList(group->getString("pointer"), list);
}

static const unordered_map<string, void(__cdecl *)(const IniGroup *, const wstring &)> exedatafuncmap = {
	{ "landtable", ProcessLandTableINI },
	{ "model", ProcessModelINI },
	{ "basicdxmodel", ProcessModelINI },
	{ "chunkmodel", ProcessModelINI },
	{ "action", ProcessActionINI },
	{ "animation", ProcessAnimationINI },
	{ "objlist", ProcessObjListINI },
	{ "startpos", ProcessStartPosINI },
	{ "texlist", ProcessTexListINI },
	{ "leveltexlist", ProcessLevelTexListINI },
	{ "triallevellist", ProcessTrialLevelListINI },
	{ "bosslevellist", ProcessBossLevelListINI },
	{ "fieldstartpos", ProcessFieldStartPosINI },
	{ "soundtestlist", ProcessSoundTestListINI },
	{ "musiclist", ProcessMusicListINI },
	{ "soundlist", ProcessSoundListINI },
	{ "stringarray", ProcessStringArrayINI },
	{ "nextlevellist", ProcessNextLevelListINI },
	{ "cutscenetext", ProcessCutsceneTextINI },
	{ "recapscreen", ProcessRecapScreenINI },
	{ "npctext", ProcessNPCTextINI },
	{ "levelclearflaglist", ProcessLevelClearFlagListINI },
	{ "deathzone", ProcessDeathZoneINI },
	{ "skyboxscale", ProcessSkyboxScaleINI },
	{ "levelpathlist", ProcessLevelPathListINI },
	{ "stagelightdatalist", ProcessStageLightDataListINI }
};

static unordered_map<string, void *> dlllabels;

static void LoadDLLLandTable(const wstring &path)
{
	LandTableInfo *info = new LandTableInfo(path);
	auto labels = info->getlabels();
	for (auto iter = labels->cbegin(); iter != labels->cend(); iter++)
		dlllabels[iter->first] = iter->second;
}

static void LoadDLLModel(const wstring &path)
{
	ModelInfo *info = new ModelInfo(path);
	auto labels = info->getlabels();
	for (auto iter = labels->cbegin(); iter != labels->cend(); iter++)
		dlllabels[iter->first] = iter->second;
}

static void LoadDLLAnimation(const wstring &path)
{
	AnimationFile *info = new AnimationFile(path);
	dlllabels[info->getlabel()] = info->getmotion();
}

static const unordered_map<string, void(__cdecl *)(const wstring &)> dllfilefuncmap = {
	{ "landtable", LoadDLLLandTable },
	{ "model", LoadDLLModel },
	{ "basicdxmodel", LoadDLLModel },
	{ "chunkmodel", LoadDLLModel },
	{ "animation", LoadDLLAnimation }
};

static void ProcessLandTableDLL(const IniGroup *group, void *exp)
{
	memcpy(exp, dlllabels[group->getString("Label")], sizeof(LandTable));
}

static void ProcessLandTableArrayDLL(const IniGroup *group, void *exp)
{
	((LandTable **)exp)[group->getInt("Index")] = (LandTable *)dlllabels[group->getString("Label")];
}

static void ProcessModelDLL(const IniGroup *group, void *exp)
{
	memcpy(exp, dlllabels[group->getString("Label")], sizeof(NJS_OBJECT));
}

static void ProcessModelArrayDLL(const IniGroup *group, void *exp)
{
	((NJS_OBJECT **)exp)[group->getInt("Index")] = (NJS_OBJECT *)dlllabels[group->getString("Label")];
}

static void ProcessActionArrayDLL(const IniGroup *group, void *exp)
{
	string field = group->getString("Field");
	NJS_ACTION *act = ((NJS_ACTION **)exp)[group->getInt("Index")];
	if (field == "object")
		act->object = (NJS_OBJECT *)dlllabels[group->getString("Label")];
	else if (field == "motion")
		act->motion = (NJS_MOTION *)dlllabels[group->getString("Label")];
}

static const unordered_map<string, void(__cdecl *)(const IniGroup *, void *)> dlldatafuncmap = {
	{ "landtable", ProcessLandTableDLL },
	{ "landtablearray", ProcessLandTableArrayDLL },
	{ "model", ProcessModelDLL },
	{ "modelarray", ProcessModelArrayDLL },
	{ "basicdxmodel", ProcessModelDLL },
	{ "basicdxmodelarray", ProcessModelArrayDLL },
	{ "chunkmodel", ProcessModelDLL },
	{ "chunkmodelarray", ProcessModelArrayDLL },
	{ "actionarray", ProcessActionArrayDLL },
};

static const string dlldatakeys[] = {
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

static unordered_map<wstring, HMODULE> dllhandles;

struct dllexportinfo { void *address; string type; };
struct dllexportcontainer { unordered_map<string, dllexportinfo> exports; };
static unordered_map<wstring, dllexportcontainer> dllexports;

static void __cdecl InitMods(void)
{
	FILE *f_ini = _wfopen(L"mods\\SADXModLoader.ini", L"r");
	if (!f_ini)
	{
		MessageBox(NULL, L"mods\\SADXModLoader.ini could not be read!", L"SADX Mod Loader", MB_ICONWARNING);
		return;
	}
	unique_ptr<IniFile> ini(new IniFile(f_ini));
	fclose(f_ini);

	// Get sonic.exe's path and filename.
	wchar_t pathbuf[MAX_PATH];
	GetModuleFileName(NULL, pathbuf, MAX_PATH);
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
		PrintDebug("SADX Mod Loader v%s (API version %d), built %s\n",
			MODLOADER_VERSION_STRING, ModLoaderVer, __TIMESTAMP__);
#ifdef MODLOADER_GIT_VERSION
#ifdef MODLOADER_GIT_DESCRIBE
		PrintDebug("%s, %s\n", MODLOADER_GIT_VERSION, MODLOADER_GIT_DESCRIBE);
#else /* !MODLOADER_GIT_DESCRIBE */
		PrintDebug("%s\n", MODLOADER_GIT_VERSION);
#endif /* MODLOADER_GIT_DESCRIBE */
#endif /* MODLOADER_GIT_VERSION */
	}

	// Other various settings.
	if (!settings->getBool("DontFixWindow"))
		WriteJump((void *)0x789E50, (void *)sub_789E50_r);
	if (settings->getBool("DisableCDCheck"))
		WriteJump((void *)0x402621, (void *)0x402664);
	if (settings->getBool("UseCustomResolution"))
	{
		WriteJump((void *)0x40297A, (void *)0x402A90);

		int hres = settings->getInt("HorizontalResolution");
		if (hres > 0)
		{
			HorizontalResolution = hres;
			HorizontalStretch = HorizontalResolution / 640.0f;
		}

		int vres = settings->getInt("VerticalResolution");
		if (vres > 0)
		{
			VerticalResolution = vres;
			VerticalStretch = VerticalResolution / 480.0f;
		}
	}

	windowedfullscreen = settings->getBool("WindowedFullscreen");

	// Hijack a ton of functions in SADX.
	*(void **)0x38A5DB8 = (void *)0x38A5D94; // depth buffer fix
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

	// Unprotect the .rdata section.
	// TODO: Get .rdata address and length dynamically.
	DWORD oldprot;
	VirtualProtect((void *)0x7DB2A0, 0xB6D60, PAGE_WRITECOPY, &oldprot);

	// Map of files to replace and/or swap.
	// This is done with a second map instead of sadx_fileMap directly
	// in order to handle multiple mods.
	unordered_map<string, string> filereplaces;

	// It's mod loading time!
	PrintDebug("Loading mods...\n");
	for (int i = 1; i < 999; i++)
	{
		char key[8];
		snprintf(key, sizeof(key), "Mod%d", i);
		if (!settings->hasKey(key))
			break;

		const string mod_dirA = "mods\\" + settings->getString(key);
		const wstring mod_dir = L"mods\\" + settings->getWString(key);
		const wstring mod_inifile = mod_dir + L"\\mod.ini";
		FILE *f_mod_ini = _wfopen(mod_inifile.c_str(), L"r");
		if (!f_mod_ini)
		{
			PrintDebug("Could not open file mod.ini in \"mods\\%s\".\n", mod_dirA.c_str());
			continue;
		}
		unique_ptr<IniFile> ini_mod(new IniFile(f_mod_ini));
		fclose(f_mod_ini);

		const IniGroup *modinfo = ini_mod->getGroup("");
		const string mod_nameA = modinfo->getString("Name");
		const wstring mod_name = modinfo->getWString("Name");
		PrintDebug("%d. %s\n", i, mod_nameA.c_str());

		if (ini_mod->hasGroup("IgnoreFiles"))
		{
			const IniGroup *group = ini_mod->getGroup("IgnoreFiles");
			auto data = group->data();
			for (unordered_map<string, string>::const_iterator iter = data->begin();
			     iter != data->end(); ++iter)
			{
				sadx_fileMap.addIgnoreFile(iter->first);
				PrintDebug("Ignored file: %s\n", iter->first.c_str());
			}
		}

		if (ini_mod->hasGroup("ReplaceFiles"))
		{
			const IniGroup *group = ini_mod->getGroup("ReplaceFiles");
			auto data = group->data();
			for (unordered_map<string, string>::const_iterator iter = data->begin();
			     iter != data->end(); ++iter)
			{
				filereplaces[FileMap::normalizePath(iter->first)] =
					FileMap::normalizePath(iter->second);
			}
		}

		if (ini_mod->hasGroup("SwapFiles"))
		{
			const IniGroup *group = ini_mod->getGroup("SwapFiles");
			auto data = group->data();
			for (unordered_map<string, string>::const_iterator iter = data->begin();
			     iter != data->end(); ++iter)
			{
				filereplaces[FileMap::normalizePath(iter->first)] =
					FileMap::normalizePath(iter->second);
				filereplaces[FileMap::normalizePath(iter->second)] =
					FileMap::normalizePath(iter->first);
			}
		}

		// Check for SYSTEM replacements.
		// TODO: Convert to WString.
		const string modSysDirA = mod_dirA + "\\system";
		if ((GetFileAttributesA(modSysDirA.c_str()) & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
			sadx_fileMap.scanFolder(modSysDirA);

		// Check if a custom EXE is required.
		if (modinfo->hasKeyNonEmpty("EXEFile"))
		{
			wstring modexe = modinfo->getWString("EXEFile");
			transform(modexe.begin(), modexe.end(), modexe.begin(), ::towlower);

			// Are we using the correct EXE?
			if (modexe.compare(exefilename) != 0)
			{
				wchar_t msg[4096];
				swprintf(msg, LengthOfArray(msg),
					L"Mod \"%s\" should be run from \"%s\", but you are running \"%s\".\n\n"
					L"Continue anyway?", mod_name.c_str(), modexe.c_str(), exefilename.c_str());
				if (MessageBox(NULL, msg, L"SADX Mod Loader", MB_ICONWARNING | MB_YESNO) == IDNO)
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
			if (module)
			{
				const ModInfo *info = (const ModInfo *)GetProcAddress(module, "SADXModInfo");
				if (info)
				{
					if (info->Patches)
					{
						for (int i = 0; i < info->PatchCount; i++)
							WriteData(info->Patches[i].address, info->Patches[i].data, info->Patches[i].datasize);
					}
					if (info->Jumps)
					{
						for (int i = 0; i < info->JumpCount; i++)
							WriteJump(info->Jumps[i].address, info->Jumps[i].data);
					}
					if (info->Calls)
					{
						for (int i = 0; i < info->CallCount; i++)
							WriteCall(info->Calls[i].address, info->Calls[i].data);
					}
					if (info->Pointers)
					{
						for (int i = 0; i < info->PointerCount; i++)
							WriteData((void **)info->Pointers[i].address, info->Pointers[i].data);
					}
					if (info->Init)
					{
						// TODO: Convert to Unicode later. (Will require an API bump.)
						info->Init(mod_dirA.c_str(), helperFunctions);
					}
					const ModInitFunc init = (const ModInitFunc)GetProcAddress(module, "Init");
					if (init)
						init(mod_dirA.c_str(), helperFunctions);
					const PatchList *patches = (const PatchList *)GetProcAddress(module, "Patches");
					if (patches)
						for (int i = 0; i < patches->Count; i++)
							WriteData(patches->Patches[i].address, patches->Patches[i].data, patches->Patches[i].datasize);
					const PointerList *jumps = (const PointerList *)GetProcAddress(module, "Jumps");
					if (jumps)
						for (int i = 0; i < jumps->Count; i++)
							WriteJump(jumps->Pointers[i].address, jumps->Pointers[i].data);
					const PointerList *calls = (const PointerList *)GetProcAddress(module, "Calls");
					if (calls)
						for (int i = 0; i < calls->Count; i++)
							WriteCall(calls->Pointers[i].address, calls->Pointers[i].data);
					const PointerList *pointers = (const PointerList *)GetProcAddress(module, "Pointers");
					if (pointers)
						for (int i = 0; i < pointers->Count; i++)
							WriteData((void **)pointers->Pointers[i].address, pointers->Pointers[i].data);
					const ModFrameFunc frame = (const ModFrameFunc)GetProcAddress(module, "OnFrame");
					if (frame)
						modFrameFuncs.push_back(frame);
				}
				else
				{
					const string dll_filenameA = UTF16toMBS(dll_filename, CP_ACP);
					PrintDebug("File \"%s\" is not a valid mod file.\n", dll_filenameA.c_str());
				}
			}
			else
			{
				const string dll_filenameA = UTF16toMBS(dll_filename, CP_ACP);
				PrintDebug("Failed loading file \"%s\".\n", dll_filenameA.c_str());
			}
		}

		// Check if the mod has EXE data replacements.
		if (modinfo->hasKeyNonEmpty("EXEData"))
		{
			IniFile *exedata = new IniFile(mod_dir + L'\\' + modinfo->getWString("EXEData"));
			for (auto iter = exedata->cbegin(); iter != exedata->cend(); iter++)
			{
				IniGroup *group = iter->second;
				auto type = exedatafuncmap.find(group->getString("type"));
				if (type != exedatafuncmap.end())
					type->second(group, mod_dir);
			}
			delete exedata;
		}

		// Check if the mod has DLL data replacements.
		for (unsigned int i = 0; i < LengthOfArray(dlldatakeys); i++)
			if (modinfo->hasKeyNonEmpty(dlldatakeys[i]))
			{
				IniFile *dlldata = new IniFile(mod_dir + L'\\' + modinfo->getWString(dlldatakeys[i]));
				dlllabels.clear();
				const IniGroup *group = dlldata->getGroup("Files");
				for (auto iter = group->cbegin(); iter != group->cend(); iter++)
				{
					auto type = dllfilefuncmap.find(split(iter->second, '|')[0]);
					if (type != dllfilefuncmap.end())
						type->second(mod_dir + L'\\' + MBStoUTF16(iter->first, CP_UTF8));
				}
				wstring dllname = dlldata->getWString("", "name");
				HMODULE dllhandle;
				if (dllhandles.find(dllname) != dllhandles.cend())
					dllhandle = dllhandles[dllname];
				else
				{
					dllhandle = GetModuleHandle(dllname.c_str());
					dllhandles[dllname] = dllhandle;
				}
				if (dllexports.find(dllname) == dllexports.end())
				{
					group = dlldata->getGroup("Exports");
					dllexportcontainer exp;
					for (auto iter = group->cbegin(); iter != group->cend(); iter++)
					{
						dllexportinfo inf;
						inf.address = GetProcAddress(dllhandle, iter->first.c_str());
						inf.type = iter->second;
						exp.exports[iter->first] = inf;
					}
					dllexports[dllname] = exp;
				}
				const auto exports = &dllexports[dllname].exports;
				char buf[9];
				for (int i = 0; i < 9999; i++)
				{
					_snprintf(buf, 5, "Item%d", i);
					if (dlldata->hasGroup(buf))
					{
						group = dlldata->getGroup(buf);
						const dllexportinfo &exp = (*exports)[group->getString("Export")];
						auto type = dlldatafuncmap.find(exp.type);
						if (type != dlldatafuncmap.end())
							type->second(group, exp.address);
					}
				}
				delete dlldata;
			}
	}

	// Replace filenames. ("ReplaceFiles", "SwapFiles")
	for (auto iter = filereplaces.cbegin(); iter != filereplaces.cend(); ++iter)
	{
		sadx_fileMap.addReplaceFile(iter->first, iter->second);
	}

	for (auto i = StartPositions.cbegin(); i != StartPositions.cend(); ++i)
	{
		auto poslist = &i->second;
		StartPosition *newlist = new StartPosition[poslist->size() + 1];
		StartPosition *cur = newlist;
		for (auto j = poslist->cbegin(); j != poslist->cend(); ++j)
			*cur++ = j->second;
		cur->LevelID = LevelIDs_Invalid;
		switch (i->first)
		{
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

	for (auto i = FieldStartPositions.cbegin(); i != FieldStartPositions.cend(); ++i)
	{
		auto poslist = &i->second;
		FieldStartPosition *newlist = new FieldStartPosition[poslist->size() + 1];
		FieldStartPosition *cur = newlist;
		for (auto j = poslist->cbegin(); j != poslist->cend(); ++j)
			*cur++ = j->second;
		cur->LevelID = LevelIDs_Invalid;
		((FieldStartPosition **)0x90BEFC)[i->first] = newlist;
	}

	if (PathsInitialized)
	{
		PathDataPtr *newlist = new PathDataPtr[Paths.size() + 1];
		PathDataPtr *cur = newlist;
		for (auto i = Paths.cbegin(); i != Paths.cend(); ++i)
			*cur++ = i->second;
		cur->LevelAct = 0xFFFF;
		WriteData((PathDataPtr **)0x49C1A1, newlist);
		WriteData((PathDataPtr **)0x49C1AF, newlist);
	}

	for (auto i = CharacterPVMs.cbegin(); i != CharacterPVMs.cend(); ++i)
	{
		const vector<PVMEntry> *pvmlist = &i->second;
		auto size = pvmlist->size();
		PVMEntry *newlist = new PVMEntry[size + 1];
		memcpy(newlist, pvmlist->data(), sizeof(PVMEntry) * size);
		newlist[size].TexList = nullptr;
		((PVMEntry **)0x90ED54)[i->first] = newlist;
	}

	if (CommonObjectPVMsInitialized)
	{
		auto size = CommonObjectPVMs.size();
		PVMEntry *newlist = new PVMEntry[size + 1];
		PVMEntry *cur = newlist;
		memcpy(newlist, CommonObjectPVMs.data(), sizeof(PVMEntry) * size);
		newlist[size].TexList = nullptr;
		*((PVMEntry **)0x90EC70) = newlist;
		*((PVMEntry **)0x90EC74) = newlist;
	}

	for (auto i = _TrialLevels.cbegin(); i != _TrialLevels.cend(); ++i)
	{
		const vector<TrialLevelListEntry> *levellist = &i->second;
		auto size = levellist->size();
		TrialLevelListEntry *newlist = new TrialLevelListEntry[size];
		memcpy(newlist, levellist->data(), sizeof(TrialLevelListEntry) * size);
		TrialLevels[i->first].Levels = newlist;
		TrialLevels[i->first].Count = size;
	}

	for (auto i = _TrialSubgames.cbegin(); i != _TrialSubgames.cend(); ++i)
	{
		const vector<TrialLevelListEntry> *levellist = &i->second;
		auto size = levellist->size();
		TrialLevelListEntry *newlist = new TrialLevelListEntry[size];
		memcpy(newlist, levellist->data(), sizeof(TrialLevelListEntry) * size);
		TrialSubgames[i->first].Levels = newlist;
		TrialSubgames[i->first].Count = size;
	}

	PrintDebug("Finished loading mods\n");

	// Check for codes.
	ifstream codes_str("mods\\Codes.dat", ifstream::binary);
	if (codes_str.is_open())
	{
		static const char codemagic[6] = {'c', 'o', 'd', 'e', 'v', '4'};
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
	WriteJump((void *)0x426063, (void *)ProcessCodes);
}

static void __cdecl LoadChrmodels(void)
{
	chrmodelshandle = LoadLibrary(L".\\system\\CHRMODELS_orig.dll");
	if (!chrmodelshandle)
	{
		MessageBox(NULL, L"CHRMODELS_orig.dll could not be loaded!\n\n"
			L"SADX will now proceed to abruptly exit.",
			L"SADX Mod Loader", MB_ICONERROR);
		ExitProcess(1);
	}
	dllhandles[L"CHRMODELS_orig"] = chrmodelshandle;
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
			myhandle = hinstDll;
			HookTheAPI();

			// Make sure this is the correct version of SADX.
			if (memcmp(verchk_data, verchk_addr, sizeof(verchk_data)) != 0)
			{
				MessageBox(NULL, L"This copy of Sonic Adventure DX is not the US version.\n\n"
					L"Please obtain the EXE file from the US version and try again.",
					L"SADX Mod Loader", MB_ICONERROR);
				ExitProcess(1);
			}

			WriteData((unsigned char*)0x401AE1, (unsigned char)0x90);
			WriteCall((void *)0x401AE2, (void *)LoadChrmodels);
			break;

		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;

		case DLL_PROCESS_DETACH:
			// Make sure the log file is closed.
			if (dbgFile)
			{
				fclose(dbgFile);
				dbgFile = nullptr;
			}
			break;
	}

	return TRUE;
}
