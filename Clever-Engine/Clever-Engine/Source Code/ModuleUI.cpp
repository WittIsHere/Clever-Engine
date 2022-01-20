#include "Application.h"
#include "Globals.h"
#include "GameObject.h"
#include "ModuleUI.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"

#include "c_Transform.h"
#include "c_Material.h"
#include "c_Mesh.h"

#include <vector>
#include <string.h>
#include <algorithm>
#include <filesystem>

#include "Dependencies/ImGui/imgui.h"
#include "Dependencies/ImGui/imgui_internal.h"
#include "Dependencies/ImGui/imgui_impl_sdl.h"
#include "Dependencies/ImGui/imgui_impl_opengl3.h" 

using namespace std;

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled),
fps_log(LOG_LENGTH), ms_log(LOG_LENGTH)
{}

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

    selected_file[0] = '\0';

	return ret;
}

update_status ModuleUI::PreUpdate(float dt)
{
    //Begin new ImGui Frame	
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->window->window);
    ImGui::NewFrame();

    //Update ImGui input usage
    ImGuiIO& io = ImGui::GetIO();
    usingKeyboard = io.WantCaptureKeyboard;
    usingMouse = io.WantCaptureMouse;

    //check for fps limit changes and update the limit in case it has changed
    if (changeFPSlimit)
    {
        App->SetFRLimit(max_fps);
    }

    return UPDATE_CONTINUE;
}

update_status ModuleUI::Update(float dt)
{
    if ((App->input->GetKey(SDL_SCANCODE_LALT) == KEY_DOWN) && (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN))
    {
        App->input->ExitApp();
    }
    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->GetWorkPos());
    ImGui::SetNextWindowSize(viewport->GetWorkSize());
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |ImGuiWindowFlags_NoBackground;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    if (ImGui::Begin("Dockspace", &activeDockingSpace, window_flags))
    {
        ImGui::PopStyleVar(3);
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
        
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Scene"))
                {
                    // New Scene code
                }
                if (ImGui::MenuItem("Open Scene"))
                {
                    App->scene->LoadScene("Assets/Scenes/scene saving test.json");
                }
                if (ImGui::MenuItem("Save Scene"))
                {
                    App->scene->SaveScene("scene saving test");
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
                if (ImGui::BeginMenu("GameObject"))
                {
                    if (ImGui::MenuItem("Cube"))
                    {
                        LOG("Loading Basic Shape Cube");
                        App->importer->ImportAndLoadScene("Assets/Basic Shapes/Cube.FBX");
                    }

                    if (ImGui::MenuItem("Cone"))
                    {
                        LOG("Loading Basic Shape Cone");
                        App->importer->ImportAndLoadScene("Assets/Basic Shapes/Cone.FBX");
                    }

                    if (ImGui::MenuItem("Sphere"))
                    {
                        LOG("Loading Basic Shape Sphere");
                        App->importer->ImportAndLoadScene("Assets/Basic Shapes/Sphere.FBX");
                    }

                    if (ImGui::MenuItem("Cylinder"))
                    {
                        LOG("Loading Basic Shape Cylinder");
                        App->importer->ImportAndLoadScene("Assets/Basic Shapes/Cylinder.FBX");
                    }

                    if (ImGui::MenuItem("Pyramid"))
                    {
                        LOG("Loading Basic Shape Pyramid");
                        App->importer->ImportAndLoadScene("Assets/Basic Shapes/Pyramid.FBX");
                    }

                    if (ImGui::MenuItem("Torus"))
                    {
                        LOG("Loading Basic Shape Torus");
                        App->importer->ImportAndLoadScene("Assets/Basic Shapes/Torus.FBX");
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Window"))
            {
                ImGui::Checkbox("Scene", &activeScene);
                ImGui::Checkbox("Console", &activeConsole);
                ImGui::Checkbox("Folder Browser", &activeBrowser);
                ImGui::Checkbox("Configuration", &activeConfiguration);
                ImGui::Checkbox("Hierarchy", &activeHierarchy);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help"))
            {
                if (ImGui::MenuItem("Documentation"))
                    App->RequestBrowser("https://github.com/WittIsHere/Clever-Engine/wiki");

                if (ImGui::MenuItem("Download latest"))
                    App->RequestBrowser("https://github.com/WittIsHere/Clever-Engine/releases");

                if (ImGui::MenuItem("Report a bug"))
                    App->RequestBrowser("https://github.com/WittIsHere/Clever-Engine/issues");  

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
                ImGui::TextWrapped("Clever Engine\n\n");
                ImGui::Text("Open 3D Game Engine developed during the 3rd course of Design and Development of Videogames at CITM (UPC-Barcelona)\n\n");
                ImGui::Text("By Aram Galarza & Albert Espinosa\n");
                ImGui::Text("3rd party libraries used:\n\n");
                ImGui::Text("- SDL\n\n- Glew\n\n- OpenGL\n\n- ImGui\n\n- MathGeoLib\n\n- Assimp\n\n");
                ImGui::Text("License:\n");
                ImGui::Text("MIT License\n");
                ImGui::Text("Copyright (c) 2021 Albert Espinosa & Aram Galarza\n");
                ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the 'Software'),"); 
                ImGui::Text("to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,");
                ImGui::Text("and /or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: \n\n");
                ImGui::Text("The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n");
                ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, \n");
                ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,\n");
                ImGui::Text("WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n");
                
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();
    }

    // Draw windows

    DrawConsoleSpace(&activeConsole);
    DrawBrowserSpace(&activeBrowser);
    DrawContentBrowserSpace(&activeContentB);
    DrawConfigurationSpace(&activeConfiguration);
    DrawHierarchySpace(&activeHierarchy);
    DrawSceneSpace(&activeScene);
    DrawInspectorSpace(&activeInspector);

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

void ModuleUI::PickedGO(uint32 id)
{
    nodeClicked = id;
    if (activeInspector == false)
        activeInspector = true;
}

void ModuleUI::DrawConsoleSpace(bool* active)
{
    if (*active == false)
        return;

    if (!ImGui::Begin("Console", active))
    {
        ImGui::End();
        return;
    }
    {   //console space
        for (uint i = 0; i < buffer.size(); i++)
        {
            const char* item = buffer[i];
            ImGui::TextUnformatted(item);
        }
        if (scrollToBottom)
        {
            ImGui::SetScrollHere(1.0f);
            scrollToBottom = false;
        }
    }
    ImGui::End();
}

void ModuleUI::ConsoleLog(const char* text)
{
    buffer.push_back(_strdup(text));
    scrollToBottom = true;
}

void ModuleUI::DrawConfigurationSpace(bool* active)
{
    if (*active == false)
        return;

    if (!ImGui::Begin("Configuration", active))
    {
        ImGui::End();
        return;
    }
    {   //configuration space
        if (ImGui::CollapsingHeader("Application"))
        {
            char appName[100];
            strcpy_s(appName, 100, App->GetTitleName());

            if (ImGui::InputText("Project Name", appName, 100, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                App->SetTitleName(appName);
            }

            if (ImGui::SliderInt("Max FPS", &max_fps, 0, 140))
            {
                changeFPSlimit = true;
            }

            ImGui::Text("Limit Framerate:");
            ImGui::SameLine();
            ImGui::TextColored(YELLOW, "%i", max_fps);

            char title[25];
            sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
            ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
            sprintf_s(title, 25, "Milliseconds %0.1f", ms_log[ms_log.size() - 1]);
            ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));

            //MMGR ---------------------------------------------
            /*sMStats stats = m_getMemoryStatistics();
            static int speed = 0;
            static vector<float> memory(100);
            if (++speed > 20)
            {
                speed = 0;
                if (memory.size() == 100)
                {
                    for (uint i = 0; i < 100 - 1; ++i)
                        memory[i] = memory[i + 1];

                    memory[100 - 1] = (float)stats.totalReportedMemory;
                }
                else
                    memory.push_back((float)stats.totalReportedMemory);
            }

            ImGui::PlotHistogram("##memory", &memory[0], memory.size(), 0, "Memory Consumption", 0.0f, (float)stats.peakReportedMemory * 1.2f, ImVec2(310, 100));

            ImGui::Text("Total Reported Mem: %u", stats.totalReportedMemory);
            ImGui::Text("Total Actual Mem: %u", stats.totalActualMemory);
            ImGui::Text("Peak Reported Mem: %u", stats.peakReportedMemory);
            ImGui::Text("Peak Actual Mem: %u", stats.peakActualMemory);
            ImGui::Text("Accumulated Reported Mem: %u", stats.accumulatedReportedMemory);
            ImGui::Text("Accumulated Actual Mem: %u", stats.accumulatedActualMemory);
            ImGui::Text("Accumulated Alloc Unit Count: %u", stats.accumulatedAllocUnitCount);
            ImGui::Text("Total Alloc Unit Count: %u", stats.totalAllocUnitCount);
            ImGui::Text("Peak Alloc Unit Count: %u", stats.peakAllocUnitCount);
            */
        }

        if (ImGui::CollapsingHeader("Render"))
        {
            //DEPTH BUFFER
           /* if (ImGui::Checkbox("Depth Buffer", &App->renderer3D->depthEnabled))
            {
                App->renderer3D->SetDepthBufferEnabled();
            }*/
            //if (ImGui::Checkbox("Wireframe Mode", &App->renderer3D->wireframeMode)) {}

            bool vsync = App->renderer3D->GetVSync();
            if (ImGui::Checkbox("Vertical Sync", &vsync))
            {
                changeFPSlimit = true;
                App->renderer3D->SetVSync(vsync);
            }
        }
        if (ImGui::CollapsingHeader("Input"))
        {
            ImGui::Text("Mouse position: %i, %i", App->input->GetMouseX(), App->input->GetMouseY());
            ImGui::Text("Mouse motion: %i, %i", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());
            ImGui::Text("Mouse wheel: %i", App->input->GetMouseZ());
        }
        if (ImGui::CollapsingHeader("Window"))
        {
            int w = App->window->GetWidth();
            int h = App->window->GetHeight();
            if (ImGui::SliderInt("Width", &w, 0, 1920)) { App->window->SetWidth(w); }
            if (ImGui::SliderInt("Height", &h, 0, 1080)) { App->window->SetHeight(h); }

            bool fullscreen = App->window->IsFullscreen();
            bool resizable = App->window->IsResizable();
            bool borderless = App->window->IsBorderless();
            bool full_desktop = App->window->IsFullscreenDesktop();

            ImGui::SameLine();
            if (ImGui::Checkbox("Fullscreen Desktop", &full_desktop))
            {
                App->window->SetFullscreenDesktop(full_desktop);
            }

            if (ImGui::Checkbox("Resizable ", &resizable))
            {
                App->window->SetResizable(resizable);
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("Must restart to apply");
            }

            ImGui::SameLine();
            if (ImGui::Checkbox("Borderless", &borderless))
            {
                App->window->SetBorderless(borderless);
            }

        }
        if (ImGui::CollapsingHeader("Software"))
        {
            ImGui::Text("ImGui Version:");
            ImGui::SameLine();
            ImGui::TextColored(YELLOW, "%s", IMGUI_VERSION);
            if (ImGui::Checkbox("ImGui Demo Window", &showDemoWindow));
            ImGui::ShowDemoWindow(&showDemoWindow);    
            
            SDL_version sdl_version;
            SDL_GetVersion(&sdl_version);
            ImGui::Text("SDL Version:");
            ImGui::SameLine();
            ImGui::TextColored(YELLOW, "%d.%d.%d", sdl_version.major, sdl_version.minor, sdl_version.patch);

            ImGui::Text("OpenGL Version:");
            ImGui::SameLine();
            ImGui::TextColored(YELLOW, "%s", App->renderer3D->GetOpenGLVersion());

            ImGui::Text("DevIL Version:");
            ImGui::SameLine();
            ImGui::TextColored(YELLOW, "%s", IMGUI_VERSION);		//change ImGui for Devil
        }
    }
    ImGui::End();
}

void ModuleUI::DrawHierarchySpace(bool* active)
{
    if (*active == false)
        return; 
    
    ImGui::Begin("Hierarchy", active, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    GameObject* root = App->scene->rootNode;
    ShowChildData(root);
    //how to right click menu
    ImGui::End();   
}

 void ModuleUI::ShowChildData(GameObject* GO)
 {
     if (GO == nullptr)
     {
         LOG("[error]: trying to show data of an emty GO");
         return;
     }

     ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

     if (GO == App->scene->rootNode) flags |= ImGuiTreeNodeFlags_DefaultOpen;
     if (GO->UUID == nodeClicked) flags |= ImGuiTreeNodeFlags_Selected;

     const char* GOName = GO->name.c_str();
     if (GOName == nullptr)
     {
        //LOG("[Warning] UI: Unnamed GO");
         GOName = "no name";
     }
     if (ImGui::TreeNodeEx(GOName, flags))
     {
         if (ImGui::IsItemClicked())
         {
             nodeClicked = GO->UUID;
             if (activeInspector == false)
                activeInspector = true;
 
         }
         
         for (int i = 0; i < GO->GetChildCount(); i++)
         {
             ShowChildData(GO->GetChildData(i));
         }
         ImGui::TreePop();
     }
 }

 void ModuleUI::DrawInspectorSpace(bool* active)
 {
     if (*active == false)
         return;

     if (!ImGui::Begin("Inspector", active))
     {
         ImGui::End();
         return;
     }

     {   //configuration space
         if (nodeClicked != -1 && nodeClicked != App->scene->rootNode->UUID)
         {
             GameObject* GO = App->scene->GetGO(nodeClicked);
            

             if (ImGui::CollapsingHeader(GO->name.c_str()))
             {  
                //-----------NODE PROPERTIES-------
                    bool gameObjectIsActive = GO->isActive;
                if (ImGui::Button("Delete Game Object"))
                     GO->toDestroy = true;

                if (ImGui::Checkbox("Is Active", &gameObjectIsActive))
                {
                    GO->isActive = gameObjectIsActive;
                }
                //-----------Components--------------
                for (int i = 0; i < GO->GetComponentCount(); i++)
                {
                     const Component* cmp = GO->GetComponent(i);
                     switch (cmp->type)
                     {
                        case (COMPONENT_TYPE::TRANSFORM):
                        {
                            c_Transform* transform = (c_Transform*)cmp;
                            c_Camera* camera = (c_Camera*)cmp;
                            if (ImGui::CollapsingHeader("Transform"))
                            {
                                if (!transform->isEmpty)
                                {
                                    // --- POSITION ---
                                    ImGui::Text("Position");

                                    ImGui::SameLine(100.0f);

                                    float3 translation = transform->GetLocalPosition();
                                    if (ImGui::DragFloat3("T", (float*)&translation, 0.05f, 0.0f, 0.0f, "%.3f", NULL))
                                    {
                                        transform->SetLocalPosition(translation);
                                        camera->SetAABB(translation);
                                    }

                                    // --- ROTATION ---
                                    ImGui::Text("Rotation");

                                    ImGui::SameLine(100.0f);

                                    float3 rotation = transform->GetLocalEulerRotation() * RADTODEG;
                                    if (ImGui::DragFloat3("R", (float*)&rotation, 1.0f, 0.0f, 0.0f, "%.3f", NULL))
                                    {
                                        transform->SetLocalRotation(rotation * DEGTORAD);
                                    }

                                    // --- SCALE ---
                                    ImGui::Text("Scale");

                                    ImGui::SameLine(100.0f);

                                    float3 scale = transform->GetLocalScale();
                                    if (ImGui::DragFloat3("S", (float*)&scale, 0.05f, 0.0f, 0.0f, "%.3f", NULL))
                                    {
                                        transform->SetLocalScale(scale);
                                    }
                                }
                            }
                            break;
                        }
                        case (COMPONENT_TYPE::MESH):
                        {
                            c_Mesh* mesh = (c_Mesh*)cmp;
                            if (ImGui::CollapsingHeader("Mesh"))
                            {
                                if (ImGui::Button("Delete Mesh"))
                                {
                                    GO->DeleteComponent((Component*)mesh);
                                    break;
                                }

                                bool meshActive = mesh->isActive;
                                if (ImGui::Checkbox("Is Active", &meshActive))
                                {
                                    mesh->isActive = meshActive;
                                }

                                if (!mesh->isEmpty)
                                {
                                    ImGui::Text("Vertex Count:"); ImGui::SameLine(); ImGui::TextColored(YELLOW, "%d", mesh->GetVertexCount());
                                }
                            }
                            break;
                        }
                        case (COMPONENT_TYPE::MATERIAL):
                        {
                            c_Material* material = (c_Material*)cmp;
                            if (ImGui::CollapsingHeader("Material"))
                            {
                                if (ImGui::Button("Delete Material"))
                                {
                                    GO->DeleteComponent((Component*)material);
                                    break;
                                }

                            bool materialActive = material->isActive;
                            if (ImGui::Checkbox("Is Active", &materialActive))
                            {
                                material->isActive = materialActive;
                            }
                                ImGui::Text("Path:"); ImGui::SameLine(); ImGui::TextColored(YELLOW, material->getPath());
                                ImGui::Text("Texture Sample:");
                                ImGui::Image((void*)material->getTextureID(), ImVec2(256, 256));
                            }
                            break;
                        }
                     }
                 }
                

             }
         }
         else
         {
             ImGui::Text("Select a valid node from the hierarchy window to display its properties");
         }  
     }
     ImGui::End();
 }

 void ModuleUI::DrawSceneSpace(bool* active)
 {
     if (*active == true)
     {
         ImGui::Begin("Scene", &activeScene, ImGuiWindowFlags_NoScrollbar);

         ImVec2 viewportSize = ImGui::GetCurrentWindow()->Size;


         viewPortX = viewportSize.x;
         viewPortY = viewportSize.y;

         if (viewportSize.x != lastViewportSize.x || viewportSize.y != lastViewportSize.y)
         {
             //App->camera->aspectRatio = viewportSize.x / viewportSize.y;
             App->camera->RecalculateProjection();
         }
         lastViewportSize = viewportSize;
         ImGui::Image((ImTextureID)App->viewPort->texture, viewportSize, ImVec2(0, 1), ImVec2(1, 0));

         if (ImGui::BeginDragDropTarget())
         {
             if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
             {
                 const char* path = (const char*)payload->Data;
                 std::string full = BROWSER_PATH + currentFolder + '/' + path;
                 App->importer->ImportAndLoadScene(full.c_str());
             }
             ImGui::EndDragDropTarget();
         }

         ImGui::End();
     }
 }

void ModuleUI::AddLogFPS(float fps, float ms)
{
    static uint count = 0;

    if (count == LOG_LENGTH)
    {
        for (uint i = 0; i < LOG_LENGTH - 1; ++i)
        {
            fps_log[i] = fps_log[i + 1];
            ms_log[i] = ms_log[i + 1];
        }
    }
    else
        ++count;

    fps_log[count - 1] = fps;
    ms_log[count - 1] = ms;
}

void ModuleUI::ShowDockingDisabledMessage()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("ERROR: Docking is not enabled! See Demo > Configuration.");
    ImGui::Text("Set io.ConfigFlags |= ImGuiConfigFlags_DockingEnable in your code, or ");
    ImGui::SameLine(0.0f, 0.0f);
    if (ImGui::SmallButton("click here")) 
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void ModuleUI::DrawBrowserSpace(bool* active)
{
    if (*active == false)
        return;

    std::vector<std::string> files_list;
    std::vector<std::string> dirs_list;

    if (ImGui::Begin("Folder Browser", active))
    {
        App->fileSystem->DiscoverFiles(BROWSER_PATH, files_list, dirs_list);

        for (int i = 0; i < dirs_list.size(); i++)
        {
            if (App->fileSystem->IsDirectory(dirs_list[i].c_str()))
            {
                if (ImGui::Button(dirs_list[i].c_str()))
                {
                    std::vector<std::string> files_list2;
                    std::vector<std::string> dirs_list2;
                    App->fileSystem->DiscoverFiles(dirs_list[i].c_str(), files_list2, dirs_list2);
                    currentFolder = dirs_list[i];
                    content_files = files_list2;
                }
            }
        }
        ImGui::End();
        return;
    }

    ImGui::End();
}

void ModuleUI::DrawContentBrowserSpace(bool* active)
{
    if (*active == false)
        return;

    std::vector<std::string> myContentFiles = content_files;
    std::string myCurrentFolder = currentFolder;

    if (ImGui::Begin("Content Browser", active))
    {
        ImGui::Text(myCurrentFolder.c_str());
        ImGui::Separator();

        static float padding = 16.0f;
        static float thumbnailSize = 128;
        float cellSize = thumbnailSize + padding;
        float panelWidth = ImGui::GetContentRegionAvail().x;

        int columnCount = (int)(panelWidth / cellSize);
        if (columnCount < 1)
            columnCount = 1;

        ImGui::Columns(columnCount, 0, false);

        for (int i = 0; i < myContentFiles.size(); i++)
        {
            ImGui::Button(myContentFiles[i].c_str(), { thumbnailSize,thumbnailSize });
            if (ImGui::BeginDragDropSource())
            {
                const char* arg = myContentFiles[i].c_str();
                int size = strlen(arg);
                ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", arg, size * sizeof(const char), ImGuiCond_Once);
                ImGui::EndDragDropSource();
            }

            ImGui::Text(myContentFiles[i].c_str());
            ImGui::NextColumn();
        }

        ImGui::Columns(1);

        ImGui::End();
        return;
    }

   

    ImGui::End();
}
