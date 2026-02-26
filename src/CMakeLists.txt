add_subdirectory(common)
add_subdirectory(cpu)

if(CUDA_DEFINED)
    add_subdirectory(gpu)
endif()