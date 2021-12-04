//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This is responsible for implementing the hardware information structure.
    Documentation Status: Complete
    Additonal Notes: None
    Date Created: 2021-12-03
*/



/**
 * @brief This structure is used to store static hardware information.
 * 
 */
struct StaticHardwareInfo {


};


/**
 * @brief This structure is used to store dynamically changing hardware information.
 * 
 */
struct DynamicHardwareInfo {

};



/**
 * @brief This structure is used as a container for static and dynamic hardware info structs.
 * 
 */
struct ERS_STRUCT_HardwareInfo {

    // Set Dynamic, Static Info
    StaticHardwareInfo Static_; /**<Structure To Store Static Hardware Information*/
    DynamicHardwareInfo Dynamic_; /**<Structure To Store Dynamic Hardware Information*/

};


