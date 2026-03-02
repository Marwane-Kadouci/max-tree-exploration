#include <random>

#include "cpu/filterTreeContractions.hpp"

namespace cpu
{
    namespace filterTreeContractions
    {
        auto generateHash(uint32_t a, uint32_t b) {
            return [a, b](int p) -> bool {
                return  ((a * (p + 1) + b) >> 31);
            };
        } 

        RootfixResult rootfixMergeList(const cpu::maxtree::MaxTree& mt)
        {
            std::mt19937 rng(42);

            MergeList merge_first{};
            MergeList merge_last{};

            int count = mt.width * mt.height;

            std::vector<bool> activeNodes(mt.width * mt.height, true);
            std::vector<bool> currentMerge(mt.width * mt.height);
            std::vector<int> parentContracted(mt.parentImage);

            int iter{0};

            while (count > 1)
            {
                auto hash = generateHash(rng(), rng());
                std::fill(currentMerge.begin(), currentMerge.end(), false);
                EdgeSet first{};
                EdgeSet contracting{};

                for (int p{}; p < mt.width * mt.height; p++)
                {
                    if (activeNodes[p] && hash(p) == 1 && hash(parentContracted[p]) == 0)
                    {
                        contracting.push_back({parentContracted[p], p});
                        activeNodes[p] = false;
                        currentMerge[p] = true;
                        count--;
                    }
                }
                for (int p{}; p < mt.width * mt.height; p++)
                {
                    if (activeNodes[p] && currentMerge[parentContracted[p]])
                    {
                        first.push_back({parentContracted[p], p});
                        parentContracted[p] = parentContracted[parentContracted[p]];
                    }
                }

                if (!first.empty())
                {
                    merge_first.push_back(std::move(first));
                }
                if (!contracting.empty())
                {
                    // Pushed back to make it easy
                    // Traverse backward when processing the list
                    merge_last.push_back(std::move(contracting));
                }
            }
            
            return {merge_first, merge_last};
        }

        int z(int p, std::vector<int> attribute, int threshold, const cpu::maxtree::MaxTree& mt)
        {
            return attribute[p] > threshold ? mt.getPixel(p) : -1;
        }

        int plus(int x, int y, const std::vector<int>& out)
        {
            return std::max(out[x], out[y]);
        }

        std::vector<int> applyFilter(const cpu::maxtree::MaxTree& mt, std::vector<int> attribute, int threshold)
        {
            std::vector<int> out(mt.width * mt.height);

            auto rootfixResult = rootfixMergeList(mt);
            auto merge_first = rootfixResult.merge_first;
            auto merge_last = rootfixResult.merge_last;

            for (int p{}; p < mt.width * mt.height; p++)
            {
                out[p] = z(p, attribute, threshold, mt);
            }

            for (EdgeSet edgeSet : merge_first)
            {
                for (Edge edge : edgeSet)
                {
                    out[edge.second] = plus(edge.first, edge.second, out);
                }
            }

            for (auto edgeSet = merge_last.rbegin(); edgeSet != merge_last.rend(); ++edgeSet) {
                for (Edge edge : *edgeSet)
                {
                    out[edge.second] = plus(edge.first, edge.second, out);
                }
            }
            
            return out;
        }
    }
}