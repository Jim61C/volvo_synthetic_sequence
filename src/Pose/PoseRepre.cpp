#include "PoseRepre.h"
#include <math.h>
#include <unordered_map>
#include <assert.h>  
#include <fstream>
#include <sstream>

// constructor
PoseRepre::PoseRepre() {
    this->root_ptr = NULL;
}

PoseRepre::~PoseRepre() {
    if (this->root_ptr != NULL) {
        delete this->root_ptr; // free the heap allocated memory
    }
}

bool validJointDetected(Joint & j) {
    if (j.x == -1 || j.y == -1 || j.z == -1 || j.name.compare("None") == 0) {
        return false;
    }

    return true;
}
double getRelativeAngleBetweenJoints(Joint & parent, Joint & child) {
    // 2D relative angle
    double dx = child.x - parent.x;
    double dy = child.y - parent.y;

    return atan2 (dy,dx);
}

double getDistanceBetweenJoints(Joint & parent, Joint & child) {
    assert (parent.is_2d == child.is_2d);
    double d;
    if (parent.is_2d && child.is_2d) {
        d = sqrt(pow(parent.x - child.x, 2) + pow(parent.y - child.y, 2));
    }
    else {
        d = sqrt(pow(parent.x - child.x, 2) + pow(parent.y - child.y, 2) + pow(parent.z - child.z, 2));
    }

    return d;
}

void inOrderWalk(KinematicTreeNode * node_ptr, vector<double> & results) {
    // push this node's r
    results.push_back(node_ptr-> r);

    // recurse, if no children , will just stop here
    for (int i = 0;i< node_ptr->children.size(); i ++) {
        inOrderWalk(node_ptr->children[i], results);
    }
}

vector<double> PoseRepre::getThisKinematicFeatureRepre() {
    // do an inorder walk
    vector<double> results;
    inOrderWalk(this->root_ptr, results);
    return results;
}

vector<double> PoseRepre::getKinematicFeatureRepreGivenNode(KinematicTreeNode * node) {
    vector<double> results;
    inOrderWalk(node, results);
    return results;
}

void PoseRepre::deformPoseRandom(KinematicTreeNode * node) {
    // TODO: deform randomly along the kinematic tree
}

KinematicTreeNode* PoseRepre::constructKinematicTreeMannual(vector<Joint> joints) {
    Joint& neck = joints[PoseEncoding::joint_name_to_idx["neck"]];
    KinematicTreeNode * root_node = new KinematicTreeNode(0, 0, "neck");

    double r;
    double d;
    // neck's children: [head, Lsho, Lhip, Rhip, Rsho]
    r = getRelativeAngleBetweenJoints(neck, joints[PoseEncoding::joint_name_to_idx["head"]]);
    d = getDistanceBetweenJoints(neck, joints[PoseEncoding::joint_name_to_idx["head"]]);
    KinematicTreeNode *head_node = new KinematicTreeNode(r, d, "head");
    root_node->children.push_back(head_node);

    Joint & Lsho = joints[PoseEncoding::joint_name_to_idx["Lsho"]];
    Joint & Lhip = joints[PoseEncoding::joint_name_to_idx["Lhip"]];
    Joint & Rhip = joints[PoseEncoding::joint_name_to_idx["Rhip"]];
    Joint & Rsho = joints[PoseEncoding::joint_name_to_idx["Rsho"]];

    r = getRelativeAngleBetweenJoints(neck, Lsho);
    d = getDistanceBetweenJoints(neck, Lsho);
    KinematicTreeNode *Lsho_node = new KinematicTreeNode(r, d, "Lsho");
    root_node->children.push_back(Lsho_node);

    r = getRelativeAngleBetweenJoints(neck, Lhip);
    d = getDistanceBetweenJoints(neck, Lhip);
    KinematicTreeNode *Lhip_node = new KinematicTreeNode(r, d, "Lhip");
    root_node->children.push_back(Lhip_node);

    r = getRelativeAngleBetweenJoints(neck, Rhip);
    d = getDistanceBetweenJoints(neck, Rhip);
    KinematicTreeNode *Rhip_node = new KinematicTreeNode(r, d, "Rhip");
    root_node->children.push_back(Rhip_node);

    r = getRelativeAngleBetweenJoints(neck, Rsho);
    d = getDistanceBetweenJoints(neck, Rsho);
    KinematicTreeNode *Rsho_node= new KinematicTreeNode(r, d, "Rsho");
    root_node->children.push_back(Rsho_node);
        

    // Lsho's children: [Lelb]
    Joint & Lelb = joints[PoseEncoding::joint_name_to_idx["Lelb"]];
    r = getRelativeAngleBetweenJoints(Lsho, Lelb);
    d = getDistanceBetweenJoints(Lsho, Lelb);
    KinematicTreeNode *Lelb_node = new KinematicTreeNode(r, d, "Lelb");
    Lsho_node->children.push_back(Lelb_node); 

    // Lelb's children: [Lwri]
    r = getRelativeAngleBetweenJoints(Lelb, joints[PoseEncoding::joint_name_to_idx["Lwri"]]);
    d = getDistanceBetweenJoints(Lelb, joints[PoseEncoding::joint_name_to_idx["Lwri"]]);
    KinematicTreeNode *Lwri_node = new KinematicTreeNode(r, d, "Lwri");
    Lelb_node->children.push_back(Lwri_node);



    // Rsho's children: [Relb]
    Joint & Relb = joints[PoseEncoding::joint_name_to_idx["Relb"]];
    r = getRelativeAngleBetweenJoints(Rsho, Relb);
    d = getDistanceBetweenJoints(Rsho, Relb);
    KinematicTreeNode *Relb_node = new KinematicTreeNode(r, d, "Relb");
    Rsho_node->children.push_back(Relb_node); 

    // Relb's children: [Rwri]
    r = getRelativeAngleBetweenJoints(Relb, joints[PoseEncoding::joint_name_to_idx["Rwri"]]);
    d = getDistanceBetweenJoints(Relb, joints[PoseEncoding::joint_name_to_idx["Rwri"]]);
    KinematicTreeNode *Rwri_node = new KinematicTreeNode(r, d, "Rwri");
    Relb_node->children.push_back(Rwri_node);


    // Lhip's children: [Lkne]
    Joint & Lkne = joints[PoseEncoding::joint_name_to_idx["Lkne"]];
    r = getRelativeAngleBetweenJoints(Lhip, Lkne);
    d = getDistanceBetweenJoints(Lhip, Lkne);
    KinematicTreeNode *Lkne_node = new KinematicTreeNode(r, d, "Lkne");
    Lhip_node->children.push_back(Lkne_node); 

    // Lkne's children: [Lank]
    r = getRelativeAngleBetweenJoints(Lkne, joints[PoseEncoding::joint_name_to_idx["Lank"]]);
    d = getDistanceBetweenJoints(Lkne, joints[PoseEncoding::joint_name_to_idx["Lank"]]);
    KinematicTreeNode *Lank_node = new KinematicTreeNode(r, d, "Lank");
    Lkne_node->children.push_back(Lank_node);


    // Rhip's children: [Rkne]
    Joint & Rkne = joints[PoseEncoding::joint_name_to_idx["Rkne"]];
    r = getRelativeAngleBetweenJoints(Rhip, Rkne);
    d = getDistanceBetweenJoints(Rhip, Rkne);
    KinematicTreeNode *Rkne_node = new KinematicTreeNode(r, d, "Rkne");
    Rhip_node->children.push_back(Rkne_node); 

    // Rkne's children: [Rank]
    r = getRelativeAngleBetweenJoints(Rkne, joints[PoseEncoding::joint_name_to_idx["Rank"]]);
    d = getDistanceBetweenJoints(Rkne, joints[PoseEncoding::joint_name_to_idx["Rank"]]);
    KinematicTreeNode *Rank_node = new KinematicTreeNode(r, d, "Rank");
    Rkne_node->children.push_back(Rank_node);

    return (root_node);
}

KinematicTreeNode* PoseRepre::constructKinematicTreeAuto(vector<Joint> joints) {
    unordered_map<string, KinematicTreeNode*> joint_name_to_node;
    KinematicTreeNode *root_node = new KinematicTreeNode(0, 0, "neck");
    joint_name_to_node.insert({"neck", root_node});

    for (int i =0; i< PoseEncoding::joint_correspondences.size(); i++) {
        std::pair<string, vector<string> > & this_corre = PoseEncoding::joint_correspondences[i];
        Joint& this_parent = joints[PoseEncoding::joint_name_to_idx[this_corre.first]];
        for (int j = 0; j < this_corre.second.size(); j++) {
            // add all the children to this node 
            Joint & this_child = joints[PoseEncoding::joint_name_to_idx[this_corre.second[j]]];

            // check that this_child is valid (actually detected, not -1, -1)
            if (validJointDetected(this_child)) {
                double r = getRelativeAngleBetweenJoints(this_parent, this_child);
                double d = getDistanceBetweenJoints(this_parent, this_child);

                // parent node should already be in the map, if not, means parent is not detected, skip
                if (joint_name_to_node.find(this_parent.name) != joint_name_to_node.end()) {
                    KinematicTreeNode* parent_ptr = joint_name_to_node[this_parent.name];
                
                    // add child node to the map, if not already in
                    if (joint_name_to_node.find(this_child.name) == joint_name_to_node.end()) {
                        KinematicTreeNode * child_node = new KinematicTreeNode(r, d, this_child.name);
                        parent_ptr->children.push_back(child_node);

                        joint_name_to_node.insert({this_child.name, child_node});
                    }
                    else {
                        KinematicTreeNode * child_ptr = joint_name_to_node[this_child.name];
                        parent_ptr->children.push_back(child_ptr);
                    }
                }
            }
        }
    }

    return (root_node);
}

Joint getChildJoint(Joint parent_joint, double r, double d, string child_joint_name) {
    double dx = d * cos(r);
    double dy = d * sin(r);

    double child_x = parent_joint.x + dx;
    double child_y = parent_joint.y + dy;

    Joint child_joint(child_x, child_y, child_joint_name);

    return child_joint;
}

// helper function to recover joints position given tree recursively
void recoverJointsRecursive(KinematicTreeNode * node, Joint node_pos, unordered_map<string, Joint> & results) {
    // pre order walk
    string this_node_name = node->name;
    results.insert({this_node_name, node_pos});

    // recursive on children
    for (int i = 0; i< node->children.size();i++) {
        Joint this_child_pos = getChildJoint(node_pos, node->children[i]->r, node->children[i]->d, node->children[i]->name);
        recoverJointsRecursive(node->children[i], this_child_pos, results);
    }
}

vector<Joint> PoseRepre::recoverJointsFromKinematicTree(KinematicTreeNode * root, Joint root_pos) {
    unordered_map<string, Joint> results;
    recoverJointsRecursive(root, root_pos, results);

    // return joints vector in the desired sequence
    vector <Joint> recovered_joints;
    recovered_joints.reserve(14);
    for (int i =0; i< PoseEncoding::joint_names.size();i++) {
        string this_joint_name = PoseEncoding::joint_names[i];
        if (results.find(this_joint_name) != results.end()){
            recovered_joints.push_back(results[this_joint_name]);
        }
        else {
            Joint dummy_joint;
            recovered_joints.push_back(dummy_joint);
        }
    }

    return recovered_joints;
}

vector<Joint> PoseRepre::detectPoseGivenFrame(Mat & frame) {
    // do the python embedding here
    vector<Joint> joints;
    return joints;
}

vector<Joint> PoseRepre::loadOnePoseFromFile(string path) {
    vector<Joint> parsed_joints; // 14 by 2
    ifstream pose_in_stream(path.c_str());
    string next_line;
    double x, y;

    int i = 0;
    while (!pose_in_stream.eof()) {
        getline(pose_in_stream, next_line);
        if (next_line.length() != 0) {
            istringstream is_with_space(next_line);
            is_with_space >> x;
            is_with_space >> y;
            Joint this_joint(x, y, PoseEncoding::joint_names[i]);
            parsed_joints.push_back(this_joint);
            i ++;
        }
    }

    for(int i = 0;i< parsed_joints.size();i++) {
        cout << parsed_joints[i].name << ": "<< parsed_joints[i].x << ", " << parsed_joints[i].y << ", " 
        << parsed_joints[i].is_2d<< endl;
    }
    return parsed_joints;
}

