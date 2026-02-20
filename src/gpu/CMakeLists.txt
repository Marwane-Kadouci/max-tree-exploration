if(CUDA_DEFINED)
    target_sources(main_gpu PRIVATE main_gpu.cpp maxtree.cu filter.cu display.cpp)
endif()