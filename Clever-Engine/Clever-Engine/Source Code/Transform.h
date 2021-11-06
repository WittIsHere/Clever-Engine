#pragma once

#include "Globals.h"
#include "Component.h"


class TransformComponent : public Component
{
public:
	TransformComponent();
	~TransformComponent();

	bool Enable();
	bool Update();
	bool Disable();

public:


private:
	float x, y, z;
};