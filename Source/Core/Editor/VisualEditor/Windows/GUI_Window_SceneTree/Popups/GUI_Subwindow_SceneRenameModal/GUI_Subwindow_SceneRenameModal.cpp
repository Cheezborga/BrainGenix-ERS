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
    ImGui::OpenPopup("Rename Scene");
        

}

// Define Draw Function
void Subwindow_SceneRenameModal::Draw() {


    if (ImGui::BeginPopupModal("Rename Scene", &Enabled_, ImGuiWindowFlags_AlwaysAutoResize)) {
        std::cout<<"Opening Popup\n";
        ImGui::SetItemDefaultFocus();
        ImGui::InputTextWithHint("Rename Scene", "Enter New Scene Name", SceneInputName_, IM_ARRAYSIZE(SceneInputName_));

        ImGui::Separator();

        if (ImGui::Button("Rename", ImVec2(120, 0))) {
            SceneManager_->Scenes_[SelectedScene_].SceneName = std::string(SceneInputName_);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }


}
