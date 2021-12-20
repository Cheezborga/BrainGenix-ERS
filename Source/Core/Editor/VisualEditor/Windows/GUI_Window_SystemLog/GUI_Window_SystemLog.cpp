//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This Window file is responsible for creating a system Window.
    Documentation Status: Complete
    Additonal Notes: None
    Date Created: 2021-12-17
*/

#include <GUI_Window_SystemLog.h>


// Constructor
Window_SystemLog::Window_SystemLog(std::shared_ptr<ERS_STRUCT_SystemUtils> SystemUtils) {

    // Copy System Utils Pointer Struct
    SystemUtils_ = SystemUtils;

    // Log Init
    SystemUtils_->Logger_->Log("Initializing GUI_Window_SystemLog", 5);

}

// Destructor
Window_SystemLog::~Window_SystemLog() {

    // Log Destructor Call
    SystemUtils_->Logger_->Log("GUI_Window_SystemLog Destructor Called", 6);


}


// Draw Window
void Window_SystemLog::Draw() {

    if (Enabled_) {
    ImGui::Begin("System Log", &Enabled_);

        // Set Default Window Size
        ImGui::SetWindowSize(ImVec2(300,0), ImGuiCond_FirstUseEver);

        ImGui::BeginChild("Log Text");

        for (int i = 0; i < SystemUtils_->Logger_->LogMessages_.size(); i++) {

            float ColorRed = SystemUtils_->Logger_->LogColors_[i].Red/255.0f;
            float ColorGreen = SystemUtils_->Logger_->LogColors_[i].Green/255.0f;
            float ColorBlue = SystemUtils_->Logger_->LogColors_[i].Blue/255.0f;


            std::string LogText = SystemUtils_->Logger_->FullLogMessages_[i];
            ImVec4 TextColor = ImVec4(ColorRed, ColorGreen, ColorBlue, 1.0f);
            ImGui::TextColored(TextColor, "%s", LogText.c_str());

        }

        ImGui::EndChild();


    ImGui::End();
    }


}