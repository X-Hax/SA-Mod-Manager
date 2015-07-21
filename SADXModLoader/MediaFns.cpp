/**
 * SADX Mod Loader
 * Media functions.
 */

#include "stdafx.h"
#include "MediaFns.hpp"
#include "FileReplacement.h"

#include "bass_vgmstream.h"

#include <forward_list>
#include <string>
using std::forward_list;
using std::string;

static bool enablevgmstream = false;
static bool musicwmp = true;
static DWORD basschan = 0;


/**
 * Initialize media playback.
 * Replacement for SADX's WMPInit().
 */
void WMPInit_r()
{
	enablevgmstream = BASS_Init(-1, 44100, 0, 0, NULL) ? true : false;
}

/**
 * BASS callback: Current track has ended.
 * @param handle
 * @param channel
 * @param data
 * @param user
 */
static void __stdcall onTrackEnd(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	dword_3ABDFA0 = 0;
	dword_3ABDF98 = 5;
	BASS_ChannelStop(channel);
	BASS_StreamFree(channel);
}

/**
 * Play a music file.
 * Replacement for SADX's PlayMusicFile().
 *
 * If the music file has been replaced by a VGMSTREAM-playable
 * file, BASS VGMSTREAM will be used to play the music file
 * instead of Windows Media Player.
 *
 * @param filename Filename of the music file to play.
 * @param loop If non-zero, loop the music.
 */
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
	filename = sadx_fileMap.replaceFile(filename);
	if (enablevgmstream)
	{
		basschan = BASS_VGMSTREAM_StreamCreate(filename, loop ? BASS_SAMPLE_LOOP : 0);
		if (basschan != 0)
		{
			// Stream opened!
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

/**
 * Shut down media playback.
 * Replacement for SADX's WMPRelease().
 */
void WMPRelease_r()
{
	BASS_Free();
}

__declspec(naked) int PlayVideoFile_r()
{
	__asm
	{
		mov eax, [esp + 4]
		push esi
		push eax
		call _ReplaceFile
		add esp, 4
		pop esi
		mov[esp + 4], eax
		jmp PlayVideoFilePtr
	}
}

int __cdecl PlayVoiceFile_r(LPCSTR filename)
{
	filename = sadx_fileMap.replaceFile(filename);
	return PlayVoiceFile(filename);
}
