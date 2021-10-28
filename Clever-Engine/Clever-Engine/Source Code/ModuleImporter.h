#pragma once
#include "Module.h"
#include "Globals.h"


#include <vector>

struct aiMesh;
struct MeshData;
struct TextureData;

struct SceneData
{
	std::vector<MeshData*> myMeshes; 
	std::vector<TextureData*> myTextures;
};

class ModuleImporter : public Module
{
public:

	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	bool CleanUp();

	void ImportScene(const char* file_path);
	void ImportMesh(aiMesh* mesh, MeshData* myMesh);

	void LoadTextureFromPathAndFill(const char* path, MeshData* myMesh);
	uint LoadTextureFromPath(const char* path);

	SceneData myScene;

private:
	const char* textPath;
	//const void* bakerHouseTexData = nullptr;
};