#include "spatial/kdtree.hpp"
#include "benchmark.hpp"

// Want to benchmark and time how long each algorithm takes on the naive algorithm vs the kd tree algorithm. 

// 2D case

constexpr uint16_t NUM_QUERIES = 500;

template <size_t K>
class InputGenerator {

    public: 
    InputGenerator(size_t n):n_{n} {
        size_t seed = time(nullptr);
        rng_ = std::mt19937(seed);
        dist_ = std::uniform_real_distribution<double>(-10000, 10000);
    }

    ~InputGenerator() = default;


    using Point = std::array<int, K>;

    Rectangle<int, K> getBB(){
        std::uniform_int_distribution<int> lower(-11000, 0);
        std::uniform_int_distribution<int> upper(0, 11000);

        // Pick a start minimum. Then add bounds

        Rectangle<int, K> rq;
        for (size_t dim = 0; dim < K; ++dim){
            int low = lower(rng_);
            int high = upper(rng_);
            rq.insert(dim, low * 0.2, high* 0.2);
        }
        return rq;
    }

    double getRadius(){
        std::uniform_real_distribution<double> rdist(100, 1000);
        return rdist(rng_);

    }

    Point getPoint(){
        Point p;
        for (size_t dim = 0; dim < K; ++dim){
            double coord = dist_(rng_);
            p[dim] = coord;
        }
        return p;
    };

    void getRandomPoints(std::vector<Point>& points){

        for (size_t i = 0; i < n_; ++i){
            Point p = getPoint();
            points.push_back(p);
        }

    }

    size_t n_;
    std::mt19937 rng_;
    std::uniform_real_distribution<double> dist_;
};

// Functions:
template <size_t K>
class SpatialQuery {

    public: 
    SpatialQuery() = default;
    virtual ~SpatialQuery(){};

    using Point = std::array<int, K>;

    virtual std::vector<Point> knearest(const Point& q, size_t k) = 0;
    virtual std::vector<Point> radiusQuery(const Point& q, double r) = 0;
    virtual std::vector<Point> rectangleQuery(Rectangle<int, K> q) = 0;

};

template <size_t K>
struct NaiveQuery : public SpatialQuery<K> {
    using Point = SpatialQuery<K>::Point;

    private:

    std::vector<Point> points_;
    
    double dist(Point p1, Point p2){

        int sum = 0;
        for (size_t i = 0; i < K; ++i){
            int diff = p1[i] - p2[i];
            sum += (diff * diff);
        }
        return std::sqrt(sum);
    }

    public:

    NaiveQuery(InputGenerator<K> input) {
        input.getRandomPoints(points_);
    };
    ~NaiveQuery() {};

    std::vector<Point> knearest(const Point& q, size_t k){
        std::multimap<double, Point> distances;
        for (Point pt : points_){
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

    std::vector<Point> radiusQuery(const Point& q, double r){
        std::vector<Point> in;
        for (Point& p : points_){
            if (dist(p, q) <= r){
                in.push_back(p);
            }
        }
        return in;
    }

    std::vector<Point> rectangleQuery(Rectangle<int, K> q){
        std::vector<Point> in;
        for (auto p : points_){
            if (q.contains(p)){
                in.push_back(p);
            }
        }
        return in;
    }

};

template <size_t K>
class KDQuery : public SpatialQuery<K> {


    KDTree<int, K> tree_;

    public:

    using Point = SpatialQuery<K>::Point;

    KDQuery(InputGenerator<K> input){
        std::vector<Point> points;
        input.getRandomPoints(points);

        for (Point& p : points){
            tree_.insert(p);
        }
    }

    ~KDQuery() {};

    void addPoints(InputGenerator<K> input){
        std::vector<Point> points;
        input.getRandomPoints(points);

        for (Point& p : points){
            tree_.insert(p);
        }
    }

    std::vector<Point> knearest(const Point& q, size_t k) override {
        return tree_.kNearestNeighbors(q, k);
    }

    std::vector<Point> radiusQuery(const Point& q, double r) override {

        return tree_.radialRangeQuery(r, q);
    }

    std::vector<Point> rectangleQuery(Rectangle<int, K> q) override {
        return tree_.rectangleRangeQuery(q);
    }
};


// How fast (asymtotically) are KD Trees? 
// Given an already populated KD Tree with N items, how fast can we perform each query on average? 
// Given a vector of points , how quickly can can these queries be satisfied? 
// At what N value does a KD Tree become useful 

// Looking at the time to perform 100 queries


template <size_t K>
void NearestNeighbor(SpatialQuery<K>* query, std::vector<std::array<int, K>>& queries){

    for (uint16_t i = 0; i < NUM_QUERIES; ++i){
        query->knearest(queries[i], 1);
    }

}

template <size_t K>
void KNearest(SpatialQuery<K>* query, std::vector<std::array<int, K>> queries, size_t k){

    for (uint16_t i = 0; i < NUM_QUERIES; ++i){
        query->knearest(queries[i], k);
    }

}

template <size_t K>
void radius(SpatialQuery<K>* query, std::vector<std::array<int, K>> queries, std::vector<double> radii){

    for (uint16_t i = 0; i < NUM_QUERIES; ++i){
        query->radiusQuery(queries[i], radii[i]);
    }

}

template <size_t K>
void rectangle(SpatialQuery<K>* query, std::vector<Rectangle<int, K>> boxes){
    for (uint16_t i = 0; i < NUM_QUERIES; ++i){
        query->rectangleQuery(boxes[i]);
    }

}

int main(){

    BenchmarkSuite suite2D("2D");
    for (size_t n : std::vector<size_t>{ 1000, 5000, 10000, 20000}){
        suite2D.setConfig(n, 10);
        // Generate Input. 
        InputGenerator<2> gen2D(n);
        NaiveQuery<2> q2(gen2D);
        KDQuery<2> kd2(gen2D);
        std::vector<Rectangle<int, 2>> boxes;
        std::vector<double>radii;
        std::vector<std::array<int, 2>> queryPoints;
        for (size_t i = 0; i < NUM_QUERIES; ++i){
            boxes.push_back(gen2D.getBB());
            radii.push_back(gen2D.getRadius());
            queryPoints.push_back(gen2D.getPoint());
        }

        suite2D.addConfiguredTest("Nearest Naive", NearestNeighbor<2>,&q2, std::ref(queryPoints));
        suite2D.addConfiguredTest("Nearest KDTree", NearestNeighbor<2>, &kd2, std::ref(queryPoints));
        suite2D.addConfiguredTest("KNearest Naive", KNearest<2>,&q2, std::ref(queryPoints), 20);
        suite2D.addConfiguredTest("KNearest KDTree", KNearest<2>, &kd2, std::ref(queryPoints), 20);
        suite2D.addConfiguredTest("Radius Naive", radius<2>,&q2, std::ref(queryPoints), std::ref(radii));
        suite2D.addConfiguredTest("Radius KDTree", radius<2>, &kd2, std::ref(queryPoints), std::ref(radii));
        suite2D.addConfiguredTest("Rectangle Naive", rectangle<2>,&q2, std::ref(boxes));
        suite2D.addConfiguredTest("Rectangle KDTree", rectangle<2>, &kd2, std::ref(boxes));
        
        // suite2D.run();
    }

    // suite2D.resultsToCSV("2d.csv");

    // Initial Findings: When looking for BIG bounding boxes, in practice you end up searching a lot. 
    // Smaller bounding boxes net a sizeable speed up.  In practice, 1000 NN queries takes almost no time. 

    // 3D case: 
    // Want to _really_ show the runtime curves of these functions
    BenchmarkSuite suite3D("3D");
    InputGenerator<3> gen(20000);
    KDQuery<3> kd(InputGenerator<3>(0));
    for (size_t n = 20000; n <= 5000000; n += 20000){
        suite3D.setConfig(n, 10);
        // Generate Input. 
        std::cout << "Starting N = " << n << std::endl;
        // Add 20000 points
        kd.addPoints(gen);
        std::vector<Rectangle<int, 3>> boxes;
        std::vector<double>radii;
        std::vector<std::array<int, 3>> queryPoints;
        for (size_t i = 0; i < NUM_QUERIES; ++i){
            boxes.push_back(gen.getBB());
            radii.push_back(gen.getRadius());
            queryPoints.push_back(gen.getPoint());
        }

        suite3D.addConfiguredTest("KNearest KDTree", KNearest<3>, &kd, std::ref(queryPoints), 100);
        suite3D.addConfiguredTest("Radius KDTree", radius<3>, &kd, std::ref(queryPoints), std::ref(radii));
        suite3D.addConfiguredTest("Rectangle KDTree", rectangle<3>, &kd, std::ref(boxes));
        
        suite3D.run();

    }
    suite3D.resultsToCSV("3dlarge.csv");

}






