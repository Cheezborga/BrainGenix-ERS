//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This file provides asset import functionality.
    Documentation Status: Complete
    Additonal Notes: None
    Date Created: 2021-12-18
*/

#include <ERS_Editor_ImportAsset.h>


// Asset Importer Constructor
ERS_CLASS_ImportAsset::ERS_CLASS_ImportAsset(std::shared_ptr<ERS_STRUCT_SystemUtils> SystemUtils) {

    // Copy Shared Pointer
    SystemUtils_ = SystemUtils;

    // Log Init
    SystemUtils_->Logger_->Log("Initializing Asset Importer Backend", 5);

}


// Asset Importer Destructor
ERS_CLASS_ImportAsset::~ERS_CLASS_ImportAsset() {

    // Log Destructor
    SystemUtils_->Logger_->Log("Asset Importer Backend Destructor Called", 6);

}

// Item Import Thread
void ERS_CLASS_ImportAsset::ImportThread() {

    // Enter Loop
    while (true) {

        // Check Control Variables
        BlockThread_.lock();
        if (StopThread_) {
            break;
        }
        BlockThread_.unlock();

        // Check Queue, Import As Needed
        LockAssetImportQueue_.lock();
        if (AssetImportQueue_.size() > 0) {
            std::string AssetPath = AssetImportQueue_[0];
            AssetImportQueue_.erase(AssetImportQueue_.begin());
            LockAssetImportQueue_.unlock();



        } else {
            LockAssetImportQueue_.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

    }

}

// Call To Add Items To Import List
void ERS_CLASS_ImportAsset::AddToImportQueue(std::vector<std::string> AssetPaths) {

    // Log Asset Append
    SystemUtils_->Logger_->Log("Appending Assets To Asset Import Queue", 5);
    LockAssetImportQueue_.lock();

    // Iterate Through List
    for (int i = 0; i < AssetPaths.size(); i++) {

        // Log Append
        std::string LogStr = std::string("Appending Asset: '") + AssetPaths[i] + std::string("' To Import Queue");
        SystemUtils_->Logger_->Log(LogStr.c_str(), 4);

        // Append Item
        AssetImportQueue_.push_back(AssetPaths[i]);
        TotalItemsToImport_ += 1;

    }

    // Unlock Mutex
    LockAssetImportQueue_.unlock();

}

// Get Stats
long ERS_CLASS_ImportAsset::GetTotalSubitems() {
    return CurrentSubitemTotal_;
}

// Get Stats
long ERS_CLASS_ImportAsset::GetTotalSubitemsImported() {
    return CurrentSubitemIndex_;
}

// Get Stats
long ERS_CLASS_ImportAsset::GetTotalItemsToImport() {
    return TotalItemsToImport_;
}

// Get Stats
long ERS_CLASS_ImportAsset::GetTotalItemsImported() {
    return TotalItemsProcessed_;
}