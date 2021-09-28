#include "Application.h"
#include "ModuleUI.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"

#include "Dependencies/ImGui/imgui.h"
#include "Dependencies/ImGui/imgui_internal.h"
#include "Dependencies/ImGui/imgui_impl_sdl.h"
#include "Dependencies/ImGui/imgui_impl_opengl3.h" 


ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

// Destructor
ModuleUI::~ModuleUI()
{}


bool ModuleUI::Start()
{
	LOG("Starting UI Module");
	bool ret = true;

    //Set all the atributes and flags for our Gui window
    const char* glsl_version = "#version 130";

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 7.0f;    
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Our state
    showDemoWindow = false;

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
    ImGui_ImplOpenGL3_Init(glsl_version);

	return ret;
}

update_status ModuleUI::PreUpdate(float dt)
{
    //Begin new ImGui Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->window->window);
    ImGui::NewFrame();

    //Update ImGui input usage
    ImGuiIO& io = ImGui::GetIO();
    usingKeyboard = io.WantCaptureKeyboard;
    usingMouse = io.WantCaptureMouse;

    return UPDATE_CONTINUE;
}

update_status ModuleUI::Update(float dt)
{
    if ((App->input->GetKey(SDL_SCANCODE_LALT) == KEY_DOWN) && (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN))
    {
        App->input->ExitApp();
    }

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
            if (ImGui::MenuItem("New Scene"))
            {
                // New Scene code
            }
            if (ImGui::MenuItem("Open Scene"))
            {
                // Open Scene code
            }
            if (ImGui::MenuItem("Save Scene"))
            {
                // Save Scene code
            }
            if (ImGui::MenuItem("Save Scene as"))
            {
                // New Scene code
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit"))
            {
                App->input->ExitApp();
            }
			ImGui::EndMenu();
		}

        if (ImGui::BeginMenu("Create"))
        {
            // Insert options here

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Window"))
        {
            // Insert options here

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {
            // Insert options here

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Development"))
        {
            if (ImGui::MenuItem("ImGui Github"))
                App->RequestBrowser("https://github.com/ocornut/imgui");
           
            if (ImGui::MenuItem("MathGeoLib Github"))
                App->RequestBrowser("https://github.com/juj/MathGeoLib");

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("About"))
        {
            if (ImGui::MenuItem("Documentation"))
                App->RequestBrowser("https://github.com/WittIsHere/Clever-Engine/wiki");

            if (ImGui::MenuItem("Download latest"))
                App->RequestBrowser("https://github.com/WittIsHere/Clever-Engine/releases");

            if (ImGui::MenuItem("Report a bug"))
                App->RequestBrowser("https://github.com/WittIsHere/Clever-Engine/issues");
            ImGui::EndMenu();
        }

		ImGui::EndMenuBar();
		ImGui::End();
	}

    if (showDemoWindow)
        ImGui::ShowDemoWindow(&showDemoWindow);
    {
        ImGui::Begin("DEMO");
        ImGui::Checkbox("Demo Window", &showDemoWindow);
        ImGui::End();
    }

    return UPDATE_CONTINUE;
}
update_status ModuleUI::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

void ModuleUI::Render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
}

// Called before quitting
bool ModuleUI::CleanUp()
{
	LOG("Cleaning up UI");

   /* std::vector<Window>::iterator item = winArray.begin();
    for (item; item != winArray.end(); ++item)
        (item)->CleanUp();*/

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

	return true;
}
