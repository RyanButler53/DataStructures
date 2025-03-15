
#include <concepts>
// #include "benchmark.hpp"
#include <vector>
#include <tuple>
#include <functional>
#include <iostream>

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
template <typename F, typename... Args>
double measure(F func, Args&&... args) {
    auto start = std::chrono::steady_clock::now();
    std::apply(func, args...);
    auto end = std::chrono::steady_clock::now();
    long ms = std::chrono::duration_cast<std::chrono::milliseconds>((end - start)).count();
    return double(ms / 1000.0);
};
//Function that takes in a function and vector of tuple of arguments to it
template <typename F, typename... Args>
void hof(F func, std::vector<std::tuple<Args...>>& args){
    for (auto&& a : args){
        double result = measure(func, a);
        std::cout << result << std::endl;
    }
}

int add(int x, int y){
    std::cout << x + y << std::endl;
    return x + y;
}

int main() {
    std::vector<std::tuple<int, int>> v{{1, 2}, {3, 4}, {4, 6}};
    hof(add, v);
}