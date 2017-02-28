#include "PoseRepre.h"
#include <math.h>
#include <unordered_map>

double getRelativeAngleBetweenJoints(Joint & parent, Joint & child) {
    // 2D relative angle
    double dx = child.x - parent.x;
    double dy = child.y - parent.y;

    return atan2 (dy,dx);
}

void inOrderWalk(KinematicTreeNode * node_ptr, vector<double> & results) {
    // push this node's r
    results.push_back(node_ptr-> r);

    // recurse, if no children , will just stop here
    for (int i = 0;i< node_ptr->children.size(); i ++) {
        inOrderWalk(node_ptr->children[i], results);
    }
}

vector<double> PoseRepre::getKinematicFeatureRepre() {
    // do an inorder walk
    vector<double> results;
    inOrderWalk(this->root_ptr, results);
    return results;
}

KinematicTreeNode* PoseRepre::constructKinematicTreeMannual(vector<Joint> joints) {
    Joint& neck = joints[PoseEncoding::joint_name_to_idx["neck"]];
    KinematicTreeNode * root_node = new KinematicTreeNode(0, "neck");

    double r;
    // neck's children: [head, Lsho, Lhip, Rhip, Rsho]
    r = getRelativeAngleBetweenJoints(neck, joints[PoseEncoding::joint_name_to_idx["head"]]);
    KinematicTreeNode head_node(r, "head");
    root_node->children.push_back(&head_node);

    Joint & Lsho = joints[PoseEncoding::joint_name_to_idx["Lsho"]];
    Joint & Lhip = joints[PoseEncoding::joint_name_to_idx["Lhip"]];
    Joint & Rhip = joints[PoseEncoding::joint_name_to_idx["Rhip"]];
    Joint & Rsho = joints[PoseEncoding::joint_name_to_idx["Rsho"]];

    r = getRelativeAngleBetweenJoints(neck, Lsho);
    KinematicTreeNode Lsho_node(r, "Lsho");
    root_node->children.push_back(&Lsho_node);

    r = getRelativeAngleBetweenJoints(neck, Lhip);
    KinematicTreeNode Lhip_node(r, "Lhip");
    root_node->children.push_back(&Lhip_node);

    r = getRelativeAngleBetweenJoints(neck, Rhip);
    KinematicTreeNode Rhip_node(r, "Rhip");
    root_node->children.push_back(&Rhip_node);

    r = getRelativeAngleBetweenJoints(neck, Rsho);
    KinematicTreeNode Rsho_node(r, "Rsho");
    root_node->children.push_back(&Rsho_node);
        

    // Lsho's children: [Lelb]
    Joint & Lelb = joints[PoseEncoding::joint_name_to_idx["Lelb"]];
    r = getRelativeAngleBetweenJoints(Lsho, Lelb);
    KinematicTreeNode Lelb_node(r, "Lelb");
    Lsho_node.children.push_back(&Lelb_node); 

    // Lelb's children: [Lwri]
    r = getRelativeAngleBetweenJoints(Lelb, joints[PoseEncoding::joint_name_to_idx["Lwri"]]);
    KinematicTreeNode Lwri_node(r, "Lwri");
    Lelb_node.children.push_back(&Lwri_node);



    // Rsho's children: [Relb]
    Joint & Relb = joints[PoseEncoding::joint_name_to_idx["Relb"]];
    r = getRelativeAngleBetweenJoints(Rsho, Relb);
    KinematicTreeNode Relb_node(r, "Relb");
    Rsho_node.children.push_back(&Relb_node); 

    // Relb's children: [Rwri]
    r = getRelativeAngleBetweenJoints(Relb, joints[PoseEncoding::joint_name_to_idx["Rwri"]]);
    KinematicTreeNode Rwri_node(r, "Rwri");
    Relb_node.children.push_back(&Rwri_node);


    // Lhip's children: [Lkne]
    Joint & Lkne = joints[PoseEncoding::joint_name_to_idx["Lkne"]];
    r = getRelativeAngleBetweenJoints(Lhip, Lkne);
    KinematicTreeNode Lkne_node(r, "Lkne");

    Lhip_node.children.push_back(&Lkne_node); 

    // Lkne's children: [Lank]
    r = getRelativeAngleBetweenJoints(Lkne, joints[PoseEncoding::joint_name_to_idx["Lank"]]);
    KinematicTreeNode Lank_node(r, "Lank");
    Lkne_node.children.push_back(&Lank_node);


    // Rhip's children: [Rkne]
    Joint & Rkne = joints[PoseEncoding::joint_name_to_idx["Rkne"]];
    r = getRelativeAngleBetweenJoints(Rhip, Rkne);
    KinematicTreeNode Rkne_node(r, "Rkne");
    Rhip_node.children.push_back(&Rkne_node); 

    // Rkne's children: [Rank]
    r = getRelativeAngleBetweenJoints(Rkne, joints[PoseEncoding::joint_name_to_idx["Rank"]]);
    KinematicTreeNode Rank_node(r, "Rank");
    Rkne_node.children.push_back(&Rank_node);

    return (root_node);
}

KinematicTreeNode* PoseRepre::constructKinematicTreeAuto(vector<Joint> joints) {
    unordered_map<string, KinematicTreeNode*> joint_name_to_node;
    KinematicTreeNode *root_node = new KinematicTreeNode(0, "neck");
    joint_name_to_node.insert({"neck", root_node});

    for (int i =0; i< PoseEncoding::joint_correspondences.size(); i++) {
        std::pair<string, vector<string> > & this_corre = PoseEncoding::joint_correspondences[i];
        Joint& this_parent = joints[PoseEncoding::joint_name_to_idx[this_corre.first]];
        for (int j = 0; j < this_corre.second.size(); j++) {
            // add all the children to this node 
            Joint & this_child = joints[PoseEncoding::joint_name_to_idx[this_corre.second[j]]];
            double r = getRelativeAngleBetweenJoints(this_parent, this_child);

            // parent node should already be in the map
            KinematicTreeNode* parent_ptr = joint_name_to_node[this_parent.name];
            
            // add child node to the map, if not already in
            if (joint_name_to_node.find(this_child.name) == joint_name_to_node.end()) {
                KinematicTreeNode child_node(r, this_child.name);
                parent_ptr->children.push_back(&child_node);

                joint_name_to_node.insert({this_child.name, &child_node});
            }
            else {
                KinematicTreeNode * child_ptr = joint_name_to_node[this_child.name];
                parent_ptr->children.push_back(child_ptr);
            }
        }
    }

    return (root_node);
}