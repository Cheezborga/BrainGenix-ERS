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
sERSSubSystem::sERSSubSystem(LoggerClass *Logger, YAML::Node *SystemConfiguration) {

    // Start Initialization
    Logger->Log("Initializing Main Subsystem", 6);

    // Copy Pointer
    Logger_ = Logger;
    SystemConfiguration_ = SystemConfiguration;

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


// Initialize Renderers
void sERSSubSystem::InitializeRenderers() {

    // Initialize Renderer
    Logger_->Log("Initializing Renderer", 5);
    VisualRenderer_.InitializeRenderer(SystemConfiguration_, Logger_);
    Logger_->Log("Initialized Renderer", 5);

}