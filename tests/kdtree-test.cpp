#include "spatial/kdtree.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <array>
#include <iostream>
#include <algorithm>
#include <map>
#include <random>
#include <ranges>

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

    template <typename T>
    double dist(std::array<T,2> p1, std::array<T,2> p2){
        T dx = p1[0]-p2[0];
        T dy = p1[1] - p2[1];
        return std::sqrt((dx * dx) + (dy * dy));
    }

    template <typename point_t>
    std::vector<point_t> pointsInRadius(std::vector<point_t> pts, const point_t& q, double r){
        std::vector<point_t> inRadius;
        for (point_t& p : pts){
            if (dist(p, q) <= r){
                inRadius.push_back(p);
            }
        }
        return inRadius;
    }

    void rangeQueryTest(const Tree2D<int>& t, std::vector<Point2D> points, Point2D q, double r){
        std::vector<Point2D> inRadius = pointsInRadius(points,q,r);
        std::vector<Point2D> pts = t.rangeQuery(q, r, DistanceFunction::Euclidean);
        auto matcher = UnorderedElementsAreArray(inRadius.begin(), inRadius.end());
        ASSERT_THAT(pts, matcher);
    }

    void nearestNeighborTest(const Tree2D<int>& t, std::vector<Point2D> points, const Point2D& q){
        std::multimap<double, Point2D> distances;
        for (Point2D pt : points){
            distances.insert({dist(pt, q), pt});
        }
        Point2D tree_nn = t.nearestNeighbor(q, DistanceFunction::Euclidean);
        Point2D actual_nn = distances.begin()->second;
        ASSERT_EQ(tree_nn, actual_nn);
    }

    std::vector<Point2D> v1{{35, 40}, {5, 45}, {25, 35}, {50,10}, {90,5}, {60,75}, {80,65}, {85,15}};
    std::vector<Point2D> v2{{35,60}, {20,45}, {60,80}, {80,40},{90,60}, {50,30}, {70,20}, {75,10}, {10,35}, {20,20}};
    std::vector<Point2D> v3{{10,20}, {11,5}, {5,10}, {5,8}, {15,2}, {20,1}};
    std::vector<Point2D> v4{{8,10}, {12,7}, {8,6}, {11,6}, {10,2}};
    
    Tree2D<int> t1;  // Simpler tree
    Tree2D<int> t2; // More complicated deletion from lecture
    Tree2D<int> t3; // First example from lecture
    Tree2D<int> t4; // Breaking the invariant from lecture
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
}

TEST_F(Test2D, delete1){
    t2.remove({35,60});
    ASSERT_FALSE(t2.contains({35,60}));
    ASSERT_EQ(t2.findMin(1), 10);

    t2.remove({10, 35});
    ASSERT_FALSE(t2.contains({10, 35}));
    ASSERT_EQ(t2.findMin(0),20);

    t3.remove({10,20});
    ASSERT_FALSE(t3.contains({10,20}));
    
}
TEST_F(Test2D, rangeQuery){

    std::vector<Point2D> q = t4.rangeQuery({10,7}, 4.0);
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

TEST_F(Test2D, nearestNeighbor1){

    Point2D nn = t2.nearestNeighbor({50,60});
    Point2D expected{35,60};
    ASSERT_EQ(nn, expected);

    nn = t2.nearestNeighbor({35,30});
    expected = {50,30};
    ASSERT_EQ(nn, expected);

    nn = t2.nearestNeighbor({100,100});
    expected = {90,60};
    ASSERT_EQ(nn, expected);
}

TEST_F(Test2D, nearestNeighbor2){
    nearestNeighborTest(t3, v3, {12, 6});
    nearestNeighborTest(t4, v4, {11,11});
}

TEST_F(Test2D, kNearestNeighbor1){
    Point2D query = {35, 30};
    std::vector<Point2D> exp{{50,30},{20,20}, {20,45}};
    std::vector<Point2D> nn = t2.nearestNeighbor(query, 3);
    ASSERT_EQ(nn, exp);
}

TEST_F(Test2D, RectangleBoundingBox){
    Rectangle<int,2> bounds;
    bounds.insert(0, 59, 95);
    bounds.insert(1, 39, 81);
    ASSERT_TRUE(bounds.contains({60,80}));
    ASSERT_TRUE(bounds.contains({80,40}));
 
    // Empty bounds means entire space
    bounds.clear();
    // Previously outside of the bounds should be in bounds
    ASSERT_TRUE(bounds.contains({20,60}));

    // bounds the x dimension, all y values are valid. 
    bounds.insert(0, 59, 95);
    ASSERT_TRUE(bounds.contains({60, 150}));
    ASSERT_TRUE(bounds.contains({80, -150}));
    ASSERT_FALSE(bounds.contains({30, 75}));
}

TEST_F(Test2D, RectangleRange){
    Rectangle<int, 2> bounds;
    bounds.insert(0, 59, 95);
    bounds.insert(1, 39, 81);
    std::vector<Point2D> points = t2.rangeQuery(bounds);
    std::vector<Point2D> exp{{60,80}, {80,40}, {90,60}};
    auto matcher = UnorderedElementsAreArray(exp.begin(), exp.end());
    ASSERT_THAT(points, matcher);

    Rectangle<int, 2> bounds2;
    bounds2.insert(0, 33, 65);
    points = t2.rangeQuery(bounds2);
    exp = {{60,80}, {35, 60}, {50,30}};
    matcher = UnorderedElementsAreArray(exp.begin(), exp.end());
    ASSERT_THAT(points, matcher);
}

/// FUZZ TEST WITH N DIMENSIONS
class KDTreeTest : public testing::TestWithParam<size_t>{
    
    public:

    static constexpr size_t DIM = 4;

    using Point = std::array<int, DIM>;
    template <typename T>
    double dist(std::array<T,DIM> p1, std::array<T,DIM> p2){

        T sum = 0;
        for (size_t i = 0; i < DIM; ++i){
            T diff = p1[i] - p2[i];
            sum += (diff * diff);
        }
        return std::sqrt(sum);
    }

    Rectangle<int, DIM> getBB(){
        std::uniform_int_distribution<int> lower(-11000, 0);
        std::uniform_int_distribution<int> upper(0, 11000);
        Rectangle<int, DIM> rq;
        for (size_t dim = 0; dim < DIM; ++dim){
            int low = lower(rng_);
            int high = upper(rng_);
            rq.insert(dim, low, high);
        }
        return rq;
    }

    double getRadius(){
        std::uniform_real_distribution<double> rdist(10, 80);
        return rdist(rng_);

    }

    Point getPoint(){
        Point p;
        for (size_t dim = 0; dim < DIM; ++dim){
            double coord = dist_(rng_);
            p[dim] = coord;
        }
        return p;
    };

    void getRandomPoints(int n, std::vector<Point>& points){

        for (size_t i = 0; i < n; ++i){
            Point p = getPoint();
            points.push_back(p);
        }

    }

    std::vector<Point> knearest(std::vector<Point>& points, const Point& q, size_t k){
        std::multimap<double, Point> distances;
        for (Point pt : points){
            distances.insert({dist(pt, q), pt});
        }
        auto iter = distances.begin();
        std::vector<Point> nearest;
        for (size_t i = 0; i < k; ++i){
            nearest.push_back(iter->second);
            ++iter;
        }
        return nearest;
    }

    std::vector<Point> inRadius(std::vector<Point>& points, const Point& q, double r){
        std::vector<Point> in;
        for (Point& p : points){
            if (dist(p, q) <= r){
                in.push_back(p);
            }
        }
        return in;
    }

    std::vector<Point> inBox(std::vector<Point>& points, Rectangle<int, DIM>q){
        std::vector<Point> in;
        for (auto p : points){
            if (q.contains(p)){
                in.push_back(p);
            }
        }
        return in;
    }

    void SetUp() override {
        size_t seed = time(nullptr);
        rng_ = std::mt19937(seed);
        dist_ = std::uniform_real_distribution<double>(-10000, 10000);
    }

    std::mt19937 rng_;
    std::uniform_real_distribution<double> dist_;

};


TEST_P(KDTreeTest, NearestNeighbor){
    std::vector<Point> points;
    int n = this->GetParam();
    getRandomPoints(n, points);
    KDTree<int, DIM> t;

    for (auto p : points){
        t.insert(p);
    }

    for (size_t query_i = 0; query_i < 15; ++query_i){
        Point q = getPoint();
        Point actual = knearest(points,q , 1)[0];
        Point exp = t.nearestNeighbor(q);
        ASSERT_EQ(actual, exp);
    }
}

TEST_P(KDTreeTest, KNearestNeighbors){
    std::vector<Point> points;
    int n = this->GetParam();
    getRandomPoints(n, points);
    KDTree<int, DIM> t;

    for (auto p : points){
        t.insert(p);
    }
    for (size_t query_i = 0; query_i < 15; ++query_i){
        Point q = getPoint();
        std::vector<Point> actual = knearest(points,q , 20);
        
        std::vector<Point> exp = t.nearestNeighbor(q, 20);
        if (std::ranges::equal(actual, exp)){
            ASSERT_EQ(actual, exp);
            return;
        } else { // Check if there is a duplicate distance 
            for (size_t i = 0; i < actual.size(); ++i){
                ASSERT_EQ(dist(actual[i], q), dist(exp[i], q));
            }
        }
    }
}

TEST_P(KDTreeTest, RadiusQuery){
    std::vector<Point> points;
    int n = this->GetParam();
    getRandomPoints(n, points);
    KDTree<int, DIM> t;

    for (auto p : points){
        t.insert(p);
    }
    for (size_t query_i = 0; query_i < 15; ++query_i){
        double r = getRadius();
        Point q = getPoint();
        std::vector<Point> actual = inRadius(points, q, r);
        std::vector<Point> exp = t.rangeQuery(q, r);
        
        auto matcher = UnorderedElementsAreArray(actual.begin(), actual.end());
        ASSERT_THAT(exp, matcher);
    }
}

TEST_P(KDTreeTest, BoundingBoxQuery){
    int n = this->GetParam();
    std::vector<Point> points;
    getRandomPoints(n, points);
    KDTree<int, DIM> t;

    for (auto p : points){
        t.insert(p);
    }
    for (size_t query_i = 0; query_i < 10; ++query_i){
        Rectangle<int, DIM> bbox = getBB();
        std::vector<Point> actual = inBox(points, bbox);
        std::vector<Point> exp = t.rangeQuery(bbox);
        
        auto matcher = UnorderedElementsAreArray(actual.begin(), actual.end());
        ASSERT_THAT(exp, matcher);
    }

}

INSTANTIATE_TEST_SUITE_P(KDFuzzTest,
    KDTreeTest,
    testing::Values(100, 1000, 5000));

int main(){
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}