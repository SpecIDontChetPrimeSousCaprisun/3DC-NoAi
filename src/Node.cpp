#include "Node.hpp"

Node::Node() : parent(nullptr) {}
Node::Node(Node* parent) {
    setParent(parent);
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
    for (Node* child : children) {
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
