#include <iostream>

#include "Player.hpp"
#include "IdleState.hpp"

Player* Player::currentPlayer = nullptr;

void Player::init() {
    currentPlayer = new Player("Monkey.obj");
}

Player::Player(std::string path) : Mesh(path) {
    anchored = false;
    canCollide = true;
    transparency = 1.0f;

    IdleState* newState = new IdleState();
    currentState = new State();

    switchStates(newState);
}

void Player::beforeUpdate() {
    if (this != currentPlayer) return;

    currentState->update();
}

void Player::switchStates(State* state) {
    currentState->exit();
    currentState = state;
    state->player = this;
    state->enter();
}
