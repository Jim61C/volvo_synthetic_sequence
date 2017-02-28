#include "Joint.h"
#include <string>

Joint::Joint(double x, double y, string name) {
    this->x = x;
    this->y = y;
    this->is_2d = true;
    this->name = name;
}

Joint::Joint(double x, double y, double z, string name) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->is_2d = false;
    this->name = name;
}

Joint::Joint() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->is_2d = true;
    this->name = "None";
    
}