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
	void* G_pCameraEntry;
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

struct __declspec(align(4)) ___stcFog
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

struct __declspec(align(4)) stcWaterSurface
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

struct __declspec(align(4)) DDlgType
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

struct __declspec(align(4)) AvaStgActPrmT
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

struct __declspec(align(4)) CharMdlWk
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

struct __declspec(align(4)) CharSelWk
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

// Title screen worker

struct __declspec(align(4)) TitleNewWk
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

#endif /* SADXMODLOADER_SADXSTRUCTSNEW_H */