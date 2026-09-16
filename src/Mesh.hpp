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

class Mesh : Node {
public:
    static void loadModel(std::string path);
    static void init();

    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 size = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 linearVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
protected:
    void draw();
private:
    Mesh(
	std::vector<Vertex> vertices,
	std::vector<unsigned int> indices,
	std::vector<Texture> textures
    );

    static Shader* shader;

    static void processNode(aiNode* node, const aiScene* scene, std::string dir);
    static void processMesh(aiMesh* mesh, const aiScene* scene, std::string dir);
    static std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, std::string dir);

    unsigned int VAO, VBO, EBO;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
};
