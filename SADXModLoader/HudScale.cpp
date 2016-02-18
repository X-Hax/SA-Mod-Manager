#include "stdafx.h"

#include "SADXModLoader.h"
#include "Trampoline.h"
#include <stack>

#include "HudScale.h"

// TODO: misc. 2D things (i.e lens flare), main menu, character select

#pragma region trampolines

static void __cdecl Draw2DSpriteHax(NJS_SPRITE* sp, Int n, Float pri, Uint32 attr, char zfunc_type);
Trampoline drawTrampoline(0x00404660, 0x00404666, (DetourFunction)Draw2DSpriteHax);

// TODO: Give Trampoline a default constructor and stuff
static Trampoline* drawObjects;
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
static Trampoline* scaleGammaTimeAddHud;
static Trampoline* scaleEmblemScreen;
static Trampoline* scaleBossName;

#pragma endregion

#pragma region scale stack

enum class Align
{
	Auto,
	Left,
	Center,
	Right
};

static bool doScale = false;
static std::stack<Align, std::vector<Align>> scale_stack;

static const float patch_dummy = 1.0f;
static const float screen_third = 640.0f / 3.0f;

static float scale = 1.0f;
static float last_h = 0.0f;
static float last_v = 0.0f;

static void __cdecl ScalePush(Align align)
{
	scale_stack.push(align);

	if (doScale)
		return;

	last_h = HorizontalStretch;
	last_v = VerticalStretch;

	HorizontalStretch = 1.0f;
	VerticalStretch = 1.0f;

	doScale = true;
}

static void __cdecl ScalePop()
{
	scale_stack.pop();
	doScale = scale_stack.size() > 0;

	if (!doScale)
	{
		HorizontalStretch = last_h;
		VerticalStretch = last_v;
	}
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
	{
		HorizontalStretch = last_h;
		VerticalStretch = last_v;
	}

	VoidFunc(original, drawObjects->Target());
	original();

	if (doScale)
	{
		HorizontalStretch = 1.0f;
		VerticalStretch = 1.0f;
	}
}

#pragma endregion

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

static void __cdecl ScaleGammaTimeAddHud(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Right, a1, scaleGammaTimeAddHud);
}

static void __cdecl ScaleEmblemScreen(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Center, a1, scaleEmblemScreen);
}

static void __cdecl ScaleBossName(ObjectMaster* a1)
{
	ScaleObjFunc(Align::Center, a1, scaleBossName);
}

#ifdef _DEBUG
static std::vector<NJS_SPRITE*> sprites;
#endif

static void __cdecl Draw2DSpriteHax(NJS_SPRITE* sp, Int n, Float pri, Uint32 attr, char zfunc_type)
{
	if (sp == nullptr)
		return;

#ifdef _DEBUG
	if (std::find(sprites.begin(), sprites.end(), sp) == sprites.end())
		sprites.push_back(sp);
#endif

	FunctionPointer(void, original, (NJS_SPRITE* sp, Int n, Float pri, Uint32 attr, char zfunc_type), drawTrampoline.Target());

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
		Align top = scale_stack.top();

		if (top == Align::Auto)
		{
			if (sp->p.x < screen_third)
				top = Align::Left;
			else if (sp->p.x < screen_third * 2.0f)
				top = Align::Center;
			else
				top = Align::Right;
		}

		NJS_POINT2 old_scale = { sp->sx, sp->sy };
		NJS_POINT3 old_pos = sp->p;

		sp->sx *= scale;
		sp->sy *= scale;
		sp->p.x *= scale;
		sp->p.y *= scale;

		switch (top)
		{
			default:
				break;

			case Align::Center:
				if ((float)HorizontalResolution / last_v > 640.0f)
					sp->p.x += (float)HorizontalResolution / 8.0f;
				if ((float)VerticalResolution / last_h > 480.0f)
					sp->p.y += (float)VerticalResolution / 8.0f;
				break;

			case Align::Right:
				if ((float)HorizontalResolution / last_v > 640.0f)
					sp->p.x += (float)HorizontalResolution / 4.0f;
				if ((float)VerticalResolution / last_h > 480.0f)
					sp->p.y += (float)VerticalResolution / 4.0f;
				break;
		}

		original(sp, n, pri, attr | NJD_SPRITE_SCALE, zfunc_type);

		sp->p = old_pos;
		sp->sx = old_scale.x;
		sp->sy = old_scale.y;
	}
}

void SetupHudScale()
{
	scale = min(HorizontalStretch, VerticalStretch);
	WriteJump((void*)0x0042BEE0, ScaleResultScreen);

	drawObjects = new Trampoline(0x0040B540, 0x0040B546, (DetourFunction)DrawAllObjectsHax);
	WriteCall((void*)((size_t)drawObjects->Target() + 1), (void*)0x004128F0);

	scaleRingLife = new Trampoline(0x00425F90, 0x00425F95, (DetourFunction)ScaleA);
	scaleScoreTime = new Trampoline(0x00427F50, 0x00427F55, (DetourFunction)ScaleB);
	scaleStageMission = new Trampoline(0x00457120, 0x00457126, (DetourFunction)ScaleStageMission);

	scalePause = new Trampoline(0x00415420, 0x00415425, (DetourFunction)ScalePauseMenu);
	WriteCall(scalePause->Target(), (void*)0x40FDC0);

	scaleTargetLifeGague = new Trampoline(0x004B3830, 0x004B3837, (DetourFunction)ScaleTargetLifeGague);

	scaleScoreA = new Trampoline(0x00628330, 0x00628335, (DetourFunction)ScaleScoreA);

	WriteData((const float**)0x006288C2, &patch_dummy);
	scaleTornadoHP = new Trampoline(0x00628490, 0x00628496, (DetourFunction)ScaleTornadoHP);

	// TODO: Consider tracking down the individual functions so that they can be individually aligned.
	scaleTwinkleCircuitHUD = new Trampoline(0x004DB5E0, 0x004DB5E5, (DetourFunction)ScaleTwinkleCircuitHUD);
	WriteCall(scaleTwinkleCircuitHUD->Target(), (void*)0x590620);

	// Rod scaling disabled.
	// TODO: Figure out how gauge is drawn (it doesn't use the texture).
	//scaleReel = new Trampoline(0x0046C9F0, 0x0046C9F5, (DetourFunction)ScaleReel);
	//scaleRod = new Trampoline(0x0046CAB0, 0x0046CAB9, (DetourFunction)ScaleRod);
	//scaleRodMeters = new Trampoline(0x0046CC70, 0x0046CC75, (DetourFunction)ScaleRodMeters);
	scaleFishingHit = new Trampoline(0x0046C920, 0x0046C926, (DetourFunction)ScaleFishingHit);
	scaleBigHud = new Trampoline(0x0046FB00, 0x0046FB05, (DetourFunction)ScaleBigHud);

	scaleAnimalPickup = new Trampoline(0x0046B330, 0x0046B335, (DetourFunction)ScaleAnimalPickup);

	scaleItemBoxSprite = new Trampoline(0x004C0790, 0x004C0795, (DetourFunction)ScaleItemBoxSprite);

	scaleBalls = new Trampoline(0x005C0B70, 0x005C0B75, (DetourFunction)ScaleBalls);

	scaleCheckpointTime = new Trampoline(0x004BABE0, 0x004BABE5, (DetourFunction)ScaleCheckpointTime);
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
	scaleEmeraldRadarA = new Trampoline(0x00475A70, 0x00475A75, (DetourFunction)ScaleEmeraldRadarA);
	scaleEmeraldRadarB = new Trampoline(0x00475E50, 0x00475E55, (DetourFunction)ScaleEmeraldRadarB);
	scaleEmeraldRadar_Grab = new Trampoline(0x00475D50, 0x00475D55, (DetourFunction)ScaleEmeraldRadar_Grab);

	scaleSandHillMultiplier = new Trampoline(0x005991A0, 0x005991A6, (DetourFunction)ScaleSandHillMultiplier);

	WriteData((const float**)0x0049FF70, &patch_dummy);
	WriteData((const float**)0x004A005B, &patch_dummy);
	WriteData((const float**)0x004A0067, &patch_dummy);
	scaleGammaTimeAddHud = new Trampoline(0x0049FDA0, 0x0049FDA5, (DetourFunction)ScaleGammaTimeAddHud);

	WriteData((float**)0x004B4470, &last_h);
	WriteData((float**)0x004B444E, &last_v);
	scaleEmblemScreen = new Trampoline(0x004B4200, 0x004B4205, (DetourFunction)ScaleEmblemScreen);

	scaleBossName = new Trampoline(0x004B33D0, 0x004B33D5, (DetourFunction)ScaleBossName);
}
