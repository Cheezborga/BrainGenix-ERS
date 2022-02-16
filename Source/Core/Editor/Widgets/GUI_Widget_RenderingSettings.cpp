//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

#include <GUI_Widget_RenderingSettings.h>

Widget_RenderingSettings::Widget_RenderingSettings(ERS_STRUCT_SystemUtils* SystemUtils) {

    SystemUtils_ = SystemUtils;
    SystemUtils_->Logger_->Log("Initializing Rendering Settings Widget", 5);

}

Widget_RenderingSettings::~Widget_RenderingSettings() {

}

void Widget_RenderingSettings::Draw() {

    if (Enabled_) {
        bool Visible = ImGui::Begin("Rendering Settings", &Enabled_);

            // Set Initial Window Size
            ImGui::SetWindowSize(ImVec2(400,250), ImGuiCond_FirstUseEver);


            if (Visible) {

                // Wireframe Rendering Mode
                ImGui::Checkbox("Wireframe Rendering Mode", &OpenGLDrawLines_);
                ImGui::NewLine();

                // Rendering Background Clear Color
                ImGui::ColorEdit4("Background Clear Color", (float*)&ClearColor_);
                ImGui::NewLine();


                // Framerate Settings
                ImGui::Separator();
                ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "Framerate Settings:");

                // Framerate Cap (Linux Only, Windows Doesn't work for whatever reason)
                if (SystemUtils_->IsLinux_) {

                    int OldFrameRate = SystemUtils_->FramerateManager_->TargetFrameRate_;
                    ImGui::SliderInt("Target Framerate", &SystemUtils_->FramerateManager_->TargetFrameRate_, 10, 100);
                    if (OldFrameRate != SystemUtils_->FramerateManager_->TargetFrameRate_) {
                        SystemUtils_->FramerateManager_->SetTargetFramerate(SystemUtils_->FramerateManager_->TargetFrameRate_);
                    }
                }

                // Unlock FPS
                bool UnlockChanged = ImGui::Checkbox("Unlock Framerate", &SystemUtils_->FramerateManager_->UnlockFramerate_);

                // Sync To Monitor
                ImGui::SameLine();
                bool SyncChanged = ImGui::Checkbox("Sync To Monitor", &SystemUtils_->FramerateManager_->SyncToMonitor_);
   

                // Disallow Both To Be Checked At The Same Time
                if (!SystemUtils_->IsLinux_) {
                    if (UnlockChanged) {
                        if (SystemUtils_->FramerateManager_->UnlockFramerate_) {
                            SystemUtils_->FramerateManager_->SyncToMonitor_ = false;
                        } else {
                            SystemUtils_->FramerateManager_->SyncToMonitor_ = true;
                        }
                    } else if (SyncChanged) {
                        if (SystemUtils_->FramerateManager_->SyncToMonitor_) {
                            SystemUtils_->FramerateManager_->UnlockFramerate_ = false;
                        } else {
                            SystemUtils_->FramerateManager_->UnlockFramerate_ = true;
                        }
                    }
                } else {
                    if (UnlockChanged) {
                        if (SystemUtils_->FramerateManager_->UnlockFramerate_) {
                            SystemUtils_->FramerateManager_->SyncToMonitor_ = false;
                        }
                    } else if (SyncChanged) {
                        if (SystemUtils_->FramerateManager_->SyncToMonitor_) {
                            SystemUtils_->FramerateManager_->UnlockFramerate_ = false;
                        }
                    }
                }

                // FIXME: Add Camera Struct To Grab FOV For All Cameras
                // FOV Slider
                //ImGui::Separator();
                //ImGui::SliderInt("Viewport FOV", &Camera_, 1, 360)

            }



        // End System Controls Window
        ImGui::End();

    }

    // Update OpenGL Settings
    if (OpenGLDrawLines_) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glClearColor(ClearColor_.x, ClearColor_.y, ClearColor_.z, ClearColor_.w);


}