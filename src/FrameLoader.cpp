#include "FrameLoader.h"
#include <fstream>
#include <sstream>
#include <stdio.h>
using namespace std;

int countNumberfLines(string txt_path) {
    ifstream myfile(txt_path);

    int number_of_lines = 0;
    std::string line;

    while (std::getline(myfile, line))
        number_of_lines++;

    cout << "number of lines in " << txt_path << " = " << number_of_lines << endl;

    return number_of_lines;
}

void loadOneRawTxtFile(string txt_path, Mat & target) {
    ifstream rect_stream(txt_path);
    string next_line;
    string token;

    int x, y, w, h;
    int num_frames = countNumberfLines(txt_path);
    target = Mat::zeros(num_frames, 4, CV_32S);

    int i = 0;
    while (!rect_stream.eof()) {
        getline(rect_stream, next_line); // next_line is expected to be space delimited
        if (next_line.length() != 0) {
            istringstream iss (next_line);
            iss >> x;
            iss >> y;
            iss >> w;
            iss >> h;

            target.at<int>(i, 0) = x;
            target.at<int>(i, 1) = y;
            target.at<int>(i, 2) = w;
            target.at<int>(i, 3) = h;

            i++;
        }
    }
}


bool FrameLoader::has_suffix(const string& s, const string& suffix)
{
    return (s.size() >= suffix.size()) && equal(suffix.rbegin(), suffix.rend(), s.rbegin());
}

// non member functions needs to be defined before calling, or have function auxliaries at the top
void FrameLoader::loadOTB(DataManager& dm, string directory, int begin_frame, int end_frame, int step) {
    if (directory.back()!='/') {
        directory += '/';
    }

    DIR *p_dir;
    int index=0;
    struct dirent *entry;
    vector<string> img_name_list;
    if( (p_dir=opendir(directory.c_str())) !=NULL){
        while((entry = readdir(p_dir))!= NULL){
            if( has_suffix(entry->d_name, ".png") || has_suffix(entry->d_name, ".jpg") ) {
                string this_img_path = (directory + entry->d_name);
                // cout << this_img_path << endl;
                img_name_list.push_back(this_img_path);
                index++;
            }
        }
        closedir(p_dir);
    }
    sort(img_name_list.begin(), img_name_list.end());

    for (int i=begin_frame; i<MIN(end_frame, img_name_list.size()); i+=step){
        cv::Mat frame_buffer =imread(img_name_list[i], CV_LOAD_IMAGE_UNCHANGED);
        dm.frames.push_back(frame_buffer);
        // imshow("read in frame", frame_buffer);
        // waitKey(0);
    }
}


void FrameLoader::load(DataManager & dm) {
	// load frames
    if (this->data_source.compare("OTB") == 0) {
		loadOTB(dm, this->directory+"/img", this->begin_frame, this->end_frame, this->step);
	}

    // load bounding boxes
    loadBoundingBoxes(dm);
}


void FrameLoader::loadBoundingBoxes(DataManager & dm) {
    // bounding boxes is contained in this->directory/result_rect*.txt
    string bounding_box_dir = this->directory;
    if (bounding_box_dir.back() != '/') {
        bounding_box_dir += '/';
    }

    DIR *p_dir;
    struct dirent *entry;
    vector<string> rect_names;
    if ((p_dir = opendir(bounding_box_dir.c_str())) != NULL) {
        while ( (entry = readdir(p_dir)) != NULL) {
            string this_entry = entry->d_name;
            if (has_suffix(this_entry, ".txt") && this_entry.find("result_rect") != string::npos) {
                // one of the result_rect*.txt
                Mat rects;
                loadOneRawTxtFile(bounding_box_dir + entry->d_name, rects);

                // TODO: check against dm.frames.size() and make sure they are consistent
                dm.boxes.push_back(rects);
            }
        }
    }
}
