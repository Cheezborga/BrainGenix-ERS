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
#include <optional>
#include <set>
#include <cstdint>
#include <algorithm>

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
    Logger_.Log("Initializing Vulkan API", 4);

    // Create Vulkan Instance
    Logger_.Log("Initializing 'Core::Renderer::Visual::VisualRenderer::CreateVulkanInstance'", 3);
    CreateVulkanInstance();
    Logger_.Log("Initialized 'Core::Renderer::Visual::VisualRenderer::CreateVulkanInstance'", 2);

    // Create Display Surface
    if (LocalWindowEnabled_) {
        Logger_.Log("Initialization [ START] 'Core::Renderer::Visual::LocalWindowDisplaySystem::CreateSurface'", 3);
        sERSLocalWindowDisplaySystem_.CreateSurface(VulkanInstance_);
        Logger_.Log("Initialization [FINISH] 'Core::Renderer::Visual::LocalWindowDisplaySystem::CreateSurface'", 2);
    } else {
        Logger_.Log("Initialization [SKIP] [Configuration DISABLED] 'Core::Renderer::Visual::LocalWindowDisplaySystem::CreateSurface'", 2);
    }

    // Pick Physical Device
    Logger_.Log("Initializing 'Core::Renderer::Visual::VisualRenderer::PickPhysicalDevice", 3);
    PickPhysicalDevice();
    Logger_.Log("Initialized 'Core::Renderer::Visual::VisualRenderer::PickPhysicalDevice", 2);

    // Create Logical Device
    Logger_.Log("Initializing 'Core::Renderer::Visual::VisualRenderer::CreateLogicalDevice", 3);
    CreateLogicalDevice();
    Logger_.Log("Initialized 'Core::Renderer::Visual::VisualRenderer::CreateLogicalDevice", 2);

    // Create Swapchain
    if (LocalWindowEnabled_) {
        Logger_.Log("INIT [ START] Creating Swap Chain", 3);
        CreateSwapChain();
        Logger_.Log("INIT [FINISH] Created Swap Chain", 2);
    } else {
        Logger_.Log("INIT [  SKIP] [CONFIGURATION DISABLE] Skipping Swapchain Initialization", 3);
    }

    // Create Image Views
    if (LocalWindowEnabled_) {
        Logger_.Log("Initialization [ START] Creating Image Views", 3);
        CreateImageViews();
        Logger_.Log("Initialization [FINISH] Created Image Views", 2);
    } else {
        Logger_.Log("Initialization [  SKIP] [CONFIGURATION DISABLE] Skipping Image View Creation", 3);
    }

}

// Define VisualRenderer::CreateImageViews
void VisualRenderer::CreateImageViews() {

    // Create Image View
    SwapChainImageViews_.resize(SwapChainImages_.size());

    // Iterate Through Image Views
    for (size_t i = 0; i < SwapChainImages_.size(); i++) {

        // Populate ImageViewCreateInfo Struct
        VkImageViewCreateInfo CreateInfo{};

        CreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        CreateInfo.image = SwapChainImages_[i];
        CreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        CreateInfo.format = SwapChainImageFormat_;

        CreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        CreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        CreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        CreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        CreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        CreateInfo.subresourceRange.baseMipLevel = 0;
        CreateInfo.subresourceRange.levelCount = 1;
        CreateInfo.subresourceRange.baseArrayLayer = 0;
        CreateInfo.subresourceRange.layerCount = 1; // For stereoscopic 3d application, make more layers

        // Create Image View
        if (vkCreateImageView(LogicalDevice_, &CreateInfo, nullptr, &SwapChainImageViews_[i]) != VK_SUCCESS) {
            Logger_.Log("Failed To Create Image Views", 10);
            SystemShutdownInvoked_ = true;
        }


    }

}

// Define VisualRenderer::CreateSwapChain
void VisualRenderer::CreateSwapChain() {

    // Get Swapchain Support Info
    Logger_.Log("INIT [ START] Querying Swap Chain Support Information", 3);
    SwapChainSupportDetails SwapChainSupport = QuerySwapChainSupport(PhysicalDevice_, false);
    Logger_.Log("INIT [FINISH] Fetched Swap Chain Support Information", 2);

    // Setup Swap Chain
    Logger_.Log("INIT [ START] Setting Up Swap Chain Surface Formats", 3);
    VkSurfaceFormatKHR SurfaceFormat = ChooseSwapChainSurfaceFormat(SwapChainSupport.Formats);
    Logger_.Log("INIT [FINISH] Set Up Swap Chain Surface Formats", 2);

    Logger_.Log("INIT [ START] Setting Up Swap Chain Present Modes", 3);
    VkPresentModeKHR PresentMode = ChooseSwapPresentMode(SwapChainSupport.PresentModes);
    Logger_.Log("INIT [FINISH] Setting Up Swap Chain Present Modes", 2);
    
    Logger_.Log("INIT [ START] Setting Up Swap Chain Extent Capabilities", 3);
    VkExtent2D Extent = ChooseSwapExtent(SwapChainSupport.Capabilities);
    Logger_.Log("INIT [FINISH] Setting Up Swap Chain Extent Capabilities", 2);


    // Set Number Of Images In SwapChain
    Logger_.Log("INIT [ START] Setting Up Swap Chain Image Count", 3);
    uint32_t ImageCount = SwapChainSupport.Capabilities.minImageCount + 1;
    
    if (SwapChainSupport.Capabilities.maxImageCount > 0 && ImageCount > SwapChainSupport.Capabilities.maxImageCount) {
        ImageCount = SwapChainSupport.Capabilities.maxImageCount;
    }

    Logger_.Log(std::string(std::string("INIT [FINISH] Set Swap Chain Image Count To: ") + std::to_string(ImageCount)).c_str(), 2);


    // Configure Swap Chain
    Logger_.Log("INIT [ START] Populating Swap Chain CreateInfo Struct", 3);
    VkSwapchainCreateInfoKHR CreateInfo{};

    CreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    CreateInfo.surface = sERSLocalWindowDisplaySystem_.Surface_;

    CreateInfo.minImageCount = ImageCount;
    CreateInfo.imageFormat = SurfaceFormat.format;
    CreateInfo.imageColorSpace = SurfaceFormat.colorSpace;
    CreateInfo.imageExtent = Extent;
    CreateInfo.imageArrayLayers = 1;
    CreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices Indices = FindQueueFamilies(PhysicalDevice_, false);
    uint32_t QueueFamilyIndices[] = {Indices.GraphicsFamily.value(), Indices.PresentFamily.value()};

    if (Indices.GraphicsFamily != Indices.PresentFamily) {
        CreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        CreateInfo.queueFamilyIndexCount = 2;
        CreateInfo.pQueueFamilyIndices = QueueFamilyIndices;
    } else {
        CreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        CreateInfo.queueFamilyIndexCount = 0;
        CreateInfo.pQueueFamilyIndices = nullptr;

    }

    CreateInfo.preTransform = SwapChainSupport.Capabilities.currentTransform;
    CreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    CreateInfo.presentMode = PresentMode;
    CreateInfo.clipped = VK_TRUE;

    CreateInfo.oldSwapchain = VK_NULL_HANDLE;

    Logger_.Log("INIT [FINISH] Populated Swap Chain CreateInfo Struct", 2);


    // Create Swap Chain
    Logger_.Log("INIT [ START] Creating Swap Chain Object", 3);
    if (vkCreateSwapchainKHR(LogicalDevice_, &CreateInfo, nullptr, &SwapChain_) != VK_SUCCESS) {
        Logger_.Log("INIT [ERROR] Failed To Create Swap Chain", 10);
        SystemShutdownInvoked_ = true;
    }
    Logger_.Log("INIT [FINISH] Created Swap Chain Object", 2);


    // Assign To Member Variables
    Logger_.Log("INIT [ START] Assigning Swapchain Info To Member Variables", 3);
    vkGetSwapchainImagesKHR(LogicalDevice_, SwapChain_, &ImageCount, nullptr);
    SwapChainImages_.resize(ImageCount);
    vkGetSwapchainImagesKHR(LogicalDevice_, SwapChain_, &ImageCount, SwapChainImages_.data());

    SwapChainImageFormat_ = SurfaceFormat.format;
    SwapChainExtent_ = Extent;
    Logger_.Log("INIT [ START] Assigned Swapchain Info To Member Variables", 2);


}

// Define VisualRenderer::SwapchainPresentMode
VkPresentModeKHR VisualRenderer::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& AvailablePresentModes) {

    // Iterate Through Presentation Modes
    for (const auto& AvailablePresentMode : AvailablePresentModes) {
        if (AvailablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return AvailablePresentMode;
        }
    }
    
    // Fallback Mode
    return VK_PRESENT_MODE_FIFO_KHR;

}

// Define VisualRenderer::Swap Extent
VkExtent2D VisualRenderer::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities) {

    // Get Framebuffer MaxSize Requirements
    if (Capabilities.currentExtent.width != UINT32_MAX) {
        return Capabilities.currentExtent;
    } else {
            int Width, Height;
            glfwGetFramebufferSize(sERSLocalWindowDisplaySystem_.Window_, &sERSLocalWindowDisplaySystem_.WindowX_, &sERSLocalWindowDisplaySystem_.WindowY_);

            VkExtent2D ActualExtent = {
                static_cast<uint32_t>(sERSLocalWindowDisplaySystem_.WindowX_),
                static_cast<uint32_t>(sERSLocalWindowDisplaySystem_.WindowY_)
            };

            ActualExtent.width = std::clamp(ActualExtent.width, Capabilities.minImageExtent.width, Capabilities.maxImageExtent.width);
            ActualExtent.height = std::clamp(ActualExtent.height, Capabilities.minImageExtent.height, Capabilities.maxImageExtent.height);

            return ActualExtent;
    }    

}

// Define VisualRenderer::ChooseSwapChainSurfaceFormat
VkSurfaceFormatKHR VisualRenderer::ChooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& AvailableFormats) {

    // Iterate Through Formats
    for (const auto& AvailableFormat : AvailableFormats) {

        // Check Format
        if (AvailableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && AvailableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {

            return AvailableFormat;

        }


    }

    // If Target Format Not Available, Use Whatever Is
    return AvailableFormats[0];


}

// Define VisualRenderer::QuerySwapChainSupport
SwapChainSupportDetails VisualRenderer::QuerySwapChainSupport(VkPhysicalDevice Device, bool IndentLogs) {

    // Get Swap Chain Support Information
    if (IndentLogs) {
        Logger_.Log("        Querying Number Of Swap Chain Formats", 3);
    } else {
        Logger_.Log("Querying Number Of Swap Chain Formats", 3);
    }
    SwapChainSupportDetails Details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Device, sERSLocalWindowDisplaySystem_.Surface_, &Details.Capabilities);

    // Get Number Of Formats
    uint32_t FormatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(Device, sERSLocalWindowDisplaySystem_.Surface_, &FormatCount, nullptr);


    if (FormatCount != 0) {
        Details.Formats.resize(FormatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(Device, sERSLocalWindowDisplaySystem_.Surface_, &FormatCount, Details.Formats.data());
    }

    // Log Formats
    if (IndentLogs) {
        Logger_.Log(std::string(std::string("        Found ") + std::to_string(FormatCount) + std::string(" Formats")).c_str(), 2);
    } else {
        Logger_.Log(std::string(std::string("Found ") + std::to_string(FormatCount) + std::string(" Formats")).c_str(), 2);
    }


    // Get Number Of Present Modes
    if (IndentLogs) {
        Logger_.Log("        Querying Number Of Presentation Modes", 3);
    } else {
        Logger_.Log("Querying Number Of Presentation Modes", 3);
    }
    uint32_t PresentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(Device, sERSLocalWindowDisplaySystem_.Surface_, &PresentModeCount, nullptr);

    if (PresentModeCount != 0) {
        Details.PresentModes.resize(PresentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(Device, sERSLocalWindowDisplaySystem_.Surface_, &PresentModeCount, Details.PresentModes.data());
    }

    // Log Number Of Presentation Modes
    if (IndentLogs) {
        Logger_.Log(std::string(std::string("        Found ") + std::to_string(PresentModeCount) + std::string(" Modes")).c_str(), 2);
    } else {
        Logger_.Log(std::string(std::string("Found ") + std::to_string(PresentModeCount) + std::string(" Modes")).c_str(), 2);
    }


    // Return Results
    return Details;

}

// Define VisualRenderer::CheckDeviceExtensionSupport
bool VisualRenderer::CheckDeviceExtensionSupport(VkPhysicalDevice Device, bool IndentLogs) {

    // Check Extensions
    if (IndentLogs) {
        Logger_.Log("        Querying Number Of Device Extensions", 3);
    } else {
        Logger_.Log("Querying Number Of Device Extensions", 3);
    }
    uint32_t ExtensionCount;
    vkEnumerateDeviceExtensionProperties(Device, nullptr, &ExtensionCount, nullptr);

    std::vector<VkExtensionProperties> AvailableExtensions(ExtensionCount);
    vkEnumerateDeviceExtensionProperties(Device, nullptr, &ExtensionCount, AvailableExtensions.data());

    std::set<std::string> RequiredExtensions(DeviceExtensions_.begin(), DeviceExtensions_.end());

    for (const auto& Extension : AvailableExtensions) {

        RequiredExtensions.erase(Extension.extensionName);

    }


    // Return Data
    if (IndentLogs) {
        Logger_.Log(std::string(std::string("        Found ") + std::to_string(ExtensionCount) + std::string(" Extensions")).c_str(), 2);
    } else {
       Logger_.Log(std::string(std::string("Found ") + std::to_string(ExtensionCount) + std::string(" Extensions")).c_str(), 2);
    }

    return RequiredExtensions.empty();


}

// Define VisualRenderer::CreateLogicalDevice
void VisualRenderer::CreateLogicalDevice() {

    // Create Logical Device
    Logger_.Log("Getting Queue Families For Selected Device", 3);
    QueueFamilyIndices Indices = FindQueueFamilies(PhysicalDevice_, false);

    // Setup Creation Process
    Logger_.Log("Configuring 'VkDeviceQueueCreateInfo' Struct", 3);
    

    VkDeviceQueueCreateInfo QueueCreateInfo{};
    std::vector<VkDeviceQueueCreateInfo> QueueCreateInfos;

    if (LocalWindowEnabled_) {

        
        std::set<uint32_t> UniqueQueueFamilies = {Indices.GraphicsFamily.value(), Indices.PresentFamily.value()};

        float QueuePriority = 1.0f;
        for (uint32_t QueueFamily : UniqueQueueFamilies) {

            VkDeviceQueueCreateInfo QueueCreateInfo{};
            QueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            QueueCreateInfo.queueFamilyIndex = QueueFamily;
            QueueCreateInfo.queueCount = 1;
            QueueCreateInfo.pQueuePriorities = &QueuePriority;
            QueueCreateInfos.push_back(QueueCreateInfo);


        }


    } else {
    
        QueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        QueueCreateInfo.queueFamilyIndex = Indices.GraphicsFamily.value();
        QueueCreateInfo.queueCount = 1;

        // Set Queue Priority
        float QueuePriority = 1.0f;
        QueueCreateInfo.pQueuePriorities = &QueuePriority;
    
    }



    // Set Device Features
    Logger_.Log("Setting Required Device Features", 2);
    VkPhysicalDeviceFeatures DeviceFeatures{};

    // Configure Logical Device
    Logger_.Log("Configuring 'VkDeviceCreateInfo' Struct", 5);
    VkDeviceCreateInfo CreateInfo{};
    CreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    if (LocalWindowEnabled_) {
        CreateInfo.queueCreateInfoCount = static_cast<uint32_t>(QueueCreateInfos.size());
        CreateInfo.pQueueCreateInfos = QueueCreateInfos.data();

        CreateInfo.enabledExtensionCount = static_cast<uint32_t>(DeviceExtensions_.size());
        CreateInfo.ppEnabledExtensionNames = DeviceExtensions_.data();
    } else {
        CreateInfo.pQueueCreateInfos = &QueueCreateInfo;
        CreateInfo.queueCreateInfoCount = 1;
        CreateInfo.enabledExtensionCount = 0;
    }
    
    CreateInfo.pEnabledFeatures = &DeviceFeatures;


    if (ValidationLayersToBeUsed_) {
        CreateInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers_.size());
        CreateInfo.ppEnabledLayerNames = ValidationLayers_.data();
    } else {
        CreateInfo.enabledLayerCount = 0;
    }




    // Creating Logical Device
    Logger_.Log("Creating Logical Device Instance", 3);
    if (vkCreateDevice(PhysicalDevice_, &CreateInfo, nullptr, &LogicalDevice_) != VK_SUCCESS) {

        // Logical Device Creation Failure
        Logger_.Log("Failed To Create Logical Device, System Shutting Down", 10);
        SystemShutdownInvoked_ = true;
    }

    // Setup Graphics Queue
    Logger_.Log("INIT [ START] Setting Up Graphics Queue", 3);
    vkGetDeviceQueue(LogicalDevice_, Indices.GraphicsFamily.value(), 0, &GraphicsQueue_);
    Logger_.Log("INIT [FINISH] Set Up Graphics Queue", 2);

    // Setup Presentation Queue
    if (LocalWindowEnabled_) {
        Logger_.Log("INIT [ START] Creating Presentation Queue", 3);
        vkGetDeviceQueue(LogicalDevice_, Indices.PresentFamily.value(), 0, &PresentationQueue_);
        Logger_.Log("INIT [FINISH] Created Presentation Queue", 2);
    } else {
        Logger_.Log("INIT [  SKIP] [Configuration: 'DISABLE'] Skipping Presentation Queue Creation", 4);
    }
}

// Define VisualRenderer::FindQueueFamilies
QueueFamilyIndices VisualRenderer::FindQueueFamilies(VkPhysicalDevice Device, bool IndentLogs) {

    // Initialize Struct
    QueueFamilyIndices Indices;

    // Find Graphics Queue Family
    if (IndentLogs) {
        Logger_.Log("        Querying Number Of Queue Families", 3);
    } else {
        Logger_.Log("Querying Number Of Queue Families", 3);
    }

    uint32_t QueueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, nullptr);

    // Check Log Indentation Level
    if (IndentLogs) {
        Logger_.Log(std::string(std::string("        Found ") + std::to_string(QueueFamilyCount) + std::string(" Queue Families")).c_str(), 2);
    } else {
        Logger_.Log(std::string(std::string("Found ") + std::to_string(QueueFamilyCount) + std::string(" Queue Families")).c_str(), 2);
    }
    
    std::vector<VkQueueFamilyProperties> QueueFamilies(QueueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, QueueFamilies.data());

    // Iterate Through Queues
    int Index = 0;
    for (const auto& QueueFamily : QueueFamilies) {

        // Check If Queue Family Has Requred Features
        if (QueueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            Indices.GraphicsFamily = Index;
        }

        // Check For Present Support If Required
        if (LocalWindowEnabled_) {
            VkBool32 PresentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(Device, Index, sERSLocalWindowDisplaySystem_.Surface_, &PresentSupport);

            if (PresentSupport) {
                Indices.PresentFamily = Index;
            }
        }

        Index++;


    }

    // Return Results
    return Indices;

}

// Define VisualRenderer::PickPhysicalDevice
void VisualRenderer::PickPhysicalDevice() {

    // Log Selection Process
    Logger_.Log("Querying Available Graphics Processers", 5);

    // Get List Of GPUS
    uint32_t DeviceCount = 0;
    vkEnumeratePhysicalDevices(VulkanInstance_, &DeviceCount, nullptr);
    Logger_.Log(std::string(std::string("Found ") + std::to_string(DeviceCount) + std::string(" Physical Devices")).c_str(), 4);

    std::vector<VkPhysicalDevice> Devices(DeviceCount);
    vkEnumeratePhysicalDevices(VulkanInstance_, &DeviceCount, Devices.data());

    // Create Map Of Device Suitabillity
    std::multimap<int, VkPhysicalDevice> DeviceCandidates;

    // Identify And Score Physical Devices
    PhysicalDevice_ = VK_NULL_HANDLE;
    for (const auto& Device : Devices) {
        int Score = RateDeviceSuitability(Device);
        DeviceCandidates.insert(std::make_pair(Score, Device));
    }

    // Pick Best Device Candidate
    if (DeviceCandidates.rbegin()->first > 0) {
        PhysicalDevice_ = DeviceCandidates.rbegin()->second;
        Logger_.Log("Identified Suitable Physical Device", 4);
    } else {
        Logger_.Log("No Suitable Physical Device Found, Exiting", 10);
        SystemShutdownInvoked_ = true;
    }

}

// Define VisualRenderer::RateDeviceSuitability
int VisualRenderer::RateDeviceSuitability(VkPhysicalDevice Device) {

    // Get Device Properties
    VkPhysicalDeviceProperties DeviceProperties;
    VkPhysicalDeviceFeatures DeviceFeatures;

    vkGetPhysicalDeviceProperties(Device, &DeviceProperties);
    vkGetPhysicalDeviceFeatures(Device, &DeviceFeatures);


    // Score Device -- Success Cases
    int Score = 0;

    // Check GPU Descrete Or Integrated
    if (DeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        Score += 1000;
    };

    // Get Max Texture Size
    Score += DeviceProperties.limits.maxImageDimension2D;
    

    // Log Device + Score
    Logger_.Log(std::string("    Found Physical Device With Name: " + std::string(DeviceProperties.deviceName) + std::string(", Score ") + std::to_string(Score)).c_str(), 4);



    // Score Device -- Failure Cases
        
    // Check If Device Supports Geometry Shaders
    if (!DeviceFeatures.geometryShader) {
        return 0;
    }

    // Check If Device Supports Queue Families
    QueueFamilyIndices Indices = FindQueueFamilies(Device);
    if (!Indices.GraphicsFamily.has_value()) {
        return 0;
    }

    // Check If Device Supports The Needed Extensions
    bool DeviceSupportsExtensions = CheckDeviceExtensionSupport(Device);
    if (!DeviceSupportsExtensions) {
        return 0;
    }

    // Check If Device Has Needed Swap Chain Features
    bool SwapChainAdequate = false;
    if (DeviceSupportsExtensions && LocalWindowEnabled_) {
        SwapChainSupportDetails SwapChainSupport = QuerySwapChainSupport(Device);
        SwapChainAdequate = !SwapChainSupport.Formats.empty() && !SwapChainSupport.PresentModes.empty();
    }


    // Return Device Score
    return Score;

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
    Logger_.Log("Enumerating Available Vulkan Extensions:", 4);
    for (auto& Extension : Extensions) {
        Logger_.Log(std::string(std::string("\t Found Vulkan Extension: ") + std::string(Extension.extensionName)).c_str(), 1);
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

    // Cleanup Image Views
    for (auto ImageView : SwapChainImageViews_) {
        vkDestroyImageView(LogicalDevice_, ImageView, nullptr);
    }

    // Destroy Swapchain
    vkDestroySwapchainKHR(LogicalDevice_, SwapChain_, nullptr);

    // Call Subclass's Destructors
    sERSLocalWindowDisplaySystem_.CleanUp();

    // Destroy LogicalDevice
    vkDestroyDevice(LogicalDevice_, nullptr);

    // Destroy Vulkan Instance
    vkDestroyInstance(VulkanInstance_, nullptr);

}