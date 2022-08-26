/**
 * SADX Mod Loader.
 * SADX actions and motions based on symbols.
 */

#ifndef SADXMODLOADER_SADXMOTNEW_H
#define SADXMODLOADER_SADXMOTNEW_H

#include "ninja.h"
#include "MemAccess.h"

// Chao Animals - Animations in Action Stages
DataPointer(NJS_ACTION, action_goma_move_goma, 0x949FCC);
DataPointer(NJS_ACTION, action_pen_move_pen, 0x949FD4);
DataPointer(NJS_ACTION, action_tuba_move_tuba, 0x949FDC);
DataPointer(NJS_ACTION, action_kuja_move_kuja, 0x949FE4);
DataPointer(NJS_ACTION, action_oum_move_oum, 0x949FEC);
DataPointer(NJS_ACTION, action_koar_move_koar, 0x949FF4);
DataPointer(NJS_ACTION, action_wara_move_wara, 0x949FFC);
DataPointer(NJS_ACTION, action_banb_move_banb, 0x94A004);
DataPointer(NJS_ACTION, action_usa_move_usa, 0x94A00C);
DataPointer(NJS_ACTION, action_mogu_move_mogu, 0x94A014);
DataPointer(NJS_ACTION, action_zou_move_zou, 0x94A01C);
DataPointer(NJS_ACTION, action_lion_move_lion, 0x94A024);
DataPointer(NJS_ACTION, action_gori_move_gori, 0x94A02C);
DataPointer(NJS_ACTION, action_suka_move_suka, 0x94A034);
DataPointer(NJS_ACTION, action_rako_move_rako, 0x94A03C);

// Chao Animals - Motion lists for garden animations
DataArray(NJS_MOTION*, pMotionMinimalHug, 0x03612FF4, 15);
DataArray(NJS_MOTION*, pMotionMinimalWait, 0x03619AD4, 15);
DataArray(NJS_MOTION*, pMotionMinimalWalk, 0x0361555C, 15);
DataArray(NJS_MOTION*, pMotionMinimalFly, 0x03619B10, 15);

// Chao Animals - All motions
DataPointer(NJS_MOTION, motion_banb_hand_banb, 0x0360D158);
DataPointer(NJS_MOTION, motion_banb_meet_banb, 0x0361EC70);
DataPointer(NJS_MOTION, motion_banb_move_banb, 0x0093A8B0);
DataPointer(NJS_MOTION, motion_banb_wait_banb, 0x03617588);
DataPointer(NJS_MOTION, motion_goma_hand_goma, 0x036093D8);
DataPointer(NJS_MOTION, motion_goma_meet_goma, 0x0361A420);
DataPointer(NJS_MOTION, motion_goma_move_goma, 0x0092ACD8);
DataPointer(NJS_MOTION, motion_goma_wait_goma, 0x03615928);
DataPointer(NJS_MOTION, motion_gori_hand_gori, 0x0360F318);
DataPointer(NJS_MOTION, motion_gori_meet_gori, 0x03621610);
DataPointer(NJS_MOTION, motion_gori_move_gori, 0x00945958);
DataPointer(NJS_MOTION, motion_gori_wait_gori, 0x03618368);
DataPointer(NJS_MOTION, motion_koar_hand_koar, 0x03612648);
DataPointer(NJS_MOTION, motion_koar_meet_koar, 0x03624F20);
DataPointer(NJS_MOTION, motion_koar_move_koar, 0x009359A8);
DataPointer(NJS_MOTION, motion_koar_wait_koar, 0x03619708);
DataPointer(NJS_MOTION, motion_kuja_hand_kuja, 0x0360B2F8);
DataPointer(NJS_MOTION, motion_kuja_meet_kuja, 0x0361CB60);
DataPointer(NJS_MOTION, motion_kuja_move_kuja, 0x00931420);
DataPointer(NJS_MOTION, motion_kuja_wait_kuja, 0x03616808);
DataPointer(NJS_MOTION, motion_kuja_walk_kuja, 0x03613DD0);
DataPointer(NJS_MOTION, motion_lion_hand_lion, 0x03610058);
DataPointer(NJS_MOTION, motion_lion_meet_lion, 0x036224F0);
DataPointer(NJS_MOTION, motion_lion_move_lion, 0x00944088);
DataPointer(NJS_MOTION, motion_lion_wait_lion, 0x036188E8);
DataPointer(NJS_MOTION, motion_mogu_hand_mogu, 0x03611EC8);
DataPointer(NJS_MOTION, motion_mogu_meet_mogu, 0x03624280);
DataPointer(NJS_MOTION, motion_mogu_move_mogu, 0x0093ECE0);
DataPointer(NJS_MOTION, motion_mogu_wait_mogu, 0x036191E8);
DataPointer(NJS_MOTION, motion_oum_hand_oum, 0x0360C488);
DataPointer(NJS_MOTION, motion_oum_meet_oum, 0x0361E110);
DataPointer(NJS_MOTION, motion_oum_move_oum, 0x009334E0);
DataPointer(NJS_MOTION, motion_oum_wait_oum, 0x036170E8);
DataPointer(NJS_MOTION, motion_oum_walk_oum, 0x03615550);
DataPointer(NJS_MOTION, motion_pen_hand_pen, 0x0360A078);
DataPointer(NJS_MOTION, motion_pen_meet_pen, 0x0361B350);
DataPointer(NJS_MOTION, motion_pen_move_pen, 0x0092D390);
DataPointer(NJS_MOTION, motion_pen_wait_pen, 0x03615D88);
DataPointer(NJS_MOTION, motion_rako_hand_rako, 0x0360AA48);
DataPointer(NJS_MOTION, motion_rako_meet_rako, 0x0361BE10);
DataPointer(NJS_MOTION, motion_rako_move_rako, 0x0949FB8);
DataPointer(NJS_MOTION, motion_rako_wait_rako, 0x036162D8);
DataPointer(NJS_MOTION, motion_suka_hand_suka, 0x03612FE8);
DataPointer(NJS_MOTION, motion_suka_meet_suka, 0x036258A0);
DataPointer(NJS_MOTION, motion_suka_move_suka, 0x00947890);
DataPointer(NJS_MOTION, motion_suka_wait_suka, 0x03619AC8);
DataPointer(NJS_MOTION, motion_tuba_hand_tuba, 0x0360BC08);
DataPointer(NJS_MOTION, motion_tuba_meet_tuba, 0x0361D690);
DataPointer(NJS_MOTION, motion_tuba_move_tuba, 0x0092F2B0);
DataPointer(NJS_MOTION, motion_tuba_wait_tuba, 0x03616C88);
DataPointer(NJS_MOTION, motion_tuba_walk_tuba, 0x03614870);
DataPointer(NJS_MOTION, motion_usa_hand_usa, 0x0360DCB8);
DataPointer(NJS_MOTION, motion_usa_meet_usa, 0x0361F8F0);
DataPointer(NJS_MOTION, motion_usa_move_usa, 0x0093CF68);
DataPointer(NJS_MOTION, motion_usa_wait_usa, 0x03617A68);
DataPointer(NJS_MOTION, motion_wara_hand_wara, 0x0360ECB8);
DataPointer(NJS_MOTION, motion_wara_meet_wara, 0x03620CB0);
DataPointer(NJS_MOTION, motion_wara_move_wara, 0x00938520);
DataPointer(NJS_MOTION, motion_wara_wait_wara, 0x03618088);
DataPointer(NJS_MOTION, motion_zou_hand_zou, 0x03611118);
DataPointer(NJS_MOTION, motion_zou_meet_zou, 0x03623660);
DataPointer(NJS_MOTION, motion_zou_move_zou, 0x00941600);
DataPointer(NJS_MOTION, motion_zou_wait_zou, 0x03618E08);

#endif /* SADXMODLOADER_SADXMOTNEW */