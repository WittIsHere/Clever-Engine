#pragma once

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/OBB.h"

struct MeshData;
class ModuleImporter;

class c_Mesh : public Component
{
public:
	c_Mesh(GameObject* parent, ComponentData* data);
	~c_Mesh();

	bool Enable();
	bool Update();
	bool Disable();

	bool SaveState(ParsonNode& root) const override;
	bool LoadState(ParsonNode& root) override;

	const uint GetVertexCount();
	MeshData* GetMeshData();
	void CreateBox();
	void DrawBox() const;
	void UpdateBox();

public:
	bool drawBBox;
	AABB aabbox, drawingBox;
	OBB obb;
	MeshData* mesh;

private:
	MeshData* meshData = nullptr;

};