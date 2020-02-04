#include "stdafx.h"
#include "Trampoline.h"
#include "uiscale.h"
#include "hudscale.h"

using namespace uiscale;

static const float patch_dummy = 1.0f;

#pragma region Trampolines

static Trampoline* HudDisplayRingTimeLife_Check_t;
static Trampoline* HudDisplayScoreOrTimer_t;
static Trampoline* DrawStageMissionImage_t;
static Trampoline* DisplayPauseMenu_t;
static Trampoline* LifeGauge_Main_t;
static Trampoline* scaleScoreA;
static Trampoline* scaleTornadoHP;
static Trampoline* scaleTwinkleCircuitHUD;
static Trampoline* FishingHud_DrawHIT_t;
static Trampoline* FishingHud_DrawReel_t;
static Trampoline* FishingHud_DrawRod_t;
static Trampoline* BigHud_DrawWeightAndLife_t;
static Trampoline* FishingHud_DrawMeters_t;
static Trampoline* scaleAnimalPickup;
static Trampoline* scaleItemBoxSprite;
static Trampoline* scaleBalls;
static Trampoline* scaleCheckpointTime;
static Trampoline* scaleEmeraldRadarA;
static Trampoline* scaleEmeraldRadar_Grab;
static Trampoline* scaleEmeraldRadarB;
static Trampoline* scaleSandHillMultiplier;
static Trampoline* scaleIceCapMultiplier;
static Trampoline* scaleGammaTimeAddHud;
static Trampoline* scaleGammaTimeRemaining;
static Trampoline* scaleEmblemScreen;
static Trampoline* scaleBossName;
static Trampoline* scaleNightsCards;
static Trampoline* scaleNightsJackpot;
static Trampoline* scaleMissionStartClear;
static Trampoline* scaleMissionTimer;
static Trampoline* scaleMissionCounter;
static Trampoline* scaleTailsWinLose;
static Trampoline* scaleTailsRaceBar;
static Trampoline* scaleDemoPressStart;
static Trampoline* ChaoDX_Message_PlayerAction_Load_t;
static Trampoline* ChaoDX_Message_PlayerAction_Display_t;
static Trampoline* MissionCompleteScreen_Draw_t;
static Trampoline* CharSelBg_Display_t;
static Trampoline* TrialLevelList_Display_t;
static Trampoline* SubGameLevelList_Display_t;
static Trampoline* EmblemResultMenu_Display_t;
static Trampoline* FileSelect_Display_t;
static Trampoline* MenuObj_Display_t;
static Trampoline* OptionsMenu_Display_t;
static Trampoline* SoundTest_Display_t;
static Trampoline* GreenMenuRect_Draw_t;
static Trampoline* TutorialInstructionOverlay_Display_t;
static Trampoline* DisplayTitleCard_t;
static Trampoline* Credits_Main_t;
static Trampoline* PauseMenu_Map_Display_t;
static Trampoline* DrawSubtitles_t;
static Trampoline* EmblemCollected_Init_t;
static Trampoline* EmblemCollected_Main_t;
static Trampoline* DrawTitleScreen_t;

#pragma endregion

static void __cdecl scale_result_screen(ObjectMaster* _this)
{
	scale_push(Align::center, false);
	ScoreDisplay_Main(_this);
	scale_pop();
}

static void __cdecl HudDisplayRingTimeLife_Check_r()
{
	scale_trampoline(Align::automatic, false, HudDisplayRingTimeLife_Check_r, HudDisplayRingTimeLife_Check_t);
}

static void __cdecl HudDisplayScoreOrTimer_r()
{
	scale_trampoline(Align::left, false, HudDisplayScoreOrTimer_r, HudDisplayScoreOrTimer_t);
}

static void __cdecl DrawStageMissionImage_r(ObjectMaster* _this)
{
	scale_trampoline(Align::center, false, DrawStageMissionImage_r, DrawStageMissionImage_t, _this);
}

static short __cdecl DisplayPauseMenu_r()
{
	return scale_trampoline<short>(Align::center, false, DisplayPauseMenu_r, DisplayPauseMenu_t);
}

static void __cdecl LifeGauge_Main_r(ObjectMaster* a1)
{
	scale_trampoline(Align::right, false, LifeGauge_Main_r, LifeGauge_Main_t, a1);
}

static void __cdecl ScaleScoreA()
{
	scale_trampoline(Align::left, false, ScaleScoreA, scaleScoreA);
}

static void __cdecl ScaleTornadoHP(ObjectMaster* a1)
{
	scale_trampoline(Align::left | Align::bottom, false, ScaleTornadoHP, scaleTornadoHP, a1);
}

static void __cdecl ScaleTwinkleCircuitHUD(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, ScaleTwinkleCircuitHUD, scaleTwinkleCircuitHUD, a1);
}

static void __cdecl FishingHud_DrawHIT_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, FishingHud_DrawHIT_r, FishingHud_DrawHIT_t, a1);
}

static void __cdecl FishingHud_DrawReel_r()
{
	scale_trampoline(Align::right | Align::bottom, false, FishingHud_DrawReel_r, FishingHud_DrawReel_t);
}
static void __cdecl FishingHud_DrawRod_r()
{
	scale_trampoline(Align::right | Align::bottom, false, FishingHud_DrawRod_r, FishingHud_DrawRod_t);
}

static void __cdecl BigHud_DrawWeightAndLife_r(ObjectMaster* a1)
{
	scale_trampoline(Align::automatic, false, BigHud_DrawWeightAndLife_r, BigHud_DrawWeightAndLife_t, a1);
}
static void __cdecl FishingHud_DrawMeters_r(float length)
{
	scale_trampoline(Align::right | Align::bottom, false, FishingHud_DrawMeters_r, FishingHud_DrawMeters_t, length);
}

static void __cdecl ScaleAnimalPickup(ObjectMaster* a1)
{
	scale_trampoline(Align::right | Align::bottom, false, ScaleAnimalPickup, scaleAnimalPickup, a1);
}

static void __cdecl ScaleItemBoxSprite(ObjectMaster* a1)
{
	scale_trampoline(Align::bottom | Align::horizontal_center, false, ScaleItemBoxSprite, scaleItemBoxSprite, a1);
}

static void __cdecl ScaleBalls(ObjectMaster* a1)
{
	scale_trampoline(Align::right, false, ScaleBalls, scaleBalls, a1);
}

static void __cdecl ScaleCheckpointTime(int a1, int a2, int a3)
{
	scale_trampoline(Align::right | Align::bottom, false, ScaleCheckpointTime, scaleCheckpointTime, a1, a2, a3);
}

static void __cdecl ScaleEmeraldRadarA(ObjectMaster* a1)
{
	scale_trampoline(Align::automatic, false, ScaleEmeraldRadarA, scaleEmeraldRadarA, a1);
}

static void __cdecl ScaleEmeraldRadar_Grab(ObjectMaster* a1)
{
	scale_trampoline(Align::automatic, false, ScaleEmeraldRadar_Grab, scaleEmeraldRadar_Grab, a1);
}

static void __cdecl ScaleEmeraldRadarB(ObjectMaster* a1)
{
	scale_trampoline(Align::automatic, false, ScaleEmeraldRadarB, scaleEmeraldRadarB, a1);
}

static void __cdecl ScaleSandHillMultiplier(ObjectMaster* a1)
{
	scale_trampoline(Align::automatic, false, ScaleSandHillMultiplier, scaleSandHillMultiplier, a1);
}

static void __cdecl ScaleIceCapMultiplier(ObjectMaster* a1)
{
	scale_trampoline(Align::automatic, false, ScaleIceCapMultiplier, scaleIceCapMultiplier, a1);
}

static void __cdecl ScaleGammaTimeAddHud(ObjectMaster* a1)
{
	scale_trampoline(Align::right, false, ScaleGammaTimeAddHud, scaleGammaTimeAddHud, a1);
}
static void __cdecl ScaleGammaTimeRemaining(ObjectMaster* a1)
{
	scale_trampoline(Align::bottom | Align::horizontal_center, false, ScaleGammaTimeRemaining, scaleGammaTimeRemaining, a1);
}

static void __cdecl ScaleEmblemScreen(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, ScaleEmblemScreen, scaleEmblemScreen, a1);
}

static void __cdecl ScaleBossName(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, ScaleBossName, scaleBossName, a1);
}

static void __cdecl ScaleNightsCards(ObjectMaster* a1)
{
	scale_trampoline(Align::automatic, false, ScaleNightsCards, scaleNightsCards, a1);
}
static void __cdecl ScaleNightsJackpot(ObjectMaster* a1)
{
	scale_trampoline(Align::automatic, false, ScaleNightsJackpot, scaleNightsJackpot, a1);
}

static void __cdecl ScaleMissionStartClear(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, ScaleMissionStartClear, scaleMissionStartClear, a1);
}
static void __cdecl ScaleMissionTimer()
{

	scale_trampoline(Align::center, false, ScaleMissionTimer, scaleMissionTimer);
}
static void __cdecl ScaleMissionCounter()
{
	scale_trampoline(Align::center, false, ScaleMissionCounter, scaleMissionCounter);
}

static void __cdecl ScaleTailsWinLose(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, ScaleTailsWinLose, scaleTailsWinLose, a1);
}
static void __cdecl ScaleTailsRaceBar(ObjectMaster* a1)
{
	scale_trampoline(Align::horizontal_center | Align::bottom, false, ScaleTailsRaceBar, scaleTailsRaceBar, a1);
}

static void __cdecl ScaleDemoPressStart(ObjectMaster* a1)
{
	scale_trampoline(Align::right, false, ScaleDemoPressStart, scaleDemoPressStart, a1);
}

static void __cdecl ChaoDX_Message_PlayerAction_Load_r()
{
	scale_trampoline(Align::automatic, false, ChaoDX_Message_PlayerAction_Load_r, ChaoDX_Message_PlayerAction_Load_t);
}

static void __cdecl ChaoDX_Message_PlayerAction_Display_r(ObjectMaster* a1)
{
	scale_trampoline(Align::top | Align::right, false, ChaoDX_Message_PlayerAction_Display_r, ChaoDX_Message_PlayerAction_Display_t, a1);
}

void __cdecl MissionCompleteScreen_Draw_r()
{
	scale_trampoline(Align::center, false, MissionCompleteScreen_Draw_r, MissionCompleteScreen_Draw_t);
}

static void __cdecl CharSelBg_Display_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, CharSelBg_Display_r, CharSelBg_Display_t, a1);
}

static void __cdecl TrialLevelList_Display_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, TrialLevelList_Display_r, TrialLevelList_Display_t, a1);
}

static void __cdecl SubGameLevelList_Display_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, SubGameLevelList_Display_r, SubGameLevelList_Display_t, a1);
}

static void __cdecl EmblemResultMenu_Display_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, EmblemResultMenu_Display_r, EmblemResultMenu_Display_t, a1);
}

static void __cdecl FileSelect_Display_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, FileSelect_Display_r, FileSelect_Display_t, a1);
}

static void __cdecl MenuObj_Display_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, MenuObj_Display_r, MenuObj_Display_t, a1);
}

static void __cdecl OptionsMenu_Display_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, OptionsMenu_Display_r, OptionsMenu_Display_t, a1);
}

static void __cdecl SoundTest_Display_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, SoundTest_Display_r, SoundTest_Display_t, a1);
}

static void __cdecl GreenMenuRect_Draw_r(float x, float y, float z, float width, float height)
{
	scale_trampoline(Align::center, false, GreenMenuRect_Draw_r, GreenMenuRect_Draw_t, x, y, z, width, height);
}

static void __cdecl TutorialInstructionOverlay_Display_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, TutorialInstructionOverlay_Display_r, TutorialInstructionOverlay_Display_t, a1);
}

static Sint32 __cdecl DisplayTitleCard_r()
{
	return scale_trampoline<Sint32>(Align::center, false, DisplayTitleCard_r, DisplayTitleCard_t);
}

static void __cdecl Credits_Main_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, Credits_Main_r, Credits_Main_t, a1);
}

static void __cdecl PauseMenu_Map_Display_r()
{
	scale_trampoline(Align::center, false, PauseMenu_Map_Display_r, PauseMenu_Map_Display_t);
}

static void __cdecl DrawSubtitles_r()
{
	scale_trampoline(Align::center, false, DrawSubtitles_r, DrawSubtitles_t);
}

static void EmblemCollected_Init_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, EmblemCollected_Init_r, EmblemCollected_Init_t, a1);
}

static void EmblemCollected_Main_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, EmblemCollected_Main_r, EmblemCollected_Main_t, a1);
}

static void __cdecl DrawTitleScreen_o(void* a1)
{
	auto orig = DrawTitleScreen_t->Target();

	__asm
	{
		mov esi, a1
		call orig
	}
}

// this replacement function forces the title screen into a 4:3 box for aspect-correct scale
static void __cdecl DrawTitleScreen_r(void* a1)
{
	auto old_fill = bg_fill;
	bg_fill = FillMode::fit;

	scale_push(Align::center, true);

	DrawTitleScreen_o(a1);

	bg_fill = old_fill;
	scale_pop();

	constexpr auto ratio_4_3 = 4.0f / 3.0f;

	const auto width = static_cast<float>(HorizontalResolution);
	const auto height = static_cast<float>(VerticalResolution);

	const auto ratio = width / height;

	const bool is_4_3 = std::abs(ratio - ratio_4_3) < std::numeric_limits<float>::epsilon();
	const bool is_tall = ratio < ratio_4_3 && !is_4_3;

	// convert the current resolution to the smallest fit 4:3 resolution
	const auto ratio_width = is_tall ? width : height * ratio_4_3;
	const auto ratio_height = is_tall ? width / ratio_4_3 : height;

	NJS_POINT2COL rect {};

	// all vertex colors black
	Uint32 colors[4] = {
		0xFF000000,
		0xFF000000,
		0xFF000000,
		0xFF000000
	};

	rect.num = 4;
	rect.col = reinterpret_cast<NJS_COLOR*>(colors); // NJS_COLOR is a union of Uint32 and NJS_BGRA

	// if we're a tall resolution (ratio < 4:3), draw bars
	// above and below, otherwise draw bars to the left and right
	// if the resolution is non-4:3
	if (is_tall)
	{
		const float margin = (height - ratio_height) / 2.0f;

		NJS_POINT2 points[4] = {
			{ 0.0f, 0.0f },
			{ 0.0f, margin },
			{ width, 0.0f },
			{ width, margin },
		};

		rect.p = points;

		njDrawTriangle2D(&rect, 2, -1.0f, NJD_DRAW_CONNECTED);

		for (NJS_POINT2& point : points)
		{
			point.y += margin + ratio_height;
		}

		njDrawTriangle2D(&rect, 2, -1.0f, NJD_DRAW_CONNECTED);
	}
	else if (!is_4_3)
	{
		const float margin = (width - ratio_width) / 2.0f;

		NJS_POINT2 points[4] = {
			{ 0.0f, 0.0f },
			{ 0.0f, height },
			{ margin, 0.0f },
			{ margin, height },
		};

		rect.p = points;

		njDrawTriangle2D(&rect, 2, -1.0f, NJD_DRAW_CONNECTED);

		for (NJS_POINT2& point : points)
		{
			point.x += margin + ratio_width;
		}

		njDrawTriangle2D(&rect, 2, -1.0f, NJD_DRAW_CONNECTED);
	}
}

static void __declspec(naked) DrawTitleScreen_asm()
{
	__asm
	{
		push esi
		call DrawTitleScreen_r
		pop esi
		ret
	}
}


void hudscale::initialize()
{
	MissionCompleteScreen_Draw_t         = new Trampoline(0x00590690, 0x00590695, MissionCompleteScreen_Draw_r);
	CharSelBg_Display_t                  = new Trampoline(0x00512450, 0x00512455, CharSelBg_Display_r);
	TrialLevelList_Display_t             = new Trampoline(0x0050B410, 0x0050B415, TrialLevelList_Display_r);
	SubGameLevelList_Display_t           = new Trampoline(0x0050A640, 0x0050A645, SubGameLevelList_Display_r);
	EmblemResultMenu_Display_t           = new Trampoline(0x0050DFD0, 0x0050DFD5, EmblemResultMenu_Display_r);
	FileSelect_Display_t                 = new Trampoline(0x00505550, 0x00505555, FileSelect_Display_r);
	MenuObj_Display_t                    = new Trampoline(0x00432480, 0x00432487, MenuObj_Display_r);
	OptionsMenu_Display_t                = new Trampoline(0x00509810, 0x00509815, OptionsMenu_Display_r);
	SoundTest_Display_t                  = new Trampoline(0x00511390, 0x00511395, SoundTest_Display_r);
	GreenMenuRect_Draw_t                 = new Trampoline(0x004334F0, 0x004334F5, GreenMenuRect_Draw_r);
	TutorialInstructionOverlay_Display_t = new Trampoline(0x006430F0, 0x006430F7, TutorialInstructionOverlay_Display_r);
	DisplayTitleCard_t                   = new Trampoline(0x0047E170, 0x0047E175, DisplayTitleCard_r);
	Credits_Main_t                       = new Trampoline(0x006411A0, 0x006411A5, Credits_Main_r);
	PauseMenu_Map_Display_t              = new Trampoline(0x00458B00, 0x00458B06, PauseMenu_Map_Display_r);
	EmblemCollected_Init_t               = new Trampoline(0x004B4860, 0x004B4867, EmblemCollected_Init_r);
	EmblemCollected_Main_t               = new Trampoline(0x004B46A0, 0x004B46A6, EmblemCollected_Main_r);
	DrawTitleScreen_t                    = new Trampoline(0x0050E470, 0x0050E476, DrawTitleScreen_asm);

	DrawSubtitles_t = new Trampoline(0x0040D4D0, 0x0040D4D9, DrawSubtitles_r);
	WriteCall(reinterpret_cast<void*>(reinterpret_cast<size_t>(DrawSubtitles_t->Target()) + 4), reinterpret_cast<void*>(0x00402F00));

	// Fixes selection of high/low resolution title screen textures
	WriteData(reinterpret_cast<float**>(0x0050E4B6 + 2), &scale_min);
	WriteData(reinterpret_cast<float**>(0x0050EA42 + 2), &scale_min);
	WriteData(reinterpret_cast<float**>(0x0050F47F + 2), &scale_min);
	WriteData(reinterpret_cast<float**>(0x0050F503 + 2), &scale_min);

	// Fixes character scale on character select screen.
	WriteData(reinterpret_cast<const float**>(0x0051285E), &patch_dummy);

	WriteJump(reinterpret_cast<void*>(0x0042BEE0), scale_result_screen);

	HudDisplayRingTimeLife_Check_t = new Trampoline(0x00425F90, 0x00425F95, HudDisplayRingTimeLife_Check_r);
	HudDisplayScoreOrTimer_t = new Trampoline(0x00427F50, 0x00427F55, HudDisplayScoreOrTimer_r);
	DrawStageMissionImage_t = new Trampoline(0x00457120, 0x00457126, DrawStageMissionImage_r);

	DisplayPauseMenu_t = new Trampoline(0x00415420, 0x00415425, DisplayPauseMenu_r);

	LifeGauge_Main_t = new Trampoline(0x004B3830, 0x004B3837, LifeGauge_Main_r);

	scaleScoreA = new Trampoline(0x00628330, 0x00628335, ScaleScoreA);

	WriteData(reinterpret_cast<const float**>(0x006288C2), &patch_dummy);
	scaleTornadoHP = new Trampoline(0x00628490, 0x00628496, ScaleTornadoHP);

	// TODO: Consider tracking down the individual functions so that they can be individually aligned.
	scaleTwinkleCircuitHUD = new Trampoline(0x004DB5E0, 0x004DB5E5, ScaleTwinkleCircuitHUD);

	// Rod scaling
	FishingHud_DrawReel_t = new Trampoline(0x0046C9F0, 0x0046C9F5, FishingHud_DrawReel_r);
	FishingHud_DrawRod_t = new Trampoline(0x0046CAB0, 0x0046CAB9, FishingHud_DrawRod_r);
	FishingHud_DrawMeters_t = new Trampoline(0x0046CC70, 0x0046CC75, FishingHud_DrawMeters_r);
	FishingHud_DrawHIT_t = new Trampoline(0x0046C920, 0x0046C926, FishingHud_DrawHIT_r);
	BigHud_DrawWeightAndLife_t = new Trampoline(0x0046FB00, 0x0046FB05, BigHud_DrawWeightAndLife_r);

	scaleAnimalPickup = new Trampoline(0x0046B330, 0x0046B335, ScaleAnimalPickup);

	scaleItemBoxSprite = new Trampoline(0x004C0790, 0x004C0795, ScaleItemBoxSprite);

	scaleBalls = new Trampoline(0x005C0B70, 0x005C0B75, ScaleBalls);

	scaleCheckpointTime = new Trampoline(0x004BABE0, 0x004BABE5, ScaleCheckpointTime);
	WriteData(reinterpret_cast<const float**>(0x0044F2E1), &patch_dummy);
	WriteData(reinterpret_cast<const float**>(0x0044F30B), &patch_dummy);
	WriteData(reinterpret_cast<const float**>(0x00476742), &patch_dummy);
	WriteData(reinterpret_cast<const float**>(0x0047676A), &patch_dummy);

	// EmeraldRadarHud_Load
	WriteData(reinterpret_cast<const float**>(0x00475BE3), &patch_dummy);
	WriteData(reinterpret_cast<const float**>(0x00475C00), &patch_dummy);
	// Emerald get
	WriteData(reinterpret_cast<const float**>(0x00477E8E), &patch_dummy);
	WriteData(reinterpret_cast<const float**>(0x00477EC0), &patch_dummy);

	scaleEmeraldRadarA = new Trampoline(0x00475A70, 0x00475A75, ScaleEmeraldRadarA);
	scaleEmeraldRadarB = new Trampoline(0x00475E50, 0x00475E55, ScaleEmeraldRadarB);
	scaleEmeraldRadar_Grab = new Trampoline(0x00475D50, 0x00475D55, ScaleEmeraldRadar_Grab);

	scaleSandHillMultiplier = new Trampoline(0x005991A0, 0x005991A6, ScaleSandHillMultiplier);
	scaleIceCapMultiplier = new Trampoline(0x004EC120, 0x004EC125, ScaleIceCapMultiplier);

	WriteData(reinterpret_cast<const float**>(0x0049FF70), &patch_dummy);
	WriteData(reinterpret_cast<const float**>(0x004A005B), &patch_dummy);
	WriteData(reinterpret_cast<const float**>(0x004A0067), &patch_dummy);
	scaleGammaTimeAddHud = new Trampoline(0x0049FDA0, 0x0049FDA5, ScaleGammaTimeAddHud);
	scaleGammaTimeRemaining = new Trampoline(0x004C51D0, 0x004C51D7, ScaleGammaTimeRemaining);

	WriteData(reinterpret_cast<float**>(0x004B4470), &scale_h);
	WriteData(reinterpret_cast<float**>(0x004B444E), &scale_v);
	scaleEmblemScreen = new Trampoline(0x004B4200, 0x004B4205, ScaleEmblemScreen);

	scaleBossName = new Trampoline(0x004B33D0, 0x004B33D5, ScaleBossName);

	scaleNightsCards = new Trampoline(0x005D73F0, 0x005D73F5, ScaleNightsCards);
	WriteData(reinterpret_cast<float**>(0x005D701B), &scale_h);
	scaleNightsJackpot = new Trampoline(0x005D6E60, 0x005D6E67, ScaleNightsJackpot);

	scaleMissionStartClear = new Trampoline(0x00591260, 0x00591268, ScaleMissionStartClear);

	scaleMissionTimer = new Trampoline(0x00592D50, 0x00592D59, ScaleMissionTimer);
	scaleMissionCounter = new Trampoline(0x00592A60, 0x00592A68, ScaleMissionCounter);

	scaleTailsWinLose = new Trampoline(0x0047C480, 0x0047C485, ScaleTailsWinLose);
	scaleTailsRaceBar = new Trampoline(0x0047C260, 0x0047C267, ScaleTailsRaceBar);

	scaleDemoPressStart = new Trampoline(0x00457D30, 0x00457D36, ScaleDemoPressStart);

#if 0
	ChaoDX_Message_PlayerAction_Load_t = new Trampoline(0x0071B3B0, 0x0071B3B7, ChaoDX_Message_PlayerAction_Load_r);
	ChaoDX_Message_PlayerAction_Display_t = new Trampoline(0x0071B210, 0x0071B215, ChaoDX_Message_PlayerAction_Display_r);
#endif
}
