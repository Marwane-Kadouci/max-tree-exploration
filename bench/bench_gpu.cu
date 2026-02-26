#include <benchmark/benchmark.h>
#include <iostream>

#include "gpu/maxtree.hpp"
#include "gpu/display.hpp"
#include "gpu/filter.hpp"
#include "common/opencv.hpp"

void BM_BuildMaxtree(benchmark::State& state)
{
    auto image = "tests/images/cameraman.png";
    for (auto _ : state)
    {
        auto mt = gpu::maxtree::MaxTree(image);
        mt.initMaxTree();
    }
    return;
}

void BM_ComputeArea(benchmark::State& state)
{
    auto image = "tests/images/cameraman.png";
    auto mt = gpu::maxtree::MaxTree(image);
    mt.initMaxTree();

    size_t dataSize = mt.width * mt.height;
    
    int* d_areaAttribute;
    cudaMalloc(&d_areaAttribute, dataSize * sizeof(int));
    cudaMemset(d_areaAttribute, 0, dataSize * sizeof(int));

    int* d_parent;
    cudaMalloc(&d_parent, dataSize * sizeof(int));
    cudaMemcpy(d_parent, mt.parentImage.data(), dataSize * sizeof(int), cudaMemcpyHostToDevice);
        
    for (auto _ : state)
    {
        gpu::filter::computeAreaAttribute(d_areaAttribute, d_parent, mt.width, mt.height);
    }

    cudaFree(d_areaAttribute);
    cudaFree(d_parent);
    return;
}

void BM_Filter(benchmark::State& state)
{
    auto image = "tests/images/cameraman.png";
    auto mt = gpu::maxtree::MaxTree(image);
    mt.initMaxTree();
    
    size_t dataSize = mt.width * mt.height;
    
    uchar* d_out;
    cudaMalloc(&d_out, dataSize * sizeof(uchar));
    cudaMemcpy(d_out, mt.image.data, dataSize * sizeof(uchar), cudaMemcpyHostToDevice);
    
    uchar* d_pixels;
    cudaMalloc(&d_pixels, dataSize * sizeof(uchar));
    cudaMemcpy(d_pixels, mt.image.data, dataSize * sizeof(uchar), cudaMemcpyHostToDevice);
    
    int* d_areaAttribute;
    cudaMalloc(&d_areaAttribute, dataSize * sizeof(int));
    cudaMemset(d_areaAttribute, 0, dataSize * sizeof(int));

    int* d_parent;
    cudaMalloc(&d_parent, dataSize * sizeof(int));
    cudaMemcpy(d_parent, mt.parentImage.data(), dataSize * sizeof(int), cudaMemcpyHostToDevice);

    gpu::filter::computeAreaAttribute(d_areaAttribute, d_parent, mt.width, mt.height);

    for (auto _ : state)
    { 
        gpu::filter::applyFilter(d_pixels, d_parent, d_areaAttribute, 5000, d_out, mt.width, mt.height);
    }

    cudaFree(d_out);
    cudaFree(d_pixels);
    cudaFree(d_parent);
    cudaFree(d_areaAttribute);
    return;
}

BENCHMARK(BM_BuildMaxtree);
BENCHMARK(BM_ComputeArea);
BENCHMARK(BM_Filter);
BENCHMARK_MAIN();