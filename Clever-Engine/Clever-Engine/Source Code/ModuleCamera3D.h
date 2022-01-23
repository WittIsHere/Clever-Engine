#pragma once
#define NOMINMAX 1
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "MathGeoLib/include/Geometry/Frustum.h"
#include "MathGeoLib/include/Geometry/LineSegment.h"


class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	LineSegment GenerateRaycast(float normalizedX, float normalizedY);

	void MainCameraDrawFrustum();
	void MainCameraCheckFrustum();

	void LookAt(const float3&Spot);
	//void Move(const float3&Movement);
	float* GetViewMatrix();
	float* GetProjectionMatrix();
	void CalculateViewMatrix();
	void RecalculateProjection();
	float3 GetCameraPosition() { return Position; }

	bool test = false;

public:
	Frustum cameraFrustum;
	LineSegment lastRay;

	float3 X, Y, Z, Position, Reference;
	float Distance;
	float4x4 viewMatrix;

	float aspectRatio = 1.f;
	float verticalFOV = 0.1f;
	float nearPlaneDistance = 0.1f;
	float farPlaneDistance = 5000.f;
	
	float lastDeltaX = 0.f, lastDeltaY = 0.f;
	mat4x4 ViewMatrixInverse;

	bool showMainFrustum = true;
};