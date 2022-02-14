//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

#include <ERS_ProjectWriter.h>



ERS_CLASS_ProjectWriter::ERS_CLASS_ProjectWriter(std::shared_ptr<ERS_STRUCT_SystemUtils> SystemUtils) {

    SystemUtils_ = SystemUtils;
    SystemUtils->Logger_->Log("Initializing ERS Project Writer Class", 5);

}

ERS_CLASS_ProjectWriter::~ERS_CLASS_ProjectWriter() {

    SystemUtils_->Logger_->Log("System Destructor Called", 6);

}


bool ERS_CLASS_ProjectWriter::SaveProject(std::shared_ptr<ERS_STRUCT_Project> ProjectPointer, long AssetID) {

    SystemUtils_->Logger_->Log(std::string(std::string("Saving Project To Asset ID ") + std::to_string(AssetID)).c_str(), 5);

    // Serialize Data
    SystemUtils_->Logger_->Log("Serializing Project", 4);

    // Get Current Time In YYYY-MM DD-HH-MM-SS Format
    std::time_t RawCurrentTime;
    std::tm* TimeInformation;
    char TimeBuffer [80];

    std::time(&RawCurrentTime);
    TimeInformation = std::localtime(&RawCurrentTime);

    std::strftime(TimeBuffer, 80, "%Y-%m-%d %H-%M-%S", TimeInformation);
    std::string CurrentTime = std::string(TimeBuffer);

    // Write Data
    YAML::Emitter ProjectEmitter;
    ProjectEmitter<<YAML::BeginMap;
    ProjectEmitter<<YAML::Key<<"ProjectName"<<YAML::Value<<ProjectPointer->ProjectName;
    ProjectEmitter<<YAML::Key<<"ProjectDescription"<<YAML::Value<<ProjectPointer->ProjectDescription;
    ProjectEmitter<<YAML::Key<<"ProjectVersion"<<YAML::Value<<ProjectPointer->ProjectVersion;
    ProjectEmitter<<YAML::Key<<"ProjectCreationDate"<<YAML::Value<<ProjectPointer->ProjectCreationDate;
    ProjectEmitter<<YAML::Key<<"ProjectModificationDate"<<YAML::Value<<CurrentTime.c_str();
    ProjectEmitter<<YAML::Key<<"ProjectCreator"<<YAML::Value<<ProjectPointer->ProjectCreator;
    ProjectEmitter<<YAML::Key<<"ProjectLicense"<<YAML::Value<<ProjectPointer->ProjectLicense;
    ProjectEmitter<<YAML::Key<<"IsLicenseProprietary"<<YAML::Value<<ProjectPointer->IsLicenseProprietary;


    ProjectEmitter<<YAML::Key<<"SceneIDs";
    ProjectEmitter<<YAML::Key<<YAML::BeginMap;
    for (int i = 0; i < ProjectPointer->SceneIDs.size(); i++) {
        ProjectEmitter<<YAML::Key<<i<<ProjectPointer->SceneIDs[i];
    }
    ProjectEmitter<<YAML::EndMap;
    ProjectEmitter<<YAML::Key<<"DefaultScene"<<YAML::Value<<ProjectPointer->DefaultScene;


    ProjectEmitter<<YAML::Key<<"EditorLayouts";
    ProjectEmitter<<YAML::Key<<YAML::BeginMap;
    for (int i = 0; i < ProjectPointer->EditorLayoutIDs.size(); i++) {
        ProjectEmitter<<YAML::Key<<i<<ProjectPointer->EditorLayoutIDs[i];
    }
    ProjectEmitter<<YAML::EndMap;
    ProjectEmitter<<YAML::Key<<"DefaultLayout"<<YAML::Value<<ProjectPointer->DefaultLayout;

    ProjectEmitter<<YAML::Key<<"DefaultShaderProgram"<<YAML::Value<<ProjectPointer->DefaultShaderProgram;
    ProjectEmitter<<YAML::Key<<"ShaderPrograms";
    ProjectEmitter<<YAML::Key<<YAML::BeginMap;
    for (int i = 0; i < ProjectPointer->ShaderPrograms.size(); i++) {
        ProjectEmitter<<YAML::Key<<i;
        ProjectEmitter<<YAML::Key<<YAML::BeginMap;
        ProjectEmitter<<YAML::Key<<"Name"<<YAML::Value<<ProjectPointer->ShaderPrograms[i].Name;
        ProjectEmitter<<YAML::Key<<"VertexID"<<YAML::Value<<ProjectPointer->ShaderPrograms[i].VertexID;
        ProjectEmitter<<YAML::Key<<"FragmentID"<<YAML::Value<<ProjectPointer->ShaderPrograms[i].FragmentID;
        ProjectEmitter<<YAML::EndMap;
    }
    ProjectEmitter<<YAML::EndMap;


    // Convert Emitter To String
    std::string ProjectByteString = ProjectEmitter.c_str();


    // Write To IOData
    SystemUtils_->Logger_->Log("Writing Project To Asset", 4);
    std::shared_ptr<ERS_STRUCT_IOData> ProjectData = std::make_shared<ERS_STRUCT_IOData>();
    
    ProjectData->Data.reset(new unsigned char[ProjectByteString.size()]);
    ProjectData->Size_B = ProjectByteString.size();
    memcpy(ProjectData->Data.get(), ProjectByteString.c_str(), ProjectByteString.size());

    bool Status = SystemUtils_->ERS_IOSubsystem_->WriteAsset(AssetID, ProjectData);


    return Status;

}