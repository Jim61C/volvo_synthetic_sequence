#include "Joint.h"
#include <string>

Joint::Joint(double x, double y, string name) {
    this->x = x;
    this->y = y;
    this->z = -1;
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
    // NULL joint, the coordinates are all -1, invalid
    this->x = -1;
    this->y = -1;
    this->z = -1;
    this->is_2d = true;
    this->name = "None";
    
}