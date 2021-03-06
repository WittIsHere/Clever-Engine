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
#include "ResourceMesh.h"
#include "ResourceTexture.h"

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

	particles.clear();
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

	return true;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	App->camera->CalculateViewMatrix();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(App->camera->GetProjectionMatrix());
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	PlanePrimitive p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	DrawScene();
	DrawParticles();

	DMPlane(App->importer->textureBuffer1);
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
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
	//glViewport(0, 0, width, height);
	//App->camera->RecalculateProjection();
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

void ModuleRenderer3D::PrepareMesh(ResourceMesh* mesh)
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->indicesCount, mesh->indicesData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void ModuleRenderer3D::AddParticle(const float4x4& transform, ResourceTexture* material, Color color, float distanceToCamera)
{
	ParticleRenderer renderer = ParticleRenderer(material, color, transform);
	particles.insert(std::map<float, ParticleRenderer>::value_type(distanceToCamera, renderer));
}

void ModuleRenderer3D::DrawParticles()
{
	std::map<float, ParticleRenderer>::reverse_iterator it;				//Render from far to close to the camera
	for (it = particles.rbegin(); it != particles.rend(); ++it)
	{
		it->second.Render();
	}
	
	particles.clear();
}

void ModuleRenderer3D::DrawScene()
{
	App->scene->Draw();
}

void ModuleRenderer3D::DrawMesh(c_Mesh* mesh, c_Transform* transform, c_Material* material)
{
	if (mesh == nullptr)
	{
		LOG("[error]: mesh equals nullptr");
		return;
	}
	else if (mesh->GetMeshData() == nullptr)
	{
		LOG("[error]: trying to draw a component mesh without data");
		return;
	}

	//PollErrors();

	glPushMatrix();

	float4x4 worldPos = transform->GetWorldTransform();
	glMultMatrixf((float*)worldPos.Transposed().ptr());

	// We start drawing the mesh

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->GetMeshData()->vPosID);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->GetMeshData()->vTexCoordsID);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	if (mesh->GetMeshData()->indicesID != 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetMeshData()->indicesID);
	}
	else
	{
		LOG("INFO: indices buffer ID not found");
	}	
	
	if (material != nullptr)
	{
		//if (!material->isEmpty)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindTexture(GL_TEXTURE_2D, material->getTextureID());
		}
	}
	else
		BindCheckerTex();

	if (render) glDrawElements(GL_TRIANGLES, mesh->GetMeshData()->indicesCount, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
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
		LOG("OpenGL error found! %d\n",error);
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

ParticleRenderer::ParticleRenderer(ResourceTexture* material, Color color, const float4x4 transform) :
	color(color),
	transform(transform),
	mat(material)
{

}

void ParticleRenderer::Render()
{
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);

	//App->renderer3D->BindCheckerTex();
	/*
	if(mat->GetTextureID() != 0)
		glBindTexture(GL_TEXTURE_2D, mat->GetTextureID());*/

	glColor4f(color.r, color.g, color.b, color.a);

	glPushMatrix();
	glMultMatrixf(transform.ptr());

	//Drawing to tris in direct mode
	glBegin(GL_TRIANGLES);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(.5f, -.5f, .0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-.5f, .5f, .0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-.5f, -.5f, .0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(.5f, -.5f, .0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(.5f, .5f, .0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-.5f, .5f, .0f);

	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);


	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
}

void ModuleRenderer3D::DMPlane(uint buffer)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, buffer);

	//glColor4b(color.r, color.g, color.b, color.a);

	glPushMatrix();

	//Drawing to tris in direct mode
	glBegin(GL_TRIANGLES);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(.5f, -.5f, .0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-.5f, .5f, .0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-.5f, -.5f, .0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(.5f, -.5f, .0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(.5f, .5f, .0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-.5f, .5f, .0f);

	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}