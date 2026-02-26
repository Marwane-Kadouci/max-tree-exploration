#include "gpu/filter.hpp"

namespace gpu
{
    namespace filter
    {
        namespace device
        {
            __global__ void computeAreaAttribute(int* d_areaAttribute, int* d_parent, int width, int height)
            {
                int index = blockIdx.x * blockDim.x + threadIdx.x;
                int gridSize = gridDim.x * blockDim.x;

                for (int p{index}; p < width * height; p += gridSize)
                {
                    atomicAdd(&d_areaAttribute[p], 1);
                    int old = p;
                    int q = d_parent[old];
                    while (old != q)
                    {
                        atomicAdd(&d_areaAttribute[q], 1);
                        old = q;
                        q = d_parent[old];
                    }
                }
            }

            __global__ void applyFilter(uchar* d_pixels, int* d_parent, int* d_areaAttribute, int threshold, uchar* d_out, int width, int height)
            {
                int index = blockIdx.x * blockDim.x + threadIdx.x;
                int gridSize = gridDim.x * blockDim.x;

                int q;
                for (int p{index}; p < width * height; p += gridSize) {
                    q = p;
                    while (d_areaAttribute[q] < threshold && q != d_parent[q])
                    {
                        q = d_parent[q];
                    }
                    d_out[p] = d_pixels[q];
                }
            }
        }

        void computeAreaAttribute(int* d_areaAttribute, int* d_parent, int width, int height)
        {
            device::computeAreaAttribute<<<1024,1024>>>(d_areaAttribute, d_parent, width, height);
            cudaDeviceSynchronize();
        }

        void applyFilter(uchar* d_pixels, int* d_parent, int* d_areaAttribute, int threshold, uchar* d_out, int width, int height)
        {
            device::applyFilter<<<1024,1024>>>(d_pixels, d_parent, d_areaAttribute, threshold, d_out, width, height);
            cudaDeviceSynchronize();
        }

        cv::Mat filterOnArea(const gpu::maxtree::MaxTree& mt, int threshold)
        {
            size_t dataSize = mt.width * mt.height;
            
            uchar* d_out;
            cudaMalloc(&d_out, dataSize * sizeof(uchar));
            cudaMemcpy(d_out, mt.image.data, dataSize * sizeof(uchar), cudaMemcpyHostToDevice);
            
            int* d_areaAttribute;
            cudaMalloc(&d_areaAttribute, dataSize * sizeof(int));
            cudaMemset(d_areaAttribute, 0, dataSize * sizeof(int));

            uchar* d_pixels;
            cudaMalloc(&d_pixels, dataSize * sizeof(uchar));
            cudaMemcpy(d_pixels, mt.image.data, dataSize * sizeof(uchar), cudaMemcpyHostToDevice);

            int* d_parent;
            cudaMalloc(&d_parent, dataSize * sizeof(int));
            cudaMemcpy(d_parent, mt.parentImage.data(), dataSize * sizeof(int), cudaMemcpyHostToDevice);
             
            computeAreaAttribute(d_areaAttribute, d_parent, mt.width, mt.height);
            applyFilter(d_pixels, d_parent, d_areaAttribute, threshold, d_out, mt.width, mt.height);

            uchar* h_out = (uchar*)malloc(dataSize * sizeof(uchar));
            cudaMemcpy(h_out, d_out, dataSize * sizeof(uchar), cudaMemcpyDeviceToHost);
         

            cudaFree(d_out);
            cudaFree(d_areaAttribute);
            cudaFree(d_pixels);
            cudaFree(d_parent);
            
            auto res = std::vector<int>(h_out, h_out + dataSize);
            free(h_out);
    
            return myopencv::arrayToImage(res, mt.width, mt.height);
        }
    }
}