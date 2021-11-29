//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This file contains the visual renderer.
    Additonal Notes: None
    Date Created: 2021-11-20
*/

#pragma once


// Standard Libraries (BG convention: use <> instead of "")
#include <iostream>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


// Internal Libraries (BG convention: use <> instead of "")
#include <LoggingSystem.h>
#include <Shader.h>
#include <Model.h>
#include <NoClip.h>
#include <TextureLoader.h>
#include <ModelLoader.h>
#include <SceneManager.h>
#include <SceneLoader.h>
#include <ShaderLoader.h>
#include <Grid.h>
#include <GUI.h>
#include <Framebuffer.h>




class VisualRenderer {

    private:

        // Member Variables
        YAML::Node *SystemConfiguration_; /**<Pointer to the system configuration YAML::Node*/
        LoggerClass *Logger_; /**<Pointer to the logging system instance*/

        // MOVE TO ASSET LOADER CLASS LATER:
        TextureLoader *TextureLoader_; /**<Pointer to Texture Loader Instance*/

        // SceneManager Class
        SceneManager *SceneManager_; /**<Pointer to Scene Manager Class Instance*/



        std::vector<ERS_OBJECT_CAMERA_NOCLIP*> Cameras_; /**<List Of Pointers To Camera Instances*/
        std::vector<ERS_OBJECT_SHADER*> Shaders_; /**<List Of Pointers To Shader Instances*/


        
        // Control Values
        bool *SystemShouldRun_;





        


        // Internal Functions
        void InitializeOpenGL();



    public:

        // Constructor
        VisualRenderer(YAML::Node *SystemConfiguration, LoggerClass *Logger, bool *SystemShouldRun);


        // Destructor
        ~VisualRenderer();


        void CreateViewport(ERS_OBJECT_SHADER *Shader, ERS_OBJECT_CAMERA_NOCLIP *Camera);

        // Update Loop Func
        void UpdateViewport(float DeltaTime, float RenderWidth, float RenderHeight);

        

};
