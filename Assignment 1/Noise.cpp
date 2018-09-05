#include "Noise.hpp"
#include "Color.hpp"

Mat noise_saltpepper(const Mat &img, float density){
	if(img.type() == CV_32FC3 || img.type() == CV_8UC3) return noise_saltpepper_color(img, density);
	if(img.type() == CV_32FC1 || img.type() == CV_8UC1) return noise_saltpepper_mono(img, density);
}

Mat noise_saltpepper_color(const Mat &img, float density){

	Mat channel[3];

	for(int i = 0; i < 3; i++){
		channel[i] = noise_saltpepper_mono(split(img, i, true), density);
	}

	return merge(channel[0], channel[1], channel[2]);
}

Mat noise_saltpepper_mono(const Mat &img, float density){

	uchar pixel;
	Mat out_img(img.size(), img.type());

	

	for(int j = 0; j < img.rows; j++){
		for(int i = 0; i < img.cols; i++){
			pixel = uchar(rand()%256);			
			out_img.at<uchar>(j, i) = pixel < uint(127*density) ? 0 : img.at<uchar>(j, i);
			out_img.at<uchar>(j, i) = pixel > uint(127*(2.0f-density)) ? 255 : out_img.at<uchar>(j, i);
		}
	}

	return out_img;
}