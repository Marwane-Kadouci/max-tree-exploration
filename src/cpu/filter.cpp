#include "cpu/filter.hpp"

namespace cpu
{
    namespace filter
    {
        std::vector<int> computeArea(cpu::maxtree::MaxTree mt)
        {
            std::vector<int> attribute(mt.width * mt.height);
            int pRoot = mt.orderingArray[0];
            
            for (auto p : mt.orderingArray)
            {
                attribute[p] = 1;
            }

            int q;
            int pixelIndex;
            for (auto p = mt.orderingArray.rbegin(); p != mt.orderingArray.rend(); p++)
            {
                pixelIndex = *p;
                if (pixelIndex != pRoot)
                {
                    q = mt.parentImage[pixelIndex];
                    attribute[q] += attribute[pixelIndex];
                }
            }

            return attribute;
        }

        std::vector<int> applyFilter(cpu::maxtree::MaxTree mt, std::vector<int> attribute, int threshold)
        {
            std::vector<int> out(mt.width * mt.height, -1);
            int pRoot = mt.orderingArray[0];
            if (attribute[pRoot] < threshold)
            {
                out[pRoot] = 0;
            }
            else
            {
                out[pRoot] = mt.getPixel(pRoot);
            }
            int q;
            for (auto p : mt.orderingArray)
            {
                q = mt.parentImage[p];
                if (mt.getPixel(q) == mt.getPixel(p))
                {
                    out[p] = out[q];
                }
                else if (attribute[p] < threshold)
                {
                    out[p] = out[q];
                }
                else
                {
                    out[p] = mt.getPixel(p);
                }
            }
            return out;
        }
    }
}