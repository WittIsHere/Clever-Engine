#pragma once
#include "Module.h"
#include "Globals.h"
#include "c_Mesh.h"


#include <vector>

struct aiMesh;
struct c_Material;

struct SceneData
{
	std::vector<c_Mesh*> myMeshes; 
	std::vector<c_Material*> myTextures;
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
	void ImportMesh(aiMesh* mesh, c_Mesh* myMesh);

	void LoadTextureFromPathAndFill(const char* path, c_Mesh* myMesh);
	uint LoadTextureFromPath(const char* path);

	SceneData myScene;

private:
	const char* textPath;
	//const void* bakerHouseTexData = nullptr;
};