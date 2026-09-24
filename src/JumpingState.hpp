#pragma once

#include "State.hpp"

class JumpingState : public State {
public:
    JumpingState();

    void enter() override;
protected:
    void init() override;
};
