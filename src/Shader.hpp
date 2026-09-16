#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    Shader(std::string vertexPath, std::string fragPath);

    void setBool(std::string name, bool value);
    void setInt(std::string name, int value);
    void setFloat(std::string name, float value);
    
    unsigned int program;
private:
    void createShader(std::string path, unsigned int shader);

    unsigned int vert;
    unsigned int frag;
};
