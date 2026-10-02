#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "Node.hpp"
#include "Camera.hpp"
#include "DirLight.hpp"
#include "Player.hpp"

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
    static float gamma;
    static float farPlane;
    static std::string renderType;
    static unsigned int depthCubemap;
    static std::vector<glm::mat4> shadowTransforms;
    static bool debug;
private:
    static void frameBufferSizeCallback(GLFWwindow*, int, int);
    static void processInput();
    static void enableGlFunctions();
    static void initOtherClasses();
    static void generateDepthMap();
    static void genDepthCubemapTransformMats();

    static unsigned int depthMapFBO;
    static unsigned int SHADOW_WIDTH;
    static unsigned int SHADOW_HEIGHT;
    static Player* oldPlayer;
    static bool pPressed;
    static bool mPressed;
};
