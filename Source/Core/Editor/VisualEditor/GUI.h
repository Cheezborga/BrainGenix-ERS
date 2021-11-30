//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This is responsible for providing the gui.
    Documentation Status: Complete
    Additonal Notes: None
    Date Created: 2021-01-25
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")

// Third-Party Libraries (BG convention: use <> instead of "")
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui.h>
//#include <imgui_dock.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>


// Internal Libraries (BG convention: use <> instead of "")
#include <LoggingSystem.h>
#include <Theme.h>

/**
 * @brief Implements the ERS gui system.
 * 
 */
class GUISystem {


    private:

        LoggerClass* Logger_; /**<Pointer To LoggerClass Instance*/
        GLFWwindow* Window_; /**<Pointer to window instance*/


        bool *SystemShouldRun_; /**<Control Variable Indicating System Run State */


        // Window Show/Hide Vars
        bool ShowColorThemePicker_ = false; /**<Control Variable indicating weither to show/hide the color theme picker*/


        // Update Window Functions
        void UpdateWindow_ColorThemePicker(); /**<Updates Color Theme Picker Window*/

    public:

        ThemeManager* ThemeManager_; /**<Pointer To ThemeManager Instance*/


        /**
         * @brief Construct a new Gui System object
         * 
         * @param Logger 
         */
        GUISystem(LoggerClass* Logger, GLFWwindow* Window, bool *SystemShouldRun);

        /**
         * @brief Destroy the Gui System object
         * 
         */
        ~GUISystem();


        /**
         * @brief Generate Frame REMEMBER TO CALL UPDATE FRAME AFTER THE CLEAR CALL.
         * 
         */
        void UpdateGUI();

        /**
         * @brief Update Frame, make sure you've called updategui.
         * 
         */
        void UpdateFrame();


        void MenuExitFunction();
};
