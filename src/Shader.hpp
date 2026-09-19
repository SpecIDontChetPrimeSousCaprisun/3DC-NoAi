#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "DirLight.hpp"
#include "PointLight.hpp"
#include "Material.hpp"

class Shader {
public:
    Shader(std::string vertexPath, std::string fragPath);

    void setBool(std::string name, bool value);
    void setInt(std::string name, int value);
    void setFloat(std::string name, float value);
    void setMatrix(std::string name, glm::mat4 value);
    void setVec3(std::string name, glm::vec3 value);
    void setDirLight(DirLight light);
    void setPointLight(int number, PointLight light);
    void setMaterial(Material material);
    
    unsigned int program;
private:
    void createShader(std::string path, unsigned int shader);

    unsigned int vert;
    unsigned int frag;
};
