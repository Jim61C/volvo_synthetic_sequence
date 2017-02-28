#include "PoseEncoding.h"

unordered_map<string, int> PoseEncoding::joint_name_to_idx;
vector<string> PoseEncoding::joint_names;
vector<pair<string, vector<string> > > PoseEncoding::joint_correspondences;