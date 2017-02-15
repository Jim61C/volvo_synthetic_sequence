#include "Renderer.h"

Renderer::Renderer(string character_path) {
	if (character_path.back()!= '/') {
		character_path += '/';
	}
	this->character_path = character_path;
}


cv::Mat Renderer::loadCharacter(string character_file) {
	return imread(this->character_path + character_file, 1);
}

void Renderer::overlayCharacter(Mat &bg, Mat character, Mat bounding_box, Mat fgbg) {

	int bottom_mid_x = bounding_box.at<int>(0, 0) + bounding_box.at<int>(0, 2)/2;
	int bootom_mid_y = bounding_box.at<int>(0, 1) + bounding_box.at<int>(0, 3);

	int target_height = bounding_box.at<int>(0, 3);
	int target_width = int(target_height * float(character.size().width)/character.size().height);

	Mat resized_char;
	cv::resize(character, resized_char, Size(target_width, target_height));

	Rect roi = Rect(bottom_mid_x - target_width/2, bootom_mid_y - target_height, target_width, target_height);
	Mat bg_roi = bg(roi);

	float center_portion = 0.5;
	Rect centre_roi = Rect(bottom_mid_x - int(target_width * center_portion/2), bootom_mid_y - target_height, 
		int(target_width * center_portion), target_height);

	// create an inverse background mask of the centre area of the bounding box 
	// (bg will be dark and target will be white) from fgbg for current frame
	Mat bg_mask = getBackgroundMask(fgbg, roi);
	Mat bg_mask_thresholded;
	threshold(bg_mask, bg_mask_thresholded, 5, 255, cv::THRESH_BINARY);
	imshow("bg_mask for this frame:", bg_mask_thresholded);
	waitKey(0);

	// TODO: use remove unconnected component to get smooth bg_mask_threshholded

	Mat resized_char_occluded;  
	bitwise_and(resized_char, resized_char, resized_char_occluded, bg_mask_thresholded);
	imshow("bitwise_and with bg_mask:", resized_char_occluded);


	// create a mask of bg regions in resized_char 
	Mat resized_char_bw;
	cvtColor(resized_char, resized_char_bw, CV_RGB2GRAY);
	Mat mask;
	int threshold_value = 5, max_BINARY_value = 255;
	threshold(resized_char_bw, mask, threshold_value, max_BINARY_value, cv::THRESH_BINARY_INV);
	// imshow("mask", mask);
	// waitKey(0);

	// blend bg_roi and resized_char
	cv::add(resized_char, bg_roi , resized_char, mask);

	// imshow("bg_roi added", resized_char);
	// waitKey(0);

	resized_char.copyTo(bg.colRange(roi.x, roi.x + roi.width).rowRange(roi.y, roi.y + roi.height));
}


cv::Mat Renderer::getBackgroundMask(Mat foreground_mask, Rect roi) {
	Mat foreground_mask_roi = foreground_mask(roi);

	Mat bg_mask;
	// bitwise_not(foreground_mask_roi, bg_mask);

	// return bg_mask;
	return foreground_mask_roi;
}