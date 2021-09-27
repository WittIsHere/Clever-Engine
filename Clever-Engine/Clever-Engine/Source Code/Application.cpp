#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	ui = new ModuleUI(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(ui);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	for (std::vector<Module*>::iterator e = list_modules.begin(); e != list_modules.end(); e++)
	{
		Module* module = *e;
		delete module;
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules

	for (std::vector<Module*>::iterator e = list_modules.begin(); e != list_modules.end(); e++)
	{
		Module* module = *e;
		ret = module->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");

	for (std::vector<Module*>::iterator e = list_modules.begin(); e != list_modules.end(); e++)
	{
		Module* module = *e;
		ret = module->Start();
	}

	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	for (std::vector<Module*>::iterator e = list_modules.begin(); e != list_modules.end(); e++)
	{
		Module* module = *e;
		ret = module->PreUpdate(dt);
	}

	for (std::vector<Module*>::iterator e = list_modules.begin(); e != list_modules.end(); e++)
	{
		Module* module = *e;
		ret = module->Update(dt);
	}

	for (std::vector<Module*>::iterator e = list_modules.begin(); e != list_modules.end(); e++)
	{
		Module* module = *e;
		ret = module->PostUpdate(dt);
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (std::vector<Module*>::iterator e = list_modules.end(); e != list_modules.end(); e--)
	{
		Module* module = *e;
		ret = module->CleanUp();
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

void Application::RequestBrowser(char* path)
{
	ShellExecuteA(0, "Open", path, 0, "", 3);
}
