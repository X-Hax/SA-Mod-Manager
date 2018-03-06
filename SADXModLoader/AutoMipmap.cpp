#include "stdafx.h"
#include <unordered_set>

#include "SADXModLoader.h"
#include "Trampoline.h"

#include "AutoMipmap.h"
#include "direct3d.h"

using namespace std;

namespace mipmap
{
	static Trampoline* Direct3D_PvrToTexture_t = nullptr;

	// Blacklisted PVM textures.
	// NOTE: Must be sorted alphabetically, since this array
	// is used by bsearch().
	static const char *const PVM_BLACKLIST[] = {
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
		nullptr
	};

	// Blacklisted PVR textures.
	// NOTE: Must be sorted alphabetically, since this array
	// is used by bsearch().
	static const char *const PVR_BLACKLIST[] = {
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
		nullptr
	};

	// Blacklisted global indexes.
	static unordered_set<Uint32> gbix_blacklist;

	static bool blacklisted = false;
	static bool enabled = false;

	static HRESULT copy_texture(IDirect3DTexture8* dest, IDirect3DTexture8* src, UINT height)
	{
		D3DLOCKED_RECT rectA, rectB;

		HRESULT result = src->LockRect(0, &rectA, nullptr, 0);

		if (!SUCCEEDED(result))
		{
			return result;
		}

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

	inline void set_surface(IDirect3DTexture8* src, NJS_TEXSURFACE* surface)
	{
		D3DSURFACE_DESC info;
		src->GetLevelDesc(0, &info);

		surface->pSurface = reinterpret_cast<Uint32*>(src);
		surface->TextureSize = info.Size;
	}

	static void generate_mipmaps(NJS_TEXMEMLIST* tex)
	{
		if (tex == nullptr)
		{
			return;
		}

		auto pSurface = reinterpret_cast<IDirect3DTexture8*>(tex->texinfo.texsurface.pSurface);

		if (pSurface == nullptr)
		{
			return;
		}

		if (blacklisted || pSurface->GetLevelCount() > 1)
		{
			goto ABORT;
		}

#ifndef PALLETIZED_MIPMAPS
		auto format = tex->texinfo.texsurface.PixelFormat;

		if (format == NJD_PIXELFORMAT_PALETTIZED_4BPP || format == NJD_PIXELFORMAT_PALETTIZED_8BPP)
		{
			goto ABORT;
		}
#endif

		D3DSURFACE_DESC info;
		HRESULT result = pSurface->GetLevelDesc(0, &info);

		if (!SUCCEEDED(result))
		{
			goto ABORT;
		}

		IDirect3DTexture8* dest;
		result = Direct3D_Device->CreateTexture(info.Width, info.Height, 0, info.Usage, info.Format, info.Pool, &dest);
		if (!SUCCEEDED(result))
		{
			goto ABORT;
		}

		result = copy_texture(dest, pSurface, info.Height);
		if (!SUCCEEDED(result))
		{
			goto ABORT;
		}

		result = D3DXFilterTexture(dest, nullptr, D3DX_DEFAULT, D3DX_DEFAULT);
		if (!SUCCEEDED(result))
		{
			PrintDebug("Mipmap generation failed with error code 0x%08X\n", result);
			dest->Release();
			goto ABORT;
		}

		pSurface->Release();
		set_surface(dest, &tex->texinfo.texsurface);
		return;

		// TODO: just use exceptions
	ABORT:
		set_surface(pSurface, &tex->texinfo.texsurface);
	}

	void __fastcall Direct3D_PvrToTexture_r(NJS_TEXMEMLIST *tex, IDirect3DTexture8 *surface)
	{
		auto original = static_cast<decltype(Direct3D_PvrToTexture_r)*>(Direct3D_PvrToTexture_t->Target());

		MipGuard _guard(tex != nullptr && is_blacklisted_gbix(tex->globalIndex));
		original(tex, surface);
		generate_mipmaps(tex);
	}

	void enable_auto_mipmaps()
	{
		enabled = true;

		Direct3D_PvrToTexture_t = new Trampoline(0x0078CBD0, 0x0078CBD6, Direct3D_PvrToTexture_r);

#ifdef PALLETIZED_MIPMAPS
		// This happens every frame for every palletized texture in the scene. Rather inefficient where mipmap generation is concerned.
		// Assuming somebody can figure out a method of keeping track of palette changes and textures whose mipmaps have already been generated,
		// we can enable this for mainstream builds. Otherwise it should stay off.
		WriteJump((void*)0x0078CF06, GeneratePalletizedMipmaps_asm);
#endif
	}

	/**
	 * Comparison function for bsearch().
	 * @param a Pointer to first const char*.
	 * @param b Pointer to second const char*.
	 * @return _stricmp() result.
	 */
	static int compar_stricmp(const void *a, const void *b)
	{
		const char *const str1 = *(static_cast<const char *const *>(a));
		const char *const str2 = *(static_cast<const char *const *>(b));
		return _stricmp(str1, str2);
	}

	bool is_blacklisted_pvm(const char* name)
	{
		const char *const res = static_cast<const char*>(bsearch(&name,
			PVM_BLACKLIST, LengthOfArray(PVM_BLACKLIST)-1,
			sizeof(PVM_BLACKLIST[0]), compar_stricmp));
		return (res != nullptr);
	}
	bool is_blacklisted_pvr(const char* name)
	{
		const char *const res = static_cast<const char*>(bsearch(&name,
			PVR_BLACKLIST, LengthOfArray(PVR_BLACKLIST)-1,
			sizeof(PVR_BLACKLIST[0]), compar_stricmp));
		return (res != nullptr);
	}
	bool is_blacklisted_gbix(Uint32 gbix)
	{
		return (gbix_blacklist.find(gbix) != gbix_blacklist.cend());
	}

	void blacklist_gbix(Uint32 gbix)
	{
		if (gbix_blacklist.find(gbix) == gbix_blacklist.cend())
		{
			// gbix is not blacklisted yet. Blacklist it.
			gbix_blacklist.insert(gbix);
			PrintDebug("Blacklisted GBIX %u\n", gbix);
		}
	}

	void skip_mipmap(bool value)
	{
		blacklisted = value;
	}

	bool auto_mipmaps_enabled()
	{
		return enabled;
	}

	MipGuard::MipGuard(bool skip)
	{
		last = blacklisted;
		current = skip;
		skip_mipmap(skip);
	}

	MipGuard::~MipGuard()
	{
		skip_mipmap(last);
	}

	// This has something to do with dynamic texture generation. It's used for videos and menus.
	static Sint32 __cdecl sub_77FA10_hook(Uint32 gbix, IDirect3DTexture8* texture);
	static Trampoline sub_77FA10_trampoline(0x0077FA10, 0x0077FA16, sub_77FA10_hook);
	static Sint32 __cdecl sub_77FA10_hook(Uint32 gbix, IDirect3DTexture8* texture)
	{
		MipGuard _guard(true);

		NonStaticFunctionPointer(Sint32, original, (Uint32, IDirect3DTexture8*), sub_77FA10_trampoline.Target());
		return original(gbix, texture);
	}

	// This one seems to be related to text.
	// On first call, it generates a new texture maybe. From that point on, it calls the above function.
	static void sub_40D2A0_hook(void* a1);
	static Trampoline sub_40D2A0_trampoline(0x0040D2A0, 0x0040D2A8, sub_40D2A0_hook);
	static void sub_40D2A0_hook(void* a1)
	{
		MipGuard _guard(true);

		NonStaticFunctionPointer(void, original, (void*), sub_40D2A0_trampoline.Target());
		original(a1);
	}
}
