//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This Window file is responsible for creating a system Window.
    Documentation Status: Complete
    Additonal Notes: None
    Date Created: 2021-12-07
*/

#include <GUI_Subwindow_SceneRenameModal.h>

// Constructor
Subwindow_SceneRenameModal::Subwindow_SceneRenameModal(SceneManager* SceneManager) {

    // Update Ptr
    SceneManager_ = SceneManager;


}

// Destructor
Subwindow_SceneRenameModal::~Subwindow_SceneRenameModal() {
    
}

// Define Activation Function
void Subwindow_SceneRenameModal::Activate(int SceneIndex) {

    // Activate
    SelectedScene_ = SceneIndex;
    Enabled_ = true;
    FirstFrame_ = true;        

}

// Define Draw Function
void Subwindow_SceneRenameModal::Draw() {

    // Draw Rename Scene Window
    if (Enabled_) {
    ImGui::Begin("Rename Scene", &Enabled_, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);

        // Grab Window Focus
        ImGui::SetWindowFocus();
        
        // Set Default Keyboard Input
        if (FirstFrame_) {
            ImGui::SetKeyboardFocusHere(0);
            FirstFrame_ = false;
        }

        // Set Default Item To Type On, Add Input Box
        ImGui::SetItemDefaultFocus();
        ImGui::InputTextWithHint("Rename Scene", "Enter New Scene Name", SceneInputName_, IM_ARRAYSIZE(SceneInputName_));

        ImGui::Separator();

        // Rename And Cancel Buttons
        if (ImGui::Button("Rename", ImVec2(120, 0)) || ImGui::IsKeyPressed(GLFW_KEY_ENTER)) { // If Button Pressed, Or Enter Key Pressed
            SceneManager_->Scenes_[SelectedScene_].SceneName = std::string(SceneInputName_);
            Enabled_ = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0)) || ImGui::IsKeyPressed(GLFW_KEY_ESCAPE)) { // If Button Pressed, Or Escape Key Pressed
            Enabled_ = false;
        }

    ImGui::End();
    }


}
