#include <iostream>
#include <string>
#include <map>
#include <gtest/gtest.h>

#include "tree/scapegoat.hpp"

TEST(Scapegoat, insertion){
    int values[20] = {1, 26, 3, 4, 5, 7, 19, 6, 8, 11, 10, 9, 2, 12, 111, 16, 14, 15, 18, 17};
    std::string keys[20] = {"a", "z", "c", "d", "e", "g", "s", "f", "h", "k", "j", "i", "b", "l", "k", "p", "n", "o", "r", "q"};
    std::map<std::string, int> correct;

    ScapegoatTree<std::string, int> sg{0.75};
    for (size_t i = 0; i < 20; ++i){
         sg.insert({keys[i], values[i]});
         correct.insert({keys[i], values[i]});
    }
    sg.erase("b");
    sg.erase("g");
    sg.erase("h");
    sg.erase("s");
    sg.erase("f");
    sg.erase("k");
    sg.erase("j");
    sg.erase("a");
    sg.erase("c");
    sg.erase("n");
    std::vector<std::string> finalKeys;
    for (const auto& [k, _] : sg){
        finalKeys.push_back(k);
    }
    // Check if all keys are sorted
    ASSERT_TRUE(std::ranges::is_sorted(finalKeys));
    ASSERT_EQ(sg.size(), 9);
    ASSERT_EQ(sg.size(), finalKeys.size());
    for (std::string s : std::vector<std::string>{"z", "d", "e", "i", "l", "p", "o", "r", "q"}){
        std::cout << s << " " << sg[s] << " " << correct[s] << std::endl;
        ASSERT_EQ(sg[s], correct[s]);
    }


}

TEST(Scapegoat, reshuffle){
    int arr[10] = {13, 12, 15, 17, 9, 2, 1, 0, 7, 4};
    ScapegoatTree<int, float> sg{2.0 / 3};
    for (int e : arr) {
        sg.insert({e, e/10.0});
    }
    // cout << sg << endl;
    sg.insert({5, 9.0});
    // cout << sg << endl;
}

TEST(Scapegoat, reshuffle2){
    ScapegoatTree<unsigned int, unsigned char> sg{0.7};

    for (size_t x = 0; x < 26; ++x) {
        sg.insert({x, x+66});
    }
    // cout << sg << endl;
}


int main(){
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}