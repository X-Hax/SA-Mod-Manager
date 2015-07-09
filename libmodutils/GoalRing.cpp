// GoalRing.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SADXModLoader.h"
extern NJS_OBJECT object_GoalRing;
extern NJS_OBJECT object_GoalText;
extern NJS_TEXLIST GoalRingTextures;
#define GoalRingSize 34

Uint32 GoalTextFlags;

FunctionPointer(void, sub_408530, (NJS_OBJECT *), 0x408530);
void __cdecl GoalRing_Display(ObjectMaster *obj)
{
	SetTextures(&GoalRingTextures);
	PushMatrix2(0);
	MatrixTranslate2(0, &obj->Data1->Position);
	int yrot = obj->Data1->Rotation.y;
	if (yrot)
		MatrixRotateY(0, yrot);
	if (FrameCounter % 40 >= 25)
		object_GoalText.evalflags |= NJD_EVAL_HIDE;
	else
		object_GoalText.evalflags &= ~NJD_EVAL_HIDE;
	sub_408530(&object_GoalRing);
	PopMatrices(1);
}

void __cdecl GoalRing_Main(ObjectMaster *obj)
{
	switch (obj->Data1->Action)
	{
	case 0:
		obj->DisplaySub = GoalRing_Display;
		obj->Data1->Rotation.y = 0;
		obj->Data1->Action = 1;
		GoalTextFlags = object_GoalText.evalflags;
		break;
	case 1:
		if (GetCurrentCharacterID() == Characters_Tails)
		{
			switch (IsPlayerInsideSphere(&obj->Data1->Position, GoalRingSize))
			{
			case 0:
				break;
			case 1:
				SetTailsRaceVictory();
				LoadLevelResults();
				obj->Data1->Action = 2;
				break;
			default:
				SetOpponentRaceVictory();
				LoadLevelResults();
				obj->Data1->Action = 2;
				break;
			}
		}
		else if ( IsPlayerInsideSphere(&obj->Data1->Position, GoalRingSize) == 1 )
		{
			LoadLevelResults();
			obj->Data1->Action = 2;
		}
		obj->Data1->Rotation.y += 0x180;
		GoalRing_Display(obj);
		break;
	}
}
