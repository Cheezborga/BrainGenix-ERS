//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This is the main file of the system.
    Additional Notes: None
    Date Created: 2021-11-01
*/


// Standard Libraries (BG convention: use <> instead of "")

// Third-Party Libraries (BG convention: use <> instead of "")
#include <Tracy.hpp>

#include <yaml-cpp/yaml.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

// Internal Libraries (BG convention: use <> instead of "")
#include <LocalConfigurationLoader.h>
#include <RendererManager.h>
#include <LoggingSystem.h>
#include <HardwareInformation.h>

#include <ERS_SceneManager.h>


#include <ERS_InputOutputSubsystem.h>
#include <ERS_ModelWriter.h>
#include <ERS_FramerateManager.h>
#include <ERS_ProjectLoader.h>
#include <ERS_ProjectManager.h>
#include <ERS_ProjectWriter.h>
#include <ERS_ControllerInputManager.h>

#include <ERS_STRUCT_HumanInputDeviceUtils.h>
#include <ERS_STRUCT_SystemUtils.h>
#include <ERS_STRUCT_ProjectUtils.h>




//
//-----------------------------------------------------------------------------------------------------
//
// FIXME: Add proper lighting system (phong and more advanced (and shadow maps))
// FIXME: implement animations (character deformations)
// FIXME: implement offscreen rendering
// FIXME: add other features to this list later, (audio rendering, ray tracing, etc.)
// FIXME: Update this card with more info as needed!
//
//-----------------------------------------------------------------------------------------------------
//


/**
 * @brief Entry point.
 * 
 * @return int 
 */
int main() {

    // Initialize System Vars
    std::shared_ptr<bool> SystemShouldRun = std::make_shared<bool>(true);
    std::shared_ptr<ERS_STRUCT_SystemUtils> SystemUtils = std::make_shared<ERS_STRUCT_SystemUtils>();
    SystemUtils->SystemShouldRun_ = SystemShouldRun;

    // Load Local System Configuration File
    YAML::Node sERSLocalSystemConfiguration = LoadConfig("Config.yaml");
    SystemUtils->LocalSystemConfiguration_ = std::make_shared<YAML::Node>(sERSLocalSystemConfiguration);

    // Instantiate Logging Subsystem
    std::shared_ptr<LoggerClass> sERSLogger = std::make_shared<LoggerClass>(sERSLocalSystemConfiguration);
    SystemUtils->Logger_ = sERSLogger;
    sERSLogger->Log("Initialized Logging System", 5);

    // Setup Framerate Manager
    sERSLogger->Log("Initializing Framerate Manager Subsystem", 5);
    std::shared_ptr<ERS_CLASS_FramerateManager> sERSFrameRateManager = std::make_shared<ERS_CLASS_FramerateManager>();
    SystemUtils->FramerateManager_ = sERSFrameRateManager;
    sERSLogger->Log("Initialized Framerate Manager", 4);

    // Copy Config Params
    try {
        sERSLogger->Log("Reading Configuration File For 'IsLinux'", 2);
        SystemUtils->IsLinux_ = sERSLocalSystemConfiguration["IsLinux"].as<bool>();
    } catch(YAML::BadSubscript) {
        sERSLogger->Log("Error Reading Configuration File For 'IsLinux' Boolean Value, Defaulting To False", 9);
    }

    // Startup IO Subsystem And Other Related Systems
    std::shared_ptr<ERS_CLASS_InputOutputSubsystem> sERSIOSubSystem = std::make_shared<ERS_CLASS_InputOutputSubsystem>(sERSLogger, sERSLocalSystemConfiguration);
    SystemUtils->ERS_IOSubsystem_ = sERSIOSubSystem;
    std::shared_ptr<ERS_CLASS_ModelWriter> sERSModelWriter = std::make_shared<ERS_CLASS_ModelWriter>(sERSLogger, sERSIOSubSystem);
    SystemUtils->ERS_ModelWriter_ = sERSModelWriter;


    // Instantiate HardwareInformation System
    HardwareInformation sERSHardwareInformation(sERSLogger, sERSLocalSystemConfiguration);


    // Create ProjectUtils Struct
    sERSLogger->Log("Setting Up Project Utilities Structure", 3);
    std::shared_ptr<ERS_STRUCT_ProjectUtils> ProjectUtils = std::make_shared<ERS_STRUCT_ProjectUtils>();

    // Setup Loaders
    sERSLogger->Log("Instantiating Scene Manager Shared Pointer", 4);
    std::shared_ptr<ERS_CLASS_SceneManager> sERSSceneManager = std::make_shared<ERS_CLASS_SceneManager>(SystemUtils->Logger_);
    sERSLogger->Log("Copying Shared Pointer To Project Utils Struct", 3);
    ProjectUtils->SceneManager_ = sERSSceneManager;

    sERSLogger->Log("Instantiating Model Loader Shared Pointer", 4);
    std::shared_ptr<ERS_CLASS_ModelLoader> sERSModelLoader = std::make_shared<ERS_CLASS_ModelLoader>(SystemUtils);
    sERSLogger->Log("Copying Shared Pointer To Project Utils Struct", 3);
    ProjectUtils->ModelLoader_ = sERSModelLoader;

    sERSLogger->Log("Instantiating Scene Loader Shared Pointer", 4);
    std::shared_ptr<ERS_CLASS_SceneLoader> sERSSceneLoader = std::make_shared<ERS_CLASS_SceneLoader>(SystemUtils, sERSModelLoader);
    sERSLogger->Log("Copying Shared Pointer To Project Utils Struct", 3);
    ProjectUtils->SceneLoader_ = sERSSceneLoader;

    sERSLogger->Log("Instantiating ERS Project Loader Pointer", 4);
    std::shared_ptr<ERS_CLASS_ProjectLoader> sERSProjectLoader = std::make_shared<ERS_CLASS_ProjectLoader>(SystemUtils);
    sERSLogger->Log("Instantiating Project Loader Shared Pointer", 4);
    ProjectUtils->ProjectLoader_ = sERSProjectLoader;

    sERSLogger->Log("Instantiating ERS Project Writer Pointer", 4);
    std::shared_ptr<ERS_CLASS_ProjectWriter> sERSProjectWriter = std::make_shared<ERS_CLASS_ProjectWriter>(SystemUtils);
    sERSLogger->Log("Instantiating Project Writer Shared Pointer", 4);
    ProjectUtils->ProjectWriter_ = sERSProjectWriter;

    sERSLogger->Log("Instantiating ERS Project Manager Pointer", 4);
    std::shared_ptr<ERS_CLASS_ProjectManager> sERSProjectManager = std::make_shared<ERS_CLASS_ProjectManager>(SystemUtils, sERSProjectLoader, sERSProjectWriter, sERSSceneManager, sERSSceneLoader);
    sERSLogger->Log("Copying Shared Pointer To Project Utils Struct", 3);
    ProjectUtils->ProjectManager_ = sERSProjectManager;


    // Setup Human Input Devices
    sERSLogger->Log("Setting Up Human Input Device Managers", 5);

    // Create Struct
    std::shared_ptr<ERS_STRUCT_HumanInputDeviceUtils> HIDUtils = std::make_shared<ERS_STRUCT_HumanInputDeviceUtils>();

    // Setup Controller Manager
    sERSLogger->Log("Instantiating ERS Controller Input Manager", 5);
    std::shared_ptr<ERS_CLASS_ControllerInputManager> sERSControllerManager = std::make_shared<ERS_CLASS_ControllerInputManager>(SystemUtils);
    sERSLogger->Log("Copying Shared Pointer To HumanInputDeviceUtils Struct", 4);
    HIDUtils->ControllerInputManager = sERSControllerManager;

    // VR Driver here?

    // More????

    sERSLogger->Log("Finished Setting Up Human Input Device Managers", 5);    


    // Instantiate RendererManager
    RendererManager sERSRendererManager(SystemUtils, ProjectUtils);


    
    // Log Logo Text
    sERSLogger->Log("Starting BrainGenix-ERS Instance", 2);
    sERSLogger->Log("", 5);
    sERSLogger->Log("---------------------------------------------------------------------------", 5);
    sERSLogger->Log("\x1b[38;2;0;128;55m██████╗ ██████╗  █████╗ ██╗███╗   ██╗\x1b[38;2;130;68;208m ██████╗ ███████╗███╗   ██╗██╗██╗  ██╗", 5);
    sERSLogger->Log("\x1b[38;2;0;128;55m██╔══██╗██╔══██╗██╔══██╗██║████╗  ██║\x1b[38;2;130;68;208m██╔════╝ ██╔════╝████╗  ██║██║╚██╗██╔╝", 5);
    sERSLogger->Log("\x1b[38;2;0;128;55m██████╔╝██████╔╝███████║██║██╔██╗ ██║\x1b[38;2;130;68;208m██║  ███╗█████╗  ██╔██╗ ██║██║ ╚███╔╝ ", 5);
    sERSLogger->Log("\x1b[38;2;0;128;55m██╔══██╗██╔══██╗██╔══██║██║██║╚██╗██║\x1b[38;2;130;68;208m██║   ██║██╔══╝  ██║╚██╗██║██║ ██╔██╗ ", 5);
    sERSLogger->Log("\x1b[38;2;0;128;55m██████╔╝██║  ██║██║  ██║██║██║ ╚████║\x1b[38;2;130;68;208m╚██████╔╝███████╗██║ ╚████║██║██╔╝ ██╗", 5);
    sERSLogger->Log("\x1b[38;2;0;128;55m╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝\x1b[38;2;130;68;208m ╚═════╝ ╚══════╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═╝", 5);
    sERSLogger->Log("---------------------------------------------------------------------------", 5);
    sERSLogger->Log("", 5);
    sERSLogger->Log("    +-----------------------------------------------------------------+", 4);
    sERSLogger->Log("    |     BrainGenix-ERS Real-Time Environment Rendering System       |", 4);
    sERSLogger->Log("    +-----------------------------------------------------------------+", 4);
    sERSLogger->Log("", 4);


    // Initialize Times
    float DeltaTime = 0.0f;
    float LastFrame = 0.0f;

    // Enter Main Loop
    while (*SystemShouldRun) {

        // Calculate Frametime
        float CurrentTime = glfwGetTime();
        DeltaTime = CurrentTime - LastFrame;
        LastFrame = CurrentTime;

        // Calculate Last FrameTime
        sERSFrameRateManager->StartNewFrame();


        // Update Joysticks
        sERSControllerManager->UpdateControllers();


        // Update Renderers
        sERSRendererManager.UpdateLoop(DeltaTime);



        // End Frame
        sERSFrameRateManager->DelayUntilNextFrame();

    }


    // Test Clear Project
    sERSSceneManager->Scenes_.erase(sERSSceneManager->Scenes_.begin(), sERSSceneManager->Scenes_.end());


    // Program Exit (Destructors Automatically Called)
    return 0;

}