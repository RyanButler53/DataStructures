#include <gtest/gtest.h>
#include <vector>
#include <concepts>
#include <ranges>
#include <iostream>
#include <algorithm>
#include <random>
#include "d-ary-heap/heap.hpp"
#include "binomial-heap/heap.hpp"
#include "fibonacci-heap/heap.hpp"

template <typename T>
concept Heap = requires(T& heap,
                        T::value_type item,
                        T::priority_type priority) {
    heap.empty();
    heap.size();
    heap.pop();
    heap.push(item, priority);
    heap.top();
    heap.decreaseKey(item, priority);
};

class NameGenerator {
  public:
    template <typename T>
    static std::string GetName(int) {
       if constexpr (std::is_same_v<T, BinaryHeap<int>>) return "Binary Heap";
       if constexpr (std::is_same_v<T, DAryHeap<int,int,4>>) return "D-Ary Heap (d=4)";
       if constexpr (std::is_same_v<T, BinomialHeap<int, int>>) return "Binomial Heap";
       if constexpr (std::is_same_v<T, FibonacciHeap<int, int>>) return "Fibonacci Heap";
    }
};

template <Heap heap_t>
class HeapTest : public testing::Test {
  public:
    void SetUp() override {}
    void TearDown() override {}

    bool checkHeap(){

        std::vector<typename heap_t::value_type> items;
        while (!this->heap_.empty()){
            items.push_back(this->heap_.top());
            this->heap_.pop();
        }
        return std::ranges::is_sorted(items, [](heap_t::value_type x, heap_t::value_type y)
                                      { return x < y; });
    }
    // Data
    heap_t heap_;
};

using testing::Types;

typedef Types<BinaryHeap<int>, 
              DAryHeap<int, int, 4>, 
              BinomialHeap<int, int>,
              FibonacciHeap<int, int>> Implementations;

TYPED_TEST_SUITE(HeapTest, Implementations, NameGenerator);


TYPED_TEST(HeapTest, pushing){
    this->heap_.push(1,1);
    this->heap_.push(2,2);
    this->heap_.push(-80,3);
    this->heap_.push(-1,-1);

    ASSERT_EQ(this->heap_.size(), 4);
    ASSERT_FALSE(this->heap_.empty());

    ASSERT_EQ(this->heap_.top(), -1);

    this->heap_.pop();
    ASSERT_EQ(this->heap_.top(), 1);
    
    this->heap_.pop();
    ASSERT_EQ(this->heap_.top(), 2);

    this->heap_.pop();
    ASSERT_EQ(this->heap_.top(), -80);

    this->heap_.pop();
    try {
        this->heap_.top();
        FAIL() << "Expected logic error, didn't throw error";
    } catch (std::logic_error &e) {
        ASSERT_EQ(e.what(), std::string("Cannot get top of an empty heap"));
    } catch (...) {
        FAIL() << "Expected logic error, threw a different error";
    }
    
}

TYPED_TEST(HeapTest, popping){
    this->heap_.push(2,1);
    this->heap_.push(3,2);
    this->heap_.push(4,4);
    this->heap_.push(-14, -1);

    this->heap_.pop();
    ASSERT_EQ(this->heap_.top(), 2);
    ASSERT_TRUE(this->checkHeap());
}

TYPED_TEST(HeapTest, FuzzPush){
    std::vector<int> v;
    for (int i = 0; i < 100; ++i){
        v.push_back(i);
    }
    long long seed = time(nullptr);
    std::mt19937 rng(seed);

    std::ranges::shuffle(v, rng);
    for (auto x : v){
        this->heap_.push(x, x);
    }
    ASSERT_TRUE(this->checkHeap());
    for (auto x : v){
        this->heap_.push(x, x);
    }
    // Trigger a re-shuffle
    this->heap_.pop();
    // Tests destructor
}

TYPED_TEST(HeapTest, FuzzPop){
    std::vector<int> v;
    for (int i = 0; i < 200; ++i){
        v.push_back(i);
    }
    long long seed = time(nullptr);
    std::mt19937 rng(seed);

    std::ranges::shuffle(v, rng);
    for (auto x : v){
        this->heap_.push(x-10,x);
    }
    while (!this->heap_.empty()){
        this->heap_.pop();
    }
    ASSERT_EQ(this->heap_.size(), 0);
    ASSERT_TRUE(this->heap_.empty());
}

TYPED_TEST(HeapTest, DecreaseKey){
    this->heap_.push(1,1);
    this->heap_.push(2,2);
    this->heap_.push(5,5);
    this->heap_.push(-1,-1);
    this->heap_.push(7,7);

    this->heap_.decreaseKey(5, -2);
    ASSERT_EQ(this->heap_.top(), 5);
    try {
        this->heap_.decreaseKey(-1, 2);
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
    // Push an element with minimum priority the pop to trigger re-shuffle
    this->heap_.push(15, -15);
    this->heap_.pop();

    this->heap_.decreaseKey(7, 5);
    this->heap_.decreaseKey(5, -3);
    this->heap_.decreaseKey(7, -2);
    ASSERT_EQ(this->heap_.top(), 5);
    this->heap_.decreaseKey(2, -5);
    ASSERT_EQ(this->heap_.top(), 2);
    this->heap_.pop();
    this->heap_.decreaseKey(5, -5);
    ASSERT_EQ(this->heap_.top(), 5);
    this->heap_.pop();
    ASSERT_EQ(this->heap_.top(), 7);
}

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}