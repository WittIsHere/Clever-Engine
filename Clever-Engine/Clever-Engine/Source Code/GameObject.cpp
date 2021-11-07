#include "GameObject.h"
#include "c_Transform.h"
#include "c_Mesh.h"
#include "c_Material.h"

GameObject::GameObject(char* name, GameObject* parent)
{
	toDestroy = false;
	this->name = name;
	this->parent = parent;

}

GameObject::GameObject(char* name, GameObject* parent, Component* mesh)
{
	toDestroy = false;
	this->name = name;
	this->parent = parent;
	CreateComponent(mesh->type);
}

GameObject::~GameObject()
{

}

void GameObject::Init()
{
	for (int i = 0; i < myComponents.size(); i++)
	{
		myComponents[i]->Update();
	}
}

void GameObject::Update()
{
	for (int i = 0; i < myComponents.size(); i++)
	{
		myComponents[i]->Update();
	}
}

Component* GameObject::CreateComponent(ComponentType type)
{
	Component* ret = nullptr;
	switch (type)
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
