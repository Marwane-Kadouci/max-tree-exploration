#include <opencv2/opencv.hpp>
#include <vector>

#include "maxtree/core.hpp"
#include "maxtree/display.hpp"
#include "maxtree/filter.hpp"
#include "opencv.hpp"

int main() {
    auto mt = maxtree::core::MaxTree( "tests/images/cameraman.png" );
    mt.initMaxTree();

    std::vector<int> areaAttribute = maxtree::filter::computeArea(mt);
    std::vector<int> out = maxtree::filter::applyFilter(mt, areaAttribute, 5000);

    cv::Mat outputImage = myopencv::arrayToImage(out, mt.width, mt.height);
    maxtree::display::showImage(outputImage);
    myopencv::saveImage(outputImage, "tests/output/filteredImage.png");
}