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
