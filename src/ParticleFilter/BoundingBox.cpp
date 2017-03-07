#include "BoundingBox.h"

// constructors
BoundingBox::BoundingBox() {
    this->x = 0;
    this->y = 0;
    this->w = 0;
    this->h = 0;
}

BoundingBox::BoundingBox(int x, int y, int w, int h) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}


void BoundingBox::setBoxCoordinate(int x, int y, int w, int h) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

void BoundingBox::setBox(Mat rect) {
    this->x = rect.at<int>(0, 0);
    this->y= rect.at<int>(0, 1);
    this->w = rect.at<int>(0, 2);
    this->h = rect.at<int>(0, 3);
}

void BoundingBox::setAsBox(BoundingBox & b) {
    this->x = b.x;
    this->y = b.y;
    this->w = b.w;
    this->h = b.h;
}

// Draw a rectangle corresponding to this bbox with the given color.
void BoundingBox::Draw(const int r, const int g, const int b, cv::Mat & image, const int thickness) {
    Rect rect = Rect(this->x, this->y, this->w, this->h);
    const cv::Scalar box_color(b, g, r);

    // Draw a rectangle corresponding to this bbox with the given color
    cv::rectangle(image, rect, box_color, thickness);
}

// Draw a white rectangle corresponding to this bbox.
void BoundingBox::DrawBoundingBox(cv::Mat & figure_ptr) {
    // cout << "In drawing bounding box, " << "x:" << this->x << ",y:" << this->y << ", w:" << this->w << ",h:" << this->h << endl;
    Draw(255, 255, 255, figure_ptr);
}


void BoundingBox::calBoundingBoxNewScale  (double s, BoundingBox &box) {
    double centre_x = this->x + this->w / 2.0;
    double centre_y = this->y + this->h / 2.0;

    double new_w = this->w * s;
    double new_h = this->h * s;

    double new_x = centre_x - 0.5 * new_w;
    double new_y = centre_y - 0.5 * new_y;

    return box.setBoxCoordinate((int)(new_x), (int)(new_y), (int)(new_w), (int)(new_h));
}


// get bounding box center
pair<int, int> BoundingBox::getBoundingBoxCenter() {
    pair <int,int> centre;

    centre.first = (int)(this->x + this->w /2.0);
    centre.second = (int)(this->y + this->h /2.0);

    return centre;
}