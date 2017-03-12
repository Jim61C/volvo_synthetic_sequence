#include "AppConfig.h"
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <unistd.h>


void printUsage(const char * executable_name) {
    printf("\n");
    printf("Usage: %s [options] <data_source> <input_dir> <groundtruth_rect_path>\n", executable_name);
    printf("<data_source> -- e.g., OTB\n");
    printf("<input_dir> -- e.g., ../data/Walking\n");
    printf("<groundtruth_rect_path> -- e.g., ../data/Walking/groundtruth_rect.txt\n");
    printf("Program Options:\n");
    printf("-v                  print verbose info\n");
    printf("\n");
}

void printUsageAbort(const char * executable_name) {
    printUsage(executable_name);
    exit(-1);
}

AppConfig parseArgs(int argc, char**argv) {
    AppConfig config;
    // parse the options
    int c;
    while ((c = getopt(argc,argv, "v")) != -1) {
        switch (c) {
            case 'v':
            config.verbose = true;
            case '?':
            if (isprint(optopt)) {
                fprintf(stderr, "Unknown option %c\n", optopt);
            }
            else {
                fprintf(stderr, "Unknown option \\x%x\n", optopt);
            }
            printUsageAbort(argv[0]);
        }
    }

    // parse arguments
    if (argc < 4) {
        printUsageAbort(argv[0]);
    }

    config.data_source = argv[1];
    config.input_dir = argv[2];
    config.ground_truth_rect_path = argv[3];
    
    return config;
}