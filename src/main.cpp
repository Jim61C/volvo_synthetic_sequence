#include <iostream>
#include "DataManager.h"
#include "FrameLoader.h"
#include "BgSubtractor.h"
#include <string.h>

using namespace std;
int main(int argc, char **argv)
{
    // cv::viz::Viz3d window = cv::viz::Viz3d("Viz demonstration");

    // cv::Point3d min(0.25, 0.0, 0.25);
    // cv::Point3d max(0.75, 0.5, 0.75);

    // cv::viz::WCube cube(min, max, true, cv::viz::Color::blue());
    // cube.setRenderingProperty(cv::viz::LINE_WIDTH, 4.0);

    // window.showWidget("Axis widget", cv::viz::WCoordinateSystem());
    // window.showWidget("Cube widget", cube);

    // while(!window.wasStopped()) {
    //     window.spinOnce(1, true);
    // }

    // input path 
    string input_dir = "../data/Walking";
    string data_source = "OTB";

    BgSubtractor subtractor;

    int begin_frame = 0, end_frame = 100, step = 1;
    FrameLoader frame_loader(input_dir, data_source, begin_frame, end_frame, step);
    DataManager dm;
    frame_loader.load(dm);

    cout << "number of frames read in:" << dm.frames.size() << endl;
    
    for (int i = 0;i< dm.frames.size();i++) {
        Mat this_frame = dm.frames[i];
        Mat fgbg = subtractor.extractfgbg(this_frame);
        imshow("fgbg", fgbg);
        waitKey(0);
    }



    return 0;
}

