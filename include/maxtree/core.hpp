#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <tuple>
#include <vector>

namespace maxtree
{
    namespace core
    {
        class MaxTree {
        public:
            MaxTree(cv::Mat image);
            MaxTree(std::string imagePath);
            ~MaxTree() = default;
            void initMaxTree();
            int getPixel(int p);

        // private:
            void initAttributes(cv::Mat image);
            void makeSet(std::vector<int> &par, std::vector<int> &zpar, int p);
            int findRoot(std::vector<int> &par, int p);
            void mergeSet(std::vector<int> &par, int r, int p);
            void canonicalize(cv::Mat &f, std::vector<int> &parent, std::vector<int> &S);
            void setNeighbors(std::vector<int>& neighbors, int pixelIndex);

            cv::Mat image;
            int width;
            int height;

            std::vector<int> parentImage;
            std::vector<int> orderingArray;
        };
    }
}
