//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This is responsible for implementing a struct to simplify includes within classes..
    Documentation Status: Complete
    Additonal Notes: None
    Date Created: 2021-12-16
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <memory.h>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <yaml-cpp/yaml.h>

// Internal Libraries (BG convention: use <> instead of "")
#include <ERS_SceneManager.h>

#include <ERS_ModelLoader.h>
#include <ERS_SceneLoader.h>
#include <ERS_ProjectLoader.h>
#include <ERS_ProjectManager.h>


/**
 * @brief The System utils struct contains most project mgmt utils/classes/structs.
 * 
 */
struct ERS_STRUCT_ProjectUtils {


    std::shared_ptr<ERS_CLASS_SceneManager> SceneManager_; /**<Shared Ptr To Scene Manager*/
    std::shared_ptr<ERS_CLASS_ModelLoader> ModelLoader_; /**<Shared Ptr To Model Loader*/
    std::shared_ptr<ERS_CLASS_SceneLoader> SceneLoader_; /**<Shared Ptr To Scene Loader*/
    std::shared_ptr<ERS_CLASS_ProjectLoader> ProjectLoader_; /**<Shared Pointer To Project Loader*/
    std::shared_ptr<ERS_CLASS_ProjectManager> ProjectManager_; /**<Shared Pointer To Projet Manager Instance*/

};