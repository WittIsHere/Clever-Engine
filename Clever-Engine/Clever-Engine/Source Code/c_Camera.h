#ifndef __C_CAMERA_H__
#define __C_CAMERA_H__

// IMPORTANT macro
#define NOMINMAX 1
#include "Globals.h"
#include "Component.h"
#include "TransformData.h"

#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/OBB.h"
#include "MathGeoLib/include/Geometry/Sphere.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Geometry/Frustum.h"
#include "MathGeoLib//include/Geometry/Plane.h"

class GameObject;

class c_Camera : public Component
{
public:
	c_Camera(GameObject* parent, COMPONENT_TYPE type);
	c_Camera(GameObject* parent, ComponentData* data);
	~c_Camera();

	bool Enable();
	bool Disable();
	bool SaveState(ParsonNode& root) const override;
	bool LoadState(ParsonNode& root) override;

	bool Update(float dt);
	void Draw();
	void DrawFrustum();
	void CheckFrustum();

	void DrawCameraIcon();

	//void Inspector(Application* App);
	
	const AABB& GetAABB() const;

	bool ContainBOX(const AABB& referenceBox) const;

	bool frustumActive;

private:
	Frustum frustum;
	float horizontalFOV = 60.0f;
	float aspectRatio = 1.5f;

	AABB aabbox, drawingBox;


	float radius;
	float3 centerPoint;

};

#endif   // !__C_CAMERA_H__