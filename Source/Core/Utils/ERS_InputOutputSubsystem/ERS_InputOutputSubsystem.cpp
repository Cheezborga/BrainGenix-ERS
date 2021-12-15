//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This is responsible for providing a single I/O interface for asset loading/saving.
    Documentation Status: Complete
    Additonal Notes: None
    Date Created: 2021-12-19
*/

#include <ERS_InputOutputSubsystem.h>


// Constructor
ERS_CLASS_InputOutputSubsystem::ERS_CLASS_InputOutputSubsystem(std::shared_ptr<LoggerClass> Logger, YAML::Node SystemConfiguration) {

    // Copy Pointer
    Logger_ = Logger;

    // Log Initialization
    Logger_->Log("Initializing Input/Output Subsystem", 5);

    // Get Database Loading / File Loading Config
    Logger_->Log("Reading Configuration For 'BOOL' 'UseDatabaseLoading'", 1);
    try {
        UseDatabase_ = SystemConfiguration["UseDatabaseLoading"].as<bool>();
    } 
    catch(YAML::TypedBadConversion<bool>) { // Config Param Doesn't Exist
        Logger_->Log("Configuration Error, Parameter 'UseDatabaseLoading' Is Not In Config, System Will Default To False", 8);
        UseDatabase_ = false;
    }

    // If Using DB Loading, Get DB Params
    if (UseDatabase_) {
        Logger_->Log("Database Loading Enabled, Reading Config For Database Parameters", 5);

        /*

            DATABASE LOADING STUFF HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        */

    } else {
        Logger_->Log("Database Lading Disabled, Reading Config For Asset Path", 5);

        try {
            Logger_->Log("Reading Configuration For 'STRING' 'AssetPath'", 1);
            AssetPath_ = SystemConfiguration["AssetPath"].as<std::string>();
        } catch (YAML::TypedBadConversion<std::string>) {
            Logger_->Log("Configuration Error, Parameter 'AssetPath' Is Not In Config, System Will Exit", 10);
            exit(1);
        }

    }

}



// Destructor
ERS_CLASS_InputOutputSubsystem::~ERS_CLASS_InputOutputSubsystem() {

    // Log Destructor Call
    Logger_->Log("Input/Output Subsystem Destructor Called", 6);

}



// Read Assets From File/DB, Return Bytes
char* ERS_CLASS_InputOutputSubsystem::ReadAsset(long AssetID) {

    // Var To STore Data In
    char* Data;


    // If Database Loading
    if (UseDatabase_) {

        // Load From DB

    } else {

        // Generate File Path
        std::string FilePath = AssetPath_ + std::to_string(AssetID);

        // Open File, Read Contents Into Data Buffer, Close Stream
        std::ifstream FileStream;
        FileStream.open(FilePath, std::ios::binary | std::ios::ate);
        FileStream.read(Data, FileStream.tellg());
        FileStream.close();

    }


    // Return Data
    return Data

}