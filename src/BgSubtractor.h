#include "CommonCV.h"

class BgSubtractor {
public:
	// cv::BackgroundSubtractorMOG2 *subtractor = cv::createBackgroundSubtractorMOG2();
    Ptr<BackgroundSubtractor> subtractor;

    BgSubtractor();
    cv::Mat extractfgbg(cv::Mat frame);

};