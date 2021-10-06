#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;
	bool isFullScreen = false;
	bool isFullScreenDesktop = false;
	bool isResizable = true;
	bool isBorderless = false;


public:
	uint GetWidth() const;
	void SetWidth(uint width);
	uint GetHeight() const;
	void SetHeight(uint height);

	bool IsFullscreen() const;
	void SetFullscreen(bool set);
	bool IsResizable() const;
	void SetResizable(bool set);
	bool IsBorderless() const;
	void SetBorderless(bool set);
	bool IsFullscreenDesktop() const;
	void SetFullscreenDesktop(bool set);
};

#endif // __ModuleWindow_H__