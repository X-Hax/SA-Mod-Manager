#include "stdafx.h"

#include "bass_vgmstream.h"
#include "UsercallFunctionHandler.h"

// Make sound effects use BASS instead of DirectSound

static constexpr int MAX_SOUND = 36;

auto dsGetVolume = GenerateUsercallWrapper<int(*)(int ii)>(rEAX, 0x4244A0, rEAX);
auto IsPlayOk = GenerateUsercallWrapper<BOOL(*)(int tone)>(rEAX, 0x424590, rEDX);
auto makesndfilename = GenerateUsercallWrapper<void(*)(int bk, int li)>(noret, 0x423B20, rESI, stack4);

VoidFunc(dsSoundServer, 0x4250D0);
VoidFunc(wmapause, 0x40D060);
VoidFunc(wmaresume, 0x40D0A0);
FunctionPointer(void, dsPauseAll, (), 0x424320);
FunctionPointer(void, dsReleaseAll, (), 0x424380);
FunctionPointer(BOOL, PlayPCM, (int ch), 0x410050);
FunctionPointer(BOOL, Set3DMinMaxPCM, (int ch, float _min, float _max), 0x4103B0);
FunctionPointer(BOOL, Load3DPCM, (int ch, void* wavememory, int wavesize, int loopflag), 0x410970);
FunctionPointer(BOOL, LoadPCM, (int ch, void* wavememory, int wavesize, int loopflag), 0x4106F0);
FunctionPointer(BOOL, dsEVboss, (int tone), 0x4246C0);
FunctionPointer(void, SetVolumePCM, (int ch, int volume), 0x4101A0);
FunctionPointer(void, Set3DPositionPCM, (int ch, float x, float y, float z), 0x4102C0);
FunctionPointer(void, IsndVolume, (int vol, int handleno), 0x423C20);
FunctionPointer(void, SetPanPCM, (int ch, int pan), 0x410260);
FunctionPointer(void, IsndPan, (int pan, int handleno), 0x423C80);
FunctionPointer(void, IsndPitch, (int pitch, int handleno), 0x423CC0);
FunctionPointer(void, SetFreqencyPCM, (int ch, float freqency), 0x410200);
FunctionPointer(void, StopPCM, (int ch), 0x410100);
DataArray(int, vol_save, 0x3B29C28, MAX_SOUND);
DataArray(int, banktbl, 0x910090, 64 * 2);
DataPointer(void*, sndmemory, 0x3B291C0);
DataPointer(BOOL, snd_pause, 0x3B29CE0);
DataPointer(BOOL, snd_pause_dolby, 0x3B29CE4);
DataPointer(BOOL, s_3DFlag, 0x3B0EF28);
DataPointer(uint8_t, n, 0x3B29CE8);
DataArray(uint8_t, gu8overlap_se, 0x3B292A8, MAX_SOUND * 2);

HSTREAM bass_channels[MAX_SOUND]{};

enum MD_SOUND
{
	MD_SOUND_INIT = 0x1,
	MD_SOUND_2 = 0x2,
	MD_SOUND_AUTOSTOP = 0x10,
	MD_SOUND_20 = 0x20,
	MD_SOUND_VOL_DIST = 0x100,
	MD_SOUND_VOL_CUSTOM = 0x200,
	MD_SOUND_PAN = 0x800,
	MD_SOUND_PITCH = 0x2000,
	MD_SOUND_3D = 0x4000
};

static void __cdecl Set3DPositionPCM_r(int ch, float x, float y, float z)
{
	auto& channel = bass_channels[ch];
	if (channel)
	{
		BASS_ChannelSet3DPosition(channel, &BASS_3DVECTOR(x, y, z), nullptr, nullptr);
		BASS_Apply3D();
	}
}

static void dsDolbySound()
{
	if (!camera_twp)
	{
		return;
	}

	// Update 3D sounds center
	NJS_POINT3 front = { 0.0f, 0.0f, 1.0f };
	NJS_POINT3 top = { 0.0f, 1.0f, 0.0f };

	njPushMatrix(_nj_unit_matrix_);
	if (camera_twp->ang.y)
		njRotateY(0, (unsigned __int16)LOWORD(camera_twp->ang.y));
	if (camera_twp->ang.x)
		njRotateX(0, (unsigned __int16)LOWORD(camera_twp->ang.x));
	njCalcPoint(0, &front, &front);
	njCalcPoint(0, &top, &top);
	njPopMatrixEx();

	BASS_Set3DPosition(NULL, &BASS_3DVECTOR(playermwp[0]->spd.x, playermwp[0]->spd.y, playermwp[0]->spd.z), &BASS_3DVECTOR(front.x, front.y, front.z), &BASS_3DVECTOR(top.x, top.y, top.z));
	BASS_Apply3D();

	for (int i = 0; i < MAX_SOUND; ++i)
	{
		auto& entry = sebuf[i];
		auto& dolbytwp = gpDolbyTask[i];

		if (entry.mode && (entry.mode & 0x4000) && dolbytwp)
		{
			Set3DPositionPCM_r(i, dolbytwp->pos.x, dolbytwp->pos.y, dolbytwp->pos.z);
		}
		else
		{
			dolbytwp = nullptr;
		}
	}
}

static void InitOverlapSE(uint8_t ini)
{
	if (n++ > 3ui8 || ini)
	{
		memset(gu8overlap_se, 0, sizeof(gu8overlap_se));
		n = 0;
	}
}

static void LoadBASS(int ch, void* wavememory, int wavesize, bool dolby)
{
	auto& channel = bass_channels[ch];
	if (channel)
	{
		BASS_ChannelStop(channel);
		BASS_StreamFree(channel);
	}

	channel = BASS_VGMSTREAM_StreamCreateFromMemory((unsigned char*)wavememory, wavesize, "temp.wav", dolby ? BASS_SAMPLE_3D : NULL);
}

static void __cdecl LoadPCM_r(int ch, void* wavememory, int wavesize, int loopflag)
{
	return LoadBASS(ch, wavememory, wavesize, false);
}

static void __cdecl Load3DPCM_r(int ch, void* wavememory, int wavesize, int loopflag)
{
	return LoadBASS(ch, wavememory, wavesize, true);
}

static void __cdecl Set3DMinMaxPCM_r(int ch, float min, float max)
{
	auto& channel = bass_channels[ch];
	if (channel)
	{
		// Doesn't work properly
		BASS_ChannelSet3DAttributes(channel, BASS_3DMODE_NORMAL, min, max, 360, 360, 1.0f);
		BASS_Apply3D();
	}
}

static void __cdecl PlayPCM_r(int ch)
{
	auto& channel = bass_channels[ch];
	if (channel)
	{
		BASS_ChannelPlay(channel, FALSE);
	}
}

static void __cdecl StopPCM_r(int ch)
{
	auto& channel = bass_channels[ch];
	if (channel)
	{
		BASS_ChannelStop(channel);
		BASS_StreamFree(channel);
		channel = NULL;
	}
}

static void __cdecl IsndVolume_r(int vol, int handleno)
{
	auto& channel = bass_channels[handleno];
	if (channel)
	{
		BASS_ChannelSetAttribute(channel, BASS_ATTRIB_VOL, (float)(vol + 127) / 254.0f);
	}
}

static void __cdecl IsndPan_r(int pan, int handleno)
{
	auto& channel = bass_channels[handleno];
	if (channel)
	{
		BASS_ChannelSetAttribute(channel, BASS_ATTRIB_PAN, min(1.0f, max(-1.0f, (float)(79 * pan) / 1000.0f)));
	}
}

static void __cdecl IsndPitch_r(int pitch, int handleno)
{
	auto& channel = bass_channels[handleno];
	if (channel)
	{
		pitch = min(100000, 22050 - (int)((float)pitch * -1.5f));
		if (pitch <= 100)
			pitch = 0; // original
		BASS_ChannelSetAttribute(channel, BASS_ATTRIB_FREQ, pitch);
	}
}

static void __cdecl dsSoundServer_r()
{
	if (snd_pause)
	{
		return;
	}

	// Inlined in SADXPC
	InitOverlapSE(0);

	for (int i = 0; i < MAX_SOUND; ++i)
	{
		auto& entry = sebuf[i];
		auto& dolbytwp = gpDolbyTask[i];
		auto& current_vol = vol_save[i];

		if (!(entry.mode & MD_SOUND_INIT))
		{
			if (entry.timer > 0)
			{
				if (!(entry.mode & MD_SOUND_3D))
				{
					if (entry.mode & (MD_SOUND_20 | MD_SOUND_2))
					{
						if (entry.mode & MD_SOUND_VOL_DIST)
						{
							auto vol = dsGetVolume(i);
							if (current_vol != vol)
							{
								IsndVolume_r(vol, i);
								current_vol = vol;
							}

							entry.mode &= ~MD_SOUND_2;
						}
					}

					if (entry.mode & MD_SOUND_VOL_CUSTOM)
					{
						auto vol = entry.vol;
						if (current_vol != vol)
						{
							IsndVolume_r(vol, i);
							current_vol = vol;
						}
					}
				}

				if (entry.mode & MD_SOUND_PAN)
				{
					IsndPan_r(entry.angle, i);
					entry.mode &= ~MD_SOUND_PAN;
				}

				if (entry.mode & MD_SOUND_PITCH)
				{
					IsndPitch_r(entry.pitch, i);
					entry.mode &= ~MD_SOUND_PITCH;
				}

				--entry.timer;
			}

			if (!entry.timer)
			{
				if (entry.mode & MD_SOUND_AUTOSTOP)
				{
					StopPCM_r(i);
				}

				entry.timer = 0;
				entry.id = 0;
				entry.mode = 0;
				entry.tone = -1;
				dolbytwp = nullptr;
			}
		}
		else
		{
			if (entry.tone <= SE_BANK_ADX)
			{
				int startid = 0;
				int bank = 0;

				// Find which bank the sound belongs to (likely inlined function)
				for (int* test = &banktbl[0]; *test >= 0; test += 2)
				{
					if (entry.tone > *test)
					{
						startid = *test;
						bank = test[1];
						break;
					}
				}

				// If the sound comes from an opponent in character fights (ie. Sonic v Knuckles), do something
				if (dsEVboss(entry.tone))
				{
					bank = (bank == 6) + 4 * (bank == 3);
				}

				if (IsPlayOk(entry.tone))
				{
					makesndfilename(bank, startid ? entry.tone - startid - 1 : entry.tone);
					entry.banknum = bank;

					if (entry.mode & MD_SOUND_3D)
					{
						Load3DPCM_r(i, sndmemory, *(int*)0x3B29AE0, 2);
						Set3DMinMaxPCM_r(i, 10.0f, 500.0f);
					}
					else
					{
						LoadPCM_r(i, sndmemory, *(int*)0x3B29AE0, 2);
					}

					// Likely inlined function since in vanilla code it rechecks MD_SOUND_3D
					if (!(entry.mode & MD_SOUND_3D))
					{
						if (entry.mode & MD_SOUND_VOL_DIST)
						{
							current_vol = dsGetVolume(i);
						}
						else if (entry.mode & MD_SOUND_VOL_CUSTOM)
						{
							current_vol = entry.vol;
						}
						else
						{
							current_vol = 0;
						}

						IsndVolume_r(current_vol, i);
					}

					if (entry.mode & MD_SOUND_PAN)
					{
						IsndPan_r(entry.angle, i);
					}

					if (entry.mode & MD_SOUND_PITCH)
					{
						IsndPitch_r(entry.pitch, i);
					}

					PlayPCM_r(i);
				}

				entry.mode &= ~MD_SOUND_INIT;
			}
			else
			{
				PlayVoice(entry.tone + 345);
				entry.mode &= ~(MD_SOUND_INIT | MD_SOUND_AUTOSTOP);
				entry.timer = 0;
			}
		}
	}

	// Run 3D sounds (inlined in SADXPC)
	if (s_3DFlag)
	{
		dsDolbySound();
	}
}

static void __cdecl dsPauseSndOnly_r()
{
	snd_pause = -1;
	snd_pause_dolby = -1;

	for (int i = 0; i < MAX_SOUND; ++i)
	{
		auto& channel = bass_channels[i];
		if (channel)
		{
			BASS_ChannelPause(channel);
		}
	}
}

static void __cdecl dsPauseAll_r()
{
	dsPauseSndOnly_r();
	wmapause();
}

static void __cdecl dsReleaseAll_r()
{
	snd_pause = 0;
	snd_pause_dolby = 0;

	for (int i = 0; i < MAX_SOUND; ++i)
	{
		auto& channel = bass_channels[i];
		if (channel)
		{
			BASS_ChannelPlay(channel, FALSE);
		}
	}

	wmaresume();
}

void Sound_Init()
{
	WriteJump(dsSoundServer, dsSoundServer_r);
	WriteJump(IsndPitch, IsndPitch_r);
	WriteJump(IsndPan, IsndPan_r);
	WriteJump(IsndVolume, IsndVolume_r);
	WriteJump(StopPCM, StopPCM_r);
	WriteJump(PlayPCM, PlayPCM_r);
	WriteJump(Set3DMinMaxPCM, Set3DMinMaxPCM_r);
	WriteJump(Load3DPCM, Load3DPCM_r);
	WriteJump(LoadPCM, LoadPCM_r);
	WriteJump(Set3DPositionPCM, Set3DPositionPCM_r);
	WriteJump(dsPauseSndOnly, dsPauseSndOnly_r);
	WriteJump(dsPauseAll, dsPauseAll_r);
	WriteJump(dsReleaseAll, dsReleaseAll_r);
}
