#pragma once

#include "Globals.h"
#include "Component.h"
#include "c_Mesh.h"
#include "c_Transform.h"

#include <vector>
#include <string>

class MeshData;
class ComponentData;

class GameObject
{
public:
	GameObject(const char* name);	//If no parent is specified, then GO is set to be the root 
	GameObject(const char* name, GameObject* parent); 

	~GameObject();

	// Methods

	bool Init();
	bool Update();

	Component* CreateComponent(ComponentData* CD);
	bool DeleteComponent(Component* componentToDelete);
	//bool DeleteAllComponents();

	uint GetComponentCount();
	Component* GetComponent(uint componentIndex);
	c_Transform* GetComponentTransform();

	void AddChild(GameObject* child); //copy an existing "component"
	uint GetChildCount();
	uint32 GetChildUID(uint childIndex);
	GameObject* GetChildData(uint childIndex);

	GameObject* GetParent();
	void Draw(); 

public:

	std::string name;
	bool isRoot = false;
	bool isActive = true;
	bool toDestroy = false;
	int UUID = 0;

private:
	std::vector<GameObject*> myChildren;

	GameObject* parent;
	std::vector<Component*> myComponents;
	c_Transform* transform; //direct access to the component transform.
};