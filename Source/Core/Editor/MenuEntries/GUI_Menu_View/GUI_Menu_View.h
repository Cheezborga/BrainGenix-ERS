//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

#pragma once


// Standard Libraries (BG convention: use <> instead of "")
#include <vector>
#include <iostream>
#include <filesystem>
#include <string>
#include <memory>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <imgui.h>

// Internal Libraries (BG convention: use <> instead of "")
#include <ERS_STRUCT_SystemUtils.h>

#include <ERS_Editor_ThemeManager.h>
#include <ERS_Editor_FontManager.h>

#include <GUI_Window_ThemeSelector.h>

#include <GUI_Window_About.h>



/**
 * @brief This class provides the "File" Menu In The Editor.
 * 
 */
class GUI_Menu_View {


private:

    ERS_STRUCT_SystemUtils* SystemUtils_; /**<SystemUtils Instance, Used To Get Systems From Other Classes*/
    ERS_CLASS_ThemeManager* ThemeManager_; /**<Theme Manager Instance Ptr*/
    std::unique_ptr<Window_ThemeSelector> Window_ThemeSelector_; /**<Theme Selector Window*/
    std::shared_ptr<ERS_CLASS_FontManager> FontManager_; /**<Pointer To FontManager Instance*/
    std::unique_ptr<Window_About> Window_About_; /**<Pointer to about window*/

    // Window Show/Hide Vars
    bool ShowFontPicker_ = false; /**<Control Varaible to show/hide font picker window*/


public:


    /**
     * @brief Construct a new gui menu object
     * 
     * @param SystemUtils 
     */
    GUI_Menu_View(ERS_STRUCT_SystemUtils* SystemUtils, ERS_CLASS_ThemeManager* ThemeManager, std::shared_ptr<ERS_CLASS_FontManager> FontManager);


    /**
     * @brief Destroy the gui menu object
     * 
     */
    ~GUI_Menu_View();


    /**
     * @brief This function Draws The View Menu Contents.
     * 
     */
    void Draw();


};