#include "Rendering/Common.h"
#include "Rendering/CommonCV.h"
#include "ParticleFilter/ParticleFilter.h"
#include "Rendering/FrameLoader.h"
#include "ParticleFilter/PFTracker.h"

int main(int argc, char **argv) {
    cout << "first load all frames inside" << endl;
    // input path 
    string data_source = "OTB";
    string input_dir = "../data/Walking";
    // string input_dir = "../data/Surfer";
    string ground_truth_rect_name = "groundtruth_rect.txt";
    // string char_dir = "../data/chars";


    int begin_frame = 0, end_frame = 412, step = 1;
    FrameLoader frame_loader(input_dir, data_source, begin_frame, end_frame, step);
    DataManager dm;
    frame_loader.load(dm);

    PFTracker tracker;
    tracker.start(dm);    

    return 0;
}

