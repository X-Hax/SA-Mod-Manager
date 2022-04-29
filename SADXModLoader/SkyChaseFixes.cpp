#include "stdafx.h"
#include <SADXModLoader.h>
#include "SkyChaseFixes.h"

static float HorizontalResolution_float = 640.0f;
static float VerticalResolution_float = 480.0f;
static float VerticalResolutionHalf_float = 240.0f;

static const float float_one = 1.0f;
static float target_collision_size = 1.0f;
static float target_speed_multiplier = 2.0f;

static const double SkyRotationMultiplier = -0.5;

template <typename T>
T clamp(T value, T low, T high)
{
	if (value < low)
	{
		return low;
	}

	if (value > high)
	{
		return high;
	}

	return value;
}

static void __cdecl TornadoTarget_MoveTargetWithinBounds_r(task* tp)
{
	auto pos = &tp->twp->pos;

	const float m = min(VerticalStretch, HorizontalStretch);
	const float move_speed = m * 0.000308093f;

	// 640x480 - 160x160 margin
	const float w = 480.0f * m;
	const float h = 320.0f * m;

	const float left = (HorizontalResolution_float - w) / 2.0f;
	const float top = (VerticalResolution_float - h) / 2.0f;
	const float right = left + w;
	const float bottom = top + h;

	const float x = (static_cast<float>(static_cast<int>(Controllers[0].LeftStickX) << 8) * move_speed) + pos->x;
	const float y = (static_cast<float>(static_cast<int>(Controllers[0].LeftStickY) << 8) * move_speed) + pos->y;

	pos->x = clamp(x, left, right);
	pos->y = clamp(y, top, bottom);
}

static void __declspec(naked) TornadoTarget_MoveTargetWithinBounds_asm()
{
	__asm
	{
		push eax
		call TornadoTarget_MoveTargetWithinBounds_r
		pop eax
		retn
	}
}

static void __cdecl TornadoTarget_CalculateCenterPoint_r(task* tp)
{
	auto pos = &tp->twp->pos;
	auto parent_pos = &tp->ptp->twp->pos;

	pos->x = parent_pos->x - _nj_screen_.cx;
	pos->y = parent_pos->y - _nj_screen_.cy;
	pos->z = 1000.0f * min(VerticalStretch, HorizontalStretch);

	njPushMatrix(nullptr);
	njInvertMatrix(nullptr);
	njCalcPoint(nullptr, pos, pos);
	njPopMatrixEx();
}

static void __cdecl TornadoFixTargetSpriteScale(NJS_SPRITE* sp, Int n, Float pri, NJD_SPRITE attr)
{
	if (!IsGamePaused())
	{
		sp->sy = sp->sy * min(VerticalStretch, HorizontalStretch);
		njDrawSprite2D_ForcePriority(sp, n, pri, attr);
	}
}

void SkyChaseFix_UpdateBounds()
{
	// Resolution related fixes
	HorizontalResolution_float = static_cast<float>(HorizontalResolution);
	VerticalResolution_float = static_cast<float>(VerticalResolution);
	VerticalResolutionHalf_float = VerticalResolution_float / 2.0f;

	auto m = min(VerticalStretch, HorizontalStretch);

	// Sky Chase reticle and multiplier fixes
	target_speed_multiplier = m;
	target_collision_size = 1024.0f * m;

	WriteData(reinterpret_cast<float*>(0x00628951), m); // Reticle scale X
	WriteData(reinterpret_cast<float*>(0x0062895B), m); // Reticle scale Y
}

void SkyChaseFix_Init()
{
	SkyChaseFix_UpdateBounds();

	WriteJump(reinterpret_cast<void*>(0x00628970), TornadoTarget_MoveTargetWithinBounds_asm);

	WriteData(reinterpret_cast<const float**>(0x00627F4D), &float_one); // Tornado Speed (always 1)
	WriteData(reinterpret_cast<const float**>(0x00627F60), &float_one); // Horizontal limit
	WriteData(reinterpret_cast<const float**>(0x00627F72), &float_one); // Vertical limit

	WriteJump(reinterpret_cast<void*>(0x628D50), TornadoTarget_CalculateCenterPoint_r); // Calculate center for bullets

	// Hodai fixes
	WriteData(reinterpret_cast<float**>(0x0043854D), &HorizontalResolution_float);
	WriteData(reinterpret_cast<float**>(0x00438571), &VerticalResolutionHalf_float);
	WriteData(reinterpret_cast<float**>(0x0043857F), &VerticalResolutionHalf_float);

	WriteData(reinterpret_cast<float**>(0x628AF7), &target_collision_size);     // Target size
	WriteData(reinterpret_cast<float**>(0x00629472), &target_speed_multiplier); // Target speed

	// Visual stuff
	WriteData(reinterpret_cast<const double**>(0x00627D14), &SkyRotationMultiplier);
	WriteCall(reinterpret_cast<void*>(0x00628E4D), TornadoFixTargetSpriteScale);
}