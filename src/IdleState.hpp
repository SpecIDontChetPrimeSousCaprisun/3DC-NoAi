#pragma once

#include "State.hpp"

class IdleState : public State {
public:
    IdleState();

    void update() override;
protected:
    void init() override;
};
