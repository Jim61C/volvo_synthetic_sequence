#include <iostream>
#include "./Rendering/DataManager.h"
#include "./Rendering/FrameLoader.h"
#include "./Rendering/BgSubtractor.h"
#include "./Rendering/Renderer.h"
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
    string data_source = "OTB";
    string input_dir = "../data/Walking";
    string char_dir = "../data/chars";

    BgSubtractor subtractor;
    Renderer renderer(char_dir);


    int begin_frame = 0, end_frame = 412, step = 1;
    FrameLoader frame_loader(input_dir, data_source, begin_frame, end_frame, step);
    DataManager dm;
    frame_loader.load(dm);

    cout << "number of frames read in:" << dm.frames.size() << endl;
    cout << "number of boxes read in:" << dm.boxes.size() << endl;
    cout << "dm.boxes[0].rows:" << dm.boxes[0].rows << endl;
    cout << "dm.boxes[0].cols:" << dm.boxes[0].cols << endl;
    
    Mat bg = BgSubtractor::extractBGFromFrames(dm.frames);
    Mat character = renderer.loadCharacter("char1.png");

    for (int i = 0;i<dm.frames.size();i++) {
        Mat this_frame = dm.frames[i];
        Mat this_fgbg = subtractor.extractfgbg(this_frame);

        // blend characters
        Mat this_bg = bg.clone();
        for (int j = 0;j<dm.boxes.size();j++) {
            Mat this_frame_char_rect = dm.boxes[j].rowRange(i, i+1);
            cout << "bounding box at frame "<< i << " and box " << j << ":\n" << this_frame_char_rect << endl;

            renderer.overlayCharacter(this_bg, character, this_frame_char_rect, this_fgbg);
        }
        
        // std::ostringstream oss;
        // oss << "overlayCharacter in frame: " << i;
        // imshow(oss.str(), this_bg);
        imshow("overlayCharacter on bg", this_bg);
        waitKey(0);
    }

    // for (int i = 0;i< dm.frames.size();i++) {
    //     Mat this_frame = dm.frames[i];
    //     Mat fgbg = subtractor.extractfgbg(this_frame);
    //     imshow("fgbg", fgbg);
    //     waitKey(0);
    // }



    return 0;
}

