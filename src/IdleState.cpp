#include <iostream>

#include "IdleState.hpp"
#include "JumpingState.hpp"
#include "Window.hpp"
#include "Player.hpp"

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
    std::vector<Mesh*> result = Mesh::getMeshesInBounds(player->getWorldPosition() - glm::vec3(0.0f, 0.01f, 0.0f), player->size);
    
    for (auto it = result.begin(); it != result.end(); ) {
	if (*it == player) {
	    it = result.erase(it);
	    break;
	} else {
	    it++;
	}
    }

    if (glfwGetKey(Window::window, GLFW_KEY_SPACE) == GLFW_PRESS && result.size() > 0) {
	player->switchStates(new JumpingState());
    }
}

void IdleState::init() {
    name = "idle";
}
