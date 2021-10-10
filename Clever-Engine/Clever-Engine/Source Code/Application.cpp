#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	ui = new ModuleUI(this);
	importer = new ModuleImporter(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(ui);
	AddModule(importer);

	// Renderer last!
	AddModule(renderer3D);

	titleName = ENGINE_NAME;
	contFPS = 0;
	frames = 0;
	miliseconds = 1000 / 60;
	last_fps = -1;
	last_ms = -1;
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
	//Send fps values to module UI
	++frames;
	++contFPS;

	if (fps_timer.Read() >= 1000)
	{
		last_fps = contFPS;
		contFPS = 0;
		fps_timer.Start();
	}

	last_ms = ms_timer.Read();

	//if ms of last frame were less than "miliseconds" (which is the cap), wait the difference with SDL_Delay
	if (miliseconds > 0 && (last_ms < miliseconds)) 
	{
		SDL_Delay(miliseconds - last_ms);
	}

	ui->AddLogFPS((float)last_fps, (float)last_ms);
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
		if (ret == update_status::UPDATE_STOP)
		{
			exit = true;
		}
	}

	for (std::vector<Module*>::iterator e = list_modules.begin(); e != list_modules.end(); e++)
	{
		Module* module = *e;
		ret = module->Update(dt);
		if (ret == update_status::UPDATE_STOP)
		{
			exit = true;
		}
	}

	for (std::vector<Module*>::iterator e = list_modules.begin(); e != list_modules.end(); e++)
	{
		Module* module = *e;
		ret = module->PostUpdate(dt);
		if (ret == update_status::UPDATE_STOP)
		{
			exit = true;
		}
	}
	FinishUpdate();

	if (exit)
	{
		ret = update_status::UPDATE_STOP;
	}

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

//Additional functions --------------------

void Application::RequestBrowser(const char* path) const
{
	ShellExecuteA(NULL, "open", path, NULL, NULL, SW_SHOWNORMAL);
}

uint Application::GetFRLimit() const
{
	if (miliseconds > 0)
		return (uint)((1.0f / (float)miliseconds) * 1000.0f);
	else
		return 0;
}

void Application::SetFRLimit(uint max_framerate)
{
	if (max_framerate > 0)
		miliseconds = 1000 / max_framerate;
	else
		miliseconds = 0;
}

const char* Application::GetTitleName() const
{
	return titleName.c_str();
}

void Application::SetTitleName(const char* title)
{
	titleName = title;
}

