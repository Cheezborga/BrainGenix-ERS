//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This file contains the model manager classes.
    Documentation Status: Done
    Additonal Notes: None
    Date Created: 2021-11-21
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <vector>

// Third-Party Libraries (BG convention: use <> instead of "")

// Internal Libraries (BG convention: use <> instead of "")
#include <LoggingSystem.h>
#include <Model.h>


/**
 * @brief The Model Manager class Handles Rendering and Calling Loading Of Models.
 * 
 */
class ModelManager{

    private:

        // Internal Member Pointers
        LoggerClass *Logger_; /**<Pointer To Logger Instance*/

        std::vector<ERS_OBJECT_MODEL> Models_; /**<Vector Containing Models To be Iterated Through When Drawn*/


    public:

        /**
         * @brief Construct a new Model Manager object
         * 
         * @param Logger 
         */
        ModelManager(LoggerClass *Logger);
        
        /**
         * @brief Destroy the Model Manager object
         * 
         */
        ~ModelManager();

        /**
         * @brief Add a model to the manager's list of managed models.
         * 
         */
        void AddModel(ERS_OBJECT_MODEL);

};