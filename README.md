# Max-Tree : Basic Implementation + Area Filtering

Small C++ implementation of a Max‑Tree class and a morphological area filter to explore the concepts in
["A Comparative Review of Component Tree Computation Algorithms"](https://inria.hal.science/hal-01474830/file/carlinet.201X.itip.pdf) (Edwin Carlinet & Thierry Géraud).


## What I implemented
This minimal sequential implementation includes:
- Max‑Tree construction using union‑find without union-by-rank.
- Area-based filtering of connected components.


## Example

**Original :**

![Original](readmeImages/original.png)

**Area filtered (λ = 5e3) :**

![Filtered](readmeImages/filtered.png)


## Build and run

#### Dependencies:
- C++20
- CMake >= 3.21.2
- OpenCV >= 4.6.0

#### Commands:
```bash
cmake -B build
cmake --build build --target main
./build/main
```

#### Notes:
Input images and area threshold can be adjusted in [`src/main.cpp`](src/main.cpp) for experimentation. Sample images for testing are provided in [`tests/images/`](tests/images).
