#pragma once
#include "Module.h"
#include "Globals.h"
#include "ResourceBase.h"

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


private:

	std::map<uint32, Resource*>		resources;
	std::map<uint32, ResourceBase>	library;

};