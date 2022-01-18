#pragma once
#include "Module.h"
#include "Globals.h"
#include "ResourceBase.h"
#include "ModuleFileSystem.h"
#include "ModuleImporter.h"

#include <map>

class Resource;
class ResourceBase;

class ModuleResources : public Module
{
public:
	ModuleResources(Application* app, bool start_enabled = true);
	~ModuleResources();

	bool Init();
	bool Start();
	update_status Update(float ft);
	bool CleanUp();

	void ImportAssetsFolder();
	void ImportOurAssets(PathNode node);

	bool SaveMetaFile(ResourceBase res, const char* name = nullptr);
	Resource* CreateResource(ResourceType type, const char* assetsPath, uint32 forcedUID);
	ParsonNode LoadMetaFile(const char* assetsPath, char** buffer);
	
public:
	Resource* GetResource(uint32 id);
	bool AllocateResource(uint32 id, ResourceBase base);
	bool DeleteResource(uint32 UID);

public:

	std::map<uint32, Resource*>		resources;
	std::map<uint32, ResourceBase>	library;

};