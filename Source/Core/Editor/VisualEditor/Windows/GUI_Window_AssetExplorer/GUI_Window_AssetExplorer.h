//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This Window file is responsible for creating a system Window.
    Documentation Status: Complete
    Additonal Notes: None
    Date Created: 2021-12-17
*/

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
class Window_AssetExplorer {

private:

    std::shared_ptr<ERS_STRUCT_SystemUtils> SystemUtils_; /**<Pointers To System Resources*/
    std::vector<bool> AssetIDSelectionList_; /**<Select Multiple Asset IDs*/
    int LastSelectedIndex_; /**<Index of the last item to be clicked on*/

public:

    bool Enabled_ = true; /**<Is Popup Enabled*/
    bool AdvancedMode_ = true; /**<Toggles on/off Advanced Mode*/

public:

    /**
     * @brief Construct a new Window_AssetExplorer object
     * 
     */
    Window_AssetExplorer(std::shared_ptr<ERS_STRUCT_SystemUtils> SystemUtils);
    
    /**
     * @brief Destroy the Window_AssetExplorer object
     * 
     */
    ~Window_AssetExplorer();


    /**
     * @brief Draw system log needed (call every frame)
     * 
     */
    void Draw();

};