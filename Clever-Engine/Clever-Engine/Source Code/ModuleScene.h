#pragma once
#include "Module.h"
#include "Globals.h"

#include <vector>

class GameObject;

struct meshData
{
	uint vertexCount = 0;				// Number of Vertex

	uint vPosID = 0;					// Vertex Positions Buffer ID
	float* vPosData = nullptr;			// Vertex Positions Array

	uint vTexCoordsID = 0;				// Vertex Texture Coords Buffer ID
	float* vTexCoordsData = nullptr;	// Vertex Texture Coords Array

	//uint vNormID = 0;					// Vertex Normals Buffer ID
	//float* vNormData = nullptr;		// Vertex Normals Array

	uint indicesID = 0;					// Indices Buffer ID
	uint indicesCount = 0;				// Number of Indices
	uint* indicesData = nullptr;		// Indices Array

	c_Material* texture = 0;			// Pointer to acess the texture inside myTexures() vector
};

struct textureData
{
	const char* path;
	uint textureID;
};

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Init();
	bool Start();
	update_status Update(float ft);
	bool CleanUp();

	GameObject* CreateGameObject();

public:
	std::vector<GameObject*> gameObjects;

	std::vector<meshData*> meshPool;
	std::vector<textureData*> texturePool;
};