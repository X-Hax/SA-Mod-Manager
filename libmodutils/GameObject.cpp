#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject(LoadObj flags, int index)
{
	objData = LoadObject(flags, index, CallMain);
	if (objData)
	{
		objData->DisplaySub = CallDisplay;
		objData->DeleteSub = CallDelete;
		objData->Data2 = this;
	}
	initFromObj = false;
}

GameObject::GameObject(int index) : GameObject((LoadObj)0, index){}

GameObject::GameObject(ObjectMaster *obj)
{
	objData = obj;
	objData->MainSub = CallMain;
	objData->DisplaySub = CallDisplay;
	objData->DeleteSub = CallDelete;
	objData->Data2 = this;
	initFromObj = true;
}

void GameObject::CallMain(ObjectMaster *obj)
{
	((GameObject *)obj->Data2)->Main();
}

void GameObject::CallDisplay(ObjectMaster *obj)
{
	((GameObject *)obj->Data2)->Display();
}

void GameObject::CallDelete(ObjectMaster *obj)
{
	GameObject *cgo = (GameObject *)obj->Data2;
	cgo->Delete();
	obj->Data2 = nullptr;
	cgo->objData = nullptr;
	if (cgo->initFromObj)
		delete cgo;
}

void GameObject::Display(){}

void GameObject::Delete(){}

SETDataUnion GameObject::GetSETData(){ return objData->SETData; }

GameObject::~GameObject()
{
	if (objData)
	{
		Delete();
		objData->Data2 = nullptr;
		objData->DeleteSub = nullptr;
		DeleteObject_(objData);
	}
}

GameEntity::GameEntity(int index) :GameObject(LoadObj_Data1, index){}

EntityData1 *GameEntity::GetData() { return objData->Data1; }

char GameEntity::GetAction() { return GetData()->Action; }

void GameEntity::SetAction(char action) { GetData()->Action = action; }

short GameEntity::GetInvulnerableTime() { return GetData()->InvulnerableTime; }

void GameEntity::SetInvulnerableTime(short time) { GetData()->InvulnerableTime = time; }

char GameEntity::GetCharIndex() { return GetData()->CharIndex; }

void GameEntity::SetCharIndex(char index) { GetData()->CharIndex = index; }

Rotation3 &GameEntity::GetRotation() { return GetData()->Rotation; }

void GameEntity::SetRotation(Rotation3 &rotation) { GetData()->Rotation = rotation; }

NJS_VECTOR &GameEntity::GetPosition() { return GetData()->Position; }

void GameEntity::SetPosition(NJS_VECTOR &position) { GetData()->Position = position; }

NJS_VECTOR &GameEntity::GetScale() { return GetData()->Scale; }

void GameEntity::SetScale(NJS_VECTOR &scale) { GetData()->Scale = scale; }

CollisionInfo *GameEntity::GetCollisionInfo() { return GetData()->CollisionInfo; }