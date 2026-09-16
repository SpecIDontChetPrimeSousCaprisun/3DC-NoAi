#include <iostream>

#include "Window.hpp"
#include "Mesh.hpp"

int main() {
    if (Window::init() < 0) return -1;

    Mesh::loadModel("Monkey.obj");

    Window::mainLoop();

    return 0;
}
