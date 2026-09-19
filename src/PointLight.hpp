#pragma once

#include "Node.hpp"

class PointLight : public Node {
public:
    static void init();

    static PointLight lights[8];
    
    float constant = 0.7f;
    float linear = 0.45f;
    float quadratic = 0.31f;

    glm::vec3 ambient = glm::vec3(0.25f, 0.1f, 0.0f);
    glm::vec3 diffuse = glm::vec3(0.0f, 0.7f, 0.3f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
};
