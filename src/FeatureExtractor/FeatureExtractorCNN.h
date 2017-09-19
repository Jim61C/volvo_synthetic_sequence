#ifndef FEATUREEXTRACTORCNN_H
#define FEATUREEXTRACTORCNN_H

#include "FeatureExtractorBase.h"
#include <caffe/caffe.hpp>
#include <boost/shared_ptr.hpp>
#include "Constants.h"

class FeatureExtractorCNN : public FeatureExtractorBase {

public:
    /*
    Constructor
    */
    FeatureExtractorCNN(const string& deploy_proto,
                        const string& caffe_model,
                        const int gpu_id,
                        const int num_inputs,
                        const bool do_train = false);

    /*
    Implement interface
    */
    void ExtractFeatureBBoxes(vector<BoundingBox> & bboxes, Mat & image, vector<vector<Mat> > *features);

    /*
    Init network
    */
    void Init();

    /*
    Set the image mean for network
    */
    void SetMean();
    
    /*
    reset weights of network
    */
    void Reset();

    /*
    initialise assign memory for net_ and load weights from pretrained model
    */
    void SetUpNetwork(const string &deploy_proto, const string &caffe_model, const int gpu_id, 
                      const int num_inputs, const bool do_train);

    /*
    Find the corresponding index of target layer in the network, if not found return -1
    */
    int FindLayerIndexByName( const vector<string> & layer_names, const string & target);

    /*
    Wrap the memory correspond to input layer indexed by input_idx in the given vector<Mat>
    */
    void WrapInputLayerGivenIndex(std::vector<cv::Mat>* channels, int input_idx);

    /*
    Preprocess on img to fit the shape of input blob
    Put the actual content of img into input_channels using split
    \input: keep_original_size, if true, then do not reshape img into the blob width and height recoreded by input_geometry_
    */
    void Preprocess(const cv::Mat& img, std::vector<cv::Mat>* input_channels, bool keep_original_size);

    /*
    Set the rois with correct scale into the network rois input blob [batch_id, x1, y1, x2, y2]
    */
    void SetRois(const std::vector<BoundingBox>& candidate_bboxes, const double scale, const int batch_id = 0);

    /*
    Wrap the batch number of feature blob into vector<vector<Mat> >
    */
    void WrapOutputBlob(const std::string &blob_name, std::vector<std::vector<cv::Mat> > *output_channels);



protected:
    boost::shared_ptr<caffe::Net<float> > net_;

    int num_inputs_;

    string deploy_proto_; 

    string caffe_model_;

    bool modified_params_;

    // Size of the input images (Width and Height)
    cv::Size input_geometry_;

    // Number of image channels: normally either 1 (black and white) or 3 (color).
    int num_channels_;

    cv::Mat mean_;
};

#endif