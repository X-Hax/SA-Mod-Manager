#pragma once
#include "SADXModLoader.h"

class GameObject
{
private:
	static void CallMain(ObjectMaster* obj);
	static void CallDisplay(ObjectMaster* obj);
	static void CallDelete(ObjectMaster* obj);
	bool initFromObj;
protected:
	ObjectMaster* objData;
	GameObject(LoadObj flags, int index);
	explicit GameObject(ObjectMaster* obj);
	virtual void Delete();
public:
	explicit GameObject(int index);
	~GameObject();
	virtual void Main() = 0;
	virtual void Display();

	template <typename T>
	static void Load(ObjectMaster* obj)
	{
		T* t = new T(obj);
		t->Main();
	}

	SETDataUnion GetSETData() const;
	// MSVC-specific property emulation.
#ifdef _MSC_VER
	__declspec(property(get = GetSETData)) SETDataUnion SETData;
#endif
};

class GameEntity : GameObject
{
protected:
	EntityData1* GetData() const;
public:
	explicit GameEntity(int index);
	char GetAction() const;
	void SetAction(char action) const;
	short GetInvulnerableTime() const;
	void SetInvulnerableTime(short time) const;
	char GetCharIndex() const;
	void SetCharIndex(char index) const;
	Rotation3& GetRotation() const;
	void SetRotation(Rotation3& rotation) const;
	NJS_VECTOR& GetPosition() const;
	void SetPosition(NJS_VECTOR& position) const;
	NJS_VECTOR& GetScale() const;
	void SetScale(NJS_VECTOR& scale) const;
	CollisionInfo* GetCollisionInfo() const;
	// MSVC-specific property emulation.
#ifdef _MSC_VER
	__declspec(property(get = GetAction, put = SetAction)) char Action;
	__declspec(property(get = GetInvulnerableTime, put = SetInvulnerableTime)) short InvulnerableTime;
	__declspec(property(get = GetCharIndex, put = SetCharIndex)) char CharIndex;
	__declspec(property(get = GetRotation, put = SetRotation)) Rotation3& Rotation;
	__declspec(property(get = GetPosition, put = SetPosition)) NJS_VECTOR& Position;
	__declspec(property(get = GetScale, put = SetScale)) NJS_VECTOR& Scale;
	__declspec(property(get = GetCollisionInfo)) CollisionInfo* CollisionInfo;
#endif
};
