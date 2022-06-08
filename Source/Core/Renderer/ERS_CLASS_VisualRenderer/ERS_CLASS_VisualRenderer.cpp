//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

#include <ERS_CLASS_VisualRenderer.h>



ERS_CLASS_VisualRenderer::ERS_CLASS_VisualRenderer(ERS_STRUCT_SystemUtils* SystemUtils, ERS_STRUCT_ProjectUtils* ProjectUtils, GLFWwindow* Window, Cursors3D* Cursors3D) {

    SystemUtils->Logger_->Log("Populating Renderer Member Pointers", 5);
    SystemUtils_ = SystemUtils;
    ProjectUtils_ = ProjectUtils;
    Window_ = Window;
    Cursors3D_ = Cursors3D;

    SystemUtils_->Logger_->Log("Initializing MeshRenderer Class", 5);
    MeshRenderer_ = std::make_unique<ERS_CLASS_MeshRenderer>(SystemUtils_);

    SystemUtils_->Logger_->Log("Initializing Viewport Overlay Subsystem", 5);
    ViewportOverlay_ = std::make_unique<ERS_CLASS_ViewportOverlay>(SystemUtils_, ProjectUtils_);

    SystemUtils_->Logger_->Log("Initializing Viewport Menu Subsystem", 5);
    ViewportMenu_ = std::make_unique<ERS_CLASS_ViewportMenu>(SystemUtils_, ProjectUtils_, &GameStartTime_, &IsEditorMode_, &Shaders_);

    ShadowMaps_ = std::make_unique<ERS_CLASS_ShadowMaps>(SystemUtils_, ProjectUtils_, MeshRenderer_.get());

    // DEFAULT MODES, CHANGE THIS LATER! --------------------------------
    IsEditorMode_ = true;
    

}

ERS_CLASS_VisualRenderer::~ERS_CLASS_VisualRenderer() {

    // Destroy Framebuffers
    for (int i = 0; (long)i < (long)Viewports_.size(); i++) {

        glDeleteFramebuffers(1, &Viewports_[i]->FramebufferObject);
        glDeleteTextures(1, &Viewports_[i]->FramebufferColorObject);
        glDeleteRenderbuffers(1, &Viewports_[i]->RenderbufferObject);

    }


    // Cleanup
    SystemUtils_->Logger_->Log("Cleaning Up OpenGL/GLFW", 6);
    glfwTerminate();

}


void ERS_CLASS_VisualRenderer::SetDefaultShader(int ShaderID) {
    
    DefaultShader_ = ShaderID;
}

void ERS_CLASS_VisualRenderer::SetOpenGLDefaults(ERS_STRUCT_OpenGLDefaults* Defaults) {

    OpenGLDefaults_ = Defaults;

}

void ERS_CLASS_VisualRenderer::UpdateViewports(float DeltaTime, ERS_CLASS_SceneManager* SceneManager) {

    // Set Depth Shader For Shadow System
    DepthMapShader_ = Shaders_[ERS_FUNCTION_FindShaderByName(std::string("_DepthMap"), &Shaders_)].get();


    // Close Any Viewports That Aren't All Open
    int ViewportsToClose = -1;
    for (int i = 0; (long)i < (long)Viewports_.size(); i++) {
        if (!*Viewports_[i]->Enabled) {
            ViewportsToClose = i;
        }
    }
    if (ViewportsToClose != -1) {
        DeleteViewport(ViewportsToClose);
    }


    // Generate Shadows
    //DepthMapShader_ = Shaders_[ERS_FUNCTION_FindShaderByName(std::string("Preview Shader"), &Shaders_)].get();
    ShadowMaps_->UpdateShadowMaps(DepthMapShader_);


    // Setup Vars
    glEnable(GL_DEPTH_TEST);
    CaptureCursor_ = false;
    CaptureIndex_ = -1;
    FrameNumber_++;



    RunTime_ = glfwGetTime() - GameStartTime_;
    SystemUtils_->ERS_CLASS_PythonInterpreterIntegration_->UpdateSystemInfoData(RunTime_);


    // Iterate Through Viewports
    for (int i = 0; (long)i < (long)Viewports_.size(); i++) {
        UpdateViewport(i, SceneManager, DeltaTime);
    }

    // Update Mouse Capture State
    if (CaptureCursor_) {
        glfwSetInputMode(Window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        glfwSetInputMode(Window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }





    // Handle Window Input
    for (int i = 0; (long)i < (long)Viewports_.size(); i++) {

        // Get Input Processor
        ERS_CLASS_InputProcessor* InputProcessorInstance = Viewports_[i]->Processor.get();

        bool CaptureEnabled = false;
        if ((CaptureIndex_ == i) && (!Cursors3D_->IsUsing())) {
            CaptureEnabled = true;
        }

        // Update Viewport Camera/Position/Etc.
        InputProcessorInstance->ProcessKeyboardInput(DeltaTime, CaptureEnabled);
        InputProcessorInstance->UpdateFramebuffer();
        InputProcessorInstance->UpdateMouse(CaptureEnabled);
        InputProcessorInstance->ProcessMouseScroll(CaptureEnabled);

    }
    CaptureCursor_ = false;

    // BIND To Default Framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);




    // RUN SCRIPTS WHEN NOT IN EDITOR MODE
    if (!IsEditorMode_) {

        for (unsigned long i = 0; i < SceneManager->Scenes_[SceneManager->ActiveScene_]->Models.size(); i++) {

            // Get Model
            ERS_STRUCT_Model* Model = SceneManager->Scenes_[SceneManager->ActiveScene_]->Models[i].get();

            // Go Through All Scripts In Model
            for (unsigned long x = 0; x < Model->AttachedScriptIndexes_.size(); x++) {

                long ScriptIndex = Model->AttachedScriptIndexes_[x];
                std::string Code = ProjectUtils_->ProjectManager_->Project_.Scripts[ScriptIndex].Code_;

                bool Status;
                if (x == (unsigned long)SelectedScript_) {
                    Status = SystemUtils_->ERS_CLASS_PythonInterpreterIntegration_->ExecuteModelScript(Code, Model, DebugLog_);
                } else {
                    Status = SystemUtils_->ERS_CLASS_PythonInterpreterIntegration_->ExecuteModelScript(Code, Model);
                }

                if (!Status) {
                    IsEditorMode_ = true;
                }
            }
        }

        for (unsigned long i = 0; i < SceneManager->Scenes_[SceneManager->ActiveScene_]->PointLights.size(); i++) {

            // Get Model
            ERS_STRUCT_PointLight* Target = SceneManager->Scenes_[SceneManager->ActiveScene_]->PointLights[i].get();

            // Go Through All Scripts In Model
            for (unsigned long x = 0; x < Target->AttachedScriptIndexes_.size(); x++) {

                long ScriptIndex = Target->AttachedScriptIndexes_[x];
                std::string Code = ProjectUtils_->ProjectManager_->Project_.Scripts[ScriptIndex].Code_;

                bool Status;
                if (x == (unsigned long)SelectedScript_) {
                    Status = SystemUtils_->ERS_CLASS_PythonInterpreterIntegration_->ExecutePointLightScript(Code, Target, DebugLog_);
                } else {
                    Status = SystemUtils_->ERS_CLASS_PythonInterpreterIntegration_->ExecutePointLightScript(Code, Target);
                }

                if (!Status) {
                    IsEditorMode_ = true;
                }
            }
        }

        for (unsigned long i = 0; i < SceneManager->Scenes_[SceneManager->ActiveScene_]->DirectionalLights.size(); i++) {

            // Get Model
            ERS_STRUCT_DirectionalLight* Target = SceneManager->Scenes_[SceneManager->ActiveScene_]->DirectionalLights[i].get();

            // Go Through All Scripts In Model
            for (unsigned long x = 0; x < Target->AttachedScriptIndexes_.size(); x++) {

                long ScriptIndex = Target->AttachedScriptIndexes_[x];
                std::string Code = ProjectUtils_->ProjectManager_->Project_.Scripts[ScriptIndex].Code_;

                bool Status;
                if (x == (unsigned long)SelectedScript_) {
                    Status = SystemUtils_->ERS_CLASS_PythonInterpreterIntegration_->ExecuteDirectionalLightScript(Code, Target, DebugLog_);
                } else {
                    Status = SystemUtils_->ERS_CLASS_PythonInterpreterIntegration_->ExecuteDirectionalLightScript(Code, Target);
                }

                if (!Status) {
                    IsEditorMode_ = true;
                }
            }
        }

        for (unsigned long i = 0; i < SceneManager->Scenes_[SceneManager->ActiveScene_]->SpotLights.size(); i++) {

            // Get Model
            ERS_STRUCT_SpotLight* Target = SceneManager->Scenes_[SceneManager->ActiveScene_]->SpotLights[i].get();

            // Go Through All Scripts In Model
            for (unsigned long x = 0; x < Target->AttachedScriptIndexes_.size(); x++) {

                long ScriptIndex = Target->AttachedScriptIndexes_[x];
                std::string Code = ProjectUtils_->ProjectManager_->Project_.Scripts[ScriptIndex].Code_;

                bool Status;
                if (x == (unsigned long)SelectedScript_) {
                    Status = SystemUtils_->ERS_CLASS_PythonInterpreterIntegration_->ExecuteSpotLightScript(Code, Target, DebugLog_);
                } else {
                    Status = SystemUtils_->ERS_CLASS_PythonInterpreterIntegration_->ExecuteSpotLightScript(Code, Target);
                }

                if (!Status) {
                    IsEditorMode_ = true;
                }
            }
        }

    }

    // Reset Selected Script
    SelectedScript_ = -1;



}

void ERS_CLASS_VisualRenderer::SetScriptDebug(int Index, std::vector<std::string>* DebugLog) {

    SelectedScript_ = Index;
    DebugLog_ = DebugLog;

}

void ERS_CLASS_VisualRenderer::UpdateViewport(int Index, ERS_CLASS_SceneManager* SceneManager, float DeltaTime, bool DrawCursor) {


    // Render To ImGui
    ImGuiWindowFlags Flags = ImGuiWindowFlags_None;
    if (Viewports_[Index]->MenuEnabled) {
        Flags |= ImGuiWindowFlags_MenuBar;
    }

    bool Visible = ImGui::Begin(Viewports_[Index]->Name.c_str(), Viewports_[Index]->Enabled.get(), Flags);

    // Set Default Window Size
    ImGui::SetWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);


    // Check If Window Visible
    if (Visible) {

        // Handle Viewport Menu
        if (ImGui::IsKeyPressed(GLFW_KEY_GRAVE_ACCENT)) {
            Viewports_[Index]->MenuEnabled = !Viewports_[Index]->MenuEnabled;
        }
        ViewportMenu_->DrawMenu(Viewports_[Index].get());


        // Calculate Window Position
        ImVec2 vMin = ImGui::GetWindowContentRegionMin();
        ImVec2 vMax = ImGui::GetWindowContentRegionMax();

        vMin.x += ImGui::GetWindowPos().x;
        vMin.y += ImGui::GetWindowPos().y;
        vMax.x += ImGui::GetWindowPos().x;
        vMax.y += ImGui::GetWindowPos().y;

        int WindowTopLeftCornerX = vMin.x;
        int WindowTopLeftCornerY = vMin.y;
        int WindowBottomRightCornerX = vMax.x;
        int WindowBottomRightCornerY = vMax.y;


        // Get Window Input
        int RenderWidth = WindowBottomRightCornerX - WindowTopLeftCornerX;
        int RenderHeight = WindowBottomRightCornerY - WindowTopLeftCornerY;


        // Get Mouse Pos
        int MousePositionX = ImGui::GetMousePos().x;
        int MousePositionY = ImGui::GetMousePos().y;

        // Check If In Bounding Box
        bool MouseXInRange = (MousePositionX >= WindowTopLeftCornerX) && (MousePositionX < WindowBottomRightCornerX);
        bool MouseYInRange = (MousePositionY >= WindowTopLeftCornerY) && (MousePositionY < WindowBottomRightCornerY);
        bool MouseInRange = MouseXInRange && MouseYInRange;
        

        // Check If Input Enabled
        bool EnableCameraMovement = !Cursors3D_->IsUsing();
        if (ImGui::IsKeyDown(341)) { // Bind to left control key
            EnableCameraMovement = true;
        }

        bool EnableCursorCapture;
        if (EnableCameraMovement && ImGui::IsWindowFocused() && (MouseInRange | Viewports_[Index]->WasSelected) && (glfwGetMouseButton(Window_, 0) == GLFW_PRESS)) {
            CaptureCursor_ = true;
            EnableCursorCapture = true;
            CaptureIndex_ = Index;
            Viewports_[Index]->WasSelected = true;
        } else {
            EnableCursorCapture = false;
            Viewports_[Index]->WasSelected = false;
        }





        glViewport(0, 0, RenderWidth, RenderHeight);
        glScissor(0, 0, RenderWidth, RenderHeight);


        // Resize Viewport If Needed
        if ((RenderWidth != Viewports_[Index]->Width) || (RenderHeight != Viewports_[Index]->Height)) {
            ResizeViewport(Index, RenderWidth, RenderHeight);
        }


        // Bind To Framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, Viewports_[Index]->FramebufferObject);

        // Rendering Commands Here
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update Camera
        float AspectRatio = (float)RenderWidth / (float)RenderHeight;
        Viewports_[Index]->Camera->SetAspectRatio(AspectRatio);
        glm::mat4 projection = Viewports_[Index]->Camera->GetProjectionMatrix();
        glm::mat4 view = Viewports_[Index]->Camera->GetViewMatrix();
        



        // Use Shader
        int ShaderIndex = Viewports_[Index]->ShaderIndex;
        Shaders_[ShaderIndex]->MakeActive();

        // Update Shaders
        UpdateShader(ShaderIndex, DeltaTime, RenderWidth, RenderHeight, SceneManager, Viewports_[Index]->Camera.get());
        Shaders_[ShaderIndex]->SetMat4("projection", projection);
        Shaders_[ShaderIndex]->SetMat4("view", view);
        Shaders_[ShaderIndex]->SetBool("GammaCorrectionEnabled_", Viewports_[Index]->GammaCorrection);
        Shaders_[ShaderIndex]->SetBool("HDREnabled_", Viewports_[Index]->HDREnabled_);
        Shaders_[ShaderIndex]->SetFloat("Exposure_", Viewports_[Index]->Exposure_);
        Shaders_[ShaderIndex]->SetFloat("Gamma_", Viewports_[Index]->Gamma_);
        


        // Update Cursor If Selection Changed
        if (SceneManager->Scenes_[SceneManager->ActiveScene_]->HasSelectionChanged && DrawCursor) {

            // Get Selected Model
            int SelectedObject = SceneManager->Scenes_[SceneManager->ActiveScene_]->SelectedObject;

            // Get LocRotScale
            glm::vec3 Position;        
            glm::vec3 Rotation;      
            glm::vec3 Scale;
            bool HasRotation = false;
            bool HasScale = false;

            if (SceneManager->Scenes_[SceneManager->ActiveScene_]->SceneObjects_[SelectedObject].Type_ == std::string("Model")) {
                unsigned long Index = SceneManager->Scenes_[SceneManager->ActiveScene_]->SceneObjects_[SelectedObject].Index_;
                Position = SceneManager->Scenes_[SceneManager->ActiveScene_]->Models[Index]->ModelPosition;        
                Rotation = SceneManager->Scenes_[SceneManager->ActiveScene_]->Models[Index]->ModelRotation;        
                Scale = SceneManager->Scenes_[SceneManager->ActiveScene_]->Models[Index]->ModelScale;
                HasRotation = true;
                HasScale = true;
            } else if (SceneManager->Scenes_[SceneManager->ActiveScene_]->SceneObjects_[SelectedObject].Type_ == std::string("PointLight")) {
                unsigned long Index = SceneManager->Scenes_[SceneManager->ActiveScene_]->SceneObjects_[SelectedObject].Index_;
                Position = SceneManager->Scenes_[SceneManager->ActiveScene_]->PointLights[Index]->Pos;        
            } else if (SceneManager->Scenes_[SceneManager->ActiveScene_]->SceneObjects_[SelectedObject].Type_ == std::string("DirectionalLight")) {
                unsigned long Index = SceneManager->Scenes_[SceneManager->ActiveScene_]->SceneObjects_[SelectedObject].Index_;
                Position = SceneManager->Scenes_[SceneManager->ActiveScene_]->DirectionalLights[Index]->Pos;        
                Rotation = SceneManager->Scenes_[SceneManager->ActiveScene_]->DirectionalLights[Index]->Rot;    
                HasRotation = true;    
            } else if (SceneManager->Scenes_[SceneManager->ActiveScene_]->SceneObjects_[SelectedObject].Type_ == std::string("SpotLight")) {
                unsigned long Index = SceneManager->Scenes_[SceneManager->ActiveScene_]->SceneObjects_[SelectedObject].Index_;
                Position = SceneManager->Scenes_[SceneManager->ActiveScene_]->SpotLights[Index]->Pos;        
                Rotation = SceneManager->Scenes_[SceneManager->ActiveScene_]->SpotLights[Index]->Rot;    
                HasRotation = true;    
            }

            // Set Cursor Position        
            Cursors3D_->SetLocRotScale(Position, Rotation, Scale, HasRotation, HasScale);

            // Indicate Selection Hasn't Changed
            SceneManager->Scenes_[SceneManager->ActiveScene_]->HasSelectionChanged = false;
        }





        // glUniform1i(glGetUniformLocation(Shaders_[ShaderIndex]->ShaderProgram_, "DepthMap"), 9);
        // glActiveTexture(GL_TEXTURE9);
        // glBindTexture(GL_TEXTURE_2D,  SceneManager->Scenes_[SceneManager->ActiveScene_]->DirectionalLights[0]->DepthMap.DepthMapTextureID);
        Shaders_[ShaderIndex]->SetMat4("LightSpaceMatrix", SceneManager->Scenes_[SceneManager->ActiveScene_]->DirectionalLights[0]->LightSpaceMatrix);
        Shaders_[ShaderIndex]->SetInt("Index", SceneManager->Scenes_[SceneManager->ActiveScene_]->DirectionalLights[0]->DepthMap.DepthMapTextureIndex);
        

        // OpenGLDefaults_->DefaultTexture_ = SceneManager->Scenes_[SceneManager->ActiveScene_]->DirectionalLights[0]->DepthMap.DepthMapTextureID;


        glUniform1i(glGetUniformLocation(Shaders_[ShaderIndex]->ShaderProgram_, "DepthMapArray"), 8);
        glActiveTexture(GL_TEXTURE8);
        glBindTexture(GL_TEXTURE_2D_ARRAY, ShadowMaps_->ERS_CLASS_DepthMaps_->DepthTextureArrayID_);

        // int W, H;
        // glGetTexLevelParameteriv(GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_WIDTH, &W);
        // glGetTexLevelParameteriv(GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_WIDTH, &H);

        // std::cout<<W<<"|"<<H<<std::endl;
        



        // Render
        
        //MeshRenderer_->RenderSceneNoTextures(SceneManager->Scenes_[SceneManager->ActiveScene_].get(), Shaders_[ShaderIndex].get());
        MeshRenderer_->RenderScene(SceneManager->Scenes_[SceneManager->ActiveScene_].get(), OpenGLDefaults_, Shaders_[ShaderIndex].get());
        

        if (Viewports_[Index]->GridEnabled) {
            Viewports_[Index]->Grid->DrawGrid(view, projection, Viewports_[Index]->Camera->Position_);
        }
        if (Viewports_[Index]->LightIcons) {
            Viewports_[Index]->LightIconRenderer->Draw(Viewports_[Index]->Camera.get(), SceneManager);
        }




        // Render Framebuffer To Window
        ImGui::GetWindowDrawList()->AddImage(
            (void*)(intptr_t)Viewports_[Index]->FramebufferColorObject,
            ImGui::GetCursorScreenPos(),
            ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetWindowSize().x, ImGui::GetCursorScreenPos().y + ImGui::GetWindowSize().y),
            ImVec2(0, 1),
            ImVec2(1, 0)        
        );


        // Draw 3D Cursor
        if (Cursors3D_->HasStateChanged()) {
            if (ImGui::IsWindowHovered()) {
                ActiveViewportCursorIndex_ = Index;
            }
        }

        bool DrawCursor;
        Cursors3D_->SetGridSnap(Viewports_[Index]->GridSnapAmountTranslate_, Viewports_[Index]->GridSnapAmountRotate_, Viewports_[Index]->GridSnapAmountScale_);
        if (Cursors3D_->IsUsing() && (ActiveViewportCursorIndex_ == Index)) {
            DrawCursor = true;
        } else if (!Cursors3D_->IsUsing()) {
            DrawCursor = true;
        } else {
            DrawCursor = false;
        }

        if (DrawCursor) {
            Cursors3D_->Draw(Viewports_[Index]->Camera.get(), EnableCursorCapture, Viewports_[Index]->ShowCube, Viewports_[Index]->GizmoEnabled);
        } else {
            Cursors3D_->Draw(Viewports_[Index]->Camera.get(), false, Viewports_[Index]->ShowCube, false);

        }



        // Update Selected Object
        if (!Cursors3D_->HasObjectChanged_) {
            SceneManager->UpdateLocRotScale(Cursors3D_->Pos_, Cursors3D_->Rot_, Cursors3D_->Scale_);
        } else {
            Cursors3D_->HasObjectChanged_ = false;
            
        }


        ViewportOverlay_->DrawOverlay(Viewports_[Index].get());


    }
    

    ImGui::End();
}

void ERS_CLASS_VisualRenderer::ResizeViewport(int Index, int Width, int Height) {

    // Update Render Color Buffer Size
    glBindTexture(GL_TEXTURE_2D, Viewports_[Index]->FramebufferColorObject);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);


    // Update RBO Size
    glBindRenderbuffer(GL_RENDERBUFFER, Viewports_[Index]->RenderbufferObject);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width, Height);

}

void ERS_CLASS_VisualRenderer::DeleteViewport(int Index) {

    // Log Deletion
    SystemUtils_->Logger_->Log(std::string(std::string("Destroying Viewport '") + Viewports_[Index]->Name + std::string("'")).c_str(), 5);

    // Cleanup OpenGL Objects
    glDeleteFramebuffers(1, &Viewports_[Index]->FramebufferObject);
    glDeleteTextures(1, &Viewports_[Index]->FramebufferColorObject);
    glDeleteRenderbuffers(1, &Viewports_[Index]->RenderbufferObject);

    // Delete Viewport Struct
    Viewports_.erase(Viewports_.begin() + Index);

}

void ERS_CLASS_VisualRenderer::CreateViewport() {

    // Generate Name
    std::string Name = std::string("Viewport ") + std::to_string(Viewports_.size() + 1);

    // Create Viewport
    CreateViewport(Name);

}

void ERS_CLASS_VisualRenderer::CreateViewport(std::string ViewportName) {


    // Log Creation
    SystemUtils_->Logger_->Log(std::string(std::string("Creating New Viewport '") + ViewportName + std::string("'")).c_str(), 5);


    // Create Viewport Struct
    std::shared_ptr<ERS_STRUCT_Viewport> Viewport = std::make_shared<ERS_STRUCT_Viewport>();


    // Populate Viewport Struct
    Viewport->ShaderIndex = DefaultShader_;
    Viewport->Camera = std::make_unique<ERS_STRUCT_Camera>();
    Viewport->Grid = std::make_unique<ERS_CLASS_Grid>(SystemUtils_, Shaders_[ERS_FUNCTION_FindShaderByName(std::string("_Grid"), &Shaders_)].get());
    Viewport->LightIconRenderer = std::make_unique<ERS_CLASS_LightIconRenderer>(OpenGLDefaults_, SystemUtils_, Shaders_[ERS_FUNCTION_FindShaderByName(std::string("_LightIcon"), &Shaders_)].get()); //Set TO Shader 19 For Billboard Shader, Temp. Disabled As It Doesn't Work ATM
    Viewport->Name = ViewportName;
    
    Viewport->Width = 1;
    Viewport->Height = 1;

    Viewport->WasSelected = false;
    Viewport->Enabled = std::make_unique<bool>(true);



    // Create Input Processor
    SystemUtils_->Logger_->Log("Creating New Input Processor", 4);
    Viewport->Processor = std::make_unique<ERS_CLASS_InputProcessor>(Viewport->Camera.get(), Window_);

    // Create Framebuffer
    unsigned int FramebufferObject;
    SystemUtils_->Logger_->Log("Creating Framebuffer Object", 4);
    glGenFramebuffers(1, &FramebufferObject);


    // Bind To Framebuffer
    SystemUtils_->Logger_->Log("Binding To Framebuffer Object", 4);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferObject);
    Viewport->FramebufferObject = FramebufferObject;


    // Create RenderTexture
    unsigned int FramebufferColorObject;
    SystemUtils_->Logger_->Log("Creating Render Texture", 4);
    glGenTextures(1, &FramebufferColorObject);
    glBindTexture(GL_TEXTURE_2D, FramebufferColorObject);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 800, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); // NOTE: THIS MUST HAPPEN ON RESIZE!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Viewport->FramebufferColorObject = FramebufferColorObject;


    // Attach Texture To Framebuffer
    SystemUtils_->Logger_->Log("Attaching Texture To Framebuffer", 4);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FramebufferColorObject, 0);


    // Create Render Buffer
    unsigned int RenderbufferObject;
    SystemUtils_->Logger_->Log("Creating Render Buffer Object", 5);
    glGenRenderbuffers(1, &RenderbufferObject);
    glBindRenderbuffer(GL_RENDERBUFFER, RenderbufferObject);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 800); // RESIZE THIS WITH THE WINDOW!


    // Attach Renderbuffer to Depth And Stencil Attachment
    SystemUtils_->Logger_->Log("Attaching Render Buffer Object To Depth Stencil", 5);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RenderbufferObject);
    Viewport->RenderbufferObject = RenderbufferObject;


    // Check Framebuffer Status
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {

        // Log Error
        SystemUtils_->Logger_->Log("Failed To Initialize Framebuffer", 9);
    }


    // Add To Viewports Vector
    Viewports_.push_back(Viewport);

}

void ERS_CLASS_VisualRenderer::UpdateShader(int ShaderIndex, float DeltaTime, int RenderWidth, int RenderHeight, ERS_CLASS_SceneManager*SceneManager, ERS_STRUCT_Camera* Camera) {

    /**

    -- Current list of supported shader params: 

    uniform float Time; // Time since program started in seconds
    uniform float FrameTime; // Render Time Of The Frame
    uniform int FrameNumber; // Number of the frame, counts up from zero
    uniform vec2 ViewportRes; // XY Resolution of the viewport

    **/


    // Get Pointer to Shader
    ERS_STRUCT_Shader* ActiveShader = Shaders_[ShaderIndex].get();

    // Set Metadata Params
    float Time = glfwGetTime();
    ActiveShader->SetFloat("Time", Time);

    ActiveShader->SetFloat("FrameTime", DeltaTime);
    ActiveShader->SetInt("FrameNumber", FrameNumber_);
    ActiveShader->SetVec2("ViewportRes", RenderWidth, RenderHeight);
    ActiveShader->SetVec3("CameraPosition", Camera->Position_);



    // ---- SEND LIGHTING INFORMATION TO SHADERS ---- //
    // NOTE: Due to limitations with shaders, the maximum number of lights is as follows (per object) 
    // Directional lights: 4
    // Point Lights: 32
    // Spot Lights: 16
    // TO DO BELOW:
    // When these limitations are reached, ERS will remove the lights that are farthest from the object

    // Prepare To Handle Lights
    //const int DirectionalLightLimit = 4;
    //const int PointLightLimit = 64;
    //const int SpotLightLimit = 32;


    ERS_STRUCT_Scene* ActiveScene = SceneManager->Scenes_[SceneManager->ActiveScene_].get();


    // ~-------------------------------------------------------------------!!!!!!!!!!!!!!!FIXME: IMPLEMENT SYSTEM TO USE THE LIGHTS CLOSEST TO THE OBJECT !!!!!!!!!!!!!!!!!!!!!~----------------------------------- //


    // Directional Lights
    int NumberDirectionalLights = ActiveScene->DirectionalLights.size();
    ActiveShader->SetInt("NumberDirectionalLights", NumberDirectionalLights);
    for (int i = 0; i < NumberDirectionalLights; i++) {
    
        std::string UniformName = std::string("DirectionalLights[") + std::to_string(i) + std::string("]");
        
        ActiveShader->SetVec3((UniformName + std::string(".Direction")).c_str(), ActiveScene->DirectionalLights[i]->Rot);
        ActiveShader->SetVec3((UniformName + std::string(".Color")).c_str(), ActiveScene->DirectionalLights[i]->Color);
        ActiveShader->SetFloat((UniformName + std::string(".Intensity")).c_str(), ActiveScene->DirectionalLights[i]->Intensity);


    
    }

    // Point Lights
    int NumberPointLights = ActiveScene->PointLights.size();
    ActiveShader->SetInt("NumberPointLights", NumberPointLights);
    for (int i = 0; i < NumberPointLights; i++) {
    
        std::string UniformName = std::string("PointLights[") + std::to_string(i) + std::string("]");

        ActiveShader->SetVec3((UniformName + std::string(".Position")).c_str(), ActiveScene->PointLights[i]->Pos);
        ActiveShader->SetFloat((UniformName + std::string(".Intensity")).c_str(), ActiveScene->PointLights[i]->Intensity);
        ActiveShader->SetVec3((UniformName + std::string(".Color")).c_str(), ActiveScene->PointLights[i]->Color);
    
    }


    // Spot Lights
    int NumberSpotLights = ActiveScene->SpotLights.size();
    ActiveShader->SetInt("NumberSpotLights", NumberSpotLights);
    for (int i = 0; i < NumberSpotLights; i++) {
    
        std::string UniformName = std::string("SpotLights[") + std::to_string(i) + std::string("]");

        ActiveShader->SetVec3((UniformName + std::string(".Position")).c_str(), ActiveScene->SpotLights[i]->Pos);
        ActiveShader->SetVec3((UniformName + std::string(".Direction")).c_str(), ActiveScene->SpotLights[i]->Rot);
        ActiveShader->SetFloat((UniformName + std::string(".Intensity")).c_str(), ActiveScene->SpotLights[i]->Intensity);
        ActiveShader->SetFloat((UniformName + std::string(".CutOff")).c_str(), ActiveScene->SpotLights[i]->CutOff);
        ActiveShader->SetFloat((UniformName + std::string(".OuterCutOff")).c_str(), ActiveScene->SpotLights[i]->OuterCutOff);
        ActiveShader->SetVec3((UniformName + std::string(".Color")).c_str(), ActiveScene->SpotLights[i]->Color);

    }



    ActiveShader->SetFloat("Shinyness", 32.0f);


}

