#include "bindStructure.hpp"
#include "spatial/kdtree.hpp"
#include "spatial/intervalTree.hpp"
#include <nanobind/nanobind.h>

namespace nb = nanobind;

void bindTrees(nb::module_ m){
     // KD Tree
    bindEnum<DistanceFunction>(m, "DistanceFunction");
    template for (constexpr size_t dim : std::views::iota(2UL, 4UL)){
        bindStructure<KDTree<int, dim>>(m, std::format("IntKDTree{}D", dim));
        bindStructure<KDTree<double, dim>>(m, std::format("FloatKDTree{}D", dim));
        bindStructure<Rectangle<int, dim>>(m, std::format("IntRectangle{}D", dim));
        bindStructure<Rectangle<double, dim>>(m, std::format("FloatRectangle{}D", dim));
    }

    // Interval Tree
    bindStructure<SimpleInterval<int>>(m, "IntervalInt");
    bindStructure<IntervalTree<SimpleInterval<int>>>(m, "IntervalTreeInt");
    bindStructure<SimpleInterval<double>>(m, "IntervalFloat");
    bindStructure<IntervalTree<SimpleInterval<double>>>(m, "IntervalTreeFloat");
}
