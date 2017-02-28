#include <iostream>
#include <string.h>
#include "CommonCV.h"
#include "DataManager.h"
using namespace std;

class FrameLoader {

private:
	string directory;
	string data_source;
	int begin_frame;
	int end_frame;
	int step;

public:
	FrameLoader(string input_dir, 
		string data_source, 
		int begin_frame, 
		int end_frame, 
		int step):
	directory(input_dir), 
	data_source(data_source), 
	begin_frame(begin_frame), 
	end_frame(end_frame), 
	step(step) {}

	void load(DataManager & dm);
	bool has_suffix(const string& s, const string& suffix);

	/**
	OTB format should contain image files in directory/img
	*/
	void loadOTB(DataManager& dm, string directory, int begin_frame, int end_frame, int step);

	/**
	Bouding boxes file needs to be at directory/result_rect*.txt
	with format of [x y w h] per line
	*/
	void loadBoundingBoxes(DataManager & dm);
};