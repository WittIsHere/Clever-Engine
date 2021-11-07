#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleScene::~ModuleScene()
{}

bool ModuleScene::Init()
{
	LOG("ModuleScene Init");
	bool ret = true;

	return ret;
}

bool ModuleScene::Start()
{
	LOG("ModuleScene Starting");
	bool ret = true;

	return ret;
}

update_status ModuleScene::Update(float dt)
{

	return UPDATE_CONTINUE;
}


bool ModuleScene::CleanUp()
{
	LOG("Cleaning up ModuleScene");

	return true;
}


GameObject* ModuleScene::CreateRoot()
{
	GameObject* ret = new GameObject("Root Node", nullptr);
	return ret;
}


GameObject* ModuleScene::CreateGameObject()
{
	GameObject* ret = new GameObject;
	return ret;
}
