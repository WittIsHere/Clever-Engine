#pragma once
#include "Module.h"
#include "Globals.h"


#include <vector>

struct GameObject;
struct aiMesh;
struct aiNode;
struct aiScene;
struct c_Material;
struct MeshData;
struct MaterialData;
struct TMYMODEL;
struct PathNode;

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

	void LoadRoot(aiNode* sceneRoot, const aiScene* currentScene, const char* fileName);
	void LoadNode(GameObject* parent, aiNode* currentNode, const aiScene* currentScene);

	void LoadTextureFromPathAndFill(const char* path, MeshData* myMesh);
	uint LoadTextureFromPath(const char* path);

	void ImportAssetsFolder();
	void ImportOurAssets(PathNode node);

	TMYMODEL* callCreateAndSave(const aiMesh* mesh, const char* path, TMYMODEL* myModel);

	TMYMODEL* createMyModel(const aiMesh* m);
	bool SaveModel(const TMYMODEL* m, const char* path);
	MeshData* LoadModel(const char* path);

private:
	const char* textPath;

};