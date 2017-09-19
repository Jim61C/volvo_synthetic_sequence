#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "../Rendering/CommonCV.h"

// VGG M training image mean, BGR
const cv::Scalar mean_scalar_VGG(102.71699095,  115.77261497,  123.5093643);

// INPUT INDEX
#define INPUT_IMAGE_INDEX 0
#define INPUT_ROIS_INDEX 1

// ROI Pooling
const double TARGET_SIZE = 600.0; // compare to min (W, H)
const double MAX_SIZE = 1000.0; // make sure the image_curr does not exceed this size

#endif