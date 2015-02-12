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

static void __cdecl ProcessCodes()
{
	codeParser.processCodeList();
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

DataPointer(int, dword_3D08534, 0x3D08534);
static void __cdecl sub_789BD0()
{
	MSG v0; // [sp+4h] [bp-1Ch]@1

	if ( PeekMessageA(&v0, 0, 0, 0, 1u) )
	{
		do
		{
			TranslateMessage(&v0);
			DispatchMessageA(&v0);
		}
		while ( PeekMessageA(&v0, 0, 0, 0, 1u) );
		dword_3D08534 = v0.wParam;
	}
	else
	{
		dword_3D08534 = v0.wParam;
	}
}

static Gdiplus::Bitmap *bgimg;
DataPointer(HWND, hWnd, 0x3D0FD30);
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
	default:
		// alternatively we can return SendMe
		return DefWindowProc(wrapper, uMsg, wParam, lParam);
	}
	/* unreachable */ return 0;
}

static bool windowedfullscreen = false;

DataPointer(int, Windowed, 0x38A5DC4);
DataPointer(HINSTANCE, hInstance, 0x3D0FD34);
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
	if (!Windowed && windowedfullscreen)
	{
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

		int scrnw = GetSystemMetrics(SM_CXSCREEN);
		int scrnh = GetSystemMetrics(SM_CYSCREEN);

		HWND wrapper = CreateWindowExA(WS_EX_APPWINDOW,
			"WrapperWindow",
			(const char *)0x7DB82C,
			WS_POPUP,
			0, 0, scrnw, scrnh,
			NULL, NULL, hInstance, NULL);

		if (wrapper == NULL)
			return;

		float num = min((float)scrnw / (float)HorizontalResolution, (float)scrnh / (float)VerticalResolution);
		int dispw = (int)((float)HorizontalResolution * num);
		int disph = (int)((float)VerticalResolution * num);
		int dispx = (scrnw - dispw) / 2;
		int dispy = (scrnh - disph) / 2;

		hWnd = CreateWindowExA(0, GetWindowClassName(), GetWindowClassName(), WS_CHILD | WS_VISIBLE,
			dispx, dispy, dispw, disph, wrapper, NULL, hInstance, 0);
		SetFocus(hWnd);
		ShowWindow(wrapper, nCmdShow);
		UpdateWindow(wrapper);
		SetForegroundWindow(wrapper);
		Windowed = 1;
		WriteJump((void *)0x789BD0, (void *)sub_789BD0);
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
		RECT wndsz = { 0, 0, HorizontalResolution, VerticalResolution };
		AdjustWindowRectEx(&wndsz, v2, false, 0);
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

void ClearTrialSubgameList(unsigned char character)
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

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    std::stringstream ss(s);
    string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

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
							WriteData((void**)info->Pointers[i].address, info->Pointers[i].data);
					}
					if (info->Init)
					{
						// TODO: Convert to Unicode later. (Will require an API bump.)
						info->Init(mod_dirA.c_str(), helperFunctions);
					}
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
				string type = group->getString("type");
				if (type == "landtable")
				{
					if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointers")) continue;
					LandTable *landtable = (new LandTableInfo(mod_dir + L'\\' + group->getWString("filename")))->getlandtable();
					vector<string> ptrs = split(group->getString("pointer"), ',');
					for (unsigned int i = 0; i < ptrs.size(); i++)
						*(LandTable **)(strtol(ptrs[i].c_str(), nullptr, 16) + 0x400000) = landtable;
				}
				else if (type == "model")
				{
					if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointers")) continue;
					NJS_OBJECT *model = (new ModelInfo(mod_dir + L'\\' + group->getWString("filename")))->getmodel();
					vector<string> ptrs = split(group->getString("pointer"), ',');
					for (unsigned int i = 0; i < ptrs.size(); i++)
						*(NJS_OBJECT **)(strtol(ptrs[i].c_str(), nullptr, 16) + 0x400000) = model;
				}
			}
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
		static const char codemagic[6] = {'c', 'o', 'd', 'e', 'v', '3'};
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
