#include "c_Camera.h"
#include "Application.h"
#include "GameObject.h"
#include "SDL/include/SDL_opengl.h"
#include "MathGeoLib/include/Geometry/Frustum.h"

c_Camera::c_Camera(GameObject* parent, COMPONENT_TYPE type) : Component(parent, type)
{
	type = COMPONENT_TYPE::CAMERA;
	frustum.SetPos(float3(0.0f, 0.0f, -5.0f));
	frustum.SetUp(float3(0.0f, 1.0f, 0.0f));
	frustum.SetFront(float3(0.0f, 0.0f, 1.0f));

	//This function calculates the verticalFOV using the given horizontal FOV and aspect ratio. Also sets type to PerspectiveFrustum.
	frustum.SetHorizontalFovAndAspectRatio(horizontalFOV * DEGTORAD, aspectRatio);
	frustum.SetViewPlaneDistances(1.0f, 100.0f);
	CreateCameraIcon();
	GetOwner()->isCamera = true;

	frustumActive = true;
	drawingBox.SetFromCenterAndSize(vec(0.0f, 0.0f, 0.0f), vec(0.2f, 0.2f, 0.2f));
}

c_Camera::c_Camera(GameObject* parent, ComponentData* data) : Component(parent, data->type)
{
	type = COMPONENT_TYPE::CAMERA;
	frustum.SetPos(float3(0.0f, 0.0f, -5.0f));
	frustum.SetUp(float3(0.0f, 1.0f, 0.0f));
	frustum.SetFront(float3(0.0f, 0.0f, 1.0f));

	//This function calculates the verticalFOV using the given horizontal FOV and aspect ratio. Also sets type to PerspectiveFrustum.
	frustum.SetHorizontalFovAndAspectRatio(horizontalFOV * DEGTORAD, aspectRatio);
	frustum.SetViewPlaneDistances(1.0f, 100.0f);
	GetOwner()->isCamera = true;
	CreateCameraIcon();

	frustumActive = true;
	drawingBox.SetFromCenterAndSize(vec(0.0f, 0.0f, 0.0f), vec(0.2f, 0.2f, 0.2f));
}

c_Camera::~c_Camera() {}

bool c_Camera::Enable()
{
	return true;
}

bool c_Camera::Disable()
{
	return true;
}

bool c_Camera::Update()
{
	frustum.SetPos(owner->GetComponentTransform()->GetLocalPosition());
	frustum.SetUp(owner->GetComponentTransform()->GetWorldTransform().WorldY());
	frustum.SetFront(owner->GetComponentTransform()->GetWorldTransform().WorldZ());

	aabbox.SetFromCenterAndSize(owner->GetComponentTransform()->GetLocalPosition(), vec(0.2f, 0.2f, 0.2f));

	return true;
}

void c_Camera::Draw()
{
	DrawCameraIcon();
	if (frustumActive == true)
	{
		DrawFrustum();
	}
}

void c_Camera::DrawFrustum()
{
	glLineWidth(2.0f);
	glBegin(GL_LINES);

	float3 cornerpoints[8];
	frustum.GetCornerPoints(cornerpoints);

	glColor3f(0.0f, 0.0f, 0.0f);

	glVertex3f(cornerpoints[0].x, cornerpoints[0].y, cornerpoints[0].z);
	glVertex3f(cornerpoints[1].x, cornerpoints[1].y, cornerpoints[1].z);

	glVertex3f(cornerpoints[0].x, cornerpoints[0].y, cornerpoints[0].z);
	glVertex3f(cornerpoints[2].x, cornerpoints[2].y, cornerpoints[2].z);

	glVertex3f(cornerpoints[2].x, cornerpoints[2].y, cornerpoints[2].z);
	glVertex3f(cornerpoints[3].x, cornerpoints[3].y, cornerpoints[3].z);

	glVertex3f(cornerpoints[1].x, cornerpoints[1].y, cornerpoints[1].z);
	glVertex3f(cornerpoints[3].x, cornerpoints[3].y, cornerpoints[3].z);

	glVertex3f(cornerpoints[0].x, cornerpoints[0].y, cornerpoints[0].z);
	glVertex3f(cornerpoints[4].x, cornerpoints[4].y, cornerpoints[4].z);

	glVertex3f(cornerpoints[5].x, cornerpoints[5].y, cornerpoints[5].z);
	glVertex3f(cornerpoints[4].x, cornerpoints[4].y, cornerpoints[4].z);

	glVertex3f(cornerpoints[5].x, cornerpoints[5].y, cornerpoints[5].z);
	glVertex3f(cornerpoints[1].x, cornerpoints[1].y, cornerpoints[1].z);

	glVertex3f(cornerpoints[5].x, cornerpoints[5].y, cornerpoints[5].z);
	glVertex3f(cornerpoints[7].x, cornerpoints[7].y, cornerpoints[7].z);

	glVertex3f(cornerpoints[7].x, cornerpoints[7].y, cornerpoints[7].z);
	glVertex3f(cornerpoints[6].x, cornerpoints[6].y, cornerpoints[6].z);

	glVertex3f(cornerpoints[6].x, cornerpoints[6].y, cornerpoints[6].z);
	glVertex3f(cornerpoints[2].x, cornerpoints[2].y, cornerpoints[2].z);

	glVertex3f(cornerpoints[6].x, cornerpoints[6].y, cornerpoints[6].z);
	glVertex3f(cornerpoints[4].x, cornerpoints[4].y, cornerpoints[4].z);

	glVertex3f(cornerpoints[7].x, cornerpoints[7].y, cornerpoints[7].z);
	glVertex3f(cornerpoints[3].x, cornerpoints[3].y, cornerpoints[3].z);

	glEnd();
}

// Methods related to creating the icon of the camera:

void c_Camera::DrawCameraIcon()
{
	glPushMatrix();
	glMultMatrixf(owner->GetComponentTransform()->GetWorldTransformPtr());
	float3 cornerPoints[8];
	drawingBox.GetCornerPoints(cornerPoints);

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glLineWidth(5.0f);
	glBegin(GL_LINES);

	glVertex3f(cornerPoints[0].x, cornerPoints[0].y, cornerPoints[0].z);
	glVertex3f(cornerPoints[1].x, cornerPoints[1].y, cornerPoints[1].z);

	glVertex3f(cornerPoints[0].x, cornerPoints[0].y, cornerPoints[0].z);
	glVertex3f(cornerPoints[2].x, cornerPoints[2].y, cornerPoints[2].z);

	glVertex3f(cornerPoints[2].x, cornerPoints[2].y, cornerPoints[2].z);
	glVertex3f(cornerPoints[3].x, cornerPoints[3].y, cornerPoints[3].z);

	glVertex3f(cornerPoints[1].x, cornerPoints[1].y, cornerPoints[1].z);
	glVertex3f(cornerPoints[3].x, cornerPoints[3].y, cornerPoints[3].z);

	glVertex3f(cornerPoints[0].x, cornerPoints[0].y, cornerPoints[0].z);
	glVertex3f(cornerPoints[4].x, cornerPoints[4].y, cornerPoints[4].z);

	glVertex3f(cornerPoints[5].x, cornerPoints[5].y, cornerPoints[5].z);
	glVertex3f(cornerPoints[4].x, cornerPoints[4].y, cornerPoints[4].z);

	glVertex3f(cornerPoints[5].x, cornerPoints[5].y, cornerPoints[5].z);
	glVertex3f(cornerPoints[1].x, cornerPoints[1].y, cornerPoints[1].z);

	glVertex3f(cornerPoints[5].x, cornerPoints[5].y, cornerPoints[5].z);
	glVertex3f(cornerPoints[7].x, cornerPoints[7].y, cornerPoints[7].z);

	glVertex3f(cornerPoints[7].x, cornerPoints[7].y, cornerPoints[7].z);
	glVertex3f(cornerPoints[6].x, cornerPoints[6].y, cornerPoints[6].z);

	glVertex3f(cornerPoints[6].x, cornerPoints[6].y, cornerPoints[6].z);
	glVertex3f(cornerPoints[2].x, cornerPoints[2].y, cornerPoints[2].z);

	glVertex3f(cornerPoints[6].x, cornerPoints[6].y, cornerPoints[6].z);
	glVertex3f(cornerPoints[4].x, cornerPoints[4].y, cornerPoints[4].z);

	glVertex3f(cornerPoints[7].x, cornerPoints[7].y, cornerPoints[7].z);
	glVertex3f(cornerPoints[3].x, cornerPoints[3].y, cornerPoints[3].z);

	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glLineWidth(1.0f);
	glPopMatrix();
}

bool c_Camera::SaveState(ParsonNode& root) const
{
	root.SetNumber("Type", (uint)type);

	return true;
}

bool c_Camera::LoadState(ParsonNode& root)
{
	return true;
}

const AABB& c_Camera::GetAABB() const
{
	return aabbox;
}

bool c_Camera::ContainBOX(const AABB& referenceBox) const
{
	float3 vertexCorner[8];
	int totalIn = 0;

	referenceBox.GetCornerPoints(vertexCorner);

	for (int i = 0; i < 6; ++i)
	{
		int outsidecorner = 8;
		int iPtn = 1;

		for (int j = 0; j < 8; ++j)
		{
			if (frustum.GetPlane(i).IsOnPositiveSide(vertexCorner[j]))
			{
				iPtn = 0;
				--outsidecorner;
			}
		}

		if (outsidecorner == 0) return false;
		totalIn += iPtn;
	}
	if (totalIn == 6) return true;

	return true;
}