// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <deque>
#include <algorithm>
#include <cstdio>
#include <memory>
#include <cerrno>
#include <cstring>

#include <dbghelp.h>
#include <shlwapi.h>
#include <wmsdkidl.h>

// NOTE: mmsystem.h defines PlaySound.
// Undefine it afterwards due to an SADX function conflict.
#include <mmsystem.h>
#ifdef PlaySound
#undef PlaySound
#endif
#include <dsound.h>

#include "bass_vgmstream.h"
using namespace std;

#include "config.SADXModLoader.h"
#include "git.h"

#include "IniFile.hpp"
#include "CodeParser.hpp"
#include "FileMap.hpp"
#include "SADXModLoader.h"

HMODULE myhandle;
HMODULE chrmodelshandle;
FARPROC __stdcall MyGetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	return GetProcAddress(hModule == myhandle ? chrmodelshandle : hModule, lpProcName);
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
static FileMap fileMap;

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
HANDLE __stdcall MyCreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	return CreateFileA(fileMap.replaceFile(lpFileName), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

int __cdecl PlayVoiceFile_r(LPCSTR filename)
{
	filename = fileMap.replaceFile(filename);
	return PlayVoiceFile(filename);
}

#ifndef _MSC_VER
// MinGW doesn't have IWMHeaderInfo.
struct WMHeaderInfo;
#endif

#pragma pack(push, 1)
struct WMPInfo
{
	void *WMReaderCallback;
	LPDIRECTSOUNDBUFFER DirectSoundBuffer;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
	HANDLE EventHandle;
	int field_20;
	IWMReader *WMReader;
	IWMHeaderInfo *WMHeaderInfo;
	int field_2C;
	int field_30;
	WCHAR *CurrentFile;
	int field_38;
	int field_3C;
	LPWAVEFORMATEX WaveFormat;
	int Status;
	int field_48;
	int field_4C;
};
#pragma pack(pop)

enum WMPStatus
{
	WMPStatus_Buffering,
	WMPStatus_Playing,
	WMPStatus_Stopped
};

DataPointer(int, MusicVolume, 0x909F28);
DataPointer(WMPInfo *, WMPVoiceInfo, 0x3ABDF94);
DataPointer(int, dword_3ABDF98, 0x3ABDF98);
DataPointer(WMPInfo *, WMPMusicInfo, 0x3ABDF9C);
DataPointer(int, dword_3ABDFA0, 0x3ABDFA0);
DataPointer(int, MusicLooping, 0x3ABDFA4);
DataPointer(int, dword_3ABDFA8, 0x3ABDFA8);

ThiscallFunctionPointer(int, WMPInfo__Release, (WMPInfo *), 0x410F70);
ThiscallFunctionPointer(int, WMPInfo__Play, (WMPInfo *, int, int, int), 0x410FF0);
ThiscallFunctionPointer(int, WMPInfo__Pause, (WMPInfo *), 0x4111B0);
ThiscallFunctionPointer(int, WMPInfo__Resume, (WMPInfo *), 0x4111E0);
ThiscallFunctionPointer(int, WMPInfo__GetStatus, (WMPInfo *), 0x4113E0);
ThiscallFunctionPointer(int, WMPInfo__Release2, (WMPInfo *), 0x411450);
ThiscallFunctionPointer(int, WMPInfo__CreateReader, (WMPInfo *), 0x4116A0);
ThiscallFunctionPointer(int, WMPInfo__Close, (WMPInfo *), 0x411720);
ThiscallFunctionPointer(int, WMPInfo__Stop, (WMPInfo *), 0x411760);
ThiscallFunctionPointer(unsigned int, WMPInfo__Open, (WMPInfo *, const wchar_t *), 0x411830);
ThiscallFunctionPointer(WMPInfo *, WMPInfo__WMPInfo, (WMPInfo *), 0x411970);

bool enablevgmstream = false;
bool musicwmp = true;
DWORD basschan = 0;

void WMPInit_r()
{
	enablevgmstream = BASS_Init(-1, 44100, 0, 0, NULL) ? true : false;
}

void __stdcall onTrackEnd(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	dword_3ABDFA0 = 0;
	dword_3ABDF98 = 5;
	BASS_ChannelStop(channel);
	BASS_StreamFree(channel);
}

int __cdecl PlayMusicFile_r(LPCSTR filename, int loop)
{
	if (!WMPMusicInfo) return 0;
	if (musicwmp)
	{
		WMPInfo__Stop(WMPMusicInfo);
		WMPInfo__Close(WMPMusicInfo);
	}
	else if (basschan != 0)
	{
		BASS_ChannelStop(basschan);
		BASS_StreamFree(basschan);
	}
	if (enablevgmstream)
	{
		char pathnoext[MAX_PATH];
		strncpy(pathnoext, filename, sizeof(pathnoext));
		PathRemoveExtensionA(pathnoext);
		string path = pathnoext;
		transform(path.begin(), path.end(), path.begin(), backslashes);
		if (path.length() > 2 && (path[0] == '.' && path[1] == '\\'))
			path = path.substr(2, path.length() - 2);
		transform(path.begin(), path.end(), path.begin(), ::tolower);
		// FIXME: Add a list with just filenames.
		// MSVC apparently maintains ordering; STL does not.
#if 0 // FIXME: Port to fileMap.
#ifdef _MSC_VER
		for (auto i = filemap.crbegin(); i != filemap.crend(); i++)
#else
		for (auto i = filemap.cbegin(); i != filemap.cend(); i++)
#endif
		{
			i->first.copy(pathnoext, i->first.length(), 0);
			pathnoext[i->first.length()] = 0;
			PathRemoveExtensionA(pathnoext);
			if (path.compare(pathnoext) == 0)
			{
				basschan = BASS_VGMSTREAM_StreamCreate(i->second, loop ? BASS_SAMPLE_LOOP : 0);
				if (basschan != 0)
				{
					musicwmp = false;
					BASS_ChannelPlay(basschan, false);
					BASS_ChannelSetAttribute(basschan, BASS_ATTRIB_VOL, (MusicVolume + 10000) / 30000.0f);
					BASS_ChannelSetSync(basschan, BASS_SYNC_END, 0, onTrackEnd, 0);
					MusicLooping = loop;
					dword_3ABDFA0 = 1;
					dword_3ABDF98 = 3;
					return 1;
				}
			}
		}
#endif
	}
	filename = fileMap.replaceFile(filename);
	musicwmp = true;
	WCHAR WideCharStr[MAX_PATH];
	MultiByteToWideChar(0, 0, filename, -1, WideCharStr, LengthOfArray(WideCharStr));
	if ( WMPMusicInfo && (WMPInfo__Open(WMPMusicInfo, WideCharStr) & 0x80000000u) == 0)
	{
		WMPInfo__Play(WMPMusicInfo, 0, 0, MusicVolume);
		MusicLooping = loop;
		dword_3ABDFA0 = 1;
		dword_3ABDF98 = 3;
		if ( WMPInfo__GetStatus(WMPMusicInfo) == WMPStatus_Stopped )
		{
			do
			Sleep(0);
			while ( WMPInfo__GetStatus(WMPMusicInfo) == WMPStatus_Stopped );
		}
		return 1;
	}
	return 0;
}

void __cdecl WMPRestartMusic_r()
{
	LPDIRECTSOUNDBUFFER v0; // eax@6

	if (!musicwmp)
		return;
	if ( dword_3ABDFA0 )
	{
		if ( WMPInfo__GetStatus(WMPMusicInfo) == WMPStatus_Stopped )
		{
			if ( MusicLooping )
			{
				WMPInfo__Stop(WMPMusicInfo);
				WMPInfo__Play(WMPMusicInfo, 0, 0, MusicVolume);
			}
			else
			{
				dword_3ABDFA0 = 0;
				dword_3ABDF98 = 5;
				WMPInfo__Stop(WMPMusicInfo);
				WMPInfo__Close(WMPMusicInfo);
			}
		}
		else
		{
			v0 = WMPMusicInfo->DirectSoundBuffer;
			if ( v0 )
				v0->SetVolume(MusicVolume);
		}
	}
}

void __cdecl PauseSound_r()
{
	if ( dword_3ABDFA0 )
	{
		++dword_3ABDFA8;
		if (musicwmp)
			WMPInfo__Pause(WMPMusicInfo);
		else
			BASS_ChannelPause(basschan);
	}
	if ( WMPVoiceInfo )
	{
		WMPInfo__Stop(WMPVoiceInfo);
		WMPInfo__Close(WMPVoiceInfo);
	}
}

void __cdecl ResumeSound_r()
{
	if ( dword_3ABDFA0 )
	{
		--dword_3ABDFA8;
		if ( dword_3ABDFA8 <= 0 )
		{
			if (musicwmp)
				WMPInfo__Resume(WMPMusicInfo);
			else
				BASS_ChannelPlay(basschan, false);
			dword_3ABDFA8 = 0;
		}
	}
}

void __cdecl WMPClose_r(int a1)
{
	if ( a1 )
	{
		if ( a1 == 1 && WMPVoiceInfo )
		{
			WMPInfo__Stop(WMPVoiceInfo);
			WMPInfo__Close(WMPVoiceInfo);
			dword_3ABDFA8 = 0;
			return;
		}
	}
	else
	{
		if ( dword_3ABDFA0 )
		{
			if (musicwmp)
			{
				WMPInfo__Stop(WMPMusicInfo);
				WMPInfo__Close(WMPMusicInfo);
			}
			else
			{
				BASS_ChannelStop(basschan);
				BASS_StreamFree(basschan);
			}
			dword_3ABDFA0 = 0;
			dword_3ABDF98 = 0;
		}
	}
	dword_3ABDFA8 = 0;
}

void WMPRelease_r()
{
	BASS_Free();
}

/**
 * C wrapper to call FileMap::replaceFile() from asm.
 * @param lpFileName Filename.
 * @return Replaced filename, or original filename if not replaced by a mod.
 */ 
const char *_ReplaceFile(const char *lpFileName)
{
	return fileMap.replaceFile(lpFileName);
}

__declspec(naked) int PlayVideoFile_r()
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

void HookTheAPI()
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

struct message { string text; uint32_t time; };

deque<message> msgqueue;

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
CodeParser codeParser;

void __cdecl ProcessCodes()
{
	codeParser.processCodeList();
	const int numrows = (VerticalResolution / 12);
	int pos;
	if ((int)msgqueue.size() <= numrows - 1)
		pos = (numrows - 1) - (msgqueue.size() - 1);
	else
		pos = 0;
	if (msgqueue.size() > 0)
		for (deque<message>::iterator i = msgqueue.begin(); i != msgqueue.end(); i++)
		{
			int c = -1;
			if (300 - i->time < LengthOfArray(fadecolors))
				c = fadecolors[LengthOfArray(fadecolors) - (300 - i->time) - 1];
			SetDebugTextColor((int)c);
			DisplayDebugString(pos++, (char *)i->text.c_str());
			if (++i->time >= 300)
			{
				msgqueue.pop_front();
				if (msgqueue.size() == 0)
					break;
				i = msgqueue.begin();
			}
			if (pos == numrows)
				break;
		}
}

char * ShiftJISToUTF8(char *shiftjis)
{
	int cchWcs = MultiByteToWideChar(932, 0, shiftjis, -1, NULL, 0);
	if (cchWcs <= 0) return nullptr;
	wchar_t *wcs = new wchar_t[cchWcs];
	MultiByteToWideChar(932, 0, shiftjis, -1, wcs, cchWcs);
	int cbMbs = WideCharToMultiByte(CP_UTF8, 0, wcs, -1, NULL, 0, NULL, NULL);
	if (cbMbs <= 0) { delete[] wcs; return nullptr; }
	char *utf8 = new char[cbMbs];
	WideCharToMultiByte(CP_UTF8, 0, wcs, -1, utf8, cbMbs, NULL, NULL);
	delete[] wcs;
	return utf8;
}

static bool dbgConsole, dbgScreen, dbgFile;
static ofstream dbgstr;
int __cdecl SADXDebugOutput(const char *Format, ...)
{
	va_list ap;
	va_start(ap, Format);
	int result = vsnprintf(NULL, 0, Format, ap) + 1;
	va_end(ap);
	char *buf = new char[result];
	va_start(ap, Format);
	result = vsnprintf(buf, result, Format, ap);
	va_end(ap);
	if (dbgConsole)
		cout << buf;
	if (dbgScreen)
	{
		message msg = { buf };
		if (msg.text[msg.text.length() - 1] == '\n')
			msg.text = msg.text.substr(0, msg.text.length() - 1);
		msgqueue.push_back(msg);
	}
	if (dbgFile && dbgstr.good())
	{
		char *utf8 = ShiftJISToUTF8(buf);
		dbgstr << utf8;
		delete[] utf8;
	}
	delete[] buf;
	return result;
}

DataPointer(int, dword_3D08534, 0x3D08534);
void __cdecl sub_789BD0()
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

DataPointer(HWND, hWnd, 0x3D0FD30);
LRESULT CALLBACK WrapperWndProc(HWND wrapper, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CLOSE:
		// we also need to let SADX do cleanup
		SendMessage(hWnd, WM_CLOSE, wParam, lParam);
		// what we do here is up to you: we can check if SADX decides to close, and if so, destroy ourselves, or something like that
		return 0;
	default:
		// alternatively we can return SendMe
		return DefWindowProc(wrapper, uMsg, wParam, lParam);
	}
	/* unreachable */ return 0;
}

bool windowedfullscreen = false;

DataPointer(int, Windowed, 0x38A5DC4);
DataPointer(HINSTANCE, hInstance, 0x3D0FD34);
void CreateSADXWindow(HINSTANCE _hInstance, int nCmdShow)
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

__declspec(naked) void sub_789E50_r()
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

unordered_map<unsigned char, unordered_map<int, StartPosition>> StartPositions;
void RegisterStartPosition(unsigned char character, const StartPosition &position)
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

void ClearStartPositionList(unsigned char character)
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

unordered_map<unsigned char, unordered_map<int, FieldStartPosition>> FieldStartPositions;
void RegisterFieldStartPosition(unsigned char character, const FieldStartPosition &position)
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

void ClearFieldStartPositionList(unsigned char character)
{
	if (character >= Characters_MetalSonic) return;
	FieldStartPositions[character] = unordered_map<int, FieldStartPosition>();
}

unordered_map<int, PathDataPtr> Paths;
bool PathsInitialized;
void RegisterPathList(const PathDataPtr &paths)
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

void ClearPathListList()
{
	Paths.clear();
	PathsInitialized = true;
}

unordered_map<unsigned char, vector<PVMEntry>> CharacterPVMs;
void RegisterCharacterPVM(unsigned char character, const PVMEntry &pvm)
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

void ClearCharacterPVMList(unsigned char character)
{
	if (character > Characters_MetalSonic) return;
	CharacterPVMs[character] = vector<PVMEntry>();
}

vector<PVMEntry> CommonObjectPVMs;
bool CommonObjectPVMsInitialized;
void RegisterCommonObjectPVM(const PVMEntry &pvm)
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

void ClearCommonObjectPVMList()
{
	CommonObjectPVMs.clear();
	CommonObjectPVMsInitialized = true;
}

unsigned char trialcharacters[] = { 0, 0xFFu, 1, 2, 0xFFu, 3, 5, 4, 6 };
inline unsigned char gettrialcharacter(unsigned char character)
{
	if (character >= LengthOfArray(trialcharacters))
		return 0xFF;
	return trialcharacters[character];
}

unordered_map<unsigned char, vector<TrialLevelListEntry>> _TrialLevels;
void RegisterTrialLevel(unsigned char character, const TrialLevelListEntry &level)
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

void ClearTrialLevelList(unsigned char character)
{
	character = gettrialcharacter(character);
	if (character == 0xFF) return;
	_TrialLevels[character] = vector<TrialLevelListEntry>();
}

unordered_map<unsigned char, vector<TrialLevelListEntry>> _TrialSubgames;
void RegisterTrialSubgame(unsigned char character, const TrialLevelListEntry &level)
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

const HelperFunctions helperFunctions = {
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

void __cdecl InitMods(void)
{
	FILE *f_ini = fopen("mods\\SADXModLoader.ini", "r");
	if (!f_ini)
	{
		MessageBox(NULL, L"mods\\SADXModLoader.ini could not be read!", L"SADX Mod Loader", MB_ICONWARNING);
		return;
	}
	unique_ptr<IniFile> ini(new IniFile(f_ini));
	fclose(f_ini);

	// Get sonic.exe's filename.
	// FIXME: Don't use MAX_PATH.
	char pathbuf[MAX_PATH];
	GetModuleFileNameA(NULL, pathbuf, MAX_PATH);
	string exefilename = pathbuf;
	exefilename = exefilename.substr(exefilename.find_last_of("/\\") + 1);
	transform(exefilename.begin(), exefilename.end(), exefilename.begin(), ::tolower);

	// Process the main Mod Loader settings.
	const IniGroup *settings = ini->getGroup("");

	if (settings->getBool("DebugConsole"))
	{
		// Enable the debug console.
		AllocConsole();
		SetConsoleTitle(L"SADX Mod Loader output");
		freopen("CONOUT$", "wb", stdout);
		dbgConsole = true;
	}

	dbgScreen = settings->getBool("DebugScreen");
	if (settings->getBool("DebugFile"))
	{
		// Enable debug logging to a file.
		dbgstr = ofstream("mods\\SADXModLoader.log", ios_base::ate | ios_base::app);
		dbgFile = dbgstr.is_open();
	}

	// Is any debug method enabled?
	if (dbgConsole || dbgScreen || dbgFile)
	{
		WriteJump((void *)PrintDebug, (void *)SADXDebugOutput);
		PrintDebug("SADX Mod Loader v%s (API version %d), built %s\n",
			SADXMODLOADER_VERSION_STRING, ModLoaderVer, __TIMESTAMP__);
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
	// This is done with a second map instead of fileMap directly
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

		string modname = settings->getString(key);
		string dir = "mods\\" + modname;
		string filename = dir + "\\mod.ini";
		FILE *f_mod_ini = fopen(filename.c_str(), "r");
		if (!f_mod_ini)
		{
			PrintDebug("Could not open file mod.ini in \"mods\\%s\".\n", modname.c_str());
			continue;
		}
		unique_ptr<IniFile> ini_mod(new IniFile(f_mod_ini));
		fclose(f_mod_ini);

		const IniGroup *modinfo = ini_mod->getGroup("");
		PrintDebug("%d. %s\n", i, modinfo->getString("Name").c_str());

		if (ini_mod->hasGroup("IgnoreFiles"))
		{
			const IniGroup *group = ini_mod->getGroup("IgnoreFiles");
			auto data = group->data();
			for (unordered_map<string, string>::const_iterator iter = data->begin();
			     iter != data->end(); ++iter)
			{
				fileMap.addIgnoreFile(iter->first);
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
		string modSysDir = dir + "\\system";
		if ((GetFileAttributesA(modSysDir.c_str()) & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
			fileMap.scanFolder(modSysDir);

		// Check if a custom EXE is required.
		if (modinfo->hasKey("EXEFile"))
		{
			string modexe = modinfo->getString("EXEFile");
			transform(modexe.begin(), modexe.end(), modexe.begin(), ::tolower);

			// Is the EXE correct?
			if (modexe.compare(exefilename) != 0)
			{
				char msg[4096];
				string mod_name = modinfo->getString("Name");
				snprintf(msg, sizeof(msg),
						"Mod \"%s\" should be run from \"%s\", but you are running \"%s\".\n\n"
						"Continue anyway?", mod_name.c_str(), modexe.c_str(), exefilename.c_str());
				if (MessageBoxA(NULL, msg, "SADX Mod Loader", MB_ICONWARNING | MB_YESNO) == IDNO)
					ExitProcess(1);
			}
		}

		// Check if the mod has a DLL file.
		string dll_filename = modinfo->getString("DLLFile");
		if (!dll_filename.empty())
		{
			dll_filename = dir + "\\" + dll_filename;
			HMODULE module = LoadLibraryA(dll_filename.c_str());
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
						info->Init(dir.c_str(), helperFunctions);
					}
				}
				else
				{
					PrintDebug("File \"%s\" is not a valid mod file.\n", dll_filename.c_str());
				}
			}
			else
			{
				PrintDebug("Failed loading file \"%s\".\n", dll_filename.c_str());
			}
		}
	}

	// Replace filenames. ("ReplaceFiles", "SwapFiles")
	for (auto iter = filereplaces.cbegin(); iter != filereplaces.cend(); ++iter)
	{
		fileMap.addReplaceFile(iter->first, iter->second);
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

void __cdecl LoadChrmodels(void)
{
	chrmodelshandle = LoadLibrary(L".\\system\\CHRMODELS_orig.dll");
	if (!chrmodelshandle)
	{
		MessageBox(NULL, L"CHRMODELS_orig.dll could not be loaded!\n\nSADX will now proceed to abruptly exit.", L"SADX Mod Loader", MB_ICONERROR);
		ExitProcess(1);
	}
	WriteCall((void *)0x402513, (void *)InitMods);
}

static const uint8_t verchk[] = { 0x83, 0xEC, 0x28, 0x57, 0x33 };
BOOL APIENTRY DllMain( HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		myhandle = hModule;
		HookTheAPI();
		if (memcmp(verchk, (void *)0x789E50, SizeOfArray(verchk)) != 0)
		{
			MessageBox(NULL, L"This copy of Sonic Adventure DX is not the US version.\n\nPlease obtain the EXE file from the US version and try again.", L"SADX Mod Loader", MB_ICONERROR);
			ExitProcess(1);
		}
		WriteData((unsigned char*)0x401AE1, (unsigned char)0x90);
		WriteCall((void *)0x401AE2, (void *)LoadChrmodels);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
