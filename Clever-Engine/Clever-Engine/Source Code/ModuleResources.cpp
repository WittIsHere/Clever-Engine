#include "Globals.h"
#include "Application.h"
#include "JSONParser.h"
#include "ModuleResources.h"
#include "ModuleImporter.h"
#include "Resource.h"
#include "ResourceMesh.h"
#include "ResourceParticleSystem.h"
#include "ResourceTexture.h"

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
	
	//ImportAssetsFolder();

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
		// Grab all files with extension .FBX from a folder and put them iniside myAssets vector.
		App->fileSystem->GetAllFilesWithExtensionMod(node.children[i].path.c_str(), "FBX", myAssets);
		App->fileSystem->GetAllFilesWithExtensionMod(node.children[i].path.c_str(), "fbx", myAssets);
	}

	for (int i = 0; i < myAssets.size(); i++)
	{
		//new attempt
		std::string path;
		std::string filename;
		App->fileSystem->SplitFilePath(myAssets[i].c_str(), &path, &filename);

		std::string fullName = path + filename + META_EXTENSION;
		int j = 0;
		
		// If the meta exists then load all related lib files into lib map and skip the file
		if (App->fileSystem->Exists(fullName.c_str()))
		{
			while (App->fileSystem->Exists(fullName.c_str()))
			{
				char* buffer = nullptr;
				ParsonNode metaRoot = LoadMetaFile(fullName.c_str(), &buffer);
				//get the id and type from meta
				uint32 resourceUid = (uint32)metaRoot.GetNumber("UID");
				int type = metaRoot.GetNumber("Type");
				RELEASE_ARRAY(buffer);

				if (metaRoot.NodeIsValid())
				{
					//create the lib path "library/meshes/XXXXXXXXX.MYMESH"
					std::string ffExtension = CUSTOM_FF_EXTENSION;
					std::string libPath = MESHES_PATH + std::to_string(resourceUid) + ffExtension;  //TODO fix this path with a GetLibFolderByExtension

					//emplace the resourceBase 
					library.emplace(resourceUid, ResourceBase(resourceUid, myAssets[i].c_str(), libPath, (ResourceType)type));

					//update the path to make the root to point to the next meta of the fbx filename + i
					j++;
					fullName = path + filename + std::to_string(j) + META_EXTENSION;
				}
				else
				{
					LOG("ERROR: meta file not valid. Meta path: %s", fullName.c_str());
					continue;
				}

			}
		}
		else
		{
			// If the meta cannot be located, import to CFF.d (load the .FBX with assimp and convert it to MYMODDEL)
			App->importer->ImportToCustomFF(myAssets[i].c_str());
		}
	}
	int j = 0;

}

bool ModuleResources::SaveMetaFile(ResourceBase resource, const char* name, uint32 textureUID)
{
	//Load meta file with info ------------------						
	ParsonNode metaRoot = ParsonNode();		
	metaRoot.SetString("Name", name);
	metaRoot.SetNumber("UID", resource.GetUID());																					
	metaRoot.SetNumber("Type", (uint)resource.GetType());			
	if (resource.GetType() == ResourceType::MESH)
	{
		metaRoot.SetNumber("materialUID", textureUID);
		metaRoot.SetString("texName", App->importer->comodínGuapissimo.c_str()); 
	}

	//metaRoot.SetString("Name", name); 																							
	metaRoot.SetString("AssetsPath", resource.GetAssetsPath());
	metaRoot.SetString("LibraryPath", resource.GetLibraryPath());																	

	double modTime = (double)App->fileSystem->GetLastModTime(resource.GetAssetsPath());
	metaRoot.SetNumber("ModTime", modTime);
	//resource->SaveMeta(metaRoot);																									

	//Save Meta into a file
	char* buffer = nullptr;

	std::string path;
	std::string filename;
	App->fileSystem->SplitFilePath(resource.GetAssetsPath(), &path, &filename);

	std::string fullPath = path + filename + META_EXTENSION;
	//add a number after the filename in case the file already exists
	int i = 1;
	while (App->fileSystem->Exists(fullPath.c_str()))
	{
		fullPath = path + filename + std::to_string(i) + META_EXTENSION;
		i++;
	}

	uint written = metaRoot.SerializeToFile(fullPath.c_str(), &buffer);

	if (written > 0)
	{
		LOG("[STATUS] Resource Manager: Successfully Saved the Meta File for Resource %d Path: %s", resource.GetUID(), fullPath.c_str());
	}
	else
	{
		LOG("[ERROR] Resource Manager: Could not Save the Meta File of Resource %lu! Error: File System could not write the file.", resource.GetUID());
	}

	RELEASE_ARRAY(buffer);

	return true;
}

ParsonNode ModuleResources::LoadMetaFile(const char* finalPath, char** buffer)
{
	if (finalPath == nullptr)
	{
		LOG("[ERROR] Resource Manager: Could not load the .meta File! Error: Given path was nullptr!");
	}

	uint read = App->fileSystem->Load(finalPath, buffer);
	if (read == 0)
	{
		LOG("[ERROR] Resource Manager: Could not load the .meta File with Path: %s! Error: No Meta File exists with the given path.", finalPath);
	}

	//release the buffer after

	return ParsonNode(*buffer);
}

//Create and 
Resource* ModuleResources::CreateResource(ResourceType type, const char* assetsPath, uint32 forcedUID)
{
	Resource* resource = nullptr;
	switch (type)
	{
	case ResourceType::MESH: { resource = new ResourceMesh(); }			break;
	//case ResourceType::MATERIAL: { resource = new R_Material(); }		break;
	case ResourceType::TEXTURE: 
	{ 
		resource = new ResourceTexture();
		
	}	break;
	//case ResourceType::MODEL: { resource = new R_Model(); }			break;
	case ResourceType::PARTICLE_SYSTEM: { resource = new ResourceParticleSystem(); }	break;
	case ResourceType::NONE: { /*resource = nullptr;*/ }				break;			// In case NONE is a trigger and a method needs to be called.
	}

	if (resource != nullptr)
	{
		if (assetsPath != nullptr)
		{
			resource->assetsPath = assetsPath;
		}

		if (forcedUID != 0) resource->ForceUID(forcedUID);

		switch (type)
		{
		case (ResourceType::MESH):
		{
			resource->libraryPath = MESHES_PATH + std::to_string(resource->GetUID()) + CUSTOM_FF_EXTENSION;
			break;
		}
		case (ResourceType::TEXTURE):
		{
			resource->libraryPath = TEXTURES_PATH + std::to_string(resource->GetUID()) + TEXTURES_EXTENSION;
			break;
		}
		case (ResourceType::PARTICLE_SYSTEM):
		{
			resource->libraryPath = PARTICLESYSTEMS_PATH + std::to_string(resource->GetUID()) + PARTICLESYSTEMS_AST_EXTENSION;
			break;
		}
		}
	}

	return resource;
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
			LOG("[Error] Module Resources: trying to get a resource with an ID that is not found inside library. ID: %d", id);
			return nullptr;
		}
		else
		{
			auto rItem = library.find(id);
			if (AllocateResource(rItem->first, rItem->second))
			{
				auto rItem = resources.find(id);
				//add 1 to the references count and return the resource
				rItem->second->AddReference();
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
		rItem->second->AddReference();
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

	if (base.GetType() == ResourceType::MESH)
	{
		//TODO: CreateResource(type, path, uid)
		Resource* resource = CreateResource(base.GetType(), base.GetAssetsPath(), id);

		bool success = false;
		success = App->importer->CustomMeshToScene(resource->libraryPath.c_str(), (ResourceMesh*)resource);

		if (success)
		{
			resources.emplace(resource->GetUID(), resource);
			LOG("[STATUS] Resource Manager: Successfully Allocated Resource in Memory!");
		}
		else
		{
			DeleteResource(resource->GetUID());	// This deletes from resources and library!.
			LOG("[ERROR] Resource Manager: Importer could not load the Resource Data from file");
		}
		return success;
	}
	else
	{
		ResourceTexture* resource = (ResourceTexture*)CreateResource(base.GetType(), base.GetAssetsPath(), id);
		uint texID = App->importer->LoadTextureFromPath(base.GetAssetsPath(), id);
		resource->SetTextureID(texID);
		resources.emplace(resource->GetUID(), resource);
	}

	return true;

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


