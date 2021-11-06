#pragma once

#include "Globals.h"
#include "Component.h"


class c_Material : public Component
{
public:
	c_Material();
	~c_Material();

	bool Enable();
	bool Update();
	bool Disable();

public:
	const char* path;
	uint textureID;
};
