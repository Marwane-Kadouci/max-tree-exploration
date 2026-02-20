#pragma once

namespace myopencv
{
    cv::Mat arrayToImage(const std::vector<int>& array, int width, int height);
    void showImage(cv::Mat image);
    void saveImage(cv::Mat image, std::string path);
}