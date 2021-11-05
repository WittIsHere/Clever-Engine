#pragma once

#include "Globals.h"

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


public:
	ComponentType type = ComponentType::NONE;
	bool isActive = true;

private:
	GameObject* parent = nullptr;
};