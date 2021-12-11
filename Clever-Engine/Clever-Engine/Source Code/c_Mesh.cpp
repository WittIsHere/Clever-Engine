#include "GameObject.h"
#include "c_Mesh.h"
#include "ModuleScene.h"
#include "JSONParser.h"
#include "SDL/include/SDL_opengl.h"


c_Mesh::c_Mesh(GameObject* parent, ComponentData* data) : Component(parent, data), drawBBox(false)
{
	type = COMPONENT_TYPE::MESH;
	meshData = (MeshData*)data;
	if (drawBBox) DrawBox();
}

c_Mesh::~c_Mesh()
{
}

bool c_Mesh::Enable()
{
	return true;
}

bool c_Mesh::Update()
{
	return true;
}

bool c_Mesh::Disable()
{
	return true;
}

bool c_Mesh::SaveState(ParsonNode& root) const
{
	root.SetNumber("Type", (uint)type);

	return true;
}

bool c_Mesh::LoadState(ParsonNode& root)
{
	return true;
}

const uint c_Mesh::GetVertexCount()
{
	return meshData->vertexCount;
}

MeshData* c_Mesh::GetMeshData()
{
	return meshData;
}

void c_Mesh::CreateBox()
{
	if (mesh != nullptr)
	{
		drawingBox.SetNegativeInfinity();
		drawingBox.Enclose(&mesh->vertices[0], mesh->vertices.size());
		aabbox.SetNegativeInfinity();
		aabbox.Enclose(&mesh->vertices[0], mesh->vertices.size());
	}
	else
	{
		drawingBox.SetNegativeInfinity();
		drawingBox.SetFromCenterAndSize(vec(0.0f, 0.0f, 0.0f), vec(1.0f, 1.0f, 1.0f));
		aabbox.SetNegativeInfinity();
		aabbox.SetFromCenterAndSize(vec(0.0f, 0.0f, 0.0f), vec(1.0f, 1.0f, 1.0f));
	}

	obb.SetFrom(aabbox);
	obb.Transform(owner->GetComponentTransform()->GetWorldTransform());
	aabbox.SetNegativeInfinity();
	aabbox.Enclose(obb);
}

void c_Mesh::UpdateBox()
{
	if (mesh != nullptr)
	{
		aabbox.SetNegativeInfinity();
		aabbox.Enclose(&mesh->vertices[0], mesh->vertices.size());
	}
	else
	{
		aabbox.SetNegativeInfinity();
		aabbox.SetFromCenterAndSize(owner->GetComponentTransform()->GetLocalPosition(), vec(1.0f, 1.0f, 1.0f));
	}

	obb.SetFrom(aabbox);
	obb.Transform(owner->GetComponentTransform()->GetWorldTransform());
	aabbox.SetNegativeInfinity();
	aabbox.Enclose(obb);
}

void c_Mesh::DrawBox() const
{
	glPushMatrix();
	glMultMatrixf(owner->GetComponentTransform()->GetWorldTransformPtr());
	float3 cornerPoints[8];
	drawingBox.GetCornerPoints(cornerPoints);

	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	glLineWidth(3.5f);
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