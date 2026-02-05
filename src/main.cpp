#include <opencv2/opencv.hpp>
#include <vector>

#include "cpu/maxtree.hpp"
#include "cpu/display.hpp"
#include "cpu/filter.hpp"
#include "opencv.hpp"

int main() {
    auto mt = cpu::maxtree::MaxTree( "tests/images/cameraman.png" );
    mt.initMaxTree();

    std::vector<int> areaAttribute = cpu::filter::computeArea(mt);
    std::vector<int> out = cpu::filter::applyFilter(mt, areaAttribute, 5000);

    cv::Mat outputImage = myopencv::arrayToImage(out, mt.width, mt.height);
    cpu::display::showImage(outputImage);
    myopencv::saveImage(outputImage, "tests/output/filteredImage.png");
}