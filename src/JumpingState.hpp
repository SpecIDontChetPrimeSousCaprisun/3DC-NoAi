#pragma once

#include "State.hpp"

class JumpingState : public State {
public:
    void enter() override;
    void update() override;
};
