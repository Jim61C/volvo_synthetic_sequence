#include <iostream>
#include <string.h>
#include "CommonCV.h"
#include "DataManager.h"
using namespace std;

class FrameLoader {
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
	void loadOTB(DataManager& dm, string directory, int begin_frame, int end_frame, int step);

private:
	string directory;
	string data_source;
	int begin_frame;
	int end_frame;
	int step;
};