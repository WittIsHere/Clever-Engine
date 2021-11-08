#include "c_Mesh.h"
#include "ModuleScene.h"


c_Mesh::c_Mesh()
{
	type = COMPONENT_TYPE::MESH;
}

c_Mesh::~c_Mesh()
{
}

bool c_Mesh::Enable()
{
	return false;
}

bool c_Mesh::Update()
{
	return false;
}

bool c_Mesh::Disable()
{
	return false;
}
