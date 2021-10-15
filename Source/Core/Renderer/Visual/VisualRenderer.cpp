//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This file is responsible for implementing the main renderer system.
    Additonal Notes: None
    Date Created: 2021-10-13
*/ 

#include <vulkan/vulkan.h>
#include <vector>
#include <cstring>

#include "Core/Renderer/Visual/LocalWindowDisplaySystem.cpp"

#include "Core/Renderer/Visual/VisualRenderer.h"



// Define VisualRenderer::InitializeSystem
void VisualRenderer::InitializeSystem(LoggerClass sERSLogger, YAML::Node sERSConfig, bool ShutdownToggle) {

    // Create Local References
    Logger_ = sERSLogger;
    SystemConfiguration_ = sERSConfig;
    SystemShutdownInvoked_ = ShutdownToggle;

    // Initialize GLFW
    Logger_.Log("Reading System Configuration For 'BOOL': 'WindowEnabled'", 2);
    LocalWindowEnabled_ = SystemConfiguration_["WindowEnabled"].as<bool>();
    if (LocalWindowEnabled_) {
        Logger_.Log("Initializing 'Core::Renderer::Visual::LocalWindowDisplaySystem'", 4);
        sERSLocalWindowDisplaySystem_.InitWindow(Logger_, SystemConfiguration_);
        Logger_.Log("Initialized 'Core::Renderer::Visual::LocalWindowDisplaySystem'", 3);
    } else {
        Logger_.Log("Initialization Skip 'Core::Renderer::Visual::LocalWindowDisplaySystem' Due To Config Param", 3);
    };

    // Initialize Vulkan
    Logger_.Log("Initializing 'Core::Renderer::Visual::VisualRenderer::Vulkan'", 4);
    InitVulkan();
    Logger_.Log("Initialized 'Core::Renderer::Visual::VisualRenderer::Vulkan'", 3);

}

// Define VisualRenderer::InitVulakn
void VisualRenderer::InitVulkan() {

    // Log Vulkan Init Creation
    Logger_.Log("Initializng Vulkan API", 4);

    // Create Vulkan Instance
    Logger_.Log("Initializng 'Core::Renderer::Visual::VisualRenderer::CreateVulkanInstance'", 3);
    CreateVulkanInstance();
    Logger_.Log("Initializng 'Core::Renderer::Visual::VisualRenderer::CreateVulkanInstance'", 2);

}

// Define VisualRenderer::CreateVulkanInstance
void VisualRenderer::CreateVulkanInstance() {

    // Fill AppInfo Struct
    Logger_.Log("Populating Vulkan 'appInfo' Struct", 3);

    VkApplicationInfo AppInfo{};
    AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    AppInfo.pApplicationName = "BrainGenix::ERS (Environment Rendering System)";
    AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    AppInfo.pEngineName = "BrainGenix::ERS";
    AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    AppInfo.apiVersion = VK_API_VERSION_1_0;

    // Vulkan Instance Creation Information
    Logger_.Log("Generating VKInstanceCreateInfo", 3);
    VkInstanceCreateInfo VkCreateInfo{};
    VkCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    VkCreateInfo.pApplicationInfo = &AppInfo;

    // If GLFW Enabled
    if (LocalWindowEnabled_) 
        VkCreateInfo = sERSLocalWindowDisplaySystem_.GetVulkanInitExtensions(VkCreateInfo);




    // Check For Validation Layers
    Logger_.Log("Reading System Configuration For 'BOOL': 'EnableValidationLayers'", 2);
    bool ValidationLayersRequested = SystemConfiguration_["EnableValidationLayers"].as<bool>();

    if (ValidationLayersRequested) {
        
        // Check For Validation Layer Support
        Logger_.Log("Validation Layers Requested", 2);
        ValidationLayersToBeUsed_ = CheckValidationLayerSupport();

    } else {

        // Validation Layers Not Requested
        Logger_.Log("Validation Layers Disabled In Configuration File", 2);

    }

    // Enable Validation Layers If To Be Used
    if (ValidationLayersToBeUsed_) {

        // Add Validation Layers To Create Info
        Logger_.Log("Adding Enabled Validation Layers To Vulkan Instance Creation Information", 4);
        VkCreateInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers_.size());
        VkCreateInfo.ppEnabledLayerNames = ValidationLayers_.data();
    } else {

        // Disable Validation Layers
        Logger_.Log("Disabling Validation Layers In Vulkan Creation Information", 4);
        VkCreateInfo.enabledLayerCount = 0;
    }




    // Create Vulkan Instance
    Logger_.Log("Creating Vulkan Instance", 3);
    VkResult Result = vkCreateInstance(&VkCreateInfo, nullptr, &VulkanInstance_);

    // Check State Of Vulkan Instance Creation
    if (Result != VK_SUCCESS) {
        Logger_.Log("Failed To Create Vulkan Instance", 10);

        // Invoke System Shutdown
        SystemShutdownInvoked_ = true;
    } else {
        Logger_.Log("Created Vulkan Instance", 3);
    }


    // Extension Support
    Logger_.Log("Checking For Vulkan Extension Support", 4);
    
    // Get Number Of Extensions
    Logger_.Log("Querying Number Of Supported Extensions", 3);
    uint32_t ExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &ExtensionCount, nullptr);

    std::vector<VkExtensionProperties> Extensions(ExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &ExtensionCount, Extensions.data());
    Logger_.Log(std::string(std::string("Found ") + std::to_string(ExtensionCount) + std::string(" Vulkan Extensions")).c_str(), 2);

    // Query Extension Information
    Logger_.Log("Enumerating Available Vulkan Extensions", 4);
    for (auto& Extension : Extensions) {
        Logger_.Log(std::string(std::string("\t Found Vulkan Extension: ") + std::string(Extension.extensionName)).c_str(), 3);
    }



}



// Define VisualRenderer::CheckValidationLayerSupport
bool VisualRenderer::CheckValidationLayerSupport() {

    // Check For Validation Layer Support
    Logger_.Log("Checking For Validation Layer Support", 4);

    uint32_t ValidationLayerCount;
    vkEnumerateInstanceLayerProperties(&ValidationLayerCount, nullptr);

    std::vector<VkLayerProperties> AvailableValidationLayers(ValidationLayerCount);
    vkEnumerateInstanceLayerProperties(&ValidationLayerCount, AvailableValidationLayers.data());

    // Iterate Through Required Layers
    for (const char* LayerName : ValidationLayers_) {

        bool ValidationLayerFound = false;
        Logger_.Log(std::string(std::string("Checking Available Layers For Validation Layer: ") + std::string(LayerName)).c_str(), 2);

        // Check If Layer In Available Layers
        for (const auto& LayerProperties : AvailableValidationLayers) {
            if (strcmp(LayerName, LayerProperties.layerName) == 0) {
                
                // Layer Found
                Logger_.Log(std::string(std::string("Found Validation Layer: ") + std::string(LayerName)).c_str(), 1);
                ValidationLayerFound = true;
                break;
            }
        }

        // If Layer Not Found
        if (!ValidationLayerFound) {
            
            // Layer NOT Found
            Logger_.Log(std::string(std::string("Failed To Find Validation Layer: ") + std::string(LayerName)).c_str(), 6);
            Logger_.Log("Validation Layers Disabled Due To Missing Layer(s), Please Check Your Installation Of The Vulkan SDK", 6);
            return false;
        }


    }

    // If All Layers Were Found, Return True
    return true;

}


// Define VisualRenderer::RenderLoop
void VisualRenderer::RenderLoop() {

    // GLFW Poll Events
    sERSLocalWindowDisplaySystem_.FetchEvents();

}

// Define VisualRenderer::CleanUp
void VisualRenderer::CleanUp() {

    // Log Shutdown Called
    Logger_.Log("Shutting Down 'Core::Renderer::Visual::VisualRenderer'", 5);

    // Call Subclass's Destructors
    sERSLocalWindowDisplaySystem_.CleanUp();

    // Destroy Vulkan
    vkDestroyInstance(VulkanInstance_, nullptr);

}