#include <gtest/gtest.h>
#include <vector>
#include <ranges>
#include <iostream>
#include <algorithm>
#include <random>
#include "d-ary-heap/heap.hpp"

bool compare(int x, int y){
    return x > y;
}

TEST(Push, basicPushing){
    BinaryHeap<int> heap;
    heap.push(1);
    heap.push(2);
    heap.push(3);
    heap.push(-1);

    ASSERT_EQ(heap.size(), 4);
    ASSERT_FALSE(heap.empty());

    ASSERT_EQ(heap.top(), -1);

    std::vector<int> cmp{-1, 1, 3, 2};
    std::vector<int> elems = heap.getElements();
    ASSERT_EQ(cmp, elems);
    ASSERT_TRUE(std::ranges::is_heap(elems, compare));
}

TEST(PushPop, basicPopping){
    BinaryHeap<int> heap;
    heap.push(1);
    heap.push(2);
    heap.push(3);
    heap.push(-1);

    auto elems = heap.getElements();
    ASSERT_TRUE(heap.checkMaps());
    ASSERT_TRUE(std::ranges::is_heap(elems, compare));
    heap.pop();
    ASSERT_TRUE(heap.checkMaps());
    ASSERT_TRUE(std::ranges::is_heap(elems, compare));
    std::vector<int> cmp = {1, 2, 3};
    ASSERT_EQ(cmp, heap.getElements());
    heap.pop();
    cmp = {2, 3};
    ASSERT_EQ(cmp, heap.getElements());
}

TEST(Push, Fuzz){
    BinaryHeap<int> heap;
    std::vector<int> v;
    for (int i = 0; i < 100; ++i){
        v.push_back(i);
    }
    long long seed = time(nullptr);
    std::mt19937 rng(seed);

    std::ranges::shuffle(v, rng);
    for (auto x : v){
        heap.push(x);
        ASSERT_TRUE(heap.checkMaps());
        ASSERT_TRUE(std::ranges::is_heap(heap.getElements(), compare));
    }
}


TEST(PushPop, Fuzz){
    BinaryHeap<int> heap;
    std::vector<int> v;
    for (int i = 0; i < 200; ++i){
        v.push_back(i);
    }
    long long seed = time(nullptr);
    std::mt19937 rng(seed);

    std::ranges::shuffle(v, rng);
    for (auto x : v){
        heap.push(x);
    }

    while (!heap.empty()){
        heap.pop();
        ASSERT_TRUE(heap.checkMaps());
        ASSERT_TRUE(std::ranges::is_heap(heap.getElements(), compare));
    }
    std::cout << std::endl;
}

TEST(DecreaseKey, Basic){
    BinaryHeap<int> heap;
    heap.push(1);
    heap.push(2);
    heap.push(5);
    heap.push(-1);
    heap.push(7);

    heap.decreaseKey(5, -2);
    ASSERT_TRUE(std::ranges::is_heap(heap.getElements(), compare));
    ASSERT_TRUE(heap.checkMaps());
    std::vector<int> cmp = {-2, 1, -1, 2, 7};
    ASSERT_EQ(heap.getElements(), cmp);

    try
    {
        heap.decreaseKey(-1, 2);
        FAIL() << "Expected invalid argument";
    }
    catch (std::invalid_argument &e)
    {
        ASSERT_EQ(e.what(), std::string("Cannot decrease a key value to a greater value"));
    }
    catch (...)
    {
        FAIL() << "Expected invalid_argument";
    }
    heap.decreaseKey(7, 5);
    heap.decreaseKey(5, -10);
}

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}