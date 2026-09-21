#pragma once

#include "Mesh.hpp"
#include "State.hpp"

class Player : public Mesh {
public:
    Player(std::string path);

    static void init();
    
    static Player* currentPlayer;

    void switchStates(State* state);

    State* currentState;
protected:
    void beforeUpdate() override;
};
