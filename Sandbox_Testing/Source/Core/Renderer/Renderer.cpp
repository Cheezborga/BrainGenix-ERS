//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This is the openGL Renderer.
    Additonal Notes: None
    Date Created: 2021-11-01
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "Core/Renderer/Renderer.h"

void Renderer::InitializeRenderer(YAML::Node *SystemConfiguration, LoggerClass *Logger) {

    // Create Pointers
    SystemConfiguration_ = SystemConfiguration;
    Logger_ = Logger;

    Logger_.Log("ttest", 10);

}

void Renderer::InitializeGLFW() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* Window = glfwCreateWindow(800, 600, "Jesse is a bad person", NULL, NULL);
    if (Window == NULL) {
        glfwTerminate();
    }

    glfwMakeContextCurrent(Window);

}