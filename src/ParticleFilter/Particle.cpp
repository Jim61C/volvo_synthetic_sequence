#include "Particle.h"

// #define DEBUG_RESCALE

Particle::Particle() {
    this->w = 1.0;
    this->s = 1.0;

    // dummy speeds
    this->u = 1.0;
    this->v = 1.0;
}

void Particle::rescale() {
    // s is updated, now rescale self and normalise s = 1
#ifdef DEBUG_RESCALE
    if (this->s != 1.0) {
        cout << "about to rescale" << endl;
    }
#endif
    BoundingBox new_roi;
    this->roi.calBoundingBoxNewScale (this->s, new_roi);
    this->roi.setAsBox(new_roi);
    this->s = 1.0; // after rescale, s fall back to 1.0
}