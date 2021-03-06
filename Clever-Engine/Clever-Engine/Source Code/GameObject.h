#pragma once
#define NOMINMAX 1
#include "Globals.h"
#include "Component.h"
#include "c_Mesh.h"
#include "c_Transform.h"
#include "c_Camera.h"
#include "JSONParser.h"

#include <vector>
#include <string>

class MeshData;
class ComponentData;
class c_Material;

class GameObject
{
public:
	GameObject(const char* name = "GameObject");	//If no parent is specified, then GO is set to be the root 
	GameObject(const char* name, GameObject* parent); 

	~GameObject();

	// Methods

	bool Init();
	bool Update(float dt);
	bool CleanUp();


	bool SaveState(ParsonNode& root) const;
	bool LoadState(ParsonNode& root);

public:
	//components
	Component* CreateComponent(Resource* CD);
	Component* CreateComponent(COMPONENT_TYPE type);

	uint GetComponentCount();
	Component* GetComponent(uint componentIndex);
	Component* GetComponentByType(COMPONENT_TYPE type);
	c_Transform* GetComponentTransform();
	c_Material* GetComponentMaterial();
	bool DeleteComponent(Component* componentToDelete);
	void DeleteAllComponents();

	//childs
	void AddChild(GameObject* child); 
	uint GetChildCount();
	uint32 GetChildUID(uint childIndex);
	GameObject* GetChildData(uint childIndex);

	void DeleteChild(uint childIndex);
	void DeleteChildFromArray(GameObject* GO);
	void DeleteAllChilds();

	GameObject* GetParent();
	uint GetParentUID();
	void UpdateParent();
	void ForceUID(uint32 id);
	void Draw(); 

	bool IsCamera();

	std::vector<Component*>GetComponentsVector() { return myComponents; };

public:
	bool hasMesh;
	bool isCamera = false;
	std::string name;
	bool isRoot = false;
	bool isActive = true;
	bool insideFrustum = false;
	bool toDestroy = false;
	uint32 parentID = 0;
	uint32 UUID = 0;

private:
	std::vector<GameObject*> myChildren;

	GameObject* parent;
	std::vector<Component*> myComponents;
	c_Transform* transform; //direct access to the component transform.
	c_Material* material; //direct access to the component material
};