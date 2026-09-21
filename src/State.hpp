#pragma once

#include <vector>
#include <string>

class Player;

class State {
public:
    State();

    std::string name;
    Player* player;

    virtual void enter();
    virtual void exit();
    virtual void update();
protected:
    virtual void init();
};
