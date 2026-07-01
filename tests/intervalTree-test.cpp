#include "spatial/intervalTree.hpp"
#include <iostream>
#include <gtest/gtest.h>


class IntervalTreeTest : public ::testing::Test {

    protected:
    IntervalTree<int, ITree::Interval<int>> itree_;

    void SetUp() override {
        int min = std::numeric_limits<int>::min();
        int max = std::numeric_limits<int>::max();
        int lowerBounds[] = {10, 15, 18, 12, 21, 9, 17, 30, 19, 12, min};
        int upperBounds[] = {20, 25, 22, 22,23,15,20,40,20,13, max};

        for (size_t i = 0; i < 11; ++i) {
            ITree::Interval<int> bounds {lowerBounds[i], upperBounds[i]};
            itree_.insert(bounds);
        }
    }

};

TEST_F(IntervalTreeTest, Query){
    std::vector<ITree::Interval<int>> overlaps = itree_.findOverlaps(13);
    ASSERT_EQ(overlaps.size(), 5);
    EXPECT_EQ(overlaps[0], ITree::Interval<int>(12, 13));
    EXPECT_EQ(overlaps[1], ITree::Interval<int>(12, 22));
    EXPECT_EQ(overlaps[2], ITree::Interval<int>(9, 15));
    EXPECT_EQ(overlaps[3], ITree::Interval<int>(10, 20));
    EXPECT_EQ(overlaps[4], ITree::Interval<int>(-2147483648, 2147483647));
}

TEST_F(IntervalTreeTest, Supersets){
    std::vector<ITree::Interval<int>> supersets = itree_.findSupersets(17, 21);
    ASSERT_EQ(supersets.size(), 3);
    EXPECT_EQ(supersets[0], ITree::Interval<int>(12, 22));
    EXPECT_EQ(supersets[1], ITree::Interval<int>(15, 25));
    EXPECT_EQ(supersets[2], ITree::Interval<int>(-2147483648, 2147483647));
}

TEST_F(IntervalTreeTest, SupersetsMax){
    std::vector<ITree::Interval<int>> supersets = itree_.findSupersets(-2147483648, 2147483647);
    ASSERT_EQ(supersets.size(), 1);
    EXPECT_EQ(supersets[0], ITree::Interval<int>(-2147483648, 2147483647));
}
