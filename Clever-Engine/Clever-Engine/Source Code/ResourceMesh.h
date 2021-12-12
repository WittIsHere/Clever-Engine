#pragma once

#include <map>

#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/OBB.h"
#include "MathGeoLib/include/Math/float2.h"
#include "MathGeoLib/include/Math/float4x4.h"

#include "Resource.h"

class ParsonNode;

struct Bone;
struct BoneWeight;

typedef unsigned int uint;

class ResourceMesh : public Resource
{
public:
	ResourceMesh();
	~ResourceMesh();

	bool CleanUp() override;

	static inline ResourceType GetType() { return ResourceType::MESH; }

public:
	AABB GetAABB() const;
	void SetAABB();

public:
	uint vertexCount = 0;				// Number of Vertex

	uint vPosID = 0;					// Vertex Positions Buffer ID
	float* vPosData = nullptr;			// Vertex Positions Array

	uint vTexCoordsID = 0;				// Vertex Texture Coords Buffer ID
	float* vTexCoordsData = nullptr;	// Vertex Texture Coords Array

	uint vNormID = 0;					// Vertex Normals Buffer ID
	float* vNormData = nullptr;			// Vertex Normals Array

	uint indicesID = 0;					// Indices Buffer ID
	uint indicesCount = 0;				// Number of Indices
	uint* indicesData = nullptr;		// Indices Array

	//bool drawVertexNormals;
	//bool drawFaceNormals;
	//bool hasBones;

	AABB aabb;
};