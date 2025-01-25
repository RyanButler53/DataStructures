#include <gtest/gtest.h>
#include <cassert>

TEST(UnrolledLinkedListTest, begin){
    ASSERT_EQ(5, 3 + 2);
}



int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}