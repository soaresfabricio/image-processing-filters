#include "Noise.hpp"
#include "Color.hpp"
#include "Filter.hpp"
#include <string>

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        std::cout << ">> Usage: ./[this tool] [input file name] [mono | color] [filter number]" << std::endl;
        std::cout << ">> Available filters: \n\t0: \tNo filter \n\t1-3: \tR, G and B splitting to monocromatic image. \n\t4-6: \tR, G and B splitting to color image.\n\t7: \tAdd Salt and Pepper noise.\n\t8-10: \tBox filters of size 3, 7 and 11.." << std::endl;
        exit(EXIT_FAILURE);
    }

    int filter_opt = std::strtol(argv[3], NULL, 10);

    std::string file_name = std::string(argv[1]);
    std::string input_name = "./original/" + file_name;
    file_name.substr(0, file_name.length() - 4); // remove extension

    std::cout << "Input image loaded from: " + input_name << std::endl;

    int load_type = strcmp(argv[2], "mono") ? CV_LOAD_IMAGE_COLOR : CV_LOAD_IMAGE_GRAYSCALE;

    Mat img = imread(input_name, load_type);
    Mat img_out, mask;

    std::string output_name = "./processed/" + file_name;

    switch (filter_opt)
    {

    case 1:
        img_out = split(img, 0, true);
        std::cout << ">> Splitting R channel and generating monochromatic image." << std::endl;
        output_name += "_R_mono";
        break;
    case 2:
        img_out = split(img, 1, true);
        std::cout << ">> Splitting G channel and generating monochromatic image." << std::endl;
        output_name += "_G_mono";
        break;
    case 3:
        img_out = split(img, 2, true);
        std::cout << ">> Splitting B channel and generating monochromatic image." << std::endl;
        output_name += "_B_mono";
        break;
    case 4:
        img_out = split(img, 0, false);
        std::cout << ">> Splitting B channel and generating color image." << std::endl;
        output_name += "_R_tri";
        break;
    case 5:
        img_out = split(img, 1, false);
        std::cout << ">> Splitting G channel and generating color image." << std::endl;
        output_name += "_G_tri";
        break;
    case 6:
        img_out = split(img, 2, false);
        std::cout << ">> Splitting B channel and generating color image." << std::endl;
        output_name += "_B_tri";
        break;
    case 7:
        img_out = noise_saltpepper(img, 0.05f);
        std::cout << ">> Adding salt and pepper noise to input image." << std::endl;
        output_name += "_saltpepper";
        break;
    case 8:
        img_out = mean_filter(img, 3);
        output_name += "_mean_3";
        break;
    case 9:
        img_out = mean_filter(img, 7);
        output_name += "_mean_7";
        break;
    case 10:
        img_out = mean_filter(img, 11);
        output_name += "_mean_11";
        break;
    }

    output_name += ".png";
    imwrite(output_name, img_out);
    std::cout << "Output image saved to: " + output_name << std::endl;

    return 0;
}
