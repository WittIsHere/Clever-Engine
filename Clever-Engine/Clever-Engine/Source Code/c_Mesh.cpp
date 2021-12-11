#include "c_Mesh.h"
#include "ModuleScene.h"
#include "JSONParser.h"


c_Mesh::c_Mesh(GameObject* parent, COMPONENT_TYPE type) : Component(parent, type)
{
	//is empty by default
	meshData = nullptr;
}

c_Mesh::c_Mesh(GameObject* parent, ComponentData* data) : Component(parent, data->type)
{
	this->isEmpty = false;
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
	//to save the name first make sure it is assigned propperly dividing the path with the filesystem

	
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

bool c_Mesh::AssignNewData(MeshData* meshData)
{
	bool ret = true;
	
	if (this->isEmpty == true && this->data == nullptr)
	{
		data = meshData;
	}
	else
	{
		RELEASE(data);
		data = meshData;
	}
	return ret;
}



