/**
 * SADX Mod Loader.
 * SADX structs based on symbols.
 */

#ifndef SADXMODLOADER_SADXSTRUCTSNEW_H
#define SADXMODLOADER_SADXSTRUCTSNEW_H

#include <stdint.h>
#include "ninja.h"
#include "d3d8types.h"
#include "SADXEnumsNew.h"

struct task;
struct taskwk;
struct _OBJ_CAMERAPARAM;
struct _OBJ_ADJUSTPARAM;

typedef void(__cdecl* TaskFuncPtr)(task*);
typedef void(__cdecl* CamFuncPtr)(_OBJ_CAMERAPARAM*);
typedef void(__cdecl* CamAdjustPtr)(taskwk*, taskwk*, _OBJ_ADJUSTPARAM*);

struct Angle3
{
	int x;
	int y;
	int z;
};

struct NJS_MKEY_SA
{
	unsigned __int16 keyframe;
	__int16 key[3];
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

struct thread
{
	thread* next;
	thread* prev;
	thread* parent;
	thread* child;
	void* sp;
	void* spbase;
	void(__cdecl* dest)();
	int control;
	int status;
};

struct EVINFO2
{
	void(__cdecl* func)(int);
	thread* tp;
	int no;
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
	TaskFuncPtr exec;
	TaskFuncPtr disp;
	TaskFuncPtr dest;
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

struct _OBJ_LANDENTRY
{
	float xCenter;
	float yCenter;
	float zCenter;
	float xWidth;
	float yWidth;
	float zWidth;
	NJS_OBJECT* pObject;
	int blockbit;
	int slAttribute;
};

struct _OBJ_MOTLANDENTRY
{
	float fFrame;
	float fStep;
	float fMaxFrame;
	NJS_OBJECT* pObject;
	NJS_ACTION* pMotion; // NJS_MOTION* in the original struct, but that's wrong
	NJS_TEXLIST* pTexList;
};

struct _OBJ_LANDTABLE
{
	__int16 ssCount;
	__int16 ssMotCount;
	__int16 ssAttribute;
	__int16 ssLoadFlag;
	float fFarClipping;
	_OBJ_LANDENTRY* pLandEntry;
	_OBJ_MOTLANDENTRY* pMotLandEntry;
	const char* pPvmFileName;
	NJS_TEXLIST* pTexList;
	const char* pBinaryFileName;
	void* pLoadBinary;
};

struct TEX_PVMTABLE
{
	const char* pname;
	NJS_TEXLIST* ptexlist;
};

struct PDS_PERIPHERALINFO
{
	unsigned int type;
	unsigned int reserved[3];
	char is_root;
	char area_code;
	char connector_dir[2];
	char product_name[32];
	char license[64];
	unsigned __int16 stdby_pow;
	unsigned __int16 max_pow;
};

struct PDS_PERIPHERAL
{
	unsigned int id;
	unsigned int support;
	unsigned int on;
	unsigned int off;
	unsigned int press;
	unsigned int release;
	unsigned __int16 r;
	unsigned __int16 l;
	__int16 x1;
	__int16 y1;
	__int16 x2;
	__int16 y2;
	const char* name;
	void* extend;
	unsigned int old;
	PDS_PERIPHERALINFO* info;
};

struct PDS_KEYBOARDINFO
{
	char lang;
	char type;
	char led;
	char led_ctrl;
};

struct PDS_KEYBOARD
{
	char ctrl;
	char led;
	char key[6];
	PDS_KEYBOARDINFO* info;
};

struct PDS_MOUSE_INFO
{
	char reserved;
	char button;
	char axis;
};

struct PDS_MOUSE_BUTTON
{
	unsigned int on;
	unsigned int off;
	unsigned int press;
	unsigned int release;
	unsigned int old;
	char connect;
	char opt;
	char reserved[2];
};

struct _m_file
{
	char* fname;
	int sfade;
	int efade;
	__int16 s16W;
	__int16 s16H;
};

union taskwork_subs
{
	char b[4];
	__int16 w[2];
	int l;
	float f;
	void* ptr;
};

struct xssunit
{
	int findflag;
	int objatt;
	int angx;
	int angz;
	float onpos;
	NJS_POINT3 normal;
};

struct xyyzzxsdwstr
{
	NJS_POINT3 pos;
	xssunit hit[6];
};

struct enemywk
{
	taskwork_subs sub;
	NJS_POINT3 velo;
	NJS_POINT3 acc;
	Angle3 ang_aim;
	Angle3 ang_spd;
	float radius;
	float height;
	float weight;
	task* lock_tp;
	TaskFuncPtr dest_org;
	char pnum;
	char old_mode;
	char old_smode;
	__int16 aim_num;
	unsigned __int16 flag;
	NJS_POINT3 home;
	NJS_POINT3 aim;
	NJS_POINT3 pre;
	NJS_POINT3 force;
	NJS_POINT3 norm;
	NJS_ARGB argb;
	NJS_POINT3 colli_center;
	float colli_top;
	float colli_radius;
	float colli_bottom;
	float cliff_height;
	float bound_side;
	float bound_floor;
	float bound_friction;
	float bound_ceiling;
	int bound_add_angle;
	float shadow_scl;
	float shadow_scl_ratio;
	xyyzzxsdwstr shadow;
	float buyoscale;
	int angx_spd;
	int angy_spd;
	int angz_spd;
	int sub_angle[8];
	int aim_angle;
	int view_angle;
	float view_range;
	float hear_range;
	float nframe;
	float pframe;
	float lframe;
	float link_ratio;
	float link_spd;
	NJS_ACTION* actp;
	NJS_ACTION* linkactp;
	float mat[16];
};

struct NpcMessageTable
{
	const __int16* code;
	const char** message;
};

struct NpcMessageTable2
{
	__int16* code;
	const char** message;
};

struct SPLINE_DATA
{
	void* src;
	int data_size;
	char* fmt;
	float rate;
	void* dst;
	int spline_type;
	int variety;
};

struct _OBJ_CAMERAPARAM
{
	unsigned __int16 xCamAng;
	unsigned __int16 yCamAng;
	float xCamPos;
	float yCamPos;
	float zCamPos;
	float xDirPos;
	float yDirPos;
	float zDirPos;
	float fDistance;
	unsigned int ulTimer;
};

struct _OBJ_ADJUSTPARAM
{
	__int16 ssAdjustFlag;
	int angSpeed[3];
	float fSpeed;
	int counter;
};

struct _OBJ_CAMERAMODE {
	CamFuncPtr fnCamera;
	char scCameraLevel;
	char scCameraDirectMode;
	int boolManual;
	CamFuncPtr fnDebug;
};

struct _OBJ_CAMERAADJUST {
	int slAttribute;
	CamAdjustPtr fnAdjust;
};

struct _OBJ_CAMERAENTRY
{
	char scMode;
	char scPriority;
	unsigned __int8 ucAdjType;
	char scColType;
	unsigned __int16 xColAng;
	unsigned __int16 yColAng;
	float xColPos;
	float yColPos;
	float zColPos;
	float xColScl;
	float yColScl;
	float zColScl;
	unsigned __int16 xCamAng;
	unsigned __int16 yCamAng;
	float xDirPos;
	float yDirPos;
	float zDirPos;
	float xCamPos;
	float yCamPos;
	float zCamPos;
	float fDistance;
};

struct _OBJ_CAMERAENTRYTABLE
{
	__int16 ssCount;
	__int16 ssAttribute;
	_OBJ_CAMERAENTRY* pObjCameraEntry;
};

struct _CameraSystemWork
{
	int G_boolSwitched;
	__int16 G_ssCameraEntry;
	char G_scCameraMode;
	char G_scCameraDirect;
	char G_scCameraLevel;
	char G_scCameraAdjust;
	char G_scCameraPriority;
	char G_scCameraAttribute;
	CamFuncPtr G_pfnCamera;
	CamAdjustPtr G_pfnAdjust;
	unsigned int G_ulTimer;
	int G_ssRestoreLevel[6];
	__int16 G_ssRestoreEntry[6];
	char G_scRestoreAttribute[6];
	CamFuncPtr G_pfnRestoreCamera[6];
	CamAdjustPtr G_pfnRestoreAdjust[6];
	_OBJ_CAMERAENTRY* G_pCameraEntry;
	char G_scRestoreCameraMode[6];
	char G_scRestoreCameraAdjust[6];
	char G_scRestoreCameraDirect[6];
	__int16 G_scRestoreCameraLevel[6];
	NJS_POINT3 G_vecCameraOffset;
};

struct _camcontwk
{
	unsigned __int8 cammode;
	unsigned __int8 camsmode;
	unsigned __int8 bBlank;
	unsigned __int8 btimer;
	unsigned __int16 wtimer;
	__int16 ssFlag;
	float tgtdist;
	float camxpos;
	float camypos;
	float camzpos;
	float tgtxpos;
	float tgtypos;
	float tgtzpos;
	int angx;
	int angy;
	int angz;
	float prevcamxpos;
	float prevcamypos;
	float prevcamzpos;
	int angx_spd;
	int angy_spd;
	int angz_spd;
	float xspd;
	float yspd;
	float zspd;
	float xacc;
	float yacc;
	float zacc;
};

// Free Camera information
struct FCWRK
{
	float dist1;
	float dist2;
	float dist0;
	float dist;
	Angle3 _ang;
	NJS_POINT3 cammovepos;
	NJS_POINT3 campos;
	int counter;
	int timer;
	Angle3 pang;
	NJS_POINT3 camspd;
	NJS_POINT3 pos;
	NJS_POINT3 _vec;
};

struct sSplineData
{
	unsigned __int16 InitPoint;
	unsigned __int16 NumPos;
	NJS_POINT3* Points;
};

struct uv_info
{
	NJS_COLOR* uv_list;
	int uv_num;
};

struct neon_info
{
	uv_info* info;
	__int16 info_num;
	__int16 type;
	__int16 uv_spd;
	__int16 wait_max;
	__int16 timer;
	__int16 uv;
};

struct HintMessageString
{
	const char* message;
	int wait;
};

struct HintMessageTable
{
	const __int16* code;
	HintMessageString* message;
};

struct PL_ACTION
{
	NJS_ACTION* actptr;
	char objnum;
	char mtnmode;
	unsigned __int16 next;
	float frame;
	float racio;
};

struct PL_JOIN_VERTEX
{
	NJS_OBJECT* objptr;
	NJS_OBJECT* srcobj;
	NJS_OBJECT* dstobj;
	char numVertex;
	char inpmode;
	char srcdepth;
	char dstdepth;
	NJS_POINT3* org;
	unsigned __int16* pnum;
};

struct bosswk
{
	taskwork_subs work;
	__int16 mtnmode;
	__int16 jvmode;
	unsigned __int16 req_action;
	unsigned __int16 action;
	unsigned __int16 lastaction;
	unsigned __int16 action_tm;
	float nframe;
	float xacc;
	float yacc;
	float zacc;
	float xspd;
	float yspd;
	float zspd;
	PL_ACTION* plactptr;
	PL_JOIN_VERTEX* pljvptr;
	NJS_ACTION* actwkptr;
};

struct MPOSANG
{
	int at_angle[2];
	NJS_POINT3 gpos;
};

struct MOBJ
{
	__int16 level;
	NJS_OBJECT* obj;
	NJS_POINT3* pm;
	MPOSANG* pos_ang;
	NJS_POINT3* points_ptr_org;
};

struct MORPHWK
{
	char mode;
	__int16 objNum;
	float t;
	MOBJ* objlist;
	float puru_a0;
	float puru_dec0;
	Rotation3 puru_ang0;
	Rotation3 puru_angadd0;
};

struct chaoswk
{
	bosswk bwk;
	int generaltimer;
	char txchg_tmr;
	char txchg_num;
	char form;
	char sp_mode;
	char colli_mode;
	char headsts;
	char camera_mode;
	__int16 etcflag;
	__int16 dispflag;
	__int16 morph_time;
	NJS_POINT3 sp_scale;
	NJS_POINT3 tgtpos_s;
	NJS_POINT3 tgtpos;
	float HitPoint;
	float attack_dist;
	int attack_zang;
	int attack_yang;
	NJS_ARGB argb_ofs[2];
	float morph_param;
	__int16 attack_tmr;
	char barrier_ptno;
	char barrier_tmr;
	float barrier_a;
	NJS_TEXLIST* texlist;
	float ground_y;
	void* morph_wp;
	float param;
	unsigned int old_texaddr;
};

struct CHAOS_PARAM
{
	char hitpoint;
	float height;
	float leg_len;
	float walk_spd;
	int turn_spd;
	NJS_POINT3 c_initpos;
	float alpha_obj;
	float morph_xsp;
	float morph_ysp;
	float limit_r;
	float limit_r_sprate;
	float arm_strtch_spd;
	__int16 atc_tame_time;
	__int16 model_alpha;
	float puncheff_scl;
	float arm_len;
	NJS_POINT3 field_center_pos;
	float sphere_scale;
	__int16 attack_time_base;
	__int16 attack_time_ofs;
	float chaoseme_radius;
	float chaoseme_yofs;
};

struct CHAOS_OBJPVTBL
{
	char objno;
	char vecno;
	NJS_POINT3 ofs_pos;
};

struct CHAOS_OBJPV
{
	NJS_POINT3 pos;
	NJS_POINT3 oldpos;
	NJS_POINT3 vec;
};

struct Chaos0Work
{
	NJS_POINT3 dest_pos;
	NJS_POINT3 under_pos;
	NJS_POINT3 velocity;
	float phai;
	float dphai;
	float jumph;
	int frame;
	float dmot;
	int jump_mode;
	float mot_spd;
	int cng_int;
	float human_y;
	float human_dy;
	NJS_POINT3 manju_base_scale;
	NJS_POINT3 manju_dscale;
	NJS_POINT3 manju_scale;
	float manju_alpha;
	float manju_dalpha;
	int manju_anim_cnt;
};

struct Chaos1Work
{
	int cng_int;
	float mot_spd;
	float human_y;
	float human_dy;
	NJS_POINT3 manju_base_scale;
	NJS_POINT3 manju_dscale;
	NJS_POINT3 manju_scale;
	float manju_alpha;
	float manju_dalpha;
};

struct Chaos2Work
{
	NJS_POINT3 dest_pos;
	NJS_POINT3 velocity;
	int frame;
	int cng_int;
	float mot_spd;
	float mot_cnt;
	float human_y;
	float human_dy;
	NJS_POINT3 manju_base_scale;
	NJS_POINT3 manju_dscale;
	NJS_POINT3 manju_scale;
	float manju_alpha;
	float manju_dalpha;
	int manju_anim_cnt;
};

struct Chaos4Work
{
	NJS_POINT3 dest_pos;
	NJS_POINT3 velocity;
	int frame;
	int cng_int;
	float mot_spd;
	float human_y;
	float human_dy;
	NJS_POINT3 manju_base_scale;
	NJS_POINT3 manju_dscale;
	NJS_POINT3 manju_scale;
	float manju_alpha;
	float manju_dalpha;
	int manju_anim_cnt;
};

struct C4PunchParam
{
	Angle3 joint_angle[5];
	Angle3 joint_speed[5];
	float joint_scale;
	float joint_spring_k;
	float joint_spring_f;
	__int16 joint_timer;
	__int16 joint_now;
	Angle3 tmp;
};

struct Chaos4work
{
	Angle3 angle_speed;
	Angle3 angle_target;
	NJS_POINT3 speed;
	__int16 timer;
	unsigned __int8 movemode;
	unsigned __int8 headmode;
	Angle3 tmp_ang;
	C4PunchParam punchparam;
	unsigned __int8 dispmode;
	char action_cnt;
	NJS_POINT3 eme_pos[4];
	char up_cnt;
};

struct pathinfo
{
	int slangx;
	int slangz;
	int slangax;
	int slangaz;
	float onpathpos;
	NJS_POINT3 pos;
	float xpos;
	float ypos;
	float zpos;
	NJS_POINT3 normal;
	NJS_POINT3 normala;
	NJS_POINT3 front;
};

struct motionwk2
{
	taskwork_subs work;
	NJS_POINT3 spd;
	NJS_POINT3 acc;
	Angle3 ang_aim;
	Angle3 ang_spd;
	float force;
	float accel;
	float frict;
};

struct XYZ_SHADOW_WORK
{
	NJS_POINT3 pos;
	xssunit hit[6];
	xssunit pre_hit[6];
};

struct MOVE_WORK
{
	taskwork_subs work;
	NJS_POINT3 Velo;
	NJS_POINT3 Acc;
	Angle3 AimAng;
	Angle3 RotSpd;
	float rad;
	float height;
	float weight;
	unsigned __int16 Flag;
	unsigned __int16 Timer;
	float Spd;
	float Gravity;
	int ViewAngle;
	float ViewRange;
	NJS_POINT3 AimPos;
	NJS_POINT3 PrePos;
	NJS_POINT3 HomePos;
	Angle3 HomeAng;
	Angle3 Phase;
	NJS_LINE FrontWall;
	NJS_POINT3 Offset;
	float Top;
	float Side;
	float Bottom;
	float CliffHeight;
	float BoundSide;
	float BoundFloor;
	float BoundCeiling;
	float BoundFriction;
	float TopY;
	float BottomY;
	float WaterY;
	XYZ_SHADOW_WORK Shadow;
};

struct al_entry_work
{
	unsigned __int16 category;
	unsigned __int16 num;
	unsigned __int16 kind;
	unsigned __int16 flag;
	void* pSaveInfo;
	int CommuID;
	NJS_POINT3 pos;
	Angle3 ang;
	float radius;
	float offset;
	float CamDist;
	__int16 command;
	__int16 command_value;
	__int16 state;
	task* tp;
	al_entry_work* pCommu;
	al_entry_work* pLockOn;
};

#pragma pack(push, 2)
struct AL_BODY_INFO
{
	float HPos;
	float VPos;
	float APos;
	float AimHPos;
	float AimVPos;
	float AimAPos;
	float growth;
	float EyePos;
	float EyeScl;
	float EyeRatio;
	char EyeColor;
	char DefaultEyeNum;
	char DefaultMouthNum;
	char HonbuNum;
	char HonbuColorNum;
	char ObakeHead;
	char ObakeBody;
	char MedalNum;
	char ColorNum;
	char NonTex;
	char JewelNum;
	char MultiNum;
	char EggColor;
	char FormNum;
	char FormSubNum;
};
#pragma pack(pop)

struct AL_TIME
{
	char minute;
	char second;
	char frame;
};

struct cnkobj
{
	unsigned int evalflags;
	NJS_CNK_MODEL* model;
	float pos[3];
	int ang[3];
	float scl[3];
	cnkobj* child;
	cnkobj* sibling;
};

struct AL_BUYO_PARAM
{
	float Spring1;
	float Spring2;
	float Friction1;
	float Friction2;
	float MaxDiff;
	float MaxSpd;
	float Weight1;
	float Weight2;
	float Weight3;
	NJS_POINT3 Center;
};

struct AL_RACE_PERSONAL_INFO
{
	AL_TIME PersonalRecord[10];
	char nbWin[10];
	unsigned __int16 MedalFlag;
};

struct AL_RACE_PERSONAL_INFO_DX
{
	AL_TIME PersonalRecord[5];
};

struct TMP_PARAM
{
	char Exp[8];
	char Abl[8];
	char Lev[8];
	unsigned __int16 Skill[8];
};

struct CNK_VN_VERTEX
{
	NJS_POINT3 Pos;
	NJS_POINT3 Normal;
};

struct AL_VERTEX_INFO
{
	NJS_POINT3 Velo;
	float Weight;
	float Spring;
	float Friction;
};

struct al_model
{
	int* VList;
	__int16* PList;
	NJS_POINT3 Center;
	float Radius;
	__int16 OrgTexId[4];
	int PListSize;
	int nbVertex;
	CNK_VN_VERTEX* pVertex;
	NJS_POINT3* pOrgVertexPos;
	NJS_POINT3* pOrgVertexNormal;
	AL_VERTEX_INFO* pVertexInfo;
};

struct al_object
{
	unsigned int EvalFlags;
	al_model* pModel;
	float Pos[3];
	int Ang[3];
	float Scl[3];
	al_object* pChild;
	al_object* pSibling;
	al_object* pParent;
	NJS_POINT3 diff;
	NJS_POINT3 GlobalAimPos;
	NJS_POINT3 OrgPos;
	NJS_POINT3 Velo;
	NJS_POINT3 OrgAng;
	NJS_POINT3 AngSpd;
	float Weight;
	float Spring;
	float Friction;
	int NoBuyoFlag;
	int CalcBuyoPosFlag;
	cnkobj* pPartsObject;
	cnkobj* pItemObject;
	NJS_TEXLIST* pItemTexlist;
	float ItemScale;
	int ItemActiveFlag;
	int ItemOffsetFlag;
	NJS_POINT3 ItemOffsetPos;
	Angle3 ItemOffsetAng;
	float(*pOldMatrix)[16];
	AL_BUYO_PARAM* pBuyoParam;
	void* DisplayList;
};

struct AL_FACE_CTRL
{
	int EyeTimer;
	__int16 EyeColorNum;
	__int16 EyeCurrNum;
	__int16 EyeDefaultNum;
	int MouthTimer;
	__int16 MouthCurrNum;
	__int16 MouthDefaultNum;
	float EyePosX;
	float EyePosY;
	float EyeSclX;
	float EyeSclY;
	unsigned int Flag;
	al_object* pEyeObject[2];
	al_object* pMouthObject;
	int EyeLidBlinkMode;
	int EyeLidBlinkTimer;
	int EyeLidBlinkAng;
	int EyeLidExpressionMode;
	int EyeLidExpressionTimer;
	int EyeLidExpressionDefaultCloseAng;
	int EyeLidExpressionCurrCloseAng;
	int EyeLidExpressionAimCloseAng;
	int EyeLidExpressionDefaultSlopeAng;
	int EyeLidExpressionCurrSlopeAng;
	int EyeLidExpressionAimSlopeAng;
};

struct AL_KNOWLEDGE_OTHER
{
	char like;
};

struct AL_EMOTION
{
	unsigned __int16 Flag;
	unsigned __int16 MoodTimer;
	unsigned __int16 IllTimer;
	unsigned __int16 Timer;
	char Mood[8];
	unsigned __int16 State[11];
	char Personality[13];
	char Taste;
	char Tv;
	char Music;
	char IllState[6];
};

struct al_perception_link
{
	__int16 info[4];
	float dist;
	int InSightFlag;
	int HearFlag;
	int SmellFlag;
	al_entry_work* pEntry;
};

struct AL_PERCEPTION_INFO
{
	unsigned __int16 nbPerception;
	int InSightFlag;
	int HeardFlag;
	int SmellFlag;
	float NearestDist;
	__int16 NearestNum;
	al_perception_link list[32];
};

struct AL_PERCEPTION
{
	float SightRange;
	int SightAngle;
	int SightAngleHalf;
	float HearRange;
	float SmellRange;
	AL_PERCEPTION_INFO Player;
	AL_PERCEPTION_INFO Chao;
	AL_PERCEPTION_INFO Fruit;
	AL_PERCEPTION_INFO Tree;
	AL_PERCEPTION_INFO Toy;
	AL_PERCEPTION_INFO Sound;
};

struct NJS_RGB
{
	float r;
	float g;
	float b;
};

struct LE_LIGHT_ENV
{
	char stgNo;
	char actNo;
	char ligNo;
	char flgs;
	NJS_POINT3 vec;
	float spe;
	float dif;
	float r;
	float g;
	float b;
	float ambR;
	float ambG;
	float ambB;
};

struct LE_PALIGHT_ENV
{
	char stgNo;
	char actNo;
	char chrNo;
	char flgs;
	NJS_POINT3 vec;
	float dif;
	NJS_RGB amb;
	float cpow;
	NJS_RGB co;
	float spow;
	NJS_RGB spe;
	float cpow2;
	NJS_RGB co2;
	float spow2;
	NJS_RGB spe2;
};

struct player_parameter
{
	int jump2_timer;
	float pos_error;
	float lim_h_spd;
	float lim_v_spd;
	float max_x_spd;
	float max_psh_spd;
	float jmp_y_spd;
	float nocon_speed;
	float slide_speed;
	float jog_speed;
	float run_speed;
	float rush_speed;
	float crash_speed;
	float dash_speed;
	float jmp_addit;
	float run_accel;
	float air_accel;
	float slow_down;
	float run_break;
	float air_break;
	float air_resist_air;
	float air_resist;
	float air_resist_y;
	float air_resist_z;
	float grd_frict;
	float grd_frict_z;
	float lim_frict;
	float rat_bound;
	float rad;
	float height;
	float weight;
	float eyes_height;
	float center_height;
};

union playerwk_work
{
	char b[4];
	__int16 w[2];
	int l;
	float f;
	void* ptr;
};

union playerwk_free
{
	char sb[32];
	char ub[32];
	__int16 sw[16];
	unsigned __int16 uw[16];
	int sl[8];
	unsigned int ul[8];
	float f[8];
	void* ptr[8];
};

struct csts
{
	float radius;
	NJS_POINT3 pos;
	NJS_POINT3 spd;
	NJS_POINT3 tnorm;
	unsigned __int16 find_count;
	unsigned __int16 selected_nmb;
	float yt;
	float yb;
	int angx;
	int angz;
	NJS_POINT3 normal;
	NJS_POINT3 normal2;
	NJS_POINT3 onpoly;
	NJS_POINT3 pshbk;
	NJS_POINT3 anaspdh;
	NJS_POINT3 anaspdv;
};

struct PL_FACE
{
	char lastface;
	char face;
	char reqface;
	char tblpoint;
	float frame;
	float framespeed;
	FACETBL* tbl;
	NJS_OBJECT* sibling;
	NJS_OBJECT* faceorg;
	NJS_OBJECT* facebuf;
	NJS_MOTION* facetypes;
};

struct PL_LANDPOSI
{
	float x;
	float y;
	float z;
	float r;
	float d;
	float h;
	int angy_dif;
	int angy_aim;
};

struct mtnjvwk
{
	__int16 mtnmode;
	__int16 jvmode;
	unsigned __int16 reqaction;
	unsigned __int16 action;
	unsigned __int16 lastaction;
	unsigned __int16 nextaction;
	unsigned __int16 acttimer;
	__int16 flag;
	float nframe;
	float start_frame;
	float* spdp;
	float* workp;
	PL_ACTION* plactptr;
	PL_JOIN_VERTEX* pljvptr;
	NJS_ACTION* actwkptr;
};

struct shadowwk
{
	int angx;
	int angz;
	unsigned int Attr_top;
	unsigned int Attr_bottom;
	float y_top;
	float y_bottom;
	unsigned int _Attr_top;
	unsigned int _Attr_bottom;
	float scl;
};

struct playerwk
{
	playerwk_work work;
	__int16 equipment;
	__int16 item;
	__int16 jumptimer;
	__int16 nocontimer;
	__int16 breathtimer;
	__int16 waittimer;
	__int16 flag;
	__int16 island_num;
	__int16 path_point;
	float hpos;
	float dotp;
	NJS_POINT3 eff;
	NJS_POINT3 acc;
	NJS_POINT3 spd;
	NJS_POINT3 wall_normal;
	NJS_POINT3 floor_normal;
	unsigned int attr;
	unsigned int last_attr;
	csts* cstsp;
	task* htp;
	task* ttp;
	PL_FACE* pfp;
	NJS_OBJECT* lclop;
	PL_LANDPOSI* island_list;
	playerwk_free free;
	player_parameter p;
	mtnjvwk mj;
	NJS_POINT3 righthand_pos;
	NJS_POINT3 lefthand_pos;
	NJS_POINT3 rightfoot_pos;
	NJS_POINT3 leftfoot_pos;
	NJS_POINT3 user0_pos;
	NJS_POINT3 user1_pos;
	NJS_POINT3 righthand_vec;
	NJS_POINT3 lefthand_vec;
	NJS_POINT3 rightfoot_vec;
	NJS_POINT3 leftfoot_vec;
	NJS_POINT3 user0_vec;
	NJS_POINT3 user1_vec;
	shadowwk shadow;
};

struct CHS_BILL_INFO
{
	int adjust; // if true use 0-1 UV, otherwise use 0-255 UV
	float wd;
	float ht;
	float s0;
	float t0;
	float s1;
	float t1;
	NJS_TEXLIST* pTexlist;
	int TexNum;
};

struct PADREC_DATA
{
	BYTE on;
	BYTE angle;
	BYTE stroke;
};

struct PADREC_DATA_TAG
{
	PADREC_DATA** data;
	int size;
};

struct cpathtbl
{
	__int16 roty;
	__int16 rotx;
	__int16 rotz;
	float camx;
	float camy;
	float camz;
};

struct cpathtag
{
	unsigned __int8 type;
	float points;
	cpathtbl* tblhead;
};

struct pathctrl
{
	float length;
	NJS_POINT3 pos;
};

struct pathtbl
{
	__int16 slangx;
	__int16 slangz;
	float length;
	float xpos;
	float ypos;
	float zpos;
};

struct pathtag
{
	unsigned __int8 pathtype;
	unsigned __int16 points;
	float totallen;
	pathtbl* tblhead;
	void* pathtask;
};

struct pathgrp
{
	unsigned int wanmb;
	pathtag** tags;
};

struct pathwk
{
	float mPosX;
	float mPosZ;
	float mWidthX;
	float mWidthZ;
	float mRadius;
	int mAngle;
};

struct SEQ_SECTION
{
	void(__cdecl* init)();
	void(__cdecl* main)(int);
};

struct SEQ_SECTIONTBL
{
	SEQ_SECTION* psec;
	unsigned __int8 stg;
	unsigned __int8 act;
	unsigned __int16 entrance;
	char name[32];
};

struct SEQUENCE
{
	char time;
	char emerald;
	__int16 seqno;
	__int16 sec;
	__int16 nextsec;
	unsigned __int16 stage;
	unsigned __int16 destination;
};

struct _OBJ_LANDCOLL
{
	int slAttribute;
	NJS_OBJECT* pObject;
	task* pTask;
};

struct PL_KILLCOLLI
{
	int character;
	NJS_OBJECT* object;
};

struct _OBJ_ITEMENTRY
{
	unsigned __int8 ucInitMode;
	unsigned __int8 ucLevel;
	__int16 ssAttribute;
	float fRange;
	void* ptr;
	TaskFuncPtr fnExec;
	const char* strObjName;
};

struct _OBJ_ITEMTABLE
{
	__int16 ssCount;
	__int16 ssAttribute;
	_OBJ_ITEMENTRY* pObjItemEntry;
};

struct ___stcClip
{
	float f32Near;
	float f32Far;
};

struct ___stcFog
{
	float f32StartZ;
	float f32EndZ;
	unsigned int Col;
	unsigned __int8 u8Enable;
};

struct particle_info
{
	float scl;
	float sclspd;
	float animspd;
	float friction;
	float yacc;
	NJS_POINT3 pos;
	NJS_POINT3 velo;
	NJS_ARGB argb;
};

struct stcWaterSurface
{
	float f32x0;
	float f32y0;
	float f32z0;
	char s8AnimNo0;
	char s8Frame0;
	char u8WrapX;
	char u8WrapZ;
	float f32WrapXZ;
	float f32TransScale;
	char u8StripArrayNo;
};

struct stcAnim
{
	char s8TexNo;
	char s8Frame;
};

// Collision information set by GetZxShadowOnFDPolygon
struct zxsdwstr
{
	NJS_POINT3 pos;
	xssunit lower;
	xssunit upper;
};

// Draw queue node
struct LATE_LNK
{
	LATE_LNK* lnk;
	LATE_LNK* last;
};

struct LATE_RQ
{
	LATE_RQ* lnk;
	float zkey;
	unsigned __int8 typ;
	unsigned __int8 alpMd;
	unsigned __int16 no;
	NJS_TEXLIST* texLst;
	NJS_ARGB argb;
	unsigned int ctrl3dFlg;
	unsigned int atrAnd;
	unsigned int atrOr;
	unsigned int etc;
};

struct LATE_DRAWTEX_RQ
{
	LATE_RQ cmn;
	NJS_MATRIX mtx;
	NJS_TEXTURE_VTX* vtx;
	int num;
	int ind;
	int tf;
};

struct LATE_ACT_RQ
{
	LATE_RQ cmn;
	NJS_MATRIX mtx;
	NJS_ACTION* act;
	float frm;
};

struct LATE_ACTLNK_RQ
{
	LATE_RQ cmn;
	NJS_MATRIX mtx;
	float frm;
	NJS_ACTION_LINK actlnk;
	NJS_MOTION_LINK motlnk;
};

struct LATE_PO2D_RQ
{
	LATE_RQ cmn;
	NJS_POINT2COL p2c;
	NJS_MATRIX mtx;
	unsigned int atr;
	int texNum;
	float pri;
};

struct LATE_S3D_RQ
{
	LATE_RQ cmn;
	NJS_SPRITE spr;
	unsigned int atr;
	unsigned int dmy;
	NJS_MATRIX mtx;
};

struct LATE_PO3D_RQ
{
	LATE_RQ cmn;
	NJS_POINT3COL p3c;
	NJS_MATRIX mtx;
	unsigned int atr;
	int texNum;
};

struct LATE_SHPMOT_RQ
{
	LATE_RQ cmn;
	NJS_MATRIX mtx;
	float frm;
	NJS_OBJECT* obj;
	NJS_MOTION* mot;
	NJS_MOTION* shp;
	int flgs;
	float clpScl;
	void* drwMdl;
};

struct LATE_OBJ_RQ
{
	LATE_RQ cmn;
	NJS_MATRIX mtx;
};

struct LATE_BOX2D_RQ
{
	LATE_RQ cmn;
	float x;
	float y;
	float x2;
	float y2;
	float pri;
	unsigned int argb;
};

struct LATE_S2D_RQ
{
	LATE_RQ cmn;
	NJS_SPRITE spr;
	unsigned int atr;
	float pri;
	NJS_MATRIX mtx;
};

struct LATE_FUNC_RQ
{
	LATE_RQ cmn;
	NJS_MATRIX mtx;
	void(__cdecl* fnc)(void*);
	void* arg;
};

union LATE_RQ_T
{
	LATE_S3D_RQ s3d;
	LATE_S2D_RQ s2d;
	LATE_OBJ_RQ obj;
	LATE_ACT_RQ act;
	LATE_PO2D_RQ po2d;
	LATE_PO3D_RQ po3d;
	LATE_SHPMOT_RQ shpmot;
	LATE_BOX2D_RQ box2d;
	LATE_DRAWTEX_RQ tex;
	LATE_FUNC_RQ func;
	LATE_RQ rq;
};

struct ___stcFogEmu
{
	unsigned __int8 u8Emulation;
	unsigned __int8 u8FogSplitCnt;
};

struct OBJECT_SAVEPOINT_DATA
{
	int flag;
	Angle3 ang;
	Angle3 ang_spd;
	task* tp[2];
	float materiral[3];
	int write_timer;
	__int16 kiseki_timer;
};

// Itembox item information
struct ITEM_INFOMATION
{
	unsigned int texture_id;
	void(__cdecl* effect_func)(taskwk*);
};

// Cart data
struct ENEMY_CART_DATA
{
	int flag;
	int ring_timer;
	float player_colli_r;
	int fan_angle;
	__int16 cart_color;
	__int16 invincible_timer;
	__int16 motion_timer;
	__int16 ignor_collision;
	__int16 drift_mode;
	__int16 drift_timer;
	int drift_angle;
	char vitality;
	Angle3 unstable;
	int loop_pos;
	float restless;
	float restless_timer;
	int smoke_timer;
	int drift_effect_timer;
	float shadow_pos;
	NJS_POINT3 vector;
	Angle3 shadow_ang;
	float max_spd;
	NJS_POINT3 add_key;
	NJS_POINT3 add_nature;
	float bonus_add;
	int rest_timer;
	unsigned __int8 next_point;
	unsigned __int8 load_line;
	unsigned __int8 load_indp[2];
	NJS_POINT3 last_pos;
	char cart_type;
	char start_wait;
	__int16 last_player_flag;
	char explose_flag[10];
	NJS_POINT3 explose_point[10];
	NJS_POINT3 explose_spd[10];
	Angle3 explose_angle[10];
	Angle3 explose_rotate[10];
	float tmp_posi[3];
	int tmp_angle[3];
	unsigned int hamari_cnt;
};

struct CART_PLAYER_PARAMETER
{
	float max_spd;
	float min_spd;
	float max_back_spd;
	float slide_spd;
	float gravitation;
	float gravitation_max;
	float hover_point;
	float grip;
	float spd_accell;
	float spd_break;
	float spd_masatu;
	int max_vitality;
	int smoke_vitality;
	float cart_handle;
	float drift_rate;
};

struct CART_OTHER_PARAM
{
	float chase_distance;
	int rest_time;
	float cart_enemy_accell;
	float cart_enemy_break;
	float cart_enemy_search_addspd;
	float explose_spd;
	int explose_ang;
	int dead_wait_time;
	int ring_sub_timer;
};

struct CART_LOAD_DATA
{
	NJS_POINT3* load_data;
	int point_num;
};

struct CartDispTime
{
	char sign;
	char min;
	char sec;
	char sec100;
};

// Information for MirenInitTask
struct TaskInfo
{
	unsigned int size;
	unsigned int initMode;
	void(__cdecl* init_p)(task*, void*);
	TaskFuncPtr exec_p;
	TaskFuncPtr disp_p;
	TaskFuncPtr dest_p;
};

struct PLAYER_CONTINUE_DATA
{
	unsigned __int16 stage_number;
	unsigned __int8 minutes;
	unsigned __int8 second;
	unsigned __int8 frame;
	int continue_flag;
	NJS_POINT3 pos;
	Angle3 ang;
};

struct ADVPOS
{
	NJS_POINT3 pos;
	int angy;
};

// StartPosition
struct GM_START_POSANG
{
	__int16 stage;
	__int16 act;
	NJS_POINT3 p;
	int angy;
};

// PolarCoord 
struct _INPUT
{
	int angle;
	float stroke;
};

// _INPUT also exists as SONIC_INPUT
typedef _INPUT SONIC_INPUT;

struct WIREMODEL
{
	NJS_POINT3* pos;
	NJS_COLOR* color;
	__int16* index;
	__int16 nbLine;
	NJS_POINT3COL p3col;
};

// Additional E102 worker
struct E102WK
{
	char laser_mode;
	char gun_mode;
	char lockon_noshot_num;
	char ctrl_mode;
	char lock_num;
	char beam_num;
	char actgundir;
	__int16 generaltimer;
	__int16 lock_combo_num;
	__int16 shot_combo_num;
	__int16 add_sec_total;
	__int16 laser_tmr;
	__int16 laser_lock_tmr;
	__int16 dead_eye_tmr;
	__int16 se_flag;
	float fan_yofs;
	int fan_ydif_ang;
	int fan_ang[3];
	int wrun_mov_yang;
	task* lock_now_cs;
	NJS_POINT3 laser_pos;
	NJS_POINT3 laser_vec;
	float laser_dist;
	float laser_dist2;
	NJS_POINT3 gunarm_pos;
	NJS_POINT3 gunarm_vec;
	task* searchtp;
	float arm_frame;
	int arm_ang_y;
	int arm_ang_z;
	NJS_ACTION* actwkptr;
	NJS_ACTION* actgunptr;
	float scope_frame;
	int headfl_tmr;
	int head_ang;
	float shot_shock;
	NJS_POINT3 koshi_pos[2];
	float yspdbak;
	WIREMODEL cursor1;
	WIREMODEL cursor2;
	task* shot_now_csp;
	task* lock_tp[32];
	task* beam_tp[32];
	_INPUT input_buf[8];
	float ground_ypos;
	float ground_ydist;
	unsigned int ground_attr;
	float walk_dist;
};

// E-Series boss worker
struct TGT_WK
{
	bosswk bwk;
	float fHitPoint;
	int timerGeneral;
	int timerShot;
	int flag;
	int mode_req;
	int mode_old;
	int spotTarget;
	int angHead;
	int angSpdX;
	float fGroundY;
	float fGroundDist;
	float fTargetDist2;
	NJS_POINT3 oldPos;
	NJS_POINT3 spd;
	NJS_POINT3 posBody;
	NJS_POINT3 posShoulder[2];
	NJS_POINT3 posMuzzle[2];
	NJS_POINT3 vecMissile[2];
	int angFan;
	float fFanOfsY;
	int ColliEntryFlag;
};

// E-Series bosses missile worker
struct MISSILE_WK
{
	NJS_POINT3 spd;
	int nLifeTime;
	int type;
	int flag;
	int flagBrink;
	float fShadowY;
	colliwk* pIgnoreColliWk;
};

struct ADVERTISE_WORK
{
	char flags[8];
	char SelectedVmsNo;
	char SelectedFileNo;
	char MainMenuSelectedMode;
	char CharSelCsr;
	char TitleCnt;
	char PlayerChar;
	char Stage;
	char Act;
	unsigned __int16 GamePlayerNumber;
};

struct PanelPrmType
{
	float OfsX;
	float OfsY;
	unsigned __int8 PvrIdx;
};

struct DDlgType
{
	float CntrX;
	float CntrY;
	float BaseZ;
	float SclX;
	float SclY;
	char Csr;
};

struct AvaStgActT
{
	unsigned __int8 Stg;
	unsigned __int8 Act;
};

struct AvaStgActPrmT
{
	AvaStgActT* StgActPtr;
	unsigned __int8 Num;
};

struct ModeSelPrmType
{
	AdvaModeEnum PrevMode;
	AdvaModeEnum NextMode;
	AdvaModeEnum NextModeSub;
};

struct FileSelWk
{
	AdvaStatEnum Stat;
	AdvaModeEnum PrevMode;
	AdvaModeEnum NextMode;
	float BaseZ;
	float T;
	unsigned int BaseCol;
	float CsrScale;
	unsigned int CsrCol;
	int CsrAniAng;
	FileSbMdEnum SubMode;
	int GB_plBak;
	AvaCharEnum SelCh;
	__int16 WaitTime;
	unsigned __int8 CsrAlpha;
	float OfsX;
};

#pragma pack(push, 2)
struct WakuPrmT
{
	float OfsX;
	float OfsY;
	float OfsZ;
	unsigned int ColU;
	unsigned int ColD;
	unsigned __int8 KadoPvrIdx;
	unsigned __int8 YokoPvrIdx;
	unsigned __int8 TatePvrIdx;
};
#pragma pack(pop)

struct AvaTexLdPrmT
{
	char* FileName;
	NJS_TEXLIST* TexListPtr;
};

struct GH_FONT_INFO
{
	float char_width;
	float char_height;
	float tex_width;
	float tex_height;
	NJS_TEXLIST* texlist;
	unsigned __int8* prop_tbl;
	unsigned int color[4];
};

struct _GC_MEMOCA_STATE
{
	unsigned __int8 u8FileName;
	char* string;
	unsigned __int8 u8FileNum;
	ERR_TYPE ErrFlag[1];
	unsigned __int8 MsgFlag;
};

struct TrialActSelWk
{
	AdvaStatEnum Stat;
	AdvaModeEnum PrevMode;
	AdvaModeEnum NextMode;
	float BaseZ;
	float T;
	unsigned int BaseCol;
	TrialActSbMdEnum SubMode;
	AvaStgActT NowStgAct[10];
	AvaStgActPrmT NowStgActPrm;
	AvaStgEnum Stg[33];
	unsigned __int8 StgEnaMax;
	unsigned int CsrCol;
	float CsrScale;
	int CsrAniAng;
	unsigned __int8 CsrAlpha;
	unsigned __int8 DispStg;
	unsigned __int8 SelStg;
};

struct CharMdlWk
{
	task* PlTskPtrs[7];
	task* CamTskPtr;
	float LocX;
	float LocY;
	float Scale;
	char MdlDspCnt;
	char MotCnts[7];
	unsigned __int8 SelFlg;
};

struct CharSelWk
{
	AdvaStatEnum Stat;
	AdvaModeEnum PrevMode;
	AdvaModeEnum NextMode;
	float BaseZ;
	float T;
	unsigned int BaseCol;
	int CsrAniAng;
	ChSbMdEnum SubMode;
	AvaCharEnum SelCh;
	unsigned __int8 SelFile;
	float AngT;
	__int16 Cnt;
	AdvDlgSelEnum DlgStat;
	unsigned __int8 WakeFlg;
};

struct TitleMenuWk
{
	AdvaStatEnum Stat;
	AdvaModeEnum PrevMode;
	AdvaModeEnum NextMode;
	AdvaModeEnum NextModeSub;
	float BaseZ;
	float T;
	unsigned int BaseCol;
	TitleMenuSbMdEnum SubMode;
	TitleMenuEnum SelMenu;
	__int16 Cnt;
	unsigned __int8 WakeFlg;
};

struct DlgSndPrmType
{
	int OpenSnd;
	int CloseSnd;
	int MoveSnd;
	int DecideSnd;
	int CancelSnd;
};

struct DialogPrmType
{
	DlgPnlStyleEnum DlgStyle;
	void(__cdecl* EachDrawFnc)(DDlgType*);
	NJS_TEXLIST* PnlTlsPtr;
	PanelPrmType* PnlPrmPtr;
	DlgSndPrmType* DlgSndPrmPtr;
	unsigned int DlgColU;
	unsigned int DlgColD;
	float CntrX;
	float CntrY;
	float BaseZ;
	float SzX;
	float SzY;
	float MagX;
	float MagY;
	char CsrMax;
	char CsrCancel;
};

struct MSGC
{
	unsigned __int8 kind;
	__int16 x;
	__int16 y;
	unsigned __int16 width;
	unsigned __int16 height;
	unsigned __int16 buf_width;
	unsigned __int16 buf_height;
	unsigned __int16 buf_width2;
	unsigned __int16 buf_height2;
	__int16 lx;
	__int16 ly;
	__int16 sx;
	__int16 sy;
	unsigned __int16 color;
	NJS_COLOR fc;
	NJS_COLOR bc;
	float scale;
	void* bitmap;
	unsigned int globalindex;
	NJS_TEXLIST texlist;
	NJS_TEXNAME texname;
	int msgc_flag;
};

struct _SC_NUMBERS
{
	unsigned __int8 type;
	unsigned __int8 attr;
	__int16 rot;
	unsigned int max;
	unsigned int value;
	NJS_POINT3 pos;
	float scl;
	unsigned int color;
};

struct KnFragmNmbStr
{
	char nmb[4];
	unsigned __int16 stgnmb;
};

struct KnFragmSetStr
{
	int id;
	int boutflag;
	int psflag;
	int cpflag;
	NJS_POINT3 pos;
	NJS_POINT3 contpos;
};

struct sMRacePath
{
	int flag;
	NJS_POINT3 pos;
};

struct sSonicCtrl
{
	NJS_POINT3 now_path_pos;
	NJS_POINT3 tgt_path_pos;
	NJS_POINT3 vec_snc_tgt;
	float dist_snc_tgt;
	int path_flag;
	int path_flag_bak;
	int last_ang;
	float pl_last_spd;
	int jump_cnt;
};

struct CUSTOM_OBJ
{
	obj* obj;
	void(__cdecl* exad)(NJS_OBJECT*);
};

// Chaos bubble effect

struct BUBBLE
{
	char mode;
	char flag;
	char id;
	char timer;
	char posnum;
	float size;
	float scale_x;
	float swing;
	int spiral;
	int spiral_add;
	NJS_POINT3* spline;
	float t;
	float tadd;
	float tmax;
};

struct BUBBLE_LIST
{
	__int16 id;
	char num;
	NJS_POINT3* pos;
};

struct BUBBLE_TABLE
{
	unsigned __int16 objnum;
	NJS_POINT3 pos;
	int ang[3];
};

struct BUBBLE_DATA
{
	char object_num;
	char spline_num;
	BUBBLE_LIST* list;
	BUBBLE_TABLE* tbl;
	float scl;
};

struct bubble_data
{
	float radius_min;
	float radius_max;
	int yuragi_angspd;
	float yuragi_min;
	float yuragi_max;
	float y_spd_min;
	float y_spd_max;
};

// Title screen worker
struct TitleNewWk
{
	AdvaStatEnum Stat;
	AdvaModeEnum PrevMode;
	AdvaModeEnum NextMode;
	float BaseZ;
	float T;
	unsigned int BaseCol;
	TitleNewSbMdEnum SubMode;
	ObjectMaster* AlertTsk;
	char VMStatAll;
	int titletimer;
	unsigned int titleblinktimer;
	ObjectMaster* CamTskPtr;
	unsigned int wavetimer;
	unsigned int logotimer;
	int bBgmEnabled;
	unsigned int movetimer;
	float kumotimer;
	char kumoindex;
};

struct ObjectInfo
{
	CCL_INFO* colli_t;
	unsigned int colliNumb;
	float colliRange;
};

struct ObjCmnInfo
{
	unsigned __int8 id;
	unsigned __int8 copy;
};

// Texture animation used in Casinopolis
struct TEXANIMINFO
{
	NJS_MATERIAL* pmat;
	unsigned int newtexid;
	unsigned int orgtexid;
};

// UV animation used by Casinopolis objects
struct TEXPATINFO
{
	int patpitch;
	int texcnt;
	TEXANIMINFO* texaniminfo;
	char* patinfo;
	char nowpatnum;
};

// Volume information for MirenSoundPlayOneShotSE
struct VolumeInfo
{
	__int16 nearVolOfs;
	__int16 farVolOfs;
	float nearDist;
	float farDist;
};

// Data for each item box hud item
struct ITEM_MANAGER_DATA
{
	int item_list;
	float item_pos;
	float scale;
	int random_ring;
};

// Data for item box hud
struct ITEM_MANAGER
{
	int mode;
	int current_list;
	unsigned int counter;
	ITEM_MANAGER_DATA itemdata[20];
};

// Additional data for air item box
struct OBJECT_ITEMBOX_AIR_DATA
{
	int flag;
	int item;
	NJS_POINT3 position;
	int panel_ang;
	float scale;
	float timer;
};

// Parabola data (see MakeParabolaInitSpeed, ChkParabolaEnd)
struct sParabola
{
	NJS_POINT3 pos_start;
	NJS_POINT3 pos_end;
	float gravity;
	int time;
	NJS_POINT3 speed;
};

// Key information for Amy's locked doors
struct KeyInfo
{
	int LastKey;
	int Buff[4];
	int Point;
	int RightCount;
	int LeftCount;
};

// Handle information for Amy's handles
struct amyhndlstr
{
	int hndlmode;
	int touchflag;
	int turnang;
	int hndlangy;
	NJS_POINT3 hndlpos;
};

struct Big_ydata
{
	int attr;
	int angx;
	int angz;
	float ypos;
};

// Lure height information set by calcFishingLureY
struct Big_ypos
{
	Big_ydata top;
	Big_ydata bottom;
	Big_ydata water;
};

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

//Eggman worker

//Egg Hornet
struct Eggmoble1Work
{
	NJS_POINT3 direction;
	NJS_POINT3 velocity;
	NJS_POINT3 accel;
	NJS_POINT3 tar_pos;
	NJS_POINT3 src_pos;
	Angle3 dang;
	int frame;
	int role_ang;
	float speed;
};

struct Egm1BakuWk
{
	NJS_POINT3* Posp;
	unsigned int Cnt;
	Egm1BakuCmdEnum Cmd;
};

struct Egm1BarriPrmT
{
	task* BossTp;
	float OfsAlpha;
	float GenFrame;
	float Progress;
	int TexChgFrame;
	int TexChgFrameMax;
	unsigned int PatNum;
	unsigned __int8 Stat;
	unsigned __int8 Req;
};

struct Egm1JetPrmT
{
	task* BossTp;
};

struct Egm1BgmWk
{
	int Cnt;
};

struct Egm1ClayManWk
{
	NJS_POINT3 Pos;
	int Frame;
	unsigned __int8 Mode;
};

struct Egm1CrushPrm
{
	NJS_TEXLIST* TexListPtr;
	obj* Obj;
	NJS_POINT3* PosPtr;
	Angle3* AngPtr;
	void(__cdecl* ExeFnc)(NJS_POINT3*, int);
	int AngSpdRenge;
	float BaseSpd;
	unsigned __int16 DropFrameInterval;
};

struct Egm1JetWk
{
	int TexChgFrame[5];
	int TexChgFrameMax[5];
	unsigned int TexPatNum[5];
	int BTexChgFrame[3];
	int BTexChgFrameMax[3];
	__int16 BTexPatNum[3];
	NJS_POINT3* Egm1JetPntSt[5];
	NJS_POINT3* Egm1JetPntEd[5];
	unsigned int Egm1JetPntNum[5];
	task* BossTp;
	float Scale;
	float JetStat;
	unsigned int PassFrame;
	Egm1JetCmdEnum Cmd;
};

struct Egm1MissileWk
{
	NJS_POINT3 Pnt3p[3];
	NJS_POINT3* DstPosPtr;
	float MslT;
	float DeltaT;
	unsigned __int16 SmokeTime;
	unsigned __int16 HomingTime;
};

struct Egm1MissilesPrm
{
	taskwk* Parent_twp;
	NJS_POINT3* DstPosPtr;
	unsigned __int16 FireInterval;
	unsigned __int16 HomingInterval;
	unsigned __int16 MissileNum;
};

struct Egm1MissilesT
{
	Egm1MissilesPrm Prm;
	unsigned __int16 Frame;
	unsigned __int16 FireNum;
};

struct Egm1OneClayPrm
{
	obj* Obj;
	NJS_TEXLIST* TexListPtr;
	NJS_POINT3* Posp;
	NJS_POINT3* Spdp;
	float Scale;
	int AngSpdRange;
};

struct Egm1OneClayWk
{
	obj* Obj;
	NJS_TEXLIST* TexListPtr;
	NJS_POINT3 Spd;
	Angle3 AngSpd;
	float Scale;
};

struct Egm1SmokeManWk
{
	NJS_POINT3 Pos;
	unsigned __int8 Mode;
};


struct Egm1SpdDivTblT
{
	float t;
	float div;
};

//Egg hornet worker
struct bossextwk
{
	bosswk bwk;
	int Fmode;
	Egm1LocEnum AimArea;
	Egm1LocEnum OldArea;
	NJS_POINT3 FlyRoute[3];
	float LasSpd;
	float FlyT;
	float FlyDeltaT;
	float BasePosY;
	int UdFrq;
	int BaseAngX;
	int KeepAngX;
	int KeepAngY;
	float RotRBase;
	int RotAngOld;
	int RotAng;
	int RotAngSpd;
	int FuriTotalOneSin;
	int FuriSin;
	int FuriAngX;
	int BankZ;
	int NowBankZ;
	Egm1BarriPrmT BariPrm;
	int ForceBreakTime;
	__int16 WaitTime;
	char ShotNum;
	char NowShotsNum;
	char ShotsNum;
	char DispMode;
	char LoopSndFlg;
	char HitPoint;
	char DropDieFlg;
	char TikalFlg;
};

struct STAGENAME_WORK
{
	unsigned __int8 mode;
	unsigned __int8 timer;
	unsigned __int8 StageNo;
	unsigned __int8 StageType;
	unsigned __int16 DispTime;
	NJS_TEXLIST* texlist;
};

struct __HPPOS
{
	NJS_POINT3 pos;
	int hp;
};

union STAFF_DATA_FLAG
{
	unsigned __int8 SnapShotNo;
	unsigned __int8 pvrindex;
	unsigned __int8 interval;
};

struct STAFF_DATA
{
	unsigned __int8 StaffType;
	STAFF_DATA_FLAG flag;
	char* str;
};

struct STAFFROLL_DATA
{
	STAFF_DATA* StaffTbl;
	int nbStaff;
};

struct standlight_taskwk
{
	char free0;
	char free1;
	unsigned __int8 id;
	unsigned __int8 free2;
	__int16 flag;
	unsigned __int16 free3;
	int rot_x_angle;
	NJS_MODEL* beam_model_p;
	void* free6;
	Angle3 ang;
	NJS_POINT3 pos;
	float rot_x;
	float length;
	float width;
	colliwk* cwp;
	eventwk* ewp;
};

// Event camera

struct EV_CAMERA_W_target
{
	unsigned __int8 mode;
	NJS_POINT3 pos;
	task* obj;
};

struct EV_CAMERA_W_pers
{
	unsigned __int8 hokanMode;
	unsigned __int16 time;
	unsigned __int16 count;
	int ang;
	int oldang;
	int newang;
};

struct EV_CAMERA_W_path
{
	cpathtag* tag;
	float timer;
	float speed;
};

struct EV_CAMERA_W_chase
{
	unsigned __int8 mode;
	unsigned __int16 time;
	unsigned __int16 count;
	NJS_POINT3 pos;
	Angle3 ang;
	float dist;
	NJS_POINT3 pos2;
	Angle3 ang2;
	float dist2;
	task* obj;
};

struct EV_CAMERA_W
{
	unsigned __int8 posHokanMode;
	unsigned __int8 angHokanMode;
	unsigned __int16 post;
	unsigned __int16 posc;
	unsigned __int16 angt;
	unsigned __int16 angc;
	NJS_POINT3 pos;
	Angle3 ang;
	NJS_POINT3 oldpos;
	Angle3 oldang;
	NJS_POINT3 newpos;
	Angle3 newang;
	EV_CAMERA_W_target target;
	EV_CAMERA_W_chase chase;
	EV_CAMERA_W_pers pers;
	EV_CAMERA_W_path path;
	EVINFO2* event;
};

// Chao

struct AL_SHAPE_ELEMENT
{
	unsigned __int8 type;
	unsigned __int8 DefaultEyeNum;
	unsigned __int8 DefaultMouthNum;
	unsigned __int8 HonbuNum;
	unsigned __int8 ObakeHead;
	unsigned __int8 ObakeBody;
	unsigned __int8 MedalNum;
	unsigned __int8 ColorNum;
	unsigned __int8 NonTex;
	unsigned __int8 JewelNum;
	unsigned __int8 MultiNum;
	char MinimalParts[8];
	__int16 HPos;
	__int16 VPos;
	__int16 APos;
	__int16 Growth;
	unsigned __int8 name[8];
	unsigned __int16 Skill[8];
};

struct AL_RECORD
{
	char RecordFlag;
	AL_TIME time;
	AL_SHAPE_ELEMENT ShapeElement;
};

struct CHAO_ID
{
	unsigned int gid[2];
	unsigned int id[2];
	unsigned int num;
};

struct AL_KARATE_PERSONAL_INFO
{
	unsigned __int8 rank;
	unsigned __int8 level;
	unsigned __int8 tournament;
	unsigned __int16 nbBattle;
	unsigned __int16 nbWin;
	unsigned __int16 nbLose;
	unsigned __int16 nbDraw;
};

struct AL_PARTS
{
	unsigned int MinimalFlag;
	char MinimalParts[8];
};

struct AL_KNOWLEDGE_PLAYER
{
	char like;
	unsigned __int8 fear;
	unsigned __int16 distance;
	unsigned __int16 meet;
};

struct AL_KNOWLEDGE_CHAO
{
	CHAO_ID id;
	char like;
	unsigned __int8 fear;
	unsigned __int16 distance;
	unsigned __int16 meet;
};

struct AL_KNOWLEDGE_BTL
{
	unsigned __int8 ArtFlag;
	unsigned __int8 DanceFlag;
	unsigned __int8 SongFlag;
	unsigned __int8 MusicFlag;
	unsigned __int16 SToyFlag;
	unsigned __int16 LToyFlag;
	int KwTimer;
	AL_KNOWLEDGE_PLAYER player[6];
	AL_KNOWLEDGE_CHAO chao[20];
	AL_KNOWLEDGE_OTHER bhv[120];
};

struct AL_GENE
{
	char IsAnalyzed;
	char EggColor;
	CHAO_ID MotherID;
	CHAO_ID FatherID;
	char MotherName[8];
	char FatherName[8];
	char MGroundMotherName[8];
	char MGroundFatherName[8];
	char FGroundMotherName[8];
	char FGroundFatherName[8];
	unsigned __int8 Abl[8][2];
	unsigned __int8 LifeTime[2];
	char HPos[2];
	char VPos[2];
	char APos[2];
	char Personality[13][2];
	unsigned __int8 Taste[2];
	unsigned __int8 Tv[2];
	unsigned __int8 Music[2];
	unsigned __int8 Color[2];
	unsigned __int8 NonTex[2];
	unsigned __int8 Jewel[2];
	unsigned __int8 Multi[2];
	unsigned __int8 EyePos[2];
	unsigned __int8 EyeScl[2];
	unsigned __int8 EyeRatio[2];
	unsigned __int8 EyeColor[2];
};

struct AL_KNOWLEDGE_DX
{
	AL_KNOWLEDGE_PLAYER player[6];
};

struct CHAO_PARAM_GC
{
	unsigned __int8 GBAChao;
	unsigned __int8 GBAEgg;
	unsigned __int8 GBABerry[8];
	unsigned __int8 padding0;
	unsigned __int8 padding1;
	unsigned int GBARing;
	unsigned __int8 BootMethod;
	unsigned __int8 Birthplace;
	unsigned __int8 name[7];
	unsigned __int8 GBAType;
	unsigned __int8 GBASkin;
	unsigned __int8 GBAMood;
	unsigned __int8 GBABelly;
	unsigned __int8 GBASleepy;
	unsigned __int8 GBALonelyness;
	unsigned __int8 padding2;
	unsigned __int8 Exp[8];
	unsigned __int8 Abl[8];
	unsigned __int8 Lev[8];
	unsigned __int16 Skill[8];
	unsigned __int16 GBAPalette[16];
	unsigned __int8 rmsg[16];
	unsigned int runaway;
	unsigned __int8 dummy[4];
	unsigned __int8 type;
	unsigned __int8 place;
	__int16 like;
	char ClassNum;
	unsigned __int16 age;
	unsigned __int16 old;
	unsigned __int16 life;
	unsigned __int16 LifeMax;
	unsigned __int16 nbSucceed;
	CHAO_ID ChaoID;
	unsigned int LifeTimer;
	AL_BODY_INFO body;
	AL_RACE_PERSONAL_INFO race;
	AL_KARATE_PERSONAL_INFO karate;
	AL_PARTS PartsBTL;
	AL_EMOTION emotion;
	AL_KNOWLEDGE_BTL knowledgeBTL;
	AL_GENE gene;
	int IsInitializedDX;
	AL_PARTS partsDX;
	AL_RACE_PERSONAL_INFO_DX raceDX;
	AL_KNOWLEDGE_DX knowledgeDX;
};

struct chao_save_info
{
	CHAO_PARAM_GC param;
	unsigned __int8 dummy[736];
};

struct AL_RACE_INFO
{
	char RaceActiveFlag[3];
	char CourseChallengedLevel[5];
	char CourseClearedLevel[5];
	AL_RECORD CourseRecord[5];
	AL_TIME time;
	char CurrRaceNum;
	char CurrCourseNum;
	char CurrLevelNum;
	char nbUserEntry;
	chao_save_info ChaoInfo[8];
};

struct AL_ST_POS
{
	NJS_POINT3 pos;
	int angy;
};

struct TREE_SAVE_INFO
{
	unsigned __int8 kind;
	unsigned __int8 state;
	unsigned __int8 param;
	unsigned __int8 water;
	char FruitGrowth[3];
	unsigned __int8 angy_pos;
};

struct PatChngObj
{
	obj* obj_p;
	NJS_MATERIAL* mat_pa[8];
};

struct CHAO_INFO
{
	int EntryFlag;
};

struct AL_ENT_MENU_WORK
{
	int status;
	int command;
	eLayerNum LayerNum;
	int nbRace;
	int nbCourse;
	int nbGarden;
	eGardenNum GardenCursor;
	int MaxLevel;
	int nbCourseBeginner;
	int nbCourseJewel;
	int key;
	int level;
	int xpos;
	int ypos;
	int pre_xpos;
	int pre_ypos;
	int back_ypos;
	int nbEntry;
	task* ChaoTask[8];
	CHAO_INFO ChaoInfo[4][8];
	task* pManager;
	task* pMessageBar;
	task* pBackGround;
	task* pLargeTitleBar;
	task* pSmallTitleBar;
	task* pBlueButtonModoru;
	task* pBlueButtonSusumu;
	int GardenActiveFlag[4];
	const unsigned int* pMsgList;
};

struct MOTION_INFO
{
	unsigned __int16 mode;
	float frame;
	float start;
	float end;
	float spd;
	NJS_MOTION* pMotion;
};

struct MOTION_TABLE
{
	NJS_MOTION* pMotion;
	__int16 mode;
	__int16 posture;
	int next;
	int link_step;
	float start;
	float end;
	float spd;
};

struct MOTION_CTRL
{
	unsigned __int16 flag;
	unsigned __int16 posture;
	int curr_num;
	int next_num;
	float multi_spd;
	float link_spd;
	float ratio;
	MOTION_INFO minfo[2];
	MOTION_TABLE* table;
};


union TREE_WORK_counter
{
	unsigned __int8 b[4];
	unsigned __int16 w[2];
	unsigned int l;
	float f;
	void* ptr;
};

union TREE_WORK_timer
{
	char b[4];
	__int16 w[2];
	int l;
	float f;
	void* ptr;
};

union TREE_WORK_value
{
	char b[4];
	__int16 w[2];
	int l;
	float f;
	void* ptr;
};

struct FRUIT_INFO
{
	float growth;
	float AimGrowth;
	int SwingCounter;
	NJS_POINT3 pos;
	int angx;
	int angy;
};

struct TREE_WORK
{
	char mode;
	char smode;
	unsigned __int8 id;
	unsigned __int8 btimer;
	__int16 flag;
	unsigned __int16 wtimer;
	TREE_WORK_counter counter;
	TREE_WORK_timer timer;
	TREE_WORK_value value;
	Angle3 ang;
	NJS_POINT3 pos;
	NJS_POINT3 scl;
	colliwk* cwp;
	void* ewp;
	unsigned __int8 kind;
	unsigned __int8 state;
	float growth;
	int life;
	int water;
	unsigned __int8 pos_num;
	FRUIT_INFO fruit[3];
	float scale;
	float ScaleSpd;
	int ThirstyFlag;
	int WaterFlag;
	int LeafRotAng;
	float LeafWidth;
	int LeafDispNum;
	int FruitRotAng;
	float FruitWidth;
	int FruitDispNum;
	int SwingFlag;
	int SwingCountFlag;
	float SwingDist;
	int RollAngle;
	int TouchAngle;
	int ChaoSwingPhase;
	int RollPhase;
	int garden;
	TREE_SAVE_INFO* pMySaveInfo;
	NJS_TEXLIST* texlist;
	cnkobj* pLocalObject;
	cnkobj* pShadowObject;
	cnkobj* pCopyObject;
};

union chaowk_counter
{
	unsigned __int8 b[4];
	unsigned __int16 w[2];
	unsigned int l;
	float f;
	void* ptr;
};

union chaowk_timer
{
	char b[4];
	__int16 w[2];
	int l;
	float f;
	void* ptr;
};

union chaowk_value
{
	char b[4];
	__int16 w[2];
	int l;
	float f;
	void* ptr;
};

enum eKW_BHV_KIND : __int32
{
	KW_BHV_ART = 0x0,
	KW_BHV_DANCE = 0x1,
	KW_BHV_SING = 0x2,
	KW_BHV_MUSIC = 0x3,
	KW_BHV_MINI1 = 0x4,
	KW_BHV_MINI2 = 0x5,
	KW_BHV_MINI3 = 0x6,
	KW_BHV_MINI4 = 0x7,
	KW_BHV_MINI5 = 0x8,
	KW_BHV_MINI6 = 0x9,
	KW_BHV_MINI7 = 0xA,
	KW_BHV_MINI8 = 0xB,
	KW_BHV_MINI9 = 0xC,
	KW_BHV_MINI10 = 0xD,
	KW_BHV_MINI11 = 0xE,
	KW_BHV_MINI12 = 0xF,
	KW_BHV_MINI13 = 0x10,
	KW_BHV_MINI14 = 0x11,
	KW_BHV_MINI15 = 0x12,
	KW_BHV_MINI16 = 0x13,
	KW_BHV_MINI17 = 0x14,
	KW_BHV_MINI18 = 0x15,
	KW_BHV_TOY1 = 0x16,
	KW_BHV_TOY2 = 0x17,
	KW_BHV_TOY3 = 0x18,
	KW_BHV_TOY4 = 0x19,
	KW_BHV_TOY5 = 0x1A,
	KW_BHV_TOY6 = 0x1B,
	KW_BHV_TOY7 = 0x1C,
	KW_BHV_TOY8 = 0x1D,
	KW_BHV_TOY9 = 0x1E,
	KW_BHV_TOY10 = 0x1F,
	KW_BHV_TOY11 = 0x20,
	KW_BHV_FLY = 0x21,
	KW_BHV_SWIM = 0x22,
	KW_BHV_CLIMB_TREE = 0x23,
	KW_BHV_CLIMB_WALL = 0x24,
	KW_BHV_WATER = 0x25,
	KW_BHV_SWING = 0x26,
	KW_BHV_SIT = 0x27,
	KW_BHV_DENGURI = 0x28,
	KW_BHV_TOILET = 0x29,
	KW_BHV_PYON = 0x2A,
	KW_BHV_BOWLING = 0x2B,
	KW_BHV_FUKKIN = 0x2C,
	KW_BHV_SHIRIFURI = 0x2D,
	KW_BHV_OJIGI = 0x2E,
	KW_BHV_CHIWA = 0x2F,
	KW_BHV_NADERU = 0x30,
	KW_BHV_AGERU = 0x31,
	KW_BHV_TALK = 0x32,
	KW_BHV_PUNCH = 0x33,
	KW_BHV_OKOSU = 0x34,
	KW_BHV_TEFURI = 0x35,
	KW_BHV_HAKUSYU = 0x36,
	KW_BHV_SURIYORU = 0x37,
	KW_BHV_AKANBE = 0x38,
	KW_BHV_WA = 0x39,
	KW_BHV_NAGERU = 0x3A,
	KW_BHV_FIGHT = 0x3B,
	KW_BHV_IGAMI = 0x3C,
	KW_BHV_LISTEN = 0x3D,
	KW_BHV_WATCH = 0x3E,
};

struct KW_BHV_ENTRY
{
	eKW_BHV_KIND bhv;
};

struct AL_BEHAVIOR
{
	unsigned __int16 Flag;
	unsigned __int16 Mode;
	unsigned __int16 SubMode;
	unsigned __int16 MoveMode;
	int InterruptFlag;
	int Timer;
	int SubTimer;
	int LimitTimer;
	int BehaviorTimer;
	unsigned __int16 Intention;
	unsigned __int16 IntentionMode;
	unsigned __int16 IntentionSubMode;
	unsigned int IntentionTimer[18];
	unsigned int IntervalTimer[128];
	int FreeWork;
	float MoveRadius;
	NJS_POINT3 BasePos;
	int(__cdecl* PrevFunc)(task*);
	int nbBhvFuncEntry;
	int CurrBhvFuncNum;
	int(__cdecl* BhvFuncList[16])(task*);
	int ReserveTimerList[16];
	int CurrKwBhvNum;
	KW_BHV_ENTRY KwBhvList[4];
	unsigned int dummy[16];
};

struct AL_GROUP_OBJECT_LIST
{
	cnkobj* Zero[40];
	cnkobj* Normal[40];
	cnkobj* Swim[40];
	cnkobj* Fly[40];
	cnkobj* Run[40];
	cnkobj* Power[40];
};

struct AL_SHAPE
{
	al_object* pObject;
	al_object* CurrObjectList[40];
	AL_GROUP_OBJECT_LIST* pObjectList;
	AL_GROUP_OBJECT_LIST* pObjectListH;
	AL_GROUP_OBJECT_LIST* pObjectListD;
	NJS_POINT3 BodyPos;
	NJS_POINT3 HeadPos;
	NJS_POINT3 LeftHandPos;
	NJS_POINT3 RightHandPos;
	NJS_POINT3 LeftFootPos;
	NJS_POINT3 RightFootPos;
	NJS_POINT3 MouthPos;
	NJS_POINT3 MouthVec;
	NJS_POINT3 LeftEyePos;
	NJS_POINT3 LeftEyeVec;
	NJS_POINT3 RightEyePos;
	NJS_POINT3 RightEyeVec;
	cnkobj* pLeftHandItemObject;
	NJS_TEXLIST* pLeftHandItemTexlist;
	float LeftHandItemScale;
	float LeftHandItemActiveFlag;
	cnkobj* pRightHandItemObject;
	NJS_TEXLIST* pRightHandItemTexlist;
	float RightHandItemScale;
	float RightHandItemActiveFlag;
	unsigned __int16* palette;
	unsigned __int16 Flag;
	__int16 ColorNum;
	__int16 EnvNum;
	unsigned int IconColor;
	float SclH;
	float SclV;
	float CamDist;
};

struct AL_ZONE
{
	unsigned __int16 Flag;
	unsigned __int16 CurrNum;
	float Ratio;
	NJS_LINE Plane;
};

struct AL_ICON_INFO
{
	unsigned __int16 Mode;
	unsigned __int16 TexNum;
	unsigned __int16 Timer;
	NJS_POINT3 Offset;
	NJS_POINT3 Pos;
	NJS_POINT3 Velo;
	NJS_POINT3 Scl;
	NJS_POINT3 SclSpd;
};

struct AL_ICON
{
	__int16 CurrType;
	__int16 NextType;
	int Timer;
	int NextTimer;
	int PuniPhase;
	int PosPhase;
	unsigned int Color;
	unsigned __int16 TexAnimNum;
	unsigned __int16 TexAnimTimer;
	int ang;
	NJS_POINT3 Up;
	NJS_POINT3 Pos;
	AL_ICON_INFO Upper;
	AL_ICON_INFO Lower;
};

struct chaowk
{
	char mode;
	char smode;
	unsigned __int8 id;
	unsigned __int8 btimer;
	__int16 flag;
	unsigned __int16 wtimer;
	chaowk_counter counter;
	chaowk_timer timer;
	chaowk_value value;
	Angle3 ang;
	NJS_POINT3 pos;
	NJS_POINT3 scl;
	colliwk* cwp;
	void* ewp;
	unsigned int Timer;
	task* pMayu;
	task* pBookTask;
	int NestFlag;
	task* pAnyTask;
	task* pAimTask;
	int AimNum;
	int RememberNum;
	int pitch;
	float ClimbFirstPos;
	int IsParamCopy;
	CHAO_PARAM_GC* pParamGC;
	TMP_PARAM tmpParam;
	int Stamina;
	int AimStamina;
	task* tp;
	int pre_ang[3];
	unsigned int ChaoFlag;
	unsigned __int16 ColliFormat;
	float CurrZone;
	MOTION_CTRL MotionCtrl;
	MOTION_CTRL MiniMotionCtrl;
	MOTION_TABLE MiniMotionTable[4];
	AL_BEHAVIOR Behavior;
	AL_SHAPE Shape;
	AL_FACE_CTRL Face;
	AL_ICON Icon;
	AL_ZONE Zone;
	AL_PERCEPTION Perception;
	void* pWork;
};

// Chao (SA1 related)

struct ADV1_AL_IMPLESSION
{
	char like;
	unsigned __int8 meet;
};

struct ADV1_AL_IMPLESSION2
{
	unsigned int id;
	unsigned __int8 like;
	unsigned __int8 meet;
};

struct ADV1_AL_MEMORY
{
	ADV1_AL_IMPLESSION player[6];
	ADV1_AL_IMPLESSION2 chao[32];
};

struct al_status
{
	unsigned __int8 type;
	unsigned __int8 garden_num;
	char like;
	unsigned __int8 key1;
	unsigned __int8 name[8];
	unsigned __int16 swim;
	unsigned __int16 fly;
	unsigned __int16 run;
	unsigned __int16 power;
	unsigned __int16 health;
	unsigned __int16 health_max;
	unsigned __int8 fruit_kind[8];
	float h_pos;
	float v_pos;
	float growth;
	unsigned __int16 intimate;
	unsigned __int16 life;
	unsigned __int16 old;
	unsigned __int16 get_minimal;
	unsigned __int8 win;
	unsigned __int8 key2;
	unsigned __int16 flag;
	NJS_POINT3 pos;
	unsigned int age;
	unsigned int id;
	char animal_part_num[7];
	unsigned __int8 key3;
	unsigned __int16 pt_swim;
	unsigned __int16 pt_fly;
	unsigned __int16 pt_run;
	unsigned __int16 pt_power;
	char kindness;
	char aggressive;
	char curiosity;
	unsigned __int8 charm;
	unsigned __int8 breed;
	unsigned __int8 sleep;
	unsigned __int8 hunger;
	unsigned __int8 tedious;
	unsigned __int8 tire;
	unsigned __int8 stress;
	unsigned __int8 narrow;
	unsigned __int8 pleasure;
	unsigned __int8 anger;
	unsigned __int8 sorrow;
	unsigned __int8 fear;
	unsigned __int8 lonely;
	ADV1_AL_MEMORY memory;
	unsigned __int8 nbSucceed;
	unsigned __int8 lane;
	unsigned __int8 IsCPU;
	char key4;
	char exist;
	unsigned __int16 mayu_timer;
	unsigned __int8 race_time[20];
	unsigned __int8 extra_num;
};

struct ADV1_AL_TMP_STATUS
{
	float aim_h_pos;
	float aim_v_pos;
	float aim_grouth;
	NJS_ARGB aim_argb;
};

struct al_action_ctrl
{
	unsigned __int16 old_mode;
	unsigned __int16 curr_mode;
	unsigned __int16 req_mode;
	unsigned __int16 flag;
	unsigned __int16 timer;
};

struct al_motion_table
{
	NJS_MOTION* motion;
	__int16 mode;
	__int16 next;
	float start_frame;
	float end_frame;
	float frame_spd;
	float link_spd;
};

struct ADV1_AL_MOTION_CTRL
{
	unsigned __int16 mode;
	unsigned __int16 flag;
	unsigned __int16 timer;
	__int16 curr_num;
	__int16 link_num;
	__int16 req_num;
	float nframe;
	float pframe;
	float lframe;
	float link_ratio;
	float multi_spd;
	al_motion_table* motion_table;
};

struct ADV1_AL_PERCEPTION
{
	unsigned int flag;
};

union alifewk_work
{
	char b[4];
	__int16 w[2];
	int l;
	float f;
	void* ptr;
};

struct ADV1_AL_SHADOW
{
	unsigned __int16 flag;
	NJS_POINT3 colli_center;
	float colli_top;
	float colli_radius;
	float colli_bottom;
	float cliff_height;
	float bound_side;
	float bound_floor;
	float bound_friction;
	float bound_ceiling;
	int bound_add_angle;
	float shadow_scl;
	float shadow_scl_ratio;
	float water_pos;
	xyyzzxsdwstr si;
};

struct ADV1_AL_BODY
{
	obj* object;
	obj* my_parts[50];
	obj* zero[50];
	obj* standard[50];
	obj* fly[50];
	obj* run[50];
	obj* power[50];
	obj* swim[50];
	NJS_TEXLIST* tex;
};

struct ADV1_AL_FACE
{
	unsigned __int16 timer1;
	unsigned __int16 timer2;
	unsigned __int16 timer3;
	int eye_keep_timer;
	int mouse_keep_timer;
	int eyelid_keep_timer;
	unsigned int flag;
	int eyelid_brink_ang;
	int eyelid_close_ang;
	int eyelid_curr_close_ang;
	int eyelid_request_close_ang;
	int eyelid_default_close_ang;
	int eyelid_express_ang;
	int eyelid_curr_express_ang;
	int eyelid_request_express_ang;
	int eyelid_default_express_ang;
	__int16 eye_default_num;
	__int16 eye_request_num;
	__int16 mouse1_default_num;
	__int16 mouse2_default_num;
	__int16 mouse1_request_num;
	__int16 mouse2_request_num;
};

struct ADV1_AL_ICON
{
	unsigned __int16 flag;
	unsigned __int16 mode;
	unsigned __int16 timer;
	unsigned __int16 next_mode;
	unsigned __int16 next_timer;
	NJS_POINT3 pos;
	NJS_POINT3 velo;
	NJS_POINT3 diff;
	int phase;
	unsigned __int16 timer_u;
	unsigned __int16 icon_num_u;
	unsigned __int16 smode_u;
	float ang_u;
	float ang_spd_u;
	NJS_POINT3 pos_u;
	NJS_POINT3 velo_u;
	NJS_POINT3 scl_u;
	NJS_POINT3 sclspd_u;
	unsigned __int16 timer_d;
	unsigned __int16 icon_num_d;
	unsigned __int16 smode_d;
	float ang_d;
	float ang_spd_d;
	NJS_POINT3 pos_d;
	NJS_POINT3 velo_d;
	NJS_POINT3 scl_d;
	NJS_POINT3 sclspd_d;
	NJS_COLOR col;
};


struct alifewk
{
	alifewk_work work;
	NJS_POINT3 velo;
	NJS_POINT3 acc;
	Angle3 ang_aim;
	Angle3 ang_spd;
	float rad;
	float height;
	float weight;
	task* lock_tp;
	void(__cdecl* dest_org)(task*);
	unsigned __int16 action_timer;
	unsigned __int16 stimer;
	unsigned __int16 voice_timer;
	unsigned __int16 rhythm_timer;
	__int16 old_mode;
	__int16 old_smode;
	__int16 curr_mode;
	__int16 curr_smode;
	__int16 req_mode;
	__int16 req_smode;
	__int16 hold_mode;
	unsigned int flag;
	unsigned __int16 action_mode;
	NJS_BGRA mat_col;
	float mayu_scl;
	float mayu_alpha;
	int name_alpha;
	unsigned __int16 name_timer;
	unsigned __int16 damage_timer;
	int pitch;
	int yaw;
	int roll;
	__int16 aim_num;
	NJS_POINT3 home;
	NJS_POINT3 aim;
	NJS_POINT3 pre;
	NJS_POINT3 offset;
	Angle3 aim_ang;
	int view_angle;
	float view_range2;
	float hear_range2;
	float buyo_scale;
	float chao_scl;
	float aim_hpos;
	float aim_vpos;
	float aim_growth;
	unsigned __int16 sekkachi_timer;
	al_status* status;
	ADV1_AL_TMP_STATUS tmp_status;
	al_action_ctrl action_ctrl;
	ADV1_AL_MOTION_CTRL motion_ctrl;
	ADV1_AL_SHADOW shadow;
	ADV1_AL_BODY body;
	ADV1_AL_FACE face;
	ADV1_AL_ICON icon;
	ADV1_AL_PERCEPTION perception;
	task* mytask;
	task* pAdv2ChaoTask;
};

struct AlrEffUnit
{
	NJS_POINT3 trans;
	NJS_POINT3 velocity;
	unsigned __int8 frame;
	char frame_inc_time;
	unsigned __int8 mode;
	unsigned __int8 flag;
	unsigned __int16 timer;
	unsigned __int16 tmp;
};

#pragma pack(push, 2)
struct TaskCommunication
{
	task* obj_tp;
	char obj_kind;
	char command;
	unsigned __int8 status;
};
#pragma pack(pop)

struct AlrEff
{
	AlrEffUnit unit[5];
	NJS_TEXLIST* texlist;
	NJS_TEXANIM* anim;
	float scale;
	void(__cdecl* exec)(AlrEffUnit*, NJS_POINT3*, NJS_POINT3*);
	unsigned __int16 frame_inc_time_max;
	unsigned __int8 anim_num;
	unsigned __int8 unit_num;
};

struct AlrEvent
{
	unsigned __int8 level;
	unsigned __int8 cnt;
	unsigned __int16 timer;
	void(__cdecl* event)(void*);
};

struct Alr
{
	AlrEff eff;
	AlrEvent event;
	TaskCommunication communication;
	unsigned __int8 id;
	unsigned __int8 area;
	unsigned __int16 flag;
	unsigned __int8 rank;
	unsigned __int8 mode;
	unsigned __int8 command;
	unsigned __int8 smode;
	float sloop;
	float pathpos;
	unsigned __int8 use_path;
	unsigned __int8 ex_mode;
	unsigned __int16 ex_cnt;
	int amplitude;
	unsigned __int8 now;
	unsigned __int8 motion;
	unsigned __int8 chao_flag;
	unsigned __int8 voice;
	unsigned __int8 zone;
	unsigned __int16 para_timer;
	float monooto;
	char sleep;
	char tire;
	char stress;
	char tmp;
	alifewk* awp;
	taskwk* twp;
};

// Tutorials

struct GH_PVR_TEX
{
	__int16 index;
	__int16 x;
	__int16 y;
};

struct TUTO_PAGE
{
	unsigned __int16 x;
	unsigned __int16 y;
	unsigned __int16 seqflag;
	unsigned __int16 w;
	unsigned __int16 h;
	__int16* sprite[2];
};

struct TUTO_BOOK
{
	unsigned __int16 nbPage;
	unsigned __int16 PurposePage;
	TUTO_PAGE* page;
	NJS_TEXLIST* texlist[2];
	char* pvmname[2];
};

// Characters

struct bbl_small_work
{
	NJS_POINT3 pos;
	float radius;
	float ceil;
	int life;
	float yuragi_x;
	float yuragi_z;
	float yuragi_r;
	float y_spd;
	int yuragi_ang;
	bbl_small_work* prev;
	bbl_small_work* next;
};

struct bbl_small_taskwk
{
	char free1;
	char turn;
	unsigned __int8 id;
	unsigned __int8 free2;
	__int16 flag;
	__int16 bbls_count;
	bbl_small_work* bbls_work_p;
	bbl_small_work* free_list_p;
	bbl_small_work* top_list_p;
	Angle3 ang;
	NJS_POINT3 pos;
	float radius;
	float freq;
	float bbl_max;
	colliwk* cwp;
	eventwk* ewp;
};
#endif /* SADXMODLOADER_SADXSTRUCTSNEW_H */