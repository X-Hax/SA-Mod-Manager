// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <fstream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <DbgHelp.h>
#include <cstdio>
#include "SADXModLoader.h"
using namespace std;

typedef unordered_map<string, string> IniGroup;
struct IniGroupStr { IniGroup Element; };
typedef unordered_map<string, IniGroupStr> IniDictionary;
IniDictionary LoadINI(istream &textfile)
{
	IniDictionary result = IniDictionary();
	result[""] = IniGroupStr();
	IniGroupStr *curent = &result[""];
	while (textfile.good())
	{
		string line;
		getline(textfile, line);
		string sb = string();
		sb.reserve(line.length());
		bool startswithbracket = false;
		int firstequals = -1;
		int endbracket = -1;
		for (int c = 0; c < (int)line.length(); c++)
			switch (line[c])
			{
			case '\\': // escape character
				if (c + 1 == line.length())
					goto appendchar;
				c++;
				switch (line[c])
				{
				case 'n': // line feed
					sb += '\n';
					break;
				case 'r': // carriage return
					sb += '\r';
					break;
				default: // literal character
					goto appendchar;
				}
				break;
			case '=':
				if (firstequals == -1)
					firstequals = sb.length();
				goto appendchar;
			case '[':
				if (c == 0)
					startswithbracket = true;
				goto appendchar;
			case ']':
				endbracket = sb.length();
				goto appendchar;
			case ';': // comment character, stop processing this line
				c = line.length();
				break;
			default:
appendchar:
				sb += line[c];
				break;
			}
		line = sb;
		if (startswithbracket && endbracket != -1)
		{
			line = line.substr(1, endbracket - 1);
			result[line] = IniGroupStr();
			curent = &result[line];
		}
		else if (!line.empty())
		{
			string key;
			string value = "";
			if (firstequals > -1)
			{
				key = line.substr(0, firstequals);
				value = line.substr(firstequals + 1);
			}
			else
				key = line;
			(*curent).Element[key] = value;
		}
	}
	return result;
}

HMODULE myhandle;
HMODULE chrmodelshandle;
unordered_map<string, void *> dataoverrides = unordered_map<string, void *>();
FARPROC __stdcall MyGetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	if (dataoverrides.find(lpProcName) != dataoverrides.end())
		return (FARPROC)dataoverrides[lpProcName];
	else if (hModule == myhandle)
		return GetProcAddress(chrmodelshandle, lpProcName);
	else
		return GetProcAddress(hModule, lpProcName);
}

inline int backslashes(int c)
{
	if (c == '/')
		return '\\';
	else
		return c;
}

IniGroup settings;
unordered_map<string, char *> filemap = unordered_map<string, char *>();
unordered_map<string, string> filereplaces = unordered_map<string, string>();
const string systemdir = "system\\";
CRITICAL_SECTION filereplacesection;
const char *_ReplaceFile(const char *lpFileName)
{
	EnterCriticalSection(&filereplacesection);
	string path = lpFileName;
	transform(path.begin(), path.end(), path.begin(), backslashes);
	if (path.length() > 2 && (path[0] == '.' && path[1] == '\\'))
		path = path.substr(2, path.length() - 2);
	transform(path.begin(), path.end(), path.begin(), ::tolower);
	unordered_map<string, string>::iterator replIter = filereplaces.find(path);
	if (replIter != filereplaces.end())
		path = replIter->second;
	unordered_map<string, char *>::iterator fileIter = filemap.find(path);
	if (fileIter != filemap.end())
		lpFileName = fileIter->second;
	LeaveCriticalSection(&filereplacesection);
	return lpFileName;
}

HANDLE __stdcall MyCreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	return CreateFileA(_ReplaceFile(lpFileName), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

FunctionPointer(int, PlayVoiceFile, (LPCSTR filename), 0x40CE30);
int __cdecl PlayVoiceFile_r(LPCSTR filename)
{
	filename = _ReplaceFile(filename);
	return PlayVoiceFile(filename);
}

FunctionPointer(int, PlayMusicFile, (LPCSTR filename, int loop), 0x40CD20);
int __cdecl PlayMusicFile_r(LPCSTR filename, int loop)
{
	filename = _ReplaceFile(filename);
	return PlayMusicFile(filename, loop);
}

const void *const PlayVideoFile = (const void *)0x513ED0;
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
		jmp PlayVideoFile
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

    pNewFunction = (PROC)MyGetProcAddress ;
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


int __cdecl SADXDebugOutput(const char *Format, ...)
{
	va_list ap;
	va_start(ap, Format);
	int result = vprintf(Format, ap);
	va_end(ap);
	return result;
}

FunctionPointer(char *, GetWindowClassName, (), 0x793F60);
DataPointer(int, dword_38A5DC4, 0x38A5DC4);
DataPointer(HWND, hWnd, 0x3D0FD30);
DataPointer(HINSTANCE, hInstance, 0x3D0FD34);
void CreateSADXWindow(HINSTANCE _hInstance, int nCmdShow)
{
  signed int v2; // eax@3
  DWORD v3; // esi@3
  WNDCLASSA v8; // [sp+4h] [bp-28h]@1

  v8.style = 0;
  v8.lpfnWndProc = (WNDPROC)0x789DE0;
  v8.cbClsExtra = 0;
  v8.cbWndExtra = 0;
  v8.hInstance = _hInstance;
  v8.hIcon = LoadIconA(0, MAKEINTRESOURCEA(0x7F00));
  v8.hCursor = LoadCursorA(0, MAKEINTRESOURCEA(0x7F00));
  v8.hbrBackground = (HBRUSH)GetStockObject(0);
  v8.lpszMenuName = 0;
  v8.lpszClassName = GetWindowClassName();
  if ( RegisterClassA(&v8) )
  {
    if ( dword_38A5DC4 )
    {
      v3 = 0;
      v2 = 0xC80000u;
    }
    else
    {
      v3 = 136;
      v2 = 0xC00000u;
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

string NormalizePath(string path)
{
	string pathlower = path;
	if (pathlower.length() > 2 && (pathlower[0] == '.' && pathlower[1] == '\\'))
		pathlower = pathlower.substr(2, pathlower.length() - 2);
	transform(pathlower.begin(), pathlower.end(), pathlower.begin(), ::tolower);
	return pathlower;
}

void ScanFolder(string path, int length)
{
	_WIN32_FIND_DATAA data;
	HANDLE hfind = FindFirstFileA((path + "\\*").c_str(), &data);
	if (hfind == INVALID_HANDLE_VALUE)
		return;
	do
	{
		if (data.cFileName[0] == '.')
			continue;
		else if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			ScanFolder(path + "\\" + data.cFileName, length);
		else
		{
			string filebase = path + "\\" + data.cFileName;
			transform(filebase.begin(), filebase.end(), filebase.begin(), ::tolower);
			string modfile = filebase;
			filebase = filebase.substr(length);
			string origfile = systemdir + filebase;
			char *buf = new char[modfile.length() + 1];
			if (filemap.find(origfile) != filemap.end())
				delete[] filemap[origfile];
			filemap[origfile] = buf;
			modfile.copy(buf, modfile.length());
			buf[modfile.length()] = 0;
			printf("Replaced file: \"%s\" = \"%s\"\n", origfile.c_str(), buf);
		}
	}
	while (FindNextFileA(hfind, &data) != 0);
	FindClose(hfind);
}

void __cdecl InitMods(void)
{
	chrmodelshandle = LoadLibrary(L".\\system\\CHRMODELS_orig.dll");
	if (!chrmodelshandle)
	{
		MessageBox(NULL, L"CHRMODELS_orig.dll could not be loaded!\n\nSADX will now proceed to abruptly exit.", L"SADX Mod Loader", MB_ICONERROR);
		ExitProcess(1);
	}
	ifstream str = ifstream("mods\\SADXModLoader.ini");
	if (!str.is_open())
	{
		MessageBox(NULL, L"mods\\SADXModLoader.ini could not be read!", L"SADX Mod Loader", MB_ICONWARNING);
		return;
	}
	IniDictionary ini = LoadINI(str);
	str.close();
	settings = ini[""].Element;
	char pathbuf[MAX_PATH];
	GetModuleFileNameA(NULL, pathbuf, MAX_PATH);
	string exefilename = pathbuf;
	exefilename = exefilename.substr(exefilename.find_last_of("/\\") + 1);
	transform(exefilename.begin(), exefilename.end(), exefilename.begin(), ::tolower);
	string item = settings["ShowConsole"];
	transform(item.begin(), item.end(), item.begin(), ::tolower);
	bool console = false;
	if (item == "true")
	{
		AllocConsole();
		SetConsoleTitle(L"SADX Mod Loader output");
		freopen("CONOUT$", "wb", stdout);
		console = true;
		printf("SADX Mod Loader version %d, built %s\n", ModLoaderVer, __TIMESTAMP__);
		printf("Loading mods...\n");
	}
	item = settings["ShowSADXDebugOutput"];
	transform(item.begin(), item.end(), item.begin(), ::tolower);
	if (item == "true")
		WriteJump((void *)0x401000, printf);
	item = settings["DontFixWindow"];
	transform(item.begin(), item.end(), item.begin(), ::tolower);
	if (item != "true")
		WriteJump((void *)0x789E50, sub_789E50_r);
	item = settings["DisableCDCheck"];
	transform(item.begin(), item.end(), item.begin(), ::tolower);
	if (item == "true")
		WriteJump((void *)0x402621, (void *)0x402664);
	item = settings["UseCustomResolution"];
	transform(item.begin(), item.end(), item.begin(), ::tolower);
	if (item == "true")
	{
		WriteJump((void *)0x40297A, (void *)0x402A90);
		string str = settings["HorizontalResolution"];
		if (!str.empty())
			HorizontalResolution = strtol(str.c_str(), NULL, 10);
		HorizontalStretch = HorizontalResolution / 640.0f;
		str = settings["VerticalResolution"];
		if (!str.empty())
			VerticalResolution = strtol(str.c_str(), NULL, 10);
		VerticalStretch = VerticalResolution / 480.0f;
	}
	*(void **)0x38A5DB8 = (void *)0x38A5D94; // depth buffer fix
	WriteCall((void *)0x42544C, PlayMusicFile_r);
	WriteCall((void *)0x4254F4, PlayVoiceFile_r);
	WriteCall((void *)0x425569, PlayVoiceFile_r);
	WriteCall((void *)0x513187, PlayVideoFile_r);
	DWORD oldprot;
	VirtualProtect((void *)0x7DB2A0, 0xB6D60, PAGE_WRITECOPY, &oldprot);
	char key[8];
	for (int i = 1; i < 999; i++)
	{
		sprintf_s(key, "Mod%d", i);
		if (settings.find(key) == settings.end())
			break;
		string dir = "mods\\" + settings[key];
		str = ifstream(dir + "\\mod.ini");
		if (!str.is_open())
		{
			if (console)
				printf("Could not open file mod.ini in \"mods\\%s\".\n", settings[key].c_str());
			continue;
		}
		IniDictionary modini = LoadINI(str);
		IniGroup modinfo = modini[""].Element;
		if (console)
			printf("%d. %s\n", i, modinfo["Name"].c_str());
		IniDictionary::iterator gr = modini.find("IgnoreFiles");
		if (gr != modini.end())
		{
			IniGroup replaces = gr->second.Element;
			for (IniGroup::iterator it = replaces.begin(); it != replaces.end(); it++)
			{
				filemap[NormalizePath(it->first)] = "nullfile";
				if (console)
					printf("Ignored file: %s\n", it->first.c_str());
			}
		}
		gr = modini.find("ReplaceFiles");
		if (gr != modini.end())
		{
			IniGroup replaces = gr->second.Element;
			for (IniGroup::iterator it = replaces.begin(); it != replaces.end(); it++)
				filereplaces[NormalizePath(it->first)] = NormalizePath(it->second);
		}
		gr = modini.find("SwapFiles");
		if (gr != modini.end())
		{
			IniGroup replaces = gr->second.Element;
			for (IniGroup::iterator it = replaces.begin(); it != replaces.end(); it++)
			{
				filereplaces[NormalizePath(it->first)] = NormalizePath(it->second);
				filereplaces[NormalizePath(it->second)] = NormalizePath(it->first);
			}
		}
		string sysfol = dir + "\\system";
		transform(sysfol.begin(), sysfol.end(), sysfol.begin(), ::tolower);
		if ((GetFileAttributesA(sysfol.c_str()) & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
			ScanFolder(sysfol, sysfol.length() + 1);
		if (modinfo.find("EXEFile") != modinfo.end())
		{
			string modexe = modinfo["EXEFile"];
			transform(modexe.begin(), modexe.end(), modexe.begin(), ::tolower);
			if (modexe.compare(exefilename) != 0)
			{
				const char *msg = ("Mod \"" + modinfo["Name"] + "\" should be run from \"" + modexe + "\", but you are running \"" + exefilename + "\".\n\nContinue anyway?").c_str();
				if (MessageBoxA(NULL, msg, "SADX Mod Loader", MB_ICONWARNING | MB_YESNO) == IDNO)
					ExitProcess(1);
			}
		}
		string filename = modinfo["DLLFile"];
		if (!filename.empty())
		{
			filename = dir + "\\" + filename;
			HMODULE module = LoadLibraryA(filename.c_str());
			if (module)
			{
				ModInfo *info = (ModInfo *)GetProcAddress(module, "SADXModInfo");
				if (info)
				{
					if (info->Patches)
						for (int i = 0; i < info->PatchCount; i++)
							WriteData(info->Patches[i].address, info->Patches[i].data, info->Patches[i].datasize);
					if (info->Jumps)
						for (int i = 0; i < info->JumpCount; i++)
							WriteJump(info->Jumps[i].address, info->Jumps[i].data);
					if (info->Calls)
						for (int i = 0; i < info->CallCount; i++)
							WriteCall(info->Calls[i].address, info->Calls[i].data);
					if (info->Pointers)
						for (int i = 0; i < info->PointerCount; i++)
							WriteData(info->Pointers[i].address, &info->Pointers[i].data, sizeof(void*));
					if (info->Version >= 2)
						if (info->Exports)
							for (int i = 0; i < info->ExportCount; i++)
								dataoverrides[info->Exports[i].name] = info->Exports[i].data;
					if (info->Init)
						info->Init(dir.c_str());
				}
				else if (console)
					printf("File \"%s\" is not a valid mod file.\n", filename.c_str());
			}
			else if (console)
				printf("Failed loading file \"%s\".\n", filename.c_str());
		}
	}
	printf("Finished loading mods\n");
}

BOOL APIENTRY DllMain( HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		myhandle = hModule;
		InitializeCriticalSection(&filereplacesection);
		HookTheAPI();
		WriteData((unsigned char*)0x401AE1, (unsigned char)0x90);
		WriteCall((void *)0x401AE2, InitMods);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}