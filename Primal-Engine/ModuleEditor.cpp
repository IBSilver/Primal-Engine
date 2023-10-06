#include "ModuleEditor.h"
#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "ImGui/backends/imgui_impl_sdl.h"
#include "Application.h"

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
    io.ConfigFlags |= ImGuiWindowFlags_MenuBar;
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
    ImGui_ImplOpenGL3_Init();

    return true;
}

bool ModuleEditor::DrawEditor() {

    if (Initializated == false) {
        Initializated = true;
        //These parameters are initialized here since for some reason they arent corrrectly executed in the Init() function
        ImGui_ImplOpenGL3_Init();

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

        GLenum err = glewInit();
        // � check for errors
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

        //glMatrixMode(GL_PROJECTION);
        //glLoadIdentity();
    }

    bool ret = true;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Demo Menu
    //ImGui::ShowDemoWindow();

    // MainMenuBar
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Import")) {}
            if (ImGui::MenuItem("Close", "Ctrl+Q")) { ret = false; } // <-- Exit Code
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            //if(ImGui::Checkbox("Depth Test Enabled", &Depth_Test))
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Select"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("About")) { AboutOpen = !AboutOpen; }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (AboutOpen == true) {
        if (ImGui::Begin("About Us")) {
            ImGui::SeparatorText("ABOUT PRIMAL ENGINE:");
            ImGui::Text("Primal Engine v.0.0.1");
            ImGui::End();
        }
    }

    ImGui::PlotHistogram("FPS", mFPSLog.data(), mFPSLogSize);
    ImGui::DragInt("Graph size", &mFPSLogSize, 0.5f, 1, 100);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    //ImGui::ShowDemoWindow();

    //if (Depth_Test == true) {
    //    glEnable(GL_DEPTH_TEST);
    //}
    //if (Depth_Test == false) {
    //    glDisable(GL_DEPTH_TEST);
    //}

    return ret;
}

bool ModuleEditor::CleanUp() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
    
	return false;
}

void ModuleEditor::AddFPS(const float aFPS)
{
    if (mFPSLog.size() >= mFPSLogSize) {
        mFPSLog.erase(mFPSLog.begin());

    }

    mFPSLog.push_back(aFPS);


}