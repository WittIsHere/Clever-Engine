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

bool ModuleScene::SaveScene(const char* sceneName) const
{
	LOG("Saving scene");

	ParsonNode rootNode = ParsonNode();
	ParsonArray objectsArray = rootNode.SetArray("Game Objects");
	for (auto object = gameObjects.begin(); object != gameObjects.end(); ++object)
	{
		ParsonNode arrayNode = objectsArray.SetNode((*object)->name.c_str());
		(*object)->SaveState(arrayNode);
	}

	char* buffer = nullptr;
	std::string name = (sceneName != nullptr) ? sceneName : this->rootNode->name.c_str();
	std::string path = ASSETS_SCENES_PATH + name + JSON_EXTENSION;
	uint written = rootNode.SerializeToFile(path.c_str(), &buffer);
	if (written > 0)
	{
		LOG("[SCENE] Scene: Successfully saved the current scene! Path: %s", path.c_str());
	}
	else
	{
		LOG("[ERROR] Scene: Could not save the current scene! Error: FileSystem could not write any data!");
	}

	return true;
}

bool ModuleScene::LoadScene(const char* path)
{
	char* buffer = nullptr;
	uint read = App->fileSystem->Load(path, &buffer);
	if (read == 0)
	{
		LOG("[ERROR] Scene Loading: Could not load %s from Assets! Error: File system could not read the file!", path);
		return false;
	}

	CleanUp();

	ParsonNode newRoot = ParsonNode(buffer);
	ParsonArray objectsArray = newRoot.GetArray("Game Objects");
	RELEASE_ARRAY(buffer);


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

bool ModuleScene::DeleteFromMeshPool(MeshData* mesh)
{
	for (int i = 0; i < meshPool.size(); i++)
	{
		if (meshPool[i] == mesh) 
		{
			meshPool.erase(meshPool.begin() + i);  //begin returns an iterator so just add it the number of steps to get to the desired mesh
		}
	}

	return true;
}
