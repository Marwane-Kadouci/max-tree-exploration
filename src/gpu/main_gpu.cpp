#include <opencv2/opencv.hpp>
#include <vector>

#include "gpu/maxtree.hpp"
#include "gpu/display.hpp"
#include "gpu/filter.hpp"
#include "opencv.hpp"

int main() {    
    // unsigned char donnees[] = { 
    //     15, 13,
    //     12, 14 
    // };
    // cv::Mat image(2, 2, CV_8UC1, donnees);

    // unsigned char donnees[] = { 
    //     15,
    //     12
    // };
    // cv::Mat image(1, 2, CV_8UC1, donnees);
  
    // unsigned char donnees[] = { 
    //     15, 13, 16,
    //     12, 12, 10,
    //     16, 12, 14     
    // };
    // cv::Mat image(3, 3, CV_8UC1, donnees);

    unsigned char donnees[] = { 
        15, 13, 16,
        12, 12, 10,
        16, 12, 14,
        15, 13, 16,
        12, 12, 10,
        16, 12, 14,
        15, 13, 16,
        12, 12, 10,
        16, 12, 14,
        15, 13, 16,
        12, 12, 10,
        16, 12, 14,
        
    };
    cv::Mat image(6, 6, CV_8UC1, donnees);

    // auto mt = gpu::maxtree::MaxTree(image);

    auto mt = gpu::maxtree::MaxTree("tests/images/cameraman.png");
    mt.initMaxTree();

    // gpu::display::generateDotFile("tests/output/gpu.dot", mt.parentImage, mt.width, mt.height, mt);
    
    cv::Mat outputImage = gpu::filter::filterOnArea(mt, 5000);
    
    myopencv::saveImage(outputImage, "tests/output/gpuOutput.png");

    return 0;
}