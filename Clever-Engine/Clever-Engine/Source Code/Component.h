#pragma once

#include "Globals.h"
#include "ComponentData.h"

class GameObject;

enum ComponentType
{
	NONE,
	MESH,
	TRANSFORM,
	MATERIAL
};

class Component
{
public:
	Component();
	~Component();

	virtual bool Enable();
	virtual bool Update();
	virtual bool Disable();

	//addcomponent()

public:
	ComponentType type;
	ComponentData data;

	bool isActive = true;

private:
	GameObject* parent = nullptr;
};