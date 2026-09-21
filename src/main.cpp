#include <iostream>

#include "Window.hpp"
#include "Mesh.hpp"

int main() {
    if (Window::init() < 0) return -1;

    Mesh* monkey = Mesh::loadModel("Monkey.obj")[0];
    Mesh* monkey2 = Mesh::loadModel("Monkey.obj")[0];
    Mesh* monke = Mesh::loadModel("Cube.obj")[0];

    monkey->anchored = false;
    monkey->canCollide = true;
    monkey2->anchored = false;
    monkey2->position.y = 5;
    monkey2->canCollide = true;
    monke->position.y = -5;
    monke->size = glm::vec3(100.0f, 1.0f, 100.0f);
    monke->canCollide = true;

    Window::mainLoop();

    return 0;
}
