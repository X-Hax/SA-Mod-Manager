/**
 * SADX Mod Loader
 * Media functions.
 */

#ifndef MEDIAFNS_HPP
#define MEDIAFNS_HPP

#include "ModLoader/MemAccess.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wmsdkidl.h>

// NOTE: mmsystem.h defines PlaySound.
// Undefine it afterwards due to an SADX function conflict.
#include <mmsystem.h>
#include <dsound.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _MSC_VER
// MinGW doesn't have IWMHeaderInfo.
struct IWMHeaderInfo;
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

int __cdecl PlayVoiceFile_r(LPCSTR filename);

/**
 * Initialize media playback.
 * Replacement for SADX's WMPInit().
 */
void WMPInit_r(void);

/**
 * Play a music file.
 * Replacement for SADX's PlayMusicFile().
 *
 * If the music file has been replaced by a vgmstream-supported
 * file, BASS vgmstream will be used to play the music file
 * instead of Windows Media Player.
 *
 * @param filename Filename of the music file to play.
 * @param loop If non-zero, loop the music.
 */
int __cdecl PlayMusicFile_r(LPCSTR filename, int loop);

void __cdecl WMPRestartMusic_r();
void __cdecl PauseSound_r();
void __cdecl ResumeSound_r();
void __cdecl WMPClose_r(int a1);

/**
 * Shut down media playback.
 * Replacement for SADX's WMPRelease().
 */
void WMPRelease_r();

int PlayVideoFile_r(void);
int __cdecl PlayVoiceFile_r(LPCSTR filename);

#ifdef __cplusplus
}
#endif

#endif /* MEDIAFNS_HPP */
