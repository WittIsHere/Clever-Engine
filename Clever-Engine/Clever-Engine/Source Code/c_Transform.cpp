#include "c_Transform.h"

c_Transform::c_Transform(GameObject* parent, ComponentData* data) : Component(parent, data)
{
	this->type = COMPONENT_TYPE::TRANSFORM;
}

c_Transform::~c_Transform()
{
}

bool c_Transform::Enable()
{
	return false;
}

bool c_Transform::Update()
{
	return false;
}

bool c_Transform::Disable()
{
	return false;
}
