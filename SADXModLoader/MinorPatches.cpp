#include "stdafx.h"

//fix a crash on Chaos 2 if you restart after defeating him. (Knuckles only)

static FunctionHook<void, pathtag*, Sint8, int> SetLocalPathCamera_t((intptr_t)0x469300);

void __cdecl SetLocalPathCamera_r(pathtag* path, Sint8 mode, int timer)
{
	if (!camera_twp)
	{
		return;
	}

	return SetLocalPathCamera_t.Original(path, mode, timer);
}

//fix wrong music on Chaos 2 if you restart after defeating him
void LoadDelayedSoundBGM_r(int index)
{
	if (!IsIngame())
		return;

	Load_DelayedSound_BGM(index);
}

void MinorPatches_Init()
{
	SetLocalPathCamera_t.Hook(SetLocalPathCamera_r);
	WriteCall((void*)0x415631, LoadDelayedSoundBGM_r);
}