#pragma once

#include "Mesh.hpp"

class Player : public Mesh {
public:
    Player(std::string path);

    static void init();
    
    static Player* currentPlayer;
protected:
    void beforeUpdate() override;
};
