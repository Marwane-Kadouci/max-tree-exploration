#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <tuple>
#include <vector>

namespace gpu
{
    namespace maxtree
    {
        struct RootResult
        {
            int root;
            int rootParent;
        };

        class MaxTree
        {
        public:
            MaxTree(cv::Mat image);
            MaxTree(std::string imagePath);
            ~MaxTree() = default;
            void initMaxTree();
            int getPixel(int p);

            cv::Mat image;
            int width;
            int height;

            std::vector<int> parentImage;
            std::vector<int> orderingArray;

        private:
            void initAttributes(cv::Mat image);
            RootResult findPeakRoot(int x, int lvl);
            RootResult findLevelRoot(int x);
            void flatten();
        };
    }
}
