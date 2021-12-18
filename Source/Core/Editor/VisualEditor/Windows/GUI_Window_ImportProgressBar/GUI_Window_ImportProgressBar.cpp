//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This Window file is responsible for creating a system Window.
    Documentation Status: Complete
    Additonal Notes: None
    Date Created: 2021-12-17
*/

#include <GUI_Window_ImportProgressBar.h>


// Constructor
Window_ImportProgressBar::Window_ImportProgressBar(ERS_STRUCT_SystemUtils SystemUtils) {

    // Copy System Utils Pointer Struct
    SystemUtils_ = SystemUtils;

}

// Destructor
Window_ImportProgressBar::~Window_ImportProgressBar() {

}


// Update Item Stats
void Window_ImportProgressBar::UpdateSubitems(long Imported, long Total, std::string CurrentName) {

}

// Update Total Model Stats
void Window_ImportProgressBar::UpdateTotalItems(long Current, long Total) {

}

// Draw Window
void Window_ImportProgressBar::Draw() {

    // FIXME: Lock Window Pos, Put Window In bottom Right Corner - Need To Get Window Size To Set Window Pos, Add GLFWWindow* to System Utils And Pass Into This Class?

    if (Enabled_) {
    ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;
    ImGui::Begin("Import Status", &Enabled_, WindowFlags);

        // Set Window Size, Pos
        ImGui::SetWindowSize(ImVec2(300,0));
        ImVec2 WindowPos = ImVec2(SystemUtils_.RenderWidth_, SystemUtils_.RenderHeight_);
        ImGui::SetWindowPos(WindowPos);

        // Calculate Stats
        LockViewStats_.lock();
        float TotalProgresBarFraction = CurrentAssetNumber_ / TotalAssetsToImport_;
        float ItemProgressBarFraction = SubItemsImported_ / SubItemsToImport_;
        LockViewStats_.unlock();

        // Draw Total Progres Bar
        ImGui::Text("Overall Progress");
        ImGui::ProgressBar(TotalProgresBarFraction);

        ImGui::Text("Item Progress");
        ImGui::ProgressBar(ItemProgressBarFraction);


    ImGui::End();
    }


}