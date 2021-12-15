#pragma once
#define NOMINMAX 1
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "MathGeoLib/include/Geometry/Frustum.h"


class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	GameObject* MousePicking();
	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();

private:

	void CalculateViewMatrix();

public:
	Frustum cameraFrustum;
	float3 X, Y, Z, Position, Reference;
	float Distance;
	float4x4 viewMatrix;
private:
	float lastDeltaX = 0.f, lastDelatY = 0.f;
	mat4x4 ViewMatrixInverse;
};