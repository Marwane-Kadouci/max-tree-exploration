#pragma once

#include <vector>

#include "cpu/maxtree.hpp"

namespace cpu
{
    namespace filterTreeContractions
    {
        using Edge = std::pair<int, int>;
        using EdgeSet = std::vector<Edge>;
        using MergeList = std::vector<EdgeSet>;

        struct RootfixResult {
            MergeList merge_first;
            MergeList merge_last;
        };

        RootfixResult rootfixMergeList(const cpu::maxtree::MaxTree& mt); 
        std::vector<int> applyFilter(const cpu::maxtree::MaxTree& mt, std::vector<int> attribute, int threshold);
    }
}