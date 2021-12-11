#pragma once

#include "Globals.h"
#include "ComponentData.h"

class GameObject;
class ComponentData;
class ParsonNode;

class Component
{
public:
	Component(GameObject* parent, ComponentData* data);
	~Component();

	virtual bool Enable();
	virtual bool Update();
	virtual bool Disable();

	virtual bool SaveState(ParsonNode& root) const;
	virtual bool LoadState(ParsonNode& root);

	const char* getNameFromType();
	GameObject* GetOwner();

public:
	COMPONENT_TYPE type;
	bool isActive = true;

public:
	ComponentData* data;
	GameObject* owner = nullptr;
};