#include "Noise.hpp"
#include "Color.hpp"
#include "Filter.hpp"
#include <string>

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        std::cout << ">> Usage: ./[this tool] [input file name] [mono | color] [filter number]" << std::endl;
        std::cout << ">> Available filters: \n\t0: \tNo filter \n\t1-3: \tR, G and B splitting to monocromatic image. \n\t4-6: \tR, G and B splitting to color image.\n\t7: \tAdd Salt and Pepper noise.\n\t8-10: \tBox filters of size 3, 7 and 11.\n\t11-13: \tMedian filters of size 3, 7 and 11.\n\t14: \tSobel filter (edge detection). \n\t15-16: \tNegative of input image (RGB and Y). \n\t17-21: \tThresholding (50, 20, 200, 230, Y channel mean). \n\t22-25: \tAdd brightness (30, 80, -100, -150).\n\t26-27: \tMultiply brightness (RGB, 0.5 and 1.5). \n\t28-29: \tMultiply brightness (Y, 0.5 and 1.5)." << std::endl;
        exit(EXIT_FAILURE);
    }

    int filter_opt = std::strtol(argv[3], NULL, 10);

    std::string file_name = std::string(argv[1]);
    std::string input_name = "./original/" + file_name;

    int load_type = strcmp(argv[2], "mono") ? CV_LOAD_IMAGE_COLOR : CV_LOAD_IMAGE_GRAYSCALE;

    Mat img = imread(input_name, load_type);

    if (!img.data)
    {
        std::cout << "Could not open or find the image " + input_name << std::endl;
        return -1;
    }

    file_name = file_name.substr(0, file_name.length() - 4); // remove extension

    std::cout << "Input image loaded from: " + input_name << std::endl;

    Mat img_out, mask;

    std::string output_name = "./processed/" + file_name;

    switch (filter_opt)
    {
    case 0:
        exit(EXIT_SUCCESS);
    case 1:
        std::cout << ">> Splitting R channel and generating monochromatic image." << std::endl;
        img_out = split(img, 0, true);
        output_name += "_R_mono";
        break;
    case 2:
        std::cout << ">> Splitting G channel and generating monochromatic image." << std::endl;
        img_out = split(img, 1, true);
        output_name += "_G_mono";
        break;
    case 3:
        std::cout << ">> Splitting B channel and generating monochromatic image." << std::endl;
        img_out = split(img, 2, true);
        output_name += "_B_mono";
        break;
    case 4:
        std::cout << ">> Splitting R channel and generating color image." << std::endl;
        img_out = split(img, 0, false);
        output_name += "_R_tri";
        break;
    case 5:
        std::cout << ">> Splitting G channel and generating color image." << std::endl;
        img_out = split(img, 1, false);
        output_name += "_G_tri";
        break;
    case 6:
        std::cout << ">> Splitting B channel and generating color image." << std::endl;
        img_out = split(img, 2, false);
        output_name += "_B_tri";
        break;
    case 7:
        std::cout << ">> Adding salt and pepper noise to input image." << std::endl;
        img_out = noise_saltpepper(img, 0.05f);
        output_name += "_saltpepper";
        break;
    case 8:
        std::cout << ">> Processing box filter (size 3) on the input image." << std::endl;
        img_out = mean_filter(img, 3);
        output_name += "_mean_3";
        break;
    case 9:
        std::cout << ">> Processing box filter (size 7) on the input image." << std::endl;
        img_out = mean_filter(img, 7);
        output_name += "_mean_7";
        break;
    case 10:
        std::cout << ">> Processing box filter (size 11) on the input image." << std::endl;
        img_out = mean_filter(img, 11);
        output_name += "_mean_11";
        break;
    case 11:
        std::cout << ">> Processing median filter (size 3) on the input image." << std::endl;
        img_out = median_filter(img, 3);
        output_name += "_med_3";
        break;
    case 12:
        std::cout << ">> Processing median filter (size 7) on the input image." << std::endl;
        img_out = median_filter(img, 7);
        output_name += "_med_7";
        break;
    case 13:
        std::cout << ">> Processing median filter (size 11) on the input image." << std::endl;
        img_out = median_filter(img, 11);
        output_name += "_med_11";
        break;
    case 14:
        std::cout << ">> Processing sobel filter on the input image." << std::endl;
        img_out = sobel_filter(img);
        output_name += "_sobel";
        break;
    case 15:
        std::cout << ">> Processing the negative of input image (RGB)." << std::endl;
        img_out = negative_rgb(img);
        output_name += "_negative_rgb";
        break;
    case 16:
        std::cout << ">> Processing the negative of input image (Y)." << std::endl;
        img_out = negative_y(img);
        output_name += "_negative_y";
        break;
    case 17:
        std::cout << ">> Thresholding input image (50)." << std::endl;
        img_out = thresholding(img, 50, 0, false);
        output_name += "_thres_50";
        break;
    case 18:
        std::cout << ">> Thresholding input image (120)." << std::endl;
        img_out = thresholding(img, 120, 0, false);
        output_name += "_thres_120";
        break;
    case 19:
        std::cout << ">> Thresholding input image (200)." << std::endl;
        img_out = thresholding(img, 200, 0, false);
        output_name += "_thres_200";
        break;
    case 20:
        std::cout << ">> Thresholding input image (230)." << std::endl;
        img_out = thresholding(img, 230, 0, false);
        output_name += "_thres_230";
        break;
    case 21:
        std::cout << ">> Thresholding input image (Y channel mean value)." << std::endl;
        img_out = thresholding(img, 0, 0, true);
        output_name += "_thres_mean";
        break;
    case 22:
        std::cout << ">> Adding brightness to image (30)." << std::endl;
        img_out = add_brightness(img, 30);
        output_name += "_brgt_add30";
        break;
    case 23:
        std::cout << ">> Adding brightness to image (80)." << std::endl;
        img_out = add_brightness(img, 80);
        output_name += "_brgt_add100";
        break;
    case 24:
        std::cout << ">> Adding brightness to image (-100)." << std::endl;
        img_out = add_brightness(img, -100);
        output_name += "_brgt_add-100";
        break;
    case 25:
        std::cout << ">> Adding brightness to image (-150)." << std::endl;
        img_out = add_brightness(img, -150);
        output_name += "_brgt_add-150";
        break;
    case 26:
        std::cout << ">> [RGB] Multiplying brightness of image by 0.5." << std::endl;
        img_out = mul_brightness_rgb(img, 0.5f);    
        output_name += "_brgt_mul_rgb-50";
        break;
    case 27:
        std::cout << ">> [RGB] Multiplying brightness of image by 1.5." << std::endl;
        img_out = mul_brightness_rgb(img, 1.5f);
        output_name += "_brgt_mul_rgb+50";
        break;
    case 28:
        std::cout << ">> [Y-channel] Multiplying brightness of image by 0.5." << std::endl;
        img_out = mul_brightness_y(img, 0.5f);
        output_name += "_brgt_mul_y-50";
        break;
    case 29:
        std::cout << ">> [Y-channel] Multiplying brightness of image by 1.5." << std::endl;
        img_out = mul_brightness_y(img, 1.5f);
        output_name += "_brgt_mul_y+50";
        break;
    }

    output_name += ".png";
    imwrite(output_name, img_out);
    std::cout << "Output image saved to: " + output_name << std::endl;

    return 0;
}
