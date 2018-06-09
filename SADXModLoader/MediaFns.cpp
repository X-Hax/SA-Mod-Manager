/**
 * SADX Mod Loader
 * Media functions.
 */

#include "stdafx.h"
#include "MediaFns.hpp"
#include "FileReplacement.h"
#include "FileSystem.h"

#include "bass_vgmstream.h"

#include <string>
#include <vector>
using std::string;
using std::vector;

static bool bassinit = false;
static bool musicwmp = true;
static bool voicewmp = true;
static DWORD basschan = 0;
static DWORD voicechan = 0;

/**
 * Initialize media playback.
 * Replacement for SADX's WMPInit().
 */
void WMPInit_r()
{
	bassinit = BASS_Init(-1, 44100, 0, nullptr, nullptr) ? true : false;
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

static void __stdcall onVoiceEnd(HSYNC handle, DWORD channel, DWORD data, void *user)
{
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
	if (bassinit)
	{
		basschan = BASS_VGMSTREAM_StreamCreate(filename, loop ? BASS_SAMPLE_LOOP : 0);
		if (basschan != 0)
		{
			// Stream opened!
			musicwmp = false;
			BASS_ChannelPlay(basschan, false);
			BASS_ChannelSetAttribute(basschan, BASS_ATTRIB_VOL, (MusicVolume + 10000) / 30000.0f);
			BASS_ChannelSetSync(basschan, BASS_SYNC_END, 0, onTrackEnd, nullptr);
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
		if (voicewmp)
		{
			WMPInfo__Pause(WMPVoiceInfo);
		}
		else
		{
			BASS_ChannelPause(voicechan);
		}
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
	if (WMPVoiceInfo)
	{
		if (voicewmp)
		{
			WMPInfo__Resume(WMPVoiceInfo);
		}
		else
		{
			BASS_ChannelPlay(voicechan, false);
		}
	}
}

void __cdecl WMPClose_r(int a1)
{
	if ( a1 )
	{
		if ( a1 == 1 && WMPVoiceInfo )
		{
			if (voicewmp)
			{	
				WMPInfo__Stop(WMPVoiceInfo);
				WMPInfo__Close(WMPVoiceInfo);
			}
			else
			{
				BASS_ChannelStop(voicechan);
				BASS_StreamFree(voicechan);
			}
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
	if (!WMPVoiceInfo)
		return 0;

	if (voicewmp)
	{
		WMPInfo__Stop(WMPVoiceInfo);
		WMPInfo__Close(WMPVoiceInfo);
	}
	else if (voicechan != 0)
	{
		BASS_ChannelStop(voicechan);
		BASS_StreamFree(voicechan);
	}

	filename = sadx_fileMap.replaceFile(filename);

	if (bassinit)
	{
		voicechan = BASS_VGMSTREAM_StreamCreate(filename, 0);
		if (voicechan != 0)
		{
			voicewmp = false;
			BASS_ChannelPlay(voicechan, false);
			BASS_ChannelSetAttribute(voicechan, BASS_ATTRIB_VOL, (VoiceVolume + 10000) / 30000.0f);
			BASS_ChannelSetSync(voicechan, BASS_SYNC_END, 0, onVoiceEnd, nullptr);
			return 1;
		}
	}

	voicewmp = true;
	return PlayVoiceFile(filename);
}

struc_64 *LoadSoundPack(const char *path, int bank)
{
	char filename[MAX_PATH];
	snprintf(filename, sizeof(filename), "%sindex.txt", path);
	const char *pidxpath = sadx_fileMap.replaceFile(filename);
	FILE *f = fopen(pidxpath, "r");
	if (!f)
		return nullptr;

	char path2[MAX_PATH];
	strncpy(path2, pidxpath, sizeof(path2));
	path2[MAX_PATH-1] = 0;
	// Find the last slash or backslash and remove everything after it.
	// (Equivalent to PathRemoveFileSpecA())
	char *bs = strrchr(path2, '\\');
	char *slash = strrchr(path2, '/');	// just in case...
	if (slash > bs)
		bs = slash;
	if (bs)
	{
		bs[1] = 0;
	}
	else
	{
		// No backslash is not good.
		// Add a backslash at the end just in case.
		strcat_s(path2, sizeof(path2), "\\");
	}

	// Total sound pack size.
	int size = 28;

	// Load the sound files.
	vector<DATEntry> entries;
	char line[MAX_PATH];
	while (!feof(f))
	{
		if (!fgets(line, sizeof(line), f))
		{
			if (feof(f)) break;
			// Read error.
			for (size_t i = 0; i < entries.size(); i++)
			{
				delete[] entries[i].NameOffset;
				delete[] entries[i].DataOffset;
			}
			fclose(f);
			return nullptr;
		}

		// Remove trailing newlines.
		int linelen = strnlen(line, sizeof(line));
		while (linelen > 0 && (line[linelen-1] == '\n' || line[linelen-1] == '\r'))
		{
			line[--linelen] = 0;
		}
		if (linelen == 0)
			continue;

		snprintf(filename, sizeof(filename), "%s%s", path2, line);
		FILE *f2 = fopen(filename, "rb");
		if (!f2)
		{
			// Unable to open a referenced file.
			for (size_t i = 0; i < entries.size(); i++)
			{
				delete[] entries[i].NameOffset;
				delete[] entries[i].DataOffset;
			}
			fclose(f);
			return nullptr;
		}

		DATEntry ent;
		ent.NameOffset = new char[14];
		snprintf(ent.NameOffset, 14, "B%02d_00_%02u.wav", bank, entries.size());
		fseek(f2, 0, SEEK_END);
		ent.DataLength = (int)ftell(f2);
		ent.DataOffset = new char[ent.DataLength];
		fseek(f2, 0, SEEK_SET);
		fread(ent.DataOffset, ent.DataLength, 1, f2);
		fclose(f2);
		entries.push_back(ent);

		// Add the entry data to the total sound pack size.
		size += sizeof(DATEntry) + 14 + ent.DataLength;
	}
	fclose(f);

	// Convert the sound pack to the format expected by SADX in memory.
	struc_64 *result = (struc_64 *)sub_4D41C0(size);
	// strncpy() zeroes out unused bytes.
	strncpy(result->ArchiveID, "archive  V2.2", sizeof(result->ArchiveID));
	result->Filename = (char *)sub_4D41C0(strlen(path) + 1);
	strncpy(result->Filename, path, strlen(path) + 1);
	result->DATFile = sub_4D41C0(4); // dummy value
	result->NumSFX = entries.size();
	memcpy(&result->DATEntries, entries.data(), entries.size() * sizeof(DATEntry));

	// Rearrange the sound pack data into a single memory block,
	// stored after result->DATEntries.
	char *ptr = reinterpret_cast<char*>(&(&result->DATEntries)[entries.size()]);
	DATEntry *ent = &result->DATEntries;
	for (int i = 0; i < result->NumSFX; i++, ent++)
	{
		memcpy(ptr, ent->NameOffset, 14);
		delete[] ent->NameOffset;
		ent->NameOffset = ptr;
		ptr += 14;

		memcpy(ptr, ent->DataOffset, ent->DataLength);
		delete[] ent->DataOffset;
		ent->DataOffset = ptr;
		ptr += ent->DataLength;
	}
	return result;
}

SoundList *SoundLists_Cust = SoundLists;
int SoundLists_Cust_Length = SoundLists_Length;
void __cdecl LoadSoundList_r(signed int soundlist)
{
	signed int v1; // esi@4
	SoundFileInfo *v2; // edi@5
	char sndpackpath[MAX_PATH];
	char String1[MAX_PATH]; // [sp+8h] [bp-108h]@7

	if (soundlist < SoundLists_Cust_Length)
	{
		if (sub_40FF10())
		{
			if (dword_38F6EC0)
			{
				v1 = 0;
				if (SoundLists_Cust[soundlist].Count)
				{
					v2 = SoundLists_Cust[soundlist].List;
					do
					{
						if (dword_3B291C8[v2->Bank])
						{
							snprintf(String1, sizeof(String1), "SYSTEM\\SoundData\\SE\\%s\\", v2->Filename);
							if (!lstrcmpiA(String1, dword_3B291C8[v2->Bank]->Filename))
							{
								++v1;
								++v2;
								continue;
							}
							snprintf(String1, sizeof(String1), "SYSTEM\\SoundData\\SE\\%s.dat", v2->Filename);
							if (!lstrcmpiA(String1, dword_3B291C8[v2->Bank]->Filename))
							{
								++v1;
								++v2;
								continue;
							}
							snprintf(String1, sizeof(String1), "%sSoundData\\SE\\%s.dat", CDPath, v2->Filename);
							if (!lstrcmpiA(String1, dword_3B291C8[v2->Bank]->Filename))
							{
								++v1;
								++v2;
								continue;
							}
							sub_423890(v2->Bank);
							sub_4B4F50(dword_3B291C8[v2->Bank]);
							dword_3B291C8[v2->Bank] = nullptr;
						}
						snprintf(String1, sizeof(String1), "SYSTEM\\SoundData\\SE\\%s.dat", v2->Filename);
						snprintf(sndpackpath, sizeof(sndpackpath), "SYSTEM\\SoundData\\SE\\%s\\", v2->Filename);
						if (sadx_fileMap.getModIndex(sndpackpath) >= sadx_fileMap.getModIndex(String1))
							dword_3B291C8[v2->Bank] = LoadSoundPack(sndpackpath, v2->Bank);
						if (!dword_3B291C8[v2->Bank])
						{
							dword_3B291C8[v2->Bank] = sub_4B4D10(String1, 1);
							if (!dword_3B291C8[v2->Bank])
							{
								snprintf(String1, sizeof(String1), "%sSoundData\\SE\\%s.dat", CDPath, v2->Filename);
								dword_3B291C8[v2->Bank] = sub_4B4D10(String1, 1);
							}
						}
						++v1;
						++v2;
					} while (v1 < SoundLists_Cust[soundlist].Count);
				}
			}
		}
	}
}
