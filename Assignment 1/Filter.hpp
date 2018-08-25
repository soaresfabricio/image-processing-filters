#ifndef FILTER_H
#define FILTER_H

#include "Common.hpp"

using namespace cv;

Mat filter(const Mat &img, const Mat &mask);
Mat filter_rgb(const Mat &img, const Mat &mask);
Mat filter_mono(const Mat &img, const Mat &mask);

Mat mean_filter(const Mat &img, uint size);
Mat median_filter(const Mat &img, uint size);
Mat median_filter_mono(const Mat &img, uint size);
Mat median_filter_rgb(const Mat &img, uint size);
Mat sobel_filter_hor(const Mat &img);
Mat sobel_filter_ver(const Mat &img);
Mat sobel_filter(const Mat &img);
Mat laplacian_filter_ver(const Mat &img);
Mat laplacian_filter_hor(const Mat &img);
Mat laplacian_filter(const Mat &img);

#endif