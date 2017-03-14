#include "PoseEncoding.h"

// factory methods
unordered_map<string, int> makeJointNameToIdx() {
    unordered_map<string, int> joint_name_to_idx;
    joint_name_to_idx.insert( { "head", 0});
    joint_name_to_idx.insert( { "neck", 1});
    joint_name_to_idx.insert( { "Rsho", 2});
    joint_name_to_idx.insert( { "Relb", 3});
    joint_name_to_idx.insert( { "Rwri", 4});
    joint_name_to_idx.insert( { "Lsho", 5});
    joint_name_to_idx.insert( { "Lelb", 6});
    joint_name_to_idx.insert( { "Lwri", 7});
    joint_name_to_idx.insert( { "Rhip", 8});
    joint_name_to_idx.insert( { "Rkne", 9});
    joint_name_to_idx.insert( { "Rank", 10});
    joint_name_to_idx.insert( { "Lhip", 11});
    joint_name_to_idx.insert( { "Lkne", 12});
    joint_name_to_idx.insert( { "Lank", 13});

    return joint_name_to_idx;
}

vector<string> makeJointNames() {
    vector<string> joint_names;
    joint_names.push_back("head");
    joint_names.push_back("neck");
    joint_names.push_back("Rsho");
    joint_names.push_back("Relb");
    joint_names.push_back("Rwri");
    joint_names.push_back("Lsho");
    joint_names.push_back("Lelb");
    joint_names.push_back("Lwri");
    joint_names.push_back("Rhip");
    joint_names.push_back("Rkne");
    joint_names.push_back("Rank");
    joint_names.push_back("Lhip");
    joint_names.push_back("Lkne");
    joint_names.push_back("Lank");

    return joint_names;
}

vector<pair<string, vector<string> > > makeJointCorrespondences() {
    vector<pair<string, vector<string> > > joint_correspondences;
    std::vector<string> neck_children;
    neck_children.reserve(5);
    neck_children.push_back("head");
    neck_children.push_back("Lsho");
    neck_children.push_back("Lhip");
    neck_children.push_back("Rhip");
    neck_children.push_back("Rsho");
    joint_correspondences.push_back(std::make_pair(std::string("neck"), neck_children));

    std::vector<string> Lsho_children;
    Lsho_children.push_back("Lelb");
    joint_correspondences.push_back(std::make_pair(std::string("Lsho"), Lsho_children));

    std::vector<string> Lelb_children;
    Lelb_children.push_back("Lwri");
    joint_correspondences.push_back(std::make_pair(std::string("Lelb"), Lelb_children));

    std::vector<string> Rsho_children;
    Rsho_children.push_back("Relb");
    joint_correspondences.push_back(std::make_pair(std::string("Rsho"), Rsho_children));

    std::vector<string> Relb_children;
    Relb_children.push_back("Rwri");
    joint_correspondences.push_back(std::make_pair(std::string("Relb"), Relb_children));


    std::vector<string> Lhip_children;
    Lhip_children.push_back("Lkne");
    joint_correspondences.push_back(std::make_pair(std::string("Lhip"), Lhip_children));

    std::vector<string> Lkne_children;
    Lkne_children.push_back("Lank");
    joint_correspondences.push_back(std::make_pair(std::string("Lkne"), Lkne_children));

    std::vector<string> Rhip_children;
    Rhip_children.push_back("Rkne");
    joint_correspondences.push_back(std::make_pair(std::string("Rhip"), Rhip_children));

    std::vector<string> Rkne_children;
    Rkne_children.push_back("Rank");
    joint_correspondences.push_back(std::make_pair(std::string("Rkne"), Rkne_children));
    
    return joint_correspondences;
}

vector<pair<pair<string, string>, cv::Scalar> > makeLimbColors() {
    vector<pair<pair<string, string>, cv::Scalar> > limb_colors;
    
    limb_colors.push_back(std::make_pair( std::make_pair("neck" , "Rsho"), cv::Scalar(255, 0, 0)));
    limb_colors.push_back(std::make_pair( std::make_pair("neck" , "Lsho"), cv::Scalar(255, 85, 0)));
    limb_colors.push_back(std::make_pair( std::make_pair("Rsho" , "Relb"), cv::Scalar(255, 170, 0)));
    limb_colors.push_back(std::make_pair( std::make_pair("Relb" , "Rwri"), cv::Scalar(255, 255, 0)));
    limb_colors.push_back(std::make_pair( std::make_pair("Lsho" , "Lelb"), cv::Scalar(170, 255, 0)));
    limb_colors.push_back(std::make_pair( std::make_pair("Lelb" , "Lwri"), cv::Scalar(85, 255, 0)));
    limb_colors.push_back(std::make_pair( std::make_pair("neck" , "Rhip"), cv::Scalar(0, 255, 0)));
    limb_colors.push_back(std::make_pair( std::make_pair("Rhip" , "Rkne"), cv::Scalar(0, 255, 85)));
    limb_colors.push_back(std::make_pair( std::make_pair("Rkne" , "Rank"), cv::Scalar(0, 255, 170)));
    limb_colors.push_back(std::make_pair( std::make_pair("neck" , "Lhip"), cv::Scalar(0, 255, 255)));
    limb_colors.push_back(std::make_pair( std::make_pair("Lhip" , "Lkne"), cv::Scalar(0, 170, 255)));
    limb_colors.push_back(std::make_pair( std::make_pair("Lkne" , "Lank"), cv::Scalar(0, 85, 255)));
    limb_colors.push_back(std::make_pair( std::make_pair("neck" , "head"), cv::Scalar(0, 0, 255)));

    return limb_colors;
}


unordered_map<string, int> PoseEncoding::joint_name_to_idx = makeJointNameToIdx();
vector<string> PoseEncoding::joint_names = makeJointNames();
vector<pair<string, vector<string> > > PoseEncoding::joint_correspondences = makeJointCorrespondences();
vector<pair<pair<string, string>, cv::Scalar> > PoseEncoding::limb_colors = makeLimbColors();


// PoseEncoding::PoseEncoding() {
    // this->joint_name_to_idx.insert( { "head", 0});
    // this->joint_name_to_idx.insert( { "neck", 1});
    // this->joint_name_to_idx.insert( { "Rsho", 2});
    // this->joint_name_to_idx.insert( { "Relb", 3});
    // this->joint_name_to_idx.insert( { "Rwri", 4});
    // this->joint_name_to_idx.insert( { "Lsho", 5});
    // this->joint_name_to_idx.insert( { "Lelb", 6});
    // this->joint_name_to_idx.insert( { "Lwri", 7});
    // this->joint_name_to_idx.insert( { "Rhip", 8});
    // this->joint_name_to_idx.insert( { "Rkne", 9});
    // this->joint_name_to_idx.insert( { "Rank", 10});
    // this->joint_name_to_idx.insert( { "Lhip", 11});
    // this->joint_name_to_idx.insert( { "Lkne", 12});
    // this->joint_name_to_idx.insert( { "Lank", 13});


    // this->joint_names.push_back("head");
    // this->joint_names.push_back("neck");
    // this->joint_names.push_back("Rsho");
    // this->joint_names.push_back("Relb");
    // this->joint_names.push_back("Rwri");
    // this->joint_names.push_back("Lsho");
    // this->joint_names.push_back("Lelb");
    // this->joint_names.push_back("Lwri");
    // this->joint_names.push_back("Rhip");
    // this->joint_names.push_back("Rkne");
    // this->joint_names.push_back("Rank");
    // this->joint_names.push_back("Lhip");
    // this->joint_names.push_back("Lkne");
    // this->joint_names.push_back("Lank");

    // vector<string> neck_children;
    // neck_children.reserve(5);
    // neck_children.push_back("head");
    // neck_children.push_back("Lsho");
    // neck_children.push_back("Lhip");
    // neck_children.push_back("Rhip");
    // neck_children.push_back("Rsho");
    // this->joint_correspondences.push_back(std::make_pair(std::string("neck"), neck_children));

    // vector<string> Lsho_children;
    // Lsho_children.push_back("Lelb");
    // this->joint_correspondences.push_back(std::make_pair(std::string("Lsho"), Lsho_children));

    // vector<string> Lelb_children;
    // Lelb_children.push_back("Lwri");
    // this->joint_correspondences.push_back(std::make_pair(std::string("Lelb"), Lelb_children));

    // vector<string> Rsho_children;
    // Rsho_children.push_back("Relb");
    // this->joint_correspondences.push_back(std::make_pair(std::string("Rsho"), Rsho_children));

    // vector<string> Relb_children;
    // Relb_children.push_back("Rwri");
    // this->joint_correspondences.push_back(std::make_pair(std::string("Relb"), Relb_children));


    // vector<string> Lhip_children;
    // Lhip_children.push_back("Lkne");
    // this->joint_correspondences.push_back(std::make_pair(std::string("Lhip"), Lhip_children));

    // vector<string> Lkne_children;
    // Lkne_children.push_back("Lank");
    // this->joint_correspondences.push_back(std::make_pair(std::string("Lkne"), Lkne_children));

    // vector<string> Rhip_children;
    // Rhip_children.push_back("Rkne");
    // this->joint_correspondences.push_back(std::make_pair(std::string("Rhip"), Rhip_children));

    // vector<string> Rkne_children;
    // Rkne_children.push_back("Rank");
    // this->joint_correspondences.push_back(std::make_pair(std::string("Rkne"), Rkne_children));
// }