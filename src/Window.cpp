#include <iostream>

#include "Shader.hpp"
#include "Window.hpp"
#include "Mesh.hpp"
#include "DirLight.hpp"

GLFWwindow* Window::window = nullptr;
Node* Window::parent = new Node();
Camera Window::camera;
DirLight Window::dirLight;
double Window::dt = 0;
double Window::lastFrame = glfwGetTime();
int Window::width = 1600;
int Window::height = 1200;
float Window::gamma = 2.2f;
float Window::farPlane = 20.0f;
std::string Window::renderType = "normal";
unsigned int Window::depthMapFBO;
unsigned int Window::depthCubemap;
unsigned int Window::SHADOW_WIDTH = 4096;
unsigned int Window::SHADOW_HEIGHT = 4096;
std::vector<glm::mat4> Window::shadowTransforms;
Player* Window::oldPlayer = nullptr;
bool Window::pPressed = false;
bool Window::mPressed = false;
bool Window::debug = false;

void Window::frameBufferSizeCallback(GLFWwindow*, int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
    glViewport(0, 0, width, height);
} 

void Window::processInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
	if (pPressed) return;

	pPressed = true;

	if (Player::currentPlayer == nullptr) {
	    Player::currentPlayer = oldPlayer;
	} else {
	    oldPlayer = Player::currentPlayer;
	    Player::currentPlayer = nullptr;
	}
    } else {
	pPressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
	if (mPressed) return;

	mPressed = true;
	debug = !debug;
    } else {
	mPressed = false;
    }
}

void Window::enableGlFunctions() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_FRAMEBUFFER_SRGB);
}

void Window::initOtherClasses() {
    Mesh::init();
    PointLight::init();
    Player::init();

    dirLight.position = glm::vec3(-2.0f, -1.0f, -1.0f);
    dirLight.ambient = glm::vec3(0.274509804f);
    dirLight.diffuse = glm::vec3(0.588235294f, 0.588235294f, 0.392156863f);
    dirLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    /*Mesh* mesh = Mesh::loadModel("Cube.obj")[0];

    mesh->position = dirLight.position;
    mesh->size = glm::vec3(0.25f, 0.25f, 0.25f);
    mesh->setParent(parent);
    mesh->canCollide = false;
    mesh->castShadows = false;*/
}

void Window::generateDepthMap() {
    glGenTextures(1, &depthCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  

    for (unsigned int i = 0; i < 6; ++i) {
	glTexImage2D(
	    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
	    0,
	    GL_DEPTH_COMPONENT,
	    SHADOW_WIDTH,
	    SHADOW_HEIGHT,
	    0,
	    GL_DEPTH_COMPONENT,
	    GL_FLOAT,
	    NULL
	);
    }

    glGenFramebuffers(1, &depthMapFBO);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Window::genDepthCubemapTransformMats() {
    float near_plane = 0.01f;
    glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, farPlane);
    shadowTransforms.clear();
    shadowTransforms.push_back(
	lightProjection * glm::lookAt(
	    dirLight.position,
	    dirLight.position + glm::vec3(1.0f, 0.0f, 0.0f),
	    glm::vec3(0.0, -1.0f, 0.0f)
	)
    );

    shadowTransforms.push_back(
	lightProjection * glm::lookAt(
	    dirLight.position,
	    dirLight.position + glm::vec3(-1.0f, 0.0f, 0.0f),
	    glm::vec3(0.0, -1.0f, 0.0f)
	)
    );

    shadowTransforms.push_back(
	lightProjection * glm::lookAt(
	    dirLight.position,
	    dirLight.position + glm::vec3(0.0f, 1.0f, 0.0f),
	    glm::vec3(0.0, 0.0f, 1.0f)
	)
    );

    shadowTransforms.push_back(
	lightProjection * glm::lookAt(
	    dirLight.position,
	    dirLight.position + glm::vec3(0.0f, -1.0f, 0.0f),
	    glm::vec3(0.0, 0.0f, -1.0f)
	)
    );

    shadowTransforms.push_back(
	lightProjection * glm::lookAt(
	    dirLight.position,
	    dirLight.position + glm::vec3(0.0f, 0.0f, 1.0f),
	    glm::vec3(0.0, -1.0f, 0.0f)
	)
    );

    shadowTransforms.push_back(
	lightProjection * glm::lookAt(
	    dirLight.position,
	    dirLight.position + glm::vec3(0.0f, 0.0f, -1.0f),
	    glm::vec3(0.0, -1.0f, 0.0f)
	)
    );
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

	dirLight.position.z = static_cast<float>(sin(glfwGetTime() * 0.5) * 3.0);
	genDepthCubemapTransformMats();
	processInput();

	camera.update();
	parent->updateChildren();

	renderType = "depthMap";

	glClearColor(0.0f, 0.717647059f, 0.921568627f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	//glCullFace(GL_FRONT);
	parent->drawChildren();
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	renderType = "normal";

	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	parent->drawChildren();

	glfwSwapBuffers(window);
    }

    glfwTerminate();
} 
