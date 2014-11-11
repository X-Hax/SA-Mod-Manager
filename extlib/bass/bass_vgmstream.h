#ifndef _BASS_VGMSTREAM_H_
#define _BASS_VGMSTREAM_H_


#ifdef BASS_VGMSTREAM_EXPORTS
#define BASS_VGMSTREAM_API __declspec(dllexport)
#else
#define BASS_VGMSTREAM_API __declspec(dllimport)
#endif

#include "bass.h"


#ifdef __cplusplus
extern "C"
{
#endif

BASS_VGMSTREAM_API HSTREAM BASS_VGMSTREAM_StreamCreate(const char* file, DWORD flags);

#ifdef __cplusplus
}
#endif


#endif