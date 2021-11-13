//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This is the main file of the system.
    Additonal Notes: None
    Date Created: 2021-11-01
*/

#include "Core/Loader/Configuration/ConfigurationLoader.h"
#include "Core/Management/LoggingSystem.h"
#include "Core/Renderer/Renderer.h"



int main() {



    

    // Load System Configuration File
    YAML::Node SystemConfiguration = LoadConfig("Config.yaml");

    // Instantiate Logging Subsystem //
    LoggerClass sERSLogger;
    sERSLogger.InitializeLogger(SystemConfiguration);

    sERSLogger.Log("Initialized 'Management::Logger::LoggerClass'", 5);



    // Initialize Renderer
    Renderer sERSRenderer;
    sERSLogger.Log("Initializing Renderer", 5);
    sERSRenderer.InitializeRenderer(&SystemConfiguration, &sERSLogger);
    sERSLogger.Log("Initialized Renderer", 5);


    # MOTD #
    Logger_->Log("Starting BrainGenix-ERS Instance", 2);
    Logger_->Log("", 5)
    Logger_->Log("---------------------------------------------------------------------------", 5)
    Logger_->Log("\x1b[38;2;0;128;55m██████╗ ██████╗  █████╗ ██╗███╗   ██╗\x1b[38;2;130;68;208m ██████╗ ███████╗███╗   ██╗██╗██╗  ██╗", 5)
    Logger_->Log("\x1b[38;2;0;128;55m██╔══██╗██╔══██╗██╔══██╗██║████╗  ██║\x1b[38;2;130;68;208m██╔════╝ ██╔════╝████╗  ██║██║╚██╗██╔╝", 5)
    Logger_->Log("\x1b[38;2;0;128;55m██████╔╝██████╔╝███████║██║██╔██╗ ██║\x1b[38;2;130;68;208m██║  ███╗█████╗  ██╔██╗ ██║██║ ╚███╔╝ ", 5)
    Logger_->Log('\x1b[38;2;0;128;55m██╔══██╗██╔══██╗██╔══██║██║██║╚██╗██║\x1b[38;2;130;68;208m██║   ██║██╔══╝  ██║╚██╗██║██║ ██╔██╗ ', 5)
    Logger_->Log('\x1b[38;2;0;128;55m██████╔╝██║  ██║██║  ██║██║██║ ╚████║\x1b[38;2;130;68;208m╚██████╔╝███████╗██║ ╚████║██║██╔╝ ██╗', 5)
    Logger_->Log('\x1b[38;2;0;128;55m╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝\x1b[38;2;130;68;208m ╚═════╝ ╚══════╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═╝', 5)
    Logger_->Log('---------------------------------------------------------------------------', 5)
    Logger_->Log('', 5)
    Logger_->Log('    +-----------------------------------------------------------------+', 4)
    Logger_->Log('    |     BrainGenix-NES Biological Neuron Simulation System          |', 4)
    Logger_->Log('    |     Version: {Version}                                              |', 4)
    Logger_->Log('    |     Branch: {Branch}                                                 |', 4)
    Logger_->Log('    |     Clustersize: {NodeCount}                                              |', 4)
    Logger_->Log('    +-----------------------------------------------------------------+', 4)
    Logger_->Log('', 4)


    // Enter Main Loop
    bool SystemShouldRun = true;
    while (SystemShouldRun) {

        SystemShouldRun = sERSRenderer.UpdateLoop();

    }

    // Cleanup
    sERSRenderer.CleanUp();

}