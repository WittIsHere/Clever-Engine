#pragma once
#include "Module.h"
#include "Globals.h"
#include "ImGui/imgui.h"

#include <vector>

#define FILE_MAX 250

class GameObject;

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
	void DrawConsoleSpace(bool* active);
	void DrawFolderBrowser(bool* active);
	void DrawConfigurationSpace(bool* active);
	void DrawHierarchySpace(bool* active);
	void ShowChildData(GameObject* GO);
	void DrawInspectorSpace(bool* active);
	void DrawSceneSpace(bool* active);

	void ShowDockingDisabledMessage();

public:
	void ConsoleLog(const char* text);
	void AddLogFPS(float fps, float ms);
	void DrawDirectoryRecursive(const char* directory/*, const char* filter_extension*/);

public:
	bool showDemoWindow;
	bool usingKeyboard;
	bool usingMouse;

	bool changeFPSlimit = false;
	bool changeTitleName = false;
	int max_fps = 60;

	int nodeClicked = -1;

private:
	//Bool variables to activate the different windows
	bool activeConsole = true;
	bool folderBrowser = true;
	bool activeConfiguration = false;
	bool activeDockingSpace = false;
	bool activeHierarchy = true;
	bool activeInspector = false;
	bool activeScene = true;
	//Configuration window variables
	bool need_scroll = false;
	std::vector<float> fps_log;
	std::vector<float> ms_log;
	ImVec2 lastViewportSize;

	//Console window variables
	std::vector<char*>	buffer;
	bool scrollToBottom;

	char selected_file[FILE_MAX];

};
