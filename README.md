# Max-Tree on GPU

## Overview

This repository implements an image processing pipeline based on the Max-Tree data structure.

A Max-Tree is a hierarchical representation of a grayscale image that organizes connected components (CC) across intensity levels, making it easier to extract and filter image structures.

Both sequential (CPU) and parallel (GPU/CUDA) implementations are provided in order to compare them.

Benchmark files are included to evaluate CPU vs GPU versions.

## Pipeline

1. Max-Tree construction  
2. Attribute computation  
3. Attribute-based filtering  
4. Reconstruction / output image

## Example

**Original :**

![Original](readmeImages/original.png)

**Area filtered (λ = 5e3) :**

![Filtered](readmeImages/filtered.png)

## Build and run

**Dependencies**:

- C++17
- CMake >= 3.21.2
- OpenCV >= 4.6.0
- Google Benchmark >= 1.8.3

**Commands**:

To run the benchmark, execute:
```bash
cmake -B build
./bench/run_bench.sh
```
The plot will be saved in `bench/plots`.

## References

- Carlinet, E., & Géraud, T. (2014).  ["A Comparative Review of Component Tree Computation Algorithms"](https://inria.hal.science/hal-01474830/file/carlinet.201X.itip.pdf).
- Berger, C., Géraud, T., Levillain, R., Widynski, N., Baillard, A., & Bertin, E. (2007). ["Effective Component Tree Computation with Application to Pattern Recognition in Astronomical Imaging"](https://www.researchgate.net/publication/4289461_Effective_Component_Tree_Computation_with_Application_to_Pattern_Recognition_in_Astronomical_Imaging).
- Blin, N., Carlinet, E., Lemaitre, F., Lacassagne, L., & Géraud, T. (2022). ["Max-tree computation
on GPUs."](https://hal.science/hal-03556296/document).
- Paul D. Teeninga and Michael H. F. Wilkinson. (2025). ["Shape Filtering and Max-Tree Attribute Computation on a GPU"](https://link.springer.com/chapter/10.1007/978-3-032-09544-2_20).
