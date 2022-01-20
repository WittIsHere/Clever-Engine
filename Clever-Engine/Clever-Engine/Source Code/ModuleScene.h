#pragma once
#include "Module.h"
#include "Globals.h"
#include "MeshData.h"
#include "MaterialData.h"
#include "ModuleImporter.h"
#include "EmitterInstance.h"
#include <vector>

class GameObject;

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Init();
	bool Start();
	update_status Update(float ft);
	bool Draw();
	bool CleanUp();

	bool SaveScene(const char* sceneName = nullptr) const;	// If no name is given the scene_root node's name will be used.
	bool LoadScene(const char* path);						// For now asks for full path 

	GameObject* CreateGameObject(const char* name, GameObject* parent);
	GameObject* CreateEmptyGameObject();
	void DeleteGameObject(GameObject* GO, int index = -1);
	GameObject* GetGO(uint32 uuid);

	bool DeleteFromMeshPool(MeshData* mesh);

public:
	GameObject* rootNode;
	std::vector<GameObject*> gameObjects;
	GameObject* mainCamera = nullptr;
	//std::vector<MeshData*> meshPool;
	std::vector<MeshData*> meshPool;
	std::vector<TextureData*> texturePool;

	Emitter* ourEmitter = nullptr;
	EmitterInstance* ourEmitterInstance = nullptr;

private:

	void CreateRootNode();
};