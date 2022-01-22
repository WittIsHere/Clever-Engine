#include "ResourceTexture.h"

ResourceTexture::ResourceTexture() : Resource(ResourceType::TEXTURE)
{
	textureID = 0;
}

ResourceTexture::~ResourceTexture()
{
}

bool ResourceTexture::CleanUp()
{
	return false;
}

