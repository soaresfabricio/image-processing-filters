#include "Color.hpp"

Matx33f rgb2yiq_mat (0.299f,  0.587f,  0.114f,
					 0.596f, -0.274f, -0.322f,
					 0.211f, -0.523f,  0.312f);

Matx33f yiq2rgb_mat(1.0f,  0.956f,  0.621f,
					1.0f, -0.272f, -0.647f,
					1.0f, -1.106f,  1.703f);

Mat split(const Mat &img, int channel, bool mono)
{
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

Mat negative(const Mat &img, bool rgb)
{

	return rgb ? negative_rgb(img) : negative_y(img);

	// if(rgb)
	// 	return negative_rgb(img);
	// else
	// 	return negative_y(img);
}

Mat negative_rgb(const Mat &img)
{
	bool color = true;
	if (img.type() == CV_32FC1 || img.type() == CV_8UC1)
		color = false;

	Mat img_out(img.size(), img.type());

	for (int j = 0; j < img.rows; j++)
	{
		for (int i = 0; i < img.cols + color * img.cols * 2; i++)
		{
			img_out.at<uchar>(j, i) = 255 - img.at<uchar>(j, i);
		}
	}
	return img_out;
}

Mat negative_y(const Mat &img)
{
	Mat img_out = rgb2yiq(img);

	for (int j = 0; j < img.rows; j++)
	{
		for (int i = 2; i < 3 * img.cols; i += 3)
		{

			img_out.at<float>(j, i) = 255 - img_out.at<float>(j, i);
		}
	}

	return yiq2rgb(img_out);
}


Mat rgb2yiq2rgb(const Mat &img)
{
	return yiq2rgb(rgb2yiq(img));
}

Mat rgb2yiq(const Mat &img)
{
	Mat img_out(img.size(), img.type());
	img_out = img.clone();
	img_out.convertTo(img_out, CV_32F);

	for (int j = 0; j < img.rows; j++)
	{
		for (int i = 0; i < img.cols * 3; i += 3)
		{
			Vec3f pixel(img_out.at<float>(j, i + 2), img_out.at<float>(j, i + 1), img_out.at<float>(j, i));
			pixel = rgb2yiq_mat * pixel;

			for (int k = 0; k < 3; k++)
			{
				img_out.at<float>(j, i + 2 - k) = pixel[k];
			}
		}
	}
	return img_out;
}

Mat yiq2rgb(const Mat &img)
{
	Mat img_out(img.size(), img.type());
	img_out = img;
	img_out.convertTo(img_out, CV_32F);

	for (int j = 0; j < img.rows; j++)
	{
		for (int i = 0; i < img.cols * 3; i += 3)
		{
			Vec3f pixel(img_out.at<float>(j, i + 2), img_out.at<float>(j, i + 1), img_out.at<float>(j, i));
			pixel = yiq2rgb_mat * pixel;

			for (int k = 0; k < 3; k++)
			{
				if (pixel[k] > 255.0)
					pixel[k] = 255.0;
				else if (pixel[k] < 0.0)
					pixel[k] = 0.0;
				img_out.at<float>(j, i + 2 - k) = pixel[k];
			}
		}
	}

	img_out.convertTo(img_out, CV_8UC3);

	return img_out;
}