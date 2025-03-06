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

template<typename T, typename priority_t, size_t D>
bool checkHeap(DAryHeap<T, priority_t, D> heap){
    if (!heap.checkMaps()){
        return false;
    }
    std::vector<typename DAryHeap<T, priority_t, D>::Item> elements = heap.getElements();
    // heap property in a min heap means that 
    // every node is greater than their children.
    for (size_t i = 1; i < elements.size(); ++i){
        size_t parent = (i - 1) / D;
        if (elements[parent].priority_ > elements[i].priority_){
            return false;
        }
    }
    return true;
}

TEST(Push, basicPushing){
    BinaryHeap<int> heap;
    heap.push(1,1);
    heap.push(2,2);
    heap.push(-80,3);
    heap.push(-1,-1);

    ASSERT_EQ(heap.size(), 4);
    ASSERT_FALSE(heap.empty());

    ASSERT_EQ(heap.top(), -1);

    ASSERT_TRUE(checkHeap(heap));
}

TEST(PushPop, basicPopping){
    BinaryHeap<int> heap;
    heap.push(2,1);
    heap.push(3,2);
    heap.push(4,4);
    heap.push(-14, -1);

    ASSERT_TRUE(checkHeap(heap));
    heap.pop();
    ASSERT_EQ(heap.top(), 2);
    ASSERT_TRUE(checkHeap(heap));
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
        heap.push(x, x);
        ASSERT_TRUE(checkHeap(heap));
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
        heap.push(x-10,x);
    }

    while (!heap.empty()){
        heap.pop();
        ASSERT_TRUE(checkHeap(heap));
    }
}

TEST(DecreaseKey, Basic){
    BinaryHeap<int> heap;
    heap.push(1,1);
    heap.push(2,2);
    heap.push(5,5);
    heap.push(-1,-1);
    heap.push(7,7);

    heap.decreaseKey(5, -2);
    ASSERT_TRUE(checkHeap(heap));

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
    ASSERT_EQ(heap.top(), 5);
    heap.pop();
    heap.decreaseKey(2, -2);
    ASSERT_EQ(heap.top(), 2);

}

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}