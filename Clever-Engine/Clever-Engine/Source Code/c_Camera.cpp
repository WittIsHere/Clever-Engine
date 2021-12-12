#include "c_Camera.h"
#include "Application.h"
#include "GameObject.h"
#include "SDL/include/SDL_opengl.h"
#include "MathGeoLib/include/Geometry/Frustum.h"

c_Camera::c_Camera(GameObject* parent, COMPONENT_TYPE type) : Component(parent, type)
{

}

c_Camera::c_Camera(GameObject* parent, ComponentData* data) : Component(parent, data->type)
{
	type = COMPONENT_TYPE::CAMERA;
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

	c_Transform* trs = (c_Transform*)COMPONENT_TYPE::TRANSFORM;
	trs = App->scene->mainCamera->GetComponentTransform();

	

	DrawFrustum();
	return true;
}

void c_Camera::DrawFrustum()
{
	glLineWidth(2.0f);
	glBegin(GL_LINES);

	float3 cornerpoints[8];
	frustum.GetCornerPoints(cornerpoints);

	glColor3f(1.0f, 0.0f, 1.0f);

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

	glColor3f(1, 1, 1);
	glEnd();
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