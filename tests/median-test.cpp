#include "heap/median.hpp"
#include "heap/d-ary.hpp"
#include "gtest/gtest.h"


TEST(MedianHeapTest, insertion){

    MedianHeap<BinaryMinHeap<int>, BinaryHeap<int>> h;
    int keys[11]    = {50, 40, 30, 20, 10, 60, 55, 35, 38, 65, 5};
    int medians[11] = {50, 40, 40, 30, 30, 30, 40, 35, 38, 38, 38};
    for (size_t i= 0; i < 11; ++i){
        h.insert(keys[i]);
        ASSERT_EQ(h.findMedian(), medians[i]);
    }
}

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}