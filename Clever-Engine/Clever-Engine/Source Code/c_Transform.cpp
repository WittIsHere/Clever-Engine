#include "c_Transform.h"

c_Transform::c_Transform(GameObject* parent, ComponentData* data) : Component(parent, data)
{
	this->type = COMPONENT_TYPE::TRANSFORM;
	transformData = (TransformData*)data;
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

void c_Transform::SetLocalPosition(const float3& newPosition)
{
	transformData->position = newPosition;

	UpdateLocalTransform();
}

void c_Transform::SetLocalRotation(const Quat& newRotation)
{
	transformData->rotation = newRotation;

	UpdateLocalTransform();
}

void c_Transform::SetLocalRotation(const float3& newRotation)
{
	transformData->rotation = Quat::FromEulerXYZ(newRotation.x, newRotation.y, newRotation.z);

	UpdateLocalTransform();
}

void c_Transform::SetLocalScale(const float3& newScale)
{
	transformData->scale = newScale;

	UpdateLocalTransform();
}

float3 c_Transform::GetLocalPosition() const
{
	return transformData->position;
}

Quat c_Transform::GetLocalRotation() const
{
	return transformData->rotation;
}

float3 c_Transform::GetLocalEulerRotation() const
{
	return transformData->rotation.ToEulerXYZ();
}

float3 c_Transform::GetLocalScale() const
{
	return transformData->scale;
}

float4x4 c_Transform::GetLocalTransform() const
{
	return localTransform;
}

void c_Transform::UpdateLocalTransform()
{
	localTransform = float4x4::FromTRS(transformData->position, transformData->rotation, transformData->scale);
}
