#include <iostream>
#include <string>
#include <gtest/gtest.h>

#include "scapegoat-tree/scapegoat.hpp"

TEST(Scapegoat, insertion){
    int values[20] = {1, 4, 6, 5, 9, 12, 22, 43, 47, 41, 50, 40, 20, 8, 7, 15, 13, 19, 21, 90};
    std::string keys[20] = {"a", "z", "c", "d", "e", "g", "s", "f", "h", "k", "j", "i", "b", "l", "k", "p", "n", "o", "r", "q"};
    ScapegoatTree<std::string, int> sg{0.75};
    for (size_t i = 0; i < 20; ++i){
         sg.insert(keys[i], values[i]);
    }
    // cout << "inserted all" << endl;
    // cout << sg << endl;
    sg.remove("b");
    sg.remove("g");
    sg.remove("h");
    sg.remove("s");
    sg.remove("f");
    sg.remove("k");
    sg.remove("j");
    sg.remove("a");
    sg.remove("c");
    sg.remove("n");
    // cout << sg << endl;
}

TEST(Scapegoat, reshuffle){
    int arr[10] = {13, 12, 15, 17, 9, 2, 1, 0, 7, 4};
    ScapegoatTree<int, float> sg{2.0 / 3};
    for (int e : arr) {
        sg.insert(e, e/10.0);
    }
    // cout << sg << endl;
    sg.insert(5, 9.0);
    // cout << sg << endl;
}

TEST(Scapegoat, reshuffle2){
    ScapegoatTree<unsigned int, unsigned char> sg{0.7};

    for (size_t x = 0; x < 26; ++x)
    {
        sg.insert(x, x+66);
    }
    // cout << sg << endl;
}


int main(){
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}