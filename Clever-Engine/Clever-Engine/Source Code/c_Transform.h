#pragma once

#include "Globals.h"
#include "Component.h"
#include "TransformData.h"

class c_Transform : public Component
{
public:
	c_Transform(GameObject* parent, ComponentData* data);
	~c_Transform();

	bool Enable();
	bool Update();
	bool Disable();

	void SetLocalPosition(const float3& newPosition);			// Sets localPosition to the given vector.	  
	void SetLocalRotation(const Quat& newRotation);				// Sets localRotation to the given quaternion.
	void SetLocalRotation(const float3& newRotation);			// Sets localRotation to the given vector.		
	void SetLocalScale(const float3& newScale);					// Sets localScale to the given vector.		

	float3 GetLocalPosition() const;						// Returns the position vector of the local transform.
	Quat GetLocalRotation() const;						// Returns the rotation quaternion of the local transform.
	float3 GetLocalEulerRotation() const;					// Returns the rotation quaternion of the local transform in Euler Angles.
	float3 GetLocalScale() const;							// Returns the scale vector of the local transform.

	void UpdateLocalTransform();

private:
	TransformData* transformData;
	float4x4 localTransform;
};