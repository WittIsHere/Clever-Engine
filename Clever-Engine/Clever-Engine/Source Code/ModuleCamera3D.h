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
	void LookAt(const float3&Spot);
	//void Move(const float3&Movement);
	//float* GetViewMatrix();
	void CalculateViewMatrix();
	void RecalculateProjection();

public:
	Frustum cameraFrustum;
	float3 X, Y, Z, Position, Reference;
	float Distance;
	float4x4 viewMatrix;
	bool projectionIsDirty = false;
	float aspectRatio = 1.f;
	float verticalFOV = 0.1f;
	float nearPlaneDistance = 0.1f;
	float farPlaneDistance = 5000.f;
	
private:
	float lastDeltaX = 0.f, lastDeltaY = 0.f;
	mat4x4 ViewMatrixInverse;
};