#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include "../Rendering/Common.h"
#include "../Rendering/CommonCV.h"

class BoundingBox {
public:
    int x;
    int y;
    int w;
    int h;

    // constructors
    BoundingBox();

    BoundingBox(int x, int y, int w, int h);

    void setBoxCoordinate(int x, int y, int w, int h);

    void setBox(Mat rect);

    void setAsBox(BoundingBox & b);

    // Draw a rectangle corresponding to this bbox with the given color.
    void Draw(const int r, const int g, const int b, cv::Mat &image, const int thickness = 1);

    // Draw a white rectangle corresponding to this bbox.
    void DrawBoundingBox(cv::Mat & figure_ptr);

    // get boundingbox with new scale
    void calBoundingBoxNewScale (double s, BoundingBox &box);

    // get bounding box center
    pair<int, int> getBoundingBoxCenter();
};

#endif