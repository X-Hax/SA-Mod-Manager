// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <deque>
#include <algorithm>
#include <DbgHelp.h>
#include <cstdio>
#include <Shlwapi.h>
#include <wmsdkidl.h>
#include <dsound.h>
#include "bass_vgmstream.h"
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
FARPROC __stdcall MyGetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	return GetProcAddress(hModule == myhandle ? chrmodelshandle : hModule, lpProcName);
}

inline int backslashes(int c)
{
	if (c == '/')
		return '\\';
	else
		return c;
}

IniGroup settings;
unordered_map<string, const char *> filemap;
const string systemdir = "system\\";
const char *_ReplaceFile(const char *lpFileName)
{
	string path = lpFileName;
	transform(path.begin(), path.end(), path.begin(), backslashes);
	if (path.length() > 2 && (path[0] == '.' && path[1] == '\\'))
		path = path.substr(2, path.length() - 2);
	transform(path.begin(), path.end(), path.begin(), ::tolower);
	unordered_map<string, const char *>::iterator fileIter = filemap.find(path);
	if (fileIter != filemap.cend())
		return fileIter->second;
	return lpFileName;
}

HANDLE __stdcall MyCreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	return CreateFileA(_ReplaceFile(lpFileName), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

int __cdecl PlayVoiceFile_r(LPCSTR filename)
{
	filename = _ReplaceFile(filename);
	return PlayVoiceFile(filename);
}

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
		strncpy_s(pathnoext, filename, MAX_PATH);
		PathRemoveExtensionA(pathnoext);
		string path = pathnoext;
		transform(path.begin(), path.end(), path.begin(), backslashes);
		if (path.length() > 2 && (path[0] == '.' && path[1] == '\\'))
			path = path.substr(2, path.length() - 2);
		transform(path.begin(), path.end(), path.begin(), ::tolower);
		for (auto i = filemap.crbegin(); i != filemap.crend(); i++)
			if (path.compare(0, path.length(), i->first, 0, path.length()) == 0)
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
	filename = _ReplaceFile(filename);
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


enum CodeType : uint8_t
{
	write8, write16, write32, writefloat,
	add8, add16, add32, addfloat,
	sub8, sub16, sub32, subfloat,
	mulu8, mulu16, mulu32, mulfloat,
	muls8, muls16, muls32,
	divu8, divu16, divu32, divfloat,
	divs8, divs16, divs32,
	modu8, modu16, modu32,
	mods8, mods16, mods32,
	shl8, shl16, shl32,
	shru8, shru16, shru32,
	shrs8, shrs16, shrs32,
	rol8, rol16, rol32,
	ror8, ror16, ror32,
	and8, and16, and32,
	or8, or16, or32,
	xor8, xor16, xor32,
	ifeq8, ifeq16, ifeq32, ifeqfloat,
	ifne8, ifne16, ifne32, ifnefloat,
	ifltu8, ifltu16, ifltu32, ifltfloat,
	iflts8, iflts16, iflts32,
	ifltequ8, ifltequ16, ifltequ32, iflteqfloat,
	iflteqs8, iflteqs16, iflteqs32,
	ifgtu8, ifgtu16, ifgtu32, ifgtfloat,
	ifgts8, ifgts16, ifgts32,
	ifgtequ8, ifgtequ16, ifgtequ32, ifgteqfloat,
	ifgteqs8, ifgteqs16, ifgteqs32,
	ifmask8, ifmask16, ifmask32,
	ifkbkey,
	readreg8, readreg16, readreg32,
	writereg8, writereg16, writereg32,
	addreg8, addreg16, addreg32, addregfloat,
	subreg8, subreg16, subreg32, subregfloat,
	mulregu8, mulregu16, mulregu32, mulregfloat,
	mulregs8, mulregs16, mulregs32,
	divregu8, divregu16, divregu32, divregfloat,
	divregs8, divregs16, divregs32,
	modregu8, modregu16, modregu32,
	modregs8, modregs16, modregs32,
	shlreg8, shlreg16, shlreg32,
	shrregu8, shrregu16, shrregu32,
	shrregs8, shrregs16, shrregs32,
	rolreg8, rolreg16, rolreg32,
	rorreg8, rorreg16, rorreg32,
	andreg8, andreg16, andreg32,
	orreg8, orreg16, orreg32,
	xorreg8, xorreg16, xorreg32,
	ifeqreg8, ifeqreg16, ifeqreg32, ifeqregfloat,
	ifnereg8, ifnereg16, ifnereg32, ifneregfloat,
	ifltregu8, ifltregu16, ifltregu32, ifltregfloat,
	ifltregs8, ifltregs16, ifltregs32,
	iflteqregu8, iflteqregu16, iflteqregu32, iflteqregfloat,
	iflteqregs8, iflteqregs16, iflteqregs32,
	ifgtregu8, ifgtregu16, ifgtregu32, ifgtregfloat,
	ifgtregs8, ifgtregs16, ifgtregs32,
	ifgteqregu8, ifgteqregu16, ifgteqregu32, ifgteqregfloat,
	ifgteqregs8, ifgteqregs16, ifgteqregs32,
	ifmaskreg8, ifmaskreg16, ifmaskreg32,
	_else,
	endif,
	newregs
};

union valuetype
{
	uint32_t u32;
	int32_t s32;
	uint16_t u16;
	int16_t s16;
	uint8_t u8;
	int8_t s8;
	float f;
};

struct Code
{
	bool newregs;
	CodeType type;
	void *address;
	bool pointer;
	int offsetcount;
	int32_t *offsets;
	valuetype value;
	uint32_t repeatcount;
	list<Code> trueCodes;
	list<Code> falseCodes;
};

list<Code> codes = list<Code>();
vector<valuetype *> registers;

void *GetAddress(Code &code, valuetype *regs)
{
	void *addr = code.address;
	if (addr < (void *)16)
		addr = &regs[(int)addr];
	if (!code.pointer)
		return addr;
	addr = *(void **)addr;
	if (code.offsetcount == 0 || addr == nullptr)
		return addr;
	for (int i = 0; i < code.offsetcount - 1; i++)
	{
		addr = (void *)((uint32_t)addr + code.offsets[i]);
		addr = *(void **)addr;
		if (addr == nullptr)
			return nullptr;
	}
	addr = (void *)((uint32_t)addr + code.offsets[code.offsetcount - 1]);
	return addr;
}

#define ifcode(size,op) for (uint32_t i = 0; i < it->repeatcount; i++) \
{ \
	cond &= *addru##size op it->value.u##size; \
	addru##size++; \
} \
	if (cond) \
	ProcessCodeList(it->trueCodes); \
else \
	ProcessCodeList(it->falseCodes);

#define ifcodes(size,op) for (uint32_t i = 0; i < it->repeatcount; i++) \
{ \
	cond &= *addrs##size op it->value.s##size; \
	addrs##size++; \
} \
	if (cond) \
	ProcessCodeList(it->trueCodes); \
else \
	ProcessCodeList(it->falseCodes);

#define ifcodef(op) for (uint32_t i = 0; i < it->repeatcount; i++) \
{ \
	cond &= *addrf op it->value.f; \
	addrf++; \
} \
	if (cond) \
	ProcessCodeList(it->trueCodes); \
else \
	ProcessCodeList(it->falseCodes);

#define ifcodereg(size,op) for (uint32_t i = 0; i < it->repeatcount; i++) \
{ \
	cond &= *addru##size op regs[it->value.u8].u##size; \
	addru##size++; \
} \
	if (cond) \
	ProcessCodeList(it->trueCodes); \
else \
	ProcessCodeList(it->falseCodes);

#define ifcoderegs(size,op) for (uint32_t i = 0; i < it->repeatcount; i++) \
{ \
	cond &= *addrs##size op regs[it->value.u8].s##size; \
	addrs##size++; \
} \
	if (cond) \
	ProcessCodeList(it->trueCodes); \
else \
	ProcessCodeList(it->falseCodes);

#define ifcoderegf(op) for (uint32_t i = 0; i < it->repeatcount; i++) \
{ \
	cond &= *addrf op regs[it->value.u8].f; \
	addrf++; \
} \
	if (cond) \
	ProcessCodeList(it->trueCodes); \
else \
	ProcessCodeList(it->falseCodes);

template<typename T>
inline void writecode(T *address, uint32_t repeatcount, T data)
{
	for (uint32_t i = 0; i < repeatcount; i++)
	{
		WriteData(address, data);
		address++;
	}
}

template<typename T>
inline void addcode(T *address, uint32_t repeatcount, T data)
{
	for (uint32_t i = 0; i < repeatcount; i++)
	{
		WriteData(address, (T)(*address + data));
		address++;
	}
}

template<typename T>
inline void subcode(T *address, uint32_t repeatcount, T data)
{
	for (uint32_t i = 0; i < repeatcount; i++)
	{
		WriteData(address, (T)(*address - data));
		address++;
	}
}

template<typename T>
inline void mulcode(T *address, uint32_t repeatcount, T data)
{
	for (uint32_t i = 0; i < repeatcount; i++)
	{
		WriteData(address, (T)(*address * data));
		address++;
	}
}

template<typename T>
inline void divcode(T *address, uint32_t repeatcount, T data)
{
	for (uint32_t i = 0; i < repeatcount; i++)
	{
		WriteData(address, (T)(*address / data));
		address++;
	}
}

template<typename T>
inline void modcode(T *address, uint32_t repeatcount, T data)
{
	for (uint32_t i = 0; i < repeatcount; i++)
	{
		WriteData(address, (T)(*address % data));
		address++;
	}
}

template<typename T>
inline void shlcode(T *address, uint32_t repeatcount, T data)
{
	for (uint32_t i = 0; i < repeatcount; i++)
	{
		WriteData(address, (T)(*address << data));
		address++;
	}
}

template<typename T>
inline void shrcode(T *address, uint32_t repeatcount, T data)
{
	for (uint32_t i = 0; i < repeatcount; i++)
	{
		WriteData(address, (T)(*address >> data));
		address++;
	}
}

template<typename T>
inline void andcode(T *address, uint32_t repeatcount, T data)
{
	for (uint32_t i = 0; i < repeatcount; i++)
	{
		WriteData(address, (T)(*address & data));
		address++;
	}
}

template<typename T>
inline void orcode(T *address, uint32_t repeatcount, T data)
{
	for (uint32_t i = 0; i < repeatcount; i++)
	{
		WriteData(address, (T)(*address | data));
		address++;
	}
}

template<typename T>
inline void xorcode(T *address, uint32_t repeatcount, T data)
{
	for (uint32_t i = 0; i < repeatcount; i++)
	{
		WriteData(address, (T)(*address ^ data));
		address++;
	}
}

int regnum;
void ProcessCodeList(list<Code> &codes)
{
	for (list<Code>::iterator it = codes.begin(); it != codes.end(); it++)
	{
		if (it->newregs)
			regnum++;
		valuetype *regs = nullptr;
		if (regnum >= 0)
			regs = registers[regnum];
		void *address = GetAddress(*it, regs);
		uint8_t *addru8 = (uint8_t *)address;
		uint16_t *addru16 = (uint16_t *)address;
		uint32_t *addru32 = (uint32_t *)address;
		float *addrf = (float *)address;
		int8_t *addrs8 = (int8_t *)address;
		int16_t *addrs16 = (int16_t *)address;
		int32_t *addrs32 = (int32_t *)address;
		if (it->type != ifkbkey && address == nullptr)
		{
			if (distance(it->falseCodes.begin(), it->falseCodes.end()) > 0)
				ProcessCodeList(it->falseCodes);
			continue;
		}
		bool cond = true;
		switch (it->type)
		{
		case write8:
			writecode(addru8, it->repeatcount, it->value.u8);
			break;
		case write16:
			writecode(addru16, it->repeatcount, it->value.u16);
			break;
		case write32:
		case writefloat:
			writecode(addru32, it->repeatcount, it->value.u32);
			break;
		case add8:
			addcode(addru8, it->repeatcount, it->value.u8);
			break;
		case add16:
			addcode(addru16, it->repeatcount, it->value.u16);
			break;
		case add32:
			addcode(addru32, it->repeatcount, it->value.u32);
			break;
		case addfloat:
			addcode(addrf, it->repeatcount, it->value.f);
			break;
		case sub8:
			subcode(addru8, it->repeatcount, it->value.u8);
			break;
		case sub16:
			subcode(addru16, it->repeatcount, it->value.u16);
			break;
		case sub32:
			subcode(addru32, it->repeatcount, it->value.u32);
			break;
		case subfloat:
			subcode(addrf, it->repeatcount, it->value.f);
			break;
		case mulu8:
			mulcode(addru8, it->repeatcount, it->value.u8);
			break;
		case mulu16:
			mulcode(addru16, it->repeatcount, it->value.u16);
			break;
		case mulu32:
			mulcode(addru32, it->repeatcount, it->value.u32);
			break;
		case mulfloat:
			mulcode(addrf, it->repeatcount, it->value.f);
			break;
		case muls8:
			mulcode(addrs8, it->repeatcount, it->value.s8);
			break;
		case muls16:
			mulcode(addrs16, it->repeatcount, it->value.s16);
			break;
		case muls32:
			mulcode(addrs32, it->repeatcount, it->value.s32);
			break;
		case divu8:
			divcode(addru8, it->repeatcount, it->value.u8);
			break;
		case divu16:
			divcode(addru16, it->repeatcount, it->value.u16);
			break;
		case divu32:
			divcode(addru32, it->repeatcount, it->value.u32);
			break;
		case divfloat:
			divcode(addrf, it->repeatcount, it->value.f);
			break;
		case divs8:
			divcode(addrs8, it->repeatcount, it->value.s8);
			break;
		case divs16:
			divcode(addrs16, it->repeatcount, it->value.s16);
			break;
		case divs32:
			divcode(addrs32, it->repeatcount, it->value.s32);
			break;
		case modu8:
			modcode(addru8, it->repeatcount, it->value.u8);
			break;
		case modu16:
			modcode(addru16, it->repeatcount, it->value.u16);
			break;
		case modu32:
			modcode(addru32, it->repeatcount, it->value.u32);
			break;
		case mods8:
			modcode(addrs8, it->repeatcount, it->value.s8);
			break;
		case mods16:
			modcode(addrs16, it->repeatcount, it->value.s16);
			break;
		case mods32:
			modcode(addrs32, it->repeatcount, it->value.s32);
			break;
		case shl8:
			shlcode(addru8, it->repeatcount, it->value.u8);
			break;
		case shl16:
			shlcode(addru16, it->repeatcount, it->value.u16);
			break;
		case shl32:
			shlcode(addru32, it->repeatcount, it->value.u32);
			break;
		case shru8:
			shrcode(addru8, it->repeatcount, it->value.u8);
			break;
		case shru16:
			shrcode(addru16, it->repeatcount, it->value.u16);
			break;
		case shru32:
			shrcode(addru32, it->repeatcount, it->value.u32);
			break;
		case shrs8:
			shrcode(addrs8, it->repeatcount, it->value.s8);
			break;
		case shrs16:
			shrcode(addrs16, it->repeatcount, it->value.s16);
			break;
		case shrs32:
			shrcode(addrs32, it->repeatcount, it->value.s32);
			break;
		case rol8:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				WriteData(addru8, (uint8_t)(_rotl8(*addru8, it->value.u8)));
				addru8++;
			}
			break;
		case rol16:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				WriteData(addru16, (uint16_t)(_rotl16(*addru16, it->value.u8)));
				addru16++;
			}
			break;
		case rol32:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				WriteData(addru32, (uint32_t)(_rotl(*addru32, it->value.s32)));
				addru32++;
			}
			break;
		case ror8:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				WriteData(addru8, (uint8_t)(_rotr8(*addru8, it->value.u8)));
				addru8++;
			}
			break;
		case ror16:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				WriteData(addru16, (uint16_t)(_rotr16(*addru16, it->value.u8)));
				addru16++;
			}
			break;
		case ror32:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				WriteData(addru32, (uint32_t)(_rotr(*addru32, it->value.s32)));
				addru32++;
			}
			break;
		case and8:
			andcode(addru8, it->repeatcount, it->value.u8);
			break;
		case and16:
			andcode(addru16, it->repeatcount, it->value.u16);
			break;
		case and32:
			andcode(addru32, it->repeatcount, it->value.u32);
			break;
		case or8:
			orcode(addru8, it->repeatcount, it->value.u8);
			break;
		case or16:
			orcode(addru16, it->repeatcount, it->value.u16);
			break;
		case or32:
			orcode(addru32, it->repeatcount, it->value.u32);
			break;
		case xor8:
			xorcode(addru8, it->repeatcount, it->value.u8);
			break;
		case xor16:
			xorcode(addru16, it->repeatcount, it->value.u16);
			break;
		case xor32:
			xorcode(addru32, it->repeatcount, it->value.u32);
			break;
		case ifeq8:
			ifcode(8,==)
				break;
		case ifeq16:
			ifcode(16,==)
				break;
		case ifeq32:
			ifcode(32,==)
				break;
		case ifeqfloat:
			ifcodef(==)
				break;
		case ifne8:
			ifcode(8,!=)
				break;
		case ifne16:
			ifcode(16,!=)
				break;
		case ifne32:
			ifcode(32,!=)
				break;
		case ifnefloat:
			ifcodef(!=)
				break;
		case ifltu8:
			ifcode(8,<)
				break;
		case ifltu16:
			ifcode(16,<)
				break;
		case ifltu32:
			ifcode(32,<)
				break;
		case ifltfloat:
			ifcodef(<)
				break;
		case iflts8:
			ifcodes(8,<)
				break;
		case iflts16:
			ifcodes(16,<)
				break;
		case iflts32:
			ifcodes(32,<)
				break;
		case ifltequ8:
			ifcode(8,<=)
				break;
		case ifltequ16:
			ifcode(16,<=)
				break;
		case ifltequ32:
			ifcode(32,<=)
				break;
		case iflteqfloat:
			ifcodef(<=)
				break;
		case iflteqs8:
			ifcodes(8,<=)
				break;
		case iflteqs16:
			ifcodes(16,<=)
				break;
		case iflteqs32:
			ifcodes(32,<=)
				break;
		case ifgtu8:
			ifcode(8,>)
				break;
		case ifgtu16:
			ifcode(16,>)
				break;
		case ifgtu32:
			ifcode(32,>)
				break;
		case ifgtfloat:
			ifcodef(>)
				break;
		case ifgts8:
			ifcodes(8,>)
				break;
		case ifgts16:
			ifcodes(16,>)
				break;
		case ifgts32:
			ifcodes(32,>)
				break;
		case ifgtequ8:
			ifcode(8,>=)
				break;
		case ifgtequ16:
			ifcode(16,>=)
				break;
		case ifgtequ32:
			ifcode(32,>=)
				break;
		case ifgteqfloat:
			ifcodef(>=)
				break;
		case ifgteqs8:
			ifcodes(8,>=)
				break;
		case ifgteqs16:
			ifcodes(16,>=)
				break;
		case ifgteqs32:
			ifcodes(32,>=)
				break;
		case ifmask8:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				cond &= (*addru8 & it->value.u8) == it->value.u8;
				addru8++;
			}
			if (cond)
				ProcessCodeList(it->trueCodes);
			else
				ProcessCodeList(it->falseCodes);
			break;
		case ifmask16:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				cond &= (*addru16 & it->value.u16) == it->value.u16;
				addru16++;
			}
			if (cond)
				ProcessCodeList(it->trueCodes);
			else
				ProcessCodeList(it->falseCodes);
			break;
		case ifmask32:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				cond &= (*addru16 & it->value.u32) == it->value.u32;
				addru32++;
			}
			if (cond)
				ProcessCodeList(it->trueCodes);
			else
				ProcessCodeList(it->falseCodes);
			break;
		case ifkbkey:
			if (GetAsyncKeyState(it->value.s32))
				ProcessCodeList(it->trueCodes);
			else
				ProcessCodeList(it->falseCodes);
			break;
		case readreg8:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				regs[it->value.u8 + i].u8 = *addru8;
				addru8++;
			}
			break;
		case readreg16:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				regs[it->value.u8 + i].u16 = *addru16;
				addru16++;
			}
			break;
		case readreg32:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				regs[it->value.u8 + i].u32 = *addru32;
				addru32++;
			}
			break;
		case writereg8:
			writecode(addru8, it->repeatcount, regs[it->value.u8].u8);
			break;
		case writereg16:
			writecode(addru16, it->repeatcount, regs[it->value.u8].u16);
			break;
		case writereg32:
			writecode(addru32, it->repeatcount, regs[it->value.u8].u32);
			break;
		case addreg8:
			addcode(addru8, it->repeatcount, regs[it->value.u8].u8);
			break;
		case addreg16:
			addcode(addru16, it->repeatcount, regs[it->value.u8].u16);
			break;
		case addreg32:
			addcode(addru32, it->repeatcount, regs[it->value.u8].u32);
			break;
		case addregfloat:
			addcode(addrf, it->repeatcount, regs[it->value.u8].f);
			break;
		case subreg8:
			subcode(addru8, it->repeatcount, regs[it->value.u8].u8);
			break;
		case subreg16:
			subcode(addru16, it->repeatcount, regs[it->value.u8].u16);
			break;
		case subreg32:
			subcode(addru32, it->repeatcount, regs[it->value.u8].u32);
			break;
		case subregfloat:
			subcode(addrf, it->repeatcount, regs[it->value.u8].f);
			break;
		case mulregu8:
			mulcode(addru8, it->repeatcount, regs[it->value.u8].u8);
			break;
		case mulregu16:
			mulcode(addru16, it->repeatcount, regs[it->value.u8].u16);
			break;
		case mulregu32:
			mulcode(addru32, it->repeatcount, regs[it->value.u8].u32);
			break;
		case mulregfloat:
			mulcode(addrf, it->repeatcount, regs[it->value.u8].f);
			break;
		case mulregs8:
			mulcode(addrs8, it->repeatcount, regs[it->value.u8].s8);
			break;
		case mulregs16:
			mulcode(addrs16, it->repeatcount, regs[it->value.u8].s16);
			break;
		case mulregs32:
			mulcode(addrs32, it->repeatcount, regs[it->value.u8].s32);
			break;
		case divregu8:
			divcode(addru8, it->repeatcount, regs[it->value.u8].u8);
			break;
		case divregu16:
			divcode(addru16, it->repeatcount, regs[it->value.u8].u16);
			break;
		case divregu32:
			divcode(addru32, it->repeatcount, regs[it->value.u8].u32);
			break;
		case divregfloat:
			divcode(addrf, it->repeatcount, regs[it->value.u8].f);
			break;
		case divregs8:
			divcode(addrs8, it->repeatcount, regs[it->value.u8].s8);
			break;
		case divregs16:
			divcode(addrs16, it->repeatcount, regs[it->value.u8].s16);
			break;
		case divregs32:
			divcode(addrs32, it->repeatcount, regs[it->value.u8].s32);
			break;
		case modregu8:
			modcode(addru8, it->repeatcount, regs[it->value.u8].u8);
			break;
		case modregu16:
			modcode(addru16, it->repeatcount, regs[it->value.u8].u16);
			break;
		case modregu32:
			modcode(addru32, it->repeatcount, regs[it->value.u8].u32);
			break;
		case modregs8:
			modcode(addrs8, it->repeatcount, regs[it->value.u8].s8);
			break;
		case modregs16:
			modcode(addrs16, it->repeatcount, regs[it->value.u8].s16);
			break;
		case modregs32:
			modcode(addrs32, it->repeatcount, regs[it->value.u8].s32);
			break;
		case shlreg8:
			shlcode(addru8, it->repeatcount, regs[it->value.u8].u8);
			break;
		case shlreg16:
			shlcode(addru16, it->repeatcount, regs[it->value.u8].u16);
			break;
		case shlreg32:
			shlcode(addru32, it->repeatcount, regs[it->value.u8].u32);
			break;
		case shrregu8:
			shrcode(addru8, it->repeatcount, regs[it->value.u8].u8);
			break;
		case shrregu16:
			shrcode(addru16, it->repeatcount, regs[it->value.u8].u16);
			break;
		case shrregu32:
			shrcode(addru32, it->repeatcount, regs[it->value.u8].u32);
			break;
		case shrregs8:
			shrcode(addrs8, it->repeatcount, regs[it->value.u8].s8);
			break;
		case shrregs16:
			shrcode(addrs16, it->repeatcount, regs[it->value.u8].s16);
			break;
		case shrregs32:
			shrcode(addrs32, it->repeatcount, regs[it->value.u8].s32);
			break;
		case rolreg8:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				WriteData(addru8, (uint8_t)(_rotl8(*addru8, regs[it->value.u8].u8)));
				addru8++;
			}
			break;
		case rolreg16:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				WriteData(addru16, (uint16_t)(_rotl16(*addru16, regs[it->value.u8].u8)));
				addru8++;
			}
			break;
		case rolreg32:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				WriteData(addru32, (uint32_t)(_rotl(*addru32, regs[it->value.u8].s32)));
				addru32++;
			}
			break;
		case rorreg8:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				WriteData(addru8, (uint8_t)(_rotr8(*addru8, regs[it->value.u8].u8)));
				addru8++;
			}
			break;
		case rorreg16:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				WriteData(addru16, (uint16_t)(_rotr16(*addru16, regs[it->value.u8].u8)));
				addru16++;
			}
			break;
		case rorreg32:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				WriteData(addru32, (uint32_t)(_rotr(*addru32, regs[it->value.u8].s32)));
				addru32++;
			}
			break;
		case andreg8:
			andcode(addru8, it->repeatcount, regs[it->value.u8].u8);
			break;
		case andreg16:
			andcode(addru16, it->repeatcount, regs[it->value.u8].u16);
			break;
		case andreg32:
			andcode(addru32, it->repeatcount, regs[it->value.u8].u32);
			break;
		case orreg8:
			orcode(addru8, it->repeatcount, regs[it->value.u8].u8);
			break;
		case orreg16:
			orcode(addru16, it->repeatcount, regs[it->value.u8].u16);
			break;
		case orreg32:
			orcode(addru32, it->repeatcount, regs[it->value.u8].u32);
			break;
		case xorreg8:
			xorcode(addru8, it->repeatcount, regs[it->value.u8].u8);
			break;
		case xorreg16:
			xorcode(addru16, it->repeatcount, regs[it->value.u8].u16);
			break;
		case xorreg32:
			xorcode(addru32, it->repeatcount, regs[it->value.u8].u32);
			break;
		case ifeqreg8:
			ifcodereg(8,==)
				break;
		case ifeqreg16:
			ifcodereg(16,==)
				break;
		case ifeqreg32:
			ifcodereg(32,==)
				break;
		case ifeqregfloat:
			ifcoderegf(==)
				break;
		case ifnereg8:
			ifcodereg(8,!=)
				break;
		case ifnereg16:
			ifcodereg(16,!=)
				break;
		case ifnereg32:
			ifcodereg(32,!=)
				break;
		case ifneregfloat:
			ifcoderegf(!=)
				break;
		case ifltregu8:
			ifcodereg(8,<)
				break;
		case ifltregu16:
			ifcodereg(16,<)
				break;
		case ifltregu32:
			ifcodereg(32,<)
				break;
		case ifltregfloat:
			ifcoderegf(<)
				break;
		case ifltregs8:
			ifcoderegs(8,<)
				break;
		case ifltregs16:
			ifcoderegs(16,<)
				break;
		case ifltregs32:
			ifcoderegs(32,<)
				break;
		case iflteqregu8:
			ifcodereg(8,<=)
				break;
		case iflteqregu16:
			ifcodereg(16,<=)
				break;
		case iflteqregu32:
			ifcodereg(32,<=)
				break;
		case iflteqregfloat:
			ifcoderegf(<=)
				break;
		case iflteqregs8:
			ifcoderegs(8,<=)
				break;
		case iflteqregs16:
			ifcoderegs(16,<=)
				break;
		case iflteqregs32:
			ifcoderegs(32,<=)
				break;
		case ifgtregu8:
			ifcodereg(8,>)
				break;
		case ifgtregu16:
			ifcodereg(16,>)
				break;
		case ifgtregu32:
			ifcodereg(32,>)
				break;
		case ifgtregfloat:
			ifcoderegf(>)
				break;
		case ifgtregs8:
			ifcoderegs(8,>)
				break;
		case ifgtregs16:
			ifcoderegs(16,>)
				break;
		case ifgtregs32:
			ifcoderegs(32,>)
				break;
		case ifgteqregu8:
			ifcodereg(8,>=)
				break;
		case ifgteqregu16:
			ifcodereg(16,>=)
				break;
		case ifgteqregu32:
			ifcodereg(32,>=)
				break;
		case ifgteqregfloat:
			ifcoderegf(>=)
				break;
		case ifgteqregs8:
			ifcoderegs(8,>=)
				break;
		case ifgteqregs16:
			ifcoderegs(16,>=)
				break;
		case ifgteqregs32:
			ifcoderegs(32,>=)
				break;
		case ifmaskreg8:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				cond &= (*addru8 & it->value.u8) == regs[it->value.u8].u8;
				addru8++;
			}
			if (cond)
				ProcessCodeList(it->trueCodes);
			else
				ProcessCodeList(it->falseCodes);
			break;
		case ifmaskreg16:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				cond &= (*addru16 & it->value.u16) == regs[it->value.u8].u16;
				addru16++;
			}
			if (cond)
				ProcessCodeList(it->trueCodes);
			else
				ProcessCodeList(it->falseCodes);
			break;
		case ifmaskreg32:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				cond &= (*addru16 & it->value.u32) == regs[it->value.u8].u32;
				addru32++;
			}
			if (cond)
				ProcessCodeList(it->trueCodes);
			else
				ProcessCodeList(it->falseCodes);
			break;
		}
	}
}

struct message { string text; uint32_t time; };

deque<message> msgqueue;

const int fadecolors[] = {
	0xF7FFFFFF,
	0xEEFFFFFF,
	0xE6FFFFFF,
	0xDDFFFFFF,
	0xD5FFFFFF,
	0xCCFFFFFF,
	0xC4FFFFFF,
	0xBBFFFFFF,
	0xB3FFFFFF,
	0xAAFFFFFF,
	0xA2FFFFFF,
	0x99FFFFFF,
	0x91FFFFFF,
	0x88FFFFFF,
	0x80FFFFFF,
	0x77FFFFFF,
	0x6FFFFFFF,
	0x66FFFFFF,
	0x5EFFFFFF,
	0x55FFFFFF,
	0x4DFFFFFF,
	0x44FFFFFF,
	0x3CFFFFFF,
	0x33FFFFFF,
	0x2BFFFFFF,
	0x22FFFFFF,
	0x1AFFFFFF,
	0x11FFFFFF,
	0x09FFFFFF,
	0
};

void __cdecl ProcessCodes()
{
	regnum = -1;
	ProcessCodeList(codes);
	unsigned int numrows = VerticalResolution / 12;
	int pos;
	if (msgqueue.size() <= numrows - 1)
		pos = (numrows - 1) - (msgqueue.size() - 1);
	else
		pos = 0;
	if (msgqueue.size() > 0)
		for (deque<message>::iterator i = msgqueue.begin(); i != msgqueue.end(); i++)
		{
			int c = -1;
			if (300 - i->time < LengthOfArray(fadecolors))
				c = fadecolors[LengthOfArray(fadecolors) - (300 - i->time) - 1];
			SetDebugTextColor(c);
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

bool dbgConsole, dbgScreen, dbgFile;
ofstream dbgstr;
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
		WriteJump((void *)0x789BD0, sub_789BD0);
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
			PrintDebug("Replaced file: \"%s\" = \"%s\"\n", origfile.c_str(), buf);
		}
	}
	while (FindNextFileA(hfind, &data) != 0);
	FindClose(hfind);
}

unsigned char ReadCodes(istream &stream, list<Code> &list)
{
	while (true)
	{
		uint8_t t = stream.get();
		if (t == 0xFF || t == _else || t == endif)
			return t;
		Code code = { };
		if (t == newregs)
		{
			code.newregs = true;
			valuetype *regs = new valuetype[16];
			memset(regs, 0, sizeof(valuetype) * 16);
			registers.push_back(regs);
			t = stream.get();
		}
		code.type = (CodeType)t;
		uintptr_t addr;
		stream.read((char *)&addr, sizeof(uintptr_t));
		code.pointer = (addr & 0x80000000u) == 0x80000000u;
		code.address = (void *)(addr & 0x7FFFFFFF);
		if (code.pointer)
		{
			code.offsetcount = stream.get();
			code.offsets = new int[code.offsetcount];
			for (int i = 0; i < code.offsetcount; i++)
				stream.read((char *)&code.offsets[i], sizeof(int32_t));
		}
		stream.read((char *)&code.value, sizeof(code.value));
		stream.read((char *)&code.repeatcount, sizeof(uint32_t));
		if ((code.type >= ifeq8 && code.type <= ifkbkey) || (code.type >= ifeqreg8 && code.type <= ifmaskreg32))
			switch (ReadCodes(stream, code.trueCodes))
		{
			case _else:
				if (ReadCodes(stream, code.falseCodes) == 0xFF)
					return 0xFF;
				break;
			case 0xFF:
				return 0xFF;
		}
		list.push_back(code);
	}
	return 0;
}

const char codemagic[] = "codev3";
void __cdecl InitMods(void)
{
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
	string item = settings["DebugConsole"];
	transform(item.begin(), item.end(), item.begin(), ::tolower);
	if (item == "true")
	{
		AllocConsole();
		SetConsoleTitle(L"SADX Mod Loader output");
		freopen("CONOUT$", "wb", stdout);
		dbgConsole = true;
	}
	item = settings["DebugScreen"];
	transform(item.begin(), item.end(), item.begin(), ::tolower);
	dbgScreen = item == "true";
	item = settings["DebugFile"];
	transform(item.begin(), item.end(), item.begin(), ::tolower);
	if (item == "true")
	{
		dbgstr = ofstream("mods\\SADXModLoader.log", ios_base::ate | ios_base::app);
		dbgFile = dbgstr.is_open();
	}
	if (dbgConsole || dbgScreen || dbgFile)
	{
		WriteJump(PrintDebug, SADXDebugOutput);
		PrintDebug("SADX Mod Loader version %d, built %s\n", ModLoaderVer, __TIMESTAMP__);
	}
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
	item = settings["WindowedFullscreen"];
	transform(item.begin(), item.end(), item.begin(), ::tolower);
	windowedfullscreen = item == "true";
	*(void **)0x38A5DB8 = (void *)0x38A5D94; // depth buffer fix
	WriteCall((void *)0x42544C, PlayMusicFile_r);
	WriteCall((void *)0x4254F4, PlayVoiceFile_r);
	WriteCall((void *)0x425569, PlayVoiceFile_r);
	WriteCall((void *)0x513187, PlayVideoFile_r);
	WriteJump((void *)0x40D1EA, WMPInit_r);
	WriteJump((void *)0x40CF50, WMPRestartMusic_r);
	WriteJump((void *)0x40D060, PauseSound_r);
	WriteJump((void *)0x40D0A0, ResumeSound_r);
	WriteJump((void *)0x40CFF0, WMPClose_r);
	WriteJump((void *)0x40D28A, WMPRelease_r);
	DWORD oldprot;
	VirtualProtect((void *)0x7DB2A0, 0xB6D60, PAGE_WRITECOPY, &oldprot);
	unordered_map<string, string> filereplaces = unordered_map<string, string>();
	char key[8];
	PrintDebug("Loading mods...\n");
	for (int i = 1; i < 999; i++)
	{
		sprintf_s(key, "Mod%d", i);
		if (settings.find(key) == settings.end())
			break;
		string dir = "mods\\" + settings[key];
		str = ifstream(dir + "\\mod.ini");
		if (!str.is_open())
		{
			PrintDebug("Could not open file mod.ini in \"mods\\%s\".\n", settings[key].c_str());
			continue;
		}
		IniDictionary modini = LoadINI(str);
		IniGroup modinfo = modini[""].Element;
		PrintDebug("%d. %s\n", i, modinfo["Name"].c_str());
		IniDictionary::iterator gr = modini.find("IgnoreFiles");
		if (gr != modini.end())
		{
			IniGroup replaces = gr->second.Element;
			for (IniGroup::iterator it = replaces.begin(); it != replaces.end(); it++)
			{
				filemap[NormalizePath(it->first)] = "nullfile";
				PrintDebug("Ignored file: %s\n", it->first.c_str());
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
					if (info->Init)
						info->Init(dir.c_str());
				}
				else
					PrintDebug("File \"%s\" is not a valid mod file.\n", filename.c_str());
			}
			else
				PrintDebug("Failed loading file \"%s\".\n", filename.c_str());
		}
	}
	for (unordered_map<string,string>::iterator it = filereplaces.begin(); it != filereplaces.end(); it++)
	{
		unordered_map<string, const char *>::iterator f = filemap.find(it->second);
		if (f != filemap.end())
			filemap[it->first] = f->second;
		else
		{
			char *buf = new char[it->second.length() + 1];
			filemap[it->first] = buf;
			it->second.copy(buf, it->second.length());
			buf[it->second.length()] = 0;
			PrintDebug("Replaced file: \"%s\" = \"%s\"\n", it->first.c_str(), buf);
		}
	}
	PrintDebug("Finished loading mods\n");
	str = ifstream("mods\\Codes.dat", ifstream::binary);
	if (str.is_open())
	{
		char buf[6];
		str.read(buf, sizeof(buf));
		if (memcmp(buf, codemagic, 6) != 0)
		{
			PrintDebug("Code file not in correct format.\n");
			goto closecodefile;
		}
		int32_t codecount;
		str.read((char *)&codecount, sizeof(int32_t));
		PrintDebug("Loading %d codes...\n", codecount);
		ReadCodes(str, codes);
	}
closecodefile:
	str.close();
	WriteJump((void *)0x426063, ProcessCodes);
}

void __cdecl LoadChrmodels(void)
{
	chrmodelshandle = LoadLibrary(L".\\system\\CHRMODELS_orig.dll");
	if (!chrmodelshandle)
	{
		MessageBox(NULL, L"CHRMODELS_orig.dll could not be loaded!\n\nSADX will now proceed to abruptly exit.", L"SADX Mod Loader", MB_ICONERROR);
		ExitProcess(1);
	}
	WriteCall((void *)0x402513, InitMods);
}

const uint8_t verchk[] = { 0x83, 0xEC, 0x28, 0x57, 0x33 };
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
		WriteCall((void *)0x401AE2, LoadChrmodels);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}