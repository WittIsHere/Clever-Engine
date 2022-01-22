#pragma once

#include "Application.h"

class ResourceTexture : public Resource
{
public:
	
	ResourceTexture();
	~ResourceTexture();

	bool CleanUp() override;

	static inline ResourceType GetType() { return ResourceType::MESH; }

	inline uint GetTextureID() { return this->textureID; }
	inline void SetTextureID(uint id) { this->textureID = id; }

private:
	uint textureID;
};