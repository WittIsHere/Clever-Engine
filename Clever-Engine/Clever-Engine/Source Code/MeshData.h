#pragma once
#include "ComponentData.h"
#include "MathGeoLib/include/Math/float3.h"
#include <vector>
class TextureData;

class MeshData : public ComponentData
{
public:
	MeshData() : ComponentData(COMPONENT_TYPE::MESH) {}

	uint vertexCount = 0;				// Number of Vertex
	std::vector<float3> vertices;

	uint vPosID = 0;					// Vertex Positions Buffer ID
	float* vPosData = nullptr;			// Vertex Positions Array

	uint vTexCoordsID = 0;				// Vertex Texture Coords Buffer ID
	float* vTexCoordsData = nullptr;	// Vertex Texture Coords Array

	uint vNormID = 0;					// Vertex Normals Buffer ID
	float* vNormData = nullptr;		// Vertex Normals Array

	uint indicesID = 0;					// Indices Buffer ID
	uint indicesCount = 0;				// Number of Indices
	uint* indicesData = nullptr;		// Indices Array

	const char* name = nullptr;
	TextureData* texture = 0;			// Pointer to acess the texture inside myTexures() vector
};