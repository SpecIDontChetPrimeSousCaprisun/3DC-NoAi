#include <iostream>

#include "IdleState.hpp"
#include "Window.hpp"
#include "Player.hpp"

IdleState::IdleState() : State() {}

void IdleState::update() {
    player->linearVelocity.x = 0.0f;
    player->linearVelocity.z = 0.0f;

    float y = player->linearVelocity.y;

    if (glfwGetKey(Window::window, GLFW_KEY_W)) {
	player->linearVelocity += Window::camera.forward * 10.0f;
    } else if (glfwGetKey(Window::window, GLFW_KEY_S)) {
	player->linearVelocity -= Window::camera.forward * 10.0f;
    } 

    if (glfwGetKey(Window::window, GLFW_KEY_D)) {
	player->linearVelocity += Window::camera.right * 10.0f;
    } else if (glfwGetKey(Window::window, GLFW_KEY_A)) {
	player->linearVelocity -= Window::camera.right * 10.0f;
    } 

    player->linearVelocity.y = y;
}

void IdleState::init() {
    name = "idle";
}
