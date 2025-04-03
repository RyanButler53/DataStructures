#include "quack/quack.hpp"
#include <queue>
#include <list>
#include <iostream>
#include <numeric>
#include <chrono>
#include "interfaces.hpp"
#include "benchmark.hpp"

/// @brief Quack needs an adapter to have the correct name for Queue concept
template <typename T>
class QuackAdapter {
    Quack<T> q_;

public:
    using value_type = T;
    QuackAdapter() = default;
    ~QuackAdapter() = default;
    void push_back(const T &value) { q_.push(value); }
    void push_front(const T &value) { q_.push_front(value); }
    void pop_front() { q_.dequeue(); }
    void pop_back() { q_.pop(); }
    size_t size() { return q_.size(); }
};



template <Queue Container>
void exp1(size_t n){
    Container q;
    for (size_t i = 0; i < n; ++i){
        q.push_back(i);
    }
}

template <Queue Container>
void experiment2(size_t n){
    Container q;
    for (int i = 0; i < n; ++i){
        q.push_back(i);
    }
    for (int i = 0; i < n; ++i){
        q.pop_back();
    }
}

template <Queue Container>
void exp3(size_t n){
    Container q;
    for (size_t i = 0; i < n; ++i){
        q.push_back(i);
    }
    for (size_t i = 0; i < n; ++i){
        q.pop_front();
    }
}

template <Queue Container>
void exp4(size_t n){
    Container q;
    for (size_t i = 0; i < n; ++i){
        q.push_back(i);
    }
    for (size_t i = 0; i < n / 2; ++i)
    {
        q.pop_back();
        q.pop_front();
    }
}

int main(int argc, char **argv) {

    // Process Args
    size_t n = (argc > 1) ? strtoul(argv[1], nullptr, 10) : 100000UL;
    size_t numExperiments = (argc > 2) ? strtoul(argv[2], nullptr, 10) : 10UL;

    BenchmarkSuite suite("Quack vs List");
    suite.setConfig(n, numExperiments);
    suite.addConfiguredTest("List: Push Backs", exp1<std::list<int>>, std::ref(n));
    suite.addConfiguredTest("List: Pushes then pop backs", exp2<std::list<int>>, std::ref(n));
    suite.addConfiguredTest("List: Pushes then pop fronts", exp3<std::list<int>>, std::ref(n));
    suite.addConfiguredTest("List: Alternating pops", exp4<std::list<int>>, std::ref(n));

    suite.addConfiguredTest("Quack: Push Backs", exp1<QuackAdapter<int>>, std::ref(n));
    suite.addConfiguredTest("Quack: Pushes then pop backs", exp2<QuackAdapter<int>>, std::ref(n));
    suite.addConfiguredTest("Quack: Pushes then pop fronts", exp3<QuackAdapter<int>>, std::ref(n));
    suite.addConfiguredTest("Quack: Alternating pops", exp4<QuackAdapter<int>>, std::ref(n));

    suite.run();
    suite.resultsToCSV("quack.csv");
    return 0;
}
