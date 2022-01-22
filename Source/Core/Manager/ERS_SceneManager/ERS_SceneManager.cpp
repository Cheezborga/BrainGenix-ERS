//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This file contains the scene manager classes.
    Documentation Status: Done
    Additonal Notes: None
    Date Created: 2021-11-21
*/

#include <ERS_SceneManager.h>


// Constructor For ERS_CLASS_SceneManager Class
ERS_CLASS_SceneManager::ERS_CLASS_SceneManager(std::shared_ptr<LoggerClass> Logger) {

    // Copy To Local Pointer
    Logger_ = Logger;

    // Log Initialization
    Logger_->Log("Initializing ERS_CLASS_SceneManager Instance", 5);

}


// Destructor For ERS_CLASS_SceneManager Class
ERS_CLASS_SceneManager::~ERS_CLASS_SceneManager() {

    // Log Destructor Call
    Logger_->Log("Destructor Called For ERS_CLASS_SceneManager Class", 6);

}

// Update Scene Object(s) LocRotScale
void ERS_CLASS_SceneManager::UpdateLocRotScale(glm::vec3 Pos, glm::vec3 Rot, glm::vec3 Scale) {
    Scenes_[ActiveScene_].Models[Scenes_[ActiveScene_].SelectedModel]->SetLocRotScale(Pos, Rot, Scale);
    Scenes_[ActiveScene_].Models[Scenes_[ActiveScene_].SelectedModel]->ApplyTransformations();
}

// Add Scene Function
bool ERS_CLASS_SceneManager::AddScene(ERS_OBJECT_SCENE Scene) {

    // Check If Scene Isn't Ready
    if (!Scene.IsSceneLoaded) {

        // Log Issue
        Logger_->Log("Failed To Add Scene To Buffer, Scene Isn't Yet Loaded!", 6);

        // Return Failure
        return false;
    }

    // Append Scene To Scenes Model
    Scenes_.push_back(Scene);

    // Return Success
    return true;

}

// Render All Models In Buffer
void ERS_CLASS_SceneManager::Render(std::shared_ptr<ERS_OBJECT_SHADER> Shader) {
    
    // Iterate Through Models
    for (long i = 0; i < Scenes_[ActiveScene_].Models.size(); i++) {

        // Get Model Pointer
        ERS_OBJECT_MODEL *Model = Scenes_[ActiveScene_].Models[i].get();

        // Set Shader Pointer
        Shader->SetMat4("model", Model->GetMat4());

        // Call Draw
        Model->Draw(Shader);
        

    }

}

// SetActiveScene (Overloaded, int Version)
bool ERS_CLASS_SceneManager::SetActiveScene(int SceneIndex) {

    // Check SceneIndex Validity
    if (SceneIndex < 0 || SceneIndex > Scenes_.size()-1) {

        // Log Scene Switch Error
        Logger_->Log(std::string(std::string("Failed To Set Active Scene To Index: ") + std::to_string(SceneIndex) + std::string(" Because Scenes_ Only Has") + std::to_string(Scenes_.size()-1) + std::string(" Elements")).c_str(), 7); 

        // Return Failure
        return false;
    }

    // Update SceneIndex
    ActiveScene_ = SceneIndex;

    // Return Success
    return true;

}

// SetActiveScene (Overloaded, std::string Version)
bool ERS_CLASS_SceneManager::SetActiveScene(std::string TargetSceneName) {

    // Iterate Through Scenes, Check Name Against Target
    int TargetSceneIndex;
    bool TargetSceneFound = false;

    for (TargetSceneIndex = 0; TargetSceneIndex < Scenes_.size(); TargetSceneIndex++) {

        // Get Scene Name
        std::string SceneName = Scenes_[TargetSceneIndex].SceneName;

        // Check Scene Name
        if (SceneName == TargetSceneName) {
            TargetSceneFound = true;
            break;
        }

    }

    // Check Success/Fail
    if (!TargetSceneFound) {

        // Log Error
        Logger_->Log(std::string(std::string("Failed To Set Active Scene To: ") + TargetSceneName + std::string(" Because It Isn't In The Scenes_ Vector")).c_str(), 7); 

        // Return Failure
        return false;
    } 

    // Update Target Scene
    ActiveScene_ = TargetSceneIndex;

    // Return Success
    return true;
    

}


