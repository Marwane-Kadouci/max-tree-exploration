#include <iostream>
#include <opencv2/opencv.hpp>

#include "opencv.hpp"

namespace myopencv
{
    cv::Mat arrayToImage(std::vector<int> array, int width, int height)
    {
        cv::Mat image(height, width, CV_8UC1);
    
        for (int y = 0; y < width; ++y)
        {
            for (int x = 0; x < height; ++x)
            {
                image.at<uchar>(y, x) = array[y * width + x];
            }
        }

        return image;
    }

    void saveImage(cv::Mat image, std::string path)
    {
        cv::imwrite(path, image);
    }
}