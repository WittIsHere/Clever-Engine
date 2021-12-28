#define NOMINMAX 1
#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "ModuleScene.h"
#include "MathGeoLib/include/Geometry/LineSegment.h"
#include "MathGeoLib/include/Math/float3.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{

	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	Position = float3(0.0f, 5.0f, -15.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);

	cameraFrustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);
	CalculateViewMatrix();
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");

	/*Move(vec3(3.0f, 3.0f, 3.0f));*/
	LookAt(float3(0, 0, 0));

	bool ret = true;

	aspectRatio = 1.f;
	verticalFOV = 60.f;
	nearPlaneDistance = 0.1f;
	farPlaneDistance = 1000.f;
	

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	float3 newPos(0,0,0);
	float speed = 10.0f * dt;
	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 20.0f * dt;

	// Mouse motion ----------------

	bool hasRotated = false;

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Position -= Reference;

		if (dx != 0)
		{
			const float newDeltaX = (float)dx * Sensitivity;
			float deltaX = newDeltaX + 0.95f * (lastDeltaX - newDeltaX);
			lastDeltaX = deltaX;
			Quat rotateY = Quat::RotateY(Y.y >= 0.f ? deltaX * .1f : -deltaX * .1f);
			Y = rotateY * Y;
			Z = rotateY * Z;
			CalculateViewMatrix();
			hasRotated = true;
		}

		if (dy != 0)
		{
			const float newDeltaY = (float)dy * Sensitivity;
			float deltaY = newDeltaY + 0.95f * (lastDeltaY - newDeltaY);
			lastDeltaY = deltaY;
			Quat rotateX = Quat::RotateAxisAngle(X, -deltaY * .1f);
			Y = rotateX * Y;
			Z = rotateX * Z;
			CalculateViewMatrix();
			hasRotated = true;
		}
	}

		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos.y += speed;
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos.y -= speed;

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

		Position += newPos;
		Reference += newPos;

	if (App->input->GetMouseZ() > 0)
	{
		Distance -= speed + 3;
		Position = Reference + Z * Distance;
	}
	if (App->input->GetMouseZ() < 0)
	{
		Distance += speed + 3;
		Position = Reference + Z * Distance;
	}

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		GameObject* picked = MousePicking();
	}

	// Recalculate matrix -------------
	//CalculateViewMatrix();

	return UPDATE_CONTINUE;
}


// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const float3 &Spot)
{
	Reference = Spot;

	Z = (Position - Reference).Normalized();
	X = float3(0.0f, 1.0f, 0.0f).Cross(Z).Normalized();
	Y = Z.Cross(X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
//void ModuleCamera3D::Move(const float3&Movement)
//{
//	Position += Movement;
//	Reference += Movement;
//
//	CalculateViewMatrix();
//}

// -----------------------------------------------------------------
//float* ModuleCamera3D::GetViewMatrix() //i guess the type is float* to avoid including mat4x4 everywhere
//{
//	return &viewMatrix;
//}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	if (projectionIsDirty)
	{
		RecalculateProjection();
	}

	cameraFrustum.SetPos(Position);
	cameraFrustum.SetFront(Z.Normalized());
	cameraFrustum.SetUp(Y.Normalized());
	float3::Orthonormalize((float3&)cameraFrustum.Front(), (float3&)cameraFrustum.Up());
	X = Y.Cross(Z);
	viewMatrix = cameraFrustum.ViewMatrix();
}

void ModuleCamera3D::RecalculateProjection()
{
	cameraFrustum.SetViewPlaneDistances(nearPlaneDistance, farPlaneDistance);
	cameraFrustum.SetVerticalFovAndAspectRatio(verticalFOV * DEGTORAD, aspectRatio);
}

GameObject* ModuleCamera3D::MousePicking()
{
	std::vector<GameObject*> possible;
	float normX = -(1.0f - (float(App->input->GetMouseY()) * 2.0f) / (float)App->window->GetWidth());
	float normY = -(1.0f - (float(App->input->GetMouseX()) * 2.0f) / (float)App->window->GetHeight());

	// Draw a Line to intersect with the Game Objects
	LineSegment picking = cameraFrustum.UnProjectLineSegment(normX, normY);
	float distance;

	// Iterate all Game Objects to get the list of them on screen
	for (int i = 0; i < App->scene->gameObjects.size(); i++)
	{
		if (App->scene->gameObjects[i]->hasMesh == true)
		{
			// Get the component mesh
			c_Mesh* mesh = (c_Mesh*)App->scene->gameObjects[i]->GetComponentByType(COMPONENT_TYPE::MESH);

			// Intersect the ray drawed before with the AABB box of the meshes
			if (picking.Intersects(mesh->aabbox))
			{
				float hitNear;
				float hitFar;

				if (picking.Intersects(mesh->obb, hitNear, hitFar))
				{
					possible.push_back(App->scene->gameObjects[i]);
				}
			}
		}
		else
		{
			i++;
		}
	}

	GameObject* pickedObject = nullptr;
	float finalDistance = picking.Length();
	
	/*for (int i = 0; i < possible.size(); i++)
	{

	}*/

	return nullptr;
}