#include "ModuleEditor.h"
#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "ImGui/backends/imgui_impl_sdl.h"
#include "Application.h"
#include "Loader.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled) {

}

ModuleEditor::~ModuleEditor() {

}

bool ModuleEditor::Init() {

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    //io.ConfigFlags |= ImGuiWindowFlags_MenuBar;
    
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
    ImGui_ImplOpenGL3_Init();

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    LOG("Using Glew %s", glewGetString(GLEW_VERSION));
    // Should be 2.0

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glClearDepth(1.0f);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);

    LOG("Vendor: %s", glGetString(GL_VENDOR));
    LOG("Renderer: %s", glGetString(GL_RENDERER));
    LOG("OpenGL version supported %s", glGetString(GL_VERSION));
    LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    
    return true;
}

bool ModuleEditor::DrawEditor() {

    bool ret = true;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Demo Menu
    //ImGui::ShowDemoWindow();

    ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_PassthruCentralNode);

    // MainMenuBar
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Import")) {}
            if (ImGui::MenuItem("Close Engine")) 
            { 
                ret = false;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            bool Temp = Depth_Test;
            ImGui::Checkbox("Depth Test Enabled", &Depth_Test);
            if (Temp != Depth_Test)
            {
                if (Depth_Test == true) {
                glEnable(GL_DEPTH_TEST);
                }
                if (Depth_Test == false) {
                glDisable(GL_DEPTH_TEST);
                }
            }

            Temp = Cull;
            ImGui::Checkbox("Cull Enabled", &Cull);
            if (Temp != Cull)
            {
                if (Cull == true) {
                    glEnable(GL_CULL_FACE);
                }
                if (Cull == false) {
                    glDisable(GL_CULL_FACE);
                }
            }

            Temp = Lightning;
            ImGui::Checkbox("Lightning Enabled", &Lightning);
            if (Temp != Lightning)
            {
                if (Lightning == true) {
                    glEnable(GL_LIGHTING);
                }
                if (Lightning == false) {
                    glDisable(GL_LIGHTING);
                }
            }

            Temp = colorMaterial;
            ImGui::Checkbox("Color Material Enabled", &colorMaterial);
            if (Temp != colorMaterial)
            {
                if (colorMaterial == true) {
                    glEnable(GL_COLOR_MATERIAL);
                }
                if (colorMaterial == false) {
                    glDisable(GL_COLOR_MATERIAL);
                }
            }

            Temp = Texture2D;
            ImGui::Checkbox("2D Texture Enabled", &Texture2D);
            if (Temp != Texture2D)
            {
                if (Texture2D == true) {
                    glEnable(GL_TEXTURE_2D);
                }
                if (Texture2D == false) {
                    glDisable(GL_TEXTURE_2D);
                }
            }

            Temp = App->renderer3D->MeshLoader->GetNormals();
            ImGui::Checkbox("Normals Enabled", &Temp);
            if (Temp != App->renderer3D->MeshLoader->GetNormals())
            {
                App->renderer3D->MeshLoader->ChangeNormals();
            }

            Temp = App->renderer3D->MeshLoader->GetUVs();
            ImGui::Checkbox("UVs Enabled", &Temp);
            if (Temp != App->renderer3D->MeshLoader->GetUVs())
            {
                App->renderer3D->MeshLoader->ChangeUVs();
            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Windows"))
        {
            if (ImGui::MenuItem("Console")) Console = true;
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("About")) About = true;
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (Console) ConsoleLog();

    if (About) AboutWindow();

    if (About == true) {
        
    }

    ImGui::PlotHistogram("FPS", mFPSLog.data(), mFPSLogSize);
    ImGui::DragInt("Graph size", &mFPSLogSize, 0.5f, 1, 100);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    return ret;
}

bool ModuleEditor::CleanUp() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
    
	return false;
}

void ModuleEditor::ConsoleLog()
{
    ImGui::Begin("Console", &Console, ImGuiWindowFlags_DockNodeHost);
    {
        /*for (int i = 0; i < mConsoleLog.size(); i++)
        {
            ImGui::Text(mConsoleLog[i].c_str());
        }*/
    }
    ImGui::End();
}

void ModuleEditor::AddFPS(const float aFPS)
{
    if (mFPSLog.size() >= mFPSLogSize) {
        mFPSLog.erase(mFPSLog.begin());

    }

    mFPSLog.push_back(aFPS);
}

void ModuleEditor::AboutWindow()
{
    ImGui::Begin("About Us", &About, ImGuiWindowFlags_DockNodeHost);
    {
        ImGui::SeparatorText("ABOUT PRIMAL ENGINE");
        ImGui::Text("Primal Engine v.0.1");
        ImGui::Text("By Ivan Bermudez Sagra & Maksym Polupan");
        if (ImGui::MenuItem("Press here to check our repository"))
        {
            ShellExecute(NULL, "open", "https://github.com/IvanBSupc/Primal-Engine", 0, 0, SW_SHOWNORMAL);
        }
        ImGui::NewLine();

        ImGui::SeparatorText("LICENSE");

        ImGui::Text("MIT LICENSE");
        ImGui::NewLine();

        ImGui::Text("Copyright (c) 2023 IvanBSupc & Maksym203");
        ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy");
        ImGui::Text("of this software and associated documentation files (the 'Software'), to deal");
        ImGui::Text("in the Software without restriction, including without limitation the rights");
        ImGui::Text("to use, copy, modify, merge, publish, distribute, sublicense, and/or sell");
        ImGui::Text("copies of the Software, and to permit persons to whom the Software is");
        ImGui::Text("furnished to do so, subject to the following conditions:");
        ImGui::NewLine();
        ImGui::Text("The above copyright notice and this permission notice shall be included in all");
        ImGui::Text("copies or substantial portions of the Software.");
        ImGui::NewLine();
        ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
        ImGui::Text("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
        ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE");
        ImGui::Text("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
        ImGui::Text("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
        ImGui::Text("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");
        ImGui::NewLine();

    }
    ImGui::End();
}