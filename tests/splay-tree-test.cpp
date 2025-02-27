#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <gtest/gtest.h>

#include "splay-tree/splay-tree.hpp"

TEST(SplayTreeTest, general){
    SplayTree<int, int> st;
    int keys[11] = {50, 40, 30, 20, 10, 60, 55, 35, 38, 65, 5};
// Test insertion and fill tree
    for (int x : keys)
    {
        st.insert({x, x + 100});
    }
    SplayTree<int, int> st2;
    std::string empty = "-";
    std::stringstream ss1;
    ss1 << st2;
    
    ASSERT_EQ(ss1.str(), empty);

    std::string insertTest{"(-, 5:105, ((-, 10:110, (((-, 20:120, -), 30:130, -), 35:135, -)), 38:138, ((((-, 40:140, (-, 50:150, -)), 55:155, -), 60:160, -), 65:165, -)))"};
    std::stringstream ss;
    ss << st;
    ASSERT_EQ(ss.str() , insertTest);
    ASSERT_EQ(st.erase(60), 1);
    ASSERT_EQ(st.erase(40), 1);
    ASSERT_EQ(st.erase(38), 1);
    ASSERT_EQ(st.erase(41), 0);

    std::string deleteTest{"((((-, 5:105, -), 10:110, ((-, 20:120, -), 30:130, -)), 35:135, -), 50:150, (-, 55:155, (-, 65:165, -)))"};
    std::stringstream ss2;
    ss2 << st;
    assert(ss2.str() == deleteTest);

    // test iteration
    vector<int> allKeys;
    vector<int> allValues;
    for (auto [k, v] : st)
    {
        allKeys.push_back(k);
        allValues.push_back(v);
    }

    for (size_t keyInd = 0; keyInd < st.size(); ++keyInd)
    {
        assert(allKeys[keyInd] == allValues[keyInd] - 100);
    }

    // test lookups with find();
    int newKeys[8] = {65, 5, 20, 10, 55, 35, 30, 50};
    for (int key : newKeys){
        auto [k, v] = *(st.find(key));
        ASSERT_EQ(k, key);
        ASSERT_EQ(v, key + 100);
    }

    // test invalid lookup
    auto iter = st.find(25);
    ASSERT_EQ(iter, st.end());

    // Test removing after insertion, finding and iterating. 
    ASSERT_EQ(st.erase(30),1);
    ASSERT_EQ(st.erase(50),1);

    // Test size after insertion and removal;
    ASSERT_EQ(st.size(), 6);

    // test lookups with []. Required that keys exist;
    for (size_t keyInd = 0; keyInd < 6; ++keyInd){
        int k = newKeys[keyInd];
        int val = st[k];
        ASSERT_EQ(val , k + 100);
    }

    int keysToCheck[4] = {5, 35, 60, 70};
    bool valid[4] = {true, true, false, false};
    for (size_t i = 0; i < 4; ++i){
        bool exists = st.exists(keysToCheck[i]);
        ASSERT_EQ(valid[i],  exists);
    }

    // Remove rest of tree.
    for (int x : newKeys)
    {
        st.erase(x);
    }
    ASSERT_EQ(st.size(), 0);

    
}

TEST(SplayTreeTest, destructor){
    SplayTree<int, int> st;
    int keys[11] = {50, 40, 30, 20, 10, 60, 55, 35, 38, 65, 5};
    for (int x : keys){
        st.insert({x, x + 4});
    }
}

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}