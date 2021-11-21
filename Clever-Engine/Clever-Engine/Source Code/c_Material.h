#pragma once

#include "Globals.h"
#include "Component.h"

class TextureData;

class c_Material : public Component
{
public:
	c_Material(GameObject* parent, ComponentData* data);
	~c_Material();

	bool Enable();
	bool Update();
	bool Disable();

	const char* getPath();
	const uint getTextureID();

	void setPath(const char* path);
	void setTextureID(uint id);

	void changeTextureData(TextureData* data);

private:
	TextureData* textureData = nullptr;
	
};
