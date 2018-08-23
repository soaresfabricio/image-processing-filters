#ifndef NOISE_H
#define NOISE_H

#include "Common.hpp"

using namespace cv;

Mat noise_saltpepper(const Mat &img, float density);
Mat noise_saltpepper_mono(const Mat &img, float density);
Mat noise_saltpepper_color(const Mat &img, float density);

#endif