#ifndef COLOR_H
#define COLOR_H

#include "Common.hpp"


using namespace cv;

Mat split(const Mat &img, int channel, bool mono = false);
Mat merge(const Mat &img_R, const Mat &img_G, const Mat &img_B);

#endif