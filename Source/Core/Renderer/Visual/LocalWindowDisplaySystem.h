//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This file is responsible for implementing the visual local display system.
    Additonal Notes: None
    Date Created: 2021-10-13
*/ 


// This Class Handles Local Machine Display Via A Local Window
class LocalWindowDisplaySystem {

    private:
        // Declare Private Vars
        LoggerClass Logger;
        YAML::Node SystemConfiguration;

    public:

        // Window Initialization Function
        void InitWindow(LoggerClass sERSLogger, YAML::Node sERSConfig);


};