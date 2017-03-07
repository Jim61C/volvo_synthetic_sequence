#ifndef PARTICLE_H
#define PARTICLE_H
#include "BoundingBox.h"
#include "../Pose/KinematicTreeNode.h"
#include <vector>

struct Particle {
public:
    // state
    BoundingBox roi;
    double u; // horizontal speed
    double v; // vetical speed
    double s; // scale
    
    // weight
    double w;

    // features
    KinematicTreeNode pose_root; // root has r == 0
    int synthetic_character_id = -1; // link to unity character id 

    // color HSV histogram
    MatND color_feature;

    // pose feature
    vector<double> pose_feature;

    Particle();

    // rescale itself
    void rescale();

};


#endif