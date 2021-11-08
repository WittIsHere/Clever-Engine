#pragma once
#include "ComponentData.h"

class TextureData : public ComponentData
{
public:
	const char* path = nullptr;
	uint textureID;
};