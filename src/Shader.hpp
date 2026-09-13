#include <string>

class Shader {
public:
    Shader(std::string vertexPath, std::string fragPath);
    
    unsigned int program;
private:
    void createShader(std::string path, unsigned int shader);

    unsigned int vert;
    unsigned int frag;
};
