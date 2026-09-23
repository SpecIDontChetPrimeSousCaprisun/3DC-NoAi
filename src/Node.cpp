#include "Node.hpp"
#include "Window.hpp"

Node::Node() : parent(nullptr) {}
Node::Node(Node* parent) {
    setParent(parent);
}

Node::~Node() {
    unparent();
}

Node* Node::getParent() {
    return parent;
}

std::vector<Node*> Node::getChildren() {
    return children;
}

void Node::setParent(Node* parent) {
    unparent();
    this->parent = parent;
    parent->children.push_back(this);
}

void Node::unparent() {
    if (!this->parent) return;
    for (auto it = parent->children.begin(); it != this->parent->children.end(); ) {
	if (*it == this) {
	    it = parent->children.erase(it);
	    break;
	} else ++it;
    }

    parent = nullptr;
}

void Node::updateChildren() {
    updateWorldCoordinates();

    for (Node* child : children) {
	child->preUpdate();
	child->update();
    }

    for (Node* child : children) {
	child->updateChildren();
    }
}

void Node::drawChildren() {
    for (Node* child : children) {
	child->draw();
    }

    for (Node* child : children) {
	child->drawChildren();
    }
}

void Node::update() {}
void Node::draw() {}

glm::vec3 Node::getWorldPosition() {
    return worldPosition;
}

glm::vec3 Node::getWorldRotation() {
    return worldRotation;
}

void Node::preUpdate() {
    applyVelocities();
    updateWorldCoordinates();
}

void Node::applyVelocities() {
    position += linearVelocity * (float)Window::dt;
    rotation += angularVelocity * (float)Window::dt;
}

void Node::updateWorldCoordinates() {
    if (!parent) { 
	worldPosition = position;
	worldRotation = rotation;

	return;
    }

    worldPosition = position + parent->getWorldPosition();
    worldRotation = rotation + parent->getWorldRotation();
}

void Node::setWorldPosition(glm::vec3 pos) {
    glm::vec3 offset = pos - parent->getWorldPosition();

    position += offset;
}
