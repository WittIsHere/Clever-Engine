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
	CreateComponent(ComponentType::TRANSFORM, this);
}

void GameObject::Update()
{

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
