#include "stdafx.h"

#include "SADXModLoader.h"
#include "Trampoline.h"
#include <stack>

#include "HudScale.h"

// TODO: misc. 2D things (i.e lens flare), main menu, character select

#pragma region trampolines

static Trampoline* drawTrampoline;
static Trampoline* drawObjects;
static Trampoline* scaleRingLife;
static Trampoline* scaleScoreTime;
static Trampoline* scaleStageMission;
static Trampoline* scalePause;

#pragma endregion

#pragma region scale stack

static bool doScale = false;
static std::stack<bool> scale_stack;

static float scale = 1.0f;
static float last_h = 0.0f;
static float last_v = 0.0f;

static void __cdecl ScalePush(bool center_screen)
{
	scale_stack.push(center_screen);

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

#pragma endregion

FunctionPointer(void, ScoreDisplay_Main, (ObjectMaster*), 0x0042BCC0);
static void __cdecl ScaleResultScreen(ObjectMaster* _this)
{
	ScalePush(true);
	ScoreDisplay_Main(_this);
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

static void __cdecl ScaleA()
{
	ScalePush();
	VoidFunc(original, scaleRingLife->Target());
	original();
	ScalePop();
}

static void __cdecl ScaleB()
{
	ScalePush();
	VoidFunc(original, scaleScoreTime->Target());
	original();
	ScalePop();
}

static void __cdecl ScaleStageMission(ObjectMaster* _this)
{
	ScalePush(true);
	ObjectFunc(original, scaleStageMission->Target());
	original(_this);
	ScalePop();
}

static short __cdecl ScalePauseMenu()
{
	ScalePush(true);
	FunctionPointer(short, original, (void), scalePause->Target());
	short result = original();
	ScalePop();
	return result;
}

static void __cdecl Draw2DSpriteHax(NJS_SPRITE* sp, Int n, Float pri, Uint32 attr, char zfunc_type)
{
	if (sp == nullptr)
		return;

	FunctionPointer(void, original, (NJS_SPRITE* sp, Int n, Float pri, Uint32 attr, char zfunc_type), drawTrampoline->Target());

	if (!doScale)
	{
		original(sp, n, pri, attr, zfunc_type);
	}
	else
	{
		NJS_POINT2 old_scale = { sp->sx, sp->sy };
		NJS_POINT3 old_pos = sp->p;

		sp->sx *= scale;
		sp->sy *= scale;
		sp->p.x *= scale;
		sp->p.y *= scale;

		if (scale_stack.top())
		{
			if ((float)HorizontalResolution / last_v > 640.0f)
				sp->p.x += (float)HorizontalResolution / 8.0f;
			if ((float)VerticalResolution / last_h > 480.0f)
				sp->p.y += (float)VerticalResolution / 8.0f;
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

	drawTrampoline = new Trampoline(0x00404660, 0x00404666, (DetourFunction)Draw2DSpriteHax);
	drawObjects = new Trampoline(0x0040B540, 0x0040B546, (DetourFunction)DrawAllObjectsHax);
	WriteCall((void*)((size_t)drawObjects->Target() + 1), (void*)0x004128F0);

	scaleRingLife = new Trampoline(0x00425F90, 0x00425F95, (DetourFunction)ScaleA);
	scaleScoreTime = new Trampoline(0x00427F50, 0x00427F55, (DetourFunction)ScaleB);
	scaleStageMission = new Trampoline(0x00457120, 0x00457126, (DetourFunction)ScaleStageMission);

	scalePause = new Trampoline(0x00415420, 0x00415425, (DetourFunction)ScalePauseMenu);
	WriteCall(scalePause->Target(), (void*)0x40FDC0);
}
