#include "benchmark.hpp"
#include "interfaces.hpp"
#include "spatial/kdtree.hpp"
#include "spatial/rectangle.hpp"

// How fast (asymtotically) are KD Trees? 
// Given an already populated KD Tree with N items, how fast can we perform each query on average? 
// Given a vector of points , how quickly can can these queries be satisfied? 
// At what N value does a KD Tree become useful over a naive query? 

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

template <size_t K>
struct NaiveQuery {
    using key_t = std::array<int, K>;
    using rect_t = Rectangle<int, K> ;

    NaiveQuery() = default;
    ~NaiveQuery() {};

    void insert(key_t pt){
        points_.push_back(pt);
    }

    key_t nearestNeighbor(const key_t& q) const {
        std::vector<std::pair<double, key_t>> distances;
        for (key_t pt : points_){
            distances.push_back({dist(pt, q), pt});
        } 
        auto min = std::ranges::min_element(distances);
        return min->second;
    }

    std::vector<key_t> nearestNeighbor(const key_t& q, size_t k) const {
        if (k > points_.size()){
            return points_;
        }
        std::multimap<double, key_t> distances;
        for (key_t pt : points_){
            distances.insert({dist(pt, q), pt});
        }
        auto iter = distances.begin();
        std::vector<key_t> nearest;
        for (size_t i = 0; i < k; ++i){
            nearest.push_back(iter->second);
            ++iter;
        }
        return nearest;
    }

    std::vector<key_t> rangeQuery(const key_t& q, double r) const{
        std::vector<key_t> in;
        for (const key_t& p : points_){
            if (dist(p, q) <= r){
                in.push_back(p);
            }
        }
        return in;
    }

    std::vector<key_t> rangeQuery(const rect_t& q) const {
        std::vector<key_t> in;
        for (auto p : points_){
            if (q.contains(p)){
                in.push_back(p);
            }
        }
        return in;
    }

    private:

    std::vector<key_t> points_;
    
    double dist(key_t p1, key_t p2) const {

        int sum = 0;
        for (size_t i = 0; i < K; ++i){
            int diff = p1[i] - p2[i];
            sum += (diff * diff);
        }
        return std::sqrt(sum);
    }
};

// Looking at the time to perform 100 queries
template <SpatialStructure Structure, size_t K>
void setup(Structure& structure, const std::vector<std::array<int, K>>& points){
    for (auto& p : points){
        structure.insert(p);
    }
}


template <SpatialStructure Structure, size_t K>
void KNearest(const Structure& structure, const std::vector<std::array<int, K>>& queries, size_t k){
    for (uint16_t i = 0; i < NUM_QUERIES; ++i){
        structure.nearestNeighbor(queries[i], k);
    }
}

template <SpatialStructure Structure, size_t K>
void radius(const Structure& structure, const std::vector<std::array<int, K>>& queries, std::vector<double> radii){

    for (uint16_t i = 0; i < NUM_QUERIES; ++i){
        structure.rangeQuery(queries[i], radii[i]);
    }
}

template <SpatialStructure Structure, size_t K>
void rectangle(const Structure& structure, const std::vector<Rectangle<int, K>>& boxes){
    for (uint16_t i = 0; i < NUM_QUERIES; ++i){
        structure.rangeQuery(boxes[i]);
    }
}

int main(){

    BenchmarkSuite suite2D("2D");
    for (size_t n : std::vector<size_t>{1000, 5000, 10000, 20000}){
        suite2D.setConfig(n, 10);
        // Generate Input. 
        InputGenerator<2> gen2D(n);
        NaiveQuery<2> naive;
        Tree2D<int> kdtree;
        std::vector<std::array<int, 2>> points;
        gen2D.getRandomPoints(points);
        setup(naive, points);
        setup(kdtree, points);
        std::vector<Rectangle<int, 2>> boxes;
        std::vector<double>radii;
        std::vector<std::array<int, 2>> queryPoints;
        for (size_t i = 0; i < NUM_QUERIES; ++i){
            boxes.push_back(gen2D.getBB());
            radii.push_back(gen2D.getRadius());
            queryPoints.push_back(gen2D.getPoint());
        }

        suite2D.addConfiguredTest("KNearest Naive", KNearest<NaiveQuery<2>, 2>, std::cref(naive), std::cref(queryPoints),20);
        suite2D.addConfiguredTest("KNearest KDTree", KNearest<Tree2D<int>, 2>, std::cref(kdtree), std::cref(queryPoints),20);
        suite2D.addConfiguredTest("Radius Naive", radius<NaiveQuery<2>, 2>, std::cref(naive), std::cref(queryPoints), std::cref(radii));
        suite2D.addConfiguredTest("Radius KDTree", radius<Tree2D<int>, 2>, std::cref(kdtree), std::cref(queryPoints), std::cref(radii));
        suite2D.addConfiguredTest("Rectangle Naive", rectangle<NaiveQuery<2>, 2>, std::cref(naive), std::cref(boxes));
        suite2D.addConfiguredTest("Rectangle KDTree", rectangle<Tree2D<int>, 2>, std::cref(kdtree), std::cref(boxes));
        
        suite2D.run();
    }

    suite2D.resultsToCSV("2d.csv");

    // Initial Findings: When looking for BIG bounding boxes, in practice you end up searching a lot. 
    // Smaller bounding boxes net a sizeable speed up.  
    // In practice, 1000 NN queries takes almost no time. Not worthwhile to benchmark. 

    // 3D case: 
    BenchmarkSuite suite3D("3D");
    InputGenerator<3> gen(20000);
    Tree3D<int> kd;
    for (size_t n = 20000; n <= 100000; n += 20000){
        suite3D.setConfig(n, 10);
        std::cout << "Starting N = " << n << std::endl;

        // Add 20000 points
        std::vector<std::array<int, 3>> points;
        gen.getRandomPoints(points);
        setup(kd, points);

        // Get other inputs. 
        std::vector<Rectangle<int, 3>> boxes;
        std::vector<double>radii;
        std::vector<std::array<int, 3>> queryPoints;
        for (size_t i = 0; i < NUM_QUERIES; ++i){
            boxes.push_back(gen.getBB());
            radii.push_back(gen.getRadius());
            queryPoints.push_back(gen.getPoint());
        }

        suite3D.addConfiguredTest("KNearest KDTree", KNearest<Tree3D<int>, 3>, std::cref(kd), std::ref(queryPoints), 100);
        suite3D.addConfiguredTest("Radius KDTree", radius<Tree3D<int>, 3>, std::cref(kd), std::ref(queryPoints), std::ref(radii));
        suite3D.addConfiguredTest("Rectangle KDTree", rectangle<Tree3D<int>, 3>, std::cref(kd), std::ref(boxes));
        
        suite3D.run();

    }
    suite3D.resultsToCSV("3dlarge.csv");
}
