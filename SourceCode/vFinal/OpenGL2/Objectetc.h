#pragma once
#include"Model1.h"

extern unsigned int SCR_WIDTH ;
extern unsigned int SCR_HEIGHT ;

extern Camera1 camera;




// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void RetrieveSizeOfWindow(GLFWwindow* window);

void scroll_callback(GLFWwindow* window,  double xoffset, double yoffset);