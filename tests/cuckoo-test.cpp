#include <iostream>
#include <string>
#include "cuckoo-hash/cuckoo-hash.hpp"
#include <cassert>
#include <array>
#include "gtest/gtest.h"

using namespace std;

class CuckooTest : public ::testing::Test{
    protected:
    const std::array<std::string, 30> keys_{"a", "z", "c", "d", "e", "g", "s", "f", "h", "k", "j", "i", "b", "l", "t", "p", "n", "o", "r", "q", "ab", "ac", "ad", "ae", "aq", "aa", "ag", "ah", "ai", "aj"};
    const std::array<int, 30> values_{1, 4, 6, 5, 9, 12, 22, 43, 47, 41, 50, 40, 20, 8, 7, 15, 13, 19, 21, 90, 104, 102, 103, 201, 105, 203, 254, 291, 221, 210};

};


TEST_F(CuckooTest, cuckooMap){

    CuckooHashMap<string, int> ch = CuckooHashMap<string, int>(0.3, 0.2);
        for (size_t i = 0; i < 30; ++i){
        ch.insert(keys_[i], values_[i]);
    }
    EXPECT_EQ(ch.size(), 30);
    for (size_t i = 0; i < 20; ++i)
    {
        EXPECT_EQ(values_[i], ch[keys_[i]]);
        ch.erase(keys_[i]);
    }
    EXPECT_EQ(ch.size(), 10);

    ch.insert("string", 155);

    // Reassignment, not insertion. 
    ch["string"] = 144;
    EXPECT_EQ(ch["string"], 144);
}

TEST_F(CuckooTest, cuckooSet){
 // CUCKOO SET
 CuckooHashSet<string> cs = CuckooHashSet<string>(0.3, 0.2);
 for (size_t i = 0; i < 30; ++i)
 {
     cs.insert(keys_[i]);
 }
 for (size_t i = 0; i < 30; ++i){
    EXPECT_TRUE(cs.contains(keys_[i]));
 }
 for (size_t i = 0; i < 25; ++i) 
 {
     cs.erase(keys_[i]);
 }
 cs.clear();
 EXPECT_TRUE(cs.empty());
 for (size_t i = 10; i < 15; ++i){
     cs.insert(keys_[i]);
 }

}