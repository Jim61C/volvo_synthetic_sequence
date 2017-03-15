#ifndef PARTICLEFIILTER_H
#define PARTICLEFIILTER_H
#include "Particle.h"
#include "BoundingBox.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <gsl/gsl_rng.h> 
#include <gsl/gsl_randist.h> /* GAUSSIAN*/

#define NUM_PARTICLES 100
#define TRANS_X_STD 0.5
#define TRANS_Y_STD 1.0
#define TRANS_S_STD 0.001
#define HIST_DIST_LIKELI_STD 2/6.0

#define INITIAL_SAMPLE_RANGE 10.0
#define DELTA_T 1.0
#define GAUSSIAN_MULTIPLIER 5.0

#define SCALE_VARIANCE 1.05
#define SCALE_LEVEL 2 // -2, -1, 0, 1, 2

#define TEMPLATE_UPDATE_SIMILARITY_TH 0.0 // 0.0 never update template histogram

class ParticleFilter {
public:
    int N_particles;
    vector<Particle> particles;
    Particle current_roi;
    Particle template_roi;
    gsl_rng* rng;

    // constructor
    ParticleFilter();

    // create initial particles, in a random manner, prior
    void initParticles(Mat &frame, BoundingBox & initial_box);

    // motion model, make sure not out of image boundary
    void moveParticle(Particle & p, int W, int H);
    void transition(int W, int H);

    // observation model
    double updateLikelihood(Particle & p, Particle & template_roi, Mat & frame);
    // make sure that weights of particles sum to 1
    void normalizeLikelihood();
    // invoke observation model for all particles
    void updateWeights(Mat & frame);

    // posterior, get weighted average of all particles
    void updateCurrentROI(Mat & frame);

    // resample the particles
    void resampleParticles();

    // visualise
    void DrawParticles(Mat & frame);

    // rescale it so that s =1 given a particle
    void rescale(Particle & p);


    /**
    Helper functions
    */
    // feature computation
    static MatND computeColorHistogram(BoundingBox & b, Mat & frame);
    // likelihood computation
    static double computeLikelihood(MatND & this_hist, MatND & tempalte_hist);

};

#endif