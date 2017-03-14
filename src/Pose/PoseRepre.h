#ifndef POSEREPRE_H
#define POSEREPRE_H
#include "KinematicTreeNode.h"
#include <utility>
#include <unordered_map>
#include "Joint.h"
#include "PoseEncoding.h"
#include "../Rendering/CommonCV.h"
#include "../ParticleFilter/BoundingBox.h"
#include <gsl/gsl_rng.h> 

#define PI 3.14159265
#define PERTURBATION_RANGE PI/2.0
#define POSE_BOX_PADDING 10

class PoseRepre {
public:
    KinematicTreeNode* root_ptr; // KinematicTree root node, neck, allocated on heap
    Joint neck_pos_2d; // here use neck as root, record its 2D position
    // double scale; // no scale in the KinematicTree representation
    vector<Joint> joints; // original joints parsed in;

    PoseRepre();
    ~PoseRepre();

    /**
    get the feature representation of this tree, here, I use a pre-order walk from the root
    */
    vector<double> getThisKinematicFeatureRepre();

    /**
    static Helper functions
    */
    // parse the KinematicTree Feature given a node pointer
    static vector<double> getKinematicFeatureRepreGivenNode(KinematicTreeNode * node);

    // deform the pose given a node
    static void deformPoseRandom(KinematicTreeNode * node);

    // construct kinematic tree represetation given a vector of joints, returns the root
    static KinematicTreeNode* constructKinematicTreeMannual(vector<Joint> & joints);

    // more clever way of constructing the tree
    static KinematicTreeNode* constructKinematicTreeAuto(vector<Joint> & joints);

    // get bbox of the detected pose
    static BoundingBox getBoundingBoxFromPose(vector <Joint> & joints, Mat & frame);

    // convert back from tree to vector<Joint>
    static vector<Joint> recoverJointsFromKinematicTree(KinematicTreeNode * root, Joint root_pos);

    // get the detected pose from a given image area, embed the python code here
    static vector<Joint> detectPoseGivenFrame(Mat & frame);

    // load from file a vector of joints, 14 by 2
    static vector<Joint> loadOnePoseFromFile(string path);

    // print the joints
    static void printJoints(vector<Joint> & joints);

    // visualise poses
    static void drawPoseOnFrame(Mat & frame, vector<Joint> & joints);
};

#endif