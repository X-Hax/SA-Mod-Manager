/**
 * SADX Mod Loader.
 * SADX enums based on symbols.
 */

#ifndef SADXMODLOADER_SADXENUMSNEW_H
#define SADXMODLOADER_SADXENUMSNEW_H

#include <stdint.h>

// SADX Enums

#define makemask(enumname,membername) enumname##_##membername = (1 << enumname##Bits_##membername)
#define makemasks(enumname,membername) enumname##s_##membername = (1 << enumname##Bits_##membername)
#define levelact(level, act) (((level) << 8) | (act))

#endif /* SADXMODLOADER_SADXENUMSNEW_H */
