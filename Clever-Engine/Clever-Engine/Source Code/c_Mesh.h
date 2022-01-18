#pragma once
#define NOMINMAX 1
#include "Globals.h"
#include "Component.h"
#include "ResourceMesh.h"
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
	c_Mesh(GameObject* parent, Resource* data);
	~c_Mesh();

	bool Enable();
	bool Update(float dt);
	bool Disable();

	bool SaveState(ParsonNode& root) const override;
	bool LoadState(ParsonNode& root) override;

	const uint GetVertexCount();
	ResourceMesh* GetMeshData();
	void CreateBox();
	void DrawBox() const;
	void UpdateBox();

	bool AssignNewData(ResourceMesh* meshData);

public:
	bool drawBBox;
	AABB aabbox, drawingBox;
	OBB obb;

	float radius;
	float3 centerPoint;

private:
	//MeshData* meshData = nullptr;
	ResourceMesh* resource = nullptr;

};