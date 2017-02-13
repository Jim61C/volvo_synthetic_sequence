#include "BgSubtractor.h"
#include <iostream>
using namespace std;

BgSubtractor::BgSubtractor() {
    this->subtractor = cv::bgsegm::createBackgroundSubtractorMOG();
}


cv::Mat BgSubtractor::extractfgbg(cv::Mat frame) {
    cv::Mat fgbg;
    this->subtractor->apply(frame, fgbg);
    return fgbg;
}
