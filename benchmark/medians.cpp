#include "benchmark.hpp"
#include "heap/binomial.hpp"
#include "heap/d-ary.hpp"
#include "heap/fibonacci.hpp"
#include "interfaces.hpp"
#include "adaptors.hpp"
#include "heap/median.hpp"

#include <vector>
#include <random>
#include <chrono>



template <RunningMedian heap_t>
std::vector<int> runningMedians(std::vector<int>& stream){
    
    heap_t h;
    std::vector<int> medians;
    medians.reserve(stream.size());
    for (auto& x : stream){
        h.insert(x);
        medians.push_back(h.findMedian());
    }
    return medians;
}

int main(int argc, char** argv){
    size_t n = 100000;
    if (argc == 2){
        n = stoull(argv[1]);
    }

    long long seed = time(nullptr);
    std::mt19937 rng(seed);
    std::vector<int> numbers(n);
    std::iota(numbers.begin(), numbers.end(), 0ul);

    BenchmarkSuite suite("Running Median");
    suite.setConfig(n, 5);
    suite.addConfiguredTest("std::priority_queue", runningMedians<MedianHeap<PQAdaptor<int, int>, PQAdaptor<int, int>>>, std::ref(numbers));
    suite.addConfiguredTest("Binary Heap", runningMedians<MedianHeap<BinaryMinHeap<int>, BinaryHeap<int>>>, std::ref(numbers));
    suite.addConfiguredTest("DAry Heap D = 5", runningMedians<MedianHeap<DAryHeap<int, int, 5>, DAryHeap<int, int, 10, std::greater<int>>>>, std::ref(numbers));
    suite.addConfiguredTest("DAry Heap D = 10", runningMedians<MedianHeap<DAryHeap<int, int, 5>, DAryHeap<int, int, 10, std::greater<int>>>>, std::ref(numbers));
    suite.addConfiguredTest("Binomial Heap", runningMedians<MedianHeap<BinomialHeap<int, int>, BinomialHeap<int,int, std::greater<int>>>>, std::ref(numbers));
    suite.addConfiguredTest("Fibonacci Heap", runningMedians<MedianHeap<FibonacciHeap<int, int>, FibonacciHeap<int,int, std::greater<int>>>>, std::ref(numbers));

    suite.run();
    suite.resultsToCSV("runningMedian.csv");
}