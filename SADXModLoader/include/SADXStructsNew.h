/**
 * SADX Mod Loader.
 * SADX structs based on symbols.
 */

#ifndef SADXMODLOADER_SADXSTRUCTSNEW_H
#define SADXMODLOADER_SADXSTRUCTSNEW_H

#include <stdint.h>
#include "ninja.h"
#include "d3d8types.h"
#include "SADXEnums.h"

 // All structs should be packed.
#pragma pack(push, 1)

struct task;
struct taskwk;

struct Angle3
{
	int x;
	int y;
	int z;
};

struct _OBJ_SLEEPCON
{
	Angle3 ang;
	NJS_POINT3 pos;
	NJS_POINT3 scl;
};

union _UNION_STATUS
{
	float fRangeOut;
	_OBJ_SLEEPCON* pObjSleepCondition;
};

struct _OBJ_EDITENTRY
{
	unsigned __int16 usID;
	__int16 rotx;
	__int16 roty;
	__int16 rotz;
	float xpos;
	float ypos;
	float zpos;
	float xscl;
	float yscl;
	float zscl;
};

struct OBJ_CONDITION
{
	char scCount;
	char scUserFlag;
	__int16 ssCondition;
	task* pTask;
	_OBJ_EDITENTRY* pObjEditEntry;
	_UNION_STATUS unionStatus;
};

union taskwk_counter
{
	char b[4];
	unsigned __int16 w[2];
	unsigned int l;
	float f;
	void* ptr;
};

union taskwk_timer
{
	char b[4];
	__int16 w[2];
	int l;
	float f;
	void* ptr;
};

struct CCL_INFO
{
	char kind;
	char form;
	char push;
	char damage;
	unsigned int attr;
	NJS_POINT3 center;
	float a;
	float b;
	float c;
	float d;
	int angx;
	int angy;
	int angz;
};

struct c_colli_hit_info
{
	char my_num;
	char hit_num;
	unsigned __int16 flag;
	taskwk* hit_twp;
};

struct colliwk
{
	unsigned __int16 id;
	__int16 nbHit;
	unsigned __int16 flag;
	unsigned __int16 nbInfo;
	float colli_range;
	CCL_INFO* info;
	c_colli_hit_info hit_info[16];
	NJS_POINT3 normal;
	task* mytask;
	__int16 my_num;
	__int16 hit_num;
	colliwk* hit_cwp;
};

struct eventwk_look
{
	char mode;
	Angle3 ang;
	NJS_POINT3 pos;
	task* obj;
};

union eventwk_move_param
{
	NJS_POINT3 pos;
	NJS_POINT3 spd;
	NJS_POINT3 acc;
	Angle3 ang;
};

struct eventwk_move
{
	char mode;
	eventwk_move_param param;
	float run_speed;
	float run_accel;
};

struct EVENT_ACTION_LIST
{
	char mode;
	char linkframe;
	NJS_ACTION action;
	NJS_TEXLIST* texlist;
	EVENT_ACTION_LIST* next;
	float speed;
};


struct eventwk_action
{
	float timer;
	EVENT_ACTION_LIST* list;
	EVENT_ACTION_LIST* old;
};

struct epathang3
{
	char x;
	char y;
	char z;
};

struct epathtbl
{
	epathang3 ang;
	NJS_POINT3 pos;
};

struct EPATHTAG
{
	int points;
	epathtbl* tblhead;
};

struct EVENT_PATH_LIST
{
	char mode;
	EPATHTAG* path;
	float speed;
	EVENT_PATH_LIST* next;
};

struct eventwk_path
{
	float timer;
	EVENT_PATH_LIST* list;
	NJS_POINT3 pos;
};

struct FACETBL
{
	char nbFrame;
	char faceNo;
};

struct facewk
{
	char old;
	char __new;
	__int16 nbFrame;
	__int16 frame;
	char* dataPtr;
	FACETBL* tablePtr;
};

struct eventwk
{
	char mode;
	char command;
	eventwk_look look;
	eventwk_move move;
	eventwk_action action;
	eventwk_path path;
	facewk face;
	NJS_POINT3 posofs;
	float shadowSize;
};

struct taskwk
{
	char mode;
	char smode;
	char id;
	char btimer;
	__int16 flag;
	unsigned __int16 wtimer;
	taskwk_counter counter;
	taskwk_timer timer;
	taskwk_timer value;
	Angle3 ang;
	NJS_POINT3 pos;
	NJS_POINT3 scl;
	colliwk* cwp;
	eventwk* ewp;
};

union motionwk_work
{
	char b[4];
	__int16 w[2];
	int l;
	float f;
	void* ptr;
};

struct motionwk
{
	motionwk_work work;
	NJS_POINT3 spd;
	NJS_POINT3 acc;
	Angle3 ang_aim;
	Angle3 ang_spd;
	float rad;
	float height;
	float weight;
};

struct forcewk
{
	void(__cdecl* call_back)(task*, taskwk*, forcewk*);
	Angle3 ang_spd;
	NJS_POINT3 pos_spd;
};

union anywk_work
{
	char ub[16];
	char sb[16];
	unsigned __int16 uw[8];
	__int16 sw[8];
	unsigned int ul[4];
	int sl[4];
	float f[4];
	void* ptr[4];
};

struct anywk
{
	anywk_work work;
};

struct task
{
	task* next;
	task* last;
	task* ptp;
	task* ctp;
	void(__cdecl* exec)(task*);
	void(__cdecl* disp)(task*);
	void(__cdecl* dest)(task*);
	OBJ_CONDITION* ocp;
	taskwk* twp;
	motionwk* mwp;
	forcewk* fwp;
	anywk* awp;
	void* thp;
};

struct _SEcallbuf
{
	int pri;
	int timer;
	int id;
	int mode;
	int tone;
	int angle;
	int vol;
	int volmax;
	int pitch;
	NJS_POINT3 pos;
	int qnum;
	int banknum;
};

#pragma pack(pop)

#endif /* SADXMODLOADER_SADXSTRUCTSNEW_H */