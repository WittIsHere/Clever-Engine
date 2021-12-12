#include "Globals.h"
#include "Application.h"
#include "ModuleResources.h"

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


