#include "c_Mesh.h"
#include "ModuleScene.h"


c_Mesh::c_Mesh(GameObject* parent, ComponentData* data) : Component(parent, data)
{
	type = COMPONENT_TYPE::MESH;
	meshData = (MeshData*)data;
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

const uint c_Mesh::GetVertexCount()
{
	return meshData->vertexCount;
}

MeshData* c_Mesh::GetMeshData()
{
	return meshData;
}


