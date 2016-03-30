#include "stdafx.h"

#include <vector>
#include <algorithm>
#include <SADXModLoader.h>
#include <Trampoline.h>

#include "AutoMipmap.h"
#include "D3DCommon.h"

namespace mipmap
{
	// TODO: Consider reading these form disk
	static const std::vector<std::string> PVMBlacklist = {
		"AL_DX_TEX_XYBUTTON",
		"AL_ENT_CHAR_E_TEX",
		"AL_ENT_CHAR_J_TEX",
		"AL_ENT_TITLE_E_TEX",
		"AL_ENT_TITLE_J_TEX",
		"AL_GARDEN00SS_2D",
		"AL_STG_KINDER_AD_TEX",
		"AL_TEX_COMMON",
		"AL_TEX_ENT_COMMON_EN",
		"AL_TEX_ENT_COMMON_FR",
		"AL_TEX_ENT_COMMON_JP",
		"AL_TEX_ODEKAKE_MENU_EN",
		"AL_TEX_ODEKAKE_MENU_JP",
		"AVA_BACK",
		"AVA_CHSEL",
		"AVA_CHSEL_E",
		"AVA_CSR",
		"AVA_DLG",
		"AVA_DLG_E",
		"AVA_EMBLEM",
		"AVA_EMBLEMVIEW",
		"AVA_EMBLEMVIEW_E",
		"AVA_EXPLAIN",
		"AVA_FCSCMN",
		"AVA_FCSCMN_E",
		"AVA_FILESEL",
		"AVA_FILESEL_E",
		"AVA_FSDLG",
		"AVA_FSDLG_E",
		"AVA_FSDLG_F",
		"AVA_FSDLG_G",
		"AVA_FSDLG_S",
		"AVA_GTITLE0",
		"AVA_GTITLE0_E",
		"AVA_GTITLE0_SHOW",
		"AVA_GTITLE0_SHOW_E",
		"AVA_METAL_SONIC",
		"AVA_NEW16NO",
		"AVA_OPTION",
		"AVA_OPTION_E",
		"AVA_SAN",
		"AVA_SNDTEST",
		"AVA_SNDTEXT_E",
		"AVA_SQUARE",
		"AVA_STNAM",
		"AVA_STNAM_E",
		"AVA_SUUJI",
		"AVA_TIIALACTSEL_E",
		"AVA_TITLE",
		"AVA_TITLE_BACK",
		"AVA_TITLE_BACK_E",
		"AVA_TITLE_CMN",
		"AVA_TITLE_CMN_SMALL",
		"AVA_TITLE_E",
		"AVA_TRIALACTSEL",
		"AVA_VMSSEL",
		"AVA_VMSSEL_E",
		"BOARD_SCORE",
		"B_CHNAM",
		"B_CHNAM_E",
		"B_CHNAM_SHOW",
		"B_CHNAM_SHOW_E",
		"CHAO_HYOUJI",
		"CHAO_HYOUJI_E",
		"CHAO_HYOUJI_F",
		"CHAO_HYOUJI_G",
		"CHAO_HYOUJI_S",
		"CON_REGULAR",
		"CON_REGULAR_E",
		"C_SELECT1",
		"C_SELECT2",
		"EXTRA",
		"GC_VMS",
		"GGMENU_TEXLIST",
		"GGMENU_TEXLIST_FR",
		"GGMENU_TEXLIST_GE",
		"GGMENU_TEXLIST_SP",
		"GGMENU_TEXLIST_US",
		"GG_TEXLIST",
		"GG_TEXLIST_FR",
		"GG_TEXLIST_GE",
		"GG_TEXLIST_SP",
		"GG_TEXLIST_US",
		"KNU_EFF",
		"K_TUTORIAL",
		"LOADING",
		"MAP_EC_A",
		"MAP_EC_B",
		"MAP_EC_H",
		"MAP_ICON",
		"MAP_MR_A",
		"MAP_MR_J",
		"MAP_MR_S",
		"MAP_PAST_E",
		"MAP_PAST_S",
		"MAP_SS",
		"MILESRACE",
		"MISSION_TUTO",
		"MIS_C_EN",
		"MIS_C_J",
		"MIS_P",
		"M_CHNAM",
		"NOWLOAD",
		"PRESSSTART",
		"SCORE_ACT",
		"SCORE_ACT_E",
		"SCORE_BACK",
		"SCORE_BOARD",
		"SCORE_BOARD_E",
		"SCORE_BOSS",
		"SCORE_BOSS_E",
		"SCORE_CART",
		"SCORE_CART_E",
		"SCORE_MOLE",
		"SCORE_MOLE_E",
		"SCORE_RESULT",
		"SCORE_RESULT_E",
		"SCORE_SHOOT",
		"SCORE_SHOOT_E",
		"SEGALOGO",
		"SEGALOGO_E",
		"SMRYBG_BIG",
		"SMRYBG_E102",
		"SMRYBG_KNUCKLES",
		"SMRYBG_SONIC",
		"SMRYBG_SUPERSONIC",
		"SMRYBG_TAILS",
		"SMRYGB_AMY",
		"SONICDX_GG_GB_JP",
		"SONICDX_GG_PARTS",
		"TGS_SOUSA_A",
		"TGS_SOUSA_B",
		"TGS_SOUSA_E",
		"TGS_SOUSA_K",
		"TGS_SOUSA_S",
		"TGS_SOUSA_T",
		"TIMEOVER",
		"TUTOMSG_AMY",
		"TUTOMSG_AMY_E",
		"TUTOMSG_AMY_F",
		"TUTOMSG_AMY_G",
		"TUTOMSG_AMY_S",
		"TUTOMSG_BIG",
		"TUTOMSG_BIG_E",
		"TUTOMSG_BIG_F",
		"TUTOMSG_BIG_G",
		"TUTOMSG_BIG_S",
		"TUTOMSG_E102",
		"TUTOMSG_E102_E",
		"TUTOMSG_E102_F",
		"TUTOMSG_E102_G",
		"TUTOMSG_E102_S",
		"TUTOMSG_KNUCKLES",
		"TUTOMSG_KNUCKLES_E",
		"TUTOMSG_KNUCKLES_F",
		"TUTOMSG_KNUCKLES_G",
		"TUTOMSG_KNUCKLES_S",
		"TUTOMSG_SONIC",
		"TUTOMSG_SONIC_E",
		"TUTOMSG_SONIC_F",
		"TUTOMSG_SONIC_G",
		"TUTOMSG_SONIC_S",
		"TUTOMSG_TAILS",
		"TUTOMSG_TAILS_E",
		"TUTOMSG_TAILS_F",
		"TUTOMSG_TAILS_G",
		"TUTOMSG_TAILS_S",
		"TUTO_CMN",
		"TUTO_CMN_E",
		"TUTO_CMN_SHOW",
		"TUTO_CMN_SHOW_E",
		"TVSETTING",
		"TX_CHNAM",
		"TX_CHNAM_E",
	};
	static const std::vector<std::string> PVRBlacklist = {
		"ABC_TXT",
		"ACTION_0",
		"ACTION_1",
		"ACTION_10",
		"ACTION_11",
		"ACTION_12",
		"ACTION_13",
		"ACTION_15",
		"ACTION_16",
		"ACTION_2",
		"ACTION_3",
		"ACTION_4",
		"ACTION_5",
		"ACTION_6",
		"ACTION_7",
		"ACTION_8",
		"ACTION_9",
		"A_STAGE01",
		"A_STAGE01_E",
		"A_STAGE02",
		"A_STAGE02_E",
		"A_STAGE03",
		"A_STAGE03_E",
		"B32ASCII",
		"B32ASCII_J",
		"BOSS_0",
		"BOSS_1",
		"BOSS_2",
		"BOSS_3",
		"BOSS_4",
		"BOSS__0",
		"BOSS__1",
		"BOSS__2",
		"BOSS__3",
		"BOSS__4",
		"B_STAGE01",
		"B_STAGE01_E",
		"B_STAGE02",
		"B_STAGE02_E",
		"B_STAGE03",
		"B_STAGE03_E",
		"B_STAGE04",
		"B_STAGE04_E",
		"E_STAGE01",
		"E_STAGE01_E",
		"E_STAGE02",
		"E_STAGE02_E",
		"E_STAGE03",
		"E_STAGE03_E",
		"E_STAGE04",
		"E_STAGE04_E",
		"E_STAGE05",
		"E_STAGE05_E",
		"HYOJI_BALLS_E",
		"HYOJI_EMBLEM0",
		"HYOJI_EMBLEM1",
		"K_STAGE01",
		"K_STAGE01_E",
		"K_STAGE02",
		"K_STAGE02_E",
		"K_STAGE03",
		"K_STAGE03_E",
		"K_STAGE04",
		"K_STAGE04_E",
		"K_STAGE05",
		"K_STAGE05_E",
		"MINIGAME_0",
		"MINIGAME_1",
		"MINIGAME_2",
		"MINIGAME_3",
		"MINIGAME_4",
		"MINIGAME_5",
		"MINIGAME_6",
		"MINIGAME_7",
		"MINIGAME_8",
		"MINIPRF01",
		"MINIPRF02",
		"MINIPRF03",
		"MINIPRF04",
		"MINIPRF05",
		"MINIPRF06",
		"MINIPRF07",
		"MINIPRF08",
		"MINIPRF09",
		"MISSION_A_AFIN",
		"MISSION_A_AHOT",
		"MISSION_A_ATWI",
		"MISSION_A_BALMIN",
		"MISSION_A_BALRING",
		"MISSION_A_BALRING_E",
		"MISSION_A_BALRING_F",
		"MISSION_A_BALRING_G",
		"MISSION_A_BALRING_S",
		"MISSION_A_BALZERO",
		"MISSION_A_BALZERO_E",
		"MISSION_A_BALZERO_F",
		"MISSION_A_BALZERO_G",
		"MISSION_A_BALZERO_S",
		"MISSION_A_FIN_E",
		"MISSION_A_FIN_F",
		"MISSION_A_FIN_G",
		"MISSION_A_FIN_S",
		"MISSION_A_HOT_E",
		"MISSION_A_HOT_F",
		"MISSION_A_HOT_G",
		"MISSION_A_HOT_S",
		"MISSION_A_TWIN_E",
		"MISSION_A_TWIN_F",
		"MISSION_A_TWIN_G",
		"MISSION_A_TWIN_S",
		"MISSION_BIG_1K",
		"MISSION_BIG_1K_E",
		"MISSION_BIG_1K_F",
		"MISSION_BIG_1K_G",
		"MISSION_BIG_1K_S",
		"MISSION_BIG_2K",
		"MISSION_BIG_2K_E",
		"MISSION_BIG_2K_F",
		"MISSION_BIG_2K_G",
		"MISSION_BIG_2K_S",
		"MISSION_BIG_FROG",
		"MISSION_BIG_FROG_E",
		"MISSION_BIG_FROG_F",
		"MISSION_BIG_FROG_G",
		"MISSION_BIG_FROG_S",
		"MISSION_G_103",
		"MISSION_G_103RING",
		"MISSION_G_103RING_E",
		"MISSION_G_103RING_F",
		"MISSION_G_103RING_G",
		"MISSION_G_103RING_S",
		"MISSION_G_103SEC",
		"MISSION_G_103_E",
		"MISSION_G_103_F",
		"MISSION_G_103_G",
		"MISSION_G_103_S",
		"MISSION_G_104",
		"MISSION_G_104RING",
		"MISSION_G_104RING_E",
		"MISSION_G_104RING_F",
		"MISSION_G_104RING_G",
		"MISSION_G_104RING_S",
		"MISSION_G_104SEC",
		"MISSION_G_104_E",
		"MISSION_G_104_F",
		"MISSION_G_104_G",
		"MISSION_G_104_S",
		"MISSION_G_105",
		"MISSION_G_105RING",
		"MISSION_G_105RING_E",
		"MISSION_G_105RING_F",
		"MISSION_G_105RING_G",
		"MISSION_G_105RING_S",
		"MISSION_G_105SEC",
		"MISSION_G_105_E",
		"MISSION_G_105_F",
		"MISSION_G_105_G",
		"MISSION_G_105_S",
		"MISSION_G_AEME",
		"MISSION_G_AFIN",
		"MISSION_G_AHOT",
		"MISSION_G_ARED",
		"MISSION_G_AWIN",
		"MISSION_G_BEME",
		"MISSION_G_BFIN",
		"MISSION_G_BHOT",
		"MISSION_G_BRED",
		"MISSION_G_BWIN",
		"MISSION_G_EME_E",
		"MISSION_G_EME_F",
		"MISSION_G_EME_G",
		"MISSION_G_EME_S",
		"MISSION_G_FIN_E",
		"MISSION_G_FIN_F",
		"MISSION_G_FIN_G",
		"MISSION_G_FIN_S",
		"MISSION_G_FROG",
		"MISSION_G_FROGRING",
		"MISSION_G_FROGRING_E",
		"MISSION_G_FROGRING_F",
		"MISSION_G_FROGRING_G",
		"MISSION_G_FROGRING_S",
		"MISSION_G_FROGSEC",
		"MISSION_G_FROG_E",
		"MISSION_G_FROG_F",
		"MISSION_G_FROG_G",
		"MISSION_G_FROG_S",
		"MISSION_G_HOT_E",
		"MISSION_G_HOT_F",
		"MISSION_G_HOT_G",
		"MISSION_G_HOT_S",
		"MISSION_G_RED_E",
		"MISSION_G_RED_F",
		"MISSION_G_RED_G",
		"MISSION_G_RED_S",
		"MISSION_G_SONICD",
		"MISSION_G_SONICDRING",
		"MISSION_G_SONICDRING_E",
		"MISSION_G_SONICDRING_F",
		"MISSION_G_SONICDRING_G",
		"MISSION_G_SONICDRING_S",
		"MISSION_G_SONICDSEC",
		"MISSION_G_SONICD_E",
		"MISSION_G_SONICD_F",
		"MISSION_G_SONICD_G",
		"MISSION_G_SONICD_S",
		"MISSION_G_WIN_E",
		"MISSION_G_WIN_F",
		"MISSION_G_WIN_G",
		"MISSION_G_WIN_S",
		"MISSION_K_1MIN",
		"MISSION_K_1MIN_E",
		"MISSION_K_1MIN_F",
		"MISSION_K_1MIN_G",
		"MISSION_K_1MIN_S",
		"MISSION_K_2MIN",
		"MISSION_K_2MIN_E",
		"MISSION_K_2MIN_F",
		"MISSION_K_2MIN_G",
		"MISSION_K_2MIN_S",
		"MISSION_K_3EME",
		"MISSION_K_3EME_E",
		"MISSION_K_3EME_F",
		"MISSION_K_3EME_G",
		"MISSION_K_3EME_S",
		"MISSION_K_NOHINT",
		"MISSION_K_NOHINT_E",
		"MISSION_K_NOHINT_F",
		"MISSION_K_NOHINT_G",
		"MISSION_K_NOHINT_S",
		"MISSION_S_2MIN",
		"MISSION_S_2MINH",
		"MISSION_S_3MIN",
		"MISSION_S_4MIN",
		"MISSION_S_4MINH",
		"MISSION_S_5MIN",
		"MISSION_S_BOX",
		"MISSION_S_BOX25MIN_E",
		"MISSION_S_BOX25MIN_F",
		"MISSION_S_BOX25MIN_G",
		"MISSION_S_BOX25MIN_S",
		"MISSION_S_BOX2MIN_E",
		"MISSION_S_BOX2MIN_F",
		"MISSION_S_BOX2MIN_G",
		"MISSION_S_BOX2MIN_S",
		"MISSION_S_BOX3MIN_E",
		"MISSION_S_BOX3MIN_F",
		"MISSION_S_BOX3MIN_G",
		"MISSION_S_BOX3MIN_S",
		"MISSION_S_BOX45MIN_E",
		"MISSION_S_BOX45MIN_F",
		"MISSION_S_BOX45MIN_G",
		"MISSION_S_BOX45MIN_S",
		"MISSION_S_BOX4MIN_E",
		"MISSION_S_BOX4MIN_F",
		"MISSION_S_BOX4MIN_G",
		"MISSION_S_BOX4MIN_S",
		"MISSION_S_BOX5MIN_E",
		"MISSION_S_BOX5MIN_F",
		"MISSION_S_BOX5MIN_G",
		"MISSION_S_BOX5MIN_S",
		"MISSION_S_BOXMIN",
		"MISSION_S_BOX_E",
		"MISSION_S_BOX_F",
		"MISSION_S_BOX_G",
		"MISSION_S_BOX_S",
		"MISSION_S_EGGC",
		"MISSION_S_EGGC_E",
		"MISSION_S_EGGC_F",
		"MISSION_S_EGGC_G",
		"MISSION_S_EGGC_S",
		"MISSION_S_EMECASINO",
		"MISSION_S_EMECASINO_E",
		"MISSION_S_EMECASINO_F",
		"MISSION_S_EMECASINO_G",
		"MISSION_S_EMECASINO_S",
		"MISSION_S_EMESNOW",
		"MISSION_S_EMESNOW_E",
		"MISSION_S_EMESNOW_F",
		"MISSION_S_EMESNOW_G",
		"MISSION_S_EMESNOW_S",
		"MISSION_S_EMEWIND",
		"MISSION_S_EMEWIND_E",
		"MISSION_S_EMEWIND_F",
		"MISSION_S_EMEWIND_G",
		"MISSION_S_EMEWIND_S",
		"MISSION_S_FEGG",
		"MISSION_S_FEGG_E",
		"MISSION_S_FEGG_F",
		"MISSION_S_FEGG_G",
		"MISSION_S_FEGG_S",
		"MISSION_S_ISEKI",
		"MISSION_S_ISEKI_E",
		"MISSION_S_ISEKI_F",
		"MISSION_S_ISEKI_G",
		"MISSION_S_ISEKI_S",
		"MISSION_S_RINGBOX",
		"MISSION_S_RINGBOX_E",
		"MISSION_S_RINGBOX_F",
		"MISSION_S_RINGBOX_G",
		"MISSION_S_RINGBOX_S",
		"MISSION_S_TAILS",
		"MISSION_S_TAILS_E",
		"MISSION_S_TAILS_F",
		"MISSION_S_TAILS_G",
		"MISSION_S_TAILS_S",
		"MISSION_T_BOX",
		"MISSION_T_BOX_E",
		"MISSION_T_BOX_F",
		"MISSION_T_BOX_G",
		"MISSION_T_BOX_S",
		"MISSION_T_EMECASINO",
		"MISSION_T_EMECASINO_E",
		"MISSION_T_EMECASINO_F",
		"MISSION_T_EMECASINO_G",
		"MISSION_T_EMECASINO_S",
		"MISSION_T_EMESNOW",
		"MISSION_T_EMESNOW_E",
		"MISSION_T_EMESNOW_F",
		"MISSION_T_EMESNOW_G",
		"MISSION_T_EMESNOW_S",
		"MISSION_T_EMEWIND",
		"MISSION_T_EMEWIND_E",
		"MISSION_T_EMEWIND_F",
		"MISSION_T_EMEWIND_G",
		"MISSION_T_EMEWIND_S",
		"MISSION_T_FASTEGG",
		"MISSION_T_FASTEGG_E",
		"MISSION_T_FASTEGG_F",
		"MISSION_T_FASTEGG_G",
		"MISSION_T_FASTEGG_S",
		"MISSION_T_FASTSONIC",
		"MISSION_T_FASTSONIC_E",
		"MISSION_T_FASTSONIC_F",
		"MISSION_T_FASTSONIC_G",
		"MISSION_T_FASTSONIC_S",
		"MISSION_T_MISS",
		"MISSION_T_MISS_E",
		"MISSION_T_MISS_F",
		"MISSION_T_MISS_G",
		"MISSION_T_MISS_S",
		"MISSION_T_RINGEGG",
		"MISSION_T_RINGEGG_E",
		"MISSION_T_RINGEGG_F",
		"MISSION_T_RINGEGG_G",
		"MISSION_T_RINGEGG_S",
		"MISSION_T_RINGSONIC",
		"MISSION_T_RINGSONIC_E",
		"MISSION_T_RINGSONIC_F",
		"MISSION_T_RINGSONIC_G",
		"MISSION_T_RINGSONIC_S",
		"M_STAGE01",
		"M_STAGE01_E",
		"M_STAGE02",
		"M_STAGE02_E",
		"M_STAGE03",
		"M_STAGE03_E",
		"M_STAGE04",
		"M_STAGE04_E",
		"M_STAGE05",
		"M_STAGE05_E",
		"SCORE_BACK",
		"SCORE_BACK0",
		"SCORE_BACK1",
		"SKY_H_BAL01",
		"SONIC_EMBLM01",
		"SONIC_EMBLM03",
		"SONIC_EMBLM04",
		"SONIC_EMBLM05",
		"STAFFROLL_TXT",
		"STF_S_LOCKMK",
		"ST_016S_HPBAR",
		"ST_064S_LOCKA",
		"ST_064S_LOCKB",
		"ST_064S_LOCKC",
		"ST_064S_SCORE",
		"ST_128S_HPGEJI",
		"ST_STAGE01",
		"ST_STAGE01_E",
		"ST_STAGE02",
		"ST_STAGE02_E",
		"ST_STAGE03",
		"ST_STAGE03_E",
		"ST_STAGE04",
		"ST_STAGE04_E",
		"ST_STAGE05",
		"ST_STAGE05_E",
		"S_STAGE01",
		"S_STAGE01_E",
		"S_STAGE02",
		"S_STAGE02_E",
		"S_STAGE03",
		"S_STAGE03_E",
		"S_STAGE04",
		"S_STAGE04_E",
		"S_STAGE05",
		"S_STAGE05_E",
		"S_STAGE06",
		"S_STAGE06_E",
		"S_STAGE07",
		"S_STAGE07_E",
		"S_STAGE08",
		"S_STAGE08_E",
		"S_STAGE09",
		"S_STAGE09_E",
		"S_STAGE10",
		"S_STAGE10_E",
		"T_EGGCARRIER",
		"T_EGGCARRIER_E",
		"T_MISTICRUIN",
		"T_MISTICRUIN_E",
		"T_STATIONSQUARE",
		"T_STATIONSQUARE_E",
	};
	static std::vector<Uint32> gbixBlacklist;

	static bool blacklisted = false;
	static bool enabled = false;

	static HRESULT CopyTexture(IDirect3DTexture8* dest, IDirect3DTexture8* src, UINT height)
	{
		HRESULT result;
		D3DLOCKED_RECT rectA, rectB;

		result = src->LockRect(0, &rectA, nullptr, 0);

		if (!SUCCEEDED(result))
			return result;

		result = dest->LockRect(0, &rectB, nullptr, 0);

		if (!SUCCEEDED(result))
		{
			src->UnlockRect(0);
			return result;
		}

		memcpy(rectB.pBits, rectA.pBits, rectA.Pitch * height);

		src->UnlockRect(0);
		dest->UnlockRect(0);

		return result;
	}

	inline void SetSurface(IDirect3DTexture8* src, NJS_TEXSURFACE* surface)
	{
		D3DSURFACE_DESC info;
		src->GetLevelDesc(0, &info);

		surface->pSurface = (Uint32*)src;
		surface->TextureSize = info.Size;
	}

	/// <summary>
	/// Generates mipmaps if the specified texture has the levels to accommodate them.
	/// </summary>
	/// <param name="src">The DirectX texture to apply mipmaps to.</param>
	/// <param name="njs_texture">The Dremcast texture to receive the DirectX texture.</param>
	static void __fastcall GenerateMipmaps_c(IDirect3DTexture8* src, NJS_TEXMEMLIST* njs_texture)
	{
		if (src == nullptr || njs_texture == nullptr)
			return;

		if (blacklisted || src->GetLevelCount() > 1)
			goto ABORT;

#ifndef PALLETIZED_MIPMAPS
		Uint32 format = njs_texture->texinfo.texsurface.PixelFormat;

		if (format == NJD_PIXELFORMAT_PALETTIZED_4BPP || format == NJD_PIXELFORMAT_PALETTIZED_8BPP)
			goto ABORT;
#endif

		HRESULT result;

		D3DSURFACE_DESC info;
		result = src->GetLevelDesc(0, &info);
		if (!SUCCEEDED(result))
			goto ABORT;

		IDirect3DTexture8* dest;
		result = Direct3D_Device->CreateTexture(info.Width, info.Height, 0, info.Usage, info.Format, info.Pool, &dest);
		if (!SUCCEEDED(result))
			goto ABORT;

		result = CopyTexture(dest, src, info.Height);
		if (!SUCCEEDED(result))
			goto ABORT;

		result = D3DXFilterTexture(dest, nullptr, D3DX_DEFAULT, D3DX_DEFAULT);
		if (!SUCCEEDED(result))
		{
			PrintDebug("Mipmap generation failed with error code 0x%08X\n", result);
			dest->Release();
			goto ABORT;
		}

		src->Release();
		SetSurface(dest, &njs_texture->texinfo.texsurface);
		return;

	ABORT:
		SetSurface(src, &njs_texture->texinfo.texsurface);
		return;
	}

#pragma region Assembly

	static void* GenerateMipmaps_return = (void*)0x0078CD37;
	static void __declspec(naked) GenerateMipmaps_asm()
	{
		// This could probably use some optimizing.
		__asm
		{
			mov		edx, esi
			mov		ecx, eax

			push	eax
			call	GenerateMipmaps_c
			pop		eax

			jmp		GenerateMipmaps_return
		}
	}

#pragma endregion

#ifdef PALLETIZED_MIPMAPS
	void __cdecl GeneratePalletizedMipmaps_c(IDirect3DTexture8* src)
	{
		if (src->GetLevelCount() >= 2)
			D3DXFilterTexture(src, nullptr, 0, D3DX_DEFAULT);
	}

	void* loc_78CE10 = (void*)0x0078CE10;
	void __declspec(naked) GeneratePalletizedMipmaps_asm()
	{
		__asm
		{
			push	edi
			call	test_c
			pop		edi
			jmp		loc_78CE10
		}
	}
#endif

	void EnableAutoMipmaps()
	{
		enabled = true;
		WriteJump((void*)0x0078CD2A, GenerateMipmaps_asm);	// Hooks the end of the function that converts PVRs to D3D textures
#ifdef PALLETIZED_MIPMAPS
		// This happens every frame for every palletized texture in the scene. Rather inefficient where mipmap generation is concerned.
		// Assuming somebody can figure out a method of keeping track of palette changes and textures who's mips have already been generated,
		// we can enable this for mainstream builds. Otherwise it should stay off.
		WriteJump((void*)0x0078CF06, GeneratePalletizedMipmaps_asm);
#endif
	}

	inline bool find(const std::vector<std::string>& v, const char* name)
	{
		if (name == nullptr)
			return false;

		std::string str(name);
		std::transform(str.begin(), str.end(), str.begin(), ::toupper);
		return std::find(v.begin(), v.end(), str.c_str()) != v.end();
	}

	bool IsBlacklistedPVR(const char* name)
	{
#ifdef _DEBUG
		if (!strcmp(name, "abc_txt"))
			PrintDebug("?!\n");
#endif

		return blacklisted = find(PVRBlacklist, name);
	}
	bool IsBlacklistedGBIX(Uint32 gbix)
	{
		return blacklisted = std::find(gbixBlacklist.begin(), gbixBlacklist.end(), gbix) != gbixBlacklist.end();
	}
	bool IsBlacklistedPVM(const char* name)
	{
		return blacklisted = find(PVMBlacklist, name);
	}

	void BlacklistGBIX(Uint32 gbix)
	{
		if (!IsBlacklistedGBIX(gbix))
		{
			gbixBlacklist.push_back(gbix);
			PrintDebug("Blacklisted GBIX %u\n", gbix);
		}
	}

	void SkipMipmap(bool value)
	{
		blacklisted = value;
	}

	bool AutoMipmapsEnabled()
	{
		return enabled;
	}

	// This has something to do with dynamic texture generation. It's used for videos and menus.
	static Sint32 __cdecl sub_77FA10_hook(Uint32 gbix, void* a2);
	static Trampoline sub_77FA10_trampoline(0x0077FA10, 0x0077FA16, (DetourFunction)sub_77FA10_hook);
	static Sint32 __cdecl sub_77FA10_hook(Uint32 gbix, void* a2)
	{
		blacklisted = true;

		FunctionPointer(Sint32, original, (Uint32, void*), sub_77FA10_trampoline.Target());
		Sint32 result = original(gbix, a2);

		blacklisted = false;
		return result;
	}

	// This one seems to be related to text.
	// On first call, it generates a new texture maybe. From that point on, it calls the above function.
	static void sub_40D2A0_hook(void* a1);
	static Trampoline sub_40D2A0_trampoline(0x0040D2A0, 0x0040D2A8, (DetourFunction)sub_40D2A0_hook);
	static void sub_40D2A0_hook(void* a1)
	{
		blacklisted = true;

		FunctionPointer(void, original, (void*), sub_40D2A0_trampoline.Target());
		original(a1);

		blacklisted = false;
	}
}
