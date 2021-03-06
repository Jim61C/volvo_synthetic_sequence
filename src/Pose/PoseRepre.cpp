#include "PoseRepre.h"
#include <math.h>
#include <limits>
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
    if (j.is_2d) {
        if (j.x < 0 || j.y < 0 || j.name.compare("None") == 0) {
            return false;
        }
    }
    else {
        // check all three coordinate
        if (j.x < 0 || j.y < 0 || j.z < 0 || j.name.compare("None") == 0) {
            return false;
        }
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

    // make sure 14 dimension
    while (results.size() < PoseEncoding::joint_names.size()) {
        results.push_back(0);
    }
    return results;
}

void PoseRepre::deformPoseRandom(KinematicTreeNode * node) {
    const gsl_rng_type * T;
    gsl_rng * r;
    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc (T);
    gsl_rng_set(r, time(NULL));

    // deform randomly along the kinematic tree, do not perturb root node
    if (node->name.compare("neck") != 0) {
        double deform_r = (gsl_rng_uniform (r) - 0.5) * PERTURBATION_RANGE; // -PI/4 to +Pi/4
        node->r += deform_r;
    }

    // recurse on children
    for (int i = 0;i< node->children.size(); i++) {
        deformPoseRandom(node->children[i]);
    }
}

KinematicTreeNode* PoseRepre::constructKinematicTreeMannual(vector<Joint> & joints) {
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

KinematicTreeNode* PoseRepre::constructKinematicTreeAuto(vector<Joint> & joints) {
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

void constuctKinematicTreeFeaturesRecursive(KinematicTreeNode * node, vector<double> & feature, int &pos) {
    if (pos == feature.size()) {
        return;
    }

    string this_node_name = node->name;

    // i is the index of the corre 
    int i = 0;
    for (i =0; i< PoseEncoding::joint_correspondences.size(); i++) {
        std::pair<string, vector<string> > & this_corre = PoseEncoding::joint_correspondences[i];
        if (this_corre.first.compare(this_node_name) == 0 ) {
            break;
        }
    }
    // if i has children
    if (i !=  PoseEncoding::joint_correspondences.size()) {
        vector<string> &children_names = PoseEncoding::joint_correspondences[i].second;
        for (int j = 0; j< children_names.size();j++) {
            KinematicTreeNode *this_child = new KinematicTreeNode(feature[pos], 50, children_names[j]);
            pos ++; // move to next
            node->children.push_back(this_child);

            constuctKinematicTreeFeaturesRecursive(this_child, feature, pos);
        }
    }
}

KinematicTreeNode* PoseRepre::constructKinematicTreeFromFeatures(vector<double> & feature) {
    KinematicTreeNode *root_node = new KinematicTreeNode(0, 0, "neck");
    int pos = 1;
    constuctKinematicTreeFeaturesRecursive(root_node, feature, pos);
    return root_node;
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

BoundingBox PoseRepre::getBoundingBoxFromPose(vector <Joint> & joints, Mat & frame) {
    int W = frame.size().width;
    int H = frame.size().height;
    
    BoundingBox bbox;
    double min_x = std::numeric_limits<double>::max();
    double max_x = -1;
    double min_y = std::numeric_limits<double>::max();
    double max_y = -1;
    for (int i = 0;i< joints.size();i++) {
        Joint & this_joint = joints[i];
        if (this_joint.x < min_x) {
            min_x = this_joint.x;
        }
        if (this_joint.x > max_x) {
            max_x = this_joint.x;
        }
        if (this_joint.y < min_y) {
            min_y = this_joint.y;
        }
        if (this_joint.y > max_y) {
            max_y = this_joint.y;
        }
    }

    if (max_x != -1) {
        // at least one joint is valid

        // add padding
        min_x -= POSE_BOX_PADDING;
        max_x += POSE_BOX_PADDING;
        min_y -= POSE_BOX_PADDING;
        max_y += POSE_BOX_PADDING;

        // check against boundaries
        if (min_x < 0) {
            min_x = 0;
        }
        if (min_y < 0) {
            min_y = 0;
        }
        if (max_x > W - 1 ) {
            max_x = W - 1;
        }
        if (max_y > H - 1) {
            max_y = H - 1;
        }

        bbox.x = min_x;
        bbox.y = min_y;
        bbox.w = max_x - min_x;
        bbox.h = max_y - min_y;
    }

    return bbox;
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

    return parsed_joints;
}

vector<vector<Joint> > PoseRepre::loadPosesFromFile(string path) {
    vector<vector<Joint> > multi_joints;
    int num_person = -1;
    ifstream pose_in_stream(path.c_str());
    string next_line;
    double x, y;

    int i = 0;
    while(!pose_in_stream.eof()) {
        getline(pose_in_stream, next_line);
        if (next_line.length() != 0) {
            // parse number of people if not yet parsed
            if (num_person == -1) {
                int counter = 0;
                istringstream is_with_space(next_line);
                while(! is_with_space.eof()) {
                    is_with_space >> x;
                    is_with_space >> y;
                    counter ++;
                }

                num_person = counter;
                // initialise multi_joints with correct number of people
                for (int j =0;j< num_person;j++) {
                    multi_joints.push_back(vector<Joint>());
                }
            }

            // start the actual assignment procedure
            istringstream is_multi_person(next_line); // assume space delimited
            for (int j =0;j< num_person;j++) {
                is_multi_person >> x;
                is_multi_person >> y;
                multi_joints[j].push_back(Joint(x, y, PoseEncoding::joint_names[i]));
            }

            i++; // increment to parse next joint
        }
    }

    // make sure that each vector has exactly 14 joint returned
    for (int j = 0;j< multi_joints.size();j++) {
        vector<Joint> & this_joints_loaded = multi_joints[j];
        while (this_joints_loaded.size() < PoseEncoding::joint_names.size()) {
            this_joints_loaded.push_back(Joint()); // push back dummy joints
        }
    }

    return multi_joints;
}

vector<vector<double> > PoseRepre::loadFeaturesFromFile(string path) {
    vector<vector<double> > features;
    double r;

    ifstream feature_in_stream(path.c_str());
    string next_line;
    while(!feature_in_stream.eof()) {
        getline(feature_in_stream, next_line);
        if (next_line.length() != 0) {
            vector<double> this_feature;

            istringstream is_one_feature(next_line);
            while (!is_one_feature.eof()) {
                is_one_feature >> r;
                this_feature.push_back(r);
            }

            features.push_back(this_feature);
        }
    }

    return features;

}

bool jointInsideBoundingBox(Joint p, BoundingBox b) {
    if (p.x >= b.x && 
    p.x <= b.x + b.w && 
    p.y >= b.y &&
    p.y <= b.y + b.h) {
        return true;
    }

    return false;
}

double distanceFromJointToBoxCenter(Joint p, BoundingBox b) {
    double x_centre = b.x + b.w/2.0;
    double y_centre = b.y + b.h/2.0;

    double d = sqrt(pow(p.x - x_centre, 2) + pow(p.y - y_centre, 2));
    return d;
}

Joint getPersonJointCentre2D(vector<Joint> & joints) {
    double x_sum = 0;
    double y_sum = 0;
    int count;

    for (int i =0;i<joints.size();i++) {
        Joint this_joint = joints[i];
        if (validJointDetected(this_joint)) {
            x_sum += this_joint.x;
            y_sum += this_joint.y;
            count ++;
        }
    }

    Joint centre(x_sum/count, y_sum/count, "None"); // not a valid joint, just a position, here declare as joint to reuse the model for convenience
    return centre;
}

// finds the person with largest number of poses inside b, and construct vector<Joint> from there.
vector<Joint> PoseRepre::findPoseInBoundingBox(vector<vector<Joint> > & multi_joints, BoundingBox b) {
    vector<int> most_inclusive_idxes;
    int max_n_joints_inside = -1;
    for (int i = 0;i< multi_joints.size(); i ++) {
        vector<Joint> & this_person_joints = multi_joints[i];
        int count = 0;
        for (int j = 0;j<this_person_joints.size();j++) {
            if (validJointDetected(this_person_joints[j]) && jointInsideBoundingBox(this_person_joints[j], b)) {
                count ++;
            }
        }
        if (count > max_n_joints_inside) {
            max_n_joints_inside = count;
            most_inclusive_idxes.clear();
            most_inclusive_idxes.push_back(i);
        }
        else if (count == max_n_joints_inside) {
            most_inclusive_idxes.push_back(i);
        }
    }

    if (max_n_joints_inside == 0) {
        // none of the person pose are inside the given b
        vector<Joint> dummy_joints;
        for (int i =0;i< PoseEncoding::joint_names.size();i++) {
            dummy_joints.push_back(Joint());
        }

        return dummy_joints;
    }

    // out of most_inclusive_idxes, choose the closest index
    double min_distance = std::numeric_limits<double>::max();
    int best_idx = -1;
    
    for (int i = 0;i< most_inclusive_idxes.size(); i++) {
        int this_idx = most_inclusive_idxes[i];
        Joint this_person_centre = getPersonJointCentre2D(multi_joints[this_idx]);
        double this_d = distanceFromJointToBoxCenter(this_person_centre, b);
        if (this_d < min_distance) {
            min_distance = this_d;
            best_idx = this_idx;
        }
    }

    assert(best_idx != -1);
    
    // return the joints from best_idx that is actually inside bbox, (-1,-1) if that joint is not inside
    vector<Joint> results;
    vector<Joint> & best_joints = multi_joints[best_idx];
    for (int i = 0;i< best_joints.size();i++) {
        if (validJointDetected(best_joints[i]) && jointInsideBoundingBox(best_joints[i], b)) {
            results.push_back(best_joints[i]);
        }
        else {
            results.push_back(Joint()); // dummy joints, indicating this joint not inside the box
        }
    }

    return results;
}

void PoseRepre::printJoints(vector<Joint> & joints) {
    for(int i = 0;i< joints.size();i++) {
        cout << i << ": " << joints[i].name << ": "<< joints[i].x << ", " << joints[i].y << ", " 
        << joints[i].is_2d<< endl;
    }
}

void PoseRepre::drawPoseOnFrame(Mat & frame, vector<Joint> & joints) {

    // convert to 3 channle if BW
    if (frame.channels() == 1) {
        cvtColor(frame,frame,CV_GRAY2BGR);
    }

    for(int i = 0; i< PoseEncoding::limb_colors.size();i++) {
        Mat this_frame = frame.clone();

        pair<string, string> this_limb = PoseEncoding::limb_colors[i].first;
        cv::Scalar this_color = PoseEncoding::limb_colors[i].second;
        Joint p1 = joints[PoseEncoding::joint_name_to_idx[this_limb.first]];
        Joint p2 = joints[PoseEncoding::joint_name_to_idx[this_limb.second]];

        // make sure both joints are valid, then draw this limb
        if (validJointDetected(p1) && validJointDetected(p2)) {
            Point mid_point((p1.x + p2.x)/2.0, (p1.y + p2.y)/2.0);
            double d = getDistanceBetweenJoints(p1, p2);
            double angle = getRelativeAngleBetweenJoints(p1, p2) * 180.0 / PI;
            cv::Size axes(d/2.0 , 4);

            cv::ellipse(this_frame, mid_point, axes, angle, 0.0, 360.0, this_color, -1);

            // make the limb ellipse translucent
            addWeighted(frame, 0.4, this_frame, 0.6, 0, frame);
        }
    }
}

