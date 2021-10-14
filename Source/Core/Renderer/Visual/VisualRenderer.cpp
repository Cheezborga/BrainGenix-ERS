//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This file is responsible for implementing the main renderer system.
    Additonal Notes: None
    Date Created: 2021-10-13
*/ 

#include "Core/Renderer/Visual/VisualRenderer.h"

#include "Core/Renderer/Visual/LocalWindowDisplaySystem.cpp"


// Define Visual Renderer Class
void VisualRenderer::InitializeSystem(LoggerClass sERSLogger) {

    // Create Local Reference
    Logger = sERSLogger;

    // Log Initialization Process
    Logger.Log("Initializing Vulkan API", 5);



}
