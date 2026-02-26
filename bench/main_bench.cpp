#include <benchmark/benchmark.h>
#include <iostream>

void test()
{
    return;
}

static void BM_test(benchmark::State& state)
{
  for (auto _ : state)
  {
    test();
  }
}

BENCHMARK(BM_test);
BENCHMARK_MAIN();