#include "Rendering/Common.h"
#include "Rendering/CommonCV.h"
#include "ParticleFilter/ParticleFilter.h"
#include "Rendering/FrameLoader.h"
#include "ParticleFilter/PFTracker.h"
#include "Rendering/AppConfig.h"

int main(int argc, char **argv) {
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
    frame_loader.load(dm);

    PFTracker tracker;
    tracker.start(dm);    

    return 0;
}

