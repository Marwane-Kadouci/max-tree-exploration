#include <numeric>

#include "maxtree/core.hpp"

namespace maxtree
{
    namespace core
    {
        void MaxTree::initAttributes(cv::Mat image)
        {
            width = image.cols;
            height = image.rows;

            parentImage.assign(width * height, -1);
            orderingArray.resize(width * height);
            std::iota(orderingArray.begin(), orderingArray.end(), 0);
        }

        MaxTree::MaxTree(cv::Mat image): image(image)
        {
            initAttributes(image);
        }

        MaxTree::MaxTree(std::string imagePath)
        {
            image = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
            if (image.empty())
            {
                throw std::runtime_error("File not found.");
            }
            initAttributes(image);
        }

        void MaxTree::makeSet(std::vector<int> &par, std::vector<int> &zpar, int p)
        {
            par[p] = p;
            zpar[p] = p;
        }

        int MaxTree::findRoot(std::vector<int> &par, int p)
        {
            if (par[p] != p)
            {
                par[p] = findRoot(par, par[p]);
            }
            return par[p];
        }

        void MaxTree::mergeSet(std::vector<int> &par, int r, int p)
        {
            par[r] = p;
        }

        int MaxTree::getPixel(int p)
        {
            int row = p / width;
            int col = p % width;
            return image.at<uchar>(row, col);
        }

        void MaxTree::canonicalize(cv::Mat &f, std::vector<int> &parent, std::vector<int> &S)
        {
            int q;
            for (auto p : S)
            {
                q = parent[p];
                if (getPixel(q) == getPixel(parent[q]))
                {
                    parent[p] = parent[q];
                }
            }
        }

        void MaxTree::setNeighbors(std::vector<int>& neighbors, int pixelIndex)
        {
            neighbors.clear();
            int row = pixelIndex / width;
            int col = pixelIndex % width;
            if (row > 0)
            {
                neighbors.push_back((row - 1) * width + col);
            }
            if (col > 0)
            {
                neighbors.push_back(row * width + col - 1);
            }
            if (row < height - 1)
            {
                neighbors.push_back((row + 1) * width + col);
            }
            if (col < width - 1)
            {
                neighbors.push_back(row * width + col + 1);
            }
        }

        void MaxTree::initMaxTree()
        {
            auto pixelComp = [&](int a, int b)
            {
                auto grayA = static_cast<int>(image.at<uchar>(a / width, a % width));
                auto grayB = static_cast<int>(image.at<uchar>(b / width, b % width));
                return grayA < grayB;
            };
            
            std::sort(orderingArray.begin(), orderingArray.end(), pixelComp);
            std::vector<int> neighbors;

            std::vector<int> zpar(width * height, -1);

            for (auto p = orderingArray.rbegin(); p != orderingArray.rend(); p++)
            {
                int pixelIndex = *p;
                
                makeSet(parentImage, zpar, pixelIndex);
        
                setNeighbors(neighbors, pixelIndex);
                int r;
                for (auto n : neighbors)
                {
                    if (parentImage[n] != -1)
                    {
                        r = findRoot(zpar, n);
                        if (r != pixelIndex)
                        {
                            mergeSet(zpar, r, pixelIndex);
                            mergeSet(parentImage, r, pixelIndex);                        
                        }
                    }
                }
            }
            canonicalize(image, parentImage, orderingArray);
        }
    }
}