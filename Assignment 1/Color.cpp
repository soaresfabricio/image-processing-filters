#include "Color.hpp"

Mat split(const Mat &img, int channel, bool mono){
	
	int type;

	Mat img_out(img.size(), mono ? MONO_IMAGE : COLOR_IMAGE);

	for(int j = 0; j < img.rows; j++){
		for(int i = 0; i < img.cols; i++){
			img_out.at<uchar>(j, i+2-channel + !mono*2*i + mono*(channel-2)) = img.at<uchar>(j, 3*i + 2-channel);
		}
	}

	return img_out;
}