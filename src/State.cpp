#include "State.hpp"
#include "Player.hpp"

State::State() {
    init();
}

void State::init() {
    name = "default";
}

void State::enter() {}
void State::exit() {}
void State::update() {}
