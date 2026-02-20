#pragma once

#include <string>

#include "gpu/maxtree.hpp"

namespace gpu
{
    namespace display
    {
        void showImage(cv::Mat image);
        void printImage(cv::Mat image, int width, int height);
        void printParentImage(std::vector<int> parentImage, int width, int height);
        void printS(std::vector<int> orderingArray);
        void printMaxTree(gpu::maxtree::MaxTree mt);
        void generateDotFile(const std::string& filename, std::vector<int> parent, int width, int height, gpu::maxtree::MaxTree mt);
    }
}