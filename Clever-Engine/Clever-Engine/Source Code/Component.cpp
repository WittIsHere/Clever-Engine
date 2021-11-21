#include "Component.h"

Component::Component(GameObject* parent, ComponentData* data)
{
	this->parent = parent;
	this->data = data;
}

Component::~Component()
{
}

bool Component::Enable()
{
	return false;
}

bool Component::Update()
{
	return false;
}

bool Component::Disable()
{
	return false;
}

const char* Component::getNameFromType()
{
	const char* ret = nullptr;

	switch (type)
	{
	case (COMPONENT_TYPE::MATERIAL) :
	{
		ret = "Material";
		break;
	}
	case (COMPONENT_TYPE::MESH) :
	{
		ret = "Mesh";
		break;
	}
	case (COMPONENT_TYPE::TRANSFORM) :
	{
		ret = "Transform";
		break;
	}
	}
	return ret;
}
