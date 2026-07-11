#include "spatial/intervalTree.hpp"
#include <iostream>
#include <algorithm>
#include <ranges>
#include <gtest/gtest.h>

namespace {
    template <typename T>
    void sortIntervals(std::vector<SimpleInterval<T>>& intervals){
        auto compareFunc = [](const auto& i1, const auto& i2){
            if (i1.low() == i2.low()){
                return i1.high() < i2.high();
            } else {
                return i1.low() < i2.low();
            }
        };
        std::ranges::sort(intervals, compareFunc);
    }
} // anonymous namespace

// Interval that holds its midpoint
class IntervalWithMidpoint : public SimpleInterval<float>{
    // Data
    float midpoint_;
    public: 

    IntervalWithMidpoint(float low, float high):
        SimpleInterval<float>(low, high), 
        midpoint_{static_cast<float>((high + low)/ 2.0)}{}
    IntervalWithMidpoint(float low, float high, float data):
        SimpleInterval<float>(low, high), midpoint_{data}{}
    IntervalWithMidpoint(const IntervalWithMidpoint &other) = default;
    ~IntervalWithMidpoint() = default;
    float midpoint() const {return midpoint_;}
    bool operator==(const IntervalWithMidpoint& other) const = default;
};

class IntervalTreeTest : public ::testing::Test {

    protected:
    IntervalTree<SimpleInterval<int>> itree_;
    int min_;
    int max_;

    void SetUp() override {
        min_ = std::numeric_limits<int>::min();
        max_ = std::numeric_limits<int>::max();
        std::array<int, 11> lowerBounds = {10, 15, 18, 12, 21,  9, 17, 30, 19, 12, min_};
        std::array<int, 11> upperBounds = {20, 25, 22, 22, 23, 15, 20, 40, 20, 13, max_};

        for (size_t i = 0; i < 11; ++i) {
            SimpleInterval<int> bounds {lowerBounds[i], upperBounds[i]};
            itree_.insert(bounds);
        }
    }

};

TEST_F(IntervalTreeTest, Query){
    std::vector<SimpleInterval<int>> overlaps = itree_.findOverlaps(13);
    ASSERT_EQ(overlaps.size(), 5);
    EXPECT_EQ(overlaps[0], SimpleInterval<int>(12, 13));
    EXPECT_EQ(overlaps[1], SimpleInterval<int>(12, 22));
    EXPECT_EQ(overlaps[2], SimpleInterval<int>(9, 15));
    EXPECT_EQ(overlaps[3], SimpleInterval<int>(10, 20));
    EXPECT_EQ(overlaps[4], SimpleInterval<int>(min_, max_));
}

TEST_F(IntervalTreeTest, Supersets){
    std::vector<SimpleInterval<int>> supersets = itree_.findSupersets(17, 21);
    ASSERT_EQ(supersets.size(), 3);
    EXPECT_EQ(supersets[0], SimpleInterval<int>(12, 22));
    EXPECT_EQ(supersets[1], SimpleInterval<int>(15, 25));
    EXPECT_EQ(supersets[2], SimpleInterval<int>(min_, max_));
}

TEST_F(IntervalTreeTest, SupersetsMax){
    std::vector<SimpleInterval<int>> supersets = itree_.findSupersets(min_, max_);
    ASSERT_EQ(supersets.size(), 1);
    EXPECT_EQ(supersets[0], SimpleInterval<int>(min_, max_));
}

TEST_F(IntervalTreeTest, RangeQuery){
    itree_.clear();
    itree_.insert(1874,1951);
    itree_.insert(1779,1828);
    itree_.insert(1585,1672);
    itree_.insert(1843,1907);
    itree_.insert(1888,1971);
    itree_.insert(1756,1791);

    std::vector<SimpleInterval<int>> intervals = itree_.findOverlaps(SimpleInterval<int>{1755, 1830});
    ASSERT_EQ(intervals.size(), 2);
    EXPECT_EQ(intervals[0], SimpleInterval<int>(1779, 1828));
    EXPECT_EQ(intervals[1], SimpleInterval<int>(1756, 1791));
}

TEST_F(IntervalTreeTest, RangeQuery2){
    std::vector<SimpleInterval<int>> intervals = itree_.findOverlaps(SimpleInterval<int>{17, 21});
    sortIntervals(intervals);
    ASSERT_EQ(intervals.size(), 8);
    std::array<int, 8> lower = {min_, 10, 12, 15, 17, 18, 19, 21}; // 21
    std::array<int, 8> upper = {max_, 20, 22, 25, 20, 22, 20, 23}; // 23
    for (int i : std::views::iota(0, 8)){
        EXPECT_EQ(intervals[i], SimpleInterval<int>(lower[i], upper[i]));
    }

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
    EXPECT_EQ(midpoints, std::vector<float>({1.56, 1.6, 2.0, 2.0}));
}

TEST_F(IntervalTreeWithData, duplicateIntervals){
    itree_.insert({1.5, 2.5, 3.0});
    std::vector<IntervalWithMidpoint> intervals = itree_.findOverlaps(2.0);
    EXPECT_EQ(intervals.size(), 5);

    std::vector<float> midpoints;
    std::transform(intervals.begin(), intervals.end(), std::back_inserter(midpoints), 
    [](const IntervalWithMidpoint& i){return i.midpoint();});
    std::ranges::sort(midpoints);
    EXPECT_EQ(midpoints, std::vector<float>({1.56, 1.6, 2.0, 2.0, 3.0}));

}

TEST_F(IntervalTreeWithData, duplicateIntervals2){
    itree_.clear();
    itree_.insert({0, 20, 0});
    itree_.insert({50, 80, 0});
    itree_.insert({0, 55, 1});
    itree_.insert({75, 100, 1});
    itree_.insert({0, 55, 2});
    itree_.insert({80, 100, 2});

    std::vector<IntervalWithMidpoint> overlaps = itree_.findOverlaps(15);
    ASSERT_EQ(overlaps.size(), 3);
    EXPECT_EQ(overlaps[0], IntervalWithMidpoint(0, 55, 2));
    EXPECT_EQ(overlaps[1], IntervalWithMidpoint(0, 55, 1));
    EXPECT_EQ(overlaps[2], IntervalWithMidpoint(0, 20, 0));

    overlaps = itree_.findOverlaps(40);
    ASSERT_EQ(overlaps.size(), 2);
    EXPECT_EQ(overlaps[0], IntervalWithMidpoint(0, 55, 2));
    EXPECT_EQ(overlaps[1], IntervalWithMidpoint(0, 55, 1));

    std::vector<IntervalWithMidpoint> superset = itree_.findSupersets(15, 40);
    EXPECT_EQ(overlaps, superset);
}