//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This is responsible for providing user profile management.
    Documentation Status: Complete
    Additonal Notes: None
    Date Created: 2021-12-04
*/

#pragma once


// Standard Libraries (BG convention: use <> instead of "")
#include <string>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <yaml-cpp/yaml.h>

// Internal Libraries (BG convention: use <> instead of "")
#include <LoggingSystem.h>


/**
 * @brief Creates the user profile manager class.
 * 
 */
class UserProfileManager {


    private:

        LoggerClass *Logger_; /**<LoggerClass Instance Pointer*/
        std::string UserProfilePath; /**<Path To User Profile*/

        void LoadUserProfile(); /**Load User Profile*/

    public:

        /**
         * @brief Construct a new User Profile Manager object
         * 
         * @param Logger 
         */
        UserProfileManager(LoggerClass *Logger, const char* UserProfilePath = "EditorAssets/Configuration/User/UserConfig.yaml");

        /**
         * @brief Destroy the User Profile Manager object
         * 
         */
        ~UserProfileManager();


};