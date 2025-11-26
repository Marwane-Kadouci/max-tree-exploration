#pragma once

#include <string>

#include "maxtree/core.hpp"

namespace maxtree
{
    namespace display
    {
        void showImage(cv::Mat image);
        void printImage(cv::Mat image, int width, int height);
        void printParentImage(std::vector<int> parentImage, int width, int height);
        void printS(std::vector<int> orderingArray);
        void printMaxTree(maxtree::core::MaxTree mt);
    }
}