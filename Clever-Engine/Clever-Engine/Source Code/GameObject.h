#pragma once

#include "Globals.h"
#include "Component.h"
#include "c_Mesh.h"

#include <vector>

class MeshData;

class GameObject
{
public:
	GameObject(char* name, GameObject* parent);//If prent is null then GO is set to be the root 
	//GameObject(char* name, GameObject* parent, MeshData* CD);
	~GameObject();

	// Methods

	bool Init();
	bool Update();

	Component* CreateComponent(ComponentType type, ComponentData* data);

	void AddComponent(Component* copyCmp); //copy an existing "component"

	void PrepareDraw();
	void Draw();  // ??

public:

	char* name = nullptr;
	bool isRoot = false;
	bool isActive = true;
	bool toDestroy = false;

private:

	GameObject* parent;
	std::vector<GameObject*> myChilds;
	
	std::vector<Component*> myComponents;
};