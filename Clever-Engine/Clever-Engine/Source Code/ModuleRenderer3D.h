#pragma once
#define NOMINMAX 1
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "ResourceMesh.h"
#include "MathGeoLib/include/Geometry/LineSegment.h"
#include "MathGeoLib/include/Geometry/OBB.h"
#include "Particle.h"


#define MAX_LIGHTS 8
#define CHECKERS_HEIGHT 128
#define CHECKERS_WIDTH 128

struct SceneData;
struct MeshData;
class c_Transform;
class c_Mesh;
class c_Material;
class GameObject;
class ResourceTexture;

struct ParticleRenderer
{
	ParticleRenderer(ResourceTexture* mat, Color color, const float4x4 transform);

	void Render();

	ResourceTexture* mat;
	Color		color;
	float4x4	transform;
};

const float ParticlesCoords[] = {
1, 1,
1, 0,
0, 0,
1, 0
};

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);		
	bool CleanUp();

	void OnResize(int width, int height);

	void PrepareMesh(MeshData* mesh);
	void PrepareMesh(ResourceMesh* mesh);
	void PrepareDrawScene(SceneData* scene);
	void DrawMesh(c_Mesh* mesh, c_Transform* transform, c_Material* material);

	bool GetVSync() const;
	void SetVSync(bool vsync);
	uint* GetOpenGLVersion() const;
	void PollErrors();
	void DrawRay();
	//void PollErrors(const char* additionalString);

	void AddParticle(const float4x4& transform, ResourceTexture* mat, Color color, float distanceToCamera);
	void DrawParticles();
	
private:
	void DrawScene();

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	OBB obb;
	bool render = true;
	LineSegment ray;
	// Draw Cube vars
	void DrawCube();
	void DMPlane();
	void TestPlane();
	void CreateCheckerTex();
	void BindCheckerTex();

	uint FillTexture(const void* text, uint width, uint height, int format, uint format2, const char* path);

	uint houseTexture_Buffer = 0;
	uint checker_Buffer = 0;
	bool defaultTexture = false;

    bool vsync = true;

	std::map<float, ParticleRenderer> particles;

private:
	SceneData* currentScene = nullptr;

	//test meshes buffers
	uint vertex_Buffer;
	uint indices_Buffer;
	uint texCoords_Buffer;
};