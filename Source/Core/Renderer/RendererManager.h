//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This is the renderer manager.
    Additonal Notes: None
    Date Created: 2021-11-01
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <yaml-cpp/yaml.h>

#include <FreeImage.h>

// Internal Libraries (BG convention: use <> instead of "")
#include <ERS_CLASS_LoggingSystem.h>
#include <ERS_CLASS_VisualRenderer.h>
#include <IOManager.h>
#include <ERS_STRUCT_Camera.h>

#include <ERS_CLASS_ShaderLoader.h>
#include <Framebuffer.h>
#include <GUI.h>

#include <ERS_ModelWriter.h>
#include <ERS_ModelLoader.h>

#include <ERS_Editor_3DCursor.h>

#include <ERS_STRUCT_IOData.h>
#include <ERS_STRUCT_SystemUtils.h>
#include <ERS_STRUCT_ProjectUtils.h>
#include <ERS_STRUCT_HumanInputDeviceUtils.h>



static ERS_STRUCT_Camera Camera_(glm::vec3(0.0f, 0.0f, 3.0f));


/**
 * @brief This class manages all the rendering systems including the visual, auditory and other sensory rendering systems. 
 * 
 */
class RendererManager {

    private:

        // Member Variables
        std::shared_ptr<ERS_CLASS_VisualRenderer> VisualRenderer_; /**<Pointer To VisualRenderer Instance*/
        std::shared_ptr<IOManager> IOManager_; /**<Pointer To IOManager Instance*/
        GLFWwindow* Window_; /**<GLFW Window Instance*/
        std::shared_ptr<ERS_CLASS_ShaderLoader> ShaderLoader_; /**<Pointer To Shader Loader Instnace*/
        std::shared_ptr<FramebufferManager> FramebufferManager_; /**<Pointer To Framebuffer Manager Instnace*/
        std::shared_ptr<ERS_STRUCT_Shader> Shader_; /**<ERS Shader Object (Main System Shader)*/
        std::shared_ptr<GUISystem> GuiSystem_; /**<Instance of GUI System*/
        std::shared_ptr<Cursors3D> Cursors3D_; /**<Setup 3D Cursor Class*/
        
        std::shared_ptr<ERS_STRUCT_SystemUtils> SystemUtils_; /**<Copy Of Pointer Helper Struct*/
        std::shared_ptr<ERS_STRUCT_ProjectUtils> ProjectUtils_; /**<Copy Of Project Utils Struct*/
        std::shared_ptr<ERS_STRUCT_HumanInputDeviceUtils> HIDUtils_; /**<Copy Of Pointer To HIDM Struct*/


        // Control Values
        int WindowWidth_; /**<GLFW Window Width Varaible*/
        int WindowHeight_; /**<GLFW Window Height Varaible*/
        const char* WindowTitle_; /**GLFW Window Title Variable*/


        /**
         * @brief Initialize And Create A GLFW Window
         * 
         */
        void InitializeGLFW();


    public:

        /**
         * @brief Initializes all renderers that have been enabled in the configuration file.
         * 
         * @param SystemConfiguration 
         * @param Logger 
         * @param SystemShouldRun 
         */
        RendererManager(std::shared_ptr<ERS_STRUCT_SystemUtils> SystemUtils, std::shared_ptr<ERS_STRUCT_ProjectUtils> ProjectUtils, std::shared_ptr<ERS_STRUCT_HumanInputDeviceUtils> HIDUtils);

        /**
         * @brief Shuts down the renderers and cleans up variables.
         * 
         */
        ~RendererManager();

        /**
         * @brief Called by the main loop, updates all rendered outputs.
         * 
         */
        void UpdateLoop(float DeltaTime);




};

