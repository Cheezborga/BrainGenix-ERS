//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This file provides IOManager functionality.
    Documentation Status: Complete
    Additonal Notes: None
    Date Created: 2021-11-29
*/

#include <IOManager.h>



// IOManager Constructor
IOManager::IOManager(LoggerClass *Logger) {

    // Copy Pointer
    Logger_ = Logger;

    // Log Initialization
    Logger_->Log("Initializing IOManager", 5);


}