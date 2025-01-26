#include <gtest/gtest.h>
#include <cassert>
#include "unrolled-linked-list/unrolled-linked-list.hpp"
#include <vector>
#include <algorithm>

TEST(PushFront, overloadBlock){
    UnrolledLinkedList<int, 8> ull;
    std::vector v{0, 1, 2, 3, 4, 5, 6, 7, 8};
    for (auto &x : v)
    {
        ull.push_front(x);
    }
    ASSERT_EQ(ull.size(), 9);
}

TEST(PushFront, FrontOperator){
    UnrolledLinkedList<int, 4> ull;
    ull.push_front(4);
    ull.push_front(3);
    int x = ull.front();
    ASSERT_EQ(x, 3);
    x = ull.back();
    ASSERT_EQ(x, 4);    
}

TEST(UnrolledLinkedListTest, backOperator){
    UnrolledLinkedList<int, 4> ull;
    ull.push_back(4);
    ull.push_back(3);
    int x = ull.front();
    ASSERT_EQ(x, 4);
    x = ull.back();
    ASSERT_EQ(x, 3);    
}

TEST(UnrolledLinkedListTest, testClear){
    UnrolledLinkedList<int, 4> ull;
    for (int i = 0; i < 16; ++i){
        ull.push_back(i);
    }
    ull.clear();
    ASSERT_EQ(ull.size(), 0);
    ull.push_front(14);
    ASSERT_EQ(ull.size(), 1);
    ASSERT_EQ(ull.back(), 14);
}

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    
}