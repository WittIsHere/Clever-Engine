#pragma once

#include "Globals.h"
#include "Component.h"
#include "c_Mesh.h"
#include "c_Transform.h"
#include "JSONParser.h"

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
	bool CleanUp();

	bool SaveState(ParsonNode& root) const;
	bool LoadState(ParsonNode& root);

	Component* CreateComponent(ComponentData* CD);
	uint GetComponentCount();
	Component* GetComponent(uint componentIndex);
	c_Transform* GetComponentTransform();
	bool DeleteComponent(Component* componentToDelete);
	void DeleteAllComponents();

	void AddChild(GameObject* child); 
	uint GetChildCount();
	uint32 GetChildUID(uint childIndex);
	GameObject* GetChildData(uint childIndex);
	void DeleteAllChilds();

	GameObject* GetParent();
	void Draw(); 

public:

	std::string name;
	bool isRoot = false;
	bool isActive = true;
	bool toDestroy = false;
	uint32 UUID = 0;

private:
	std::vector<GameObject*> myChildren;

	GameObject* parent;
	std::vector<Component*> myComponents;
	c_Transform* transform; //direct access to the component transform.
};