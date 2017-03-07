#ifndef PFTRACKER_H
#define PFTRACKER_H
#include "../Rendering/DataManager.h"
#include "ParticleFilter.h"

class PFTracker {
public:
    ParticleFilter pf;
    BoundingBox initial_box;

    void start(DataManager & dm);

};

#endif