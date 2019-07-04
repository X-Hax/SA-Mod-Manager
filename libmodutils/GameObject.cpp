#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject(LoadObj flags, int index)
{
	objData = LoadObject(flags, index, CallMain);
	if (objData)
	{
		objData->DisplaySub = CallDisplay;
		objData->DeleteSub  = CallDelete;
		objData->Data2      = this;
	}
	initFromObj = false;
}

GameObject::GameObject(int index)
	: GameObject((LoadObj)0, index)
{
}

GameObject::GameObject(ObjectMaster* obj)
{
	objData             = obj;
	objData->MainSub    = CallMain;
	objData->DisplaySub = CallDisplay;
	objData->DeleteSub  = CallDelete;
	objData->Data2      = this;
	initFromObj         = true;
}

void GameObject::CallMain(ObjectMaster* obj)
{
	((GameObject*)obj->Data2)->Main();
}

void GameObject::CallDisplay(ObjectMaster* obj)
{
	((GameObject*)obj->Data2)->Display();
}

void GameObject::CallDelete(ObjectMaster* obj)
{
	auto cgo = (GameObject*)obj->Data2;
	cgo->Delete();
	obj->Data2   = nullptr;
	cgo->objData = nullptr;
	if (cgo->initFromObj)
		delete cgo;
}

void GameObject::Display()
{
}

void GameObject::Delete()
{
}

SETDataUnion GameObject::GetSETData() const { return objData->SETData; }

GameObject::~GameObject()
{
	if (objData)
	{
		Delete();
		objData->Data2     = nullptr;
		objData->DeleteSub = nullptr;
		DeleteObject_(objData);
	}
}

GameEntity::GameEntity(int index)
	: GameObject(LoadObj_Data1, index)
{
}

EntityData1* GameEntity::GetData() const { return objData->Data1; }

char GameEntity::GetAction() const { return GetData()->Action; }

void GameEntity::SetAction(char action) const { GetData()->Action = action; }

short GameEntity::GetInvulnerableTime() const { return GetData()->InvulnerableTime; }

void GameEntity::SetInvulnerableTime(short time) const { GetData()->InvulnerableTime = time; }

char GameEntity::GetCharIndex() const { return GetData()->CharIndex; }

void GameEntity::SetCharIndex(char index) const { GetData()->CharIndex = index; }

Rotation3& GameEntity::GetRotation() const { return GetData()->Rotation; }

void GameEntity::SetRotation(Rotation3& rotation) const { GetData()->Rotation = rotation; }

NJS_VECTOR& GameEntity::GetPosition() const { return GetData()->Position; }

void GameEntity::SetPosition(NJS_VECTOR& position) const { GetData()->Position = position; }

NJS_VECTOR& GameEntity::GetScale() const { return GetData()->Scale; }

void GameEntity::SetScale(NJS_VECTOR& scale) const { GetData()->Scale = scale; }

CollisionInfo* GameEntity::GetCollisionInfo() const { return GetData()->CollisionInfo; }
