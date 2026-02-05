#pragma once

#include <vector>

#include "cpu/maxtree.hpp"

namespace cpu
{
    namespace filter
    {
        std::vector<int> computeArea(cpu::maxtree::MaxTree mt);
        std::vector<int> applyFilter(cpu::maxtree::MaxTree mt, std::vector<int> attribute, int threshold);
    }
}