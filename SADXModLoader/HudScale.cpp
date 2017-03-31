#include "stdafx.h"

#include "SADXModLoader.h"
#include "Trampoline.h"
#include <stack>

#include "HudScale.h"

using std::stack;
using std::vector;

// TODO: "other" things

#pragma region trampolines

static void __cdecl Draw2DSprite_r(NJS_SPRITE* sp, Int n, Float pri, Uint32 attr, char zfunc_type);
static void __cdecl njDrawSprite2D_4_r(NJS_SPRITE *sp, Int n, Float pri, Uint32 attr);
static void __cdecl DrawRectPoints_r(NJS_POINT2 *points, float scale);
static void __cdecl sub_404490_r(NJS_POINT2COL *points, int count, float depth, int size, char flags);

Trampoline Draw2DSprite_t(0x00404660, 0x00404666, Draw2DSprite_r);
Trampoline njDrawSprite2D_4_t(0x004070A0, 0x004070A5, njDrawSprite2D_4_r);
Trampoline DrawRectPoints_t(0x0077E970, 0x0077E977, DrawRectPoints_r);
Trampoline sub_404490_t(0x00404490, 0x00404496, sub_404490_r);

static Trampoline* DisplayAllObjects_t;
static Trampoline* missionScreen;
static Trampoline* scaleRingLife;
static Trampoline* scaleScoreTime;
static Trampoline* scaleStageMission;
static Trampoline* scalePause;
static Trampoline* scaleTargetLifeGague;
static Trampoline* scaleScoreA;
static Trampoline* scaleTornadoHP;
static Trampoline* scaleTwinkleCircuitHUD;
static Trampoline* scaleFishingHit;
static Trampoline* scaleReel;
static Trampoline* scaleRod;
static Trampoline* scaleBigHud;
static Trampoline* scaleRodMeters;
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

#pragma endregion

#pragma region sprite stack

enum Align : Uint8
{
	Auto,
	HorizontalCenter = 1 << 0,
	VerticalCenter   = 1 << 1,
	Center           = HorizontalCenter | VerticalCenter,
	Left             = 1 << 2,
	Top              = 1 << 3,
	Right            = 1 << 4,
	Bottom           = 1 << 5
};

struct ScaleEntry
{
	Uint8 alignment;
	NJS_POINT2 scale;
};

static stack<ScaleEntry, vector<ScaleEntry>> scale_stack;
static size_t stack_size = 0;

static const float patch_dummy = 1.0f;

static const float third_h = 640.0f / 3.0f;
static const float third_v = 480.0f / 3.0f;

static bool do_scale  = false;
static float scale    = 0.0f;
static float scale_h  = 0.0f;
static float scale_v  = 0.0f;
static float region_w = 0.0f;
static float region_h = 0.0f;

static void __cdecl ScalePush(Uint8 align, float h = 1.0f, float v = 1.0f)
{
#ifdef _DEBUG
	if (ControllerPointers[0]->HeldButtons & Buttons_Z)
	{
		return;
	}
#endif

	scale_stack.push({ align, HorizontalStretch, VerticalStretch });

#ifdef _DEBUG
	if (scale_stack.size() > stack_size)
	{
		PrintDebug("SCALE STACK SIZE: %u/%u\n", (stack_size = scale_stack.size()), scale_stack._Get_container().capacity());
	}
#endif

	HorizontalStretch = h;
	VerticalStretch = v;

	do_scale = true;
}

static void __cdecl ScalePop()
{
	if (scale_stack.size() < 1)
	{
		return;
	}

	auto point = scale_stack.top();
	HorizontalStretch = point.scale.x;
	VerticalStretch = point.scale.y;

	scale_stack.pop();
	do_scale = scale_stack.size() > 0;
}

static void __cdecl DisplayAllObjects_r()
{
	if (do_scale)
	{
		ScalePush(Align::Auto, scale_h, scale_v);
	}

	auto original = (decltype(DisplayAllObjects_r)*)DisplayAllObjects_t->Target();
	original();

	if (do_scale)
	{
		ScalePop();
	}
}

// HACK: Remove when "other things" scaling is implemented
static Sint32 __cdecl FixMissionScreen()
{
	if (do_scale)
	{
		ScalePush(Align::Auto, scale_h, scale_v);
	}

	auto original = (decltype(FixMissionScreen)*)missionScreen->Target();
	Sint32 result = original();

	if (do_scale)
	{
		ScalePop();
	}

	return result;
}

static NJS_SPRITE last_sprite;

static NJS_POINT2 AutoAlign(Uint8 align, const NJS_POINT2& center)
{
	if (align == Align::Auto)
	{
		if (center.x < third_h)
		{
			align |= Align::Left;
		}
		else if (center.x < third_h * 2.0f)
		{
			align |= Align::HorizontalCenter;
		}
		else
		{
			align |= Align::Right;
		}

		if (center.y < third_v)
		{
			align |= Align::Top;
		}
		else if (center.y < third_v * 2.0f)
		{
			align |= Align::VerticalCenter;
		}
		else
		{
			align |= Align::Bottom;
		}
	}

	NJS_POINT2 result = {};

	if (align & Align::HorizontalCenter)
	{
		if ((float)HorizontalResolution / scale_v > 640.0f)
		{
			result.x = (float)HorizontalResolution / 2.0f - region_w / 2.0f;
		}
	}
	else if (align & Align::Right)
	{
		result.x = (float)HorizontalResolution - region_w;
	}

	if (align & Align::VerticalCenter)
	{
		if ((float)VerticalResolution / scale_h > 480.0f)
		{
			result.y = (float)VerticalResolution / 2.0f - region_h / 2.0f;
		}
	}
	else if (align & Align::Bottom)
	{
		result.y = (float)VerticalResolution - region_h;
	}

	return result;
}

static NJS_POINT2 AutoAlign(Uint8 align, const NJS_POINT3& center)
{
	return AutoAlign(align, *(NJS_POINT2*)&center);
}

static void __cdecl SpritePush(NJS_SPRITE* sp)
{
	if (scale_stack.empty())
	{
		return;
	}

	auto top = scale_stack.top();
	auto align = top.alignment;

	auto offset = AutoAlign(align, sp->p);

	last_sprite = *sp;

	sp->p.x = sp->p.x * scale + offset.x;
	sp->p.y = sp->p.y * scale + offset.y;
	sp->sx *= scale;
	sp->sy *= scale;
}

static void __cdecl SpritePop(NJS_SPRITE* sp)
{
	sp->p = last_sprite.p;
	sp->sx = last_sprite.sx;
	sp->sy = last_sprite.sy;
}

static void ScaleRect(NJS_POINT2* points, size_t count)
{
	if (scale_stack.empty())
	{
		return;
	}

	auto top = scale_stack.top();
	auto align = top.alignment;

	// 0 = bottom left
	// 1 = top left
	// 2 = bottom right
	// 3 = top right

	NJS_POINT2 center = {};
	auto m = 1.0f / (float)count;

	for (size_t i = 0; i < count; i++)
	{
		const auto& p = points[i];
		center.x += p.x * m;
		center.y += p.y * m;
	}

	auto offset = AutoAlign(align, center);

	for (size_t i = 0; i < count; i++)
	{
		auto& p = points[i];
		p.x = p.x * scale + offset.x;
		p.y = p.y * scale + offset.y;
	}
}

#ifdef _DEBUG
static vector<NJS_SPRITE*> sprites;

static void __cdecl StoreSprite(NJS_SPRITE* sp)
{
	if (find(sprites.begin(), sprites.end(), sp) == sprites.end())
	{
		sprites.push_back(sp);
	}
}
#endif

#pragma endregion

#pragma region template garbage

template<typename T, typename... Args>
void ScaleTrampoline(Uint8 align, const T&, const Trampoline* t, Args... args)
{
	ScalePush(align);
	((T*)t->Target())(args...);
	ScalePop();
}

template<typename R, typename T, typename... Args>
R ScaleTrampoline(Uint8 align, const T&, const Trampoline* t, Args... args)
{
	ScalePush(align);
	R result = ((T*)t->Target())(args...);
	ScalePop();
	return result;
}

#pragma endregion

#pragma region scale functions

static void __cdecl ScaleResultScreen(ObjectMaster* _this)
{
	ScalePush(Align::Center);
	ScoreDisplay_Main(_this);
	ScalePop();
}

static void __cdecl ScaleRingLife()
{
	ScaleTrampoline(Align::Auto, ScaleRingLife, scaleRingLife);
}
static void __cdecl ScaleScoreTime()
{
	ScaleTrampoline(Align::Left, ScaleScoreTime, scaleScoreTime);
}

static void __cdecl ScaleStageMission(ObjectMaster* _this)
{
	ScaleTrampoline(Align::Center, ScaleStageMission, scaleStageMission, _this);
}

static short __cdecl ScalePauseMenu()
{
	return ScaleTrampoline<short>(Align::Center, ScalePauseMenu, scalePause);
}

static void __cdecl ScaleTargetLifeGague(ObjectMaster* a1)
{
	ScaleTrampoline(Align::Right, ScaleTargetLifeGague, scaleTargetLifeGague, a1);
}

static void __cdecl ScaleScoreA()
{
	ScaleTrampoline(Align::Left, ScaleScoreA, scaleScoreA);
}

static void __cdecl ScaleTornadoHP(ObjectMaster* a1)
{
	ScaleTrampoline(Align::Left, ScaleTornadoHP, scaleTornadoHP, a1);
}

static void __cdecl ScaleTwinkleCircuitHUD(ObjectMaster* a1)
{
	ScaleTrampoline(Align::Center, ScaleTwinkleCircuitHUD, scaleTwinkleCircuitHUD, a1);
}

static void __cdecl ScaleFishingHit(ObjectMaster* a1)
{
	ScaleTrampoline(Align::Center, ScaleFishingHit, scaleFishingHit, a1);
}

static void __cdecl ScaleReel()
{
	ScaleTrampoline(Align::Auto, ScaleReel, scaleReel);
}
static void __cdecl ScaleRod()
{
	ScaleTrampoline(Align::Auto, ScaleRod, scaleRod);
}

static void __cdecl ScaleBigHud(ObjectMaster* a1)
{
	ScaleTrampoline(Align::Auto, ScaleBigHud, scaleBigHud, a1);
}
static void __cdecl ScaleRodMeters(float a1)
{
	ScaleTrampoline(Align::Auto, ScaleRodMeters, scaleRodMeters, a1);
}

static void __cdecl ScaleAnimalPickup(ObjectMaster* a1)
{
	ScaleTrampoline(Align::Right | Align::Bottom, ScaleAnimalPickup, scaleAnimalPickup, a1);
}

static void __cdecl ScaleItemBoxSprite(ObjectMaster* a1)
{
	ScaleTrampoline(Align::Bottom | Align::HorizontalCenter, ScaleItemBoxSprite, scaleItemBoxSprite, a1);
}

static void __cdecl ScaleBalls(ObjectMaster* a1)
{
	ScaleTrampoline(Align::Right, ScaleBalls, scaleBalls, a1);
}

static void __cdecl ScaleCheckpointTime(int a1, int a2, int a3)
{
	ScaleTrampoline(Align::Right | Align::Bottom, ScaleCheckpointTime, scaleCheckpointTime, a1, a2, a3);
}

static void __cdecl ScaleEmeraldRadarA(ObjectMaster* a1)
{
	ScaleTrampoline(Align::Auto, ScaleEmeraldRadarA, scaleEmeraldRadarA, a1);
}
static void __cdecl ScaleEmeraldRadar_Grab(ObjectMaster* a1)
{
	ScaleTrampoline(Align::Auto, ScaleEmeraldRadar_Grab, scaleEmeraldRadar_Grab, a1);
}
static void __cdecl ScaleEmeraldRadarB(ObjectMaster* a1)
{
	ScaleTrampoline(Align::Auto, ScaleEmeraldRadarB, scaleEmeraldRadarB, a1);
}

static void __cdecl ScaleSandHillMultiplier(ObjectMaster* a1)
{
	ScaleTrampoline(Align::Auto, ScaleSandHillMultiplier, scaleSandHillMultiplier, a1);
}
static void __cdecl ScaleIceCapMultiplier(ObjectMaster* a1)
{
	ScaleTrampoline(Align::Auto, ScaleIceCapMultiplier, scaleIceCapMultiplier, a1);
}

static void __cdecl ScaleGammaTimeAddHud(ObjectMaster* a1)
{
	ScaleTrampoline(Align::Right, ScaleGammaTimeAddHud, scaleGammaTimeAddHud, a1);
}
static void __cdecl ScaleGammaTimeRemaining(ObjectMaster* a1)
{
	ScaleTrampoline(Align::Bottom | Align::HorizontalCenter, ScaleGammaTimeRemaining, scaleGammaTimeRemaining, a1);
}

static void __cdecl ScaleEmblemScreen(ObjectMaster* a1)
{
	ScaleTrampoline(Align::Center, ScaleEmblemScreen, scaleEmblemScreen, a1);
}

static void __cdecl ScaleBossName(ObjectMaster* a1)
{
	ScaleTrampoline(Align::Center, ScaleBossName, scaleBossName, a1);
}

static void __cdecl ScaleNightsCards(ObjectMaster* a1)
{
	ScaleTrampoline(Align::Auto, ScaleNightsCards, scaleNightsCards, a1);
}
static void __cdecl ScaleNightsJackpot(ObjectMaster* a1)
{
	ScaleTrampoline(Align::Auto, ScaleNightsJackpot, scaleNightsJackpot, a1);
}

static void __cdecl ScaleMissionStartClear(ObjectMaster* a1)
{
	ScaleTrampoline(Align::Center, ScaleMissionStartClear, scaleMissionStartClear, a1);
}
static void __cdecl ScaleMissionTimer()
{

	ScaleTrampoline(Align::Center, ScaleMissionTimer, scaleMissionTimer);
}
static void __cdecl ScaleMissionCounter()
{
	ScaleTrampoline(Align::Center, ScaleMissionCounter, scaleMissionCounter);
}

static void __cdecl ScaleTailsWinLose(ObjectMaster* a1)
{
	ScaleTrampoline(Align::Center, ScaleTailsWinLose, scaleTailsWinLose, a1);
}
static void __cdecl ScaleTailsRaceBar(ObjectMaster* a1)
{
	ScaleTrampoline(Align::HorizontalCenter | Align::Bottom, ScaleTailsRaceBar, scaleTailsRaceBar, a1);
}

static void __cdecl ScaleDemoPressStart(ObjectMaster* a1)
{
	ScaleTrampoline(Align::Right, ScaleDemoPressStart, scaleDemoPressStart, a1);
}

static void __cdecl ChaoDX_Message_PlayerAction_Load_r()
{
	ScaleTrampoline(Align::Auto, ChaoDX_Message_PlayerAction_Load_r, ChaoDX_Message_PlayerAction_Load_t);
}

static void __cdecl ChaoDX_Message_PlayerAction_Display_r(ObjectMaster* a1)
{
	ScaleTrampoline(Align::Top | Align::Right,
		ChaoDX_Message_PlayerAction_Display_r, ChaoDX_Message_PlayerAction_Display_t, a1);
}

#pragma endregion

static void __cdecl Draw2DSprite_r(NJS_SPRITE* sp, Int n, Float pri, Uint32 attr, char zfunc_type)
{
	if (sp == nullptr)
	{
		return;
	}

	FunctionPointer(void, original, (NJS_SPRITE*, Int, Float, Uint32, char), Draw2DSprite_t.Target());

#ifdef _DEBUG
	StoreSprite(sp);
#endif

	if (!do_scale || sp == (NJS_SPRITE*)0x009BF3B0)
	{
		// Scales lens flare and sun.
		// It uses njProjectScreen so there's no position scaling required.
		if (sp == (NJS_SPRITE*)0x009BF3B0)
		{
			sp->sx *= scale;
			sp->sy *= scale;
		}

		original(sp, n, pri, attr, zfunc_type);
	}
	else
	{
		SpritePush(sp);
		original(sp, n, pri, attr | NJD_SPRITE_SCALE, zfunc_type);
		SpritePop(sp);
	}
}

static void __cdecl njDrawSprite2D_4_r(NJS_SPRITE *sp, Int n, Float pri, Uint32 attr)
{
	if (sp == nullptr)
	{
		return;
	}

	FunctionPointer(void, original, (NJS_SPRITE*, Int, Float, Uint32), njDrawSprite2D_4_t.Target());

#ifdef _DEBUG
	StoreSprite(sp);
#endif

	if (!do_scale)
	{
		original(sp, n, pri, attr);
	}
	else
	{
		SpritePush(sp);
		original(sp, n, pri, attr | NJD_SPRITE_SCALE);
		SpritePop(sp);
	}
}

static void __cdecl DrawRectPoints_r(NJS_POINT2* points, float scale)
{
	if (points == nullptr)
	{
		return;
	}

	FunctionPointer(void, original, (NJS_POINT2*, float), DrawRectPoints_t.Target());

	if (!do_scale)
	{
#ifndef  _DEBUG
		original(points, scale);
		return;
#else
		ScalePush(Align::Top | Align::Left);
		ScaleRect(points, 2);
		original(points, scale);
		ScalePop();
		return;
#endif
	}

	ScaleRect(points, 2);
	original(points, scale);
}

static void __cdecl sub_404490_r(NJS_POINT2COL* points, int count, float depth, int size, char flags)
{
	if (points == nullptr)
	{
		return;
	}

	FunctionPointer(void, original, (NJS_POINT2COL*, int, float, int, char), sub_404490_t.Target());

	if (!do_scale)
	{
		original(points, count, depth, size, flags);
		return;
	}

	ScaleRect(points->p, points->num);
	original(points, count, depth, size, flags);
}

void SetHudScaleValues()
{
	scale_h = HorizontalStretch;
	scale_v = VerticalStretch;

	scale = min(scale_h, scale_v);

	region_w = 640.0f * scale;
	region_h = 480.0f * scale;
}

#ifdef _DEBUG
static Sint32 __fastcall wangis_r(int n);
static Trampoline wangis(0x0077F440, 0x0077F445, wangis_r);
static Sint32 __fastcall wangis_r(int n)
{
	if (n == 2 && *(NJS_TEXLIST**)0x03D0FA24 == (NJS_TEXLIST*)0x33A1040)
	{
		PrintDebug("\aNUMBERWANG\n");
	}
	FastcallFunctionPointer(Sint32, wonjis, (int), wangis.Target());
	return wonjis(n);
}
#endif

void SetupHudScale()
{
	SetHudScaleValues();

	// Fixes character scale on character select screen.
	// TODO: dynamically update character Z position on resolution change.
	WriteData((float**)0x0051285E, &scale);

	WriteJump((void*)0x0042BEE0, ScaleResultScreen);

	DisplayAllObjects_t = new Trampoline(0x0040B540, 0x0040B546, DisplayAllObjects_r);
	WriteCall((void*)((size_t)DisplayAllObjects_t->Target() + 1), (void*)0x004128F0);

	missionScreen = new Trampoline(0x00590E60, 0x00590E65, FixMissionScreen);

	scaleRingLife = new Trampoline(0x00425F90, 0x00425F95, ScaleRingLife);
	scaleScoreTime = new Trampoline(0x00427F50, 0x00427F55, ScaleScoreTime);
	scaleStageMission = new Trampoline(0x00457120, 0x00457126, ScaleStageMission);

	scalePause = new Trampoline(0x00415420, 0x00415425, ScalePauseMenu);
	WriteCall(scalePause->Target(), (void*)0x40FDC0);

	scaleTargetLifeGague = new Trampoline(0x004B3830, 0x004B3837, ScaleTargetLifeGague);

	scaleScoreA = new Trampoline(0x00628330, 0x00628335, ScaleScoreA);

	WriteData((const float**)0x006288C2, &patch_dummy);
	scaleTornadoHP = new Trampoline(0x00628490, 0x00628496, ScaleTornadoHP);

	// TODO: Consider tracking down the individual functions so that they can be individually aligned.
	scaleTwinkleCircuitHUD = new Trampoline(0x004DB5E0, 0x004DB5E5, ScaleTwinkleCircuitHUD);
	WriteCall(scaleTwinkleCircuitHUD->Target(), (void*)0x590620);

	// Rod scaling
	scaleReel = new Trampoline(0x0046C9F0, 0x0046C9F5, ScaleReel);
	scaleRod = new Trampoline(0x0046CAB0, 0x0046CAB9, ScaleRod);
	scaleRodMeters = new Trampoline(0x0046CC70, 0x0046CC75, ScaleRodMeters);
	scaleFishingHit = new Trampoline(0x0046C920, 0x0046C926, ScaleFishingHit);
	scaleBigHud = new Trampoline(0x0046FB00, 0x0046FB05, ScaleBigHud);

	scaleAnimalPickup = new Trampoline(0x0046B330, 0x0046B335, ScaleAnimalPickup);

	scaleItemBoxSprite = new Trampoline(0x004C0790, 0x004C0795, ScaleItemBoxSprite);

	scaleBalls = new Trampoline(0x005C0B70, 0x005C0B75, ScaleBalls);

	scaleCheckpointTime = new Trampoline(0x004BABE0, 0x004BABE5, ScaleCheckpointTime);
	WriteData((const float**)0x0044F2E1, &patch_dummy);
	WriteData((const float**)0x0044F30B, &patch_dummy);
	WriteData((const float**)0x00476742, &patch_dummy);
	WriteData((const float**)0x0047676A, &patch_dummy);

	// EmeraldRadarHud_Load
	WriteData((const float**)0x00475BE3, &patch_dummy);
	WriteData((const float**)0x00475C00, &patch_dummy);
	// Emerald get
	WriteData((const float**)0x00477E8E, &patch_dummy);
	WriteData((const float**)0x00477EC0, &patch_dummy);

	scaleEmeraldRadarA = new Trampoline(0x00475A70, 0x00475A75, ScaleEmeraldRadarA);
	scaleEmeraldRadarB = new Trampoline(0x00475E50, 0x00475E55, ScaleEmeraldRadarB);
	scaleEmeraldRadar_Grab = new Trampoline(0x00475D50, 0x00475D55, ScaleEmeraldRadar_Grab);

	scaleSandHillMultiplier = new Trampoline(0x005991A0, 0x005991A6, ScaleSandHillMultiplier);
	scaleIceCapMultiplier = new Trampoline(0x004EC120, 0x004EC125, ScaleIceCapMultiplier);

	WriteData((const float**)0x0049FF70, &patch_dummy);
	WriteData((const float**)0x004A005B, &patch_dummy);
	WriteData((const float**)0x004A0067, &patch_dummy);
	scaleGammaTimeAddHud = new Trampoline(0x0049FDA0, 0x0049FDA5, ScaleGammaTimeAddHud);
	scaleGammaTimeRemaining = new Trampoline(0x004C51D0, 0x004C51D7, ScaleGammaTimeRemaining);

	WriteData((float**)0x004B4470, &scale_h);
	WriteData((float**)0x004B444E, &scale_v);
	scaleEmblemScreen = new Trampoline(0x004B4200, 0x004B4205, ScaleEmblemScreen);

	scaleBossName = new Trampoline(0x004B33D0, 0x004B33D5, ScaleBossName);

	scaleNightsCards = new Trampoline(0x005D73F0, 0x005D73F5, ScaleNightsCards);
	WriteData((float**)0x005D701B, &scale_h);
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
	WriteCall(ChaoDX_Message_PlayerAction_Display_t->Target(), (void*)0x590620);
#endif
}
