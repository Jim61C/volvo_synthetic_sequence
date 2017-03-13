#ifndef POSEREPRE_H
#define POSEREPRE_H
#include "KinematicTreeNode.h"
#include <utility>
#include "Joint.h"
#include "PoseEncoding.h"

class PoseRepre {
public:
    KinematicTreeNode* root_ptr; // KinematicTree root node, neck
    Joint neck_pos_2d; // here use neck as root, record its 2D position
    // double scale; // no scale in the KinematicTree representation
    vector<Joint> joints; // original joints parsed in

    /**
    Get the feature representation of this tree, here, I use a pre-order walk from the root
    */
    vector<double> getThisKinematicFeatureRepre();

    static vector<double> getKinematicFeatureRepreGivenNode(KinematicTreeNode * node);

    static void deformPoseRandom(KinematicTreeNode * node);

    /**
    Construct kinematic tree represetation given a vector of joints, returns the root
    */
    static KinematicTreeNode* constructKinematicTreeMannual(vector<Joint> joints);

    static KinematicTreeNode* constructKinematicTreeAuto(vector<Joint> joints);
};

#endif