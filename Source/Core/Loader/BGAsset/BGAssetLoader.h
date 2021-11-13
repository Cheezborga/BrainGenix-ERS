//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This file is responsible for implementing scene decoding.
    Additonal Notes: None
    Date Created: 2021-10-13
*/ 

#pragma once

#include "yaml-cpp/yaml.h"

#include "Core/Management/LoggingSystem.h"
#include "Core/Structures/Scene/Scene.h"
#include "Core/Structures/DataBuffer/DataBuffer.h"


class AssetLoader {

    private:

        // Init Vars
        LoggerClass *Logger_;
        YAML::Node *SystemConfiguration_;
        ERS_STRUCTURE_DATA_BUFFER *DataBuffer_;

        // Configuration Vars
        bool DatabaseLoadingEnabled_;

    public:

        // Constructor
        AssetLoader(LoggerClass *Logger, YAML::Node *SystemConfiguration, ERS_STRUCTURE_DATA_BUFFER *DataBuffer);

        // Load Assets From Scene
        LoadSceneAssets(ERS_OBJECT_SCENE InputScene);

};