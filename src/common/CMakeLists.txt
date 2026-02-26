target_sources(main_cpu PRIVATE opencv.cpp)
target_sources(bench_cpu PRIVATE opencv.cpp)
target_sources(bench_gpu PRIVATE opencv.cpp)

if(CUDA_DEFINED)
    target_sources(main_gpu PRIVATE opencv.cpp)
endif()