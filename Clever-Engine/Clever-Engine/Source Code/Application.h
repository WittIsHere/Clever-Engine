#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleUI.h"

#include <vector>
#include <string>

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleUI* ui;

private:
	Timer ms_timer;
	float dt;
	std::vector<Module*> list_modules;
	bool exit = false;

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
	const char* GetTitleName() const;
	void SetTitleName(const char* title);

};