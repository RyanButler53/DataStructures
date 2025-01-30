#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <random>

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

TEST(PushFront, PushFront3){
    UnrolledLinkedList<int, 8> ull;
    for (size_t i = 0; i < 9; ++i){
        ull.push_front(10000+i);
    }
    ull.push_front(15);
    ull.push_front(16);    
    ull.push_front(17);
    ull.push_front(180000);

    std::vector<int> comp{180000, 17, 16, 15, 10008, 10007, 10006, 10005, 10004, 10003, 10002, 10001, 10000};
    ASSERT_EQ(comp, toVec(ull));
}

TEST(AccessOperators, backOperator){
    UnrolledLinkedList<int, 4> ull;
    ull.push_back(4);
    ull.push_back(3);
    int x = ull.front();
    ASSERT_EQ(x, 4);
    x = ull.back();
    ASSERT_EQ(x, 3);    
}

TEST(AccessOperators, testClear){
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

TEST(Insertion, InsertIterator){
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
TEST(Insertion, testInsertion2){
    UnrolledLinkedList<int, 5> ull;
    for (int i = 0; i < 6; ++i){
        ull.push_back(i);
    }
    std::vector<int> comp;
    
    comp = {0,1,2,3,4,5};
    ASSERT_EQ(comp, toVec(ull));
}

TEST(Insertion, InsertIterator2){
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
// Tests sneaky case of reusing the iterator after pushing front on empty list
TEST(Insertion, IteratorInsertion){
    UnrolledLinkedList<int, 4> ull;
    auto it = ull.begin();
    ull.insert(it, 1500);
    ull.insert(it, 400);
}

TEST(Deletion, PopBack){
    UnrolledLinkedList<int, 5> ull;
    std::vector<int> comp;
    for (int i = 0; i < 6; ++i)
    {
        ull.push_back(i);
    }
    ull.pop_back();
    comp = {0,1,2,3,4};
    ASSERT_EQ(comp, toVec(ull));
    
    ull.pop_back();
    comp = {0,1,2,3};
    ASSERT_EQ(comp, toVec(ull));
    
    ull.pop_back();
    comp = {0,1,2};
    ASSERT_EQ(comp, toVec(ull));

    ull.pop_back();
    ull.pop_back();
    ull.pop_front();
    comp = {};
    ASSERT_EQ(ull.size(), 0);
}

TEST(Deletion, PopBackAndFront){
    UnrolledLinkedList<int, 5> ull;
    std::vector<int> comp;
    for (int i = 0; i < 6; ++i)
    {
        ull.push_back(i);
    }
    for (size_t i = 0; i < 3; ++i){
        ull.pop_front();
        ull.pop_back();
    }    
}

TEST(Deletion, EmptyListException){
    UnrolledLinkedList<int, 5> ull;
    std::vector<int> comp;
    for (int i = 0; i < 6; ++i)
    {
        ull.push_back(i);
    }
    for (size_t i = 0; i < 3; ++i){
        ull.pop_front();
        ull.pop_back();
    }    

    try {
        ull.pop_back();
        FAIL() << "Expected std::out_of_range";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(),std::string("Can't call pop_back on an empty list"));
    }
    catch(...) {
        FAIL() << "Expected std::invalid_argument";
    }
}

TEST(Deletion, EraseIter1){
    UnrolledLinkedList<int, 5> ull;
    std::vector<int> comp;
    for (int i = 0; i < 11; ++i)
    {
        ull.push_back(i);
    }
    UnrolledLinkedList<int, 5>::iterator it = ull.begin();
    ++it;
    ASSERT_EQ(*it, 1);
    for (size_t i = 0; i < 4; ++i)
    {
        it = ull.erase(it);
        ASSERT_EQ(*it, 2 * (i + 1));
        ++it;
    }
    ASSERT_EQ(ull.size(), 7);
    comp = {0, 2, 4, 6, 8, 9, 10};
    ASSERT_EQ(comp, toVec(ull));
}

TEST(Deletion, InsertDelete){
    UnrolledLinkedList<int, 4> ull;
    auto it = ull.begin();
    ull.insert(it, 10000);
    ASSERT_EQ(it, ull.end());
    try {
        ull.erase(it);
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(),std::string("Cannot erase from end() iterator"));
    }
    catch(...) {
        FAIL() << "Expected std::invalid_argument";
    }
}

TEST(Deletion, ReturnEnd){
    UnrolledLinkedList<int, 4> ull;

    auto it = ull.begin();
    ull.insert(it, 15);
    --it;
    it = ull.erase(it);
    ASSERT_EQ(it, ull.end());
}


TEST(Fuzz, compareList){
    UnrolledLinkedList<int, 4> ull;
    std::list<int> ll;
    size_t seed = time(nullptr);
    std::mt19937 rng(seed);
    std::discrete_distribution<int> dist({1, 1, 1, 1});
    // Do the same set of operations on them and expect same result at the end
    auto ull_it = ull.begin();
    auto ll_it = ll.begin();
    for (int step = 0; step < 5000; ++step)
    {
        const int result = dist(rng);
        switch (result)
        {
        case 0:
            ull.insert(ull_it, step);
            ll.insert(ll_it, step);
            break;
        case 1:
            if (ull_it != ull.end()){
                ++ull_it;
            }
            if (ll_it != ll.end()){
                ++ll_it;
            }
            break;
        case 2:
            if (ull_it != ull.begin()){
                --ull_it;
            }
            if (ll_it != ll.begin()){
                --ll_it;
            }
            break;
        case 3:
            if (ull_it != ull.end()){
                ull_it = ull.erase(ull_it);
            }
            if (ll_it != ll.end()){
                ll_it = ll.erase(ll_it);
            }
            break;
        }
        std::vector<int> v = toVec(ull);
        ASSERT_TRUE(std::ranges::equal(v, ll))<<" at step " << step;
    }
    std::vector<int> v = toVec(ull);
    ASSERT_TRUE(std::ranges::equal(v, ll));
}

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}