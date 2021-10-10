#include "Globals.h"
#include "Application.h"
#include "ModuleImporter.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleImporter::~ModuleImporter()
{

}

bool ModuleImporter::Init()
{
	LOG("Init Module Imput");
	bool ret = true;

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return ret;
}

update_status ModuleImporter::PreUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	return ret;
}

bool ModuleImporter::CleanUp()
{
	LOG("CleanUp Module Importer");

	// detach log stream
	aiDetachAllLogStreams();

	return true;
}

void ImportScene(const char* file_path)
{
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene -> HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene % s", file_path);
	}
}

