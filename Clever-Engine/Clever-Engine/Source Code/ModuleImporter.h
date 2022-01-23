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
struct ResourceMesh;

class ModuleImporter : public Module
{
public:

	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	bool CleanUp();

	//model importer
	void LoadModel(const char* file_path);
	void LoadModelToScene(const char* file_path);
	
	//mesh importer
	void ImportMesh(aiMesh* mesh, ResourceMesh* myMesh); //not used

	//texture importer
	//void LoadTextureFromPathAndFill(const char* path, MeshData* myMesh);
	uint LoadTextureFromPath(const char* path, uint32 uuid);

	void ImportToCustomFF(const char* libPath);

	uint32 CreateAndSaveResourceMesh(const aiMesh* mesh, TMYMODEL* myModel, const char* name, const char* assetsPath, uint32 textureUID = 0);


	TMYMODEL* CreateCustomMesh(const aiMesh* m);
	bool SaveCustomMeshFile(const TMYMODEL* m, const char* path);

	MeshData* CustomMeshToScene(const char* path);
	bool CustomMeshToScene(const char* path, ResourceMesh* mesh);

	uint32 EmplaceTextureForParticle(const char* assPath);
	void Someting(uint32 uuid);
	uint GetTextureIDForParticle(const char* path, uint32 uuid);

private:
	void ImportModelToLibrary(aiNode* sceneRoot, const aiScene* currentScene, const char* fileName);
	void NodeToResource(aiNode* currentNode, const aiScene* currentScene, const char* fileName);

	void ImportModelToScene(aiNode* sceneRoot, const aiScene* currentScene, const char* fileName);
	void LoadNode(GameObject* parent, aiNode* currentNode, const aiScene* currentScene, const char* path);

private:
	const char* textPath;

public:
	std::string comodínGuapissimo;
	uint textureBuffer1 = 0;

};