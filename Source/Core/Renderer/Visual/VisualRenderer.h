//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This file is responsible for implementing the main renderer system.
    Additonal Notes: None
    Date Created: 2021-10-13
*/ 

#include "Core/Renderer/Visual/LocalWindowDisplaySystem.h"
 
// Define Visual Renderer Class
class VisualRenderer {

    public:

        // Initialize Vulkan API
        void InitVulkan();

        // Main Loop For Visual Renderer
        void RenderLoop();

        // Destructor
        void CleanUp();

};