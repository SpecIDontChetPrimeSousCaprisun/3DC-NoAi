#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>

#include "Node.hpp"
#include "Vertex.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

class Mesh : public Node {
public:
    static void loadModel(std::string path);
    static void init();

    Material material;

    glm::vec3 getBounds();
protected:
    void draw();
private:
    Mesh(
	std::vector<Vertex> vertices,
	std::vector<unsigned int> indices
    );

    static Shader* shader;

    static void processNode(aiNode* node, const aiScene* scene);
    static void processMesh(aiMesh* mesh);

    void sendMatrix();

    unsigned int VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};
