#include "c_Mesh.h"
#include "ModuleScene.h"
#include "JSONParser.h"


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
	return true;
}

bool c_Mesh::Update()
{
	return true;
}

bool c_Mesh::Disable()
{
	return true;
}

bool c_Mesh::SaveState(ParsonNode& root) const
{
	root.SetNumber("Type", (uint)type);

	return true;
}

bool c_Mesh::LoadState(ParsonNode& root)
{
	return true;
}

const uint c_Mesh::GetVertexCount()
{
	return meshData->vertexCount;
}

MeshData* c_Mesh::GetMeshData()
{
	return meshData;
}


