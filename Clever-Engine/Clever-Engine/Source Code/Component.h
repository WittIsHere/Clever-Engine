#pragma once

#include "Globals.h"
#include "ComponentData.h"

class GameObject;
class ComponentData;

class Component
{
public:
	Component(GameObject* parent, ComponentData* data);
	~Component();

	virtual bool Enable();
	virtual bool Update();
	virtual bool Disable();

	const char* getNameFromType();
	GameObject* GetOwner();

public:
	COMPONENT_TYPE type;
	bool isActive = true;

private:
	ComponentData* data;
	GameObject* owner = nullptr;
};