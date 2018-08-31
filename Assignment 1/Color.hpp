#ifndef COLOR_H
#define COLOR_H

#include "Common.hpp"

using namespace cv;



Mat rgb2yiq2rgb (const Mat &img);
Mat rgb2yiq (const Mat &img);
Mat yiq2rgb (const Mat &img);

Mat split(const Mat &img, int channel, bool mono = false);
Mat merge(const Mat &img_R, const Mat &img_G, const Mat &img_B);

Mat negative (const Mat &img, bool rgb = true);
Mat negative_rgb (const Mat &img);
Mat negative_y (const Mat &img);

Mat thresholding(const Mat &img, uchar threshold, int type, bool mean = true);
Mat thresholding_mono(const Mat &img, uchar threshold, int type, bool mean = true);
Mat thresholding_yiq(const Mat &img, float threshold, int type, bool mean = true);

uchar mean_y_uchar(const Mat &img);
float mean_y_float(const Mat &img);


#endif