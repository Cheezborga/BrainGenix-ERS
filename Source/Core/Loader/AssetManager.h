//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This is the asset manager class.
    Additional Notes: None
    Date Created: 2021-11-17
*/

#pragma once

#include "LoggingSystem.h"

/**
 * @brief The asset manager provides a unified multithreaded system for loading, storing, and managing assets.
 * 
 */
class AssetManager {

    private:

        LoggerClass* Logger_;


    public:

        /**
         * @brief Construct a new Asset Manager object
         * 
         */
        AssetManager();

        /**
         * @brief Destroy the Asset Manager object
         * 
         */
        ~AssetManager();

};