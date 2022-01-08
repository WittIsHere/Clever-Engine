#pragma once

#include "Globals.h"
#include "Color.h"

#include "Dependencies/MathGeoLib/include/Math/float3.h"
#include "Dependencies/MathGeoLib/include/Math/Quat.h"

class Particle
{
public:
	Particle();
	Particle(const Particle& particle);
	~Particle();

public:
	float3 position;
	Quat worldRotation;
	float3 movementDirection;
	float movementIntensity = 0.0f;
	float3 velocity;
	float3 acceleration;

	float maxLifetime;
	float currentLifetime;
	
	float size = 1.0f;
	Color color;
	float distanceToCamera;
};