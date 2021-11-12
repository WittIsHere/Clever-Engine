#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "ModuleScene.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleScene::~ModuleScene()
{}

bool ModuleScene::Init()
{
	LOG("ModuleScene Init");
	bool ret = true;
	CreateRootNode();
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

bool ModuleScene::Draw()
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if(gameObjects[i]->isActive == true)
			gameObjects[i]->Draw();
	}
	return true;
}

bool ModuleScene::CleanUp()
{
	LOG("Cleaning up ModuleScene");

	return true;
}

void ModuleScene::CreateRootNode()
{
	//root node creation
	GameObject* RN = new GameObject(ROOT_NAME);

	rootNode = RN;	//assign a root node
}

GameObject* ModuleScene::CreateGameObject(const char* name, GameObject* parent)
{
	GameObject* ret = new GameObject(name, parent);
	gameObjects.push_back(ret);

	return ret;
}

GameObject* ModuleScene::GetGO(uint32 uuid)
{
	GameObject* ret = nullptr;
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects[i]->UUID == uuid)
		{
			ret = gameObjects[i];
			break;
		}
	}
	return ret;
}
