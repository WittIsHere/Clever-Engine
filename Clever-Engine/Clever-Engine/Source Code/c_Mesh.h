#pragma once

#include "Globals.h"
#include "Component.h"

struct MeshData;

class c_Mesh : public Component
{
public:
	c_Mesh(GameObject* parent, ComponentData* data);
	~c_Mesh();

	bool Enable();
	bool Update();
	bool Disable();

	const uint GetVertexCount();
	MeshData* GetMeshData();

private:
	MeshData* meshData = nullptr;

};