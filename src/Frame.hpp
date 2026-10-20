#pragma once	

#include "Node.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

class Frame : public Node {
public:
    Frame();

    static void init();
protected:
    void update() override;
    void draw() override;
private:
    static Shader* shader;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    Texture tex;
};
