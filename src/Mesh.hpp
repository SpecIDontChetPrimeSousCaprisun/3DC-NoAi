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
    Mesh(std::string path);
    virtual ~Mesh();

    static std::vector<Mesh*> loadModel(std::string path);
    static std::vector<Mesh*> getMeshesInBounds(glm::vec3 position, glm::vec3 bounds);
    static void init();

    bool canCollide = false;
    bool anchored = true;
    bool castShadows = true;
    Material material;
    float transparency = 0.0f;

    BoundResult getBounds();
    bool intersects(Mesh *other);
protected:
    virtual void beforeUpdate();

    void update() override;
    void draw() override;
private:
    Mesh(
	std::vector<Vertex> vertices,
	std::vector<unsigned int> indices
    );

    static Shader* shader;
    static Shader* depthShader;
    static std::vector<Mesh*> meshes;

    static std::vector<Mesh*> processNode(aiNode* node, const aiScene* scene);
    static Mesh* processMesh(aiMesh* mesh);

    void init(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    void sendMatrix();
    void resolveCollision(Mesh *other);
    aiMesh* getFirstMesh(const aiScene* scene, aiNode* node);

    unsigned int VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};
