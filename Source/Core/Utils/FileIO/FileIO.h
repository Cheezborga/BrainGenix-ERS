//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This is responsible for loading/compiling shaders.
    Documentation Status: Complete
    Additonal Notes: None
    Date Created: 2021-09-28
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <fstream>
#include <string>

// Internal Libraries (BG convention: use <> instead of "")
#include <LoggingSystem.h>

// Read Image File From Disk
std::string ReadFile(const std::string& FileName, LoggerClass *Logger_ = nullptr);

