#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.hpp"

class Material {
public:
    Texture diffuse;
    Texture specular;
    float shininess = 32.0f;
};
