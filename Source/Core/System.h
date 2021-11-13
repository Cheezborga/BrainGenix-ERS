//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This file handles initailization and loading of all other ERS systems.
    Additonal Notes: None
    Date Created: 2021-11-12
*/

#include "yaml-cpp/yaml.h"

#include "Core/Renderer/Renderer.h"
#include "Core/Management/LoggingSystem.h"
#include "Core/Loader/BGAsset/BGAssetLoader.h"

class sERSSubSystem {

    private:

        // Internal Vars
        LoggerClass *Logger_;
        YAML::Node *SystemConfiguration_;
        bool *SystemShouldRun_;
        AssetLoader *AssetLoader_;


        // Renderer Related Vars
        Renderer VisualRenderer_; // FIXME: UPDATE RENDERER.h TO BE VISUALRENDERER AND MOVE THIS FILE INTO IT'S PLACE, RENAME


    public:

        // Constructor
        sERSSubSystem(LoggerClass *Logger, YAML::Node *SystemConfiguration, bool *SystemShouldRun, AssetLoader *AssetLoader);

        // Destructor
        ~sERSSubSystem();

        // Initialize Renderers
        void InitializeRenderers();

        // Update Renderers
        void UpdateRenderers();

};