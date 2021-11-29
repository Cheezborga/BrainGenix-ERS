//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This file is responsible for creating and managing the framebuffer.
    Documentation Status: Complete
    Additonal Notes: None
    Date Created: 2021-11-28
*/

#pragma once


// Standard Libraries (BG convention: use <> instead of "")

// Third-Party Libraries (BG convention: use <> instead of "")
#include <glad/glad.h>

#include <GLFW/glfw3.h>

// Internal Libraries (BG convention: use <> instead of "")
#include <LoggingSystem.h>
#include <Shader.h>
#include <ShaderLoader.h>


/**
 * @brief Class to create/renderto/destruy framebuffer object.
 * 
 */
class FramebufferManager {


    private:

        LoggerClass *Logger_; /**<Pointer to instance of Logger Class*/
        unsigned int FramebufferObject_; /**<Framebuffer OpenGL Handle*/
        unsigned int RenderTexture_; /**<OpenGL Renderes To This Texture, Then It's Displayed. (OpenGL Handle)*/
        unsigned int RenderBufferObject_; /**<RenderBuffer Object Handle.*/

        ShaderLoader *ShaderLoader_; /**<Pointer To Shader Loader Instance*/
        ERS_OBJECT_SHADER ScreenShader_; /**<Screen Shader Instance, Used To Display Framebuffer To Screen*/


        float QuadVertices_[]; /**<List Containing Positions And Texture coords For Drawing Screen FrameBuffer Quad*/
        unsigned int ScreenQuadVAO_; /**<Screen Quad VAO Reference*/
        unsigned int ScreenQuadVBO_; /**<Screen Quad VBO Reference*/


    public:

        /**
         * @brief Construct a new Framebuffer Manager object.
         * 
         * @param Logger LoggerClass Instance Pointer
         * @param Width Width In Pixels
         * @param Height Height In Pixels
         */
        FramebufferManager(LoggerClass *Logger, ShaderLoader *ShaderLoader, float Width = 800.0f, float Height = 600.0f);

        /**
         * @brief Destroy the Framebuffer Manager object.
         * 
         */
        ~FramebufferManager();


};