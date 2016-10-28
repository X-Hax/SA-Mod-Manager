#include "stdafx.h"

#include "SADXModLoader.h"
#include "Trampoline.h"
#include <stack>

#include "HudScale.h"

using std::stack;
using std::vector;

// TODO: "other" things

#pragma region trampolines

static void __cdecl Draw2DSpriteHax(NJS_SPRITE* sp, Int n, Float pri, Uint32 attr, char zfunc_type);
void __cdecl njDrawSprite2D_4_Hax(NJS_SPRITE *sp, Int n, Float pri, Uint32 attr);

Trampoline drawSprite(0x00404660, 0x00404666, Draw2DSpriteHax);
Trampoline drawSpriteWrapper(0x004070A0, 0x004070A5, njDrawSprite2D_4_Hax);

// TODO: Give Trampoline a default constructor and stuff
static Trampoline* drawObjects;
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
static Trampoline* scaleTailsOtherThing;
static Trampoline* scaleDemoPressStart;

#pragma endregion

#pragma region scale stack

enum class Align : Uint8
{
	Auto,
	Left,
	Center,
	Right
};

struct ScaleEntry
{
	Align alignment;
	NJS_POINT2 scale;
};

static stack<ScaleEntry, vector<ScaleEntry>> scale_stack;
static size_t stack_size = 0;

static const float patch_dummy  = 1.0f;
static const float screen_third = 640.0f / 3.0f;

static bool doScale   = false;
static float scale    = 0.0f;
static float scale_h  = 0.0f;
static float scale_v  = 0.0f;
static float region_w = 0.0f;
static float region_h = 0.0f;

static void __cdecl ScalePush(Align align, float h = 1.0f, float v = 1.0f)
{
#ifdef _DEBUG
	if (ControllerPointers[0]->HeldButtons & Buttons_Z)
		return;
#endif

	scale_stack.push({ align, HorizontalStretch, VerticalStretch });

#ifdef _DEBUG
	if (scale_stack.size() > stack_size)
		PrintDebug("SCALE STACK SIZE: %u/%u\n", (stack_size = scale_stack.size()), scale_stack._Get_container().capacity());
#endif

	HorizontalStretch = h;
	VerticalStretch = v;

	doScale = true;
}

static void __cdecl ScalePop()
{
	if (scale_stack.size() < 1)
		return;

	auto point = scale_stack.top();
	HorizontalStretch = point.scale.x;
	VerticalStretch = point.scale.y;

	scale_stack.pop();
	doScale = scale_stack.size() > 0;
}

static inline void __stdcall ScaleObjFunc(Align align, ObjectMaster* a1, Trampoline* trampoline)
{
	ScalePush(align);
	((ObjectFuncPtr)trampoline->Target())(a1);
	ScalePop();
}
static inline void __stdcall ScaleVoidFunc(Align align, Trampoline* trampoline)
{
	ScalePush(align);
	((void (__cdecl*)(void))trampoline->Target())();
	ScalePop();
}

static void __cdecl DrawAllObjectsHax()
{
	if (doScale)
		ScalePush(Align::Auto, scale_h, scale_v);

	VoidFunc(original, drawObjects->Target());
	original();

	if (doScale)
		ScalePop();
}

// HACK: Remove when "other things" scaling is implemented
static Sint32 __cdecl FixMissionScreen()
{
	if (doScale)
		ScalePush(Align::Auto, scale_h, scale_v);

	FunctionPointer(Sint32, original, (void), missionScreen->Target());
	Sint32 result = original();

	if (doScale)
		ScalePop();

	return result;
}

#pragma endregion

#pragma region sprite stack

static NJS_SPRITE last_sprite;

static void __cdecl SpritePush(NJS_SPRITE* sp)
{
	auto top = scale_stack.top();
	Align align = top.alignment;

	if (align == Align::Auto)
	{
		if (sp->p.x < screen_third)
			align = Align::Left;
		else if (sp->p.x < screen_third * 2.0f)
			align = Align::Center;
		else
			align = Align::Right;
	}

	last_sprite = *sp;

	sp->p.x *= scale;
	sp->p.y *= scale;
	sp->sx  *= scale;
	sp->sy  *= scale;

	switch (align)
	{
		default:
			break;

		case Align::Center:
			if ((float)HorizontalResolution / scale_v > 640.0f)
				sp->p.x += (float)HorizontalResolution / 2.0f - region_w / 2.0f;

			if ((float)VerticalResolution / scale_h > 480.0f)
				sp->p.y += (float)VerticalResolution / 2.0f - region_h / 2.0f;

			break;

		case Align::Right:
			sp->p.x += (float)HorizontalResolution - region_w;
			break;
	}
}

static void __cdecl SpritePop(NJS_SPRITE* sp)
{
	sp->p = last_sprite.p;
	sp->sx = last_sprite.sx;
	sp->sy = last_sprite.sy;
}

#ifdef _DEBUG
static vector<NJS_SPRITE*> sprites;
#endif

static void __cdecl StoreSprite(NJS_SPRITE* sp)
{
#ifdef _DEBUG
	if (find(sprites.begin(), sprites.end(), sp) == sprites.end())
		sprites.push_back(sp);
#endif
}

#pragma endregion

#pragma region scale functions

static void __cdecl ScaleResultScreen(ObjectMaster* _this)
{
	ScalePush(Align::Center);
	FunctionPointer(void, ScoreDisplay_Main, (ObjectMaster*), 0x0042BCC0);
	ScoreDisplay_Main(_this);
	ScalePop();
}

static void __cdecl ScaleA()
{
	ScaleVoidFunc(Align::Left, scaleRingLife);
}
static void __cdecl ScaleB()
{
	ScaleVoidFunc(Align::Left, scaleScoreTime);
}

static void __cdecl ScaleStageMission(ObjectMaster* _this)
{
	ScaleObjFunc(Align::Center, _this, scaleStageMission);
}

static short __cdecl ScalePauseMenu()
{
	ScalePush(Align::Center);
	FunctionPointer(short, original, (void), scalePause->Target());
	short result = original();
	ScalePop();
	return result;
}

static void __cdecl ScaleTargetLifeGague(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Right, a1, scaleTargetLifeGague);
}

static void __cdecl ScaleScoreA()
{
	ScaleVoidFunc(Align::Left, scaleScoreA);
}

static void __cdecl ScaleTornadoHP(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Left, a1, scaleTornadoHP);
}

static void __cdecl ScaleTwinkleCircuitHUD(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Center, a1, scaleTwinkleCircuitHUD);
}

static void __cdecl ScaleFishingHit(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Center, a1, scaleFishingHit);
}
static void __cdecl ScaleReel()
{
	ScaleVoidFunc(Align::Right, scaleReel);
}
static void __cdecl ScaleRod()
{
	ScaleVoidFunc(Align::Right, scaleRod);
}
static void __cdecl ScaleBigHud(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Left, a1, scaleBigHud);
}
static void __cdecl ScaleRodMeters(float a1)
{
	ScalePush(Align::Right);
	FunctionPointer(void, original, (float), scaleRodMeters->Target());
	original(a1);
	ScalePop();
}

static void __cdecl ScaleAnimalPickup(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Right, a1, scaleAnimalPickup);
}

static void __cdecl ScaleItemBoxSprite(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Center, a1, scaleItemBoxSprite);
}

static void __cdecl ScaleBalls(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Right, a1, scaleBalls);
}

static void __cdecl ScaleCheckpointTime(int a1, int a2, int a3)
{
	ScalePush(Align::Right);
	FunctionPointer(void, original, (int, int, int), scaleCheckpointTime->Target());
	original(a1, a2, a3);
	ScalePop();
}

static void __cdecl ScaleEmeraldRadarA(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Auto, a1, scaleEmeraldRadarA);
}
static void __cdecl ScaleEmeraldRadar_Grab(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Auto, a1, scaleEmeraldRadar_Grab);
}
static void __cdecl ScaleEmeraldRadarB(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Auto, a1, scaleEmeraldRadarB);
}

static void __cdecl ScaleSandHillMultiplier(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Auto, a1, scaleSandHillMultiplier);
}
static void __cdecl ScaleIceCapMultiplier(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Auto, a1, scaleIceCapMultiplier);
}

static void __cdecl ScaleGammaTimeAddHud(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Right, a1, scaleGammaTimeAddHud);
}
static void __cdecl ScaleGammaTimeRemaining(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Center, a1, scaleGammaTimeRemaining);
}

static void __cdecl ScaleEmblemScreen(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Center, a1, scaleEmblemScreen);
}

static void __cdecl ScaleBossName(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Center, a1, scaleBossName);
}

static void __cdecl ScaleNightsCards(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Auto, a1, scaleNightsCards);
}
static void __cdecl ScaleNightsJackpot(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Auto, a1, scaleNightsJackpot);
}

static void __cdecl ScaleMissionStartClear(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Center, a1, scaleMissionStartClear);
}
static void __cdecl ScaleMissionTimer()
{
	ScaleVoidFunc(Align::Center, scaleMissionTimer);
}
static void __cdecl ScaleMissionCounter()
{
	ScaleVoidFunc(Align::Center, scaleMissionCounter);
}

static void __cdecl ScaleTailsWinLose(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Center, a1, scaleTailsWinLose);
}
static void __cdecl ScaleTailsOtherThing(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Center, a1, scaleTailsOtherThing);
}

static void __cdecl ScaleDemoPressStart(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Right, a1, scaleDemoPressStart);
}

#pragma endregion

static void __cdecl Draw2DSpriteHax(NJS_SPRITE* sp, Int n, Float pri, Uint32 attr, char zfunc_type)
{
	if (sp == nullptr)
		return;

	FunctionPointer(void, original, (NJS_SPRITE*, Int, Float, Uint32, char), drawSprite.Target());

	StoreSprite(sp);

	if (!doScale || sp == (NJS_SPRITE*)0x009BF3B0)
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

void __cdecl njDrawSprite2D_4_Hax(NJS_SPRITE *sp, Int n, Float pri, Uint32 attr)
{
	if (sp == nullptr)
		return;

	FunctionPointer(void, original, (NJS_SPRITE*, Int, Float, Uint32), drawSpriteWrapper.Target());

	StoreSprite(sp);

	if (!doScale)
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

void SetHudScaleValues()
{
	scale_h = HorizontalStretch;
	scale_v = VerticalStretch;

	scale = min(scale_h, scale_v);

	region_w = 640.0f * scale;
	region_h = 480.0f * scale;
}

void SetupHudScale()
{
	SetHudScaleValues();

	WriteJump((void*)0x0042BEE0, ScaleResultScreen);

	drawObjects = new Trampoline(0x0040B540, 0x0040B546, DrawAllObjectsHax);
	WriteCall((void*)((size_t)drawObjects->Target() + 1), (void*)0x004128F0);

	missionScreen = new Trampoline(0x00590E60, 0x00590E65, FixMissionScreen);

	scaleRingLife = new Trampoline(0x00425F90, 0x00425F95, ScaleA);
	scaleScoreTime = new Trampoline(0x00427F50, 0x00427F55, ScaleB);
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

	// Rod scaling disabled.
	// TODO: Figure out how gauge is drawn (it doesn't appear to use the texture).
	//scaleReel = new Trampoline(0x0046C9F0, 0x0046C9F5, ScaleReel);
	//scaleRod = new Trampoline(0x0046CAB0, 0x0046CAB9, ScaleRod);
	//scaleRodMeters = new Trampoline(0x0046CC70, 0x0046CC75, ScaleRodMeters);
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
	scaleTailsOtherThing = new Trampoline(0x0047C260, 0x0047C267, ScaleTailsOtherThing);

	scaleDemoPressStart = new Trampoline(0x00457D30, 0x00457D36, ScaleDemoPressStart);
}
