#pragma once
#include "ComponentData.h"
#include <string>

class TextureData : public ComponentData
{
public:
	std::string path;
	uint textureID;
};