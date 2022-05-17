//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

#pragma once


// Standard Libraries (BG convention: use <> instead of "")
#include <iostream>
#include <string>
#include <chrono>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


// Internal Libraries (BG convention: use <> instead of "")
#include <ERS_CLASS_LoggingSystem.h>

#include <ERS_SceneManager.h>

#include <ERS_STRUCT_ProjectUtils.h>
#include <ERS_STRUCT_SystemUtils.h>
#include <ERS_STRUCT_Viewport.h>
#include <ERS_STRUCT_DepthMap.h>



/**
 * @brief This class renders any ui/overlay info onto the viewport as requested by the viewport struct.
 * 
 */
class ERS_CLASS_DepthMaps {

private:

    ERS_STRUCT_SystemUtils* SystemUtils_; /**<Pointer to systemtuils resource*/
    ERS_STRUCT_ProjectUtils* ProjectUtils_; /**<Pointer to project utils instance*/


    /**
     * @brief Generates a depth map with the given resolution.
     * This is usually used for shadows later on in the rendering process.
     * Will also default to a 2048x2048 depth map by default. 
     * 
     * @param ResolutionX Width in pixels of the depth map
     * @param ResolutionY Height in pixels of the depth map
     * @param LogOutput Enable/disable logging output
     * @return ERS_STRUCT_DepthMap Struct containing the relevant opengl ids for this depth map
     */
    ERS_STRUCT_DepthMap GenerateDepthMap(int ResolutionX = 2048, int ResolutionY = 2048, bool LogOutput = true);

    /**
     * @brief Setup The Depth Map For Rendering.
     * This will bind to the depth map's framebuffer, clear the screen, and get map ready to be rendered to.
     * 
     * @param Input 
     */
    void BindToDepthMap(ERS_STRUCT_DepthMap* Input);


    /**
     * @brief This function creates a depth map from the perspective of the given position data.
     * The depth map texture is cleared, and then rendered to.
     * This function is designed to be used in shadow maps, but can be used elsewhere when appropriate.
     * 
     * @param Target 
     * @param Pos 
     * @param Rot 
     * @param Scale 
     */
    void UpdateDepthMap(ERS_STRUCT_DepthMap* Target, glm::vec3 Pos, glm::vec3 Rot, glm::vec3 Scale = glm::vec3(0.0f, 1.0f, 0.0f));


public:

    /**
     * @brief Construct a new ers class DepthMaps object
     * 
     * @param SystemUtils 
     */
    ERS_CLASS_DepthMaps(ERS_STRUCT_SystemUtils* SystemUtils, ERS_STRUCT_ProjectUtils* ProjectUtils);

    /**
     * @brief Destroy the ers class DepthMaps object
     * 
     */
    ~ERS_CLASS_DepthMaps();


    /**
     * @brief Checks the number of depth maps vs the number of lights and makes sure that the number of depth maps is equal to the number of lights.
     * Will generate more depth maps and remove unused ones by calling internal functions in this class.
     * 
     */
    void UpdateNumberOfDepthMaps();

    /**
     * @brief Iterates over all lights in the scene and renders all depth maps.
     * 
     */
    void UpdateDepthMaps();


};