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

class BoundResult {
public:
    glm::vec3 min;
    glm::vec3 max;
    glm::vec3 size;
};

class Mesh : public Node {
public:
    static std::vector<Mesh*> loadModel(std::string path);
    static void init();

    bool canCollide = false;
    bool anchored = true;
    Material material;

    BoundResult getBounds();
    bool intersects(Mesh other);
protected:
    void update();
    void draw();
private:
    Mesh(
	std::vector<Vertex> vertices,
	std::vector<unsigned int> indices
    );

    static Shader* shader;
    static std::vector<Mesh*> meshes;

    static std::vector<Mesh*> processNode(aiNode* node, const aiScene* scene);
    static Mesh* processMesh(aiMesh* mesh);

    void sendMatrix();
    void resolveCollision(Mesh other);

    unsigned int VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};
