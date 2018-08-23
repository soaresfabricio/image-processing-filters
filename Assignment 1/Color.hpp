#ifndef COLOR_H
#define COLOR_H

#include "Common.hpp"

using namespace cv;

Mat split(const Mat &img, int channel, bool mono = false);

#endif