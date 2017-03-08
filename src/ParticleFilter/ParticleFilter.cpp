#include "ParticleFilter.h"
#include <math.h> 

#define DEBUG_BOUNDINGBOX_BOUNDARY

ParticleFilter::ParticleFilter() {
    this->N_particles = 100;
    gsl_rng_env_setup();
    this->rng = gsl_rng_alloc(gsl_rng_mt19937);
    gsl_rng_set(this->rng, time(NULL));
}

// create initial particles, in a random manner, prior
void ParticleFilter::initParticles(Mat &frame, BoundingBox & initial_box) {
    while (this->particles.size() < this->N_particles) {
        // create particle
        double dx_unit = (gsl_rng_uniform (this->rng) - 0.5);
        double dy_unit = (gsl_rng_uniform (this->rng) - 0.5);
        
        cout << "Particle ["<< this->particles.size() << "]" << "initial rand:" << dx_unit << ", " << dy_unit << endl;

        double dx = dx_unit * INITIAL_SAMPLE_RANGE;
        double dy = dy_unit * INITIAL_SAMPLE_RANGE;

        Particle new_p;
        new_p.roi.setBoxCoordinate(initial_box.x + dx, initial_box.y + dy, initial_box.w, initial_box.h);
        new_p.u = dx/1.0; // assume constant speed model
        new_p.v = dy/1.0;
        new_p.w = 1.0/this->N_particles;
        new_p.s = 1.0;

        this->particles.push_back(new_p);
    }
}

// motion model, make sure not out of image boundary
void ParticleFilter::moveParticle(Particle & p, int W, int H) {
    // constant speed model, here scale is not touched, scale is varied in obsevation model to find the best scale
    // cout <<"Gaussian change in new x:" << GAUSSIAN_MULTIPLIER * gsl_ran_gaussian(rng, TRANS_X_STD) << endl;
    double new_x = p.roi.x + p.u * DELTA_T + GAUSSIAN_MULTIPLIER * gsl_ran_gaussian(rng, TRANS_X_STD);
    double new_y = p.roi.y + p.v * DELTA_T + GAUSSIAN_MULTIPLIER * gsl_ran_gaussian(rng, TRANS_Y_STD);

    // make sure not out of boundary
    if (new_x < 0) {
        new_x = 0.0;
    }
    else if (new_x + p.roi.w >= W) {
        new_x = W - p.roi.w -1.0;
    }
    
    if (new_y < 0) {
        new_y = 0;
    }
    else if (new_y + p.roi.h >= H) {
        new_y = H - p.roi.h - 1.0;
    }

    // update state
    p.u = new_x - p.roi.x;
    p.v = new_y - p.roi.y;

    p.roi.setBoxCoordinate(new_x, new_y, p.roi.w, p.roi.h);

}
void ParticleFilter::transition(int W, int H) {
    for (int i = 0;i< this->particles.size(); i++) {
        moveParticle(particles[i], W, H); 
    }
}

// observation model, update w, also update scale s here (also check W and H), echo back likelihood
// note  template_roi.color_feature should be precalculated!!!
double ParticleFilter::updateLikelihood(Particle & p, Particle & template_roi, Mat & frame) {
    double best_likeli = 0.0;
    double best_s = 1.0;
    MatND best_hist;
    
    // MatND template_hist = ParticleFilter::computeColorHistogram(template_roi.roi, frame);
    // template_roi.color_feature = template_hist;
    
    // TODO: make sure that template_roi is precalculated
    MatND template_hist = template_roi.color_feature;

    double centre_x = p.roi.x + p.roi.w / 2.0;
    double centre_y = p.roi.y + p.roi.h / 2.0;
    
    for (int i = -SCALE_LEVEL;i<= SCALE_LEVEL;i++) {
        double this_s = pow(SCALE_VARIANCE, i) * 1.0;
        cout << "this_s:" << endl;

        // cout << "centre_x:" << centre_x << ", centre_y" << centre_y << endl;

        // make sure not out of boudary
        if ((int)(centre_x + 0.5 * p.roi.w * this_s) < frame.size().width &&
        (int)(centre_x - 0.5 * p.roi.w * this_s) >= 0 && 
        (int)(centre_y + 0.5 * p.roi.h * this_s) < frame.size().height &&
        (int)(centre_y - 0.5 * p.roi.h * this_s) >= 0 
        ) {
#ifdef DEBUG_BOUNDINGBOX_BOUNDARY
            cout << "boundaries:" << (int)(centre_x + 0.5 * p.roi.w * this_s) << ", "
            << (int)(centre_x - 0.5 * p.roi.w * this_s) << ", "
            <<  (int)(centre_y + 0.5 * p.roi.h * this_s) << ", "
            <<  (int)(centre_y - 0.5 * p.roi.h * this_s) << endl;
            BoundingBox this_box;
#endif
            p.roi.calBoundingBoxNewScale (this_s, this_box);

#ifdef DEBUG_BOUNDINGBOX_BOUNDARY
            cout << "this_box: " << this_box.x << ", " << this_box.y << ", " << this_box.w << ", " << this_box.h << endl;
#endif
            MatND this_hist = ParticleFilter::computeColorHistogram(this_box, frame);
            double this_likeli = ParticleFilter::computeLikelihood(this_hist, template_hist);
            // TODO: penalise large BoundingBox Difference
            if (this_likeli > best_likeli) {
                best_likeli = this_likeli;
                best_s = this_s;
                best_hist = this_hist;
            }
        }
    }

    cout << "best_likeli:" << best_likeli << endl;
    p.w = best_likeli;
    p.s = best_s;
    p.color_feature = best_hist;

    return p.w;
}

void ParticleFilter::rescale(Particle & p) {
    // s is updated, now rescale p and normalise s = 1
    BoundingBox new_roi;
    p.roi.calBoundingBoxNewScale (p.s, new_roi);
    p.roi.setAsBox(new_roi);
    p.s = 1.0; // after rescale, s fall back to 1.0
}

// make sure that weights of particles sum to 1
void ParticleFilter::normalizeLikelihood() {
    double sum = 0;
    for (int i =0;i<this->particles.size();i++) {
        sum += this->particles[i].w;
    }

    if (sum == 0) {
        // all particles are having weights zero, all of them are going out of boundary, failure case
        cout << "Failure: All particles are going out of boundary!" << endl;
        for (int i = 0;i < this->particles.size();i++) {
            this->particles[i].w = 1/this->N_particles;
        }
    }
    else {
        for (int i = 0;i < this->particles.size();i++) {
            this->particles[i].w /= sum;
        }
    }
}
// invoke observation model for all particles
void ParticleFilter::updateWeights(Mat & frame) {
    for (int i =0;i<this->particles.size();i++) {
        updateLikelihood(this->particles[i], this->template_roi, frame); // w, s get's updated here, best w, best s
    }

    normalizeLikelihood();

    for (int i = 0;i<this->particles.size();i++) {
        this->particles[i].rescale(); // update particle scale
    }
}

// posterior, get weighted average of all particles, simple model always update, expect drift
void ParticleFilter::updateCurrentROI(Mat & frame) {
    // MMSE
    double x_bar = 0;
    double y_bar = 0;
    double w_bar = 0; // captures the scale s 
    double h_bar = 0;
    double u_bar = 0;
    double v_bar = 0;

    for (int i =0;i< this->particles.size();i++) {
        Particle & this_p = this->particles[i];
        x_bar += this_p.roi.x * this_p.w;
        y_bar += this_p.roi.y * this_p.w;
        w_bar += this_p.roi.w * this_p.w;
        h_bar += this_p.roi.h * this_p.w;
        u_bar += this_p.u * this_p.w;
        v_bar += this_p.v * this_p.w;
        cout << "particle[" << i << "].w: " << this_p.w << endl; 
    }

    this->current_roi.u = u_bar;
    this->current_roi.v = v_bar;

    this->current_roi.roi.setBoxCoordinate((int)(x_bar), (int)(y_bar), (int)(w_bar), (int)(h_bar));
    this->current_roi.s = 1.0; // since s already falled back for all particles
    
    // // naive, always update template_roi, 
    // this->template_roi.u = u_bar;
    // this->template_roi.v = v_bar;

    // this->template_roi.roi.setBoxCoordinate((int)(x_bar), (int)(y_bar), (int)(w_bar), (int)(h_bar));
    // this->template_roi.s = 1.0; // since s already falled back for all particles

    // TODO: only update the template if likelihood is high (or if appearance change much)
    MatND estimate_color_feature = ParticleFilter::computeColorHistogram(this->current_roi.roi, frame);
    this->current_roi.color_feature = estimate_color_feature;
    double similarity = ParticleFilter::computeLikelihood(estimate_color_feature, this->template_roi.color_feature);
    if (similarity < TEMPLATE_UPDATE_SIMILARITY_TH) {
        // if there is much appearance change, update template
        cout << "need to update template!!!" << endl;
        this->template_roi.u = u_bar;
        this->template_roi.v = v_bar;

        this->template_roi.roi.setBoxCoordinate((int)(x_bar), (int)(y_bar), (int)(w_bar), (int)(h_bar));
        this->template_roi.s = 1.0; // since s already falled back for all particles
        this->template_roi.color_feature = estimate_color_feature;
    }

    // // MAP
    // int MAP_idx = -1;
    // int max_w = -1;

    // for (int i =0;i<this->particles.size();i++) {
    //     if (this->particles[i].w > max_w) {
    //         max_w = this->particles[i].w;
    //         MAP_idx = i;
    //     }
    // }

    // Particle & p_MAP = this->particles[MAP_idx];
    // this->current_roi.u = p_MAP.u;
    // this->current_roi.v = p_MAP.v;

    // this->current_roi.roi.setBoxCoordinate(p_MAP.roi.x, p_MAP.roi.y, p_MAP.roi.w, p_MAP.roi.h);
    // this->current_roi.s = 1.0; // since s already falled back for all particles
    // this->current_roi.color_feature = p_MAP.color_feature;

    // // TODO: only update the template if likelihood is high (or if appearance change much)
    // MatND & estimate_color_feature = p_MAP.color_feature;
    // double similarity = ParticleFilter::computeLikelihood(estimate_color_feature, this->template_roi.color_feature);
    // if (similarity < TEMPLATE_UPDATE_SIMILARITY_TH) {
    //     // if there is much appearance change, update template
    //     this->template_roi.u = p_MAP.u;
    //     this->template_roi.v = p_MAP.v;

    //     this->template_roi.roi.setBoxCoordinate(p_MAP.roi.x, p_MAP.roi.y, p_MAP.roi.w, p_MAP.roi.h);
    //     this->template_roi.s = 1.0; // since s already falled back for all particles
    //     this->template_roi.color_feature = p_MAP.color_feature;
    // }

}

bool particleComparison(Particle & a, Particle & b) {
    return a.w > b.w;
}

// resample the particles
void ParticleFilter::resampleParticles() {
    vector <Particle> new_particles;
    // sort the current particles by their weights
    std::sort(this->particles.begin(), this->particles.end(), particleComparison);
    
    cout << "best particle weight:" << this->particles[0].w << endl;
    cout << "2nd best particle weight:" << this->particles[1].w << endl;

    for (int i =0;i< this->particles.size() && new_particles.size() < this->N_particles;i++) {
        int n_from_this = floor(this->particles[i].w * this->particles.size());
        for (int j = 0;j< n_from_this && new_particles.size() < this->N_particles;j++) {
            new_particles.push_back(this->particles[i]); // creat n_from_this copies of this particle
        }
    }

    cout << "after initial resampling, new_particles.size():" << new_particles.size() << endl;
    
    // if not enough, copy the particle having heighest weight (TODO: might need to change this)
    while (new_particles.size() < this->N_particles) {
        // new_particles.push_back(this->particles[0]);
        new_particles.push_back(this->current_roi); // push the MMSE estimate
    }

    // reassign back
    this->particles.clear();
    for (int i = 0; i< new_particles.size(); i++) {
        this->particles.push_back(new_particles[i]);
    }
}


void ParticleFilter::DrawParticles(Mat & frame) {
    for (int i =0;i<this->particles.size();i++) {
        this->particles[i].roi.Draw(255, 0, 0, frame); // particles shown in red 
    }
}

/**
Helper functions
*/
// feature computation
MatND ParticleFilter::computeColorHistogram(BoundingBox & b, Mat & frame) {
    // TODO: if frame is one channel (BW), then use intensity histogram instead
#ifdef DEBUG_BOUNDINGBOX_BOUNDARY
    cout << "roi: \n" << b.x << "," << b.y << ", " << b.w << ", " << b.h << endl;
    cout << "frame W and H:" << frame.size().width  << ", " << frame.size().height << endl;
#endif
    Rect roi = Rect(b.x, b.y, b.w, b.h);
	Mat frame_roi = frame(roi);
    Mat frame_hsv;
    cvtColor( frame_roi, frame_hsv, COLOR_BGR2HSV );

    // convert to HSV and calculate hist

    // Using 50 bins for hue and 60 for saturation
    int h_bins = 50; int s_bins = 60;
    int histSize[] = { h_bins, s_bins };

    // hue varies from 0 to 179, saturation from 0 to 255
    float h_ranges[] = { 0, 180 };
    float s_ranges[] = { 0, 256 };

    const float* ranges[] = { h_ranges, s_ranges };

    // Use the o-th and 1-st channels
    int channels[] = { 0, 1 };

    MatND hist;
    calcHist( &frame_hsv, 1, channels, Mat(), hist, 2, histSize, ranges, true, false );
    // normalize( hist, hist, 0, 1, NORM_MINMAX, -1, Mat() );
    return hist;
}

double ParticleFilter::computeLikelihood(MatND & this_hist, MatND & tempalte_hist) {
    double distance = compareHist( this_hist, tempalte_hist, CV_COMP_BHATTACHARYYA ); // CV_COMP_BHATTACHARYYA is actual distance, the lower better, between 0 and 1
    return 1.0 - distance;
}