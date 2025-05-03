#include "spatial/kdtree.hpp"
#include <array>
#include "gtest/gtest.h"
#include <iostream>

using Point2D = std::array<int, 2>;
using Point3D = std::array<int, 3>;
// Test Fixture: 
class Test2D : public testing::Test {

    public:
    void SetUp() override{
        std::vector<Point2D> v{{35, 40}, {5, 45}, {25, 35}, {50,10}, {90,5}, {60,75}, {80,65}, {85,15}};
        for (auto k : v){
            t.insert(k);
        }
        // std::cout << "Finished setting up" << std::endl;
    }

    void TearDown() override{
        // std::cout << "tear down" << std::endl;
    }

    Tree2D<int> t;
};

TEST_F(Test2D, findMin){
    int xmin = t.findMin(0);
    int ymin = t.findMin(1);

    ASSERT_EQ(xmin, 5);
    ASSERT_EQ(ymin, 5);

    try
    {
        int zmin = t.findMin(2);
    }
    catch(const std::runtime_error& e)
    {
        ASSERT_EQ(e.what(), std::string("Invalid Dimension"));
    }

    // Insert minimum in both dimensions
    Point2D p{-1, -1};
    t.insert(p);

    ASSERT_EQ(t.findMin(0), -1);
    ASSERT_EQ(t.findMin(1), -1);
}

TEST_F(Test2D, contains){
    ASSERT_TRUE(t.contains({60,75}));
    t.insert({16,27});
    ASSERT_TRUE(t.contains({16, 27}));
    ASSERT_TRUE(t.contains({90,5}));
}
// }

// TEST(Tree2D, findMin){

// }

TEST(Test3d, insertion3D){
    Tree3D<int> t3;

}


int main(){
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}