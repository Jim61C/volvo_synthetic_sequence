#include "KinematicTreeNode.h"

KinematicTreeNode::KinematicTreeNode() {
    this->r = 0;
    this->name = "None";
}

KinematicTreeNode::KinematicTreeNode(double r) {
    this->r = r;
    this->name = "None";
}

KinematicTreeNode::KinematicTreeNode(double r, string name) {
    this->r = r;
    this->name = name;
}
