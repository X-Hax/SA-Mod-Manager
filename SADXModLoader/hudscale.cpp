#include "stdafx.h"
#include "Trampoline.h"
#include "uiscale.h"
#include "hudscale.h"

using namespace uiscale;

static const float patch_dummy = 1.0f;
static const float float640    = 640.0f;
static const int int640        = 640;

// Chao patches
static float aspect_scale             = 1.0f;
static float preview_egg              = -34.0f;
static float preview_animal_hat_shell = -26.0f;
static float preview_fruit            = -22.0f;
static float preview_pacifier         = -13.0f;

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
static Trampoline* BigWeightBonus_Display_t;
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
static Trampoline* HeldChaoParamWindowDisplayer_t;
static Trampoline* MissionCompleteScreen_Draw_t;
static Trampoline* CharSelBg_Display_t;
static Trampoline* TrialLevelList_Display_t;
static Trampoline* SubGameLevelList_Display_t;
static Trampoline* EmblemResultMenu_Display_t;
static Trampoline* FileSelect_Display_t;
static Trampoline* MenuObj_Display_t;
static Trampoline* InetDemo_Display_t;
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
static Trampoline* ChaoRaceTimer_t;
static Trampoline* ChaoRaceRankings_t;
static Trampoline* AlgKinderBlDisp__t;
static Trampoline* AL_BlackmarketMenuCreate_t;
static Trampoline* AL_BlackmarketRingWinDisplayer_t;
static Trampoline* AL_EntranceMenuLargeTitleBarDisplayer_t;
static Trampoline* AL_EntranceMenuSmallTitleBarDisplayer_t;
static Trampoline* AL_EntranceMenuLargeTitleBarDisplayerPost_t;
static Trampoline* AL_EntranceMenuSmallTitleBarDisplayerPost_t;
static Trampoline* AL_CreateChaoSelectMenu_t;
static Trampoline* AL_EntranceMenuRaceTitleBarDisplayer_t;
static Trampoline* ChaoSelectWindowTDisplayer_t;
static Trampoline* AL_ChaoParamWindowDisplayer_t;
static Trampoline* CourseNameBarJewelDisplayer_t;
static Trampoline* BlueButtonDisplayer_t;
static Trampoline* InfoBaseWindowDisplayer_t;
static Trampoline* PersonalRecordWindowDisplayer_t;
static Trampoline* PersonalRecordWindowExecutor_t;
static Trampoline* BlueButtonDisplayerCS_t;
static Trampoline* AL_EntranceMenuBackGroundDisplayer_t;
static Trampoline* MessageBarCreate_t;
static Trampoline* MessageBar_Display_t;
static Trampoline* OdeBGDisplayer_t;
static Trampoline* AlMsgWarnDisp_t;
static Trampoline* AlMsgSelectDisp_t;
static Trampoline* OdeLargeTitleBarDisplayer_t;
static Trampoline* AL_OdeTelopCreate_t;
static Trampoline* AL_OdeTelopDisp_t;
static Trampoline* CreateMainMenuBar_t;
static Trampoline* MainMenuBarDisplayer_t;
static Trampoline* AL_OdekakeMenuStageNazukeya_t;
static Trampoline* CreateChaoParamWindow_t;
static Trampoline* ChaoParamWindowDisplayer_t;
static Trampoline* CreateSayounaraWindow_t;
static Trampoline* SayounaraWindowDisplayer_t;
static Trampoline* CreateKetteiButton_t;
static Trampoline* GuideButtonDisplayer_t;
static Trampoline* CreateCancelButton_t;
static Trampoline* CreateDecideButton_t;
static Trampoline* DecideButtonDisplayer_t;
static Trampoline* AloG00Hintmenu_t;
static Trampoline* AlgKinderPrDisp_t;
static Trampoline* ChaoParamWindowExecutor_t;
static Trampoline* ChaoSelectWindowExecutor_t;
static Trampoline* AL_ChaoParamWindowExecutor_t;
static Trampoline* late_exec_t;
static Trampoline* MiniGameCollectionMenu_t;
static Trampoline* DrawGameOver_t;
static Trampoline* DrawGameOverTC_t;
static Trampoline* DrawGameOverHH_t;
static Trampoline* RecapBackground_Main_t;

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

static void __cdecl ChaoRaceTimer_r(int _this)
{
	scale_trampoline(Align::center, false, ChaoRaceTimer_r, ChaoRaceTimer_t, _this);
}

static void __cdecl ChaoRaceRankings_r(int this1, int this2, int this3)
{
	scale_trampoline(Align::left, false, ChaoRaceRankings_r, ChaoRaceRankings_t, this1, this2, this3);
}

static void __cdecl DrawStageMissionImage_r(ObjectMaster* _this)
{
	scale_trampoline(Align::center, false, DrawStageMissionImage_r, DrawStageMissionImage_t, _this);
}

static short __cdecl DisplayPauseMenu_r()
{
	return scale_trampoline(Align::center, false, DisplayPauseMenu_r, DisplayPauseMenu_t);
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

static void __cdecl BigWeightBonus_Display_r(ObjectMaster* a1)
{
	scale_trampoline(Align::left, false, BigWeightBonus_Display_r, BigWeightBonus_Display_t, a1);
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

static void __cdecl HeldChaoParamWindowDisplayer_o(ObjectMaster* a1)
{
	auto orig = HeldChaoParamWindowDisplayer_t->Target();

	__asm
	{
		mov eax, a1
		call orig
	}
}

static void __cdecl HeldChaoParamWindowDisplayer_r(ObjectMaster* a1)
{
	scale_push(Align::left, false);
	HeldChaoParamWindowDisplayer_o(a1);
	scale_pop();
}

static void __declspec(naked) HeldChaoParamWindowDisplayer_asm()
{
	__asm
	{
		push eax
		call HeldChaoParamWindowDisplayer_r
		pop eax
		ret
	}
}

static void __cdecl AlgKinderBlDisp__o(void* w)
{
	auto orig = AlgKinderBlDisp__t->Target();

	__asm
	{
		mov eax, w
		call orig
	}
}

static void __cdecl AlgKinderBlDisp__r(void* w)
{
	scale_push(Align::center, false);
	AlgKinderBlDisp__o(w);
	scale_pop();
}

static void __declspec(naked) AlgKinderBlDisp__asm(void* w)
{
	__asm
	{
		push eax
		call AlgKinderBlDisp__r
		pop eax
		ret
	}
}

static ObjectMaster* __cdecl AL_BlackmarketMenuCreate_r(ObjectMaster* a1)
{
	return scale_trampoline(Align::center, false, AL_BlackmarketMenuCreate_r, AL_BlackmarketMenuCreate_t, a1);
}

static void __cdecl AL_BlackmarketRingWinDisplayer_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, AL_BlackmarketRingWinDisplayer_r, AL_BlackmarketRingWinDisplayer_t, a1);
}

static void __cdecl AL_CreateChaoSelectMenu_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, AL_CreateChaoSelectMenu_r, AL_CreateChaoSelectMenu_t, a1);
}

static void __cdecl AL_EntranceMenuLargeTitleBarDisplayer_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, AL_EntranceMenuLargeTitleBarDisplayer_r, AL_EntranceMenuLargeTitleBarDisplayer_t, a1);
}

static void __cdecl AL_EntranceMenuSmallTitleBarDisplayer_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, AL_EntranceMenuSmallTitleBarDisplayer_r, AL_EntranceMenuSmallTitleBarDisplayer_t, a1);
}

static void __cdecl AL_EntranceMenuLargeTitleBarDisplayerPost_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, AL_EntranceMenuLargeTitleBarDisplayerPost_r, AL_EntranceMenuLargeTitleBarDisplayerPost_t, a1);
}

static void __cdecl AL_EntranceMenuSmallTitleBarDisplayerPost_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, AL_EntranceMenuSmallTitleBarDisplayerPost_r, AL_EntranceMenuSmallTitleBarDisplayerPost_t, a1);
}

static void __cdecl AL_EntranceMenuRaceTitleBarDisplayer_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, AL_EntranceMenuRaceTitleBarDisplayer_r, AL_EntranceMenuRaceTitleBarDisplayer_t, a1);
}

static void __cdecl ChaoSelectWindowTDisplayer_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, ChaoSelectWindowTDisplayer_r, ChaoSelectWindowTDisplayer_t, a1);
}

static void __cdecl AL_ChaoParamWindowDisplayer_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, AL_ChaoParamWindowDisplayer_r, AL_ChaoParamWindowDisplayer_t, a1);
}

static void __cdecl CourseNameBarJewelDisplayer_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, CourseNameBarJewelDisplayer_r, CourseNameBarJewelDisplayer_t, a1);
}

static void __cdecl BlueButtonDisplayer_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, BlueButtonDisplayer_r, BlueButtonDisplayer_t, a1);
}

static void __cdecl InfoBaseWindowDisplayer_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, InfoBaseWindowDisplayer_r, InfoBaseWindowDisplayer_t, a1);
}

static void __cdecl PersonalRecordWindowDisplayer_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, PersonalRecordWindowDisplayer_r, PersonalRecordWindowDisplayer_t, a1);
}

static void __cdecl PersonalRecordWindowExecutor_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, PersonalRecordWindowExecutor_r, PersonalRecordWindowExecutor_t, a1);
}

static void __cdecl BlueButtonDisplayerCS_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, BlueButtonDisplayerCS_r, BlueButtonDisplayerCS_t, a1);
}

static void __cdecl AL_EntranceMenuBackGroundDisplayer_r(ObjectMaster* a1)
{
	auto original = static_cast<decltype(AL_EntranceMenuBackGroundDisplayer_r)*>(AL_EntranceMenuBackGroundDisplayer_t->Target());
	auto old_fill = bg_fill;

	bg_fill = FillMode::fill;

	scale_push(Align::left, true);
	original(a1);
	scale_pop();

	bg_fill = old_fill;
}

static void __cdecl MessageBarCreate_r(ObjectMaster* a1)
{
	scale_trampoline(Align::bottom, false, MessageBarCreate_r, MessageBarCreate_t, a1);
}

static void __cdecl MessageBar_Display_r(ObjectMaster* a1)
{
	scale_trampoline(Align::bottom, false, MessageBar_Display_r, MessageBar_Display_t, a1);
}

static void __cdecl OdeBGDisplayer_r(ObjectMaster* a1)
{
	scale_trampoline(Align::left | Align::top, false, OdeBGDisplayer_r, OdeBGDisplayer_t, a1);
}

static void __cdecl AlMsgWarnDisp_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, AlMsgWarnDisp_r, AlMsgWarnDisp_t, a1);
}

static void __cdecl AlMsgSelectDisp_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, AlMsgSelectDisp_r, AlMsgSelectDisp_t, a1);
}

static void __cdecl OdeLargeTitleBarDisplayer_r(ObjectMaster* a1)
{
	scale_trampoline(Align::right | Align::top, false, OdeLargeTitleBarDisplayer_r, OdeLargeTitleBarDisplayer_t, a1);
}

static void __cdecl AL_OdeTelopCreate_r()
{
	scale_trampoline(Align::bottom, false, AL_OdeTelopCreate_r, AL_OdeTelopCreate_t);
}

static void __cdecl AL_OdeTelopDisp_r(ObjectMaster* a1)
{
	scale_trampoline(Align::bottom, false, AL_OdeTelopDisp_r, AL_OdeTelopDisp_t, a1);
}

static void __cdecl CreateMainMenuBar_r(char id, float xpos, float ypos, unsigned __int16 WaitTime, char active)
{
	scale_trampoline(Align::center, false, CreateMainMenuBar_r, CreateMainMenuBar_t, id, xpos, ypos, WaitTime, active);
}

static void __cdecl MainMenuBarDisplayer_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, MainMenuBarDisplayer_r, MainMenuBarDisplayer_t, a1);
}

static void __cdecl AL_OdekakeMenuStageNazukeya_r(void* a1)
{
	scale_trampoline(Align::center, false, AL_OdekakeMenuStageNazukeya_r, AL_OdekakeMenuStageNazukeya_t, a1);
}

static void __cdecl CreateChaoParamWindow_r(float xpos, float ypos, unsigned __int16 WaitTime)
{
	scale_trampoline(Align::center, false, CreateChaoParamWindow_r, CreateChaoParamWindow_t, xpos, ypos, WaitTime);
}

static void __cdecl ChaoParamWindowDisplayer_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, ChaoParamWindowDisplayer_r, ChaoParamWindowDisplayer_t, a1);
}

static void __cdecl CreateSayounaraWindow_r(float xpos, float ypos, unsigned __int16 WaitTime)
{
	scale_trampoline(Align::center, false, CreateSayounaraWindow_r, CreateSayounaraWindow_t, xpos, ypos, WaitTime);
}

static void __cdecl SayounaraWindowDisplayer_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, SayounaraWindowDisplayer_r, SayounaraWindowDisplayer_t, a1);
}

static void __cdecl CreateKetteiButton_r(float xpos, float ypos, unsigned __int16 WaitTime)
{
	scale_trampoline(Align::center, false, CreateKetteiButton_r, CreateKetteiButton_t, xpos, ypos, WaitTime);
}

static void __cdecl GuideButtonDisplayer_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, GuideButtonDisplayer_r, GuideButtonDisplayer_t, a1);
}

static void __cdecl CreateCancelButton_r(float xpos, float ypos, unsigned __int16 WaitTime)
{
	scale_trampoline(Align::center, false, CreateCancelButton_r, CreateCancelButton_t, xpos, ypos, WaitTime);
}

static void __cdecl CreateDecideButton_r(char id, int cursorX, int cursorY, float xpos, float ypos, unsigned __int16 WaitTime)
{
	scale_trampoline(Align::center, false, CreateDecideButton_r, CreateDecideButton_t, id, cursorX, cursorY, xpos, ypos, WaitTime);
}

static void __cdecl DecideButtonDisplayer_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, DecideButtonDisplayer_r, DecideButtonDisplayer_t, a1);
}

static void __cdecl AloG00Hintmenu_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, AloG00Hintmenu_r, AloG00Hintmenu_t, a1);
}

static void __cdecl AlgKinderPrDisp_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, AlgKinderPrDisp_r, AlgKinderPrDisp_t, a1);
}

static void __cdecl ChaoParamWindowExecutor_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, ChaoParamWindowExecutor_r, ChaoParamWindowExecutor_t, a1);
}

static void __cdecl ChaoSelectWindowExecutor_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, ChaoSelectWindowExecutor_r, ChaoSelectWindowExecutor_t, a1);
}

static void __cdecl AL_ChaoParamWindowExecutor_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, AL_ChaoParamWindowExecutor_r, AL_ChaoParamWindowExecutor_t, a1);
}

static void __cdecl MissionCompleteScreen_Draw_r()
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

static void __cdecl InetDemo_Display_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, InetDemo_Display_r, InetDemo_Display_t, a1);
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
	return scale_trampoline(Align::center, false, DisplayTitleCard_r, DisplayTitleCard_t);
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

static void __cdecl EmblemCollected_Init_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, EmblemCollected_Init_r, EmblemCollected_Init_t, a1);
}

static void __cdecl EmblemCollected_Main_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, EmblemCollected_Main_r, EmblemCollected_Main_t, a1);
}

static void __cdecl late_exec_r()
{
	auto original = static_cast<decltype(late_exec_r)*>(late_exec_t->Target());

	scale_disable();
	original();
	scale_enable();
}

static void __cdecl MiniGameCollectionMenu_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, false, MiniGameCollectionMenu_r, MiniGameCollectionMenu_t, a1);
}

static void __cdecl DrawGameOver_o(int a1)
{
	auto orig = DrawGameOver_t->Target();

	__asm
	{
		mov esi, a1
		call orig
	}
}

static void __cdecl DrawGameOver_r(int a1)
{
	scale_push(Align::center, false);
	DrawGameOver_o(a1);
	scale_pop();
}

static void __declspec(naked) DrawGameOver_asm()
{
	__asm
	{
		push esi
		call DrawGameOver_r
		pop esi
		ret
	}
}

static void __cdecl DrawGameOverTC_o(int a1)
{
	auto orig = DrawGameOverTC_t->Target();

	__asm
	{
		mov esi, a1
		call orig
	}
}

static void __cdecl DrawGameOverTC_r(int a1)
{
	scale_push(Align::center, false);
	DrawGameOverTC_o(a1);
	scale_pop();
}

static void __declspec(naked) DrawGameOverTC_asm()
{
	__asm
	{
		push esi
		call DrawGameOverTC_r
		pop esi
		ret
	}
}

static void __cdecl DrawGameOverHH_o(int a1)
{
	auto orig = DrawGameOverHH_t->Target();

	__asm
	{
		mov esi, a1
		call orig
	}
}

static void __cdecl DrawGameOverHH_r(int a1)
{
	scale_push(Align::center, false);
	DrawGameOverHH_o(a1);
	scale_pop();
}

static void __declspec(naked) DrawGameOverHH_asm()
{
	__asm
	{
		push esi
		call DrawGameOverHH_r
		pop esi
		ret
	}
}

static void DrawRect_DrawNowMaybe_GameOverHH(float left, float top, float right, float bottom, float depth, int color) {
	uiscale::scale_disable();
	DrawRect_DrawNowMaybe(left, top, right, bottom, depth, color);
	uiscale::scale_enable();
}

static void __cdecl RecapBackground_Main_r(ObjectMaster* a1)
{
	scale_trampoline(Align::center, true, RecapBackground_Main_r, RecapBackground_Main_t, a1);
}

void __cdecl njDrawTextureMemList_NoSkippedFrames_RecapText(NJS_TEXTURE_VTX* points, Int count, Uint32 gbix, Int flag)
{
	uiscale::scale_push(Align::center, false);

	NJS_TEXTURE_VTX* new_points = new NJS_TEXTURE_VTX[count];
	memcpy(new_points, points, sizeof(NJS_TEXTURE_VTX) * count);
	njDrawTextureMemList_NoSkippedFrames(new_points, count, gbix, flag);
	delete[] new_points;

	uiscale::scale_pop();
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

void hudscale::update() {
	float vertscale = static_cast<float>(VerticalResolution) / 480.0f;

	aspect_scale             = static_cast<float>(HorizontalResolution) / (640.0f * vertscale);
	aspect_scale             = max(aspect_scale, 1.0f);
	preview_egg              = -34.0f * vertscale;
	preview_animal_hat_shell = -26.0f * vertscale;
	preview_fruit            = -22.0f * vertscale;
	preview_pacifier         = -13.0f * vertscale;

	// Black Martket Item List
	WriteData(reinterpret_cast<float*>(0x0072773B), -68.0f * vertscale); // Egg
	WriteData(reinterpret_cast<float*>(0x0072767B), -44.0f * vertscale); // Fruit
	WriteData(reinterpret_cast<float*>(0x00727546), -52.0f * vertscale); // Hat
	WriteData(reinterpret_cast<float*>(0x00727513), -26.0f * vertscale); // Pacifier

	// Black Market Item Preview
	WriteData(reinterpret_cast<float*>(0x00726211), preview_animal_hat_shell);
	WriteData(reinterpret_cast<float*>(0x007261CF), preview_pacifier);
}

static void InitializeChaoHUDs() {
	// Chao Garden HUD (whistle, pet, pick, stats)
	WriteData(reinterpret_cast<const float**>(0x0071AEAC), &patch_dummy);
	WriteData(reinterpret_cast<const float**>(0x0071AF00), &patch_dummy);
	WriteData(reinterpret_cast<const float**>(0x0071B03C), &patch_dummy);
	WriteData(reinterpret_cast<const float**>(0x0071B093), &patch_dummy);
	ChaoDX_Message_PlayerAction_Load_t    = new Trampoline(0x0071B3B0, 0x0071B3B7, ChaoDX_Message_PlayerAction_Load_r);
	ChaoDX_Message_PlayerAction_Display_t = new Trampoline(0x0071B210, 0x0071B215, ChaoDX_Message_PlayerAction_Display_r);
	HeldChaoParamWindowDisplayer_t        = new Trampoline(0x00737BD0, 0x00737BD5, HeldChaoParamWindowDisplayer_asm);

	// Black Market
	WriteData(reinterpret_cast<const float**>(0x00725831), &patch_dummy); // Ring box vertical position
	WriteData(reinterpret_cast<const float**>(0x0072587C), &patch_dummy); // Ring box horizontal position
	WriteData(reinterpret_cast<float**>(0x0072584A), &aspect_scale); // Ring box offset
	WriteData(reinterpret_cast<float**>(0x00725852), &aspect_scale); // Ring box offset
	WriteData(reinterpret_cast<const float**>(0x0072562B), &patch_dummy); // Text position V
	WriteData(reinterpret_cast<const float**>(0x0072564F), &patch_dummy); // Text position H
	WriteData(reinterpret_cast<const float**>(0x0072599C), &patch_dummy); // Description text position V
	WriteData(reinterpret_cast<const float**>(0x007259BD), &patch_dummy); // Description text position H
	AlgKinderBlDisp__t               = new Trampoline(0x007283B0, 0x007283B8, AlgKinderBlDisp__asm);
	AL_BlackmarketMenuCreate_t       = new Trampoline(0x00728A40, 0x00728A49, AL_BlackmarketMenuCreate_r);
	AL_BlackmarketRingWinDisplayer_t = new Trampoline(0x00744990, 0x00744995, AL_BlackmarketRingWinDisplayer_r);

	// Race
	ChaoRaceTimer_t = new Trampoline(0x00750E70, 0x00750E78, ChaoRaceTimer_r);
	ChaoRaceRankings_t = new Trampoline(0x007512F0, 0x007512F5, ChaoRaceRankings_r);

	// Entrance
	WriteData(reinterpret_cast<double*>(0x0088A5D0), 24.0); // Fix sprite padding in AL_ChaoParamWindowDisplayer
	WriteData(reinterpret_cast<const float**>(0x0072C6D3), &patch_dummy); // BlueButtonCS height
	WriteData(reinterpret_cast<const float**>(0x0072C6EC), &patch_dummy); // BlueButtonCS width
	WriteData(reinterpret_cast<const float**>(0x007691CC), &float640); // Chao select 3D placement patch
	WriteData(reinterpret_cast<const float**>(0x00747115), &float640); // Chao reccord 3D placement patch
	WriteData(reinterpret_cast<const float**>(0x00767CF1), &float640); // Chao param 3D placement patch
	AL_CreateChaoSelectMenu_t                   = new Trampoline(0x007491D0, 0x007491D5, AL_CreateChaoSelectMenu_r);
	AL_EntranceMenuBackGroundDisplayer_t        = new Trampoline(0x0074AB40, 0x0074AB47, AL_EntranceMenuBackGroundDisplayer_r);
	AL_EntranceMenuLargeTitleBarDisplayer_t     = new Trampoline(0x00749830, 0x00749835, AL_EntranceMenuLargeTitleBarDisplayer_r);
	AL_EntranceMenuSmallTitleBarDisplayer_t     = new Trampoline(0x00749EB0, 0x00749EB5, AL_EntranceMenuSmallTitleBarDisplayer_r);
	AL_EntranceMenuLargeTitleBarDisplayerPost_t = new Trampoline(0x00749820, 0x00749829, AL_EntranceMenuLargeTitleBarDisplayerPost_r);
	AL_EntranceMenuSmallTitleBarDisplayerPost_t = new Trampoline(0x00749E90, 0x00749E95, AL_EntranceMenuSmallTitleBarDisplayerPost_r);
	AL_EntranceMenuRaceTitleBarDisplayer_t      = new Trampoline(0x0074A1D0, 0x0074A1D7, AL_EntranceMenuRaceTitleBarDisplayer_r);
	ChaoSelectWindowTDisplayer_t                = new Trampoline(0x00769320, 0x00769327, ChaoSelectWindowTDisplayer_r);
	AL_ChaoParamWindowDisplayer_t               = new Trampoline(0x00768080, 0x00768087, AL_ChaoParamWindowDisplayer_r);
	CourseNameBarJewelDisplayer_t               = new Trampoline(0x00768AE0, 0x00768AE7, CourseNameBarJewelDisplayer_r);
	BlueButtonDisplayer_t                       = new Trampoline(0x00747C00, 0x00747C07, BlueButtonDisplayer_r);
	InfoBaseWindowDisplayer_t                   = new Trampoline(0x00747810, 0x00747817, InfoBaseWindowDisplayer_r);
	PersonalRecordWindowExecutor_t              = new Trampoline(0x00747080, 0x00747085, PersonalRecordWindowExecutor_r);
	PersonalRecordWindowDisplayer_t             = new Trampoline(0x007472E0, 0x007472E8, PersonalRecordWindowDisplayer_r);
	BlueButtonDisplayerCS_t                     = new Trampoline(0x007480B0, 0x007480B7, BlueButtonDisplayerCS_r);
	ChaoSelectWindowExecutor_t                  = new Trampoline(0x00768E10, 0x00768E16, ChaoSelectWindowExecutor_r);
	AL_ChaoParamWindowExecutor_t                = new Trampoline(0x00767D40, 0x00767D47, AL_ChaoParamWindowExecutor_r);
	
	// Name Machine
	WriteData(reinterpret_cast<float**>(0x0074DAF5), &scale_v);
	WriteData(reinterpret_cast<float**>(0x0074DBD3), &scale_v);
	WriteData(reinterpret_cast<float**>(0x0074DB22), &scale_h);
	WriteData(reinterpret_cast<float**>(0x0074DBA2), &scale_h);
	OdeBGDisplayer_t              = new Trampoline(0x0074DC20, 0x0074DC25, OdeBGDisplayer_r);
	AlMsgWarnDisp_t               = new Trampoline(0x0072D450, 0x0072D455, AlMsgWarnDisp_r);
	AlMsgSelectDisp_t             = new Trampoline(0x0072CDA0, 0x0072CDA5, AlMsgSelectDisp_r);
	OdeLargeTitleBarDisplayer_t   = new Trampoline(0x0076B5E0, 0x0076B5E7, OdeLargeTitleBarDisplayer_r);
	CreateMainMenuBar_t           = new Trampoline(0x0076D3D0, 0x0076D3D5, CreateMainMenuBar_r);
	MainMenuBarDisplayer_t        = new Trampoline(0x0076D0E0, 0x0076D0E7, MainMenuBarDisplayer_r);
	AL_OdekakeMenuStageNazukeya_t = new Trampoline(0x0074BC60, 0x0074BC67, AL_OdekakeMenuStageNazukeya_r);
	CreateChaoParamWindow_t       = new Trampoline(0x0076B360, 0x0076B365, CreateChaoParamWindow_r);
	ChaoParamWindowDisplayer_t    = new Trampoline(0x0076AD40, 0x0076AD47, ChaoParamWindowDisplayer_r);
	CreateSayounaraWindow_t       = new Trampoline(0x0076A9D0, 0x0076A9D5, CreateSayounaraWindow_r);
	SayounaraWindowDisplayer_t    = new Trampoline(0x0076A640, 0x0076A647, SayounaraWindowDisplayer_r);
	CreateKetteiButton_t          = new Trampoline(0x0076A2F0, 0x0076A2F5, CreateKetteiButton_r);
	GuideButtonDisplayer_t        = new Trampoline(0x0076A1D0, 0x0076A1D5, GuideButtonDisplayer_r);
	CreateCancelButton_t          = new Trampoline(0x0076A390, 0x0076A395, CreateCancelButton_r);
	CreateDecideButton_t          = new Trampoline(0x0076A030, 0x0076A035, CreateDecideButton_r);
	DecideButtonDisplayer_t       = new Trampoline(0x00769E40, 0x00769E47, DecideButtonDisplayer_r);
	ChaoParamWindowExecutor_t     = new Trampoline(0x0076AA60, 0x0076AA66, ChaoParamWindowExecutor_r);

	// MessageBar
	WriteData(reinterpret_cast<float**>(0x0076CE07), &aspect_scale);
	WriteData(reinterpret_cast<float**>(0x0076CCE0), &aspect_scale);
	WriteData(reinterpret_cast<float**>(0x00749437), &aspect_scale);
	WriteData(reinterpret_cast<float**>(0x0074935A), &aspect_scale);
	AL_OdeTelopCreate_t  = new Trampoline(0x0076CD60, 0x0076CD66, AL_OdeTelopCreate_r);
	AL_OdeTelopDisp_t    = new Trampoline(0x0076CC80, 0x0076CC85, AL_OdeTelopDisp_r);
	MessageBarCreate_t   = new Trampoline(0x007493B0, 0x007493B5, MessageBarCreate_r);
	MessageBar_Display_t = new Trampoline(0x00749300, 0x00749306, MessageBar_Display_r);

	// Monitor
	AloG00Hintmenu_t  = new Trampoline(0x00746800, 0x00746806, AloG00Hintmenu_r);
	AlgKinderPrDisp_t = new Trampoline(0x00746710, 0x00746715, AlgKinderPrDisp_r);

	// Black Market item list
	WriteData(reinterpret_cast<const int**>(0x00727449), &int640); // Egg
	WriteData(reinterpret_cast<const int**>(0x007274E5), &int640); // Fruit
	WriteData(reinterpret_cast<const int**>(0x00727387), &int640); // Hat

	// Black Market item previews
	WriteData(reinterpret_cast<const int**>(0x00725D31), &int640); // Egg
	WriteData(reinterpret_cast<const int**>(0x00725EC2), &int640); // Animal
	WriteData(reinterpret_cast<const int**>(0x00725FDB), &int640); // Fruit
	WriteData(reinterpret_cast<const int**>(0x00726176), &int640); // Hat
	WriteData(reinterpret_cast<const int**>(0x007261D6), &int640); // Pacifier
	WriteData(reinterpret_cast<const int**>(0x0072634A), &int640); // Eggshells
	WriteData(reinterpret_cast<float**>(0x00725D52), &preview_egg);
	WriteData(reinterpret_cast<float**>(0x00725EED), &preview_animal_hat_shell);
	WriteData(reinterpret_cast<float**>(0x00726006), &preview_fruit);
	WriteData(reinterpret_cast<float**>(0x0072636B), &preview_animal_hat_shell);
	
	// Fix weird Z scale to prevent flickering and broken lighting in Black Market
	WriteData(reinterpret_cast<float*>(0x007276B8), 1.0f);
	WriteData(reinterpret_cast<float*>(0x00725E2F), 1.0f);
	WriteData(reinterpret_cast<float*>(0x00725F32), 1.0f);
	WriteData(reinterpret_cast<float*>(0x0072604B), 1.0f);
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
	InetDemo_Display_t                   = new Trampoline(0x0050D430, 0x0050D435, InetDemo_Display_r);
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
	HudDisplayRingTimeLife_Check_t		 = new Trampoline(0x00425F90, 0x00425F95, HudDisplayRingTimeLife_Check_r);
	HudDisplayScoreOrTimer_t			 = new Trampoline(0x00427F50, 0x00427F55, HudDisplayScoreOrTimer_r);
	DrawStageMissionImage_t              = new Trampoline(0x00457120, 0x00457126, DrawStageMissionImage_r);
	DisplayPauseMenu_t                   = new Trampoline(0x00415420, 0x00415425, DisplayPauseMenu_r);
	LifeGauge_Main_t                     = new Trampoline(0x004B3830, 0x004B3837, LifeGauge_Main_r);
	scaleScoreA                          = new Trampoline(0x00628330, 0x00628335, ScaleScoreA);
	scaleAnimalPickup                    = new Trampoline(0x0046B330, 0x0046B335, ScaleAnimalPickup);
	scaleItemBoxSprite                   = new Trampoline(0x004C0790, 0x004C0795, ScaleItemBoxSprite);
	scaleBalls                           = new Trampoline(0x005C0B70, 0x005C0B75, ScaleBalls);
	scaleEmeraldRadarA                   = new Trampoline(0x00475A70, 0x00475A75, ScaleEmeraldRadarA);
	scaleEmeraldRadarB                   = new Trampoline(0x00475E50, 0x00475E55, ScaleEmeraldRadarB);
	scaleEmeraldRadar_Grab               = new Trampoline(0x00475D50, 0x00475D55, ScaleEmeraldRadar_Grab);
	scaleSandHillMultiplier              = new Trampoline(0x005991A0, 0x005991A6, ScaleSandHillMultiplier);
	scaleIceCapMultiplier                = new Trampoline(0x004EC120, 0x004EC125, ScaleIceCapMultiplier);
	scaleBossName                        = new Trampoline(0x004B33D0, 0x004B33D5, ScaleBossName);
	scaleMissionStartClear               = new Trampoline(0x00591260, 0x00591268, ScaleMissionStartClear);
	scaleMissionTimer                    = new Trampoline(0x00592D50, 0x00592D59, ScaleMissionTimer);
	scaleMissionCounter                  = new Trampoline(0x00592A60, 0x00592A68, ScaleMissionCounter);
	scaleTailsWinLose                    = new Trampoline(0x0047C480, 0x0047C485, ScaleTailsWinLose);
	scaleTailsRaceBar                    = new Trampoline(0x0047C260, 0x0047C267, ScaleTailsRaceBar);
	scaleDemoPressStart                  = new Trampoline(0x00457D30, 0x00457D36, ScaleDemoPressStart);
	late_exec_t                          = new Trampoline(0x004086F0, 0x004086F6, late_exec_r); // Sometimes used in a display function so we have to disable scaling temporarily

	// Big UI
	WriteData(reinterpret_cast<const float**>(0x0047024E), &patch_dummy);
	WriteData(reinterpret_cast<const float**>(0x004702E5), &patch_dummy);
	WriteData(reinterpret_cast<float**>(0x0047022F), &aspect_scale);
	WriteData(reinterpret_cast<float**>(0x004702D6), &aspect_scale);
	FishingHud_DrawReel_t      = new Trampoline(0x0046C9F0, 0x0046C9F5, FishingHud_DrawReel_r);
	FishingHud_DrawRod_t       = new Trampoline(0x0046CAB0, 0x0046CAB9, FishingHud_DrawRod_r);
	FishingHud_DrawMeters_t    = new Trampoline(0x0046CC70, 0x0046CC75, FishingHud_DrawMeters_r);
	FishingHud_DrawHIT_t       = new Trampoline(0x0046C920, 0x0046C926, FishingHud_DrawHIT_r);
	BigHud_DrawWeightAndLife_t = new Trampoline(0x0046FB00, 0x0046FB05, BigHud_DrawWeightAndLife_r);
	BigWeightBonus_Display_t   = new Trampoline(0x0046F580, 0x0046F585, BigWeightBonus_Display_r);

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

	WriteData(reinterpret_cast<const float**>(0x006288C2), &patch_dummy);
	scaleTornadoHP = new Trampoline(0x00628490, 0x00628496, ScaleTornadoHP);

	// TODO: Consider tracking down the individual functions so that they can be individually aligned.
	scaleTwinkleCircuitHUD = new Trampoline(0x004DB5E0, 0x004DB5E5, ScaleTwinkleCircuitHUD);

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

	// Gamma
	WriteData(reinterpret_cast<const float**>(0x0049FF70), &patch_dummy);
	WriteData(reinterpret_cast<const float**>(0x004A005B), &patch_dummy);
	WriteData(reinterpret_cast<const float**>(0x004A0067), &patch_dummy);
	scaleGammaTimeAddHud    = new Trampoline(0x0049FDA0, 0x0049FDA5, ScaleGammaTimeAddHud);
	scaleGammaTimeRemaining = new Trampoline(0x004C51D0, 0x004C51D7, ScaleGammaTimeRemaining);

	// Emblem screen
	WriteData(reinterpret_cast<float**>(0x004B4470), &scale_h);
	WriteData(reinterpret_cast<float**>(0x004B444E), &scale_v);
	scaleEmblemScreen = new Trampoline(0x004B4200, 0x004B4205, ScaleEmblemScreen);

	// Nights
	WriteData(reinterpret_cast<float**>(0x005D701B), &scale_h);
	scaleNightsCards   = new Trampoline(0x005D73F0, 0x005D73F5, ScaleNightsCards);
	scaleNightsJackpot = new Trampoline(0x005D6E60, 0x005D6E67, ScaleNightsJackpot);

	// GameGear
	WriteData(reinterpret_cast<const float**>(0x0070144D), &patch_dummy);
	WriteData(reinterpret_cast<const float**>(0x0070146F), &patch_dummy);
	MiniGameCollectionMenu_t = new Trampoline(0x0050C010, 0x0050C017, MiniGameCollectionMenu_r);
	
	// Honeycomb transition
	WriteData(reinterpret_cast<const float**>(0x006FF5C8), &patch_dummy);
	WriteData(reinterpret_cast<const float**>(0x006FF5D9), &patch_dummy);
	WriteData(reinterpret_cast<const float**>(0x006FF5EB), &patch_dummy);
	WriteData(reinterpret_cast<const float**>(0x006FF603), &patch_dummy);
	WriteData(reinterpret_cast<const float**>(0x006FF611), &patch_dummy);
	WriteData(reinterpret_cast<const float**>(0x006FF632), &patch_dummy);
	WriteData(reinterpret_cast<const float**>(0x006FF657), &patch_dummy);

	// Game Over
	WriteData(reinterpret_cast<float**>(0x00625ED6), &scale_v);
	WriteData(reinterpret_cast<float**>(0x00625EF6), &scale_h);
	WriteCall(reinterpret_cast<void*>(0x00625F05), DrawRect_DrawNowMaybe_GameOverHH);
	DrawGameOver_t   = new Trampoline(0x0042BFD0, 0x0042BFD8, DrawGameOver_asm);   // Normal
	DrawGameOverTC_t = new Trampoline(0x004DACC0, 0x004DACC5, DrawGameOverTC_asm); // Twinkle Circuit
	DrawGameOverHH_t = new Trampoline(0x00625D00, 0x00625D09, DrawGameOverHH_asm); // Hedgehog Hammer

	// Recap
	WriteData(reinterpret_cast<const float**>(0x0064287A), &patch_dummy);
	WriteData(reinterpret_cast<const float**>(0x00642896), &patch_dummy);
	WriteCall(reinterpret_cast<void*>(0x00642427), njDrawTextureMemList_NoSkippedFrames_RecapText);
	RecapBackground_Main_t = new Trampoline(0x00643C90, 0x00643C95, RecapBackground_Main_r);

	InitializeChaoHUDs();
	hudscale::update();
}
