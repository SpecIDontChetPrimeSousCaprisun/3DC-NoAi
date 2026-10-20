#include <glad/glad.h>
#include <iostream>

#include "Frame.hpp"
#include "Window.hpp"

Shader* Frame::shader;

void Frame::init() {
    shader = new Shader("shaders/2DVert.glsl", "shaders/2DFrag.glsl");
}

Frame::Frame() : Node(true) {
    float vertices[] = {
	 0.5f,  0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f 
    };

    unsigned int indices[] = {
	0, 1, 3,   
	1, 2, 3   
    }; 

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    setParent(Window::parent);
}

void Frame::update() {}

void Frame::draw() {
    glUseProgram(shader->program);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, getWorldPosition());
    model = glm::scale(model, size);

    shader->setMatrix("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}
