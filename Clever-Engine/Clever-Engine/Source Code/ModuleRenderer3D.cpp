#define NOMINMAX 1
#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleImporter.h"
#include "ModuleScene.h"
#include "MeshData.h"
#include "MaterialData.h"
#include "c_Mesh.h"
#include "c_Material.h"
#include "c_Transform.h"
#include "GameObject.h"
#include "Primitive.h"

#include "OpenGl.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glew32.lib") /* link Microsoft OpenGL lib   */


ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	const char* logout;
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	GLenum error = glewInit();

	if (error != GL_NO_ERROR)
	{
		LOG("Error initializing glew library! %s", SDL_GetError());
		ret = false;
	}

	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
	}

	defaultTexture = true;

	// Test meshes -------
	// DrawCube();
	//TestPlane();
	
	// Create (not bind) checker texture
	CreateCheckerTex();
	
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

bool ModuleRenderer3D::Start()
{
	// Import Scene -------
	/*currentScene = &App->importer->myScene;*/

	return true;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	PlanePrimitive p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	DrawScene();
	App->ui->Render();

	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::PrepareDrawScene(SceneData* scene)
{
	/*if (scene == nullptr)
	{
		LOG("ERROR: Scene is null");
		return;
	}

	for (int i = 0; i < scene->myMeshes.size(); i++)
	{
		PrepareMesh(scene->myMeshes[i]);
	}*/
}

void ModuleRenderer3D::PrepareMesh(MeshData* mesh)
{
	glGenBuffers(1, &mesh->vPosID);
	glGenBuffers(1, &mesh->vTexCoordsID);
	glGenBuffers(1, &mesh->indicesID);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vPosID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->vertexCount * 3, mesh->vPosData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vTexCoordsID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * mesh->vertexCount, mesh->vTexCoordsData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*mesh->indicesCount, mesh->indicesData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void ModuleRenderer3D::DrawScene()
{
	App->scene->Draw();
}

void ModuleRenderer3D::DrawMesh(c_Mesh* mesh, c_Transform* transform)
{
	//vertices
	if (mesh == nullptr)
	{
		LOG("[error]: mesh equals nullptr");
		return;
	}

	if (mesh->GetMeshData() == nullptr)
	{
		LOG("[error]: trying to draw a copmponent mesh without data");
		return;
	}

	if (mesh->GetMeshData()->vPosID != 0)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->GetMeshData()->vPosID);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
	}
	else
		LOG("INFO: vertex positions buffer ID not found");

	//textures
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->GetMeshData()->vTexCoordsID);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	//indices
	if (mesh->GetMeshData()->indicesID != 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetMeshData()->indicesID);
	}
	else
	{
		LOG("INFO: indices buffer ID not found");
	}	
	
	if (mesh->GetMeshData()->texture != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, (GLuint)mesh->GetMeshData()->texture->textureID);
	}
	else
		BindCheckerTex();
	//-------------------- Modify modelview matrix to fit the current mesh to be drawn
	float* viewMatrix = App->camera->GetViewMatrix();

	//to access the MODEL MATRIX we need access to the TRANSFORM of the GO that contains the mesh to be drawn
	float* modelMatrix = transform->GetWorldTransformPtr();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(viewMatrix);
	
	/*float4x4 scaleMatrix = float4x4::identity.Scale(float3(2.0f, 2.0f, 2.0f));
	scaleMatrix.Transpose();
	glMultMatrixf((float*)&scaleMatrix);
	float4x4 translationMatrix = float4x4::identity.Translate(float3(10, 0, 0));
	translationMatrix.Transpose();
	glMultMatrixf((float*)&translationMatrix);*/

	//--------------Multiply here the model matrix
	glMultMatrixf(modelMatrix);
	
	PollErrors();
	
	glDrawElements(GL_TRIANGLES, mesh->GetMeshData()->indicesCount, GL_UNSIGNED_INT, 0);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ModuleRenderer3D::DrawCube()
{
	GLfloat vertices[] = { 0.0f,0.0f,0.0f,     // 0
					       1.0f,0.0f,0.0f,     // 1
					       1.0f,1.0f,0.0f,     // 2
					       0.0f,1.0f,0.0f,     // 3
					       0.0f,0.0f,-1.0f,    // 4
					       1.0f,0.0f,-1.0f,    // 5
					       1.0f,1.0f,-1.0f,    // 6
					       0.0f,1.0f,-1.0f };  // 7

	GLushort indices[] = { 0,1,2,
						   0,2,3,
		                   1,5,6,
		                   1,6,2,
		                   0,5,1,
		                   0,4,5,
		                   0,7,4,
		                   0,3,7,
		                   4,7,6,
		                   4,6,5,
		                   3,2,6,
		                   3,6,7};


	glGenBuffers(1, &vertex_Buffer);
	glGenBuffers(1, &indices_Buffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_Buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_Buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void ModuleRenderer3D::DMPlane()
{
	glLineWidth(2.0f);
	glBegin(GL_TRIANGLES);

	glTexCoord2f(0.0f, 0.f);       glVertex3f(-2.f, 1.f, 0.f);
	glTexCoord2f(1.f, 0.f);        glVertex3f(2.f, 1.f, 0.f);
	glTexCoord2f(0.f, 1.f);        glVertex3f(-2.f, 4.f, 0.f);

	glTexCoord2f(0.f, 1.f);        glVertex3f(-2.f, 4.f, 0.f);
	glTexCoord2f(1.f, 0.f);        glVertex3f(2.f, 1.f, 0.f);
	glTexCoord2f(1.f, 1.f);        glVertex3f(2.f, 4.f, 0.f);

	glEnd();
	glFlush();
}

void ModuleRenderer3D::TestPlane()
{
	GLfloat vertices[] = { 0.0f,0.0f,0.0f,			//0.0f,0.0f,    // 0
						   1.0f,0.0f,0.0f,			//1.0f,0.0f,    // 1
						   1.0f,1.0f,0.0f,			//1.0f,1.0f,    // 2
						   0.0f,1.0f,0.0f, };		//0.0f,1.0f };  // 3  

	GLfloat textCoords[] = { 0.0f,0.0f,
		                     1.0f,0.0f,
		                     1.0f,1.0f,
	                         0.0f,1.0f};

	GLushort indices[] = { 0,1,2,
						   0,2,3};


	glGenBuffers(1, &vertex_Buffer);
	glGenBuffers(1, &texCoords_Buffer);
	glGenBuffers(1, &indices_Buffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_Buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, texCoords_Buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 2* 4, textCoords, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_Buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ModuleRenderer3D::CreateCheckerTex()
{
	GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &checker_Buffer);
	glBindTexture(GL_TEXTURE_2D, checker_Buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ModuleRenderer3D::BindCheckerTex()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, checker_Buffer);
}

uint ModuleRenderer3D::FillTexture(const void* text, uint width, uint height, int format, uint format2, const char* path)
{
	uint tex = 0;

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format2, GL_UNSIGNED_BYTE, text);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
}

bool ModuleRenderer3D::GetVSync() const
{
	return vsync;
}

void ModuleRenderer3D::SetVSync(bool vsync)
{
	if (this->vsync != vsync)
	{
		this->vsync = vsync;
		if (SDL_GL_SetSwapInterval(vsync ? 1 : 0) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
	}
}

uint* ModuleRenderer3D::GetOpenGLVersion() const
{
	return (uint*)glGetString(GL_VERSION);
}

void ModuleRenderer3D::PollErrors() //Poll and print to the console every openGl error
{
	GLenum error = glGetError(); 

	while (error != GL_NO_ERROR)
	{
		LOG("OpenGL error found! %s\n", gluErrorString(error));
		error = glGetError();
	}
}

void ModuleRenderer3D::DrawRay()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3f(0.0f, 1.0f, 1.0f);
	glLineWidth(3.0f);

	glBegin(GL_LINES);
	glVertex3f(ray.a.x, ray.a.y, ray.a.z);
	glVertex3f(ray.b.x, ray.b.y, ray.b.z);
	glEnd();

	glLineWidth(1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
}