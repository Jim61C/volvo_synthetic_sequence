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
    printf("-o output_path      write video to output_path (optional)\n");
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
    while ((c = getopt(argc,argv, "vo:")) != -1) {
        // cout << "option parsed:" << (char)(c) << endl;
        switch (c) {
            case 'v':
            config.verbose = true;
            break;
            case 'o':
            config.output_path = optarg;
            break;
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

    // parse non option arguments
    if (optind + 2 >= argc) {
        printUsageAbort(argv[0]);
    }

    config.data_source = argv[optind];
    config.input_dir = argv[optind + 1];
    config.ground_truth_rect_path = argv[optind + 2];
    
    optind += 3;

    if (optind < argc) {
        for (int i = optind; i < argc; i++) {
            printf("Unknown non-option argument %s \n", argv[i]);
        }
        printUsageAbort(argv[0]);
    }
    
    return config;
}