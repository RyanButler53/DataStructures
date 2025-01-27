#include <gtest/gtest.h>
#include <cassert>
#include <vector>
#include <algorithm>

#include "unrolled-linked-list/unrolled-linked-list.hpp"

template <typename T, size_t K>
std::vector<T> toVec(UnrolledLinkedList<T,K>& ull){
    std::vector<T> values;
    for (auto v : ull){
        values.push_back(v);
    }
    return values;
}

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

TEST (UnrolledLinkedListTest, testIteration){
    UnrolledLinkedList<int, 4> ull;
    for (int i = 0; i < 5; ++i){
        ull.push_back(i);
    }

    std::vector v{0, 1, 2, 3, 4};
    std::vector<int> ull_values = toVec(ull);
    ASSERT_TRUE(std::ranges::equal(ull_values, v));
}

TEST(UnrolledLinkedListTest, testInsertion){
    UnrolledLinkedList<int, 4> ull;
    for (int i = 0; i < 8; ++i){
        ull.push_back(i);
    }
    UnrolledLinkedList<int, 4>::iterator it = ull.begin();
    ull.insert(it, 8);
    ASSERT_EQ(*it, 0);
    for (size_t i = 0; i < 3; ++i)
    {
        ++it;
    }
    ASSERT_EQ(*it, 3);
    ull.insert(it, 9);
    std::vector<int> comp{8, 0, 1, 2, 9, 3, 4, 5, 6, 7};
    ASSERT_EQ(comp, toVec(ull));
    ASSERT_EQ(*it, 3);
    ASSERT_EQ(ull.size(), 10);

    ull.insert(it, 10);
    comp = {8, 0, 1, 2, 9, 10, 3, 4, 5, 6, 7};
    ASSERT_EQ(ull.size(), 11);
    ASSERT_EQ(comp, toVec(ull));

    ASSERT_EQ(*it, 3);
    ull.insert(it, 11);
    ASSERT_EQ(ull.size(), 12);
    comp = {8, 0, 1, 2, 9, 10, 11, 3, 4, 5, 6, 7};
    ASSERT_EQ(comp, toVec(ull));

    for (size_t i = 0; i < 3; ++i){
        ++it;
    }
    ASSERT_EQ(*it, 6);
    ++it;
    ull.insert(it, 12);
    ASSERT_EQ(ull.size(), 13);
    ++it;
    ASSERT_EQ(it, ull.end());

    comp = {8, 0, 1, 2, 9, 10, 11, 3, 4, 5, 6, 12, 7};
    ASSERT_EQ(comp, toVec(ull));

    // it is at end;
    ull.insert(it, 13);
    comp = {8, 0, 1, 2, 9, 10, 11, 3, 4, 5, 6, 12, 7, 13};
    ASSERT_EQ(comp, toVec(ull)); 

    ull.insert(it, 14);
    comp = {8, 0, 1, 2, 9, 10, 11, 3, 4, 5, 6, 12, 7, 13, 14};
    ASSERT_EQ(comp, toVec(ull)); 
}
TEST(UnrolledLinkedListTest, testInsertion2){
    UnrolledLinkedList<int, 5> ull;
    for (int i = 0; i < 6; ++i){
        ull.push_back(i);
    }
    std::vector<int> comp;
    
    comp = {0,1,2,3,4,5};
    ASSERT_EQ(comp, toVec(ull));
}

TEST(UnrolledLinkedListTest, testInsertion3){
    UnrolledLinkedList<int, 5> ull;
    for (int i = 0; i < 7; ++i){
        ull.push_back(i);
    }
    std::vector<int> comp;
    
    comp = {0,1,2,3,4,5,6};
    ASSERT_EQ(comp, toVec(ull));

    auto it = ull.begin();
    for (size_t i = 0; i < 3; ++i){
        ++it;
    }

    ull.insert(it, 7);
    comp = {0,1,2,7,3,4,5,6};
    ASSERT_EQ(comp, toVec(ull));

    ull.insert(it, 8);
    comp = {0,1,2,7,8,3,4,5,6};
    ASSERT_EQ(comp, toVec(ull));
}

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    
}