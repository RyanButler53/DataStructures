#include <gtest/gtest.h>

#include <vector>
#include <ranges>
#include <iostream>
#include <algorithm>
#include <random>

// Trees
#include "tree/splay-tree.hpp"
#include "tree/scapegoat.hpp"
#include <map>

// Interface
#include "interfaces.hpp"

struct NameGenerator {

    template <typename T>
    static std::string GetName(int) {
        if constexpr (std::is_same_v<T, std::map<int, int>>) return "std::map";
        if constexpr (std::is_same_v<T, SplayTree<int, int>>) return "Splay Tree";
        if constexpr (std::is_same_v<T, ScapegoatTree<int,int>>) return "Scapegoat Tree";
    }
  };

template <Tree tree_t>
class TreeTest : public testing::Test {
    public:
    void SetUp() override {
        std::array<int, 11> keys = {50, 40, 30, 20, 10, 60, 55, 35, 38, 65, 5};
    
        for (auto k : keys){
            this->tree_.insert({k, k+100});
        }
    }
    void TearDown() override {
        ASSERT_TRUE(checkSorted());
    }

    bool checkSorted() const {
        
        std::vector<typename tree_t::key_type> items;
        for (auto [k, v] : this->tree_) {
            items.push_back(k);
        }
       
        return std::ranges::is_sorted(items, [](tree_t::key_type x, tree_t::key_type y)
                                      { return x < y; });
    }

    std::vector<int> getKeys(){
        std::vector<int> keys;
        for (auto [k,_] : this->tree_){
            keys.push_back(k);
        }
        return keys;
    }


    tree_t tree_;
};

using testing::Types;

typedef Types<std::map<int, int>,
              SplayTree<int, int>, 
              ScapegoatTree<int, int>> Implementations;

TYPED_TEST_SUITE(TreeTest, Implementations, NameGenerator);

TYPED_TEST(TreeTest, insertion1){

    // Finding 
    std::array<int, 8> keys = {65, 5, 20, 10, 55, 35, 30, 50};
    for (int key : keys){
        auto [k, v] = *(this->tree_.find(key));
        ASSERT_EQ(k, key);
        ASSERT_EQ(v, key + 100);
    }
}



TYPED_TEST(TreeTest, finding){
    this->tree_.clear();
    std::vector<std::tuple<int,int>>keys{{50, 150}, {40,140}, {5,105}};
    // Test range insertion
    this->tree_.insert(keys.begin(), keys.end());

    // Find for key not in the tree
    auto iter = this->tree_.find(25);
    ASSERT_EQ(iter, this->tree_.end());

}

TYPED_TEST(TreeTest, finding2){
    std::vector<int> keys = this->getKeys();
    for (auto k : keys){
        ASSERT_EQ(this->tree_[k] , k + 100);
    }
}

TYPED_TEST(TreeTest, deleting1){

    ASSERT_EQ(this->tree_.erase(60), 1);
    ASSERT_EQ(this->tree_.erase(40), 1);
    ASSERT_EQ(this->tree_.erase(38), 1);
    ASSERT_EQ(this->tree_.erase(41), 0);

    // Force splay in splay tree not necessary in scapegoat tree
    auto iter = this->tree_.find(25);
    ASSERT_EQ(iter, this->tree_.end());

    ASSERT_EQ(this->tree_.erase(30),1);
    ASSERT_EQ(this->tree_.erase(50),1);

    // Test size after removal;
    ASSERT_EQ(this->tree_.size(), 6);

}

TYPED_TEST(TreeTest, insertionAndDeletion){
    this->tree_.clear();
    std::array<int, 20> keys = {1, 26, 3, 4, 5, 7, 19, 6, 8, 11, 10, 9, 2, 12, 11, 16, 14, 15, 18, 17};
    for (auto k : keys){
        this->tree_.insert({k,k});
    }
    ASSERT_EQ(this->tree_.size(), 19); // key 11 is a duplicate

    // Remove some keys
    for (int k : std::vector<int>{2,7,8,19,6,11,10,1,3,14}) {
        ASSERT_EQ(this->tree_.erase(k), 1) << "Couldn't erase " << k;
        ASSERT_EQ(this->getKeys().size(), this->tree_.size()) << "Size mismatch after erasing " << k;
    }

    std::vector<int> finalKeys = this->getKeys();
    
    ASSERT_TRUE(std::ranges::is_sorted(finalKeys));
    ASSERT_EQ(this->tree_.size(), 9);
    ASSERT_EQ(this->tree_.size(), finalKeys.size());

    for (int s : std::vector<int>{26, 4, 5, 9, 12, 16, 15, 18, 17}){
        ASSERT_EQ(this->tree_[s], s) << " Expected value: " << s <<  " actual value " << this->tree_[s];
    }

}

TYPED_TEST(TreeTest, destructor){
    ASSERT_EQ(this->tree_.size(), 11);
}

int main(){
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}