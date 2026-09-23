#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Node {
public:
    Node();
    Node(Node* parent);
    virtual ~Node();

    Node* getParent();
    std::vector<Node*> getChildren();
    void setParent(Node* parent);
    void unparent();
    void updateChildren();
    void drawChildren();
    void setWorldPosition(glm::vec3 pos);
    glm::vec3 getWorldPosition();
    glm::vec3 getWorldRotation();

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 linearVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 angularVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
protected:
    virtual void update();
    virtual void draw();
private:
    void preUpdate();
    void applyVelocities();
    void updateWorldCoordinates();

    Node* parent;
    std::vector<Node*> children;
    glm::vec3 worldPosition;
    glm::vec3 worldRotation;
};
