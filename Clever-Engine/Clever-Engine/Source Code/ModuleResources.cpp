#include "Globals.h"
#include "Application.h"
#include "ModuleResources.h"


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
	
	return ret;
}

update_status ModuleResources::Update(float dt)
{


	return UPDATE_CONTINUE;
}

