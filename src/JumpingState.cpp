#include "JumpingState.hpp"
#include "IdleState.hpp"
#include "Player.hpp"

JumpingState::JumpingState() : State() {
    init();
}

void JumpingState::enter() {
    //player->position.y += 0.01;
    player->linearVelocity.y = 3.5f;
    player->switchStates(new IdleState());
}

void JumpingState::init() {
    name = "jumping";
}
