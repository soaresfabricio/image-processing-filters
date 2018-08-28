#include "Filter.hpp"
#include "Color.hpp"

template<typename T>
void print(T const& msg)
{
    std::cout << msg << std::endl;
}

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

	print("Filtering channel: ");
	for (int i = 0; i < 3; i++)
	{
		print(i);
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

/** For a given image, applies the median filter of square dimension _size_ */
Mat median_filter(const Mat &img, uint size)
{
	if (img.type() == CV_32FC3 || img.type() == CV_8UC3) return median_filter_rgb(img, size);
	if (img.type() == CV_32FC1 || img.type() == CV_8UC1) return median_filter_mono(img, size);
}

/** For each channel of a given image, applies the median filter of square dimension _size_  */
Mat median_filter_rgb(const Mat &img, uint size)
{
	Mat channel[3];

	for (int i = 0; i < 3; i++)
	{
		channel[i] = median_filter_mono(split(img, i, true), size);
	}

	return merge(channel[0], channel[1], channel[2]);
}

/** For a channel of a given image, applies the median filter of square dimension _size_  */
Mat median_filter_mono(const Mat &img, uint size)
{
	Mat img_out(img.size(), img.type());
	int radius = (size - 1) / 2;
	int median = int(size) * int(size) / 2;

	Mat img_pad;
	copyMakeBorder(img, img_pad, radius, radius, radius, radius, BORDER_REFLECT101);

	for (int j = radius; j < img.rows + radius; j++)
	{
		for (int i = radius; i < img.cols + radius; i++)
		{
			Rect region(i - radius, j - radius, size, size);
			Mat img_region = img_pad(region);

			std::vector<uchar> array(size * size);

			for (int k = 0; k < size; k++)
				for (int m = 0; m < size; m++)
					array[k * size + m] = img_region.at<uchar>(k, m); // Converting region to array

			std::sort(array.begin(), array.end()); // Sorting the array

			img_out.at<uchar>(j - radius, i - radius) = array[median]; // Acessing the position of the median
		}
	}

	return img_out;
}

/** For a given image, applies the sobel filter */
Mat sobel_filter(const Mat &img)
{
	Mat img_out = sobel_filter_ver(img) + sobel_filter_hor(img);

	bool color = true;
	if (img_out.type() == CV_32FC1 || img_out.type() == CV_8UC1)
		color = false;

	// Handles overflowing pixels
	for (int j = 0; j < img_out.rows; j++)
	{
		for (int i = 0; i < img_out.cols + color * img.cols * 2; i++)
		{
			uchar pixel = img_out.at<uchar>(j, i);

			if (pixel > 255)
				pixel = 255;
			else if (pixel < 0)
				pixel = 0;

			img_out.at<uchar>(j, i) = pixel;
		}
	}
	return img_out;
}

/** For a given image, applies the vertical sobel filter */
Mat sobel_filter_ver(const Mat &img)
{
	print("Vertical...");
	Mat mask = (Mat_<float>(3, 3) << 
				1, 2, 1,
				0, 0, 0,
				-1, -2, -1);

	return filter(img, mask);
}

/** For a given image, applies the horizontal sobel filter */
Mat sobel_filter_hor(const Mat &img)
{
	print("Horizontal...");
	Mat mask = (Mat_<float>(3, 3) << 
				-1, 0, 1,
				-2, 0, 2,
				-1, 0, 1);

	return filter(img, mask);
}


Mat laplacian_filter(const Mat &img){
	
	Mat	mask  = (Mat_<float>(3, 3)<<
				 0,  1,  0,
				 1, -4,  1,
				 0,  1,  0);

	return filter(img, mask);
}