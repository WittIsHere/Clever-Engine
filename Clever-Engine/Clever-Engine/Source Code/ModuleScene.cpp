
#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "Emitter.h"
#include "ModuleScene.h"

#include <math.h>

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
	/*cameraGO = CreateGameObject("Camera", rootNode);
	cameraGO->CreateComponent(COMPONENT_TYPE::CAMERA);*/

	LOG("Importing scene test");
	const char* fbxPath = ("Assets/Models/BakerHouse.FBX");
	App->importer->LoadModelToScene(fbxPath);

	//const char* fbxPath = ("Assets/Models/Street_environment.FBX");
	
	return ret;                        
}

update_status ModuleScene::Update(float dt)
{
	for (uint i = 0; i < gameObjects.size(); ++i)														// First Pass to Delete or Update the GOs' state.
	{
		if (gameObjects[i]->toDestroy)
		{
			if (gameObjects[i]->GetParent() != nullptr)
			{
				gameObjects[i]->GetParent()->DeleteChildFromArray(gameObjects[i]);
			}
			DeleteGameObject(gameObjects[i], i);
			continue;
		}

		if (gameObjects[i]->isActive)
		{
			gameObjects[i]->Update(dt);
		}

		if (ourEmitterInstance != nullptr)
			ourEmitterInstance->Update(dt);

	}
	return UPDATE_CONTINUE;
}

bool ModuleScene::Draw()
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects[i]->isActive == true /*&& gameObjects[i]->insideFrustum == true*/)
			gameObjects[i]->Draw();
	}
	return true;
}

bool ModuleScene::CleanUp()
{
	LOG("Cleaning up ModuleScene");

	for (auto object = gameObjects.begin(); object != gameObjects.end(); ++object)
	{
		(*object)->CleanUp();
		RELEASE((*object));
	}

	gameObjects.clear();	

	RELEASE(ourEmitterInstance);
	RELEASE(ourEmitter);

	rootNode = nullptr;
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

	if (buffer != nullptr)
	{
		CleanUp();

	}

	ParsonNode newRoot = ParsonNode(buffer);
	ParsonArray objectsArray = newRoot.GetArray("Game Objects");
	RELEASE_ARRAY(buffer);

	for (uint i = 0; i < objectsArray.size; ++i)					// Getting all the GameObjects in the ParsonArray
	{
		ParsonNode objectNode = objectsArray.GetNode(i);
		if (!objectNode.NodeIsValid())
		{
			continue;
		}

		GameObject* gameObject = new GameObject();

		gameObject->LoadState(objectNode); 

		if (gameObject->isRoot)
		{
			rootNode = gameObject;
		}

		gameObjects.push_back(gameObject);
	}
	
	//once all game objects are created, perform the parenting
	for (int i = 0; i < gameObjects.size(); i++)
	{
		GameObject* parent = GetGO(gameObjects[i]->parentID);
		if (parent != nullptr)
		{
			gameObjects[i]->UpdateParent();
			parent->AddChild(gameObjects[i]);
		}
	}


	return true;
}

void ModuleScene::CreateRootNode()
{
	//root node creation
	GameObject* RN = new GameObject(ROOT_NAME);
	RN->isRoot = true;
	rootNode = RN;	//assign a root node
	gameObjects.push_back(rootNode);
}

GameObject* ModuleScene::CreateGameObject(const char* name, GameObject* parent)
{
	GameObject* ret = new GameObject(name, parent);
	parent->AddChild(ret);
	gameObjects.push_back(ret);

	return ret;
}

GameObject* ModuleScene::CreateEmptyGameObject()
{
	GameObject* ret = new GameObject("Empty GameObject", rootNode);
	rootNode->AddChild(ret);
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
		GO->CleanUp();												
		
		uint32 goUID = GO->UUID;
		if (index != -1)														// If an index was given.
		{
			gameObjects.erase(gameObjects.begin() + index);						// Delete game object at index.
			
		}
		else
		{
			for (uint i = 0; i < gameObjects.size(); ++i)						// If no index was given.
			{
				if (gameObjects[i] == GO)										// Iterate game_objects until a match is found.
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

//bool ModuleScene::DeleteFromMeshPool(MeshData* mesh)
//{
//	for (int i = 0; i < meshPool.size(); i++)
//	{
//		if (meshPool[i] == mesh) 
//		{
//			meshPool.erase(meshPool.begin() + i);  //begin returns an iterator so just add it the number of steps to get to the desired mesh
//		}
//	}
//
//	return true;
//}

void ModuleScene::MousePicking(const LineSegment &picking)
{
	std::vector<GameObject*> possible;

	// Iterate all Game Objects to get the list of them on screen
	for (int i = 0; i < App->scene->gameObjects.size(); i++)
	{
		if (App->scene->gameObjects[i]->hasMesh == true)
		{
			 //Get the component mesh
			c_Mesh* mesh = nullptr;
			mesh = (c_Mesh*)App->scene->gameObjects[i]->GetComponentByType(COMPONENT_TYPE::MESH);

			 //Intersect the ray drawed before with the AABB box of the meshes
			if (picking.Intersects(mesh->GetAABB()))
			{
				float hitNear;
				float hitFar;

				possible.push_back(App->scene->gameObjects[i]);
				/*if (picking.Intersects(mesh->GetOBB(), hitNear, hitFar))
				{
					possible.push_back(App->scene->gameObjects[i]);
				}*/
			}
		}
		else if (App->scene->gameObjects[i]->IsCamera() == true)
		{
			c_Camera* camera = nullptr;
			camera = (c_Camera*)App->scene->gameObjects[i]->GetComponentByType(COMPONENT_TYPE::CAMERA);
			
			bool hit = picking.Intersects(camera->GetAABB());
			if (hit)
			{
				float dNear;
				float dFar;
				hit = picking.Intersects(camera->GetAABB(), dNear, dFar);
				
				possible.push_back(App->scene->gameObjects[i]);
			}
		}
	}
	if (possible.empty() == false)
	{
		App->ui->PickedGO(possible[0]->UUID);
		possible.clear();
	}

	//if (possible.empty() == false)
	//{
	//	for (int i = 0; possible.size(); i++)
	//	{
	//		math::float3 cameraPos = App->camera->GetCameraPosition();
	//		math::float3 originGO = possible[i]->GetComponentTransform()->GetWorldTransform().TranslatePart();
	//		int a = 20; 
	//	}
	//}
}
