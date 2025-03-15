
#include <concepts>
// #include "benchmark.hpp"
#include <vector>
#include <tuple>
#include <functional>
#include <iostream>
#include <cmath>
#include <matplot/matplot.h>

template <typename T>
concept BasicHeap = requires(T &heap,
                             T::value_type value) {
    heap();
    heap.empty();
    heap.size();
    heap.pop();
    heap.push(value);
    heap.top();
};

// There are 3 "heap based algorithms" I want to benchmark: 

// Kofn: Get the minimum K of a set of n numbers. Make N HUGE
// Heap Sort algorithm
// Dijkstra



int main() {
    using namespace matplot;
    std::vector<double> x = linspace(0, 2 * pi);
    std::vector<double> y = transform(x, [](auto x) { return sin(x); });
    
    // plot(x, y, "-o");
    // hold(on);
    // plot(x, transform(y, [](auto y) { return -y; }), "-.");
    // plot(x, transform(x, [](auto x) { return x / pi - 1.; }), "-:gs");
    // plot({1.0, 0.7, 0.4, 0.0, -0.4, -0.7, -1}, "k");
    std::vector<double> ys{0.0044, 0.0171, 0.0529, 0.3582, 1.3766};
    std::vector<double> xs{500, 1000, 2000, 5000, 10000};
    scatter(xs, ys, 6);
    show();
    return 0;
}