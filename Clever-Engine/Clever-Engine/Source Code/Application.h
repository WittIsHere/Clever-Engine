#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleUI.h"
#include "ModuleImporter.h"
#include "ModuleScene.h"
#include "ModuleFileSystem.h"

#include <vector>
#include <string>

class Application
{
public:
	ModuleFileSystem* fileSystem;
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleUI* ui;
	ModuleImporter* importer;
	ModuleScene* scene;

private:
	Timer ms_timer;
	Timer	fps_timer;
	float dt;
	std::vector<Module*> list_modules;
	bool exit = false;

	//Framerate variables
	int	miliseconds; //max miliseconds per frame
	int contFPS;
	int	last_ms;
	int	last_fps;
	Uint32 frames;

	std::string titleName;

public:
	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

public:
	// Add other functions here
	void RequestBrowser(const char* path) const;
	uint GetFRLimit() const;
	void SetFRLimit(uint max_framerate);
	const char* GetTitleName() const;
	void SetTitleName(const char* title);

};
extern Application* App;