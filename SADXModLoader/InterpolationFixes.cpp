#include "stdafx.h"

// Euler/Quat conversions: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
// Quat lerping: https://stackoverflow.com/a/46187052

struct MOTIONDATA_INFO
{
	void* mdata;
	void** key;
	unsigned int* nbkeys;
	float frame;
	unsigned int nbframes;
	unsigned int cnt;
	unsigned int type;
	NJS_MATRIX mtrx;
};

DataPointer(MOTIONDATA_INFO, nj_motion_data_info_, 0x3AB9910);

void NinjaAngleToQuaternion(NJS_QUATERNION* q, Angle3* ang)
{
	Float yaw = NJM_ANG_RAD(ang->z);
	Float pitch = NJM_ANG_RAD(ang->y);
	Float roll = NJM_ANG_RAD(ang->x);

	auto cy = cosf(yaw * 0.5f);
	auto sy = sinf(yaw * 0.5f);
	auto cp = cosf(pitch * 0.5f);
	auto sp = sinf(pitch * 0.5f);
	auto cr = cosf(roll * 0.5f);
	auto sr = sinf(roll * 0.5f);

	q->im[0] = sr * cp * cy - cr * sp * sy;
	q->im[1] = cr * sp * cy + sr * cp * sy;
	q->im[2] = cr * cp * sy - sr * sp * cy;
	q->re = cr * cp * cy + sr * sp * sy;
}

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

Float scalorQuaternion(NJS_QUATERNION* q1, NJS_QUATERNION* q2)
{
	return q1->im[0] * q2->im[0] + q1->im[1] * q2->im[1] + q1->im[2] * q2->im[2] + q1->re * q2->re;
}

void negateQuaternion(NJS_QUATERNION* q)
{
	q->im[0] = -q->im[0];
	q->im[1] = -q->im[1];
	q->im[2] = -q->im[2];
	q->re = -q->re;
}

void unitQuaternion(NJS_QUATERNION* q)
{
	float l = 1.0f / std::sqrt(scalorQuaternion(q, q));
	q->im[0] = l * q->im[0];
	q->im[1] = l * q->im[1];
	q->im[2] = l * q->im[2];
	q->re = l * q->re;
}

void lerpQuaternion(NJS_QUATERNION* q, NJS_QUATERNION* a, NJS_QUATERNION* b, Float t)
{
	if (scalorQuaternion(a, b) < 0.0f)
	{
		negateQuaternion(b);
	}

	q->im[0] = a->im[0] - t * (a->im[0] - b->im[0]);
	q->im[1] = a->im[1] - t * (a->im[1] - b->im[1]);
	q->im[2] = a->im[2] - t * (a->im[2] - b->im[2]);
	q->re = a->re - t * (a->re - b->re);
}

void nlerpQuaternion(NJS_QUATERNION* q, NJS_QUATERNION* a, NJS_QUATERNION* b, Float t)
{
	lerpQuaternion(q, a, b, t);
	unitQuaternion(q);
}

void __cdecl LinearMotionA_r(int keyno, NJS_MKEY_A* key, Angle3* dst, unsigned int n)
{
	NJS_MKEY_A* key_o = &key[keyno];
	NJS_MKEY_A* key_n = &key[(keyno + 1) % n];

	Angle3 ang_orig = { key_o->key[0], key_o->key[1], key_o->key[2] };
	Angle3 ang_next = { key_n->key[0], key_n->key[1], key_n->key[2] };

	NJS_QUATERNION q1, q2;
	NinjaAngleToQuaternion(&q1, &ang_orig);
	NinjaAngleToQuaternion(&q2, &ang_next);

	Float diff = (Float)(key_n->keyframe - key_o->keyframe);

	NJS_QUATERNION r;
	nlerpQuaternion(&r, &q1, &q2, (nj_motion_data_info_.frame - (float)key_o->keyframe) / (diff > 0 ? diff : 1.0f));

	QuaternionToNinjaAngle(dst, &r);
}

static void __declspec(naked) LinearMotionA_asm()
{
	__asm
	{
		push[esp + 04h] // int count
		push ebx // rot
		push ecx // key
		push eax // id
		call LinearMotionA_r
		pop eax // id
		pop ecx // key
		pop ebx // rot
		add esp, 4 // int count
		retn
	}
}

void init_interpolationAnimFixes()
{
	WriteJump((void*)0x404C10, LinearMotionA_asm);
}
