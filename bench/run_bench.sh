#!/bin/sh

# To be executed from the project root.

mkdir bench/results

cmake --build build --target bench_cpu bench_gpu

./build/bench_cpu --benchmark_format=json --benchmark_out=bench/results/cpu.json
./build/bench_gpu --benchmark_format=json --benchmark_out=bench/results/gpu.json

uv run bench/plot.py bench/results/cpu.json bench/results/gpu.json