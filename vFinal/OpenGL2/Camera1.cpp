#include "Camera1.h"
#include <cmath>
#include <minmax.h>
const float PI = 3.1415926;
extern int nowPos;
extern int nowFrame;
const int maxFrame = 60;
Camera1::Camera1(int width, int height, glm::vec3 position) {
    Camera1::width = width;
    Camera1::height = height;
    Position = position;
}
void Camera1::updateMatrix(float FOVdeg, float nearPlane, float farPlane) {
    // Initializes matrices since otherwise they will be the null matrix
    // Makes camera look in the right direction from the right position
    view = glm::lookAt(Position, Position + Orientation, Up);
    // Adds perspective to the scene
    projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);
    // Sets new camera matrix
    cameraMatrix = projection * view;
}

void Camera1::Matrix(Shader1& shader, const char* uniform) {
    // Exports camera matrix
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera1::setOrientation(glm::vec3 newOrientation) { Orientation = newOrientation; }

void Camera1::Inputs(GLFWwindow* window) {
    // Handles key inputs
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    float R;
    /*
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        R = float(sqrt(Position.x * Position.x + Position.y * Position.y + Position.z * Position.z));
        Position.x *= 0.99f;
        Position.z *= 0.99f;
        if (R * R - Position.x * Position.x - Position.z * Position.z < (R * 0.9) * (R * 0.9)) {
            Position.y = float(sqrt(R * R - Position.x * Position.x - Position.z * Position.z));
        } else {
            Position.x /= 0.99f;
            Position.z /= 0.99f;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        float arctan = 90;
        if (Position.x) arctan = atan(Position.z / Position.x) * 180 / PI;
        if (arctan < 0) arctan += 180;
        if (arctan < 170) {
            arctan += 1;
            arctan = arctan / 180 * PI;
            R = float(sqrt(Position.x * Position.x + Position.z * Position.z));
            Position.x = cos(arctan) * R;
            Position.z = sin(arctan) * R;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        R = float(sqrt(Position.x * Position.x + Position.y * Position.y + Position.z * Position.z));
        Position.x *= 1.01f;
        Position.z *= 1.01f;
        if (R * R - Position.x * Position.x - Position.z * Position.z > (R * 0.1) * (R * 0.1))
            Position.y = float(sqrt(R * R - Position.x * Position.x - Position.z * Position.z));
        else {
            Position.x /= 1.01f;
            Position.z /= 1.01f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        float arctan = 90;
        if (Position.x) arctan = atan(Position.z / Position.x) * 180 / PI;
        if (arctan < 0) arctan += 180;
        if (arctan > 10) {
            arctan -= 1;
            arctan = arctan / 180 * PI;
            R = float(sqrt(Position.x * Position.x + Position.z * Position.z));
            Position.x = cos(arctan) * R;
            Position.z = sin(arctan) * R;
        }
    }
    */
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    double mouseX;
    double mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    // 上下左右移动改变Position；再根据Position计算Orientation使其指向(0,0,0)
    // 移动时保持其半径不变
    {
        float arctan = max(20, min(80, 50 - 30 * (float)(mouseY - (height / 2)) / (height / 2)));
        arctan = arctan / 180 * PI;
        R = float(sqrt(Position.x * Position.x + Position.y * Position.y + Position.z * Position.z));
        Position.y = sin(arctan) * R;
        R = cos(arctan) * R;  // sqrt(x*x+z*z)
        float angle = 90;
        if (Position.x) angle = atan(Position.z / Position.x);
        Position.x = cos(angle) * R;
        Position.z = sin(angle) * R;
    }

    {
        float arctan = max(min(170, 90 - 80 * (float)(mouseX - (width / 2)) / (width / 2)), 10);
        arctan = arctan / 180 * PI;
        R = float(sqrt(Position.x * Position.x + Position.z * Position.z));
        Position.x = cos(arctan) * R;
        Position.z = sin(arctan) * R;
    }

    float cameraRatio = min(1.0f, float(nowFrame) / float(maxFrame)) * PI;
    cameraRatio = PI * (1 - cos(cameraRatio)) / 2;
    if (nowPos == 1) cameraRatio += PI;
    Position = glm::rotate(Position, cameraRatio, glm::vec3(0.0f, 1.0f, 0.0f));

    Orientation = glm::normalize(-Position);
}