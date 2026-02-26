#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

#include "gpu/maxtree.hpp"
#include "common/opencv.hpp"

namespace gpu
{
    namespace filter
    {
        void computeAreaAttribute(int* d_areaAttribute, int* d_parent, int width, int height);
        void applyFilter(uchar* d_pixels, int* d_parent, int* d_areaAttribute, int threshold, uchar* d_out, int width, int height);
        cv::Mat filterOnArea(const gpu::maxtree::MaxTree& mt, int threshold);
    }
}