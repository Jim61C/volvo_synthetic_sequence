#ifndef KINEMATICTREENODE_H
#define KINEMATICTREENODE_H
#include "../Rendering/Common.h"

class KinematicTreeNode {
public:
    double r; // relative angle with parent, 0 for root
    double d; // distance relative to parent
    string name;
    vector<KinematicTreeNode *> children;

    KinematicTreeNode();
    ~KinematicTreeNode();
    KinematicTreeNode(KinematicTreeNode * node); // deep copy
    KinematicTreeNode(double r, double d);
    KinematicTreeNode(double r, double d, string name);
};

#endif