#include <opencv2/opencv.hpp>
#include <vector>

#include "cpu/maxtree.hpp"
#include "cpu/display.hpp"
#include "cpu/filter.hpp"
#include "opencv.hpp"

int main() {
    // unsigned char donnees[] = { 
    //     15, 13, 16,
    //     12, 12, 10,
    //     16, 12, 14 
    // };
    // cv::Mat image(3, 3, CV_8UC1, donnees);
    
    // unsigned char donnees[] = { 
    //     15, 13,
    //     12, 14 
    // };
    // cv::Mat image(2, 2, CV_8UC1, donnees);

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
    
    // auto mt = cpu::maxtree::MaxTree(image);

    auto mt = cpu::maxtree::MaxTree("tests/images/cameraman.png");
    
    mt.initMaxTree();
    
    // cpu::display::generateDotFile("tests/output/cpu.dot", mt.parentImage, mt.width, mt.height, mt);

    std::vector<int> areaAttribute = cpu::filter::computeArea(mt);
    std::vector<int> out = cpu::filter::applyFilter(mt, areaAttribute, 5000);

    cv::Mat outputImage = myopencv::arrayToImage(out, mt.width, mt.height);
    // cpu::display::showImage(outputImage);
    myopencv::saveImage(outputImage, "tests/output/filteredImage.png");
}