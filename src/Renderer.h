#ifndef RENDERER_H
#define RENDERER_H
#include "Common.h"
#include "CommonCV.h"

class Renderer {
public:
	string character_path; 
	
	Renderer(string character_path);

	cv::Mat loadCharacter(string character_file);

	/**
	overlayCharacter: overlays a character on the bg image
	bg: background
	character: chacater image, not resized
	bounding_box: [x, y, w, h] position of bounding box to overlay the character
	fgbg: entire image's fgbg
	*/
	void overlayCharacter(Mat &bg, Mat character, Mat bounding_box, Mat fgbg);

	cv::Mat getBackgroundMask(Mat foreground_mask, Rect roi);
};


#endif 