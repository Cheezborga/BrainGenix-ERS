//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This file contains the scene loader class.
    Documentation Status: Completed
    Additonal Notes: None
    Date Created: 2021-11-22
*/

// FIXME: Make template textures added to their own map so they can be reused.
// FIXME: Make dynamic/static models that can each be rotated/translated (dynamically or statically). Then make updateposition functions, etc.

#include <SceneLoader.h>

// SceneLoader Constructor
SceneLoader::SceneLoader(std::shared_ptr<ERS_STRUCT_SystemUtils> SystemUtils, std::shared_ptr<ERS_CLASS_ModelLoader> ModelLoader) {

    // Create Local Pointer
    SystemUtils_ = SystemUtils;
    ModelLoader_ = ModelLoader;

    // Log Initializaton Start
    SystemUtils_->Logger_->Log("Initializing SceneLoader Subsystem", 5);

}

// SceneLoader Destructor
SceneLoader::~SceneLoader() {

    // Log Destructor Call
    SystemUtils_->Logger_->Log("SceneLoader Destructor Called", 6);

}

// Load And Process Scene
ERS_OBJECT_SCENE SceneLoader::ProcessScene(long AssetID) {

    // Read Asset Info
    std::shared_ptr<ERS_STRUCT_IOData> SceneData = std::make_shared<ERS_STRUCT_IOData>();
    

    // Load Then Process Scene
    //YAML::Node TestScene = YAML::LoadFile(ScenePath);
    //return ProcessScene(TestScene, ScenePath);

}

// SceneLoader Process Scene Function
ERS_OBJECT_SCENE SceneLoader::ProcessScene(YAML::Node RawSceneData, const char* ScenePath, bool UseMultithreading) {

    // Create Scene Instance
    ERS_OBJECT_SCENE Scene;

    // Grab Metadata
    Scene.SceneFormatVersion = RawSceneData["SceneFormatVersion"].as<long>();
    Scene.SceneName = RawSceneData["SceneName"].as<std::string>();
    Scene.ScenePath = std::string(ScenePath);

    // Log Scene Processing
    SystemUtils_->Logger_->Log(std::string(std::string("Processing Scene: ") + std::string(Scene.SceneName)).c_str(), 4);

    // Create Vector Of YAML::Nodes
    std::vector<YAML::Node> SceneItems;

    // Populate Vector With Elements From SceneData
    YAML::Node SceneDataNode = RawSceneData["SceneData"];
    for (YAML::const_iterator it=SceneDataNode.begin(); it!=SceneDataNode.end(); ++it) {
        SceneItems.push_back(it->second);
    }


    // Iterate Through Vector To Add Each Asset To Loading Queue Of Requested Type
    for (long i = 0; i < SceneDataNode.size(); i++) {

        // Get Asset Information
        std::string AssetName = SceneDataNode[i]["AssetName"].as<std::string>();
        std::string AssetType = SceneDataNode[i]["AssetType"].as<std::string>();
        long AssetID = SceneDataNode[i]["AssetID"].as<long>();


        // If type Is Model
        if (AssetType == std::string("Model")) {

            // Get Model Texture Info
            bool FlipTextures = SceneDataNode[i]["FlipTextures"].as<bool>();

            // Get Asset LocRotScale
            float PosX = SceneDataNode[i]["AssetPositionX"].as<double>();
            float PosY = SceneDataNode[i]["AssetPositionY"].as<double>();
            float PosZ = SceneDataNode[i]["AssetPositionZ"].as<double>();

            float RotX = SceneDataNode[i]["AssetRotationX"].as<double>();
            float RotY = SceneDataNode[i]["AssetRotationY"].as<double>();
            float RotZ = SceneDataNode[i]["AssetRotationZ"].as<double>();

            float ScaleX = SceneDataNode[i]["AssetScaleX"].as<double>();
            float ScaleY = SceneDataNode[i]["AssetScaleY"].as<double>();
            float ScaleZ = SceneDataNode[i]["AssetScaleZ"].as<double>();

            //Load Model 
            Scene.Models.push_back(std::make_shared<ERS_OBJECT_MODEL>());
            int CurrentSize = Scene.Models.size();
            ModelLoader_->AddModelToLoadingQueue(AssetID, Scene.Models[CurrentSize-1], FlipTextures);

            // Add Instance To Models Vector
            Scene.Models[CurrentSize-1]->IsTemplateModel = false;
            Scene.Models[CurrentSize-1]->SetLocRotScale(glm::vec3(PosX, PosY, PosZ), glm::vec3(RotX, RotY, RotZ), glm::vec3(ScaleX, ScaleY, ScaleZ));
            Scene.Models[CurrentSize-1]->ApplyTransformations();
            Scene.Models[CurrentSize-1]->AssetID = AssetID;
            Scene.Models[CurrentSize-1]->Name = AssetName;
            Scene.Models[CurrentSize-1]->FlipTextures = FlipTextures;

        }

    }





    // Indicate Scene Is Loaded
    Scene.IsSceneLoaded = true;

    // Return Scene
    return Scene;

}

