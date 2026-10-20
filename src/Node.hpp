#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Node {
public:
    Node(bool is2D = false);
    Node(Node* parent, bool is2D = false);
    virtual ~Node();

    Node* getParent();
    std::vector<Node*> getChildren();
    void setParent(Node* parent);
    void unparent();
    void updateChildren();
    void drawChildren();
    void drawUIChildren();
    void setWorldPosition(glm::vec3 pos);
    glm::vec3 getWorldPosition();
    glm::vec3 getWorldRotation();

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 linearVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 angularVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
protected:
    void applyVelocities();
    void updateWorldCoordinates();

    bool is2D = false;
    Node* parent;
    virtual void update();
    virtual void draw();
private:
    void preUpdate();

    std::vector<Node*> children;
    glm::vec3 worldPosition;
    glm::vec3 worldRotation;
};
