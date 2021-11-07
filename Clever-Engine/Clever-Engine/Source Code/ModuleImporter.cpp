#include "Globals.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "c_Mesh.h"
#include "c_Material.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

//#include "Devil/include/il.h"			// not necessary, ilut.h includes it
#pragma comment( lib, "Devil/libx86/DevIL.lib" )
//#include "Devil/include/ilu.h"		// not necessary, ilut.h includes it
#pragma comment( lib, "Devil/libx86/ILU.lib" )
#include "Devil/include/ilut.h"
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
	ImportScene(fbxPath);

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
				c_Mesh* tempMesh = new c_Mesh;
				myScene.myMeshes.push_back(tempMesh);
				aiMesh* currentAiMesh = aiScene->mMeshes[i];			

				ImportMesh(currentAiMesh, tempMesh);

				uint tempIndex = currentAiMesh->mMaterialIndex;
				if (tempIndex >= 0)
				{
					aiMaterial* currentMaterial = aiScene->mMaterials[currentAiMesh->mMaterialIndex]; //access the mesh material using the mMaterialIndex
					aiString texPath;
					if (currentMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS)
					{
						std::string fullPath = ASSETS_PATH;
						fullPath += texPath.C_Str();
						LoadTextureFromPathAndFill(fullPath.c_str(), myScene.myMeshes[i]);
					}
				}
			}
		}
		else
		{
			LOG("Error loading meshes, scene % s", file_path);
		}

		aiReleaseImport(aiScene);
	}
}

void ModuleImporter::ImportMesh(aiMesh* mesh, c_Mesh* myMesh)
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
	App->renderer3D->PrepareDrawMesh(myMesh);

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

void ModuleImporter::LoadTextureFromPathAndFill(const char* path, c_Mesh* myMesh)
{
	uint textureBuffer = 0;
	ILuint id_img = 0;
	ILenum error;

	if (path != nullptr)
	{
		ilGenImages(1, (ILuint*)&id_img);
		ilBindImage(id_img);

		error = ilGetError();
		if (error)
			LOG("ERROR: Failed generating/binding image: %s", iluErrorString(error));

		if (ilLoad(IL_PNG, path)) //or ilLoadImage(path) which behaves the same
		{
			uint tempTextureID = ilutGLBindTexImage();	//bind texture to openGL and get the ID
		
			error = ilGetError();
			if (error)
				LOG("ERROR: Failed binding the DevIL Texture with OpenGl: %s", iluErrorString(error));

			c_Material* texData = new c_Material;
			myScene.myTextures.push_back(texData);	//Add a new texture to the textures array

			texData->path = path;	//assign the new texture its path
			texData->textureID = tempTextureID;	//assign the new texture its ID

			myMesh->texture = texData;	
		}
		else LOG("ERROR: Failed loading image: %s", iluErrorString(ilGetError()));

		ilDeleteImages(1, &id_img); // Because we have already copied image data into texture data we can release memory used by image.
	}
	else LOG("ERROR: Failed loading image from path: %s", path);
}

//PREVIOUS TEXTURE LOADING METHOD ---------------
// 
//void ModuleImporter::LoadTextureFromPathAndFill(const char* path, MeshData* myMesh)
//{
//	uint textureBuffer = 0;
//	ILuint id_img = 0;
//	ILenum error;
//
//	if (path != nullptr)
//	{
//		ilGenImages(1, (ILuint*)&id_img);
//		ilBindImage(id_img);
//
//		if (ilLoadImage(path))
//		{
//			if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
//			{
//				myMesh->textureID = App->renderer3D->FillTexture(ilGetData(), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_FORMAT), path);
//			}
//			else LOG("ERROR: Failed converting image: %s", iluErrorString(ilGetError()));
//		}
//		else LOG("ERROR: Failed loading image: %s", iluErrorString(ilGetError()));
//	}
//	else LOG("ERROR: Failed loading image from path: %s", path); 
//}

uint ModuleImporter::LoadTextureFromPath(const char* path)
{
	uint textureBuffer = 0;
	ILuint id_img = 0;
	ILenum error;

	if (path != nullptr)
	{
		ilGenImages(1, (ILuint*)&id_img);
		ilBindImage(id_img);

		error = ilGetError();

		if (error)
			LOG("ERROR: Failed generating/binding image: %s", iluErrorString(error));

		if (ilLoad(IL_TYPE_UNKNOWN, path)) 
		{
			textureBuffer = ilutGLBindTexImage();

		}
		else LOG("ERROR: Failed loading image: %s", iluErrorString(ilGetError()));

		ilDeleteImages(1, &id_img); // Because we have already copied image data into texture data we can release memory used by image.
	}

	return textureBuffer;
}
