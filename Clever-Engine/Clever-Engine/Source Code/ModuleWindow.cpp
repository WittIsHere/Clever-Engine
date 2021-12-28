#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN; 

		//Use OpenGL 2.1
		/*SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);*/
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

uint ModuleWindow::GetWidth() const
{
	int width, height;
	SDL_GetWindowSize(window, &width, &height);
	return width;
}

void ModuleWindow::SetWidth(uint width)
{
	SDL_SetWindowSize(window, width, GetHeight());
}

uint ModuleWindow::GetHeight() const
{
	int width, height;
	SDL_GetWindowSize(window, &width, &height);
	return height;
}

void ModuleWindow::SetHeight(uint height)
{
	SDL_SetWindowSize(window, GetWidth(), height);
}

bool ModuleWindow::IsFullscreen() const
{
	return isFullScreen;
}

void ModuleWindow::SetFullscreen(bool set)
{
	if (set != isFullScreen)
	{
		isFullScreen = set;
		if (isFullScreen == true)
		{
			if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) != 0)
			{
				LOG("Could not switch to fullscreen: %s\n", SDL_GetError());
			}

			isFullScreenDesktop = false;
		}
		else
		{
			if (SDL_SetWindowFullscreen(window, 0) != 0)
			{
				LOG("Could not exit fullscreen: %s\n", SDL_GetError());
			}

		}
	}
}

bool ModuleWindow::IsResizable() const
{
	return isResizable;
}

void ModuleWindow::SetResizable(bool set)
{
	isResizable = true;
}

bool ModuleWindow::IsBorderless() const
{
	return isBorderless;
}

void ModuleWindow::SetBorderless(bool set)
{
	if (set != isBorderless)
	{
		SDL_SetWindowBordered(window, (SDL_bool)set);
		isBorderless = set;
	}
}

bool ModuleWindow::IsFullscreenDesktop() const
{
	return isFullScreenDesktop;
}

void ModuleWindow::SetFullscreenDesktop(bool set)
{
	if (set != isFullScreenDesktop)
	{
		isFullScreenDesktop = set;
		if (isFullScreenDesktop == true)
		{
			if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
				LOG("Could not switch to fullscreen desktop: %s\n", SDL_GetError());
			isFullScreen = false;
		}
		else
		{
			if (SDL_SetWindowFullscreen(window, 0) != 0)
				LOG("Could not exit full screen desktop: %s\n", SDL_GetError());
		}
	}
}
