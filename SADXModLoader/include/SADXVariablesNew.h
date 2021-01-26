/**
 * SADX Mod Loader.
 * SADX variables based on symbols.
 */

#ifndef SADXMODLOADER_SADXVARSNEW_H
#define SADXMODLOADER_SADXVARSNEW_H

#include "MemAccess.h"
#include "d3d8types.h"
#include "SADXStructsNew.h"

DataArray(LE_PALIGHT_ENV, le_plyrPal, 0x903E88, 255); // Character lights
DataArray(LE_LIGHT_ENV, le_stgPL, 0x900918, 30); // Unused stage lights array
DataArray(LE_LIGHT_ENV, le_stg, 0x900E88, 255); // Stage lights
DataPointer(LE_LIGHT_ENV, le_env_dflt, 0x9008B4); // Default stage light
DataPointer(LE_LIGHT_ENV, le_ctrl_dflt, 0x9008E4); // Backup default stage light

#endif /* SADXMODLOADER_SADXVARSNEW_H */
