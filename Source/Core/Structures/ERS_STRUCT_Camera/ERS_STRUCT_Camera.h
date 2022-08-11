//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <vector>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



// Define Camera Directions
enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};


// Setup Camera Options
class ERS_STRUCT_Camera {

public:

    // Camera Attributes
    glm::vec3 Position_;
    glm::vec3 Front_;
    glm::vec3 Up_;
    glm::vec3 Right_;
    glm::vec3 WorldUp_;

    // Asset Streaming Config
    int Priority_ = 1; // Higher this is, the more the system will try and load assets for this camera. Should be in range (1-10)

    // Euler Angles
    float Yaw_;
    float Pitch_;

    // Camera Options
    double MovementSpeed_;
    float MouseSensitivity_;
    float FOV_;
    float AspectRatio_;

    // Config Params
    float MinMovementSpeed_ = 0.0f;
    float MaxMovementSpeed_ = 50.0f;

    // Constructor With Vectors
    ERS_STRUCT_Camera(glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f),
                                glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f),
                                float Yaw = -90.0f,
                                float Pitch = 0.0f) : Front_(glm::vec3(0.0f, 0.0f, -1.0f)),
                                MovementSpeed_(1.0f),
                                MouseSensitivity_(0.1f),
                                FOV_(45.0f) {
        
        // Set Params
        Position = Position;
        WorldUp_ = Up;
        Yaw_ = Yaw;
        Pitch_ = Pitch;
        UpdateCameraVectors();
        
    }

    // Constructor With Scalar Values
    ERS_STRUCT_Camera(float PosX,
                                float PosY,
                                float PosZ,
                                float UpX,
                                float UpY,
                                float UpZ,
                                float Yaw,
                                float Pitch) : Front_(glm::vec3(0.0f, 0.0f, -1.0f)),
                                MovementSpeed_(1.0f),
                                MouseSensitivity_(0.1f),
                                FOV_(45.0f) {
        Position_ = glm::vec3(PosX, PosY, PosZ);
        WorldUp_ = glm::vec3(UpX, UpY, UpZ);
        Yaw_ = Yaw;
        Pitch_ = Pitch;
        UpdateCameraVectors();
    }

    // Return View Matrix
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();


    // Proces Keyboard Input
    void ProcessKeyboard(CameraMovement Direction, float DeltaTime);

    // Process Mouse Input
    void ProcessMouseMovement(float XOffset, float Yoffset, GLboolean ConstrainPitch = true);
    void ProcessMouseScroll(float YOffset);

    void SetAspectRatio(float AspectRatio);
    void Rotate(glm::vec3 Rotation);

private:

    void UpdateCameraVectors();


};

