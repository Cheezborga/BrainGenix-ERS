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
#include <ERS_CASS_LoggingSystem.h>


/**
 * @brief Creates the user profile manager class.
 * 
 */
class ERS_CLASS_UserProfileManager {


    private:

        std::shared_ptr<ERS_CLASS_LoggingSystem> Logger_; /**<ERS_CLASS_LoggingSystem Instance Pointer*/
        std::string UserProfilePath_; /**<Path To User Profile*/
        YAML::Node UserProfile_; /**<YAML::Node Containing User Profile*/

        void LoadUserProfile(); /**<Load User Profile*/

        std::string UserFont_; /**<Name Of user fornt*/
        int UserFontSize_; /**<Size of user font*/
        std::string UserColorProfileName_; /**<User Color Profile Name*/

    public:

        /**
         * @brief Construct a new User Profile Manager object
         * 
         * @param Logger 
         */
        ERS_CLASS_UserProfileManager(std::shared_ptr<LoggerClass> Logger, const char* UserProfilePath = "EditorAssets/Configuration/User/UserConfig.yaml");

        /**
         * @brief Destroy the User Profile Manager object
         * 
         */
        ~ERS_CLASS_UserProfileManager();


        std::string GetUserFont();
        int GetUserFontSize();
        std::string GetUserColorProfile();

};