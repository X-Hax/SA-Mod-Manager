/**
 * SADX Mod Loader.
 * SADX variables based on symbols.
 */

#ifndef SADXMODLOADER_SADXVARSNEW_H
#define SADXMODLOADER_SADXVARSNEW_H

#include "MemAccess.h"
#include "d3d8types.h"
#include "SADXStructsNew.h"

// General
DataArray(TaskFuncPtr, RoundMasterList, 0x90BF38, 44); // level tasks
DataArray(TaskFuncPtr, ScrollMasterList, 0x90C1F0, 44); // skybox tasks
DataArray(PL_KILLCOLLI**, KillingCollisionModelsListList, 0x915908, 43);
DataArray(_OBJ_LANDTABLE*, objLandTable, 0x97DA08, 120);
DataPointer(___stcFog, gFog, 0x3ABDC60);
DataPointer(___stcClip, gClipMap, 0x3ABDC70);
DataPointer(___stcClip, gClipSky, 0x3ABDCA0);
DataPointer(int, GameTimer, 0x3B0F108);
DataPointer(__int16, ssStageNumber, 0x3B22DCC);
DataPointer(int, ssActNumber, 0x3B22DEC);
DataPointer(int, BlockMask, 0x3B36D48); // Mask to show/hide level geometry with blockbit

// Player
DataArray(player_parameter, playerwk_default, 0x9154E8, 8);
DataArray(task*, playertp, 0x3B42E30, 8);
DataArray(taskwk*, playertwp, 0x3B42E10, 8);
DataArray(motionwk2*, playermwp, 0x3B36DD0, 8);
DataArray(playerwk*, playerpwp, 0x3B3CDF0, 8);

// Light
DataArray(LE_PALIGHT_ENV, le_plyrPal, 0x903E88, 255); // Character lights
DataArray(LE_LIGHT_ENV, le_stgPL, 0x900918, 30); // Unused stage lights array
DataArray(LE_LIGHT_ENV, le_stg, 0x900E88, 255); // Stage lights
DataPointer(LE_LIGHT_ENV, le_env_dflt, 0x9008B4); // Default stage light
DataPointer(LE_LIGHT_ENV, le_ctrl_dflt, 0x9008E4); // Backup default stage light

// Story sequence
DataPointer(int, slSeqRunning, 0x90A0A8); // Story running if 1
DataPointer(short, ssSceneNo, 0x3B18A24); // Story ID
DataArray(SEQUENCE, seqTable, 0x03B183A8, 8); // Character story data array
DataPointer(SEQUENCE*, pCurSequence, 0x3B0EF4C); // Current character story data
DataPointer(SEQ_SECTIONTBL*, pCurSectionList, 0x3B18A08); // Character story sections array
DataPointer(SEQ_SECTION*, pCurSection, 0x3B18DB0); // Current story section

// Geometry collision
DataArray(uint8_t, boolMobileLandObject, 0x3B2D410, 256);
DataArray(obj, objMobileLandObject, 0x3B33930, 256);
DataPointer(uint16_t, numMobileEntry, 0x3B2D510);
DataArray(_OBJ_LANDCOLL, MobileEntry, 0x3B32D30, 256);

// Path
DataArray(pathgrp, pathgrp_tbl, 0x91A858, 22);     // List of null-terminated path list
DataArray(pathtag*, pathdata_0002, 0x9190F4, 5);
DataArray(pathtag*, pathdata_0100, 0x10363E4, 6);  // Emerald Coast Act 1 paths
DataArray(pathtag*, pathdata_0101, 0x10363FC, 9);  // Emerald Coast Act 2 paths
DataArray(pathtag*, pathdata_0200, 0xC02550, 2);   // Windy Valley Act 1 paths
DataArray(pathtag*, pathdata_0202, 0xC02558, 8);   // Windy Valley Act 3 paths
DataArray(pathtag*, pathdata_0301, 0x26B55DC, 5);  // Twinkle Park Act 2 paths
DataArray(pathtag*, pathdata_0400, 0x26AA508, 5);  // Speed Highway Act 1 paths
DataArray(pathtag*, pathdata_0402, 0x26AA530, 4);  // Speed Highway Act 3 paths
DataArray(pathtag*, pathdata_0500, 0x26AA530, 3);  // Red Mountain Act 1 paths
DataArray(pathtag*, pathdata_0501, 0x24C31B4, 11); // Red Mountain Act 2 paths
DataArray(pathtag*, pathdata_0600, 0x2230F14, 3);  // Sky Deck Act 1 paths
DataArray(pathtag*, pathdata_0700, 0x20343CC, 4);  // Lost World Act 1 paths
DataArray(pathtag*, pathdata_0701, 0x20343DC, 5);  // Lost World Act 2 paths
DataArray(pathtag*, pathdata_0801, 0xE3BDEC, 2);   // Ice Cap Act 2 paths
DataArray(pathtag*, pathdata_0802, 0xE3BDF4, 11);  // Ice Cap Act 3 paths
DataArray(pathtag*, pathdata_0901, 0x1D83980, 9);  // Casinopolis Act 2 paths
DataArray(pathtag*, pathdata_0A00, 0x1A57714, 4);  // Final Egg Act 1 paths
DataArray(pathtag*, pathdata_0A01, 0x1A57700, 3);  // Final Egg Act 2 paths
DataArray(pathtag*, pathdata_0A02, 0x1A57720, 5);  // Final Egg Act 3 paths
DataArray(pathtag*, pathdata_1A00, 0x2BBC468, 4);  // Station Square Act 1 paths
DataArray(pathtag*, pathdata_1300, 0x21422140, 4); // Perfect Chaos paths

// Enemy
DataPointer(int, ComboTimer, 0x3B29D48);
DataPointer(int, ComboScore, 0x3B29D28);
DataPointer(int, EnemyScore, 0x3B0F104);

// Object
DataArray(TEX_PVMTABLE*, ListofPvmList, 0x90EB68, 44); // Object textures
DataArray(_OBJ_ITEMTABLE*, objItemTable, 0x974AF8, 344); // Object lists, 43 levels * 8 acts
DataArray(char, PlayerHoldingItemID, 0x3B36DC8, 8); // Identifier for current held object

// Boss
DataPointer(char, bossmtn_flag, 0x3C5A7EF);

// Camera
DataArray(_OBJ_CAMERAMODE, CameraMode, 0x975410, 77);        // List of camera modes, see CAMMD enum
DataArray(_OBJ_CAMERAADJUST, CameraAdjust, 0x975410, 28);    // List of camera adjusts (how it switches camera)
DataPointer(_CameraSystemWork, cameraSystemWork, 0x3B2CAD8); // Camera system information (current mode, etc)
DataPointer(_camcontwk, cameraControlWork, 0x3B2C660);       // Camera task information (position, angle, target...)

// Lighting
DataArray(unsigned int[512], LSPAL_0, 0x3B12210, 10);
DataArray(unsigned int, lsPal0_v_forGC, 1024, 0x3B111A8);
DataPointer(NJS_ARGB, lig_argb, 0x3B17210);
DataPointer(float, ls_iamb, 0x3B121AC);
DataPointer(NJS_VECTOR, ds_pool, 0x3B121B4); // Used in normal scaling for some objects
DataPointer(NJS_VECTOR, ds_current, 0x3B121F8); // Used in normal scaling for some objects

// Ocean data
DataArray(NJS_TEXTURE_VTX[35][4], gsaStripPool, 0x3D0B928, 2); // Ocean garbage array
DataArray(stcWaterSurface, gasPoolStat, 0x3D0B8F0, 2); // OceanData A/B
#endif /* SADXMODLOADER_SADXVARSNEW_H */