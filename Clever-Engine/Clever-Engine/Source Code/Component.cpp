#include "Component.h"

Component::Component(GameObject* parent, COMPONENT_TYPE type)
{
	this->owner = parent;
	this->type = type;
	this->isEmpty = true;
}

Component::~Component()
{
}

bool Component::Enable()
{
	return false;
}

bool Component::Update(float dt)
{
	return false;
}

void Component::Draw()
{

}

bool Component::Disable()
{
	return false;
}

bool Component::SaveState(ParsonNode& root) const
{
	return true;
}

bool Component::LoadState(ParsonNode& root)
{
	return true;
}

const char* Component::getNameFromType()
{
	const char* ret = nullptr;

	switch (type)
	{
	case (COMPONENT_TYPE::MATERIAL):
	{
		ret = "Material";
		break;
	}
	case (COMPONENT_TYPE::MESH):
	{
		ret = "Mesh";
		break;
	}
	case (COMPONENT_TYPE::TRANSFORM):
	{
		ret = "Transform";
		break;
	}
	case (COMPONENT_TYPE::CAMERA):
	{
		ret = "Camera";
		break;
	}
	return ret;
	}
}

GameObject* Component::GetOwner()
{
	return owner;
}
