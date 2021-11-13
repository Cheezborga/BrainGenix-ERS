//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This file is responsible for loading assets from the scene.
    Additonal Notes: None
    Date Created: 2021-10-13
*/ 

#include "BGAssetLoader.h"

////////////////////////
// TODO: RENAME ASSETLOADER TO ASSETMANAGER AND GIVE OWN ROOT DIR IN SOURCE TREE
////////////////////////

// Asset Loader Constructor
AssetLoader::AssetLoader(LoggerClass *Logger, YAML::Node *SystemConfiguration) {

    // Initialization Start
    Logger->Log("Initializing Asset Loader Class", 6);

    // Copy Pointer
    Logger_ = Logger;
    SystemConfiguration_ = SystemConfiguration;

    // Initialize Data Buffer
    DataBuffer_.Init(Logger_);
    

    // Get Config Values
    DatabaseLoadingEnabled_ = (*SystemConfiguration_)["DatabaseLoadingEnabled"].as<bool>();
    // TODO: ADD OTHER DB PARAMS HERE (SEE CONFIG FOR MORE INFO)
    // TODO: ADD CONFIG STRING FOR LOCAL FILE ASSET PATH (ALSO ADD TO CONFIG FILE)


    // Log Initialization
    Logger_->Log("Initialized Asset Loader Class", 5);

}

// Read Scene Subnodes And Load Models/Textures Requested By That Scene
void AssetLoader::LoadSceneAssets(ERS_OBJECT_SCENE InputScene) {

    // Iterate Through Scene Data
    for (long i = 0; i < (long)size(InputScene.Subnodes); i++) {

        // Get Subnode
        YAML::Node Subnode = InputScene.Subnodes[i];

        // Get Attributes
        long AssetID = Subnode["ID"].as<long>();
        std::string AssetType = Subnode["Type"].as<std::string>();

        // Load Asset
        if (AssetType == "Image") {

            LoadImage(AssetID, Subnode);

        }

    }

}

// Load Image
void AssetLoader::LoadImage(long AssetID, YAML::Node Params) {

    // Load From DB
    if (DatabaseLoadingEnabled_) {

        // TODO: ADD DATABASE LOADING FUNCTIONALITY LATER!

    } else {

        // Calculate File Path
        std::string FilePath = (*SystemConfiguration_)["AssetPath"].as<std::string>();
        FilePath += std::to_string(AssetID);
        FilePath += std::string(".bg");

        // Load Image
        ERSImage Image = LoadImageFromFile(FilePath.c_str());

    }

}

// Returns All Models Loaded In The Asset Buffer And Marked To Be Drawn
std::vector<ERS_OBJECT_MODEL*> AssetLoader::GetModelsToDraw() {

    // Create Pointer Vector
    std::vector<ERS_OBJECT_MODEL*> ModelPointers;

    // Get Struct Data
    for (long i = 0; i < (long)size(DataBuffer_.ERS_OBJECT_MODEL_Vector_); i++) {
        ModelPointers.push_back(&DataBuffer_.ERS_OBJECT_MODEL_Vector_[i]);
    }

    // Return Output
    return ModelPointers;


};
