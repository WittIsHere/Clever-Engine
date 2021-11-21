#pragma once
#include "ComponentData.h"
#include <string>

class TextureData : public ComponentData
{
public:
	TextureData() : ComponentData(COMPONENT_TYPE::MATERIAL) {}

	std::string path;
	uint textureID;
};