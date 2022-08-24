#include "stdafx.h"

// Quat -> Euler conversion: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

void QuaternionToNinjaAngle(Angle3* angle, NJS_QUATERNION* q)
{
	Float sinr_cosp = 2.0f * (q->re * q->im[0] + q->im[1] * q->im[2]);
	Float cosr_cosp = 1.0f - 2.0f * (q->im[0] * q->im[0] + q->im[1] * q->im[1]);
	angle->x = NJM_RAD_ANG(std::atan2(sinr_cosp, cosr_cosp));

	Float sinp = 2.0f * (q->re * q->im[1] - q->im[2] * q->im[0]);
	if (std::abs(sinp) >= 1.0f)
		angle->y = NJM_RAD_ANG(std::copysign(NJD_PI / 2, sinp)); // use 90 degrees if out of range
	else
		angle->y = NJM_RAD_ANG(std::asin(sinp));

	Float siny_cosp = 2.0f * (q->re * q->im[2] + q->im[0] * q->im[1]);
	Float cosy_cosp = 1.0f - 2.0f * (q->im[1] * q->im[1] + q->im[2] * q->im[2]);
	angle->z = NJM_RAD_ANG(std::atan2(siny_cosp, cosy_cosp));
}

void __cdecl LinearMotionA_r(int keyno, NJS_MKEY_A* key, Angle3* dst, unsigned int n)
{
	NJS_MKEY_A* key_o = &key[keyno];
	NJS_MKEY_A* key_n = &key[(keyno + 1) % n];

	Angle3 ang_orig = { key_o->key[0], key_o->key[1], key_o->key[2] };
	Angle3 ang_next = { key_n->key[0], key_n->key[1], key_n->key[2] };

	NJS_QUATERNION q1, q2;
	njXYZAngleToQuaternion(&ang_orig, &q1);
	njXYZAngleToQuaternion(&ang_next, &q2);

	Float diff = (Float)(key_n->keyframe - key_o->keyframe);

	NJS_QUATERNION r;
	njInterpolateQuaternion(&q1, &q2, (nj_motion_data_info_.frame - (float)key_o->keyframe) / (diff > 0 ? diff : 1.0f), &r);

	QuaternionToNinjaAngle(dst, &r);
}

static void __declspec(naked) LinearMotionA_asm()
{
	__asm
	{
		push[esp + 04h] // count
		push ebx // rot
		push ecx // key
		push eax // id
		call LinearMotionA_r
		pop eax // id
		pop ecx // key
		pop ebx // rot
		add esp, 4 // count
		retn
	}
}

void init_interpolationAnimFixes()
{
	WriteJump((void*)0x404C10, LinearMotionA_asm);
}
