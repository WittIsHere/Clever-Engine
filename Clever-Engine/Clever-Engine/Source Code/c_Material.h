#pragma once

#include "Globals.h"
#include "Component.h"

struct textureData;

class c_Material : public Component
{
public:
	c_Material(GameObject* parent, ComponentData* data);
	~c_Material();

	bool Enable();
	bool Update();
	bool Disable();
};
