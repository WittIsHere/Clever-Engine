#include "GameObject.h"
#include "Application.h"
#include "Globals.h"
#include "ComponentData.h"
#include "MeshData.h"
#include "MaterialData.h"
#include "c_Transform.h"
#include "c_Mesh.h"
#include "c_Material.h"

GameObject::GameObject(const char* name)
{
	this->name = *name;
	parent = nullptr;
	isRoot = true;
	toDestroy = false;
}

GameObject::GameObject(const char* name, GameObject* parent)
{
	this->name = *name;
	this->parent = parent;
	toDestroy = false;
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
	switch (CD->type)
	{
	case(COMPONENT_TYPE::TRANSFORM):
	{
		c_Transform* cmp = new c_Transform();
		myComponents.push_back((Component*)cmp);
		ret = cmp;
		break;
	}
	case(COMPONENT_TYPE::MATERIAL):
	{
		c_Material* cmp = new c_Material();
		myComponents.push_back((Component*)cmp);
		ret = cmp;
		break;
	}
	case(COMPONENT_TYPE::MESH):
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

void GameObject::Draw()
{
	for (int i = 0; i < myComponents.size(); i++)
	{
		if(myComponents[i]->type == COMPONENT_TYPE::MESH)
			App->renderer3D->DrawMesh((MeshData*)myComponents[i]);
	}
}
