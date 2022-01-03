#pragma once
#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shaderClass.h"

class Camera1 {
   public:
    // Stores the main vectors of the camera
    glm::vec3 Position;
    glm::vec3 Orientation = glm::vec3(0.0f, -0.01f, -0.01f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 cameraMatrix = glm::mat4(1.0f);

    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    // Prevents the camera from jumping around when first clicking left click
    bool firstClick = true;

    // Stores the width and height of the window
    int width;
    int height;

    // Adjust the speed of the camera and it's sensitivity when looking around
    float speed = 0.00002f;
    float sensitivity = 50.0f;

    // Camera constructor to set up initial values
    Camera1(int width, int height, glm::vec3 position);

    // Updates the camera matrix to the Vertex Shader
    void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
    // Exports the camera matrix to a shader
    void Matrix(Shader1& shader, const char* uniform);  // Handles camera inputs
    void Inputs(GLFWwindow* window);
    void setOrientation(glm::vec3 newOrientation);
    void setLookup(glm::vec3 _Position, glm::vec3 _Orientation, glm::vec3 _Up) {
        Position = _Position;
        Orientation = _Orientation;
        Up = _Up;
    }
};
#endif