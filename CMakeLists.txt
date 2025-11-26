cmake_minimum_required(VERSION 3.21.2)
project(max-tree-exploration)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(main)

target_link_libraries(main ${OpenCV_LIBS})

target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include )
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)

add_subdirectory(src)