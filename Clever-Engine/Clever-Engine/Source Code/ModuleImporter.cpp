#include "Globals.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "Mesh.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

#pragma comment( lib, "Devil/libx86/DevIL.lib" )
#include "Devil\include\ilu.h"
#pragma comment( lib, "Devil/libx86/ILU.lib" )
#include "Devil\include\ilut.h"
#pragma comment( lib, "Devil/libx86/ILUT.lib" )

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
	
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return ret;
}

bool ModuleImporter::Start()
{
	LOG("Importing scene test");

	const char* fbxPath = ("Assets/Models/BakerHouse.FBX");
	textPath = ("Assets/Textures/Baker_house.PNG");
	ImportScene(fbxPath);
	App->renderer3D->houseTexture_Buffer = LoadTextureFromPath("Assets/Textures/Baker_house.PNG");

	return true;
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
	if (aiScene != nullptr)
	{
		if (aiScene->HasMeshes())
		{
			// Use scene->mNumMeshes to iterate on scene->mMeshes array
			for (int i = 0; i < aiScene->mNumMeshes; i++)
			{
				MeshData* tempMesh = new MeshData;
				myScene.myMeshes.push_back(tempMesh);
				ImportMesh(aiScene->mMeshes[i], myScene.myMeshes[i]);
				LoadTextureFromPathAndFill(textPath, myScene.myMeshes[i]);
			}
		}
		else
		{
			LOG("Error loading meshes, scene % s", file_path);
		}

		aiReleaseImport(aiScene);
	}
}

void ModuleImporter::ImportMesh(aiMesh* mesh, MeshData* myMesh)
{
	// Copying number of vertices
	myMesh->vertexCount = mesh->mNumVertices;

	// Copying vertex positions
	myMesh->vPosData = new float[myMesh->vertexCount * 3];
	memcpy(myMesh->vPosData, mesh->mVertices, sizeof(float) * myMesh->vertexCount * 3);

	LOG("New mesh with %d vertices", myMesh->vertexCount);

	// Copying faces
	if (mesh->HasFaces())
	{
		myMesh->indicesCount = mesh->mNumFaces * 3;
		myMesh->indicesData = new uint[myMesh->indicesCount];

		for (uint i = 0; i < mesh->mNumFaces; i++)
		{
			if (mesh->mFaces[i].mNumIndices != 3)
			{
				LOG("Warning, geometry face with != indices!");
			}
			else
			{
				memcpy(&myMesh->indicesData[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}
	}

	// Copying Texture coordinates
	if (mesh->mTextureCoords != NULL)
	{
		myMesh->vTexCoordsData = new float[myMesh->vertexCount * 2];

		for (uint i = 0; i < myMesh->vertexCount; i++)
		{
				myMesh->vTexCoordsData[i*2] = mesh->mTextureCoords[0][i].x;
				myMesh->vTexCoordsData[(i*2)+1] = mesh->mTextureCoords[0][i].y;
		}
	}
	else
	{
		LOG("Warning, No texture coordinates found");
	}

	// Copying Normals
	//if (mesh->HasNormals())
	//{
	//	myMesh->vNormData = new float[myMesh->vertexCount * 3];

	//	for (uint i = 0; i < myMesh->vertexCount; i++)
	//	{
	//		memcpy(&myMesh->vNormData[i * 3], mesh->mNormals[i], 3 * sizeof(float));
	//	}
	//}
	//else
	//{
	//	LOG("Warning, No texture coordinates found");
	//}
}

void ModuleImporter::LoadTextureFromPathAndFill(const char* path, MeshData* myMesh)
{
	uint id_img = 0;

	if (path != nullptr)
	{
		ilGenImages(1, (ILuint*)&id_img);
		ilBindImage(id_img);

		if (ilLoadImage(path))
		{
			if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
			{
				myMesh->textureID = App->renderer3D->FillTexture(ilGetData(), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_FORMAT), path);
			}
			else LOG("ERROR: Failed converting image: %s", iluErrorString(ilGetError()));
		}
		else LOG("ERROR: Failed loading image: %s", iluErrorString(ilGetError()));
	}
	else LOG("ERROR: Failed loading image from path: %s", path);

}

uint ModuleImporter::LoadTextureFromPath(const char* path)
{
	uint textureBuffer = 0;
	uint id_img = 0;

	if (path != nullptr)
	{
		ilGenImages(1, (ILuint*)&id_img);
		ilBindImage(id_img);

		if (ilLoadImage(path))
		{
			if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
			{
				textureBuffer = App->renderer3D->FillTexture(ilGetData(), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_FORMAT), path);
			}
			else LOG("ERROR: Failed converting image: %s", iluErrorString(ilGetError()));
		}
		else LOG("ERROR: Failed loading image: %s", iluErrorString(ilGetError()));
	}
	else LOG("ERROR: Failed loading image from path: %s", path);

	return textureBuffer;
}

