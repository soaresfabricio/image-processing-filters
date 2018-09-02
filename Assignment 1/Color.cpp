#include "Color.hpp"

Matx33f rgb2yiq_mat(0.299f, 0.587f, 0.114f,
					0.596f, -0.274f, -0.322f,
					0.211f, -0.523f, 0.312f);

Matx33f yiq2rgb_mat(1.0f, 0.956f, 0.621f,
					1.0f, -0.272f, -0.647f,
					1.0f, -1.106f, 1.703f);

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

// Segments an image into two groups of pixels,
// one above and the other below a value
Mat thresholding(const Mat &img, uchar threshold, int type, bool mean)
{
	Mat img_out;
	bool color = true;

	if (img.type() == CV_32FC1 || img.type() == CV_8UC1)
	{
		color = false;
		img_out = img;
		return thresholding_mono(img_out, threshold, type, mean);
	}
	else
	{
		img_out = rgb2yiq(img);
		return thresholding_yiq(img_out, float(threshold), type, mean);
	}
}

// Segments a monochromatic image into two groups of pixels,
// one above and the other below a value
Mat thresholding_mono(const Mat &img, uchar threshold, int type, bool mean)
{
	Mat img_out = img;
	if (mean)
		threshold = mean_y_uchar(img_out);

	for (int j = 0; j < img.rows; j++)
	{
		for (int i = 0; i < img.cols; i++)
		{
			if (type == 0)
				img_out.at<uchar>(j, i) = img_out.at<uchar>(j, i) > threshold ? 255 : 0;
			else if (type == 1)
				img_out.at<uchar>(j, i) = img_out.at<uchar>(j, i) > threshold ? 255 : img_out.at<uchar>(j, i);
			else
				img_out.at<uchar>(j, i) = img_out.at<uchar>(j, i) > threshold ? threshold : img_out.at<uchar>(j, i);
		}
	}

	return img_out;
}

// Segments a colored image into two groups of pixels,
// one above and the other below a value
Mat thresholding_yiq(const Mat &img, float threshold, int type, bool mean)
{

	Mat img_out = img;
	if (mean)
		threshold = mean_y_float(img_out);

	for (int j = 0; j < img.rows; j++)
	{
		for (int i = 2; i < img.cols * 3; i += 3)
		{
			if (type == 0)
				img_out.at<float>(j, i) = img_out.at<float>(j, i) > threshold ? 255 : 0;
			else if (type == 1)
				img_out.at<float>(j, i) = img_out.at<float>(j, i) > threshold ? 255 : img_out.at<float>(j, i);
			else
				img_out.at<float>(j, i) = img_out.at<float>(j, i) > threshold ? threshold : img_out.at<float>(j, i);
		}
	}

	return yiq2rgb(img_out);
}

// Finds the mean of the values from the Y channel
uchar mean_y_uchar(const Mat &img)
{

	long mean = 0;

	for (int j = 0; j < img.rows; j++)
		for (int i = 0; i < img.cols; i++)
			mean += img.at<uchar>(j, i);

	mean = mean / (img.rows * img.cols);
	return mean;
}

// Finds the mean of the values from the Y channel (float version)
float mean_y_float(const Mat &img)
{

	float mean = 0;

	for (int j = 0; j < img.rows; j++)
		for (int i = 2; i < img.cols * 3; i += 3)
			mean += img.at<float>(j, i);

	mean = mean / (img.rows * img.cols);
	return mean;
}

Mat add_brightness(const Mat &img, int c)
{
	bool color = true;
	if (img.type() == CV_32FC1 || img.type() == CV_8UC1)
		color = false;

	Mat img_out(img.size(), img.type());

	for (int j = 0; j < img.rows; j++)
	{
		for (int i = 0; i < img.cols + color * img.cols * 2; i++)
		{
			int pixel = img.at<uchar>(j, i) + c;

			if (pixel > 255)
				pixel = 255;
			else if (pixel < 0)
				pixel = 0;
			img_out.at<uchar>(j, i) = pixel;
		}
	}
	return img_out;
}

Mat mul_brightness(const Mat &img, float c, bool rgb)
{
	if (rgb)
		return mul_brightness_rgb(img, c);
	else
		return mul_brightness_y(img, c);
}

Mat mul_brightness_rgb(const Mat &img, float c)
{
	if (c < 0.0f)
		return img;

	bool color = true;
	if (img.type() == CV_32FC1 || img.type() == CV_8UC1)
		color = false;

	Mat img_out(img.size(), img.type());

	for (int j = 0; j < img.rows; j++)
	{
		for (int i = 0; i < img.cols + color * img.cols * 2; i++)
		{
			float pixel = img.at<uchar>(j, i) * c;

			if (pixel > 255.0f)
				pixel = 255.0f;

			img_out.at<uchar>(j, i) = pixel;
		}
	}
	return img_out;
}

Mat mul_brightness_y(const Mat &img, float c)
{

	if (c < 0.0f)
		return img;

	Mat img_out = rgb2yiq(img);

	for (int j = 0; j < img.rows; j++)
	{
		for (int i = 2; i < img.cols * 3; i += 3)
		{

			float pixel = img_out.at<float>(j, i) * c;

			if (pixel > 255.0f)
				pixel = 255.0f;

			img_out.at<float>(j, i) = pixel;
		}
	}

	return yiq2rgb(img_out);
}