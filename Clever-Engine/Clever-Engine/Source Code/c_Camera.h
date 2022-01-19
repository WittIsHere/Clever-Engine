#ifndef __C_CAMERA_H__
#define __C_CAMERA_H__

// IMPORTANT macro
#define NOMINMAX 1
#include "Globals.h"
#include <string>
#include "MathGeoLib/include/MathGeoLib.h"
#include "Component.h"
#include "TransformData.h"
#include "Application.h"
#include "SDL/include/SDL_opengl.h"
#include "MathGeoLib/include/Geometry/Frustum.h"

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

	bool Update();
	void Draw();
	void DrawFrustum();

	void CreateCameraIcon();
	void DrawCameraIcon();
	void UpdateCameraIcon();
	//void Inspector(Application* App);
	
	const AABB& GetAABB() const;
	const OBB& GetOBB() const;

	bool ContainBOX(const AABB& referenceBox) const;

private:
	Frustum frustum;
	float horizontalFOV = 60.0f;
	float aspectRatio = 1.5f;

	AABB aabbox, drawingBox;
	OBB obb;

	float radius;
	float3 centerPoint;

};

#endif   // !__C_CAMERA_H__