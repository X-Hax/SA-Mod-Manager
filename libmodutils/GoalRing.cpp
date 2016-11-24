// Example object class.

#include "stdafx.h"
#include "SADXModLoader.h"
#include "GameObject.h"
extern NJS_OBJECT object_GoalRing;
extern NJS_OBJECT object_GoalText;
extern NJS_TEXLIST GoalRingTextures;
#define GoalRingSize 34

FunctionPointer(void, sub_408530, (NJS_OBJECT *), 0x408530);

class GoalRing : public GameEntity
{
	void Main();
	void Display();
};

void GoalRing::Display()
{
	njSetTexture(&GoalRingTextures);
	njPushMatrix(nullptr);
	njTranslateV(nullptr, &Position);
	int yrot = Rotation.y;
	if (yrot)
		njRotateY(nullptr, yrot);
	if (FrameCounter % 40 >= 25)
		object_GoalText.evalflags |= NJD_EVAL_HIDE;
	else
		object_GoalText.evalflags &= ~NJD_EVAL_HIDE;
	sub_408530(&object_GoalRing);
	njPopMatrix(1);
}

void GoalRing::Main()
{
	switch (Action)
	{
	case 0:
		Rotation.y = 0;
		Action = 1;
		break;
	case 1:
		if (GetCurrentCharacterID() == Characters_Tails)
		{
			switch (IsPlayerInsideSphere(&Position, GoalRingSize))
			{
			case 0:
				break;
			case 1:
				SetTailsRaceVictory();
				LoadLevelResults();
				Action = 2;
				break;
			default:
				SetOpponentRaceVictory();
				LoadLevelResults();
				Action = 2;
				break;
			}
		}
		else if ( IsPlayerInsideSphere(&Position, GoalRingSize) == 1 )
		{
			LoadLevelResults();
			Action = 2;
		}
		Rotation.y += 0x180;
		Display();
		break;
	}
}
