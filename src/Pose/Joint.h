#ifndef JOINT_H
#define JOINT_H
#include <string>       // std::string
#include <iostream> 
using namespace std;

class Joint {
public:
    double x;
    double y;
    double z; // might not be there
    bool is_2d;
    string name;

    Joint();
    Joint(double x, double y, string name);
    Joint(double x, double y, double z, string name);
};

#endif