#include "c_Material.h"


c_Material::c_Material()
{
	type = ComponentType::MATERIAL;
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
