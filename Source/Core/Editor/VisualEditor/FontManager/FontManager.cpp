//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This file provides font loading/selecting functionality.
    Documentation Status: Complete
    Additonal Notes: None
    Date Created: 2021-11-30
*/

#include <FontManager.h>


// FontManager Constructor
FontManager::FontManager(LoggerClass *Logger, const char* FontsDirPath) {

    // Create Pointers
    Logger_ = Logger;

    // Log Initialization
    Logger_->Log("Initializing Font Manager", 5);

    // Copy Vars
    FontsDirectoryPath_ = FontsDirPath;

    // Index Fonts
    IndexFonts();

}

// FontManager Destructor
FontManager::~FontManager() {
    
    // Log Destructor Call
    Logger_->Log("Font Manager Destructor Called", 6);

}

// Check String Endings (From: https://stackoverflow.com/questions/874134/find-out-if-string-ends-with-another-string-in-c)
bool FontManager::EndsWith(const std::string& Input, const std::string& Ending) {
    if (Ending.size() > Input.size()) return false;
    return std::equal(Input.begin() + Input.size() - Ending.size(), Input.end(), Ending.begin());
}

// Index Fonts Function
void FontManager::IndexFonts() {

    // Log Indexing
    Logger_->Log(std::string(std::string("Indexing Fonts In Dir: ") + std::string(FontsDirectoryPath_)).c_str(), 4);


    // Get List Of Files At Path
    for (const auto &Entry : std::filesystem::recursive_directory_iterator(std::string(FontsDirectoryPath_))) {

        // Get File Path        
        std::string FilePath{Entry.path().u8string()};

        // Check Extension
        if ((EndsWith(FilePath, ".ttf")) || (EndsWith(FilePath, ".otf"))) {

            // Append Path To PathList
            FontPathList_.push_back(FilePath);

            // Strip File Extension
            std::string FontName = FilePath.substr(0, strlen(FilePath.c_str())-4);

            // Strip Path
            int LastSlashIndex = FontName.find_last_of("/");
            FontName = FontName.substr(LastSlashIndex+1, strlen(FontName.c_str())-LastSlashIndex);

            // Append Font Name To FontNameList
            FontNameList_.push_back(FontName); 


            // Log Font Found
            std::string LogText = std::string("Indexed Font: ") + FilePath;
            Logger_->Log(LogText.c_str(), 3);

        }
    
    }

}

// Use Font Function
void FontManager::UseFont(int FontIndex) {

    // Get Font Path From Dir
    const char* FontPath = FontPathList_[FontIndex].c_str();

    // Log Font Adjustment
    Logger_->Log(std::string(std::string("Changing Font To: ") + std::string(FontPath)).c_str(), 4);

    // Load, Apply Font
    ImGuiIO& Io = ImGui::GetIO();
    Io.Fonts->AddFontFromFileTTF(FontPath, FontSize_);
    Io.Fonts->Build();
    ImGui_ImplOpenGL3_CreateFontsTexture();

}

// CheckUpdate
void FontManager::CheckUpdateFont() {

    // If Font Is To Be Updated
    if (UpdateFont_) {
        UseFont(FontSelector_);
        UpdateFont_ = false;
    }

}

// Font Selector Window
void FontManager::FontSelectorWindow(bool *WindowEnabled) {

    // If Window Enabled
    if (*WindowEnabled) {

        // Draw Window
        ImGuiWindowFlags Flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
        if (ImGui::Begin("Font Selector", WindowEnabled, Flags)) {
            ImGui::SetWindowSize(ImVec2(0,0));

                // Font Selector Radio Button List
                ImGui::BeginChild("Font Radio Buttons", ImVec2(300, 400), true);

                    for (int i = 0; i < FontNameList_.size(); i++) {
                        ImGui::RadioButton(FontNameList_[i].c_str(), &FontSelector_, i);
                    }

                ImGui::EndChild();

                // Dividing Line
                ImGui::Separator();

                // Reload, Apply, Close Buttons
                if (ImGui::Button("Reload")) {
                    IndexFonts();
                }
                ImGui::SameLine();

                if (ImGui::Button("Apply")) {
                    UpdateFont_ = true;
                }
                ImGui::SameLine();

                if (ImGui::Button("Close")) {
                    *WindowEnabled = false;
                }


            ImGui::End();
        }

    }

}