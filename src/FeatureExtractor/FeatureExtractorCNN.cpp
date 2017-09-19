#include "FeatureExtractorCNN.h"
#include <algorithm>

using caffe::Blob;
using caffe::Net;
using caffe::Layer;
using caffe::LayerParameter;
using caffe::ParamSpec;
using std::string;

FeatureExtractorCNN::FeatureExtractorCNN(const string& deploy_proto,
    const string& caffe_model,
    const int gpu_id,
    const int num_inputs,
    const bool do_train) : 
    deploy_proto_(deploy_proto),
    caffe_model_(caffe_model),
    num_inputs_(num_inputs),
    modified_params_(false)
    {
        SetUpNetwork(deploy_proto, caffe_model, gpu_id, num_inputs, do_train);
    }

void FeatureExtractorCNN::SetUpNetwork(const string &deploy_proto, const string &caffe_model, const int gpu_id, 
                                       const int num_inputs, const bool do_train) {
#ifdef CPU_ONLY
    printf("Setting up Caffe in CPU mode\n");
    caffe::Caffe::set_mode(caffe::Caffe::CPU);
#else
    printf("Setting up Caffe in GPU mode with ID: %d\n", gpu_id);
    caffe::Caffe::SetDevice(gpu_id);
    caffe::Caffe::set_mode(caffe::Caffe::GPU);
#endif
  
    if (do_train) {
      printf("Setting phase to train\n");
      net_.reset(new Net<float>(deploy_proto, caffe::TRAIN));
    } else {
      printf("Setting phase to test\n");
      net_.reset(new Net<float>(deploy_proto, caffe::TEST));
    }
  
    if (caffe_model != "NONE") {
      net_->CopyTrainedLayersFrom(caffe_model_);
    } else {
      printf("Not initializing network from pre-trained model\n");
    }
  
    Blob<float>* input_layer = net_->input_blobs()[INPUT_IMAGE_INDEX]; // input_blob[0] corresponds to the image, input_blob[1] corresponds to the rois
  
    printf("Network image size: %d, %d\n", input_layer->width(), input_layer->height());
  
    num_channels_ = input_layer->channels();
    CHECK(num_channels_ == 3 || num_channels_ == 1)
      << "Input layer should have 1 or 3 channels.";
    input_geometry_ = cv::Size(input_layer->width(), input_layer->height());
  
    // Load the binaryproto mean file.
    SetMean();
}

void FeatureExtractorCNN::ExtractFeatureBBoxes(vector<BoundingBox> & bboxes, Mat & image_curr, vector<vector<Mat> > *features) {
    const vector<string> & layer_names = net_->layer_names();
    //------------------------- Forward the ROIs Until -------------------

    // Process the candidate, full image's input, i.e., image_curr, just one! Also record the scales
    int im_min_size = std::min(image_curr.size().width, image_curr.size().height);
    int im_max_size = std::max(image_curr.size().width, image_curr.size().height);

    double scale_curr = TARGET_SIZE / im_min_size;
    if (round(scale_curr * im_max_size) > MAX_SIZE) {
    scale_curr = MAX_SIZE / im_max_size;
    }

    cv::Mat image_scaled;
    cv::resize(image_curr, image_scaled, cv::Size(), scale_curr, scale_curr);

#ifdef DEBUG_PRE_FORWARDFAST_IMAGE_SCALE 
    cout << "scale_curr:" << scale_curr << endl;
    cv::imshow("image_scaled:", image_scaled);
#endif

    // Reshape image input
    Blob<float>* input_image = net_->input_blobs()[INPUT_IMAGE_INDEX];
    input_image->Reshape(1, num_channels_,
    image_scaled.size().height, image_scaled.size().width);

    // Reshape the rois
    Blob<float> * input_rois_blob = net_->input_blobs()[INPUT_ROIS_INDEX];
    const size_t num_rois = bboxes.size();
    // reshape to (|R|, 5)
    vector<int> shape_rois;
    shape_rois.push_back(num_rois);
    shape_rois.push_back(5);
    input_rois_blob->Reshape(shape_rois);

    // Forward dimension change to all layers.
    net_->Reshape();

    // Put image_curr
    std::vector<cv::Mat> image_curr_channels;
    WrapInputLayerGivenIndex(&image_curr_channels, INPUT_IMAGE_INDEX);

    // Set the inputs to the network.
    Preprocess(image_scaled, &image_curr_channels, true); // set retain the original image size

    // Put the ROIs
    SetRois(bboxes, scale_curr);

    // ROI poolings
    int layer_conv1_idx = FindLayerIndexByName(layer_names, "conv1");
    int layer_roi_pool3_idx = FindLayerIndexByName(layer_names, "roi_pool3");
    net_->ForwardFromTo(layer_conv1_idx, layer_roi_pool3_idx);

    // wrap out the feature at roi_pool3
    WrapOutputBlob("roi_pool3", features);
}

int FeatureExtractorCNN::FindLayerIndexByName( const vector<string> & layer_names, const string & target) {
    for (int i = 0; i< layer_names.size(); i++) {
      if (layer_names[i].compare(target) == 0) {
        return i;
      }
    }
  
    return -1;
}

void FeatureExtractorCNN::WrapInputLayerGivenIndex(std::vector<cv::Mat>* channels, int input_idx) {
    Blob<float>* input_layer = net_->input_blobs()[input_idx];
  
    int width = input_layer->width();
    int height = input_layer->height();
    float* data = input_layer->mutable_cpu_data();
    for (int i = 0; i < input_layer->channels(); ++i) {
      cv::Mat channel(height, width, CV_32FC1, data);
      channels->push_back(channel);
      data += width * height;
    }
}

void FeatureExtractorCNN::Preprocess(const cv::Mat& img,
    std::vector<cv::Mat>* input_channels,
    bool keep_original_size) {
    // Convert the input image to the input image format of the network.
    cv::Mat sample;
    if (img.channels() == 3 && num_channels_ == 1)
    cv::cvtColor(img, sample, CV_BGR2GRAY);
    else if (img.channels() == 4 && num_channels_ == 1)
    cv::cvtColor(img, sample, CV_BGRA2GRAY);
    else if (img.channels() == 4 && num_channels_ == 3)
    cv::cvtColor(img, sample, CV_BGRA2BGR);
    else if (img.channels() == 1 && num_channels_ == 3)
    cv::cvtColor(img, sample, CV_GRAY2BGR);
    else
    sample = img;

    // Convert the input image to the expected size.
    cv::Mat sample_resized;
    if (!keep_original_size && sample.size() != input_geometry_)
    cv::resize(sample, sample_resized, input_geometry_);
    else
    sample_resized = sample;

    // Convert the input image to the expected number of channels.
    cv::Mat sample_float;
    if (num_channels_ == 3)
    sample_resized.convertTo(sample_float, CV_32FC3);
    else
    sample_resized.convertTo(sample_float, CV_32FC1);

    #ifdef DEBUG_PREPROCESS_SAMPLE
    std::cout << "after conversion to CV_32FC3: " << sample_float << endl;
    #endif
    // Subtract the image mean to try to make the input 0-mean.
    cv::Mat sample_normalized;
    cv::subtract(sample_float, cv::Mat(sample_float.size(), CV_32FC3, mean_scalar_VGG), sample_normalized);

    #ifdef DEBUG_PREPROCESS_SAMPLE
    std::cout << "after subtract mean: " << sample_normalized << endl;
    #endif
    // This operation will write the separate BGR planes directly to the
    // input layer of the network because it is wrapped by the cv::Mat
    // objects in input_channels.
    cv::split(sample_normalized, *input_channels);
}

void FeatureExtractorCNN::SetRois(const std::vector<BoundingBox>& candidate_bboxes, const double scale, const int batch_id) {
    
    // Reshape the bbox.
    Blob<float>* input_rois = net_->input_blobs()[INPUT_ROIS_INDEX];
    const size_t num_candidates = candidate_bboxes.size();
    vector<int> shape;
    shape.push_back(num_candidates);
    shape.push_back(5);
    input_rois->Reshape(shape);

    // Get a pointer to the bbox memory.
    float* input_rois_data = input_rois->mutable_cpu_data();

    int input_rois_data_counter = 0;
    for (size_t i = 0; i < candidate_bboxes.size(); ++i) {
        const BoundingBox& this_rois = candidate_bboxes[i];

        std::vector<float> bbox_vect;
        double x1, y1, x2, y2;
        this_rois.toTwoCornerCoordinates(x1, y1, x2, y2);
        bbox_vect.push_back(x1 * scale);
        bbox_vect.push_back(y1 * scale);
        bbox_vect.push_back(x2 * scale);
        bbox_vect.push_back(y2 * scale);

        input_rois_data[input_rois_data_counter] = batch_id; // put the batch id as first col
        input_rois_data_counter++;
        for (size_t j = 0; j < 4; ++j) {
            input_rois_data[input_rois_data_counter] = bbox_vect[j];
            input_rois_data_counter++;
        }
    }
}

void FeatureExtractorCNN::WrapOutputBlob(const std::string & blob_name, std::vector<std::vector<cv::Mat> > *output_channels) {
    const boost::shared_ptr<Blob<float> > layer = net_->blob_by_name(blob_name.c_str());
  
    output_channels->resize(layer->shape(0)); // make sure output_channels has number of vectors that is the same as batch size
  
    int out_width = layer->width();
    int out_height = layer->height();
    float* out_data = layer->mutable_cpu_data();
    for (int n = 0; n < layer->shape(0); ++n) {
        for (int i = 0; i < layer->channels(); ++i) {
          cv::Mat channel(out_height, out_width, CV_32FC1, out_data);
          (*output_channels)[n].push_back(channel.clone());
          out_data += out_width * out_height;
        }
      }
  }

void FeatureExtractorCNN::SetMean() {
    // Set the mean image.
    mean_ = cv::Mat(input_geometry_, CV_32FC3, mean_scalar_VGG);
}
  
void FeatureExtractorCNN::Init() {
    if (modified_params_ ) {
        printf("Reloading new params\n");
        net_->CopyTrainedLayersFrom(caffe_model_);
        modified_params_ = false;
    }
}
  
void FeatureExtractorCNN::Reset() {
    net_.reset(); // decrease reference count
    net_.reset(new Net<float>(deploy_proto_, caffe::TRAIN));
    printf("In Regressor, Reset net_\n");
    net_->CopyTrainedLayersFrom(caffe_model_);
}