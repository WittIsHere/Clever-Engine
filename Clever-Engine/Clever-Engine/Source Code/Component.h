#pragma once

#include "Globals.h"
#include "ComponentData.h"

class GameObject;

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
	COMPONENT_TYPE type;
	ComponentData data;

	bool isActive = true;

private:
	GameObject* parent = nullptr;
};