//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This file handles initailization and loading of all other ERS systems.
    Additonal Notes: None
    Date Created: 2021-11-12
*/

#include "Core/System.h"


// Constructor
sERSSubSystem::sERSSubSystem(LoggerClass *Logger, YAML::Node *SystemConfiguration, bool *SystemShouldRun, AssetLoader *AssetLoader) {

    // Start Initialization
    Logger->Log("Initializing Main Subsystem", 6);

    // Copy Pointers
    Logger_ = Logger;
    SystemConfiguration_ = SystemConfiguration;
    SystemShouldRun_ = SystemShouldRun;
    AssetLoader_ = AssetLoader;

    // Finish Initialization
    Logger_->Log("Initialized Main Subsystem", 5);

}

// Destructor
sERSSubSystem::~sERSSubSystem() {

    // Log Destructor Call
    Logger_->Log("Main Subsystem Destructor Called, Cleaning Up", 5);

    // Clean Up
    VisualRenderer_.CleanUp();

}

// Main Update Loop
void sERSSubSystem::UpdateRenderers() {

    std::cout<<"test: "<<AssetLoader_->DataBuffer_->GetNumberObjectModels()<<std::endl;

    // Update Visual Renderer
    VisualRenderer_.UpdateLoop();

}

// Initialize Renderers
void sERSSubSystem::InitializeRenderers() {

    // Initialize Renderer
    Logger_->Log("Initializing Renderer", 5);
    VisualRenderer_.InitializeRenderer(SystemConfiguration_, Logger_, SystemShouldRun_, AssetLoader_);
    Logger_->Log("Initialized Renderer", 5);

}