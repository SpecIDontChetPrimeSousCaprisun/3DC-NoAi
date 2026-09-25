#include <iostream>

#include "Shader.hpp"
#include "Window.hpp"
#include "Mesh.hpp"
#include "DirLight.hpp"
#include "Player.hpp"

GLFWwindow* Window::window = nullptr;
Node* Window::parent = new Node();
Camera Window::camera;
DirLight Window::dirLight;
double Window::dt = 0;
double Window::lastFrame = glfwGetTime();
int Window::width = 1600;
int Window::height = 1200;
std::string Window::renderType = "normal";
unsigned int Window::depthMapFBO;
unsigned int Window::depthMap;
unsigned int Window::SHADOW_WIDTH = 1024;
unsigned int Window::SHADOW_HEIGHT = 1024;

void Window::frameBufferSizeCallback(GLFWwindow*, int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
    glViewport(0, 0, width, height);
} 

void Window::processInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

void Window::enableGlFunctions() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Window::initOtherClasses() {
    Mesh::init();
    PointLight::init();
    Player::init();

    dirLight.direction = glm::vec3(0.6f, 0.0f, 0.3f);
    dirLight.ambient = glm::vec3(0.25f, 0.1f, 0.0f);
    dirLight.diffuse = glm::vec3(0.0f, 0.7f, 0.3f);
    dirLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Window::generateDepthMap() {
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
		 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenFramebuffers(1, &depthMapFBO);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int Window::init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    window = glfwCreateWindow(width, height, "3dgame", NULL, NULL);
    if (window == NULL) {
	std::cout << "Failed to create GLFW window" << std::endl;
	glfwTerminate();
	return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	std::cout << "Failed to initialize GLAD" << std::endl;
	return -1;
    }

    enableGlFunctions();

    glViewport(0, 0, width, height);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    generateDepthMap();
    initOtherClasses();

    return 0;
}

void Window::mainLoop() {
    while(!glfwWindowShouldClose(window)) {
	glfwPollEvents();    

	double currentFrame = glfwGetTime();
	dt = currentFrame - lastFrame;
	lastFrame = currentFrame;

	if (dt > 0.1) {
	    dt = 0.1;
	}

	processInput();

	camera.update();
	parent->updateChildren();

	renderType = "depthMap";

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	parent->drawChildren();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	renderType = "normal";

	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	parent->drawChildren();

	glfwSwapBuffers(window);
    }

    glfwTerminate();
} 
