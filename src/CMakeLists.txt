target_sources(main_cpu PRIVATE main.cpp opencv.cpp)
add_subdirectory(cpu)

if(CUDA_DEFINED)
    target_sources(main_gpu PRIVATE opencv.cpp)
    add_subdirectory(gpu)
endif()