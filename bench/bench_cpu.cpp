#include <benchmark/benchmark.h>
#include <iostream>

#include "cpu/maxtree.hpp"
#include "cpu/display.hpp"
#include "cpu/filter.hpp"
#include "common/opencv.hpp"

void BM_BuildMaxtree(benchmark::State& state)
{
    auto image = "tests/images/cameraman.png";
    for (auto _ : state)
    {
        auto mt = cpu::maxtree::MaxTree(image); 
        mt.initMaxTree();
    }
    return;
}

void BM_ComputeArea(benchmark::State& state)
{
    auto image = "tests/images/cameraman.png";
    auto mt = cpu::maxtree::MaxTree(image); 
    mt.initMaxTree();

    for (auto _ : state)
    {
        cpu::filter::computeArea(mt);
    }
    return;
}

void BM_Filter(benchmark::State& state)
{
    auto image = "tests/images/cameraman.png";
    auto mt = cpu::maxtree::MaxTree(image); 
    mt.initMaxTree();

    std::vector<int> areaAttribute = cpu::filter::computeArea(mt);

    for (auto _ : state)
    {
        cpu::filter::applyFilter(mt, areaAttribute, 5000);
    }
    return;
}

BENCHMARK(BM_BuildMaxtree);
BENCHMARK(BM_ComputeArea);
BENCHMARK(BM_Filter);
BENCHMARK_MAIN();