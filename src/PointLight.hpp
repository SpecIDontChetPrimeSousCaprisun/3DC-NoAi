#pragma once

#include "Node.hpp"

class Mesh;

class PointLight : public Node {
public:
    static void init();

    static PointLight lights[8];
    
    float constant = 0.7f;
    float linear = 0.45f;
    float quadratic = 0.31f;

    glm::vec3 ambient = glm::vec3(0.274509804f);
    glm::vec3 diffuse = glm::vec3(0.588235294f, 0.588235294f, 0.392156863f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
private:
    void genMesh();

    Mesh* mesh;
};
