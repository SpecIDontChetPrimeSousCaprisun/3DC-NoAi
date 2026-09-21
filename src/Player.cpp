#include <iostream>

#include "Player.hpp"
#include "Window.hpp"

Player* Player::currentPlayer = nullptr;

void Player::init() {
    currentPlayer = new Player("Monkey.obj");
}

Player::Player(std::string path) : Mesh(path) {
    anchored = false;
    canCollide = true;
    transparency = 1.0f;
}

void Player::beforeUpdate() {
    if (this != currentPlayer) return;

    linearVelocity.x = 0.0f;
    linearVelocity.z = 0.0f;

    float y = linearVelocity.y;

    if (glfwGetKey(Window::window, GLFW_KEY_W)) {
	linearVelocity += Window::camera.forward * 10.0f;
    } else if (glfwGetKey(Window::window, GLFW_KEY_S)) {
	linearVelocity -= Window::camera.forward * 10.0f;
    } 

    if (glfwGetKey(Window::window, GLFW_KEY_D)) {
	linearVelocity += Window::camera.right * 10.0f;
    } else if (glfwGetKey(Window::window, GLFW_KEY_A)) {
	linearVelocity -= Window::camera.right * 10.0f;
    } 

    linearVelocity.y = y;
}
