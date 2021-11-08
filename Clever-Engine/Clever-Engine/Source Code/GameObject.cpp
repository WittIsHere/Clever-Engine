#include "GameObject.h"
#include "ModuleScene.h"	//cutre
#include "c_Transform.h"
#include "c_Mesh.h"
#include "c_Material.h"

GameObject::GameObject(char* name, GameObject* parent)
{
	if (parent == nullptr)
		isRoot = true;

	this->parent = parent;
	this->name = name;
	toDestroy = false;
}

GameObject::GameObject(char* name, GameObject* parent, MeshData* mesh)
{
	toDestroy = false;
	this->name = name;
	this->parent = parent;
	CreateComponent(mesh->type);
}

GameObject::~GameObject()
{

}

bool GameObject::Init()
{
	bool ret = false;
	//enable all components (inactive also?)
	for (int i = 0; i < myComponents.size(); i++)
	{
		if (myComponents[i]->Enable() == true)
		{
			ret = true;
		}
		
	}
	return ret;
}

bool GameObject::Update()
{
	bool ret = false;

	for (int i = 0; i < myComponents.size(); i++)
	{
		if (myComponents[i]->Update() == true)
		{
			ret = true;
		}
	}
	return ret;
}

Component* GameObject::CreateComponent(ComponentData* CD)
{
	Component* ret = nullptr;
	switch (CD.type)
	{
	case(ComponentType::TRANSFORM):
	{
		c_Transform* cmp = new c_Transform();
		myComponents.push_back((Component*)cmp);
		ret = cmp;
		break;
	}
	case(ComponentType::MATERIAL):
	{
		c_Material* cmp = new c_Material();
		myComponents.push_back((Component*)cmp);
		ret = cmp;
		break;
	}
	case(ComponentType::MESH):
	{
		c_Mesh* cmp = new c_Mesh();
		myComponents.push_back((Component*)cmp);
		ret = cmp;
		break;
	}
	}
	return ret;
}

void GameObject::AddComponent(Component* copyCmp)
{
	myComponents.push_back(copyCmp);
}

void GameObject::PrepareDraw()
{
}

void GameObject::Draw()
{
}
