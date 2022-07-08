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

static  bool bassinit  = false;
static  bool musicwmp  = true;
static  bool voicewmp  = true;
static DWORD basschan  = 0;
static DWORD voicechan = 0;

/**
 * Initialize media playback.
 * Replacement for SADX's WMPInit().
 */
void WMPInit_r()
{
	bassinit = !!BASS_Init(-1, 44100, BASS_DEVICE_3D, nullptr, nullptr);
}

/**
 * BASS callback: Current track has ended.
 * @param handle
 * @param channel
 * @param data
 * @param user
 */
static void __stdcall onTrackEnd(HSYNC handle, DWORD channel, DWORD data, void* user)
{
	dword_3ABDFA0 = 0;
	dword_3ABDF98 = 5;
	BASS_ChannelStop(channel);
	BASS_StreamFree(channel);
}

static void __stdcall onVoiceEnd(HSYNC handle, DWORD channel, DWORD data, void* user)
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
	if (!WMPMusicInfo)
	{
		return 0;
	}

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

		if (basschan == 0)
			basschan = BASS_StreamCreateFile(false, filename, 0, 0, loop ? BASS_SAMPLE_LOOP : 0);

		if (basschan != 0)
		{
			// Stream opened!
			musicwmp = false;
			BASS_ChannelPlay(basschan, false);
			BASS_ChannelSetAttribute(basschan, BASS_ATTRIB_VOL, (MusicVolume + 10000) / 30000.0f);
			BASS_ChannelSetSync(basschan, BASS_SYNC_END, 0, onTrackEnd, nullptr);
			MusicLooping  = loop;
			dword_3ABDFA0 = 1;
			dword_3ABDF98 = 3;
			return 1;
		}
	}

	musicwmp = true;
	WCHAR WideCharStr[MAX_PATH];
	MultiByteToWideChar(0, 0, filename, -1, WideCharStr, LengthOfArray(WideCharStr));
	if (WMPMusicInfo && (WMPInfo__Open(WMPMusicInfo, WideCharStr) & 0x80000000u) == 0)
	{
		WMPInfo__Play(WMPMusicInfo, 0, 0, MusicVolume);
		MusicLooping  = loop;
		dword_3ABDFA0 = 1;
		dword_3ABDF98 = 3;

		if (WMPInfo__GetStatus(WMPMusicInfo) == WMPStatus_Stopped)
		{
			do
			{
				Sleep(0);
			} while (WMPInfo__GetStatus(WMPMusicInfo) == WMPStatus_Stopped);
		}
		return 1;
	}
	return 0;
}

void __cdecl WMPRestartMusic_r()
{
	if (!musicwmp)
	{
		return;
	}

	if (dword_3ABDFA0)
	{
		if (WMPInfo__GetStatus(WMPMusicInfo) == WMPStatus_Stopped)
		{
			if (MusicLooping)
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
			LPDIRECTSOUNDBUFFER v0 = WMPMusicInfo->DirectSoundBuffer;
			if (v0)
			{
				v0->SetVolume(MusicVolume);
			}
		}
	}
}

void __cdecl PauseMusic_r()
{
	if (dword_3ABDFA0)
	{
		++dword_3ABDFA8;
		if (musicwmp)
			WMPInfo__Pause(WMPMusicInfo);
		else
			BASS_ChannelPause(basschan);
	}
	if (WMPVoiceInfo)
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

void __cdecl ResumeMusic_r()
{
	if (dword_3ABDFA0)
	{
		--dword_3ABDFA8;
		if (dword_3ABDFA8 <= 0)
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
	if (a1)
	{
		if (a1 == 1 && WMPVoiceInfo)
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
		if (dword_3ABDFA0)
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
	{
		return 0;
	}

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

		if (voicechan == 0)
		{
			voicechan = BASS_StreamCreateFile(false, filename, 0, 0, 0);
		}

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

signed int __cdecl sub_40CF20_r()
{
	signed int result; // eax

	if (WMPVoiceInfo)
	{
		if (voicewmp)
			result = WMPInfo__GetStatus(WMPVoiceInfo) != 2 ? 3 : 5;
		else
			result = BASS_ChannelIsActive(voicechan) != BASS_ACTIVE_STOPPED ? 3 : 5;
	}
	else
	{
		result = 5;
	}
	return result;
}

struc_64* LoadSoundPack(const char* path, int bank)
{
	char filename[MAX_PATH];
	// Sound ID in the soundbank
	int soundID = 0;
	int soundCounter = 0;

	snprintf(filename, sizeof(filename), "%sindex.txt", path);
	const char* pidxpath = sadx_fileMap.replaceFile(filename);
	FILE* f = fopen(pidxpath, "r");
	if (!f)
		return nullptr;

	char path2[MAX_PATH] {};
	strncpy(path2, pidxpath, sizeof(path2));
	path2[MAX_PATH - 1] = 0;
	// Find the last slash or backslash and remove everything after it.
	// (Equivalent to PathRemoveFileSpecA())
	char* bs = strrchr(path2, '\\');
	char* slash = strrchr(path2, '/');	// just in case...

	if (slash > bs)
	{
		bs = slash;
	}

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
			if (feof(f))
			{
				break;
			}

			// Read error.
			for (auto& entrie : entries)
			{
				delete[] entrie.NameOffset;
				delete[] entrie.DataOffset;
			}

			fclose(f);
			return nullptr;
		}

		// Remove trailing newlines.
		int linelen = strnlen(line, sizeof(line));

		while (linelen > 0 && (line[linelen - 1] == '\n' || line[linelen - 1] == '\r'))
		{
			line[--linelen] = 0;
		}

		if (linelen == 0)
		{
			continue;
		}

		snprintf(filename, sizeof(filename), "%s%s", path2, line);
		FILE* fori = fopen(filename, "rb"); // Original file
		if (!fori)
		{
			PrintDebug("%s: File error\n", filename);
			// Unable to open a referenced file.
			for (auto& entrie : entries)
			{
				delete[] entrie.NameOffset;
				delete[] entrie.DataOffset;
			}

			fclose(f);
			return nullptr;
		}
		fseek(fori, 0, SEEK_END);
		int inputSize = (int)ftell(fori);
		fseek(fori, 0, SEEK_SET);
		void* InputBuffer = malloc(inputSize);
		fread(InputBuffer, 1, inputSize, fori);
		fclose(fori);
		void* vgm = BASS_VGMSTREAM_InitVGMStreamFromMemory(InputBuffer, inputSize, filename);
		if (!vgm)
		{
			PrintDebug("%s: VGMStream is null\n", filename);
			free(InputBuffer);
			fclose(f);
			return nullptr;
		}
		int outsize = BASS_VGMSTREAM_GetVGMStreamOutputSize(vgm);
		void* decodedwav = malloc(outsize);
		int converror = BASS_VGMSTREAM_ConvertVGMStreamToWav(vgm, (const char*)decodedwav);

		if (converror)
		{
			PrintDebug("%s: Format conversion error\n", filename);
			// Unable to open a referenced file.
			for (auto& entrie : entries)
			{
				delete[] entrie.NameOffset;
				delete[] entrie.DataOffset;
			}
			BASS_VGMSTREAM_CloseVGMStream(vgm);
			free(decodedwav);
			free(InputBuffer);
			fclose(f);
			return nullptr;
		}

		// Get sound name.
		std::string basename = GetBaseName(filename);
		StripExtension(basename);
		// Names starting with an underscore are ignored because there are unused files like _B02_00_15 in vanilla.
		if (basename.substr(0, 1) == "_")
			continue;
		// Put sounds with names in the B##_##_## format into correct slots.
		else if (basename.substr(0, 2) == "B0" && basename.substr(3, 2) == "_0" && basename.substr(6, 1) == "_")
		{
			soundID = std::atoi(basename.substr(7, 2).c_str());
			soundCounter++;
		}
		// Put sounds with other names according to the order they come in.
		else
		{
			soundID = soundCounter;
			soundCounter++;
		}

		// Create the sound entry.
		DATEntry ent{};
		ent.NameOffset = new char[14];
		snprintf(ent.NameOffset, 14, "B%02d_00_%02u.wav", bank, soundID);
		ent.DataLength = outsize;
		ent.DataOffset = new char[ent.DataLength];
		memcpy(ent.DataOffset, decodedwav, ent.DataLength);
		free(decodedwav);
		BASS_VGMSTREAM_CloseVGMStream(vgm);
		free(InputBuffer);
		entries.push_back(ent);

		// Add the entry data to the total sound pack size.
		size += sizeof(DATEntry) + 14 + ent.DataLength;
	}

	fclose(f);

	// Convert the sound pack to the format expected by SADX in memory.
	auto* result = (struc_64 *)_alloc(size);
	// strncpy() zeroes out unused bytes.
	strncpy(result->ArchiveID, "archive  V2.2", sizeof(result->ArchiveID));
	result->Filename = (char *)_alloc(strlen(path) + 1);
	strncpy(result->Filename, path, strlen(path) + 1);
	result->DATFile = _alloc(4); // dummy value
	result->NumSFX  = entries.size();
	memcpy(&result->DATEntries, entries.data(), entries.size() * sizeof(DATEntry));

	// Rearrange the sound pack data into a single memory block,
	// stored after result->DATEntries.
	char* ptr     = reinterpret_cast<char*>(&(&result->DATEntries)[entries.size()]);
	DATEntry* ent = &result->DATEntries;

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

struc_64* LoadSoundPackFromFile(const char* bankpath, int bank)
{
	vector<DATEntry> newentries;
	const char* path2 = sadx_fileMap.replaceFile(bankpath);

	// Load the original soundbank.
	struc_64* originalbank = MDHeaderOpen(path2, 1);
	if (!originalbank)
		return NULL;
	// Sound ID in the soundbank
	int soundID = 0;
	int soundCounter = 0;
	// Total sound pack size.
	int newsize = 28;
	int numentries = originalbank->NumSFX;

	// Convert data.
	DATEntry* entries = (DATEntry*)&originalbank->DATEntries;
	for (int i = 0; i < originalbank->NumSFX; i++)
	{		
		auto& ent = entries[i];

		// Get sound name.
		std::string basename = GetBaseName(ent.NameOffset);

		// Names starting with an underscore are ignored because there are unused files like _B02_00_15 in vanilla.
		if (basename.substr(0, 1) == "_")
			continue;

		// Get extension
		std::string ext = GetExtension(basename, true);

		// Remove extension from full name
		StripExtension(basename);
		
		// Put sounds with names in the B##_##_## format into correct slots.
		if (basename.substr(0, 2) == "B0" && basename.substr(3, 2) == "_0" && basename.substr(6, 1) == "_")
		{
			soundID = std::atoi(basename.substr(7, 2).c_str());
			soundCounter++;
		}
		// Put sounds with other names according to the order they come in.
		else
		{
			soundID = soundCounter;
			soundCounter++;
		}

		// Check for Z at the end to tell if it's from SADX 2010, if so force ADX audio.
		if (originalbank->ArchiveID[14] == 'Z') 
		{
			snprintf(ent.NameOffset, 14, "B%02d_00_%02u.ADX", bank, soundID);
		}
		// Otherwise set proper name with original file extension
		else
		{
			snprintf(ent.NameOffset, 14, "B%02d_00_%02u%s", bank, soundID, ext.c_str());
		}

		newentries.push_back(ent);

		// Add the entry data to the total sound pack size.
		newsize += sizeof(DATEntry) + 14 + ent.DataLength;
	}

	// Convert the sound pack to the format expected by SADX in memory.
	auto* result = (struc_64*)_alloc(newsize);
	// strncpy() zeroes out unused bytes.
	strncpy(result->ArchiveID, "archive  V2.2", sizeof(result->ArchiveID));
	result->Filename = (char*)_alloc(strlen(bankpath) + 1);
	strncpy(result->Filename, bankpath, strlen(bankpath) + 1);
	result->DATFile = _alloc(4); // dummy value
	result->NumSFX = newentries.size();
	memcpy(&result->DATEntries, newentries.data(), newentries.size() * sizeof(DATEntry));

	// Rearrange the sound pack data into a single memory block,
	// stored after result->DATEntries.
	char* ptr = reinterpret_cast<char*>(&(&result->DATEntries)[newentries.size()]);
	entries = &result->DATEntries;
	for (int i = 0; i < result->NumSFX; i++)
	{
		auto& ent = entries[i];

		memcpy(ptr, ent.NameOffset, 14);
		ent.NameOffset = ptr;
		ptr += 14;

		memcpy(ptr, ent.DataOffset, ent.DataLength);
		ent.DataOffset = ptr;
		ptr += ent.DataLength;
	}

	MDHeaderClose(originalbank);
	return result;
}

SoundList* SoundLists_Cust = SoundLists;
int SoundLists_Cust_Length = SoundLists.size();

void __cdecl LoadSoundList_r(signed int soundlist)
{
	char sndpackpath[MAX_PATH];
	char String1[MAX_PATH]; // [sp+8h] [bp-108h]@7

	if (soundlist < SoundLists_Cust_Length)
	{
		if (sub_40FF10())
		{
			if (dword_38F6EC0)
			{
				signed int v1 = 0;
				if (SoundLists_Cust[soundlist].Count)
				{
					SoundFileInfo* v2 = SoundLists_Cust[soundlist].List;
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
							snprintf(String1, sizeof(String1), "%sSoundData\\SE\\%s.dat", &CDPath, v2->Filename);
							if (!lstrcmpiA(String1, dword_3B291C8[v2->Bank]->Filename))
							{
								++v1;
								++v2;
								continue;
							}
							sub_423890(v2->Bank);
							MDHeaderClose(dword_3B291C8[v2->Bank]);
							dword_3B291C8[v2->Bank] = nullptr;
						}
						snprintf(String1, sizeof(String1), "SYSTEM\\SoundData\\SE\\%s.dat", v2->Filename);
						snprintf(sndpackpath, sizeof(sndpackpath), "SYSTEM\\SoundData\\SE\\%s\\", v2->Filename);
						if (sadx_fileMap.getModIndex(sndpackpath) >= sadx_fileMap.getModIndex(String1))
							dword_3B291C8[v2->Bank] = LoadSoundPack(sndpackpath, v2->Bank);
						if (!dword_3B291C8[v2->Bank])
						{
							dword_3B291C8[v2->Bank] = LoadSoundPackFromFile(String1, v2->Bank);
							if (!dword_3B291C8[v2->Bank])
							{
								snprintf(String1, sizeof(String1), "%sSoundData\\SE\\%s.dat", &CDPath, v2->Filename);
								dword_3B291C8[v2->Bank] = LoadSoundPackFromFile(String1, v2->Bank);
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

int __cdecl PlayMusicFile_CD_r(LPCSTR filename, int loop)
{
	if (!PlayMusicFile_r(filename, loop))
	{
		std::string newfilename = filename;
		ReplaceFileExtension(newfilename, ".adx");
		newfilename = (const char*)CDPath + newfilename;
			return PlayMusicFile_r(newfilename.c_str(), loop);
	}
	return 0;
}

int __cdecl PlayVoiceFile_CD_r(LPCSTR filename)
{
	if (!PlayVoiceFile_r(filename))
	{
		std::string newfilename = filename;
		ReplaceFileExtension(newfilename, ".adx");
		newfilename = (const char*)CDPath + newfilename;
		return PlayVoiceFile_r(newfilename.c_str());
	}
	return 0;
}