#pragma once

#include "Globals.h"
#include "Component.h"
#include "c_Mesh.h"

#include <vector>
#include <string>

class MeshData;
class ComponentData;

class GameObject
{
public:
	GameObject(const char* name);	//If no parent specified, then GO is set to be the root 
	GameObject(const char* name, GameObject* parent);
	~GameObject();

	// Methods

	bool Init();
	bool Update();

	Component* CreateComponent(ComponentData* CD);

	void AddComponent(Component* copyCmp); //copy an existing "component"

	void Draw(); 

public:

	std::string name;
	bool isRoot = false;
	bool isActive = true;
	bool toDestroy = false;

private:

	GameObject* parent;
	std::vector<GameObject*> myChilds;
	
	std::vector<Component*> myComponents;
};