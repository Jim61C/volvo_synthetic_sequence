#include "Rendering/Common.h"
#include "Rendering/CommonCV.h"
#include "ParticleFilter/ParticleFilter.h"
#include "Rendering/FrameLoader.h"
#include "ParticleFilter/PFTracker.h"
#include "Rendering/AppConfig.h"
#include "FeatureExtractor/FeatureExtractorCNN.h"

int main(int argc, char **argv) {
    FLAGS_alsologtostderr = 1;

    ::google::InitGoogleLogging(argv[0]);

    AppConfig config;
    config = parseArgs(argc, argv);
    // input path 
    // string data_source = "OTB";
    // string input_dir = "../data/Walking";
    // string input_dir = "../data/Surfer";
    // string input_dir = "../data/Woman";
    // string ground_truth_rect_path = "../data/Woman/groundtruth_rect.txt";
    // string char_dir = "../data/chars";

    string data_source = config.data_source;
    string input_dir = config.input_dir;
    string ground_truth_rect_path = config.ground_truth_rect_path;

    int begin_frame = 0, end_frame = 412, step = 1;
    FrameLoader frame_loader(input_dir, data_source, ground_truth_rect_path, begin_frame, end_frame, step);
    DataManager dm;
    cout << "loading frames..." << endl;
    frame_loader.load(dm);

    if (dm.frames.size() == 0) {
        cout << "No frames read in, aborting." << endl;
        exit(-1);
    }
    
    VideoWriter writer;
    if (config.output_path != NULL) {
        int fps = 30;
        Mat frame_0 = dm.frames[0];

        writer.open(config.output_path, CV_FOURCC('m', 'p', '4', 'v'), fps, Size(frame_0.size().width, frame_0.size().height));

        if(!writer.isOpened()) {
            cout << "Could not open writer path at " << config.output_path << ", aborting." << endl;
            exit(-1);
        }
    }

    const string VGG_M_prototxt = "../nets/models/VGGM_ROIPOOL/VGGM_ROIPOOL_NO_PAD_deploy.prototxt";
    const string VGG_M_model_file = "../nets/models/VGGM_ROIPOOL/VGG_CNN_M.caffemodel";
    FeatureExtractorCNN feature_extractor(VGG_M_prototxt, VGG_M_model_file, 0, 2, false);
    ParticleFilter pf(&feature_extractor);
    PFTracker tracker(&pf);
    tracker.start(dm, writer);  

    if (writer.isOpened()) {
        cout << "finished writing to " << config.output_path << endl;
    } 

    return 0;
}

