#include "Globals.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ModuleFileSystem.h"
#include "PathNode.h"
#include "ResourceBase.h"
#include "ResourceMesh.h"

#include "c_Mesh.h"
#include "MeshData.h"
#include "c_Material.h"
#include "MaterialData.h"
#include "c_Transform.h"
#include "TransformData.h"
#include "model.h"

#include "Random.h"
#include <map>
#include <functional>
#include <vector>

#include <fstream> // Added this

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
	const aiScene* aiScene = aiImportFile(file_path, 0);
	if (aiScene != nullptr)
	{
		if (aiScene->mRootNode != nullptr)
		{
			LoadRoot(aiScene->mRootNode, aiScene, file_path);
		}
	}
}

//TODO: make this method be of type myScene and return the loaded scene
void ModuleImporter::ImportScene2(const char* file_path)
{
	const aiScene* aiScene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (aiScene != nullptr)
	{
		if (aiScene->mRootNode != nullptr)
		{
			//TODO: separate filename from path and extension
			//LoadRoot2(aiScene->mRootNode, aiScene, file_path);
			LoadRoot(aiScene->mRootNode, aiScene, file_path);
		}
	}
}

void ModuleImporter::ImportMesh(aiMesh* mesh, ResourceMesh* myMesh)
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
		std::string GOname;
		App->fileSystem->SplitFilePathInverse(fileName, &GOname);
		GameObject* GO = App->scene->CreateGameObject(GOname.c_str(), App->scene->rootNode);
		App->scene->rootNode->AddChild(GO);
		for (int i = 0; i < sceneRoot->mNumChildren; i++)
		{
			LoadNode(GO, sceneRoot->mChildren[i], currentScene, fileName);
		}
	}
	else LOG("ERROR: Trying to load empty scene!");
}

void ModuleImporter::LoadNode(GameObject* parent, aiNode* currentNode, const aiScene* currentScene, const char* path)
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

			uint currentAiMeshIndex = currentNode->mMeshes[i];
			aiMesh* currentAiMesh = currentScene->mMeshes[currentAiMeshIndex]; //find the correct mesh at the scene with the index given by mMeshes array

			std::string finalName = MESHES_PATH;
			finalName = finalName + currentNode->mName.C_Str();

			TMYMODEL* myModel = new TMYMODEL();
	/*		for (size_t i = 0; i < currentAiMesh->mNumVertices; i++)
			{
				myModel->textCoords[i] = 0.0f;
			}*/

			uint32 UID = 0;
			UID = callCreateAndSave(currentAiMesh, finalName.c_str(), myModel, path);

			RELEASE(myModel);

			Resource* myRes = App->resources->GetResource(UID);

			//App->scene->meshPool.push_back(tempMesh);

			GO->CreateComponent(myRes);

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

						//tempMesh->texture = texData;	//TODO assign the texture to the current mesh
					}
				}
			}
		}
	}
	if (currentNode->mNumChildren > 0)
	{
		for (int i = 0; i < currentNode->mNumChildren; i++)
		{
			LoadNode(GO, currentNode->mChildren[i], currentScene, path);
		}
	}
}

void ModuleImporter::LoadRoot2(aiNode* sceneRoot, const aiScene* currentScene, const char* fileName)
{
	if (sceneRoot->mNumChildren > 0)
	{
		for (int i = 0; i < sceneRoot->mNumChildren; i++)
		{
			LoadNode2(sceneRoot->mChildren[i], currentScene, fileName);
		}
	}
	else LOG("ERROR: Trying to load empty scene!");
}

void ModuleImporter::LoadNode2(aiNode* currentNode, const aiScene* currentScene, const char* fileName)
{
	if (currentNode->mNumMeshes > 0)
	{
		for (int i = 0; i < currentNode->mNumMeshes; i++)
		{
			uint currentAiMeshIndex = currentNode->mMeshes[i];
			aiMesh* currentAiMesh = currentScene->mMeshes[currentAiMeshIndex]; //find the correct mesh at the scene with the index given by mMeshes array

			std::string finalName = MESHES_PATH;
			finalName = finalName + currentNode->mName.C_Str();

			TMYMODEL* myModel = new TMYMODEL();
			callCreateAndSave(currentAiMesh, finalName.c_str(), myModel, fileName);
			RELEASE(myModel);
		}
	}
	if (currentNode->mNumChildren > 0)
	{
		for (int i = 0; i < currentNode->mNumChildren; i++)
		{
			LoadNode2(currentNode->mChildren[i], currentScene, fileName);
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
			ILinfo ImageInfo; 
			iluGetImageInfo(&ImageInfo);
			if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT) 
				iluFlipImage();

			textureBuffer = ilutGLBindTexImage();

		}
		else LOG("ERROR: Failed loading image: %s", iluErrorString(ilGetError()));

		ilDeleteImages(1, &id_img); // Because we have already copied image data into texture data we can release memory used by image.
	}

	return textureBuffer;
}

void ModuleImporter::ImportToCustomFF(const char* libPath)
{
	const aiScene* aiScene = aiImportFile(libPath, aiProcessPreset_TargetRealtime_MaxQuality);
	if (aiScene != nullptr)
	{
		if (aiScene->mRootNode != nullptr)
		{
			//TODO: separate filename from path and extension
			LoadRoot2(aiScene->mRootNode, aiScene, libPath);
		}
	}

}

uint32 ModuleImporter::callCreateAndSave(const aiMesh* mesh, const char* path, TMYMODEL* myModel, const char* assetsPath)
{
	myModel = createMyModel(mesh);

	if (SaveModel(myModel, path) == true)
	{
		LOG("%s Saved correctly", path);
	}
	else
	{
		LOG("%s Saved INCORRECTLY", path);
	}
	
	uint32 uuid = Random::GetRandomUint();

	ResourceBase temp = *(new ResourceBase(uuid, assetsPath, path, ResourceTypes::R_MESH));

	App->resources->library.emplace(uuid, temp);

	return uuid;
}

TMYMODEL* ModuleImporter::createMyModel(const aiMesh* m)
{
	TMYMODEL* mymodel = (TMYMODEL*)malloc(sizeof(TMYMODEL));

	mymodel->verticesSizeBytes = m->mNumVertices * sizeof(float) * 3;//3==x,y,z
	mymodel->vertices = (float*)malloc(mymodel->verticesSizeBytes);
	memcpy(mymodel->vertices, m->mVertices, mymodel->verticesSizeBytes);

	mymodel->normalsSizeBytes = m->mNumVertices * sizeof(float) * 3;//3==x,y,z equal vertex
	mymodel->normals = (float*)malloc(mymodel->normalsSizeBytes);
	memcpy(mymodel->normals, m->mNormals, mymodel->normalsSizeBytes);
	if (m->HasTextureCoords(1))
	{
		for (size_t j = 0; j < m->mNumVertices; ++j)
		{
			mymodel->textCoordSizeBytes = m->mNumVertices * sizeof(float) * 2;//3==u,v
			mymodel->textCoords = (float*)malloc(mymodel->textCoordSizeBytes);
			for (int i = 0; i < m->mNumVertices; i++)
			{
				*(mymodel->textCoords + i * 2) = m->mTextureCoords[0][i].x;
				*(mymodel->textCoords + i * 2 + 1) = 1.0 - m->mTextureCoords[0][i].y; //this coord image is inverted
			}
		}
	}
	else
	{
		mymodel->textCoordSizeBytes = 0;
	}



	//mymodel->textCoordSizeBytes = m->mNumVertices * sizeof(float) * 2;//3==u,v
	//mymodel->textCoords = (float*)malloc(mymodel->textCoordSizeBytes);
	//for (int i = 0; i < m->mNumVertices; i++)
	//{
	//	*(mymodel->textCoords + i * 2) = m->mTextureCoords[0][i].x;
	//	*(mymodel->textCoords + i * 2 + 1) = 1.0 - m->mTextureCoords[0][i].y; //this coord image is inverted
	//}


	mymodel->indiceSizeBytes = m->mNumFaces * sizeof(unsigned) * 3; //3==indices/face
	mymodel->indices = (unsigned*)malloc(mymodel->indiceSizeBytes);
	for (int i = 0; i < m->mNumFaces; i++)
	{
		aiFace* f = m->mFaces + i;
		*(mymodel->indices + 0 + i * 3) = f->mIndices[0];
		*(mymodel->indices + 1 + i * 3) = f->mIndices[1];
		*(mymodel->indices + 2 + i * 3) = f->mIndices[2];
	}

	//const char* someinfo = "This is a great model for my engine. Author: Juan"; // '\0' OjO
	//mymodel->infoSizeBytes = 128; //or str length
	//mymodel->info = (char*)malloc(mymodel->infoSizeBytes);
	//memcpy(mymodel->info, someinfo, mymodel->infoSizeBytes);
	//mymodel->info[mymodel->infoSizeBytes - 1] = '\0';

	return mymodel;

}

bool ModuleImporter::SaveModel(const TMYMODEL* m, const char* path)
{
	//My format
	//Header
	//a)unsigned for  verticesSizeBytes
	//b)unsigned for   normalsSizeBytes
	//c)unsigned for textCoordSizeBytes
	//d)unsigned for    indiceSizeBytes
	//e)unsigned for      infoSizeBytes
	//so the header size is 5 * sizeof(unsigned) -> 20 bytes

	std::ofstream myfile;
	myfile.open(path, std::ios::in | std::ios::app | std::ios::binary);
	if (myfile.is_open())
	{
		myfile.write((char*)m, 5 * sizeof(unsigned)); //write header

		myfile.write((char*)m->vertices, m->verticesSizeBytes);
		myfile.write((char*)m->normals, m->normalsSizeBytes);
	/*	if (m->textCoordSizeBytes != 0)
		{
			myfile.write((char*)m->textCoords, m->textCoordSizeBytes);
		}*/
		myfile.write((char*)m->textCoords, m->textCoordSizeBytes);
		myfile.write((char*)m->indices, m->indiceSizeBytes);
		//myfile.write((char*)m->info, m->infoSizeBytes); // xx

		myfile.close();

		return true;
	}
	else
	{
		return false;
	}

}

//TMYMODEL* LoadModel(const char* path)
//{
//	std::ifstream myfile;
//	myfile.open(path, std::ios::binary);
//	if (myfile.is_open())
//	{
//
//		TMYMODEL* mymodel = (TMYMODEL*)malloc(sizeof(TMYMODEL));
//		myfile.read((char*)mymodel, 5 * sizeof(unsigned)); //READ HEADER
//
//		mymodel->vertices = (float*)malloc(mymodel->verticesSizeBytes);
//		myfile.read((char*)mymodel->vertices, mymodel->verticesSizeBytes);
//
//		mymodel->normals = (float*)malloc(mymodel->normalsSizeBytes);
//		myfile.read((char*)mymodel->normals, mymodel->normalsSizeBytes);
//
//		mymodel->textCoords = (float*)malloc(mymodel->textCoordSizeBytes);
//		myfile.read((char*)mymodel->textCoords, mymodel->textCoordSizeBytes);
//
//		mymodel->indices = (unsigned*)malloc(mymodel->indiceSizeBytes);
//		myfile.read((char*)mymodel->indices, mymodel->indiceSizeBytes);
//
//		mymodel->info = (char*)malloc(mymodel->infoSizeBytes);
//		myfile.read(mymodel->info, mymodel->infoSizeBytes);
//
//		myfile.close();
//		return mymodel;
//	}
//	else
//	{
//		return NULL;
//	}
//}

bool ModuleImporter::LoadModel(const char* path, ResourceMesh* mesh)
{
	std::ifstream myfile;
	myfile.open(path, std::ios::binary);

	if (myfile.is_open())
	{
		TMYMODEL* mymodel = (TMYMODEL*)malloc(sizeof(TMYMODEL));
		myfile.read((char*)mymodel, 5 * sizeof(unsigned)); //READ HEADER

		mymodel->vertices = (float*)malloc(mymodel->verticesSizeBytes);
		myfile.read((char*)mymodel->vertices, mymodel->verticesSizeBytes);

		mymodel->normals = (float*)malloc(mymodel->normalsSizeBytes);
		myfile.read((char*)mymodel->normals, mymodel->normalsSizeBytes);
		//if (mymodel->textCoordSizeBytes != 0)
		//{
		//	mymodel->textCoords = (float*)malloc(mymodel->textCoordSizeBytes);
		//	myfile.read((char*)mymodel->textCoords, mymodel->textCoordSizeBytes);
		//}
		mymodel->textCoords = (float*)malloc(mymodel->textCoordSizeBytes);
		myfile.read((char*)mymodel->textCoords, mymodel->textCoordSizeBytes);

		mymodel->indices = (unsigned*)malloc(mymodel->indiceSizeBytes);
		myfile.read((char*)mymodel->indices, mymodel->indiceSizeBytes);

		//mymodel->info = (char*)malloc(mymodel->infoSizeBytes);
		//myfile.read(mymodel->info, mymodel->infoSizeBytes);

		myfile.close();

		mesh->indicesData = mymodel->indices;
		mesh->vNormData = mymodel->normals;
		mesh->vTexCoordsData = mymodel->textCoords;
		mesh->vPosData = mymodel->vertices;

		mesh->vertexCount = (mymodel->verticesSizeBytes / (sizeof(float) * 3));
		mesh->indicesCount = (mymodel->indiceSizeBytes / sizeof(int));

		App->renderer3D->PrepareMesh(mesh);

		LOG("SUCCESS loading %s", path);
	}
	else
	{
		LOG("FAILED loading %s", path);
		return NULL;
	}
}

MeshData* ModuleImporter::LoadModel(const char* path)
{
	MeshData* loadedMesh = new MeshData();

	std::ifstream myfile;
	myfile.open(path, std::ios::binary);

	if (myfile.is_open())
	{
		TMYMODEL* mymodel = (TMYMODEL*)malloc(sizeof(TMYMODEL));
		myfile.read((char*)mymodel, 5 * sizeof(unsigned)); //READ HEADER

		mymodel->vertices = (float*)malloc(mymodel->verticesSizeBytes);
		myfile.read((char*)mymodel->vertices, mymodel->verticesSizeBytes);

		mymodel->normals = (float*)malloc(mymodel->normalsSizeBytes);
		myfile.read((char*)mymodel->normals, mymodel->normalsSizeBytes);

		mymodel->textCoords = (float*)malloc(mymodel->textCoordSizeBytes);
		myfile.read((char*)mymodel->textCoords, mymodel->textCoordSizeBytes);

		mymodel->indices = (unsigned*)malloc(mymodel->indiceSizeBytes);
		myfile.read((char*)mymodel->indices, mymodel->indiceSizeBytes);

		//mymodel->info = (char*)malloc(mymodel->infoSizeBytes);
		//myfile.read(mymodel->info, mymodel->infoSizeBytes);

		myfile.close();


		loadedMesh->indicesData = mymodel->indices;
		loadedMesh->vNormData = mymodel->normals;
		loadedMesh->vTexCoordsData = mymodel->textCoords;
		loadedMesh->vPosData = mymodel->vertices;

		loadedMesh->vertexCount = (mymodel->verticesSizeBytes / (sizeof(float) * 3));
		loadedMesh->indicesCount = (mymodel->indiceSizeBytes / sizeof(int));

		App->renderer3D->PrepareMesh(loadedMesh);

		LOG("SUCCESS loading %s", path);
		return loadedMesh;
	}
	else
	{
		LOG("FAILED loading %s", path);
		return NULL;
	}
}