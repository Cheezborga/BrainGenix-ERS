//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This widget file is responsible for creating a system widget.
    Documentation Status: Complete
    Additonal Notes: None
    Date Created: 2021-12-07
*/

#include <GUI_Widget_SceneTree.h>

// Constructor
Widget_SceneTree::Widget_SceneTree(SceneManager* SceneManager) {

    // Update Ptr
    SceneManager_ = SceneManager;


}

// Destructor
Widget_SceneTree::~Widget_SceneTree() {
    
}

// Define Draw Function
void Widget_SceneTree::Draw() {
    ImGui::ShowDemoWindow();
    // If Window Drawing Enabled
    if (Enabled_) {
        ImGui::Begin("Scene Tree", &Enabled_);

            // Set Initial Window Size
            ImGui::SetWindowSize(ImVec2(400,250), ImGuiCond_FirstUseEver);

            // Create Scene Trees
            for (int SceneIndex = 0; SceneIndex<SceneManager_->Scenes_; SceneIndex++) {

                // Begin Tree
                ImGui::TreeNode(SceneManager_->Scenes_[SceneIndex].SceneName.c_str())

                // End Node
                ImGui::TreePop();

            }



            

        // End System Controls Window
        ImGui::End();

    }



}