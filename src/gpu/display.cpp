#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "gpu/display.hpp"

namespace gpu
{
    namespace display
    {
        void showImage(cv::Mat image)
        {
            cv::namedWindow("Show", cv::WINDOW_AUTOSIZE);
            cv::imshow("Show", image);
            std::cout << "Press a key to close the window.\n";
            cv::waitKey(0);
        }

        void printImage(cv::Mat image, int width, int height)
        {
            std::cout << "image:" << std::endl;
            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    std::cout << std::setw(width) << static_cast<int>(image.at<uchar>(y, x)) << (x + 1 == width ? '\n' : ' ');
                }
            }
            std::cout << std::endl;
        }

        void printParentImage(std::vector<int> parentImage, int width, int height)
        {
            std::cout << "parentImage:" << std::endl;
            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    int idx = y * width + x;
                    std::cout << std::setw(width) << parentImage[idx] << (x + 1 == width ? '\n' : ' ');
                }
            }
            std::cout << std::endl;
        }

        void printS(std::vector<int> orderingArray)
        {
            std::cout << "S: ";
            for (int pixelIndex : orderingArray)
            {
                std::cout << pixelIndex << ' ';
            }
            std::cout << std::endl;
        }

        void printMaxTree(gpu::maxtree::MaxTree mt)
        {
            cv::Mat image = mt.image;
            std::vector<int> parentImage = mt.parentImage;
            std::vector<int> orderingArray = mt.orderingArray;
            int width = mt.width;
            int height = mt.height;
            printImage(image, width, height);
            printParentImage(parentImage, width, height);
            printS(orderingArray);
        }

        void generateDotFile(const std::string& filename, std::vector<int> parent, int width, int height, gpu::maxtree::MaxTree  mt)
        {
            std::ofstream dotFile(filename);
            if (!dotFile.is_open()) return;

            dotFile << "digraph MaxTree {" << std::endl;
            dotFile << "    node [shape=circle];" << std::endl;
            dotFile << "    label=\"GPU MaxTree\";" << std::endl;

            int numPixels = width * height;

            for (int i = 0; i < numPixels; ++i) {
                dotFile << "    " << i << "[label= \"" << mt.getPixel(i) << "\"";

                if (parent[i] == i)
                {
                    dotFile << ", color=red, style=filled";
                }
                dotFile << "]" << ";" << std::endl;
            }
            for (int i = 0; i < numPixels; ++i) {
                int p = parent[i];

                if (p != -1 && p != i) {
                    dotFile << "    " << p << " -> " << i << ";\n";
                }
            }

            dotFile << "}\n";
            dotFile.close();
        }
    }
}