#include "stdafx.h"
#include "SADXModLoader.h"
#include "Trampoline.h"
#include "HudScale.h"

// TODO: Pause menu, misc. 2D things (i.e lens flare), main menu, character select

Trampoline trampoline(0x00404660, 0x00404666, (DetourFunction)Draw2DSpriteHax);
Trampoline scaleRingLife(0x00425F90, 0x00425F95, (DetourFunction)ScaleA);
Trampoline scaleScoreTime(0x00427F50, 0x00427F55, (DetourFunction)ScaleB);

static bool doScale = false;
static bool center_x = false;
static bool center_y = false;

static float scale = 1.0f;
static float last_h = 0.0f;
static float last_v = 0.0f;

FunctionPointer(void, ScoreDisplay_Main, (ObjectMaster*), 0x0042BCC0);
void ScaleResultScreen(ObjectMaster* _this)
{
	SaveScale();
	center_x = center_y = true;
	ScoreDisplay_Main(_this);
	RestoreScale();
}

void SetupHudScale()
{
	scale = min(HorizontalStretch, VerticalStretch);
	WriteJump((void*)0x0042BEE0, ScaleResultScreen);
}

void __cdecl SaveScale()
{
	doScale = true;

	last_h = HorizontalStretch;
	last_v = VerticalStretch;

	HorizontalStretch = 1.0f;
	VerticalStretch = 1.0f;
}

void __cdecl RestoreScale()
{
	HorizontalStretch = last_h;
	VerticalStretch = last_v;
	doScale = false;
	center_x = center_y = false;
}

void Draw2DSpriteHax(NJS_SPRITE* sp, Int n, Float pri, Uint32 attr, char zfunc_type)
{
	if (sp == nullptr)
		return;

	FunctionPointer(void, original, (NJS_SPRITE* sp, Int n, Float pri, Uint32 attr, char zfunc_type), trampoline.Target());
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

		if (center_x)
		{
			float w = (float)HorizontalResolution / last_v;
			if (w > 640.0f)
				sp->p.x += (float)HorizontalResolution / 8.0f;
		}
		if (center_y)
		{
			float h = (float)VerticalResolution / last_h;
			if (h > 480.0f)
				sp->p.y += (float)VerticalResolution / 8.0f;
		}

		original(sp, n, pri, attr | NJD_SPRITE_SCALE, zfunc_type);

		sp->p = old_pos;
		sp->sx = old_scale.x;
		sp->sy = old_scale.y;
	}
}

void __cdecl ScaleA()
{
	SaveScale();
	VoidFunc(original, scaleRingLife.Target());
	original();
	RestoreScale();
}

void __cdecl ScaleB()
{
	SaveScale();
	VoidFunc(original, scaleScoreTime.Target());
	original();
	RestoreScale();
}
