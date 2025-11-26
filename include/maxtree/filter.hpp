#pragma once

#include <vector>

#include "maxtree/core.hpp"

namespace maxtree
{
    namespace filter
    {
        std::vector<int> computeArea(maxtree::core::MaxTree mt);
        std::vector<int> applyFilter(maxtree::core::MaxTree mt, std::vector<int> attribute, int threshold);
    }
}