#include "KinematicTreeNode.h"
KinematicTreeNode::~KinematicTreeNode() {
    // free its children
    for (int i =0;i<this->children.size(); i++) {
        delete this->children[i];
    }
}
KinematicTreeNode::KinematicTreeNode() {
    this->r = 0;
    this->d = 0;
    this->name = "None";
}

void recursiveCopy(KinematicTreeNode *node, KinematicTreeNode *from_node) {
    node->r = from_node->r;
    node->d = from_node->d;
    node->name = from_node->name;

    for (int i = 0;i< from_node->children.size(); i++) {
        KinematicTreeNode *new_child = new KinematicTreeNode();
        node->children.push_back(new_child);
        recursiveCopy(new_child, from_node->children[i]);
    }
}

KinematicTreeNode::KinematicTreeNode(KinematicTreeNode * node) {
    recursiveCopy(this, node);
}

KinematicTreeNode::KinematicTreeNode(double r, double d) {
    this->r = r;
    this->d = d;
    this->name = "None";
}

KinematicTreeNode::KinematicTreeNode(double r, double d, string name) {
    this->r = r;
    this->d = d;
    this->name = name;
}
