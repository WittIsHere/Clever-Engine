#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8
#define CHECKERS_HEIGHT 128
#define CHECKERS_WIDTH 128

struct SceneData;
class c_Transform;
class c_Mesh;
class GameObject;


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
	void PrepareDrawScene(SceneData* scene);
	void DrawMesh(c_Mesh* mesh, c_Transform* transform);

	bool GetVSync() const;
	void SetVSync(bool vsync);
	uint* GetOpenGLVersion() const;
	void PollErrors();
	//void PollErrors(const char* additionalString);
	
private:
	void DrawScene();

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

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

private:
	SceneData* currentScene = nullptr;

	//test meshes buffers
	uint vertex_Buffer;
	uint indices_Buffer;
	uint texCoords_Buffer;
};