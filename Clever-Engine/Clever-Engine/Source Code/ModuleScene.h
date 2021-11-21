#pragma once
#include "Module.h"
#include "Globals.h"
#include "MeshData.h"
#include "MaterialData.h"
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

	GameObject* CreateGameObject(const char* name, GameObject* parent);
	GameObject* GetGO(uint32 uuid);

	bool DeleteFromMeshPool(MeshData* mesh);

public:
	GameObject* rootNode;
	std::vector<GameObject*> gameObjects;

	std::vector<MeshData*> meshPool;
	std::vector<TextureData*> texturePool;

private:

	void CreateRootNode();
};