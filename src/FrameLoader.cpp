#include "FrameLoader.h"
#include <fstream>
#include <stdio.h>
using namespace std;

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
	if (this->data_source.compare("OTB") == 0) {
		loadOTB(dm, this->directory+"/img", this->begin_frame, this->end_frame, this->step);
	}
}
