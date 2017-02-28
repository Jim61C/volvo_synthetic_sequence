#include "BgSubtractor.h"
#include <iostream>
using namespace std;

BgSubtractor::BgSubtractor() {
    // this->subtractor = cv::bgsegm::createBackgroundSubtractorMOG();
    this->subtractor = cv::createBackgroundSubtractorMOG2();
}


cv::Mat BgSubtractor::extractfgbg(cv::Mat frame) {
    cv::Mat fgbg;
    this->subtractor->apply(frame, fgbg);
    return fgbg;
}

cv::Mat BgSubtractor::extractBGFromFrames(vector<Mat> &frames) {
	Ptr<BackgroundSubtractor> fgbg_subtractor = cv::createBackgroundSubtractorMOG2();

	for (int i = 0;i<frames.size();i++) {
		Mat temp;
		fgbg_subtractor->apply(frames[i], temp);
	}

	Mat bg;

	fgbg_subtractor->getBackgroundImage(bg);

	return bg;
}
