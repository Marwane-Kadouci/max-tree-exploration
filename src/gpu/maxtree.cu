#include <iostream>
#include <numeric>

#include "gpu/maxtree.hpp"

namespace gpu
{
    namespace maxtree
    {
        namespace device
        {
            __device__ __host__ int getPixel(int p, uchar* d_pixels)
            {
                return d_pixels[p];
            }

            __device__ void swap(int& a, int& b)
            {
                int tmp = a;
                a = b,
                b = tmp;
            }

            __device__ RootResult findPeakRoot(int* d_parent, int x, int lvl, uchar* d_pixels)
            {
                int q = d_parent[x];
                int old;
                while (q != -1 && q != x && lvl <= getPixel(q, d_pixels)) {
                    old = q;
                    q = d_parent[q];
                    x = old;
                }
                return {x, q};
            }

            __device__ RootResult findLevelRoot(int* d_parent, int x, uchar* d_pixels)
            {
                return findPeakRoot(d_parent, x, getPixel(x, d_pixels), d_pixels);
            }

            __device__ void connect(int pixelA, int pixelB, uchar* d_pixels, int* d_parent)
            {
                while (pixelB != -1) {
                    if (getPixel(pixelB, d_pixels) < getPixel(pixelA, d_pixels)) {
                        swap(pixelA, pixelB);
                    }

                    RootResult resA = findLevelRoot(d_parent, pixelA, d_pixels);
                    pixelA = resA.root;
                    int parentA = resA.rootParent;
                    
                    RootResult resB = findPeakRoot(d_parent, pixelB, getPixel(pixelA, d_pixels), d_pixels);
                    pixelB = resB.root;
                    int parentB = resB.rootParent;
        
                    int valueA = getPixel(pixelA, d_pixels);
                    int valueB = getPixel(pixelB, d_pixels);
                    if (pixelB == -1 or (valueB < valueA || (valueB == valueA && pixelB > pixelA)))
                    {
                        swap(pixelA, pixelB);
                        swap(parentA, parentB);
                    }
                    
                    if (pixelA == pixelB) {
                        return;
                    }

                    int old = atomicCAS(&d_parent[pixelB], parentB, pixelA);

                    if (old == parentB) {
                        pixelB = parentB;
                    }
                }

            }

            __global__ void kernelMaxTree(uchar* d_pixels, int width, int height, int* d_parent)
            {
                int index = blockIdx.x * blockDim.x + threadIdx.x;
                int gridSize = gridDim.x * blockDim.x;

                int row;
                int col;

                for (int p = index; p < width * height; p += gridSize) {
                    row = p / width;
                    col = p % width;
                    if (col + 1 < width)
                    {
                        connect(p, p + 1, d_pixels, d_parent);
                    }
                    if (row + 1 < height)
                    {
                        connect(p, p + width, d_pixels, d_parent);
                    }
                }
                
                return;
            }
        }

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
            try
            {
                image = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
            }
            catch (const std::exception &exc)
            {
                std::cout << exc.what() << std::endl;
            }
            if (image.empty())
            {
                throw std::runtime_error("File not found.");
            }
            initAttributes(image);
        }

        int MaxTree::getPixel(int p)
        {
            int row = p / width;
            int col = p % width;
            return image.at<uchar>(row, col);
        }

        RootResult MaxTree::findPeakRoot(int x, int lvl)
        {
            int q = parentImage[x];
            int old;
            while (q != -1 && q != x && lvl <= getPixel(q))
            {
                old = q;
                q = parentImage[q];
                x = old;
            }
            return {x, q};
        }

        RootResult MaxTree::findLevelRoot(int x)
        {
            return findPeakRoot(x, getPixel(x));
        }

        void MaxTree::flatten()
        {
            int q;
            for (int p{}; p < width * height; p++)
            {
                q = parentImage[p];
                RootResult rootResult = findLevelRoot(q);
                parentImage[p] = rootResult.root;
            }
        }

        void MaxTree::initMaxTree() {
            auto pixelComp = [&](int a, int b)
            {
                auto grayA = static_cast<int>(image.at<uchar>(a / width, a % width));
                auto grayB = static_cast<int>(image.at<uchar>(b / width, b % width));
                return grayA < grayB;
            }; 
            std::sort(orderingArray.begin(), orderingArray.end(), pixelComp);

            size_t dataSize = width * height;

            uchar* d_pixels;
            cudaMalloc(&d_pixels, dataSize * sizeof(uchar));
            cudaMemcpy(d_pixels, image.data, dataSize, cudaMemcpyHostToDevice);

            int* d_parent;
            cudaMalloc(&d_parent, dataSize * sizeof(int));
            cudaMemset(d_parent, -1, dataSize * sizeof(int));

            device::kernelMaxTree<<<100, 100>>>(d_pixels, width, height, d_parent);
            cudaDeviceSynchronize();
            
            cudaMemcpy(parentImage.data(), d_parent, dataSize * sizeof(int), cudaMemcpyDeviceToHost);

            cudaFree(d_pixels);
            cudaFree(d_parent);
            
            flatten();
        }
    }
}