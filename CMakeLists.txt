# Project
cmake_minimum_required(VERSION 3.21.2)
project(max-tree-exploration LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
find_package(OpenCV REQUIRED)

# Check CUDA
include(CheckLanguage)
check_language(CUDA)
if(CMAKE_CUDA_COMPILER)
    enable_language(CUDA)
    set(CUDA_DEFINED TRUE)
else()
    set(CUDA_DEFINED FALSE)
endif()

# CPU
add_executable(main_cpu)

target_link_libraries(main_cpu ${OpenCV_LIBS})

target_include_directories(main_cpu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(main_cpu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# GPU
if(CUDA_DEFINED)
    add_executable(main_gpu)
    target_link_libraries(main_gpu ${OpenCV_LIBS})
    target_include_directories(main_gpu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_include_directories(main_gpu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
endif()

# BENCH
if(TRUE) # CUDA_DEFINED)
    find_package(benchmark REQUIRED)

    add_executable(bench_cpu)
    target_link_libraries(bench_cpu benchmark::benchmark)
    target_link_libraries(bench_cpu ${OpenCV_LIBS})
    target_include_directories(bench_cpu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_include_directories(bench_cpu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_include_directories(bench_cpu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/bench/include)

    add_executable(bench_gpu)
    target_link_libraries(bench_gpu benchmark::benchmark)
    target_link_libraries(bench_gpu ${OpenCV_LIBS})
    target_include_directories(bench_gpu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_include_directories(bench_gpu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_include_directories(bench_gpu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/bench/include)
    
    add_subdirectory(bench)
endif()

#
add_subdirectory(src)