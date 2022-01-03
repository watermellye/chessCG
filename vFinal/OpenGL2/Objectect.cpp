#include "Objectetc.h"


unsigned int SCR_WIDTH = 1280;
unsigned int SCR_HEIGHT = 800;

Camera1 camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.33f, 0.33f));


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int wid, int hei)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, wid, hei);
    SCR_WIDTH = wid;
    SCR_HEIGHT = hei;
}

// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
void RetrieveSizeOfWindow(GLFWwindow* window)
{
    // then before rendering, configure the viewport to the original framebuffer's screen dimensions
    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);
}

//scroll new
void scroll_callback(GLFWwindow* window,double xoffset, double yoffset)
{
    glm::vec3 oldPosition = camera.Position;
    camera.Position += ((float)yoffset * 0.05f) * camera.Orientation;
    float R = float(sqrt(camera.Position.x * camera.Position.x + camera.Position.y * camera.Position.y + camera.Position.z * camera.Position.z));
    if (R < 0.2 || R > 0.8) camera.Position = oldPosition;
}