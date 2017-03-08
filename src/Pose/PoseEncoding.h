#ifndef POSEENCODING_H
#define POSEENCODING_H
#include <unordered_map>
#include <utility> 
#include "../Rendering/Common.h"

class PoseEncoding {

public:

    static unordered_map<string, int> joint_name_to_idx;
    static vector<string> joint_names;
    static vector<pair<string, vector<string> > > joint_correspondences;

    PoseEncoding() {
        this->joint_name_to_idx.insert( { "head", 0});
        this->joint_name_to_idx.insert( { "neck", 1});
        this->joint_name_to_idx.insert( { "Rsho", 2});
        this->joint_name_to_idx.insert( { "Relb", 3});
        this->joint_name_to_idx.insert( { "Rwri", 4});
        this->joint_name_to_idx.insert( { "Lsho", 5});
        this->joint_name_to_idx.insert( { "Lelb", 6});
        this->joint_name_to_idx.insert( { "Lwri", 7});
        this->joint_name_to_idx.insert( { "Rhip", 8});
        this->joint_name_to_idx.insert( { "Rkne", 9});
        this->joint_name_to_idx.insert( { "Rank", 10});
        this->joint_name_to_idx.insert( { "Lhip", 11});
        this->joint_name_to_idx.insert( { "Lkne", 12});
        this->joint_name_to_idx.insert( { "Lank", 13});


        this->joint_names.push_back("head");
        this->joint_names.push_back("neck");
        this->joint_names.push_back("Rsho");
        this->joint_names.push_back("Relb");
        this->joint_names.push_back("Rwri");
        this->joint_names.push_back("Lsho");
        this->joint_names.push_back("Lelb");
        this->joint_names.push_back("Lwri");
        this->joint_names.push_back("Rhip");
        this->joint_names.push_back("Rkne");
        this->joint_names.push_back("Rank");
        this->joint_names.push_back("Lhip");
        this->joint_names.push_back("Lkne");
        this->joint_names.push_back("Lank");

        vector<string> neck_children;
        neck_children.reserve(5);
        neck_children.push_back("head");
        neck_children.push_back("Lsho");
        neck_children.push_back("Lhip");
        neck_children.push_back("Rhip");
        neck_children.push_back("Rsho");
        this->joint_correspondences.push_back(std::make_pair(std::string("neck"), neck_children));

        vector<string> Lsho_children;
        Lsho_children.push_back("Lelb");
        this->joint_correspondences.push_back(std::make_pair(std::string("Lsho"), Lsho_children));

        vector<string> Lelb_children;
        Lelb_children.push_back("Lwri");
        this->joint_correspondences.push_back(std::make_pair(std::string("Lelb"), Lelb_children));

        vector<string> Rsho_children;
        Rsho_children.push_back("Relb");
        this->joint_correspondences.push_back(std::make_pair(std::string("Rsho"), Rsho_children));

        vector<string> Relb_children;
        Relb_children.push_back("Rwri");
        this->joint_correspondences.push_back(std::make_pair(std::string("Relb"), Relb_children));


        vector<string> Lhip_children;
        Lhip_children.push_back("Lkne");
        this->joint_correspondences.push_back(std::make_pair(std::string("Lhip"), Lhip_children));

        vector<string> Lkne_children;
        Lkne_children.push_back("Lank");
        this->joint_correspondences.push_back(std::make_pair(std::string("Lkne"), Lkne_children));

        vector<string> Rhip_children;
        Rhip_children.push_back("Rkne");
        this->joint_correspondences.push_back(std::make_pair(std::string("Rhip"), Rhip_children));

        vector<string> Rkne_children;
        Rkne_children.push_back("Rank");
        this->joint_correspondences.push_back(std::make_pair(std::string("Rkne"), Rkne_children));



    }
};


#endif