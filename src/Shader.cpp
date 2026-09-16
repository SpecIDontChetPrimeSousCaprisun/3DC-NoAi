#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

#include "Shader.hpp"

Shader::Shader(std::string vertPath, std::string fragPath) {
    vert = glCreateShader(GL_VERTEX_SHADER);
    frag = glCreateShader(GL_FRAGMENT_SHADER);

    createShader(vertPath, vert);
    createShader(fragPath, frag);
    program = glCreateProgram();

    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_COMPILE_STATUS, &success);

    if (!success) {
	glGetShaderInfoLog(program, 512, NULL, infoLog);
	std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
}

void Shader::createShader(std::string path, unsigned int shader) {
    std::ifstream stream(path);
    std::stringstream buffer;

    buffer << stream.rdbuf();

    std::string code = buffer.str();
    const char* src = code.c_str();

    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
	glGetShaderInfoLog(shader, 512, NULL, infoLog);
	std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void Shader::setBool(std::string name, bool value) {
    glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
}

void Shader::setFloat(std::string name, float value) {
    glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setInt(std::string name, int value) {
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}
