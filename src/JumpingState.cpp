#include "JumpingState.hpp"
#include "IdleState.hpp"
#include "Player.hpp"

void JumpingState::enter() {
    player->position.x += 0.01;
    player->linearVelocity.y = 2;
    player->switchStates(new IdleState());
}

void JumpingState::update() {}
