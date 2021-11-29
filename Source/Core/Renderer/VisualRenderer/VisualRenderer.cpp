//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This file contains the visual renderer.
    Additonal Notes: None
    Date Created: 2021-11-20
*/

#include <VisualRenderer.h>



// Visual Rendere constructor
VisualRenderer::VisualRenderer (YAML::Node *SystemConfiguration, LoggerClass *Logger) {

    // Create Pointers
    Logger->Log("Populating Renderer Member Pointers", 5);
    SystemConfiguration_ = SystemConfiguration;
    Logger_ = Logger;

    // Initialize OpenGL
    Logger_->Log("Initializing OpenGL", 5);
    InitializeOpenGL();

}

VisualRenderer::~VisualRenderer() {

    // Destroy Framebuffers
    for (int i = 0; i < FramebufferObjects_.size(); i++) {

        glDeleteFramebuffers(1, &FramebufferObjects_[i]);
        glDeleteTextures(1, &FramebufferColorObjects_[i]);
        glDeleteRenderbuffers(1, &RenderbufferObjects_[i]);

    }


    // Cleanup
    Logger_->Log("Cleaning Up OpenGL/GLFW", 0);
    glfwTerminate();

}

void VisualRenderer::InitializeOpenGL() {


    // Setup GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        Logger_->Log("Failed To Initialize GLAD", 10);
    }

}

void VisualRenderer::UpdateViewports(float DeltaTime, SceneManager *SceneManager) {

    // Enable Depth Test
    glEnable(GL_DEPTH_TEST);

    // Iterate Through Viewports
    for (int i = 0; i<Shaders_.size(); i++) {

        UpdateViewport(i, SceneManager, DeltaTime, 400, 200);

    }


    // BIND To Default Framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

void VisualRenderer::UpdateViewport(int Index, SceneManager *SceneManager, float DeltaTime, float RenderWidth, float RenderHeight) {


    // RENDER TO IMGUI
    // RESIZE IF NEEDED!

    // Bind To Framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferObjects_[Index]);

    // Rendering Commands Here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use Shader
    Shaders_[Index]->MakeActive();


    // Update Camera
    float AspectRatio = (float)RenderWidth / (float)RenderHeight;
    glm::mat4 projection = glm::perspective(glm::radians(Cameras_[Index]->Zoom), AspectRatio, 0.1f, 100.0f);
    glm::mat4 view = Cameras_[Index]->GetViewMatrix();
    Shaders_[Index]->SetMat4("projection", projection);
    Shaders_[Index]->SetMat4("view", view);


    // Draw Models
    SceneManager->Render(Shaders_[Index]);

    // Render Framebuffer To Window
    ImGui::Begin(ViewportNames_[Index].c_str());
    
    ImGui::GetWindowDrawList()->AddImage(
        (void*)(intptr_t)FramebufferObjects_[Index],
        ImVec2(ImGui::GetCursorScreenPos()),
        ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetWindowSize().x,
            ImGui::GetCursorScreenPos().y + ImGui::GetWindowSize().y),
        ImVec2(0, 1),
        ImVec2(1, 0)        
    );

    ImGui::End();

}

// ADD DESTROY VIEWPORT FUNCTION!

void VisualRenderer::CreateViewport(ERS_OBJECT_SHADER *Shader, std::string ViewportName, ERS_OBJECT_CAMERA_NOCLIP *Camera) {

    // Append To Vectors
    Shaders_.push_back(Shader);
    Cameras_.push_back(Camera);
    ViewportNames_.push_back(ViewportName);



    // Create Framebuffer
    unsigned int FramebufferObject;
    Logger_->Log("Creating Framebuffer Object", 4);
    glGenFramebuffers(1, &FramebufferObject);

    // Bind To Framebuffer
    Logger_->Log("Binding To Framebuffer Object", 4);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferObject);
    FramebufferObjects_.push_back(FramebufferObject);


    // Create RenderTexture
    unsigned int FramebufferColorObject;
    Logger_->Log("Creating Render Texture", 4);
    glGenTextures(1, &FramebufferColorObject);
    glBindTexture(GL_TEXTURE_2D, FramebufferColorObject);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 400, 200, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); // NOTE: THIS MUST HAPPEN ON RESIZE!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    FramebufferColorObjects_.push_back(FramebufferColorObject);

    // Attach Texture To Framebuffer
    Logger_->Log("Attaching Texture To Framebuffer", 4);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FramebufferColorObject, 0);

    // Create Render Buffer
    unsigned int RenderbufferObject;
    Logger_->Log("Creating Render Buffer Object", 5);
    glGenRenderbuffers(1, &RenderbufferObject);
    glBindRenderbuffer(GL_RENDERBUFFER, RenderbufferObject);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 400, 200); // RESIZE THIS WITH THE WINDOW!

    // Attach Renderbuffer to Depth And Stencil Attachment
    Logger_->Log("Attaching Render Buffer Object To Depth Stencil", 5);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RenderbufferObject);
    RenderbufferObjects_.push_back(RenderbufferObject);

    // Check Framebuffer Status
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {

        // Log Error
        Logger_->Log("Failed To Initialize Framebuffer", 9);
    }


}