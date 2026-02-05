#pragma once

#include <string>

#include "cpu/maxtree.hpp"

namespace cpu
{
    namespace display
    {
        void showImage(cv::Mat image);
        void printImage(cv::Mat image, int width, int height);
        void printParentImage(std::vector<int> parentImage, int width, int height);
        void printS(std::vector<int> orderingArray);
        void printMaxTree(cpu::maxtree::MaxTree mt);
    }
}