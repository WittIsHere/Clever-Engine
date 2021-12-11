#pragma once

#include "Globals.h"
#include "Component.h"

struct MeshData;

class c_Mesh : public Component
{
public:
	//default constructor
	c_Mesh(GameObject* parent, COMPONENT_TYPE type);
	c_Mesh(GameObject* parent, ComponentData* data);
	~c_Mesh();

	bool Enable();
	bool Update();
	bool Disable();

	bool SaveState(ParsonNode& root) const override;
	bool LoadState(ParsonNode& root) override;

	const uint GetVertexCount();
	MeshData* GetMeshData();

	bool AssignNewData(MeshData* meshData);

private:
	MeshData* meshData = nullptr;

};