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
