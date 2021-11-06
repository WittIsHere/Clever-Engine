#include "GameObject.h"

GameObject::GameObject(char* name, GameObject* parent)
{
	toDestroy = false;
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

Component* GameObject::CreateComponent(ComponentType type, GameObject* parent)
{


	return nullptr;
}

void GameObject::PrepareDraw()
{
}

void GameObject::Draw()
{
}
