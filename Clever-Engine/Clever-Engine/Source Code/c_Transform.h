#pragma once

#include "Globals.h"
#include "Component.h"
#include "TransformData.h";

class ParsonNode;

class c_Transform : public Component
{
public:
	c_Transform(GameObject* parent, COMPONENT_TYPE type);
	c_Transform(GameObject* parent, ComponentData* data);
	~c_Transform();

	bool Enable();
	bool Update(float dt);
	bool Disable();

	bool SaveState(ParsonNode& root) const override;
	bool LoadState(ParsonNode& root) override;

	bool AssignNewData(TransformData* data);

public:
	void SetLocalPosition(const float3& newPosition);			// Sets localPosition to the given vector.	  
	void SetLocalRotation(const Quat& newRotation);				// Sets localRotation to the given quaternion.
	void SetLocalRotation(const float3& newRotation);			// Sets localRotation to the given vector.		
	void SetLocalScale(const float3& newScale);					// Sets localScale to the given vector.		

	float3 GetLocalPosition() const;						// Returns the position vector of the local transform.
	Quat GetLocalRotation() const;						// Returns the rotation quaternion of the local transform.
	float3 GetLocalEulerRotation() const;					// Returns the rotation quaternion of the local transform in Euler Angles.
	float3 GetLocalScale() const;							// Returns the scale vector of the local transform.

	float* GetLocalTransform() const;
	void UpdateLocalTransform();

	float4x4 GetWorldTransform();
	float* GetWorldTransformPtr();
	void UpdateWorldTransform();


private:
	TransformData* transformData;
	float4x4 localTransform;

	bool updateWorld; //we use this bool to inform of a change in the localTransform so the world transform can be updated accordingly. Prevents GetWorldTransform of returning an old state.
	float4x4 worldTransform;
};