#pragma once
#include "Module.h"
#include "Globals.h"

#include <vector>

class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	void Render();
	bool CleanUp();

private:
	void DrawDockingSpace(bool active);
	void DrawConsoleSpace(bool active);

	void ConsoleLog(const char* text);
	void DrawConfigurationSpace(bool active);

	void AddLogFPS(float fps, float ms);
	void ShowDockingDisabledMessage();

public:
	bool showDemoWindow;
	bool usingKeyboard;
	bool usingMouse;

	bool changeFPSlimit = false;
	bool changeTitleName = false;
	int max_fps = 140;

private:
	//Bool variables to activate the different windows
	bool activeDockingSpace = false;
	bool activeConsole = false;
	bool activeConfiguration = false;

	//Configuration window variables
	bool need_scroll = false;
	std::vector<float> fps_log;
	std::vector<float> ms_log;

	//Console window variables
	std::vector<char*>	buffer;
	bool scrollToBottom;
};
