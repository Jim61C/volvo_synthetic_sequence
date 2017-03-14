#include <iostream>
#include <string>
#include "Pose/PoseRepre.h"
using namespace std;

int main(int argc, char **argv) {
    cout << "in main_pose, test loading from pose.txt and generate features..." << endl;
    string pose_input_path = "/home/jimxing/proj/CPM/Realtime_Multi-Person_Pose_Estimation/testing/sample_image/Skater2_0001_pose.txt";
    string input_frame_path = "/home/jimxing/proj/CPM/Realtime_Multi-Person_Pose_Estimation/testing/sample_image/Skater2_0001.jpg";
    Mat frame = imread(input_frame_path, CV_LOAD_IMAGE_UNCHANGED);
    imshow("frame", frame);
    waitKey(0);
    vector<Joint> joints_loaded = PoseRepre::loadOnePoseFromFile(pose_input_path);
    
    return 0;
}