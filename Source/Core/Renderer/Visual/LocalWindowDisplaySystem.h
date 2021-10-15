//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This file is responsible for implementing the visual local display system.
    Additonal Notes: None
    Date Created: 2021-10-13
*/ 

#pragma once

// This Class Handles Local Machine Display Via A Local Window
class LocalWindowDisplaySystem {

    private:
        // Declare Private Vars
        LoggerClass Logger_;
        YAML::Node SystemConfiguration_;

        GLFWwindow* Window_;

        int WindowX_;
        int WindowY_;
        std::string WindowTitle_;


    public:

        // Window Initialization Function
        void InitWindow(LoggerClass sERSLogger, YAML::Node sERSConfig);

        // Add GLFW Extensions To VkInstanceCreateInfo
        VkInstanceCreateInfo GetVulkanInitExtensions(VkInstanceCreateInfo VkCreateInfo);

        // Get Window Events
        void FetchEvents();

        // Destructor
        void CleanUp();

};