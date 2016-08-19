#include "stdafx.h"
#include "GameObject.h"


CGameObject::CGameObject(LoadObj flags, int index)
{
	objData = LoadObject(flags, index, CallMain);
	if (!objData)
	{
		objData->DisplaySub = CallDisplay;
		objData->DeleteSub = CallDelete;
		objData->Data2 = this;
	}
}

CGameObject::CGameObject(int index) : CGameObject((LoadObj)0, index){}

CGameObject::CGameObject(ObjectMaster *obj)
{
	objData->MainSub = CallMain;
	objData->DisplaySub = CallDisplay;
	objData->DeleteSub = CallDelete;
	objData->Data2 = this;
	initFromObj = true;
}

template <typename T>
void CGameObject::Load(ObjectMaster *obj)
{
	new T(obj);
	obj->MainSub();
}

void CGameObject::CallMain(ObjectMaster *obj)
{
	((CGameObject *)obj->Data2)->Main();
}

void CGameObject::CallDisplay(ObjectMaster *obj)
{
	((CGameObject *)obj->Data2)->Display();
}

void CGameObject::CallDelete(ObjectMaster *obj)
{
	CGameObject *cgo = (CGameObject *)obj->Data2;
	cgo->Delete();
	cgo->objData = nullptr;
	if (cgo->initFromObj)
		delete cgo;
}

void CGameObject::Display(){}

void CGameObject::Delete(){}

CGameObject::~CGameObject()
{
	if (objData)
	{
		objData->Data2 = nullptr;
		DeleteObject_(objData);
	}
}

CGameEntity::CGameEntity(int index) :CGameObject(LoadObj_Data1, index){}

EntityData1 *CGameEntity::GetData() { return objData->Data1; }

char CGameEntity::GetAction() { return GetData()->Action; }

void CGameEntity::SetAction(char action) { GetData()->Action = action; }

short CGameEntity::GetInvulnerableTime() { return GetData()->InvulnerableTime; }

void CGameEntity::SetInvulnerableTime(short time) { GetData()->InvulnerableTime = time; }

char CGameEntity::GetCharIndex() { return GetData()->CharIndex; }

void CGameEntity::SetCharIndex(char index) { GetData()->CharIndex = index; }

Rotation3 &CGameEntity::GetRotation() { return GetData()->Rotation; }

void CGameEntity::SetRotation(Rotation3 &rotation) { GetData()->Rotation = rotation; }

NJS_VECTOR &CGameEntity::GetPosition() { return GetData()->Position; }

void CGameEntity::SetPosition(NJS_VECTOR &position) { GetData()->Position = position; }

NJS_VECTOR &CGameEntity::GetScale() { return GetData()->Scale; }

void CGameEntity::SetScale(NJS_VECTOR &scale) { GetData()->Scale = scale; }

CollisionInfo *CGameEntity::GetCollisionInfo() { return GetData()->CollisionInfo; }