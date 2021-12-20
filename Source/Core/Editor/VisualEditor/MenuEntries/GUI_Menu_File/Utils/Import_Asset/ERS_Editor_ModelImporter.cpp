//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This is the model loader.
    Documentation Status: Complete
    Additonal Notes: None
    Date Created: 2021-11-13
*/

#include <ERS_Editor_ModelImporter.h>


// Constructor
ERS_CLASS_ModelImporter::ERS_CLASS_ModelImporter(std::shared_ptr<ERS_STRUCT_SystemUtils> SystemUtils) {

    // Create Local Pointer
    SystemUtils_ = SystemUtils;

}

// Destructor
ERS_CLASS_ModelImporter::~ERS_CLASS_ModelImporter() {

}

// Load Model From File
long ERS_CLASS_ModelImporter::ImportModel(std::string AssetPath) {

    // Clear Model Instance
    ERS_OBJECT_MODEL Model;
    Model.AssetPath_ = AssetPath;

    // Clear Textures List
    TextureList_ = std::vector<std::string>();


    // Get Model Path
    std::string ModelDirectory = AssetPath.substr(0, std::string(AssetPath).find_last_of("/"));

    // Read File
    Assimp::Importer Importer;
    SystemUtils_->Logger_->Log(std::string(std::string("Loading Model At File Path: ") + std::string(AssetPath)).c_str(), 3);
    const aiScene* Scene = Importer.ReadFile(AssetPath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    // Log Errors
    if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode) {
        SystemUtils_->Logger_->Log(std::string(std::string("Model Loading Error: ") + std::string(Importer.GetErrorString())).c_str(), 10);
        return -1;
    }

    // Process Root Node Recursively, Identify Textures/Files
    ProcessNode(&Model, Scene->mRootNode, Scene, ModelDirectory);


    // Copy Model File
    std::shared_ptr<ERS_STRUCT_IOData> Data = std::make_shared<ERS_STRUCT_IOData>();
    ReadFile(AssetPath, Data);
    long ModelID = SystemUtils_->ERS_IOSubsystem_->AllocateAssetID();
    SystemUtils_->Logger_->Log(std::string(std::string("Assigning Model '") + AssetPath + std::string("' ID '") + std::to_string(ModelID) + std::string("'")).c_str(), 4);
    SystemUtils_->ERS_IOSubsystem_->WriteAsset(ModelID, Data);    

    // Copy Textures
    std::vector<long> TextureIDs = SystemUtils_->ERS_IOSubsystem_->BatchAllocateIDs(TextureList_.size());
    for (int i = 0; i < TextureList_.size(); i++) {
        SystemUtils_->Logger_->Log(std::string(std::string("Assigning Texture '") + TextureList_[i] + std::string("' ID '") + std::to_string(TextureIDs[i]) + std::string("'")).c_str(), 4);
        ReadFile(TextureList_[i], Data);
        SystemUtils_->ERS_IOSubsystem_->WriteAsset(TextureIDs[i], Data);

    }

   
    // Generate Metadata
    YAML::Emitter MetadataEmitter;
    MetadataEmitter<<YAML::BeginMap;

    MetadataEmitter<<YAML::Key<<"ModelID"<<YAML::Value<<ModelID;

    MetadataEmitter<<YAML::Key<<"TextureIDs"<<YAML::BeginSeq;
    for (int i = 0; i < TextureIDs.size(); i++) {
        MetadataEmitter<<YAML::Value<<TextureIDs[i];
    }
    MetadataEmitter<<YAML::EndSeq;
    
    MetadataEmitter<<YAML::EndMap;


    // Write Metadata
    unsigned char MetadataBytes = (unsigned char)MetadataEmitter.c_str();
    Data->Data = std::make_unique<unsigned char[]>(MetadataBytes);
    Data->Size_B = sizeof(MetadataBytes);
    long MetadataID = SystemUtils_->ERS_IOSubsystem_->AllocateAssetID();
    SystemUtils_->ERS_IOSubsystem_->WriteAsset(MetadataID, Data);


    // Return Model Instance
    return MetadataID;

}



// Process Nodes
void ERS_CLASS_ModelImporter::ProcessNode(ERS_OBJECT_MODEL* Model, aiNode *Node, const aiScene *Scene, std::string ModelDirectory) {

    // Process Meshes In Current Node
    for (unsigned int i = 0; i < Node->mNumMeshes; i++) {
        aiMesh* Mesh = Scene->mMeshes[Node->mMeshes[i]];
        ProcessMesh(Model, Mesh, Scene, ModelDirectory);
    }

    // Process Children Nodes
    for (unsigned int i = 0; i < Node->mNumChildren; i++) {
        ProcessNode(Model, Node->mChildren[i], Scene, ModelDirectory);
    }


}

// Process Mesh
void ERS_CLASS_ModelImporter::ProcessMesh(ERS_OBJECT_MODEL* Model, aiMesh *Mesh, const aiScene *Scene, std::string ModelDirectory) {

    // // Create Data Holders
    // std::vector<ERS_OBJECT_VERTEX> Vertices;
    // std::vector<unsigned int> Indices;
    // std::vector<ERS_OBJECT_TEXTURE_2D> Textures;

    // // Iterate Through Meshes' Vertices
    // for (unsigned int i = 0; i < Mesh->mNumVertices; i++) {

    //     // Hold Vertex Data
    //     ERS_OBJECT_VERTEX Vertex;
    //     glm::vec3 Vector;


    //     Vector.x = Mesh->mVertices[i].x;
    //     Vector.y = Mesh->mVertices[i].y;
    //     Vector.z = Mesh->mVertices[i].z;
    //     Vertex.Position = Vector;

    //     if (Mesh->HasNormals())
    //     {
    //         Vector.x = Mesh->mNormals[i].x;
    //         Vector.y = Mesh->mNormals[i].y;
    //         Vector.z = Mesh->mNormals[i].z;
    //         Vertex.Normal = Vector;
    //     }

    //     if (Mesh->mTextureCoords[0]) {

    //         glm::vec2 Vec;

    //         // Get Texture Coordinates
    //         Vec.x = Mesh->mTextureCoords[0][i].x;
    //         Vec.y = Mesh->mTextureCoords[0][i].y;
    //         Vertex.TexCoords = Vec;

    //         // Tangent
    //         Vector.x = Mesh->mTangents[i].x;
    //         Vector.y = Mesh->mTangents[i].y;
    //         Vector.z = Mesh->mTangents[i].z;
    //         Vertex.Tangent = Vector;

    //         // Bitangent
    //         Vector.x = Mesh->mBitangents[i].x;
    //         Vector.y = Mesh->mBitangents[i].y;
    //         Vector.z = Mesh->mBitangents[i].z;
    //         Vertex.Bitangent = Vector;

    //     } else {
    //         Vertex.TexCoords = glm::vec2(0.0f, 0.0f);
    //     }

    //     Vertices.push_back(Vertex);



    // }

    // // Iterate Through Faces
    // for (unsigned int i = 0; i < Mesh->mNumFaces; i++) {

    //     aiFace Face = Mesh->mFaces[i];

    //     // Get Face Indices
    //     for (unsigned int j = 0; j < Face.mNumIndices; j++) {
    //         Indices.push_back(Face.mIndices[j]);
    //     }
    // }

    // Process Materials
    aiMaterial* Material = Scene->mMaterials[Mesh->mMaterialIndex];


    AddTexture(Model, Material, aiTextureType_DIFFUSE, "texture_diffuse", ModelDirectory);
    AddTexture(Model, Material, aiTextureType_SPECULAR, "texture_specular", ModelDirectory);
    AddTexture(Model, Material, aiTextureType_NORMALS, "texture_normal", ModelDirectory);
    AddTexture(Model, Material, aiTextureType_AMBIENT, "texture_height", ModelDirectory);


}

// Check Material Textures
void ERS_CLASS_ModelImporter::AddTexture(ERS_OBJECT_MODEL* Model, aiMaterial *Mat, aiTextureType Type, std::string TypeName, std::string ModelDirectory) {


    for (unsigned int i=0; i< Mat->GetTextureCount(Type); i++) {

        aiString Str;
        Mat->GetTexture(Type, i, &Str);

        // Calculate Texture Path, Append If Not Already In List
        std::string FilePath = std::string(ModelDirectory + std::string(Model->Directory)  + std::string("/") + std::string(Str.C_Str()));
        if (std::find(TextureList_.begin(), TextureList_.end(), FilePath) == TextureList_.end()) {
            TextureList_.push_back(FilePath);
        }   

    }

}

// Load IOData
void ERS_CLASS_ModelImporter::ReadFile(std::string FilePath, std::shared_ptr<ERS_STRUCT_IOData> OutputData) {


    struct stat Buffer;
    int FileStatus = stat(FilePath.c_str(), &Buffer);


        if (FileStatus == 0) {

            // Allocate Memory
            OutputData->Data.reset(new unsigned char[Buffer.st_size]);
            if (OutputData->Data) {

                FILE *Stream = fopen(FilePath.c_str(), "rb");
                if (Stream) {

                    fread(OutputData->Data.get(), sizeof(unsigned char), Buffer.st_size, Stream);
                    OutputData->Size_B = Buffer.st_size;
                    fclose(Stream);
                    OutputData->HasLoaded = true;

                } else {
                    SystemUtils_->Logger_->Log(std::string(std::string("Error Loading Asset '") + FilePath + std::string("', Failed To Open Filestream")).c_str(), 9);
                    OutputData->HasLoaded = false;
                }

            } else {
                SystemUtils_->Logger_->Log(std::string(std::string("Error Loading Asset '") + FilePath + std::string("', Memory Allocation Failed")).c_str(), 9);            
                OutputData->HasLoaded = false;
            }
        
        } else {
            SystemUtils_->Logger_->Log(std::string(std::string("Error Loading Asset '") + FilePath + std::string("', File Not Found")).c_str(), 9);
            OutputData->HasLoaded = false;
        }

}