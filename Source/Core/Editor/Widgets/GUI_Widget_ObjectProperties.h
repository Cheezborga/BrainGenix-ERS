//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

#pragma once


// Standard Libraries (BG convention: use <> instead of "")

// Third-Party Libraries (BG convention: use <> instead of "")
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui.h>

// Internal Libraries (BG convention: use <> instead of "")
#include <ERS_Editor_3DCursor.h>

#include <ERS_STRUCT_LocRotScale.h>



/**
 * @brief Create GUI Widget "Rendering Settings"
 * 
 */
class Widget_ObjectProperties {

    private:

        std::shared_ptr<Cursors3D> Cursors3D_; /**<Instance pointer to cursors3d class*/

    public:

        // Show/Hide Var
        bool Enabled_ = true; /**<Show/Hide Widget*/

        /**
         * @brief Construct a new Widget_ObjectProperties object
         * 
         */
        Widget_ObjectProperties(std::shared_ptr<Cursors3D> Cursors3D);

        /**
         * @brief Destroy the Widget_ObjectProperties object
         * 
         */
        ~Widget_ObjectProperties();

        /**
         * @brief Function that draws the window, should internally check if enable variable is set to true.
         * 
         */
        void Draw();

};