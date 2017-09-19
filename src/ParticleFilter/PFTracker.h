#ifndef PFTRACKER_H
#define PFTRACKER_H
#include "../Rendering/DataManager.h"
#include "ParticleFilter.h"

#define WAIT_KEY_NUM_MILLISECONDS 1

class PFTracker {
public:
    ParticleFilter pf;
    BoundingBox initial_box;

    void start(DataManager & dm, VideoWriter & writer);

};

#endif