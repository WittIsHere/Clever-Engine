#include "c_Material.h"
#include "ModuleScene.h"


c_Material::c_Material(GameObject* parent, ComponentData* data) : Component(parent, data)
{
	type = COMPONENT_TYPE::MATERIAL;
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
