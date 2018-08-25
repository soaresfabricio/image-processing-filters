#include "Filter.hpp"
#include "Color.hpp"

/** For a given image, apply a certain mask */
Mat filter(const Mat &img, const Mat &mask)
{
	if (img.type() == CV_32FC3 || img.type() == CV_8UC3) return filter_rgb(img, mask);
	if (img.type() == CV_32FC1 || img.type() == CV_8UC1) return filter_mono(img, mask);
}

/** For a given image, apply a certain mask to EACH of its channels */
Mat filter_rgb(const Mat &img, const Mat &mask)
{
	Mat channel[3];

	for (int i = 0; i < 3; i++)
	{
		channel[i] = filter_mono(split(img, i, true), mask);
	}

	return merge(channel[0], channel[1], channel[2]);
}

/** For a given image, apply a certain mask to ONE of its channels */
Mat filter_mono(const Mat &img, const Mat &mask)
{
	// Radius of the mask is needed to add padding to the image
	Mat img_out(img.size(), img.type());
	int radius = (mask.rows - 1) / 2; // Integer division

	// Adding the padding around the image
	Mat img_pad;
	copyMakeBorder(img, img_pad, radius, radius, radius, radius, BORDER_REFLECT101);

	// Starts at the first pixel of the original image (size of the radius)
	for (int j = radius; j < img.rows + radius; j++)
	{
		for (int i = radius; i < img.cols + radius; i++)
		{
			// Creats a region and multiplies it times the mask
			Rect region(i - radius, j - radius, mask.rows, mask.cols);
			Mat img_region = img_pad(region);
			img_region.convertTo(img_region, CV_32F);
			img_region = img_region.mul(mask); 

			float pixel = sum(img_region)[0];

			// Ignores non-byte values (and negative values)
			if (pixel > 255)
			{
				pixel = 255;
			}
			if (pixel < 0)
			{
				pixel = 0;
			}

			img_out.at<uchar>(j - radius, i - radius) = pixel;
		}
	}

	return img_out;
}

/** For a given image, apply a box linear filter with box of square dimension _size_ */
Mat mean_filter(const Mat &img, uint size)
{
	Mat mask = Mat::zeros(size, size, CV_32F);

	// Density is given by 1 over the number of pixels of the box
	float density = 1 / float(size * size);

	// Generates the mask
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			mask.at<float>(i, j) = density;

	// Applies the mask
	return filter(img, mask);
}