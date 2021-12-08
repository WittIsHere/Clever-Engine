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
	for (uint i = 0; i < gameObjects.size(); ++i)														// First Pass to Delete or Update the GOs' state.
	{
		if (gameObjects[i]->toDestroy)
		{
			DeleteGameObject(gameObjects[i], i);
			continue;
		}

		if (gameObjects[i]->isActive)
		{
			gameObjects[i]->Update();
		}
	}
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

void ModuleScene::DeleteGameObject(GameObject* GO, int index)
{
	if (GO == nullptr)
	{
		LOG("[ERROR] Scene: Object to delete was nullptr!");
		return;
	}

	if (App->ui->nodeClicked == GO->UUID)
	{
		App->ui->nodeClicked = -1;
	}

	if (!gameObjects.empty())													// Extra check just to make sure that at least one GameObject remains in the Scene.
	{
		GO->CleanUp();													// As it has not been Cleaned Up by its parent, the GameObject needs to call its CleanUp();
		uint32 goUID = GO->UUID;

		if (index != -1)														// If an index was given.
		{
			gameObjects.erase(gameObjects.begin() + index);						// Delete game object at index.
		}
		else
		{
			for (uint i = 0; i < gameObjects.size(); ++i)						// If no index was given.
			{
				if (gameObjects[i] == GO)								// Iterate game_objects until a match is found.
				{
					gameObjects.erase(gameObjects.begin() + i);					// Delete the game_object at the current loop index.
					break;
				}
			}
		}

		RELEASE(GO);
	}
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
