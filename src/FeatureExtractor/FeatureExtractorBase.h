#ifndef FEATUREEXTRACTORBASE_H
#define FEATUREEXTRACTORBASE_H
#include "../ParticleFilter/BoundingBox.h"
#include "../Rendering/CommonCV.h"
#include "../Rendering/Common.h"

class FeatureExtractorBase {

public:
    FeatureExtractorBase();
    /*
    Given bboxes in [x y w h] and entire image
    return: vector of N-dimensional features (each in form of vector<Mat>)
    */
    virtual void ExtractFeatureBBoxes(vector<BoundingBox> & bboxes, Mat & image, vector<vector<Mat> > *features) = 0;

    /*
    Called at the beginning of tracking a new object to initialize the Feature extractor
    */
    virtual void Init() { }

};

#endif