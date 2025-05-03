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
        std::vector<Point2D> v1{{35, 40}, {5, 45}, {25, 35}, {50,10}, {90,5}, {60,75}, {80,65}, {85,15}};
        for (auto k : v1){
            t.insert(k);
        }

        std::vector<Point2D> v2{{35,60}, {20,45}, {60,80}, {80,40},{90,60}, {50,30}, {70,20}, {75,10}, {10,35}, {20,20}};
        for (auto k : v2) {
            t2.insert(k);
        }

        std::vector<Point2D> v3{{10,20}, {11,5}, {5,10}, {5,8}, {15,2}, {20,1}};
        for (auto k : v3) {
            t3.insert(k);
        }

        std::vector<Point2D> v4{{8,10}, {12,7}, {8,6}, {11,6}, {10,2}};
        for (auto k : v4) {
            t4.insert(k);
        }


    }

    void TearDown() override{}

    
    Tree2D<int> t;  // Simpler tree
    Tree2D<int> t2; // More complicated deletion from lecture
    Tree2D<int> t3; // First example from lecture
    Tree2D<int> t4; // Breaking the invariant from lecture
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
TEST_F(Test2D, delete2){
    t3.remove({10,20});
    ASSERT_EQ(t3.findMin(0), 5);
    ASSERT_EQ(t3.findMin(1), 1);
    // Problem: the parent node to the last leaf doesn't get deleted. 
    // Solution: Add some tree traversal into the remove code
}

TEST_F(Test2D, delete1){
    t2.remove({35,60});
    ASSERT_FALSE(t2.contains({35,60}));
    ASSERT_EQ(t2.findMin(1), 10);

    t2.remove({10, 35});
    ASSERT_FALSE(t2.contains({10, 35}));
    ASSERT_EQ(t2.findMin(0),20);

}



TEST(Test3d, insertion3D){
    Tree3D<int> t3;

}


int main(){
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}