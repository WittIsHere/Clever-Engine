#include "Globals.h"
#include "Application.h"
#include "ModuleResources.h"
#include "ModuleImporter.h"
#include "Resource.h"
#include "ResourceMesh.h"

#include "PathNode.h"

ModuleResources::ModuleResources(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleResources::~ModuleResources()
{}

bool ModuleResources::Init()
{
	LOG("ModuleResources Init");
	bool ret = true;

	return ret;
}

bool ModuleResources::Start()
{
	LOG("ModuleResources Starting");
	bool ret = true;
	
	ImportAssetsFolder();

	return ret;
}

update_status ModuleResources::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleResources::CleanUp()
{
	bool ret = true;

	std::map<uint32, Resource*>::iterator rItem;
	for (rItem = resources.begin(); rItem != resources.end(); ++rItem)
	{
		rItem->second->CleanUp();
		RELEASE(rItem->second);
	}

	resources.clear();
	library.clear();

	return ret;
}

void ModuleResources::ImportAssetsFolder()
{
	PathNode ourAssets = App->fileSystem->GetAllFiles("Assets", nullptr, nullptr);
	ImportOurAssets(ourAssets);
}

void ModuleResources::ImportOurAssets(PathNode node)
{
	std::vector<std::string> myAssets;

	for (int i = 0; i < node.children.size(); i++)
	{
		App->fileSystem->GetAllFilesWithExtensionMod(node.children[i].path.c_str(), "FBX", myAssets);
	}

	for (int i = 0; i < myAssets.size(); i++)
	{
		App->importer->ImportToCustomFF(myAssets[i].c_str());
	}
}

Resource* ModuleResources::GetResource(uint32 id)
{
	if (id == 0)
	{
		LOG("[Error] Module Resources: Resource Request Failed! Error: Requested UID was 0.");
		return nullptr;
	}

	//look for an entry of the id inside the resources map
	if (resources.find(id) == resources.end())//not yet present in the scene.
	{
		if (library.find(id) == library.end()) //map.find() returns an iterator that ends up being equal to map.end() if the search was unsuccessfull.
		{
			LOG("[Error] Module Resources: trying to get a resource with an ID that is not found inside library ");
			return nullptr;
		}
		else
		{
			auto rItem = library.find(id);
			if (AllocateResource(rItem->first, rItem->second))
			{
				auto rItem = resources.find(id);
				//add 1 to the references count and return the resource
				rItem->second->references += 1;
				return rItem->second;
			}
			else
			{
				LOG("[Error] Module Resources: resource allocation failed ");
				return nullptr;
			}
		}
			
	}
	else //is already allocated in memory
	{
		auto rItem = resources.find(id);
		//add 1 to the references count and return the resource
		rItem->second->references += 1;
		return rItem->second; 
	}
}

bool ModuleResources::AllocateResource(uint32 id, ResourceBase base)
{
	//create a new resource and load it with the resource base path
	//then add it to the resources map alongside its uuid

	if (base.assetsPath.empty())
	{
		LOG("[ERROR] Resource Manager: Could not allocate Resource in Memory! Error: Given Path was nullptr.");
		return false;
	}

	if (id == 0)
	{
		LOG("[ERROR] Resource Manager: Invalid UID.");
		return false;
	}

	if (library.find(id) == library.end())
	{
		LOG("[ERROR] Resource Manager: library entry not found.");
		return false;
	}

	auto item = resources.find(id); //check if it is already loaded
	if (item != resources.end())
	{
		return true;
	}

	//char* buffer = nullptr;
	//const char* libraryPath = library.find(id)->second.libraryPath.c_str();
	//uint read = App->fileSystem->Load(libraryPath, &buffer);
	//if (read == 0)
	//{
	//	LOG("[ERROR] Resource Manager: File system could not read File [%s]", libraryPath);
	//	return false;
	//}

	ResourceType type = ResourceType::MESH; //TODO solve the hardcode
	//TODO: CreateResource(type, path, uid)
	ResourceMesh* resource = new ResourceMesh();  
	resource->assetsPath = base.assetsPath.c_str();
	resource->libraryPath = base.libraryPath.c_str();
	resource->uid = id;

	bool success = false;
	success = App->importer->LoadModel(resource->libraryPath.c_str(), resource);
	//switch (type)
	//{
	//case ResourceType::MODEL: { success = Importer::Scenes::Load(buffer, (R_Model*)resource); }				break;
	//case ResourceType::MESH: { success = Importer::Meshes::Load(buffer, (R_Mesh*)resource); }				break;
	//
	//case ResourceType::NONE: { /*success = false;*/ }														break;	// In case NONE is a trigger and a method needs to be called.
	//}

	//RELEASE_ARRAY(buffer);

	if (success)
	{
		resources.emplace(resource->uid, resource);
		LOG("[STATUS] Resource Manager: Successfully Allocated Resource in Memory!");
	}
	else
	{
		DeleteResource(resource->uid);	// This deletes from resources and library!.
		LOG("[ERROR] Resource Manager: Importer could not load the Resource Data from file");
	}

	return success;

}

bool ModuleResources::DeleteResource(uint32 UID)
{
	if (library.find(UID) != library.end())
	{
		library.erase(UID);
	}

	auto item = resources.find(UID);
	if (item == resources.end())
	{
		LOG("[ERROR] Resource Manager: Could not delete Resource! Error: Resource to delete { %lu } was not inside the resources map!", UID);
		return false;
	}

	if (item->second != nullptr)																					
	{
		item->second->CleanUp();
		RELEASE(item->second);
	}

	resources.erase(UID);

}


