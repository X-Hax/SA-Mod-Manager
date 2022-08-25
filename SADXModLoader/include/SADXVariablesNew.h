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
DataPointer(float, ScreenRaitoX, 0x8928C0); // Horizontal stretch (X res / 640)
DataPointer(float, ScreenRaitoY, 0x8928C4); // Vertical stretch (Y res / 480)
DataPointer(float, late_z_ofs___, 0x3ABD9C0); // Draw queue depth bias for models
DataPointer(float, sp_zoffset, 0x3C4859C); // Draw queue depth bias for particles
DataPointer(float, sa_s3_z_ofs, 0x3ABD9BC); // Draw queue depth bias for sprites drawn with saDrawSprite3D
DataPointer(float, late_s3_z_ofs, 0x3ABD9B8); // Draw queue depth bias for sprites
DataPointer(unsigned __int16, usPlayer, 0x3B22DC0); // Player ID
DataPointer(__int16, ssGameMode, 0x3B22DE4); // GameState (enum MD_GAME)
DataPointer(unsigned int, ulGlobalMode, 0x3ABDC7C); // Game Mode (njUserMain mode, enum MD_GameMode)
DataPointer(int, loop_count, 0x3B1117C); // MissedFrames
DataPointer(int, loop_const, 0x3B11178); // Frame increment
DataPointer(Uint32, ulGlobalTimer, 0x3B0F13C); // FrameCounter
DataPointer(Uint32, g_CurrentFrame, 0x389D7DC); // Framerate setting (1 is 60 FPS, 2 is 30 FPS)
DataPointer(Uint32, gu32GameCnt, 0x3ABDF58); // FrameCounter2
DataPointer(Uint32, gu32LocalCnt, 0x3ABDF5C); // FrameCounterUnpaused
DataArray(TaskFuncPtr, RoundMasterList, 0x90BF38, 44); // Level tasks
DataArray(TaskFuncPtr, ScrollMasterList, 0x90C1F0, 44); // Skybox tasks
DataArray(PL_KILLCOLLI**, KillingCollisionModelsListList, 0x915908, 43);
DataArray(_OBJ_LANDTABLE*[8], objLandTable, 0x97DA08, 43); // Array of landtable pointers, 8 acts per level
DataPointer(Angle, ds_perspective_value, 0x3AB98EC); // HorizontalFOV_BAMS
DataPointer(NJS_TEXLIST*, njds_texList, 0x3ABD950); // CurrentTexList
DataPointer(NJS_POINT2COL, nj_system_bg_p2c_, 0x3CE7164); // Global POINT2COL color
DataPointer(NJS_ARGB, nj_constant_material_, 0x3D0F7F0);
DataPointer(NJS_MATRIX, nj_unit_matrix_, 0x389D650);
DataPointer(___stcFog, gFog, 0x3ABDC60);
DataPointer(___stcClip, gClipMap, 0x3ABDC70);
DataPointer(___stcClip, gClipSky, 0x3ABDCA0);
DataPointer(NJS_POINT3, gSkyScale, 0x3ABDC94);
DataPointer(int, GameTimer, 0x3B0F108);
DataPointer(__int16, ssStageNumber, 0x3B22DCC); // CurrentLevel
DataPointer(int, ssActNumber, 0x3B22DEC); // CurrentAct
DataPointer(int, BlockMask, 0x3B36D48); // Mask to show/hide level geometry with blockbit
DataPointer(char, scNumPlayer, 0x3B0EF34); // Number of lives
DataPointer(Sint16, ssNumRing, 0x3B0F0E4);
DataPointer(PLAYER_CONTINUE_DATA, continue_data, 0x3B42F80); // Last checkpoint data
DataArray(_OBJ_LANDENTRY*, pDisplayEntry, 0x3B2D518, 1024); // List of all drawn landtable models for the current frame
DataPointer(_OBJ_LANDTABLE*, pObjLandTable, 0x3B2F718); // CurrentLandTable
DataPointer(int, late_execMode, 0x3AB98AC); // Draw queue state (0 run; 1 skip, 2 skip and draw a black screen)
DataPointer(task*, windtp, 0x03C5B32C); // Wind object (Sonic's quills, dandelions in Windy Valley)
DataPointer(NJS_ARGB, cur_argb, 0x3AB9864); // Current global sprite color
DataPointer(int, nj_control_3d_flag_, 0x3D0F9C8); // Current NJ_CONTROL_3D
DataPointer(int, tikal_sw, 0x94A2A4); // Set to 1 (default) to force depth bias for Tikal sprites (20048.0)
DataPointer(unsigned char, TEXTURE_FLAG, 0x3ABDF79); // Flag to let the game know the font for "Now Saving" is available
DataPointer(int, mode, 0x3B22E78); // Item selection for the quit prompt in the pause menu

// Event
DataPointer(EVINFO2, evInfo, 0x03B2C568); // Current cutscene struct

// Player
DataPointer(unsigned __int8, gu8flgPlayingMetalSonic, 0x3B18DB5); // Metal Sonic flag
DataArray(player_parameter, playerwk_default, 0x9154E8, 8);
DataArray(task*, playertp, 0x3B42E30, 8);
DataArray(taskwk*, playertwp, 0x3B42E10, 8);
DataArray(motionwk2*, playermwp, 0x3B36DD0, 8);
DataArray(playerwk*, playerpwp, 0x3B3CDF0, 8);
DataPointer(taskwk*, gpCharTwp, 0x3ABDF60); // Contains a player's taskwk when its exec function is running
DataPointer(playerwk*, gpCharPwp, 0x3ABDF64); // Contains a player's playerwk when its exec function is running
DataPointer(Angle, angGx, 0x3B0F10C); // Gravity X angle
DataPointer(Angle, angGz, 0x3B0F0F4); // Gravity Z angle

// Sonic
DataPointer(sParabola, SonicPaboBuff, 0x3C53A68);

// Tails
DataPointer(sSonicCtrl, SonicCtrlBuff, 0x3C539F8);
DataPointer(sMRacePath*, PathTbl_Sonic, 0x3C539F4); // Current race path for Sonic
DataPointer(sMRacePath*, PathTbl_Miles, 0x3C53A64); // Current race path for Tails
DataArray(sMRacePath*, PPT_MRaceEachStage, 0x91C0B8, 10); // Race path list (sonic, tails)

// Knuckles
DataArray(KnFragmSetStr, fragmset_tbl, 0x3C52B20, 3); // current emerald set information
DataArray(KnFragmNmbStr, fragmnmb_tbl, 0x7E0CD8, 6);

// Amy
DataArray(KeyInfo, KeyBuff, 0x3C72C38, 8);         // Key information for locked doors
DataPointer(amyhndlstr, amyhndlstatus, 0x3C5B300); // Handle progress

// E102
DataPointer(E102WK*, e102_work_ptr, 0x3C53B70);  // Additional information for E102 (laser, targets, etc.)
DataPointer(char, e102_hover_flag, 0x3C53C40);   // Is hover active
DataPointer(char, e102_hover_flag_p, 0x3C53C41); // Hover sound toggle

// Big
DataPointer(int, Sakana_Num, 0x3C524E8);
DataPointer(__int16, Big_Fish_Flag, 0x3C524EC);
DataPointer(int, Big_Sakana_Weight_Limit, 0x3C524F0);
DataPointer(float, reel_tension_add, 0x3C524F4);
DataPointer(task*, Big_Fish_Ptr, 0x3C524F8);
DataPointer(int, Big_Fishing_Timer, 0x3C524FC);
DataPointer(task*, Big_Lure_Ptr, 0x3C52500);
DataPointer(NJS_POINT3, big_item_pos, 0x3C52504);
DataPointer(int, Big_Sakana_Weight, 0x3C52510);
DataPointer(int, Big_Sakana_Kind_High, 0x3C52514);
DataPointer(int, Big_Stg12_Flag, 0x3C52518);
DataPointer(__int16, Big_Sakana_Weight_High, 0x3C5251C);
DataPointer(float, water_level, 0x3C52520);
DataPointer(float, reel_length, 0x3C52524);
DataPointer(float, reel_length_d, 0x3C52528);
DataPointer(float, reel_tension, 0x3C5252C);
DataPointer(float, reel_tension_aim, 0x3C52530);
DataPointer(Angle, reel_angle, 0x3C52534);
DataPointer(int, caution_timer, 0x3C52538);
DataArray(NJS_OBJECT*, lure_kind, 0x3C5253C, 7);

// Input
DataPointer(Uint8, ucInputStatus, 0x909FB0); // Is input enabled
DataArray(Uint8, ucInputStatusForEachPlayer, 0x909FB4, 4); // Is input enabled for specific player
DataArray(SONIC_INPUT, input_data, 0x3B0E368, 8);
DataArray(PDS_PERIPHERAL*, per, 0x3B0E77C, 8); // ControllerPointers
DataArray(SONIC_INPUT, input_dataG, 0x3B0E7A0, 8); // NormalizedAnalogs
DataArray(PDS_PERIPHERAL, perG, 0x3B0E7F0, 8); // Controllers

// Light
DataArray(LE_PALIGHT_ENV, le_plyrPal, 0x903E88, 255); // Character lights
DataArray(LE_LIGHT_ENV, le_stgPL, 0x900918, 30); // Unused stage lights array
DataArray(LE_LIGHT_ENV, le_stg, 0x900E88, 255); // Stage lights
DataPointer(LE_LIGHT_ENV, le_env_dflt, 0x9008B4); // Default stage light
DataPointer(LE_LIGHT_ENV, le_ctrl_dflt, 0x9008E4); // Backup default stage light
DataPointer(int, lig_curGjPaletteNo___, 0x3B12208); // CurrentLightType

// Story sequence
DataPointer(int, slSeqRunning, 0x90A0A8); // Story running if 1
DataPointer(short, ssSceneNo, 0x3B18A24); // Story ID
DataArray(SEQUENCE, seqTable, 0x03B183A8, 8); // Character story data array
DataPointer(SEQUENCE*, pCurSequence, 0x3B0EF4C); // Current character story data
DataPointer(SEQ_SECTIONTBL*, pCurSectionList, 0x3B18A08); // Character story sections array
DataPointer(SEQ_SECTION*, pCurSection, 0x3B18DB0); // Current story section

// Geometry collision
DataPointer(BOOL, boolLandCollision, 0x915460);           // Enable/disable geometry collisions
DataPointer(float, mleriRangeRad, 0x915458);              // Minimum radius for collision lookup
DataArray(uint8_t, boolMobileLandObject, 0x3B2D410, 256); // Tells if a objMobileLandObject entry is taken
DataArray(obj, objMobileLandObject, 0x3B33930, 256);      // List of NJS_OBJECTS used for mobile collisions
DataPointer(__int16, numMobileEntry, 0x3B2D510);          // Number of active mobile collisions
DataArray(_OBJ_LANDCOLL, MobileEntry, 0x3B32D30, 256);    // List of active mobile collisions
DataPointer(int, ri_landcoll_nmb, 0x3B36D38);
DataArray(_OBJ_LANDCOLL, ri_landcoll, 0x3B32728, 128);
DataPointer(__int16, numLandCollList, 0x3B32724);
DataArray(_OBJ_LANDCOLL, LandCollList, 0x3B2F720, 1024);

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
DataArray(TEX_PVMTABLE*, ListofPvmList, 0x90EB68, 44);              // Object textures
DataArray(_OBJ_ITEMTABLE*, objItemTable, 0x974AF8, 344);            // Object lists, 43 levels * 8 acts
DataArray(ITEM_INFOMATION, item_info, 0x9BF190, 9);                 // Itembox items information
DataArray(char, PlayerHoldingItemID, 0x3B36DC8, 8);                 // Identifier for current held object
DataPointer(OBJECT_SAVEPOINT_DATA*, savepoint_data, 0x3B42F7C);
DataPointer(_OBJ_ITEMTABLE*, pObjItemTable, 0x3C4E448);             // Current Object List
DataPointer(__int16, numStatusEntry, 0x3C4E454);
DataArray(OBJ_CONDITION, objStatusEntry, 0x3C4E460, 1024);          // Set file entries
DataPointer(OBJECT_ITEMBOX_AIR_DATA*, itembox_air_data, 0x3C5A9D4); // Additional data for air item boxes

// Object lists
DataPointer(_OBJ_ITEMTABLE, objItemTable00, 0x27C71C4);
DataPointer(_OBJ_ITEMTABLE, objItemTable01, 0x1036AB0);
DataPointer(_OBJ_ITEMTABLE, objItemTable02, 0xC02BB8);
DataPointer(_OBJ_ITEMTABLE, objItemTable03, 0x26B5EEC);
DataPointer(_OBJ_ITEMTABLE, objItemTable04, 0x26AB2D0);
DataPointer(_OBJ_ITEMTABLE, objItemTable05, 0x24C3974);
DataPointer(_OBJ_ITEMTABLE, objItemTable06, 0x22319D4);
DataPointer(_OBJ_ITEMTABLE, objItemTable07, 0x2034AA8);
DataPointer(_OBJ_ITEMTABLE, objItemTable08, 0xE3C80C);
DataPointer(_OBJ_ITEMTABLE, objItemTable09, 0x1D84AF0);
DataPointer(_OBJ_ITEMTABLE, objItemTable10, 0x1A57EE8);
DataPointer(_OBJ_ITEMTABLE, objItemTable1201, 0x17C6344);
DataPointer(_OBJ_ITEMTABLE, objItemTable1202, 0x17C5BCC);
DataPointer(_OBJ_ITEMTABLE, objItemTable1203, 0x17C54D0);
DataPointer(_OBJ_ITEMTABLE, objItemTable1204, 0x17C4D94);
DataPointer(_OBJ_ITEMTABLE, objItemTableb0, 0x1120264);
DataPointer(_OBJ_ITEMTABLE, objItemTable16, 0x1120C34);
DataPointer(_OBJ_ITEMTABLE, objItemTable17, 0x118FE54);
DataPointer(_OBJ_ITEMTABLE, objItemTable18, 0x11F00EC);
DataPointer(_OBJ_ITEMTABLE, objItemTable19, 0x1422A1C);
DataPointer(_OBJ_ITEMTABLE, objItemTableegm1, 0x156F7FC);
DataPointer(_OBJ_ITEMTABLE, objItemTableegm2, 0x15E8F58);
DataPointer(_OBJ_ITEMTABLE, objItemTableegm3, 0x165D554);
DataPointer(_OBJ_ITEMTABLE, objItemTableBossRobo, 0x16B56C4); // Zero
DataPointer(_OBJ_ITEMTABLE, objItemTable23, 0x14FBC58); // E-101 Beta
DataPointer(_OBJ_ITEMTABLE, objItemTable24, 0x1522D08); // E-101 R
DataPointer(_OBJ_ITEMTABLE, objItemTableSS, 0x2BBCCFC);
DataPointer(_OBJ_ITEMTABLE, objItemTableEC00, 0x10F401C);
DataPointer(_OBJ_ITEMTABLE, objItemTableEC30, 0x1101D20);
DataPointer(_OBJ_ITEMTABLE, objItemTableMR, 0x1104760);
DataPointer(_OBJ_ITEMTABLE, objItemTablePast, 0x111EE98);
DataPointer(_OBJ_ITEMTABLE, objItemTableMCart, 0x9C2304);
DataPointer(_OBJ_ITEMTABLE, objItemTableSht, 0x2998E98);
DataPointer(_OBJ_ITEMTABLE, objItemTableSBoard, 0x173BF0C);
DataPointer(_OBJ_ITEMTABLE, objItemTableGarden00, 0x38F6E54); // SS Garden
DataPointer(_OBJ_ITEMTABLE, objItemTableGarden01, 0x38F6E44); // EC Garden
DataPointer(_OBJ_ITEMTABLE, objItemTableGarden02, 0x38F6E30); // MR Garden
DataPointer(_OBJ_ITEMTABLE, objItemTableAL_Race, 0x33A0AEC);
DataPointer(_OBJ_ITEMTABLE, ObjItemTableMi, 0x170F854); // Mission Mode object list

// Cart
DataArray(__int16, cartColor, 0x88C004, 8); // Cart colors
DataArray(CCL_INFO, cci_cart, 0x38A6BF0, 8); // Cart collisions, one per character
DataArray(CART_PLAYER_PARAMETER, CartParameter, 0x38C5DA8, 8); // Cart physics
DataPointer(CART_OTHER_PARAM, CartOtherParam, 0x38C5F88);
DataArray(CART_LOAD_DATA, cart_load, 0x38C7FF0, 18);
DataPointer(task*, RaceManageTask_p, 0x3C5D554); // Pointer to race manager task
DataPointer(BOOL, CartGoalFlag, 0x3D08E00);
DataPointer(ENEMY_CART_DATA*, cart_data, 0x3D08E0C); // Pointer to player's cart data

// Boss
DataPointer(char, bossmtn_flag, 0x3C5A7EF);
DataPointer(taskwk*, chaostwp, 0x3C5A7D8); // pointer to current Chaos boss taskwk

// Camera
DataPointer(taskwk*, camera_twp, 0x3B2CBB0);                          // Camera task work pointer
DataPointer(taskwk, oldTaskWork, 0x3B2C9D8);                          // Copy of previous camera task work data, private use (prefer pOldTaskWork to access it)
DataPointer(BOOL, cameraready, 0x3B2CBB8);                            // Tells if the camera task is running
DataArray(_OBJ_CAMERAMODE, CameraMode, 0x975410, 77);                 // List of camera modes, see CAMMD enum
DataArray(_OBJ_CAMERAADJUST, CameraAdjust, 0x9758E0, 28);             // List of camera adjusts (how it switches camera)
DataPointer(_CameraSystemWork, cameraSystemWork, 0x3B2CAD8);          // Camera system information (current mode, etc), private use (prefer pCameraSystemWork to access it)
DataPointer(_camcontwk, cameraControlWork, 0x3B2C660);                // Camera script information (position, angle, target...), private use (prefer camcont_wp to access it)
DataPointer(FCWRK, fcwrk, 0x3B2C958);                                 // Free camera information
DataPointer(Uint32, free_camera_mode, 0x3B2CBA8);                     // Free camera flags
DataPointer(_OBJ_CAMERAENTRY*, pObjCameraEntry, 0x3B2CAA4);           // Camera layout
DataPointer(int, flagCameraNoUnderWater, 0x3B2C6C0);                  // Do not draw water filter
DataArray(_OBJ_CAMERAENTRYTABLE*, objCameraEntryTable, 0x3C58180, 8); // Array of camera layout information pointers (one per act)
DataPointer(_OBJ_CAMERAENTRYTABLE*, pNumCameraEntry, 0x3B2C9C4);      // Pointer to current camera layout information
DataPointer(_OBJ_CAMERAMODE*, pObjCameraMode, 0x3B2CACC);             // Pointer to the camera modes array
DataPointer(_OBJ_CAMERAADJUST*, pObjCameraAdjust, 0x3B2CABC);         // Pointer to the camera adjusts array
DataPointer(Sint32, debug_disp_camera_timer, 0x3B2C6C8);              // Unused
DataPointer(Bool, boolCameraCollision, 0x915090);                     // If the camera layout should be run
DataPointer(Bool, Player_stop_flag, 0x3B2CAAC);                       // If the player is not moving and on ground
DataPointer(NJS_POINT3, CameraInertia, 0x3B2C9CC);                    // Difference between previous camera position and current
DataPointer(taskwk*, pOldTaskWork, 0x7DFF8C);                         // Pointer to oldTaskWork, for external access
DataPointer(_camcontwk*, camcont_wp, 0x7DFF90);                       // Pointer to cameraControlWork
DataPointer(_CameraSystemWork*, pCameraSystemWork, 0x7DFF94);         // Pointer to cameraSystemWork
DataPointer(_OBJ_ADJUSTPARAM*, pObjAdjustParam, 0x7DFF98);            // Pointer to objAdjustParam
DataPointer(Sint32, default_camera_mode, 0x3B2CBAC);                  // Default camera mode (no camera active, or when returning from event camera)
DataPointer(Sint32, default_camera_adjust, 0x3B2CAC4);                // Default adjust mode (no camera active, or when returning from event camera)
DataPointer(Sint32, start_camera_mode, 0x3B2CAA8);                    // The camera mode set when the camera is initialized

// Sound
DataArray(_SEcallbuf, sebuf, 0x3B292F8, 36); // SoundQueue (length 20 in xbox version)
DataArray(taskwk*, gpDolbyTask, 0x3B29B90, 36); // SoundQueueOriginEntities (length 20 in xbox version)

// Lighting
DataArray(unsigned int[512], LSPAL_0, 0x3B12210, 10);
DataArray(unsigned int, lsPal0_v_forGC, 1024, 0x3B111A8);
DataPointer(NJS_ARGB, lig_argb, 0x3B17210);
DataPointer(float, ls_iamb, 0x3B121AC);
DataPointer(NJS_VECTOR, ds_pool, 0x3B121B4); // Used in normal scaling for some objects
DataPointer(NJS_VECTOR, ds_current, 0x3B121F8); // Used in normal scaling for some objects

// Menu
DataPointer(const DialogPrmType, DialogAskQuit, 0x7DD48C); // Dialog prompt for level quitting
DataArray(DialogPrmType, DialogPrm, 0x7EE328, 22); // Main menus dialog prompts
DataArray(void*, CreateModeFncPtrs, 0x10D7B4C, 14); // Init functions for main menus
DataArray(void*, FreeModeFncPtrs, 0x10D7B84, 14); // Free functions for main menus
DataArray(PVMEntry*, MenuTexlists, 0x10D7CB0, 5); // Table of main menu texlists arrays (one per language)
DataArray(int*, AvaTexLdLists, 0x10D7CC4, 14); // Which texlist a menu should pick from MenuTexlists (enum AvaTexLdEnum does not match sadx pc)
DataPointer(ADVERTISE_WORK, AdvertiseWork, 0x3B2A2FA); // General menu information
DataPointer(task*, DialogTp, 0x3B2C588); // Pointer to dialog manager task
DataPointer(task*, SeqTp, 0x3C5E8D0); // Pointer to menu manager task
DataPointer(BOOL, TldFlg, 0x3C5E8E0); // Menu ready flag
DataPointer(AvaStgActT, AvaCmnPrm, 0x3C5FED0);
DataPointer(task*, TrialActStelTp, 0x3C5FEE0);
DataPointer(task*, TitleNewTp, 0x3C5FF00);

// Ocean data
DataArray(NJS_TEXTURE_VTX[35][4], gsaStripPool, 0x3D0B928, 2); // Ocean garbage array
DataArray(stcWaterSurface, gasPoolStat, 0x3D0B8F0, 2); // OceanData A/B
DataArray(stcAnim, cosSaltWaterAnimation, 0x7EC250, 16); // Emerald Coast ocean animations

// Fog data
DataPointer(bool, gu8FogEnbale, 0x3ABDCFE); // FogEnabled
DataPointer(___stcFogEmu, gFogEmu, 0x909EB4); // Fog settings
DataArray(___stcFog*, pFogTable_Stg00, 0x27C6A00, 1);
DataArray(___stcFog*, pFogTable_Stg01, 0xE99E90, 3);
DataArray(___stcFog*, pFogTable_Stg02, 0xAFEAD4, 3);
DataArray(___stcFog*, pFogTable_Stg03, 0x26B3480, 4);
DataArray(___stcFog*, pFogTable_Stg04, 0x24CA598, 3);
DataArray(___stcFog*, pFogTable_Stg05, 0x22407B4, 3);
DataArray(___stcFog*, pFogTable_Stg06, 0x203A148, 3);
DataArray(___stcFog*, pFogTable_Stg07, 0x1E79B60, 3);
DataArray(___stcFog*, pFogTable_Stg08, 0xC67F90, 4);
DataArray(___stcFog*, pFogTable_Stg09, 0x1C46A80, 4);
DataArray(___stcFog*, pFogTable_Stg10, 0x19C90A4, 3);
DataArray(___stcFog*, pFogTable_Stg12, 0x17C3504, 3);
DataArray(___stcFog*, pFogTable_Chaos00, 0x111F76C, 1);
DataArray(___stcFog*, pFogTable_Chaos02, 0x1120674, 1);
DataArray(___stcFog*, pFogTable_Chaos04, 0x118FA3C, 1);
DataArray(___stcFog*, pFogTable_Chaos06, 0x11EF160, 2);
DataArray(___stcFog*, pFogTable_Chaos07, 0x1420EA8, 2);
DataArray(___stcFog*, pFogTable_Egm01, 0x1556B70, 1);
DataArray(___stcFog*, pFogTable_Egm02, 0x15E8830, 1);
DataArray(___stcFog*, pFogTable_Egm03, 0x165D370, 1);
DataArray(___stcFog*, pFogTable_Robo, 0x16B4DEC, 1);
DataArray(___stcFog*, pFogTable_E101, 0x14D5CF4, 1);
DataArray(___stcFog*, pFogTable_E101r, 0x152262C, 1);
DataArray(___stcFog*, pFogTable_Adv00, 0x2AA3E78, 6);
DataArray(___stcFog*, pFogTable_Adv01AB, 0x10F24B0, 7);
DataArray(___stcFog*, pFogTable_Adv01C, 0x1100D80, 6);
DataArray(___stcFog*, pFogTable_Adv02, 0x11035F8, 8);
DataArray(___stcFog*, pFogTable_Adv03, 0x111E63C, 3);
DataArray(___stcFog*, pFogTable_MiniCart, 0x9BF5A4, 6);
DataArray(___stcFog*, pFogTable_Sht, 0x27D6E00, 2);
DataArray(___stcFog*, pFogTable_Sand, 0x173BBB0, 1);

// Level draw distance
//pClipMap_Stg00
DataArray(___stcClip*, pClipMap_Stg01, 0xE99E84, 3);
DataArray(___stcClip*, pClipMap_Stg02, 0xAFEAC8, 3);
DataArray(___stcClip*, pClipMap_Stg03, 0x26B3474, 3);
DataArray(___stcClip*, pClipMap_Stg04, 0x24CA58C, 3);
DataArray(___stcClip*, pClipMap_Stg05, 0x22407A8, 3);
DataArray(___stcClip*, pClipMap_Stg06, 0x203A13C, 3);
DataArray(___stcClip*, pClipMap_Stg07, 0x1E79B54, 3);
DataArray(___stcClip*, pClipMap_Stg08, 0xC67F80, 4);
DataArray(___stcClip*, pClipMap_Stg09, 0x1C46A70, 4);
DataArray(___stcClip*, pClipMap_Stg10, 0x19C9098, 3);
DataArray(___stcClip*, pClipMap_Stg12, 0x17C34F8, 3);
//pClipMap_Chaos00
DataArray(___stcClip*, pClipMap_Chaos02, 0x1120670, 1);
//pClipMap_Chaos04
DataArray(___stcClip*, pClipMap_Chaos06, 0x11EF158, 2);
DataArray(___stcClip*, pClipMap_Chaos07, 0x1420EA0, 2);
DataArray(___stcClip*, pClipMap_Egm01, 0x1556B6C, 1);
DataArray(___stcClip*, pClipMap_Egm02, 0x15E882C, 1);
//pClipMap_Egm03
DataArray(___stcClip*, pClipMap_Robo, 0x16B4DE8, 1);
//pClipMap_E101
DataArray(___stcClip*, pClipMap_E101r, 0x1522628, 1);
DataArray(___stcClip*, pClipMap_Adv00, 0x2AA3E60, 6);
DataArray(___stcClip*, pClipMap_Adv01AB, 0x10F2494, 7);
DataArray(___stcClip*, pClipMap_Adv01C, 0x1100D68, 6);
DataArray(___stcClip*, pClipMap_Adv02, 0x11035E8, 4);
DataArray(___stcClip*, pClipMap_Adv03, 0x111E630, 3);
DataArray(___stcClip*, pClipMap_MiniCart, 0x9BF58C, 3);
DataArray(___stcClip*, pClipMap_Sht, 0x27D6DF8, 2);
DataArray(___stcClip*, pClipMap_Sand, 0x173BBAC, 1);

// Sky draw distance
//pClipSky_Stg00
DataArray(___stcClip*, pClipSky_Stg01, 0xE99E78, 3);
DataArray(___stcClip*, pClipSky_Stg02, 0xAFEABC, 3);
DataArray(___stcClip*, pClipSky_Stg03, 0x26B3468, 3);
DataArray(___stcClip*, pClipSky_Stg04, 0x24CA580, 3);
DataArray(___stcClip*, pClipSky_Stg05, 0x224079C, 3);
DataArray(___stcClip*, pClipSky_Stg06, 0x203A130, 3);
DataArray(___stcClip*, pClipSky_Stg07, 0x1E79B48, 3);
DataArray(___stcClip*, pClipSky_Stg08, 0xC67F70, 4);
//pClipSky_Stg09
DataArray(___stcClip*, pClipSky_Stg10, 0x19C908C, 3);
DataArray(___stcClip*, pClipSky_Stg12, 0x17C34EC, 3);
//pClipSky_Chaos00
DataArray(___stcClip*, pClipSky_Chaos02, 0x112066C, 1);
//pClipSky_Chaos04
//pClipSky_Chaos06
//pClipSky_Chaos07
//pClipSky_Egm01
DataArray(___stcClip*, pClipSky_Egm02, 0x15E8828, 1);
//pClipSky_Egm03
//pClipSky_Robo
//pClipSky_E101
//pClipSky_E101r
DataArray(___stcClip*, pClipSky_Adv00, 0x2AA3E48, 6);
DataArray(___stcClip*, pClipSky_Adv01AB, 0x10F2478, 7);
DataArray(___stcClip*, pClipSky_Adv01C, 0x1100D50, 6);
//pClipSky_Adv02
//pClipSky_Adv03
DataArray(___stcClip*, pClipSky_MiniCart, 0x9BF574, 3);
//pClipSky_Sht
DataArray(___stcClip*, pClipSky_Sand, 0x173BB44, 1);

// Skybox scale
//pScale_Stg00
DataArray(NJS_POINT3*, pScale_Stg01, 0xE99E6C, 3);
DataArray(NJS_POINT3*, pScale_Stg02, 0xAFEAB0, 3);
DataArray(NJS_POINT3*, pScale_Stg03, 0x26B345C, 3);
DataArray(NJS_POINT3*, pScale_Stg04, 0x24CA574, 3);
DataArray(NJS_POINT3*, pScale_Stg05, 0x2240790, 3);
DataArray(NJS_POINT3*, pScale_Stg06, 0x203A124, 3);
DataArray(NJS_POINT3*, pScale_Stg07, 0x1E79B3C, 3);
DataArray(NJS_POINT3*, pScale_Stg08, 0xC67F60, 4);
//pScale_Stg09
DataArray(NJS_POINT3*, pScale_Stg10, 0x19C9080, 3);
DataArray(NJS_POINT3*, pScale_Stg12, 0x17C34E0, 3);
//pScale_Chaos00
DataArray(NJS_POINT3*, pScale_Chaos02, 0x1120668, 1);
DataArray(NJS_POINT3*, pScale_Chaos06, 0x11EF148, 2);
//pScale_Chaos07
//pScale_Egm01
DataArray(NJS_POINT3*, pScale_Egm02, 0x15E8824, 1);
//pScale_Egm03
//pScale_Robo
//pScale_E101
//pScale_E101r
DataArray(NJS_POINT3*, pScale_Adv00, 0x2AA3E30, 6);
DataArray(NJS_POINT3*, pScale_Adv01AB, 0x10F245C, 7);
DataArray(NJS_POINT3*, pScale_Adv01C, 0x1100D38, 6);
//pScale_Adv02
//pScale_Adv03
DataArray(NJS_POINT3*, pScale_MiniCart, 0x9BF55C, 3);
DataArray(NJS_POINT3*, pScale_Sht, 0x27D6DE8, 2);
//pScale_Sand

// Texlists
DataPointer(NJS_TEXLIST, texlist_seates, 0x10C0508); // BEACH_SEA texlist
DataPointer(NJS_TEXLIST, texlist_beach01, 0xF812AC); // BEACH01 texlist
DataPointer(NJS_TEXLIST, texlist_beach02, 0xEF553C); // BEACH02 texlist
DataPointer(NJS_TEXLIST, texlist_beach03, 0xE9A4CC); // BEACH03 texlist

// PVM entries
DataArray(TEX_PVMTABLE, ListofPVMSS00, 0x0090F184, 1); // Station Square City Hall PVM list
DataArray(TEX_PVMTABLE, ListofPVMSS01, 0x0090F16C, 3); // Station Square Station/Casino area PVM list
DataArray(TEX_PVMTABLE, ListofPVMSS02, 0x0090F19C, 1); // Station Square Sewers PVM list
DataArray(TEX_PVMTABLE, ListofPVMSS03, 0x0090F15C, 2); // Station Square Main/Station area PVM list
DataArray(TEX_PVMTABLE, ListofPVMSS04, 0x0090F12C, 1); // Station Square Hotel area PVM list
DataArray(TEX_PVMTABLE, ListofPVMSS05, 0x0090F228, 2); // Station Square Twinkle Circuit area PVM list

DataArray(TEX_PVMTABLE, ListofPVMEC00, 0x010F34F8, 1); // Egg Carrier (Untransformed) PVM list
DataArray(TEX_PVMTABLE, ListofPVMEC01, 0x010F3508, 1); // Egg Carrier (Transformed A) PVM list
DataArray(TEX_PVMTABLE, ListofPVMEC02, 0x010F34E0, 1); // Egg Carrier (Transformed B) PVM list
DataArray(TEX_PVMTABLE, ListofPVMEC03, 0x010F34D8, 1); // Egg Carrier Captain's Room PVM list
DataArray(TEX_PVMTABLE, ListofPVMEC04, 0x010F3500, 1); // Egg Carrier Private Room PVM list
DataArray(TEX_PVMTABLE, ListofPVMEC05, 0x010F34E8, 2); // Egg Carrier Pool PVM list

DataArray(TEX_PVMTABLE, ListofPVMEC30, 0x01101370, 1); // Egg Carrier Jet Booster Room PVM list
DataArray(TEX_PVMTABLE, ListofPVMEC31, 0x01101380, 1); // Egg Carrier Main Hall PVM list
DataArray(TEX_PVMTABLE, ListofPVMEC32, 0x01101360, 1); // Egg Carrier Hedgehog Hammer Room PVM list
DataArray(TEX_PVMTABLE, ListofPVMEC33, 0x01101378, 1); // Egg Carrier Prison PVM list
DataArray(TEX_PVMTABLE, ListofPVMEC34, 0x01101388, 2); // Egg Carrier Laser Blaster Room PVM list
DataArray(TEX_PVMTABLE, ListofPVMEC35, 0x01101398, 1); // Egg Carrier Chao Transporter Room PVM list

DataArray(TEX_PVMTABLE, ListofPVMMR00, 0x0090F104, 3); // Mystic Ruins Station area PVM list
DataArray(TEX_PVMTABLE, ListofPVMMR01, 0x0090F11C, 2); // Mystic Ruins Angle Island area PVM list
DataArray(TEX_PVMTABLE, ListofPVMMR02, 0x0090F208, 4); // Mystic Ruins Jungle area PVM list
DataArray(TEX_PVMTABLE, ListofPVMMR03, 0x0090F144, 1); // Mystic Ruins Final Egg Base PVM list

DataArray(TEX_PVMTABLE, ListofPVMPast00, 0x0090F18C, 2); // Past Echidna City PVM list
DataArray(TEX_PVMTABLE, ListofPVMPast01, 0x0090F134, 2); // Past Master Emerald Altar PVM list
DataArray(TEX_PVMTABLE, ListofPVMPast02, 0x0090F14C, 2); // Past Altar on Fire PVM list

// LandTables
DataPointer(_OBJ_LANDTABLE, objLandTableegm1, 0x1570B1C);
DataPointer(_OBJ_LANDTABLE, objLandTableegm2, 0x15EC454);
DataPointer(_OBJ_LANDTABLE, objLandTableegm3, 0x165E990);
DataPointer(_OBJ_LANDTABLE, objLandTableRobo, 0x16B4D38);
DataPointer(_OBJ_LANDTABLE, objLandTable1600, 0x112136C);
DataPointer(_OBJ_LANDTABLE, objLandTable1700, 0x1190930);
DataPointer(_OBJ_LANDTABLE, objLandTable1800, 0x11EDE38);
DataPointer(_OBJ_LANDTABLE, objLandTable1801, 0x11ED6F0);
DataPointer(_OBJ_LANDTABLE, objLandTable1900, 0x142478C);
DataPointer(_OBJ_LANDTABLE, objLandTable2300, 0x14FCEE8);
DataPointer(_OBJ_LANDTABLE, objLandTable2400, 0x1522578);
DataPointer(_OBJ_LANDTABLE, objLandTableChaoBlackMarket, 0x33CAC58);
DataPointer(_OBJ_LANDTABLE, objLandTableChaoEntrance, 0x3423700);
DataPointer(_OBJ_LANDTABLE, objLandTableGarden00, 0x340E738);
DataPointer(_OBJ_LANDTABLE, objLandTableGarden01, 0x3405E54);
DataPointer(_OBJ_LANDTABLE, objLandTableChaoRace, 0x3424C58);
DataPointer(_OBJ_LANDTABLE, objLandTableMCart00, 0x9C99A4);
DataPointer(_OBJ_LANDTABLE, objLandTableMCart01, 0x9C8170);
DataPointer(_OBJ_LANDTABLE, objLandTableMCart02, 0x9C6C30);
DataPointer(_OBJ_LANDTABLE, objLandTableMCart03, 0x9C585C);
DataPointer(_OBJ_LANDTABLE, objLandTableMCart04, 0x9C453C);
DataPointer(_OBJ_LANDTABLE, objLandTableMCart05, 0x9C3534);
DataPointer(_OBJ_LANDTABLE, objLandTableSBoard00, 0x173EB64);
DataPointer(_OBJ_LANDTABLE, objLandTablePractice, 0x27C7BCC);
DataPointer(_OBJ_LANDTABLE, objLandTable0100, 0xE99CB8);
DataPointer(_OBJ_LANDTABLE, objLandTable0101, 0x1039E9C);
DataPointer(_OBJ_LANDTABLE, objLandTable0102, 0x10386B4);
DataPointer(_OBJ_LANDTABLE, objLandTable0200, 0xC051E0);
DataPointer(_OBJ_LANDTABLE, objLandTable0201, 0xC046C0);
DataPointer(_OBJ_LANDTABLE, objLandTable0202, 0xC0433C);
DataPointer(_OBJ_LANDTABLE, objLandTable0300, 0x26B975C);
DataPointer(_OBJ_LANDTABLE, objLandTable0301, 0x26B867C);
DataPointer(_OBJ_LANDTABLE, objLandTable0302, 0x26B6B34);
DataPointer(_OBJ_LANDTABLE, objLandTable0400, 0x26B1E98);
DataPointer(_OBJ_LANDTABLE, objLandTable0401, 0x26ADBC8);
DataPointer(_OBJ_LANDTABLE, objLandTable0402, 0x26ACF40);
DataPointer(_OBJ_LANDTABLE, objLandTable0500, 0x22405E0);
DataPointer(_OBJ_LANDTABLE, objLandTable0501, 0x24C8B58);
DataPointer(_OBJ_LANDTABLE, objLandTable0502, 0x24C6F14);
DataPointer(_OBJ_LANDTABLE, objLandTable0600, 0x2239ADC);
DataPointer(_OBJ_LANDTABLE, objLandTable0601, 0x22369A0);
DataPointer(_OBJ_LANDTABLE, objLandTable0602, 0x2234800);
DataPointer(_OBJ_LANDTABLE, objLandTable0700, 0x2038B60);
DataPointer(_OBJ_LANDTABLE, objLandTable0701, 0x2037C9C);
DataPointer(_OBJ_LANDTABLE, objLandTable0702, 0x2034D14);
DataPointer(_OBJ_LANDTABLE, objLandTable0800, 0xE41D2C);
DataPointer(_OBJ_LANDTABLE, objLandTable0801, 0xE414BC);
DataPointer(_OBJ_LANDTABLE, objLandTable0802, 0xE409C4);
DataPointer(_OBJ_LANDTABLE, objLandTable0803, 0xE3E024);
DataPointer(_OBJ_LANDTABLE, objLandTable0900, 0x1D8A9D0);
DataPointer(_OBJ_LANDTABLE, objLandTable0901, 0x1D887EC);
DataPointer(_OBJ_LANDTABLE, objLandTable0902, 0x1D86A1C);
DataPointer(_OBJ_LANDTABLE, objLandTable0903, 0x1D85C08);
DataPointer(_OBJ_LANDTABLE, objLandTable1001, 0x1A600D0);
DataPointer(_OBJ_LANDTABLE, objLandTable1002, 0x19C8ED0);
DataPointer(_OBJ_LANDTABLE, objLandTable1003, 0x1A5CFA8);
DataPointer(_OBJ_LANDTABLE, objLandTable1201, 0x17D09C0);
DataPointer(_OBJ_LANDTABLE, objLandTable1202, 0x17CF288);
DataPointer(_OBJ_LANDTABLE, objLandTable1203, 0x17C9B48);

// LandTable pointer arrays in the DLLs
DataPointer(_OBJ_LANDTABLE**, ___LANDTABLEBOSSCHAOS0, 0x3AB9168);
DataPointer(_OBJ_LANDTABLE**, ___LANDTABLESS, 0x3AAD0FC);
DataPointer(_OBJ_LANDTABLE**, ___LANDTABLEEC0, 0x38F6E78);
DataPointer(_OBJ_LANDTABLE**, ___LANDTABLEEC3, 0x38F6E88);
DataPointer(_OBJ_LANDTABLE**, ___LANDTABLEMR, 0x3AAD130);
DataPointer(_OBJ_LANDTABLE**, ___LANDTABLEPAST, 0x3AAD110);

//motion
DataPointer(MOTIONDATA_INFO, nj_motion_data_info_, 0x3AB9910);

//chaos
DataArray(PL_JOIN_VERTEX, chaos0_jv_list, 0x3C63930, 18);
DataArray(PL_ACTION, chaos0_action, 0x3C63AE0, 18);
DataArray(PL_JOIN_VERTEX, chaos2_jv_list, 0x1120830, 22);
DataArray(PL_ACTION, chaos2_action, 0x117C778, 14);
DataArray(PL_JOIN_VERTEX, chaos4_jv_list, 0x118FC48, 15);
DataArray(PL_ACTION, chaos4_action, 0x11C1C58, 9);
DataArray(PL_JOIN_VERTEX, chaos6_jv_list, 0x11EF3C0, 28);
DataPointer(task*, ShakeChaos0_Task, 0x03C63C14);
DataPointer(BUBBLE_DATA, chaos0_bubble, 0x112014C);
DataPointer(BUBBLE_DATA, chaos2_bubble, 0x118EE58);
DataPointer(BUBBLE_DATA, chaos4_bubble, 0x11ECD08);
DataPointer(MORPHWK*, morph_tp, 0x3C63C10);
DataPointer(MORPHWK*, morph_tp_0, 0x3C63EB8);
DataPointer(MORPHWK*, morph_tp_1, 0x3C69A30);
DataPointer(CHAOS_PARAM*, chaosparam, 0x03C5A7E8);
DataPointer(char, chaos_id, 0x3C5A7E2);
#endif /* SADXMODLOADER_SADXVARSNEW_H */