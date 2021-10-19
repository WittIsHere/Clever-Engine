#pragma once
#include "Module.h"
#include "Globals.h"

#include <vector>

struct aiMesh;

struct MeshData
{
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
};

struct SceneData
{
	std::vector<MeshData*> myMeshes; 
};

class ModuleImporter : public Module
{
public:

	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init();
	update_status PreUpdate(float dt);
	bool CleanUp();

	void ImportScene(const char* file_path);
	void ImportMesh(aiMesh* mesh, MeshData* myMesh);

	SceneData myScene;

};