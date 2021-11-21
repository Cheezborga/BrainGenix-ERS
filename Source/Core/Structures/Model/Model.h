//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This file creates the ERSModel Struct/Functions.
    Additonal Notes: None
    Date Created: 2021-11-04
*/ 

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <sys/stat.h>
#include <stdlib.h>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Internal Libraries (BG convention: use <> instead of "")
#include <Mesh.h>
#include <2DTexture.h>
#include <Vertex.h>
#include <FileIO.h>
#include <TextureLoader.h>




class ERS_OBJECT_MODEL {


    public:

        // Model Data
        std::vector<ERS_OBJECT_MESH> Meshes;
        std::vector<ERS_OBJECT_TEXTURE_2D> Textures_Loaded;
        std::string Directory;

        const char* AssetPath_;

        bool GammaCorrection;

        // Logger
        LoggerClass *Logger_;
        TextureLoader *TextureLoader_;

        // Initialize
        void ModelLoader(LoggerClass* Logger, TextureLoader *TextureLoader);

        // Load Model From File
        void LoadModelFromFile(const char* AssetPath);

        // Draw Model
        void Draw(ERS_OBJECT_SHADER &Shader);


    private:

        void ProcessNode(aiNode *Node, const aiScene *Scene);

        ERS_OBJECT_MESH ProcessMesh(aiMesh *Mesh, const aiScene *Scene);

        std::vector<ERS_OBJECT_TEXTURE_2D> LoadMaterialTextures(aiMaterial *Mat, aiTextureType Type, std::string TypeName);



};


