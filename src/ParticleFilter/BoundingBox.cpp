#include "BoundingBox.h"

// constructors
BoundingBox::BoundingBox() {
    this->x = 0.0;
    this->y = 0.0;
    this->w = 0.0;
    this->h = 0.0;
}

BoundingBox::BoundingBox(double x, double y, double w, double h) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}


void BoundingBox::setBoxCoordinate(double x, double y, double w, double h) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

void BoundingBox::setBox(Mat rect) {
    this->x = rect.at<double>(0, 0);
    this->y= rect.at<double>(0, 1);
    this->w = rect.at<double>(0, 2);
    this->h = rect.at<double>(0, 3);
}

void BoundingBox::setAsBox(BoundingBox & b) {
    this->x = b.x;
    this->y = b.y;
    this->w = b.w;
    this->h = b.h;
}

// Draw a rectangle corresponding to this bbox with the given color.
void BoundingBox::Draw(const int r, const int g, const int b, cv::Mat & image, const int thickness) {
    // get the top left point.
    Point point1(this->x, this->y);

    // get the bottom right point.
    Point point2(this->x + this->w, this->y + this->h);

    Rect rect = Rect(point1, point2);
    const cv::Scalar box_color(b, g, r);

    // if BW, convet to BGR to show colored BBox
    if (image.channels() == 1) {
        cvtColor(image,image,CV_GRAY2BGR);
    }
    // Draw a rectangle corresponding to this bbox with the given color
    cv::rectangle(image, rect, box_color, thickness);
}

// Draw a white rectangle corresponding to this bbox.
void BoundingBox::DrawBoundingBox(cv::Mat & figure_ptr) {
    Draw(255, 255, 255, figure_ptr);
}


void BoundingBox::calBoundingBoxNewScale  (double s, BoundingBox &box) {
    double centre_x = this->x + this->w / 2.0;
    double centre_y = this->y + this->h / 2.0;

    double new_w = this->w * s;
    double new_h = this->h * s;

    double new_x = centre_x - 0.5 * new_w;
    double new_y = centre_y - 0.5 * new_h;

    return box.setBoxCoordinate(new_x, new_y, new_w, new_h);
}


// get bounding box center
pair<double, double> BoundingBox::getBoundingBoxCenter() {
    pair <double, double> centre;

    centre.first = this->x + this->w /2.0;
    centre.second = this->y + this->h /2.0;

    return centre;
}