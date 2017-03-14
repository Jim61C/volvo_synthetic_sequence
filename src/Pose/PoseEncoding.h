#ifndef POSEENCODING_H
#define POSEENCODING_H
#include <unordered_map>
#include <utility> 
#include "../Rendering/Common.h"
#include "../Rendering/CommonCV.h"

class PoseEncoding {

public:

    static unordered_map<string, int> joint_name_to_idx;
    static vector<string> joint_names;
    static vector<pair<string, vector<string> > > joint_correspondences;
    static vector<pair<pair<string, string>, cv::Scalar> > limb_colors;
};


#endif