#include "Common.h"

struct AppConfig {
    char * input_dir;
    char * ground_truth_rect_path;
    char * data_source;
    char * output_path;
    bool verbose;

    AppConfig() {
        this->verbose = false;
        this->input_dir = NULL;
        this->ground_truth_rect_path = NULL;
        this->data_source = NULL;    
        this->output_path = NULL;
    }

};

void printUsage(const char * executable_name);
void printUsageAbort(const char * executable_name);
AppConfig parseArgs(int argc, char**argv);