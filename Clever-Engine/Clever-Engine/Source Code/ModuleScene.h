#pragma once
#include "Module.h"
#include "Globals.h"

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

	GameObject* CreateGameObject();

public:
	std::vector<GameObject*> gameObjects;

};