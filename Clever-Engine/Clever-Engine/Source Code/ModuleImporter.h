#pragma once
#include "Module.h"
#include "Globals.h"


#include <vector>

struct aiMesh;
struct MeshData;

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

	bool LoadTextureFromPath(const char* path, MeshData* myMesh);
	uint FillTexture(const void* text, uint width, uint height, int format, uint format2, const char* path);

	SceneData myScene;

private:
	const char* textPath;

};