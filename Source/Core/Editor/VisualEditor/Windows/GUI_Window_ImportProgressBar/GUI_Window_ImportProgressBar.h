//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This Window file is responsible for creating a system Window.
    Documentation Status: Complete
    Additonal Notes: None
    Date Created: 2021-12-17
*/

#pragma once


// Standard Libraries (BG convention: use <> instead of "")
#include <memory>
#include <thread>
#include <string>
#include <mutex>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <imgui.h>

// Internal Libraries (BG convention: use <> instead of "")


class Window_ImportProgressBar {

    private:

        long SubItemsImported_; /**<Set Number Of Items That Have Been Imported*/
        long SubItemsToImport_; /**<Set Total Number Of Items Imported*/
        std::string CurrentAssetName_; /**<Set Current Asset Name*/
        long CurrentAssetNumber_; /**<Set Current Number Of Asset Being Imported*/
        long TotalAssetsToImport_; /**<Set Total Number Of Assets To Import*/
        std::mutex LockViewStats_; /**<Mutex To Prevent Values Being Changed During View/Write*/

        bool Enabled_; /**<Is Popup Enabled*/

    public:

        Window_ImportProgressBar();
        ~Window_ImportProgressBar();

        void UpdateSubitems(long Imported, long Total, std::string CurrentName);
        void UpdateTotalItems(long Current, long Total);

        void DrawImportDialog();

};