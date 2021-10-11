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

	LOG("Importing scene test");

	const char* warriorPath = ("Assets/Models/warrior.FBX");
	ImportScene(warriorPath);

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

//TODO: make this method be of type myScene and return the loaded scene
void ModuleImporter::ImportScene(const char* file_path)
{
	const aiScene* aiScene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (aiScene != nullptr && aiScene-> HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < aiScene->mNumMeshes; i++)
		{
			MeshData* tempMesh = new MeshData;
			myScene.myMeshes.push_back(tempMesh);
			ImportMesh(aiScene->mMeshes[i], myScene.myMeshes[i]);
		}
		aiReleaseImport(aiScene);
	}
	else
	{
		LOG("Error loading scene % s", file_path);
	}
}

void ModuleImporter::ImportMesh(aiMesh* mesh, MeshData* myMesh)
{
	// Copying vertex
	myMesh->num_vertex = mesh->mNumVertices;
	myMesh->vertex = new float[myMesh->num_vertex * 3];
	memcpy(myMesh->vertex, mesh->mVertices, sizeof(float) * myMesh->num_vertex * 3);

	LOG("New mesh with %d vertices", myMesh->num_vertex);

	// Copying faces
	if (mesh->HasFaces())
	{
		myMesh->num_index = mesh->mNumFaces * 3;
		myMesh->index = new uint[myMesh->num_index];

		for (uint i = 0; i < mesh->mNumFaces; i++)
		{
			if (mesh->mFaces[i].mNumIndices != 3)
			{
				LOG("Warning, geometry face with != indices!");
			}
			else
				memcpy(&myMesh->index[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
		}
	}
}

