#include "spatial/kdtree.hpp"
#include <array>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <iostream>
#include <algorithm>
#include <random>

using ::testing::ElementsAreArray;
using ::testing::UnorderedElementsAreArray;
using Point2D = std::array<int, 2>;
using Point2Dd = std::array<double, 2>;
using Point3D = std::array<int, 3>;
// Test Fixture: 
class Test2D : public testing::Test {

    public:
    void SetUp() override{
        for (auto k : v1){
            t1.insert(k);
        }

        for (auto k : v2) {
            t2.insert(k);
        }

        for (auto k : v3) {
            t3.insert(k);
        }

        for (auto k : v4) {
            t4.insert(k);
        }


    }

    void TearDown() override{}

    double dist(Point2D p1, Point2D p2){
        int dx = p1[0]-p2[0];
        int dy = p1[1] - p2[1];
        return std::sqrt((dx * dx) + (dy * dy));
    }

    std::vector<Point2D> pointsInRadius(std::vector<Point2D> pts, const Point2D& q, double r){
        std::vector<Point2D> inRadius;
        for (Point2D& p : v2){
            if (dist(p, q) <= r){
                inRadius.push_back(p);
            }
        }
        return inRadius;
    }

    void rangeQueryTest(const Tree2D<int>& t, std::vector<Point2D> points, Point2D q, double r){
        std::vector<Point2D> inRadius = pointsInRadius(points,q,r);
        std::vector<Point2D> pts = t.range(r, q, DistanceFunction::Euclidean);
        auto matcher = UnorderedElementsAreArray(inRadius.begin(), inRadius.end());
        ASSERT_THAT(pts, matcher);
    }

    std::vector<Point2D> v1{{35, 40}, {5, 45}, {25, 35}, {50,10}, {90,5}, {60,75}, {80,65}, {85,15}};
    std::vector<Point2D> v2{{35,60}, {20,45}, {60,80}, {80,40},{90,60}, {50,30}, {70,20}, {75,10}, {10,35}, {20,20}};
    std::vector<Point2D> v3{{10,20}, {11,5}, {5,10}, {5,8}, {15,2}, {20,1}};
    std::vector<Point2D> v4{{8,10}, {12,7}, {8,6}, {11,6}, {10,2}};
    std::vector<Point2D> v5{{-2, 1}, {7,0}, {0,-2}, {4,-3}, {10,4}, {8, -3}, {9, -6}, {4,5}, {13,-4}, {12, 7}, {7, -2}, {11, -3}, {5,-5}, {7, -7}};
    
    Tree2D<int> t1;  // Simpler tree
    Tree2D<int> t2; // More complicated deletion from lecture
    Tree2D<int> t3; // First example from lecture
    Tree2D<int> t4; // Breaking the invariant from lecture
    Tree2D<int> t5;
};

TEST_F(Test2D, findMin){
    int xmin = t1.findMin(0);
    int ymin = t1.findMin(1);

    ASSERT_EQ(xmin, 5);
    ASSERT_EQ(ymin, 5);

    try
    {
        int zmin = t1.findMin(2);
    }
    catch(const std::runtime_error& e)
    {
        ASSERT_EQ(e.what(), std::string("Invalid Dimension"));
    }

    // Insert minimum in both dimensions
    Point2D p{-1, -1};
    t1.insert(p);

    ASSERT_EQ(t1.findMin(0), -1);
    ASSERT_EQ(t1.findMin(1), -1);
}

TEST_F(Test2D, contains){
    ASSERT_TRUE(t1.contains({60,75}));
    t1.insert({16,27});
    ASSERT_TRUE(t1.contains({16, 27}));
    ASSERT_TRUE(t1.contains({90,5}));
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
TEST_F(Test2D, rangeQuery){

    std::vector<Point2D> q = t4.range(4.0, {10,7});
    ASSERT_EQ(q.size(), 4);
    std::vector<Point2D> expected{{8,10}, {12,7}, {8,6}, {11,6}};

    ASSERT_EQ(q, expected);
}

TEST_F(Test2D, rangeQuery2){
    // now that all the keys in the tree are in vectors, check and find the distance
    Point2D q {20, 35};
    const double r = 15;
    rangeQueryTest(t2, v2, q, r);
}


TEST_F(Test2D, rangeQuery3) {
    Point2D q{11,5};
    const double r = 4.0;
    rangeQueryTest(t2, v2, q, r);
}

TEST_F(Test2D, rangeQueryFuzz){
    std::uniform_int_distribution<int> dist;
    std::mt19937 rng;
    size_t n = 1000;
    // generate random points. 
    // run the range query check 5 times on 5 random points with r = 20;
}



TEST(Test3d, insertion3D){
    Tree3D<int> t3;

}


int main(){
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}