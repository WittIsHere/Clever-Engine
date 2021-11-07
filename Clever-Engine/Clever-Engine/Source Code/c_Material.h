#pragma once

#include "Globals.h"
#include "Component.h"

struct textureData;

class c_Material : public Component
{
public:
	c_Material();
	c_Material(textureData* data);
	~c_Material();

	bool Enable();
	bool Update();
	bool Disable();

public:
	textureData* resourceTexture;
};
