#include "c_Material.h"
#include "ModuleScene.h"


c_Material::c_Material()
{
	type = ComponentType::MATERIAL;
	resourceTexture = nullptr;
}

c_Material::c_Material(textureData* data)
{
	type = ComponentType::MATERIAL;
	resourceTexture = data;
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
