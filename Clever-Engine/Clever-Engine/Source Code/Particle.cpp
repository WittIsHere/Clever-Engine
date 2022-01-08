#include "Particle.h"

Particle::Particle()
{
	position = float3::zero;
	worldRotation = Quat::identity;
	movementDirection = float3::zero;
	movementIntensity = 0.0f;
	velocity = float3::zero;
	acceleration = float3::zero;

	maxLifetime = 0;
	currentLifetime = 0;

	size = 1.0f;
	color = Color(0,0,0); //white
	distanceToCamera = 0; 
}

Particle::Particle(const Particle& particle)
{
}

Particle::~Particle()
{
}
