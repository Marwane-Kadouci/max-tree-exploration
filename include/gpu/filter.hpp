#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

#include "gpu/maxtree.hpp"
#include "opencv.hpp"

namespace gpu
{
    namespace filter
    {
        std::vector<int> computeArea(const gpu::maxtree::MaxTree& mt);
        cv::Mat filterOnArea(const gpu::maxtree::MaxTree& mt, int threshold);
    }
}