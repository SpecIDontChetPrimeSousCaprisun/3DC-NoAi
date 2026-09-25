#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "Node.hpp"
#include "Camera.hpp"
#include "DirLight.hpp"

class Window {
public:
    static int init();
    static void mainLoop();
    
    static GLFWwindow* window;
    static Node* parent;
    static Camera camera;
    static DirLight dirLight;
    static double dt;
    static double lastFrame;
    static int height;
    static int width;
    static std::string renderType;
    static unsigned int depthMap;
private:
    static void frameBufferSizeCallback(GLFWwindow*, int, int);
    static void processInput();
    static void enableGlFunctions();
    static void initOtherClasses();
    static void generateDepthMap();

    static unsigned int depthMapFBO;
    static unsigned int SHADOW_WIDTH;
    static unsigned int SHADOW_HEIGHT;
};
