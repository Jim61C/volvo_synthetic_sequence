#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "Common.h"
#include "CommonCV.h"
#include <vector>

using namespace std;

class DataManager {
public:
	vector<Mat> frames;

	vector<Mat> boxes; // each object's bounding box will be loaded as Mx4 Mat, M is the number of frames, single Object Tracking, size will just be 1
};

#endif