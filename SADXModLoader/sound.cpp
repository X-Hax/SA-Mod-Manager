#include "stdafx.h"

#include "bass_vgmstream.h"
#include "UsercallFunctionHandler.h"

// Make sound effects use BASS instead of DirectSound

static int SEVolume = 100; // Global sound effect volume, 1-100
static bool EmulateReverb = false;
static const BASS_DX8_REVERB ReverbEffect = { 0.0f, -20.0f, 1000.0f, 0.001f };

enum MD_SOUND
{
	MD_SOUND_INIT = 0x1,
	MD_SOUND_2 = 0x2,
	MD_SOUND_AUTOSTOP = 0x10,
	MD_SOUND_20 = 0x20,
	MD_SOUND_VOL_DIST = 0x100,
	MD_SOUND_VOL_CUSTOM = 0x200,
	MD_SOUND_PAN = 0x800,
	MD_SOUND_QSOUND = 0x1000,
	MD_SOUND_PITCH = 0x2000,
	MD_SOUND_DOLBY = 0x4000
};

static constexpr int MAX_SOUND = 36;

UsercallFunc(int, dsGetVolume, (int ii), (ii), 0x4244A0, rEAX, rEAX);
UsercallFunc(BOOL, IsPlayOk, (int tone), (tone), 0x424590, rEAX, rEDX);
DataPointer(Uint8, n, 0x3B29CE8);

HSTREAM bass_channels[MAX_SOUND]{};
LPCSTR sndname = nullptr;

// Make this function use substring instead of strcmp to allow any extension
int sub_423AD0(MDHANDLE* handle, LPCSTR name)
{
	if (!handle)
	{
		return -1;
	}

	if (handle->mdnum <= 0)
	{
		return -1;
	}

	for (int i = 0; i < handle->mdnum; ++i)
	{
		if (strstr((LPCSTR)handle->md[i].nofs + 3, name + 3) != NULL)
		{
			return i;
		}
	}

	return -1;
}

// Make this function allow for any sound extension and export the sound name
static void __cdecl makesndfilename_r(int bk, int li)
{
	CHAR name[260];
	wsprintfA(name, "B%02d_00_%02d", bk, li);

	auto bank = bankhandle[bk];
	if (bank)
	{
		int id = sub_423AD0(bank, name);

		if (id >= 0 && (sndmemory = (void*)bank->md[id].fofs))
		{
			sndname = (LPCSTR)bank->md[id].nofs;
			*(int*)0x3B29AE0 = bank->md[id].fsize;
			return;
		}
	}

	sndname = nullptr;
	sndmemory = 0;
	*(int*)0x3B29AE0 = 0;
}

static void __declspec(naked) makesndfilename_asm()
{
	__asm
	{
		push [esp+04h]
		push esi
		call makesndfilename_r
		pop esi
		add esp, 4
		retn
	}
}

static void __cdecl Set3DPositionPCM_r(int ch, float x, float y, float z)
{
	auto channel = bass_channels[ch];
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

	BASS_Set3DPosition(&BASS_3DVECTOR(playertwp[0]->pos.x, playertwp[0]->pos.y, playertwp[0]->pos.z), &BASS_3DVECTOR(playermwp[0]->spd.x, playermwp[0]->spd.y, playermwp[0]->spd.z), &BASS_3DVECTOR(front.x, front.y, front.z), &BASS_3DVECTOR(top.x, top.y, top.z));
	BASS_Apply3D();

	for (int i = 0; i < MAX_SOUND; ++i)
	{
		auto dolbytwp = gpDolbyTask[i];

		if (sebuf[i].mode && (sebuf[i].mode & MD_SOUND_DOLBY) && dolbytwp)
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
		memset(gu8overlap_se, 0, gu8overlap_se.size());
		n = 0;
	}
}

static void StopBASS(int ch)
{
	auto channel = bass_channels[ch];
	if (channel)
	{
		BASS_ChannelStop(channel);
		BASS_StreamFree(channel);
		bass_channels[ch] = NULL;
	}
}

static BOOL LoadBASS(int ch, void* wavememory, int wavesize, bool dolby)
{
	if (ch >= 0 && ch < MAX_SOUND)
	{
		StopBASS(ch);
		bass_channels[ch] = BASS_VGMSTREAM_StreamCreateFromMemory((unsigned char*)wavememory, wavesize, sndname, dolby ? BASS_SAMPLE_3D : NULL);
		return bass_channels[ch] != NULL;
	}
	return FALSE;
}

static BOOL __cdecl LoadPCM_r(int ch, void* wavememory, int wavesize, int loopflag)
{
	return LoadBASS(ch, wavememory, wavesize, false);
}

static BOOL __cdecl Load3DPCM_r(int ch, void* wavememory, int wavesize, int loopflag)
{
	return LoadBASS(ch, wavememory, wavesize, true);
}

static void __cdecl Set3DMinMaxPCM_r(int ch, float min, float max)
{
	auto channel = bass_channels[ch];
	if (channel)
	{
		BASS_ChannelSet3DAttributes(channel, BASS_3DMODE_NORMAL, min, max, 0, 0, SEVolume / 100.0f);
		BASS_Apply3D();
	}
}

static void __cdecl PlayPCM_r(int ch)
{
	auto channel = bass_channels[ch];
	if (channel)
	{
		if (EmulateReverb)
		{
			HFX effect = BASS_ChannelSetFX(channel, BASS_FX_DX8_REVERB, 1);
			BASS_FXSetParameters(effect, &ReverbEffect);
		}
		BASS_ChannelPlay(channel, FALSE);
	}
}

static void __cdecl StopPCM_r(int ch)
{
	if (ch == -1)
	{
		for (int i = 0; i < MAX_SOUND; ++i)
		{
			StopBASS(i);
		}
	}
	else if (ch >= 0 && ch < MAX_SOUND)
	{
		StopBASS(ch);
	}
}

static void __cdecl IsndVolume_r(int vol, int handleno)
{
	auto channel = bass_channels[handleno];
	if (channel)
	{
		BASS_ChannelSetAttribute(channel, BASS_ATTRIB_VOL, SEVolume / 100.0f * ((float)(min(127, vol) + 127) / 254.0f));
	}
}

static void __cdecl IsndPan_r(int pan, int handleno)
{
	auto channel = bass_channels[handleno];
	if (channel)
	{
		BASS_ChannelSetAttribute(channel, BASS_ATTRIB_PAN, min(1.0f, max(-1.0f, (float)(79 * pan) / 1000.0f)));
	}
}

static void __cdecl IsndPitch_r(int pitch, int handleno)
{
	auto channel = bass_channels[handleno];
	if (channel)
	{
		BASS_CHANNELINFO info;
		BASS_ChannelGetInfo(channel, &info);
		pitch = max(100, min(100000, info.freq - (int)((float)pitch * -1.5f)));
		BASS_ChannelSetAttribute(channel, BASS_ATTRIB_FREQ, (float)pitch);
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
		auto* entry = &sebuf[i];

		if (!(entry->mode & MD_SOUND_INIT))
		{
			if (entry->timer > 0)
			{
				if (!(entry->mode & MD_SOUND_DOLBY))
				{
					if (entry->mode & (MD_SOUND_20 | MD_SOUND_2))
					{
						if (entry->mode & MD_SOUND_VOL_DIST)
						{
							auto vol = dsGetVolume(i);
							if (vol_save[i] != vol)
							{
								IsndVolume_r(vol, i);
								vol_save[i] = vol;
							}

							entry->mode &= ~MD_SOUND_2;
						}
					}

					if (entry->mode & MD_SOUND_VOL_CUSTOM)
					{
						auto vol = entry->vol;
						if (vol_save[i] != vol)
						{
							IsndVolume_r(vol, i);
							vol_save[i] = vol;
						}
					}
				}

				if (entry->mode & MD_SOUND_PAN)
				{
					IsndPan_r(entry->angle, i);
					entry->mode &= ~MD_SOUND_PAN;
				}

				if (entry->mode & MD_SOUND_PITCH)
				{
					IsndPitch_r(entry->pitch, i);
					entry->mode &= ~MD_SOUND_PITCH;
				}

				--entry->timer;
			}

			if (!entry->timer)
			{
				if (entry->mode & MD_SOUND_AUTOSTOP)
				{
					StopPCM_r(i);
				}

				entry->timer = 0;
				entry->id = 0;
				entry->mode = 0;
				entry->tone = -1;
				gpDolbyTask[i] = NULL;
			}
		}
		else
		{
			if (entry->tone <= SE_BANK_ADX)
			{
				int startid = 0;
				int bank = 0;

				// Find which bank the sound belongs to (likely inlined function)
				for (int* test = &banktbl[0]; *test >= 0; test += 2)
				{
					if (entry->tone > *test)
					{
						startid = *test;
						bank = test[1];
						break;
					}
				}

				// If the sound comes from an opponent in character fights (ie. Sonic v Knuckles), do something
				if (dsEVboss(entry->tone))
				{
					bank = (bank == 6) + 4 * (bank == 3);
				}

				if (IsPlayOk(entry->tone))
				{
					makesndfilename_r(bank, startid ? entry->tone - startid - 1 : entry->tone);
					entry->banknum = bank;

					if (entry->mode & MD_SOUND_DOLBY)
					{
						Load3DPCM_r(i, sndmemory, *(int*)0x3B29AE0, 2);
						Set3DMinMaxPCM(i, 10.0f, 500.0f);
					}
					else
					{
						LoadPCM_r(i, sndmemory, *(int*)0x3B29AE0, 2);
					}

					// Likely inlined function since in vanilla code it rechecks MD_SOUND_3D
					if (!(entry->mode & MD_SOUND_DOLBY))
					{
						if (entry->mode & MD_SOUND_VOL_DIST)
						{
							vol_save[i] = dsGetVolume(i);
						}
						else if (entry->mode & MD_SOUND_VOL_CUSTOM)
						{
							vol_save[i] = entry->vol;
						}
						else
						{
							vol_save[i] = 0;
						}

						IsndVolume_r(vol_save[i], i);
					}

					if (entry->mode & MD_SOUND_PAN)
					{
						IsndPan_r(entry->angle, i);
					}

					if (entry->mode & MD_SOUND_PITCH)
					{
						IsndPitch_r(entry->pitch, i);
					}

					PlayPCM_r(i);
				}

				entry->mode &= ~MD_SOUND_INIT;
			}
			else
			{
				PlayVoice(entry->tone + 345);
				entry->mode &= ~(MD_SOUND_INIT | MD_SOUND_AUTOSTOP);
				entry->timer = 0;
			}
		}
	}

	// Run 3D sounds (inlined in SADXPC)
	if (!snd_pause_dolby)
	{
		dsDolbySound();
	}
}

static int __cdecl dsPause_all_r()
{
	snd_pause = -1;
	snd_pause_dolby = -1;

	for (auto channel : bass_channels)
	{
		if (channel)
		{
			BASS_ChannelPause(channel);
		}
	}

	wmapause();
	return 0;
}

static int __cdecl dsRelease_all_r()
{
	snd_pause = 0;
	snd_pause_dolby = 0;

	for (auto channel : bass_channels)
	{
		if (channel)
		{
			BASS_ChannelPlay(channel, FALSE);
		}
	}

	wmaresume();
	return 0;
}

static int __cdecl dsPauseSndOnly_r()
{
	snd_pause = -1;
	snd_pause_dolby = -1;

	for (int i = 0; i < MAX_SOUND; ++i)
	{
		int tone = sebuf[i].tone;

		if (tone != SE_PAUSE && tone != SE_CURSOR && tone != SE_DECIDE)
		{
			auto channel = bass_channels[i];
			if (channel)
			{
				BASS_ChannelPause(channel);
			}
		}
	}

	return 0;
}

void Sound_Init(int sevolume)
{
#ifdef DEBUG
	PrintDebug("Initializing BASS SFX, volume: %d\n", sevolume);
#endif
	SEVolume = sevolume;
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
	WriteJump(dsPause_all, dsPause_all_r);
	WriteJump(dsRelease_all, dsRelease_all_r);
	WriteJump(dsPauseSndOnly, dsPauseSndOnly_r);
	WriteJump((void*)0x423B20, makesndfilename_asm);
}
