#pragma once

#include "Globals.h"
#include "Component.h"


class c_Transform : public Component
{
public:
	c_Transform();
	~c_Transform();

	bool Enable();
	bool Update();
	bool Disable();

public:


private:
	float x, y, z;
};