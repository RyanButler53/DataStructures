#include <gtest/gtest.h>

#include "hashing/linear-probing.hpp"
#include "hashing/quadratic-probing.hpp"
#include "hashing/double-hashing.hpp"
#include "hashing/separate-chaining.hpp"
#include "cuckoo-hash/cuckoo-hash.hpp"
#include "interfaces.hpp"

#include <vector>
#include <ranges>
#include <iostream>
#include <algorithm>
#include <random>

class NameGenerator {
    public:
      template <typename T>
      static std::string GetName(int) {
         if constexpr (std::is_same_v<T, LinearProbing<int>>) return "Linear Probing";
        //  if constexpr (std::is_same_v<T, DAryHeap<int,int,4>>) return "D-Ary Heap (d=4)";
        //  if constexpr (std::is_same_v<T, BinomialHeap<int, int>>) return "Binomial Heap";
        //  if constexpr (std::is_same_v<T, FibonacciHeap<int, int>>) return "Fibonacci Heap";
        //  if constexpr (std::is_same_v<T, PairingHeap<int, int>>) return "Pairing Heap";
      }
  };

template <HashSet hash_t>
class HashsetTest  : public testing::Test {
    
    public:
    void SetUp() override {}
    void TearDown() override {}

    hash_t set_;
};

using testing::Types;

typedef Types<LinearProbing<int>> Implementations;

TYPED_TEST_SUITE(HashsetTest, Implementations, NameGenerator);

TYPED_TEST(HashsetTest, insertionRehashing){
    for (int i = 1; i < 65; ++i){
        this->set_.insert(i);
    }
    // Check the zero case
    ASSERT_FALSE(this->set_.contains(0));

    for (int i = 1; i < 65; ++i ){
        ASSERT_TRUE(this->set_.contains(i)) << i << " should have been in the hash set";
    }
    for (int i = 0; i < 10; ++i){
        ASSERT_FALSE(this->set_.contains(i+65)) << i << " should not have been in the hashset";
    }
}

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

