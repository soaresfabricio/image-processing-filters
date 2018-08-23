#include "Color.hpp"

Mat split(const Mat &img, int channel, bool mono)
{

	int type;

	Mat img_out(img.size(), mono ? MONO_IMAGE : COLOR_IMAGE);

	for (int j = 0; j < img.rows; j++)
	{
		for (int i = 0; i < img.cols; i++)
		{
			img_out.at<uchar>(j, i + 2 - channel + !mono * 2 * i + mono * (channel - 2)) = img.at<uchar>(j, 3 * i + 2 - channel);
		}
	}

	return img_out;
}

Mat merge(const Mat &img_R, const Mat &img_G, const Mat &img_B)
{
	Mat img_out(img_R.size(), CV_8UC3);

	for (int j = 0; j < img_R.rows; j++)
	{
		for (int i = 0; i < img_R.cols; i++)
		{
			img_out.at<uchar>(j, 3 * i + 2) = img_R.at<uchar>(j, i);
			img_out.at<uchar>(j, 3 * i + 1) = img_G.at<uchar>(j, i);
			img_out.at<uchar>(j, 3 * i) = img_B.at<uchar>(j, i);
		}
	}

	return img_out;
}