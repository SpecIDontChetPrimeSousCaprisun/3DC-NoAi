#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "Node.hpp"

class Window {
public:
    static int init();
    static void mainLoop();
    
    static GLFWwindow* window;
    static Node* parent;
    static int height;
    static int width;
private:
    static void frameBufferSizeCallback(GLFWwindow*, int, int);
    static void processInput();
};
