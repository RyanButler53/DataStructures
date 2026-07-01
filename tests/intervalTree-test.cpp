#include "spatial/intervalTree.hpp"
#include <iostream>
#include <algorithm>
#include <ranges>
#include <gtest/gtest.h>


// Interval that holds some data
class IntervalWithMidpoint : public ITree::Interval<float>{
    // Data
    float midpoint_;

    public: 

    IntervalWithMidpoint(float low, float high):
        ITree::Interval<float>(low, high), 
        midpoint_{static_cast<float>((high + low)/ 2.0)}{}
    IntervalWithMidpoint(float low, float high, float data):
        ITree::Interval<float>(low, high), midpoint_{data}{}
    IntervalWithMidpoint(const IntervalWithMidpoint &other) = default;
    ~IntervalWithMidpoint() = default;
    float midpoint() const {return midpoint_;}
    bool operator==(const IntervalWithMidpoint& other) const = default;
};

class IntervalTreeTest : public ::testing::Test {

    protected:
    IntervalTree<ITree::Interval<int>> itree_;

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

class IntervalTreeWithData : public ::testing::Test {
    protected:  
    IntervalTree<IntervalWithMidpoint> itree_;

    void SetUp() override {
        std::array<float, 9> lower{1.1,  1.5, 1.8, 1.2, 2.1, 9.25, 17.5, 30.5, 18.1};
        std::array<float, 9> upper{2.02, 2.5, 2.2, 2.2, 2.3, 15.1, 20.9, 38.2, 20.6};
        std::array<float, 9> data {1.56, 2.0, 2.0, 1.6, 2.2,12.175,19.2,34.35,19.35};
        for (size_t i = 0; i < 9; ++i) {
            IntervalWithMidpoint bounds{lower[i], upper[i], data[i]};
            itree_.insert(bounds);
        }
    }
};

TEST_F(IntervalTreeWithData, testData){
    std::vector<IntervalWithMidpoint> intervals = itree_.findOverlaps(2.0);
    ASSERT_EQ(intervals.size(), 4);
    std::vector<float> midpoints;
    std::transform(intervals.begin(), intervals.end(), std::back_inserter(midpoints), 
    [](const IntervalWithMidpoint& i){return i.midpoint();});
    std::ranges::sort(midpoints);
    ASSERT_EQ(midpoints, std::vector<float>({1.56, 1.6, 2.0, 2.0}));
}