#pragma once

#include "ComponentData.h"
#include "MathGeoLib/include/Math/TransformOps.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/float3x3.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"

class TransformData : public ComponentData
{
public:
	
	TransformData() : ComponentData(COMPONENT_TYPE::TRANSFORM), position(float3::zero), rotation(Quat::identity), scale(float3::one) {} //all set to zero
	TransformData(const float3& position, const Quat& rotation, const float3& scale) : ComponentData(COMPONENT_TYPE::TRANSFORM), position(position), rotation(rotation), scale(scale) {} //assign every value manually
	TransformData(float4x4 ModelMatrix) : ComponentData(COMPONENT_TYPE::TRANSFORM) { ModelMatrix.Decompose(position, rotation, scale); } //set values by decomposing the 4x4 Model Matrix

	float3	position;
	Quat	rotation;
	float3	scale;
};
