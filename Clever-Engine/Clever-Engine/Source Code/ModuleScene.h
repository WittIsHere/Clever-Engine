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
	bool CleanUp();

	GameObject* CreateGameObject(d);

public:
	GameObject* rootNode;
	std::vector<GameObject*> gameObjects;

	std::vector<meshData*> meshPool;
	std::vector<textureData*> texturePool;

private:

	void CreateRootNode();
};