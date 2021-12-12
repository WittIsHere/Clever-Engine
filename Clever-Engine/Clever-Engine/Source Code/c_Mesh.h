#pragma once
#define NOMINMAX 1
#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/OBB.h"
#include "MathGeoLib/include/Geometry/Sphere.h"
#include "MathGeoLib/include/Math/float3.h"

struct MeshData;
class ModuleImporter;

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
	void CreateBox();
	void DrawBox() const;
	void UpdateBox();

public:
	bool drawBBox;
	AABB aabbox, drawingBox;
	OBB obb;
	float radius;
	float3 centerPoint;
	bool AssignNewData(MeshData* meshData);

private:
	MeshData* meshData = nullptr;

};