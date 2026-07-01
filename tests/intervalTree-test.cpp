#include "spatial/intervalTree.hpp"
#include <iostream>
#include <gtest/gtest.h>


class IntervalTreeTest : public ::testing::Test {

    protected:
    IntervalTree<int> itree_;

    void SetUp() override {
        int min = std::numeric_limits<int>::min();
        int max = std::numeric_limits<int>::max();
        int lowerBounds[] = {10, 15, 18, 12, 21, 9, 17, 30, 19, 12, min};
        int upperBounds[] = {20, 25, 22, 22,23,15,20,40,20,13, max};

        std::vector<IntervalTree<int>::Interval> intervals;
        for (size_t i = 0; i < 11; ++i) {
            IntervalTree<int>::Interval bounds {lowerBounds[i], upperBounds[i]};
            itree_.insert(bounds);
        }
    }

};

TEST_F(IntervalTreeTest, Query){
    std::vector<std::pair<int, int>> overlaps = itree_.findOverlaps(13);
    ASSERT_EQ(overlaps.size(), 5);
    EXPECT_EQ(overlaps[0], std::make_pair(12, 13));
    EXPECT_EQ(overlaps[1], std::make_pair(12, 22));
    EXPECT_EQ(overlaps[2], std::make_pair(9, 15));
    EXPECT_EQ(overlaps[3], std::make_pair(10, 20));
    EXPECT_EQ(overlaps[4], std::make_pair(-2147483648, 2147483647));
}

TEST_F(IntervalTreeTest, Supersets){
    std::vector<std::pair<int, int>> supersets = itree_.findSupersets(17, 21);
    ASSERT_EQ(supersets.size(), 3);
    EXPECT_EQ(supersets[0], std::make_pair(12, 22));
    EXPECT_EQ(supersets[1], std::make_pair(15, 25));
    EXPECT_EQ(supersets[2], std::make_pair(-2147483648, 2147483647));
}

TEST_F(IntervalTreeTest, SupersetsMax){
    std::vector<std::pair<int, int>> supersets = itree_.findSupersets(-2147483648, 2147483647);
    ASSERT_EQ(supersets.size(), 1);
    EXPECT_EQ(supersets[0], std::make_pair(-2147483648, 2147483647));
}
