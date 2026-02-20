#include "gpu/filter.hpp"

namespace gpu
{
    namespace filter
    {
        namespace device
        {
            __global__ void applyFilter(uchar* d_pixels, int* d_parent, int* d_attribute, int threshold, uchar* d_out, int width, int height)
            {
                int index = blockIdx.x * blockDim.x + threadIdx.x;
                int gridSize = gridDim.x * blockDim.x;

                int q;
                for (int p{index}; p < width * height; p += gridSize) {
                    q = p;
                    while (d_attribute[q] < threshold && q != d_parent[q])
                    {
                        q = d_parent[q];
                    }
                    d_out[p] = d_pixels[q];
                }
            }
        }

        std::vector<int> computeArea(const gpu::maxtree::MaxTree& mt)
        {
            std::vector<int> attribute(mt.width * mt.height);
            int pRoot = mt.orderingArray[0];

            for (auto p : mt.orderingArray)
            {
                attribute[p] = 1;
            }

            int q;
            int pixelIndex;
            for (auto p = mt.orderingArray.rbegin(); p != mt.orderingArray.rend(); p++)
            {
                pixelIndex = *p;
                if (pixelIndex != pRoot)
                {
                    q = mt.parentImage[pixelIndex];
                    attribute[q] += attribute[pixelIndex];
                }
            }

            return attribute;
        }

        cv::Mat filterOnArea(const gpu::maxtree::MaxTree& mt, int threshold)
        {
            std::vector<int> areaAttribute = gpu::filter::computeArea(mt); 

            size_t dataSize = mt.width * mt.height;
            
            uchar* d_out;
            cudaMalloc(&d_out, dataSize * sizeof(uchar));
            cudaMemcpy(d_out, mt.image.data, dataSize * sizeof(uchar), cudaMemcpyHostToDevice);
            
            int* d_attribute;
            cudaMalloc(&d_attribute, dataSize * sizeof(int));
            cudaMemcpy(d_attribute, areaAttribute.data(), dataSize * sizeof(int), cudaMemcpyHostToDevice);

            uchar* d_pixels;
            cudaMalloc(&d_pixels, dataSize * sizeof(uchar));
            cudaMemcpy(d_pixels, mt.image.data, dataSize * sizeof(uchar), cudaMemcpyHostToDevice);

            int* d_parent;
            cudaMalloc(&d_parent, dataSize * sizeof(int));
            cudaMemcpy(d_parent, mt.parentImage.data(), dataSize * sizeof(int), cudaMemcpyHostToDevice);
             
            device::applyFilter<<<1,1>>>(d_pixels, d_parent, d_attribute, threshold, d_out, mt.width, mt.height);
            cudaDeviceSynchronize();

            uchar* h_out = (uchar*)malloc(dataSize * sizeof(uchar));
            cudaMemcpy(h_out, d_out, dataSize * sizeof(uchar), cudaMemcpyDeviceToHost);
         

            cudaFree(d_out);
            cudaFree(d_attribute);
            cudaFree(d_pixels);
            cudaFree(d_parent);
            
            auto res = std::vector<int>(h_out, h_out + dataSize);
            free(h_out);
    
            return myopencv::arrayToImage(res, mt.width, mt.height);
        }
    }
}