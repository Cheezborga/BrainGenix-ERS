//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

#pragma once


// Standard Libraries (BG convention: use <> instead of "")
#include <memory>
#include <thread>
#include <string>
#include <mutex>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <imgui.h>

// Internal Libraries (BG convention: use <> instead of "")
#include <ERS_STRUCT_SystemUtils.h>


/**
 * @brief Window for the Import progress bar.
 * 
 */
class Window_About {

private:

    std::shared_ptr<ERS_STRUCT_SystemUtils> SystemUtils_; /**<Pointers To System Resources*/

public:

    bool Enabled_ = false; /**<Is Popup Enabled*/


public:

    /**
     * @brief Construct a new Window_About object
     * 
     */
    Window_About(std::shared_ptr<ERS_STRUCT_SystemUtils> SystemUtils);
    
    /**
     * @brief Destroy the Window_About object
     * 
     */
    ~Window_About();


    /**
     * @brief Draw import bar if needed (call every frame)
     * 
     */
    void Draw();

};