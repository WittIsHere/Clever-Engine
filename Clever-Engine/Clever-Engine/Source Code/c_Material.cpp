#include "c_Material.h"
#include "ModuleScene.h"


c_Material::c_Material(GameObject* parent, ComponentData* data) : Component(parent, data)
{
	type = COMPONENT_TYPE::MATERIAL;
	textureData = (TextureData*)data;
}

c_Material::~c_Material()
{
}

bool c_Material::Enable()
{
	return false;
}

bool c_Material::Update()
{
	return false;
}

bool c_Material::Disable()
{
	return false;
}

const char* c_Material::getPath()
{
	return textureData->path.c_str();
}

const uint c_Material::getTextureID()
{
	return textureData->textureID;
}

void c_Material::setPath(const char* path)
{
	textureData->path = path;
}

void c_Material::setTextureID(uint id) //does this method make any sense? shouldn't we just change the texturedata instead of the id?
{
	textureData->textureID = id;
}

void c_Material::changeTextureData(TextureData* data)
{
	textureData = data;
}






