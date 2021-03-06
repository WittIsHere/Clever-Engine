#pragma once

#include "Globals.h"
#include "ComponentData.h"

class GameObject;
class ComponentData;
class ParsonNode;

class Component
{
public:
	Component(GameObject* parent, COMPONENT_TYPE data);
	~Component();

	virtual bool Enable();
	virtual bool Update(float dt);
	virtual void Draw();
	virtual bool Disable();

	virtual bool SaveState(ParsonNode& root) const;
	virtual bool LoadState(ParsonNode& root);

	const char* getNameFromType();
	GameObject* GetOwner();

public:
	COMPONENT_TYPE type;
	bool isActive = true;
	bool isEmpty = true;

protected:
	GameObject* owner = nullptr;
};