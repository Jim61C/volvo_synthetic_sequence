#ifndef KINEMATICTREENODE_H
#define KINEMATICTREENODE_H
#include "../Rendering/Common.h"

class KinematicTreeNode {
public:
    double r; // relative angle with parent, 0 for root
    string name;
    vector<KinematicTreeNode *> children;

    KinematicTreeNode(double r);
    KinematicTreeNode(double r, string name);
};

#endif