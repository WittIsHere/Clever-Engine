#define NOMINMAX 1
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
#include "c_Camera.h"
#include "c_ParticleSystem.h"
#include "SDL/include/SDL_opengl.h"

GameObject::GameObject(const char* name)
{
	this->name = name;
	this->parentID = 0;
	isRoot = false;
	toDestroy = false;
	hasMesh = false;
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
	this->parentID = parent->UUID;
	this->parent = parent;
	toDestroy = false;
	hasMesh = false;
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

bool GameObject::Update(float dt)
{
	bool ret = false;

	for (int i = 0; i < myComponents.size(); i++)
	{
		if (myComponents[i]->Update(dt) == true)
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

bool GameObject::SaveState(ParsonNode& root) const //set all variables inside the node
{
	//save own parameters
	root.SetNumber("UID", UUID);
	root.SetNumber("ParentUID", parentID);

	root.SetString("Name", name.c_str());
	root.SetBool("IsActive", isActive);
	//root.SetBool("IsStatic", isStatic);
	root.SetBool("IsSceneRoot", isRoot);

	//iterate components and save their properties
	ParsonArray componentArray = root.SetArray("Components");

	for (uint i = 0; i < myComponents.size(); ++i)
	{
		const char* cName = myComponents[i]->getNameFromType();
		ParsonNode componentNode = componentArray.SetNode(cName);
		myComponents[i]->SaveState(componentNode);
	}
	return true;
}

bool GameObject::LoadState(ParsonNode& root) //load from the node all the variables and assign tbem to the 
{
	ForceUID((uint)root.GetNumber("UID"));
	parentID = (uint)root.GetNumber("ParentUID"); 
	UpdateParent();

	name = root.GetString("Name");
	isActive = root.GetBool("IsActive");
	//isStatic = root.GetBool("IsStatic");
	isRoot = root.GetBool("IsSceneRoot");

	ParsonArray componentsArray = root.GetArray("Components");
	for (uint i = 0; i < componentsArray.size; ++i)
	{
		ParsonNode componentNode = componentsArray.GetNode(i);
		if (!componentNode.NodeIsValid())
		{
			continue;
		}

		COMPONENT_TYPE type = (COMPONENT_TYPE)((int)componentNode.GetNumber("Type"));
		Component* component = nullptr;

		if (type == COMPONENT_TYPE::TRANSFORM)			//a transform component is always created at the constructor of a GO
			component = transform;
		else
			component = CreateComponent(type);

		if (component != nullptr)
		{
			if (component->LoadState(componentNode))
			{
				component->Enable();

				/*if (component->GetType() == ComponentType::CAMERA && App->gameState == GameState::PLAY)
				{
					App->camera->SetCurrentCamera((C_Camera*)component);
				}*/
			}
			else
			{
				LOG("[WARNING] Game Object: Could not Load State of Component { %s } of Game Object { %s }!", component->getNameFromType(), name.c_str());

				component->Disable();
				delete component;
			}
		}
	}

	return true;
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
		cmp->Enable();
		ret = cmp;										//return it in case it needs to be modfied right away
		break;
	}
	case(COMPONENT_TYPE::MATERIAL):
	{
		c_Material* cmp = new c_Material(this, CD);
		myComponents.push_back((Component*)cmp);
		cmp->Enable();
		ret = cmp;
		break;
	}
	//case(COMPONENT_TYPE::MESH):
	//{
	//	c_Mesh* cmp = new c_Mesh(this, CD);
	//	myComponents.push_back((Component*)cmp);
	//	cmp->Enable();
	//	ret = cmp;
	//	break;
	//}
	case(COMPONENT_TYPE::CAMERA):
	{
		c_Camera* cmp = new c_Camera(this, CD);
		myComponents.push_back((Component*)cmp);
		cmp->Enable();
		ret = cmp;
		hasMesh = true;
		break;
	}
	}
	return ret;
}

Component* GameObject::CreateComponent(Resource* CD)
{
	Component* ret = nullptr;
	if (CD != NULL)
	{
		switch (CD->GetType())
		{	
		case(ResourceType::MESH):
		{
			c_Mesh* cmp = new c_Mesh(this, CD);
			myComponents.push_back((Component*)cmp);
			cmp->Enable();
			hasMesh = true;
			ret = cmp;
		}
			break;
		}
	}
	return ret;
}

Component* GameObject::CreateComponent(COMPONENT_TYPE type)
{
	Component* ret = nullptr;

	switch (((int)type))
	{
	case(COMPONENT_TYPE::TRANSFORM):
	{
		c_Transform* cmp = new c_Transform(this, type);	//create component of the corresponding type
		myComponents.push_back((Component*)cmp);		//add it to the components array
		ret = cmp;										//return it in case it needs to be modfied right away
		break;
	}
	case(COMPONENT_TYPE::MATERIAL):
	{
		c_Material* cmp = new c_Material(this, type);
		myComponents.push_back((Component*)cmp);
		ret = cmp;
		break;
	}
	case(COMPONENT_TYPE::MESH):
	{
		c_Mesh* cmp = new c_Mesh(this, type);
		myComponents.push_back((Component*)cmp);
		ret = cmp;
		hasMesh = true;
		break;
	}
	case(COMPONENT_TYPE::CAMERA):
	{
		c_Camera* cmp = new c_Camera(this, type);
		myComponents.push_back((Component*)cmp);
		ret = cmp;
		break;
	}
	case(COMPONENT_TYPE::PARTICLE_SYSTEM):
	{
		c_ParticleSystem* cmp = new c_ParticleSystem(this);
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

Component* GameObject::GetComponentByType(COMPONENT_TYPE type)
{
	for (int i = 0; i < myComponents.size(); i++)
	{
		if (myComponents[i]->type == type)
		{
			return myComponents[i];
		}
	}
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
				if (myComponents[i]->type == COMPONENT_TYPE::MESH)
					hasMesh = false;

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

void GameObject::DeleteChild(uint childIndex)
{
	if (myChildren[childIndex] != nullptr)
	{
		App->scene->DeleteGameObject(myChildren[childIndex]);
	}
	myChildren.erase(myChildren.begin() + childIndex);
}

void GameObject::DeleteChildFromArray(GameObject* GO)
{
	for (uint i = 0; i < myChildren.size(); ++i)
	{
		if (myChildren[i] == GO)
		{
			myChildren.erase(myChildren.begin() + i);
		}
	}
}

void GameObject::DeleteAllChilds()
{
	for (uint i = 0; i < myChildren.size(); ++i)
	{
		if (myChildren[i] != nullptr)
		{
			App->scene->DeleteGameObject(myChildren[i]);
		}
	}
	myChildren.clear();
}

GameObject* GameObject::GetParent()
{
	return this->parent;
}

uint GameObject::GetParentUID()
{
	return this->parent->UUID;
}

void GameObject::UpdateParent()
{
	if (parentID != 0)
	{
		parent = App->scene->GetGO(parentID);
	}
}

void GameObject::ForceUID(uint32 id)
{
	UUID = id;
}

void GameObject::Draw()
{
	for (int i = 0; i < myComponents.size(); i++)
	{
		if (myComponents[i]->type == COMPONENT_TYPE::MESH)
		{
			c_Mesh* cmp = (c_Mesh*)myComponents[i];
			App->renderer3D->DrawMesh(cmp, this->transform);
			myComponents[i]->Draw();
		}
		else if (myComponents[i]->type == COMPONENT_TYPE::CAMERA)
		{
			myComponents[i]->Draw();
		}
	}
}

bool GameObject::IsCamera()
{
	if (this->isCamera)
		return true;
	else
		return false;
}
