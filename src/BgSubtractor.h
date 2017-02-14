#include "CommonCV.h"
#include "Common.h"

class BgSubtractor {
public:
	// cv::BackgroundSubtractorMOG2 *subtractor = cv::createBackgroundSubtractorMOG2();
    Ptr<BackgroundSubtractor> subtractor;

    BgSubtractor();
    cv::Mat extractfgbg(cv::Mat frame);


    // static API for getting the bg after learned over frames
   	static cv::Mat extractBGFromFrames(vector<Mat> &frames);

};