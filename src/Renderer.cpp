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
	// TODO: use background mask, and bitwise_and, bitwise_not to have occulusion effect

	int bottom_mid_x = bounding_box.at<int>(0, 0) + bounding_box.at<int>(0, 2)/2;
	int bootom_mid_y = bounding_box.at<int>(0, 1) + bounding_box.at<int>(0, 3);

	int target_height = bounding_box.at<int>(0, 3);
	int target_width = int(target_height * float(character.size().width)/character.size().height);

	Mat resized_char;
	cv::resize(character, resized_char, Size(target_width, target_height));

	Rect roi = Rect(bottom_mid_x - target_width/2, bootom_mid_y - target_height, target_width, target_height);
	Mat bg_roi = bg(roi);

	// create an inverse background mask from fgbg for current frame
	Mat bg_mask = getBackgroundMask(fgbg, roi);
	imshow("bg_mask for this frame:", bg_mask);
	waitKey(0);

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
	bitwise_not(foreground_mask_roi, bg_mask);

	return bg_mask;
}