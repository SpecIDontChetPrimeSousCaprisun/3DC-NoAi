#include "Camera.hpp"
#include "Window.hpp"

void Camera::update() {
    if (glfwGetKey(Window::window, GLFW_KEY_W) == GLFW_PRESS) {
	position.z += 1.0f;
    } else if (glfwGetKey(Window::window, GLFW_KEY_S) == GLFW_PRESS) {
	position.z -= 1.0f;
    }

    if (glfwGetKey(Window::window, GLFW_KEY_D) == GLFW_PRESS) {
	position.x -= 1.0f;
    } else if (glfwGetKey(Window::window, GLFW_KEY_A) == GLFW_PRESS) {
	position.x += 1.0f;
    }
}
