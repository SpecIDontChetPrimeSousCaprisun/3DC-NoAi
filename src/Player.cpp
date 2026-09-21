#include <iostream>

#include "Player.hpp"

Player* Player::currentPlayer = nullptr;

void Player::init() {
    currentPlayer = new Player("Monkey.obj");
}

Player::Player(std::string path) : Mesh(path) {
    anchored = false;
    canCollide = true;
}

void Player::beforeUpdate() {
    if (this != currentPlayer) return;


}
