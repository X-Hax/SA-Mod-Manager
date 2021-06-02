#include "stdafx.h"
#include "Trampoline.h"
#include "uiscale.h"
#include "bgscale.h"

using namespace uiscale;

static Trampoline* TutorialBackground_Display_t;
static Trampoline* RecapBackground_Main_t;
static Trampoline* EndBG_Display_t;
static Trampoline* DrawTiledBG_AVA_BACK_t;
static Trampoline* DrawAVA_TITLE_BACK_t;
static Trampoline* DisplayLogoScreen_t;

static void __cdecl DrawAVA_TITLE_BACK_r(float depth)
{
	scale_trampoline(Align::Align_Center, true, DrawAVA_TITLE_BACK_r, DrawAVA_TITLE_BACK_t, depth);
}

static void __cdecl DrawTiledBG_AVA_BACK_r(float depth)
{
	scale_trampoline(Align::Align_Center, true, DrawTiledBG_AVA_BACK_r, DrawTiledBG_AVA_BACK_t, depth);
}

static void __cdecl RecapBackground_Main_r(ObjectMaster* a1)
{
	scale_trampoline(Align::Align_Center, true, RecapBackground_Main_r, RecapBackground_Main_t, a1);
}

static void __cdecl DisplayLogoScreen_r(Uint8 index)
{
	scale_trampoline(Align::Align_Center, true, DisplayLogoScreen_r, DisplayLogoScreen_t, index);
}

static void __cdecl TutorialBackground_Display_r(ObjectMaster* a1)
{
	auto orig = bg_fill;
	bg_fill = FillMode_Fit;
	scale_trampoline(Align::Align_Center, true, TutorialBackground_Display_r, TutorialBackground_Display_t, a1);
	bg_fill = orig;
}

static void __cdecl EndBG_Display_r(ObjectMaster* a1)
{
	auto orig = bg_fill;
	bg_fill = FillMode_Fit;
	scale_trampoline(Align::Align_Center, true, EndBG_Display_r, EndBG_Display_t, a1);
	bg_fill = orig;
}

void bgscale::initialize()
{
	TutorialBackground_Display_t = new Trampoline(0x006436B0, 0x006436B7, TutorialBackground_Display_r);
	RecapBackground_Main_t       = new Trampoline(0x00643C90, 0x00643C95, RecapBackground_Main_r);
	EndBG_Display_t              = new Trampoline(0x006414A0, 0x006414A7, EndBG_Display_r);
	DrawTiledBG_AVA_BACK_t       = new Trampoline(0x00507BB0, 0x00507BB5, DrawTiledBG_AVA_BACK_r);
	DrawAVA_TITLE_BACK_t         = new Trampoline(0x0050BA90, 0x0050BA96, DrawAVA_TITLE_BACK_r);
	DisplayLogoScreen_t          = new Trampoline(0x0042CB20, 0x0042CB28, DisplayLogoScreen_r);
}
