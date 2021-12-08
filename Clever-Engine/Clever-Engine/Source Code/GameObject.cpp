#include "GameObject.h"
#include "Application.h"
#include "Globals.h"
#include "Random.h"
#include "ComponentData.h"
#include "MeshData.h"
#include "MaterialData.h"
#include "TransformData.h"
#include "c_Transform.h"
#include "c_Mesh.h"
#include "c_Material.h"

GameObject::GameObject(const char* name)
{
	this->name = name;
	parent = nullptr;
	isRoot = true;
	toDestroy = false;
	UUID = Random::GetRandomUint();

	//Every game object has to have a transform, so we create the compnent at the constructor
	//First initialize the data
	TransformData* data = new TransformData;
	data->position = float3::zero;
	data->rotation = Quat::identity;
	data->scale = float3(1.0f, 1.0f, 1.0f);

	//Then create the component
	this->transform = (c_Transform*)this->CreateComponent(data);
}

GameObject::GameObject(const char* name, GameObject* parent)
{
	this->name = name;
	this->parent = parent;
	toDestroy = false;
	UUID = Random::GetRandomUint();

	//Every game object has to have a transform, so we create the compnent at the constructor
	//First initialize the data
	TransformData* data = new TransformData;
	data->position = float3::zero;
	data->rotation = Quat::identity;
	data->scale = float3(1.0f, 1.0f, 1.0f);

	//Then create the component
	this->transform = (c_Transform*)this->CreateComponent(data);
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

bool GameObject::CleanUp()
{
	bool ret = true;

	DeleteAllComponents();

	DeleteAllChilds();

	return ret;
}

bool GameObject::SaveState(ParsonNode& root) const
{
	//save own parameters
	root.SetNumber("UID", UUID);
	uint parentUID = (parent != nullptr) ? parent->UUID : 0;
	root.SetNumber("ParentUID", parentUID);

	root.SetString("Name", name.c_str());

	//iterate components and save their properties
	ParsonArray componentArray = root.SetArray("Components");

	for (uint i = 0; i < myComponents.size(); ++i)
	{
		ParsonNode componentNode = componentArray.SetNode(myComponents[i]->getNameFromType());
		myComponents[i]->SaveState(componentNode);
	}
	return false;
}

bool GameObject::LoadState(ParsonNode& root)
{
	return false;
}

Component* GameObject::CreateComponent(ComponentData* CD)
{
	Component* ret = nullptr;
	switch (CD->type)
	{
	case(COMPONENT_TYPE::TRANSFORM):
	{

		c_Transform* cmp = new c_Transform(this, CD);	//create component of the corresponding type
		myComponents.push_back((Component*)cmp);		//add it to the components array
		ret = cmp;										//return it in case it needs to be modfied right away
		break;
	}
	case(COMPONENT_TYPE::MATERIAL):
	{
		c_Material* cmp = new c_Material(this, CD);
		myComponents.push_back((Component*)cmp);
		ret = cmp;
		break;
	}
	case(COMPONENT_TYPE::MESH):
	{
		c_Mesh* cmp = new c_Mesh(this, CD);
		myComponents.push_back((Component*)cmp);
		ret = cmp;
		break;
	}
	}
	return ret;
}

uint GameObject::GetComponentCount()
{
	return myComponents.size();
}

Component* GameObject::GetComponent(uint componentIndex)
{
	return myComponents[componentIndex];
}

c_Transform* GameObject::GetComponentTransform()
{
	return transform;
}

bool GameObject::DeleteComponent(Component* componentToDelete)
{

	std::string componentName = componentToDelete->getNameFromType();

	if (componentToDelete != nullptr)
	{
		for (uint i = 0; i < myComponents.size(); ++i)
		{
			if (myComponents[i] == componentToDelete)
			{
				myComponents[i]->Disable();

				RELEASE(myComponents[i]);

				myComponents.erase(myComponents.begin() + i);

				return true;
			}
		}
	}

	LOG(" Deleted Component %s of Game Object %s", componentName.c_str(), name.c_str());

	return false;
}

void GameObject::DeleteAllComponents()
{
	for (uint i = 0; i < myComponents.size(); ++i)
	{
		myComponents[i]->Disable();

		RELEASE(myComponents[i]);
	}

	if (!myComponents.empty()) 
		myComponents.clear();
}

void GameObject::AddChild(GameObject* child)
{
	myChildren.push_back(child);
}

uint GameObject::GetChildCount()
{
	return myChildren.size();
}

uint32 GameObject::GetChildUID(uint childIndex)
{
	return myChildren[childIndex]->UUID;
}

GameObject* GameObject::GetChildData(uint childIndex)
{
	return myChildren[childIndex];
}

void GameObject::DeleteAllChilds()
{
	for (uint i = 0; i < myChildren.size(); ++i)
	{
		if (myChildren[i] != nullptr)
		{
			myChildren[i]->parent = nullptr;
			myChildren[i]->toDestroy = true;											
		}
	}

	myChildren.clear();
}

GameObject* GameObject::GetParent()
{
	return this->parent;
}

void GameObject::Draw()
{
	for (int i = 0; i < myComponents.size(); i++)
	{
		if (myComponents[i]->type == COMPONENT_TYPE::MESH)
		{
			c_Mesh* cmp = (c_Mesh*)myComponents[i];
			App->renderer3D->DrawMesh(cmp, this->transform);
		}
	}
}