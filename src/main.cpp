#include <iostream>

#include "Window.hpp"
#include "Mesh.hpp"

int main() {
    if (Window::init() < 0) return -1;

    Mesh* monkey = Mesh::loadModel("Monkey.obj")[0];
    Mesh* monke = Mesh::loadModel("Monkey.obj")[0];

    monkey->anchored = false;
    monke->position.y = -5;

    Window::mainLoop();

    return 0;
}
