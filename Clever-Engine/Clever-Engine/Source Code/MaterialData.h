#pragma once
#include "ComponentData.h"

class TextureData : public ComponentData
{
	//create constructors
public:
	const char* path = nullptr;
	uint textureID;
};