#include "Globals.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "ModuleScene.h"
#include "GameObject.h"

#include "c_Mesh.h"
#include "MeshData.h"
#include "c_Material.h"
#include "MaterialData.h"
#include "c_Transform.h"
#include "TransformData.h"

#include "Assimp/include/mesh.h"
#include "Assimp/include/material.h"
#include "Assimp/include/texture.h"
#include "Assimp/include/matrix4x4.h"
#include "Assimp/include/vector3.h"
#include "Assimp/include/quaternion.h"

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
		if (aiScene->mRootNode != nullptr)
		{
			//TODO: separate filename from path and extension
			LoadRoot(aiScene->mRootNode, aiScene, file_path);
		}
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
	App->renderer3D->PrepareMesh(myMesh);

	// Copying Normals
	if (mesh->HasNormals())
	{
		myMesh->vNormData = new float[myMesh->vertexCount * 3];

		for (uint i = 0; i < myMesh->vertexCount; i++)
		{
			memcpy(&myMesh->vNormData[(i * 3)], &mesh->mNormals[i].x, sizeof(float));
			memcpy(&myMesh->vNormData[(i * 3) + 1], &mesh->mNormals[i].y, sizeof(float));
			memcpy(&myMesh->vNormData[(i * 3) + 2], &mesh->mNormals[i].z, sizeof(float));
		}
	}
	else
	{
		LOG("Warning, no normals found");
	}
}

void ModuleImporter::LoadRoot(aiNode* sceneRoot, const aiScene* currentScene, const char* fileName)
{
	if (sceneRoot->mNumChildren > 0)
	{
		GameObject* GO = App->scene->CreateGameObject(fileName, App->scene->rootNode);
		App->scene->rootNode->AddChild(GO);
		for (int i = 0; i < sceneRoot->mNumChildren; i++)
		{
			LoadNode(GO, sceneRoot->mChildren[i], currentScene);
		}
	}
	else LOG("ERROR: Trying to load empty scene!");
}

void ModuleImporter::LoadNode(GameObject* parent, aiNode* currentNode, const aiScene* currentScene)
{
	GameObject* GO = App->scene->CreateGameObject(currentNode->mName.C_Str(), parent);
	parent->AddChild(GO); //add child to parent->myChildren to complete hierarchy

	aiVector3D aiScale;
	aiQuaternion aiRotation;
	aiVector3D aiTranslation;
	currentNode->mTransformation.Decompose(aiScale, aiRotation, aiTranslation);							// --- Getting the Transform stored in the node.

	c_Transform* transform = GO->GetComponentTransform();

	transform->SetLocalPosition({ aiTranslation.x, aiTranslation.y, aiTranslation.z });
	transform->SetLocalRotation({ aiRotation.x, aiRotation.y, aiRotation.z, aiRotation.w });					 
	transform->SetLocalScale({ aiScale.x, aiScale.y, aiScale.z });

	if (currentNode->mNumMeshes > 0) 
	{
		for (int i = 0; i < currentNode->mNumMeshes; i++)
		{
			//create empty meshData and add it to the array
			MeshData* tempMesh = new MeshData();
			App->scene->meshPool.push_back(tempMesh);

			uint currentAiMeshIndex = currentNode->mMeshes[i];
			aiMesh* currentAiMesh = currentScene->mMeshes[currentAiMeshIndex]; //find the correct mesh at the scene with the index given by mMeshes array

			//import the data into the struct
			ImportMesh(currentAiMesh, tempMesh);

			GO->CreateComponent((ComponentData*)tempMesh);

			uint tempIndex = currentAiMesh->mMaterialIndex;
			if (tempIndex >= 0)
			{
				//in case there is a texture add the component texture to the previous GO
				aiMaterial* currentMaterial = currentScene->mMaterials[currentAiMesh->mMaterialIndex]; //access the mesh material using the mMaterialIndex
				aiString texPath;
				if (currentMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS)
				{
					std::string fullPath = ASSETS_PATH;
					fullPath += texPath.C_Str();

					uint tempTextureID = LoadTextureFromPath(fullPath.c_str());

					if (tempTextureID > 0)
					{
						TextureData* texData = new TextureData();
						texData->type = COMPONENT_TYPE::MATERIAL;
						App->scene->texturePool.push_back(texData);	//Add a new texture to the textures array

						texData->path = texPath.C_Str();	//assign the new texture its path
						texData->textureID = tempTextureID;	//assign the new texture its ID

						GO->CreateComponent((ComponentData*)texData);

						tempMesh->texture = texData;	//assign the texture to the current mesh
					}
				}
			}
		}
	}
	if (currentNode->mNumChildren > 0)
	{
		for (int i = 0; i < currentNode->mNumChildren; i++)
		{
			LoadNode(GO, currentNode->mChildren[i], currentScene);
		}
	}
}

void ModuleImporter::LoadTextureFromPathAndFill(const char* path, MeshData* myMesh)
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

			//TODO: check for duplicates?

			TextureData* texData = new TextureData();
			App->scene->texturePool.push_back(texData);	//Add a new texture to the textures array

			texData->path = path;	//assign the new texture its path
			texData->textureID = tempTextureID;	//assign the new texture its ID

			myMesh->texture = texData;	//assign the texture to the current mesh
		}
		else LOG("ERROR: Failed loading image: %s", iluErrorString(ilGetError()));

		ilDeleteImages(1, &id_img); // Because we have already copied image data into texture data we can release memory used by image.
	}
	else LOG("ERROR: Failed loading image from path: %s", path);
}

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
