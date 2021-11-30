//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This is responsible for providing the gui.
    Documentation Status: Complete
    Additonal Notes: None
    Date Created: 2021-01-25
*/

#include <GUI.h>


// GUISystem Constructor
GUISystem::GUISystem(LoggerClass* Logger, GLFWwindow* Window, bool *SystemShouldRun) {

    // Create Local Pointer
    Logger_ = Logger;
    Window_ = Window;
    SystemShouldRun_ = SystemShouldRun;

    // Initialize ImGui
    Logger_->Log("Initializing DearImGui GUI Library", 5);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Initializing Theme Loader
    ThemeManager_ = new ThemeManager(Logger_);

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(Window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");







}

// GUISystem Destructor
GUISystem::~GUISystem() {

    // Log Destructor Call
    Logger_->Log("GUISystem Destructor Called", 6);

    // Deinit ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

}

// Update GUI
void GUISystem::UpdateGUI() {

    // Get Window Width, Height
    float Width;
    float Height;
    glfwGetWindowContentScale(Window_, &Width, &Height);


    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();



    ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar;
    flags |= ImGuiWindowFlags_NoDocking;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", 0, flags);
    ImGui::PopStyleVar();

    // Setup Dockspace
    // ImGuiWindowFlags Flags = ImGuiWindowFlags_MenuBar;
    // Flags |= ImGuiWindowFlags_NoDocking;
    // ImGuiViewport* Viewport = ImGui::GetMainViewport();

    // ImGui::SetNextWindowPos(Viewport->Pos);
    // ImGui::SetNextWindowSize(Viewport->Size);
    // ImGui::SetNextWindowViewport(Viewport->ID);
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

    // Flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    // Flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    // ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    // ImGui::Begin("Dockspace", 0, Flags);
    // ImGui::PopStyleVar();

    
    // Add Main Menu
    if (ImGui::BeginMainMenuBar()) {


        if (ImGui::BeginMenu("File")) {

                if (ImGui::MenuItem("Color Theme")) {
                    
                }

                ImGui::Separator();

                if (ImGui::MenuItem("Exit")) {
                    MenuExitFunction();
                }

            
            ImGui::EndMenu();
        }


        ImGui::EndMainMenuBar();
    }


    ImGui::End();
    
        // static float f = 0.0f;
        // static int counter = 0;


        // ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        // ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        // if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        //     counter++;
        // ImGui::SameLine();
        // ImGui::Text("counter = %d", counter);


    ImGui::Begin("System Controls");


    // Wireframe Rendering Mode
    static bool OpenGLDrawLines = false;
    ImGui::Checkbox("Wireframe Rendering Mode", &OpenGLDrawLines);
    ImGui::NewLine();
    if (OpenGLDrawLines) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }


    // Rendering Background Clear Color
    static ImVec4 ClearColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    ImGui::ColorEdit4("Background Clear Color", (float*)&ClearColor);
    ImGui::NewLine();
    glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w);



    // End System Controls Window
    ImGui::End();




    // System Info Window
    ImGui::Begin("System Info");

    // FPS Counter
    ImGui::Text("System Framerate %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    // End System Info Window
    ImGui::End();




    // // create an ImGui window that covers the entire viewport, so that we can have a menu bar at the top of the applications
    // ImGui::SetNextWindowPos(ImVec2(0, 0));                                                  // always at the window origin
    // ImGui::SetNextWindowSize(ImVec2(Width, Height));    // always at the window size

    // ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoBringToFrontOnFocus |                 // we just want to use this window as a host for the menubar and docking
    //     ImGuiWindowFlags_NoNavFocus |                                                      // so turn off everything that would make it act like a window
    //     //ImGuiWindowFlags_NoDocking |
    //     ImGuiWindowFlags_NoTitleBar |
    //     ImGuiWindowFlags_NoResize |
    //     ImGuiWindowFlags_NoMove |
    //     ImGuiWindowFlags_NoCollapse |
    //     ImGuiWindowFlags_MenuBar |
    //     ImGuiWindowFlags_NoBackground;                                                      // we want our game content to show through this window, so turn off the background.

    // ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));   










    


}

// Update Frame
void GUISystem::UpdateFrame() {

    // Rendering
    ImGui::Render();

    // Draw The GUI
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
}

void GUISystem::MenuExitFunction() {

    // Shutdown System
    *SystemShouldRun_ = false;
    
}