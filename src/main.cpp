#include <iostream>

#include "Window.hpp"

int main() {
    if (Window::init() < 0) return -1;
    Window::mainLoop();

    return 0;
}
