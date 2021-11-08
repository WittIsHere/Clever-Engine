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

	return ret;
}

bool ModuleScene::Start()
{
	LOG("ModuleScene Starting");
	bool ret = true;

	CreateRootNode();
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

void ModuleScene::CreateRootNode()
{
	//root node creation
	const char* rootName = "RootNode";
	GameObject* RN = new GameObject(rootName);

	rootNode = RN;	//assign a root node
}

GameObject* ModuleScene::CreateGameObject()
{
	GameObject* ret = new GameObject();
	return ret;
}
