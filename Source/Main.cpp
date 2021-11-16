//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This is the main file of the system.
    Additonal Notes: None
    Date Created: 2021-11-01
*/

#include <yaml-cpp/yaml.h>

#include "Core/Utils/Configuration/LocalConfigurationLoader.h"
#include "Core/Utils/Logger/LoggingSystem.h"


/**
 * @brief Entry point.
 * 
 * @return int 
 */
int main() {

    // Initialize System State Bool
    bool SystemShouldRun = true; /**<Indicates If System Should Run, False Will Cause System To Exit On Next RunLoop Iteration>*/

    // Load System Configuration File
    YAML::Node SystemConfiguration = LoadConfig("Config.yaml");

    // Instantiate Logging Subsystem //
    LoggerClass sERSLogger(SystemConfiguration);
    sERSLogger.Log("Initialized 'Management::Logger::LoggerClass'", 5);




    
    // Log Logo Text
    sERSLogger.Log("Starting BrainGenix-ERS Instance", 2);
    sERSLogger.Log("", 5);
    sERSLogger.Log("---------------------------------------------------------------------------", 5);
    sERSLogger.Log("\x1b[38;2;0;128;55m██████╗ ██████╗  █████╗ ██╗███╗   ██╗\x1b[38;2;130;68;208m ██████╗ ███████╗███╗   ██╗██╗██╗  ██╗", 5);
    sERSLogger.Log("\x1b[38;2;0;128;55m██╔══██╗██╔══██╗██╔══██╗██║████╗  ██║\x1b[38;2;130;68;208m██╔════╝ ██╔════╝████╗  ██║██║╚██╗██╔╝", 5);
    sERSLogger.Log("\x1b[38;2;0;128;55m██████╔╝██████╔╝███████║██║██╔██╗ ██║\x1b[38;2;130;68;208m██║  ███╗█████╗  ██╔██╗ ██║██║ ╚███╔╝ ", 5);
    sERSLogger.Log("\x1b[38;2;0;128;55m██╔══██╗██╔══██╗██╔══██║██║██║╚██╗██║\x1b[38;2;130;68;208m██║   ██║██╔══╝  ██║╚██╗██║██║ ██╔██╗ ", 5);
    sERSLogger.Log("\x1b[38;2;0;128;55m██████╔╝██║  ██║██║  ██║██║██║ ╚████║\x1b[38;2;130;68;208m╚██████╔╝███████╗██║ ╚████║██║██╔╝ ██╗", 5);
    sERSLogger.Log("\x1b[38;2;0;128;55m╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝\x1b[38;2;130;68;208m ╚═════╝ ╚══════╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═╝", 5);
    sERSLogger.Log("---------------------------------------------------------------------------", 5);
    sERSLogger.Log("", 5);
    sERSLogger.Log("    +-----------------------------------------------------------------+", 4);
    sERSLogger.Log("    |     BrainGenix-ERS Real-Time Environment Rendering System       |", 4);
    //sERSLogger.Log(std::string(std::string("    |     Version: ") + std::string(PROJECT_VERSION) + std::string("                                              |")).c_str(), 4);
    //sERSLogger.Log(std::string(std::string("    |     Branch: ") + std::string(PROJECT_BRANCH) + std::string("                                                 |")).c_str(), 4);
    //ERSLogger.Log("    |     Clustersize: {NodeCount}                                              |", 4);
    sERSLogger.Log("    +-----------------------------------------------------------------+", 4);
    sERSLogger.Log("", 4);


    // Enter Main Loop
    while (SystemShouldRun) {


    }

    // Program Exit (Destructors Automatically Called)
    return 0;

}