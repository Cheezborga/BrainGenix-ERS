//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This file provides a GUI menu entry.
    Documentation Status: Complete
    Additonal Notes: None
    Date Created: 2021-12-17
*/

#include <GUI_Menu_View.h>


// Constructor
GUI_Menu_View::GUI_Menu_View(std::shared_ptr<ERS_STRUCT_SystemUtils> SystemUtils, std::shared_ptr<ERS_CLASS_ThemeManager> ThemeManager, std::shared_ptr<ERS_CLASS_FontManager> FontManager) {

    // Copy In Pointer Struct
    SystemUtils_ = SystemUtils;
    ThemeManager_ = ThemeManager;
    FontManager_ = FontManager;

    // Log Initialization
    SystemUtils_->Logger_->Log("Editor Setting Up View Menu", 4);

    // Create Class Instances
    Window_ThemeSelector_ = std::make_unique<Window_ThemeSelector>(ThemeManager_); 
    Window_About_ = std::make_unique<Window_About>(SystemUtils_);


}

// Destructor
GUI_Menu_View::~GUI_Menu_View() {

    // Log Destructor
    SystemUtils_->Logger_->Log("Editor Destroying View Menu", 4);

}

// Draw Function
void GUI_Menu_View::Draw() {

    // View Menu
    if (ImGui::BeginMenu("View")) {


        if (ImGui::MenuItem("Color Theme")) {
            Window_ThemeSelector_->Enabled_ = true;
        }

        if (ImGui::MenuItem("System Font")) {
            ShowFontPicker_ = true;
        }

        ImGui::Separator();

        if (ImGui::MenuItem("About")) {
            Window_About_->Enabled_ = true;
        }


    ImGui::EndMenu();
    }


    // Draw Windows
    Window_ThemeSelector_->Draw();
    Window_About_->Draw();
    FontManager_->FontSelectorWindow(&ShowFontPicker_);

}