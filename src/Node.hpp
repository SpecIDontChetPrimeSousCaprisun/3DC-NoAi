#pragma once

#include <vector>

class Node {
public:
    Node();
    Node(Node* parent);

    Node* getParent();
    std::vector<Node*> getChildren();
    void setParent(Node* parent);
    void unparent();
    void updateChildren();
    void drawChildren();
protected:
    virtual void update();
    virtual void draw();
private:
    Node* parent;
    std::vector<Node*> children;
};
