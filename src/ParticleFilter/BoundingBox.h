#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include "../Rendering/Common.h"
#include "../Rendering/CommonCV.h"

class BoundingBox {
public:
    double x;
    double y;
    double w;
    double h;

    // constructors
    BoundingBox();

    BoundingBox(double x, double y, double w, double h);

    void setBoxCoordinate(double x, double y, double w, double h);

    void setBox(Mat rect);

    void setAsBox(BoundingBox & b);

    // Draw a rectangle corresponding to this bbox with the given color.
    void Draw(const int r, const int g, const int b, cv::Mat &image, const int thickness = 1);

    // Draw a white rectangle corresponding to this bbox.
    void DrawBoundingBox(cv::Mat & figure_ptr);

    // get boundingbox with new scale
    void calBoundingBoxNewScale (double s, BoundingBox &box);

    // get bounding box center
    pair<double, double> getBoundingBoxCenter();

    // to VOT coordinates
    void toTwoCornerCoordinates(double &x1, double &y1, double &x2, double &y2) const;
};

#endif