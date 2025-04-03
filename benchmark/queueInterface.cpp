#include <list>
#include <forward_list>
#include <vector>
#include "unrolled-linked-list/unrolled-linked-list.hpp"
#include "quack/quack.hpp"
#include <random>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <future>
#include "benchmark.hpp"

#include "interfaces.hpp"
// This file still needs to be rewritten

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
class Benchmark_Queue : Benchmark {
  private:

      std::vector<int> randomActions_;

  public:
    Benchmark_Queue(std::string name, std::vector<int>& actions) : 
        Benchmark(actions.size(), name) {}

    // Functions to test. Custom for each Benchmark suite
    void testPushing() const{
        Container c;
        for (size_t i = 0; i < numOps_; ++i){
            c.push_back(i);
        }
        for (size_t i = 0; i < numOps_; ++i) {
            c.push_front(i);
        }
        return;
    }

    void testAlternatingPush() const{
        Container c;
        for (size_t i = 0; i < numOps_; ++i)
        {
            c.push_front(i);
            c.push_back(i);
        }
    }

    void testAlternatingPushPop()const {
        Container c;
        for (size_t i = 0; i < numOps_; ++i) {
            c.push_front(i);
            c.push_back(i);
        }
        for (size_t i = 0; i < numOps_; i++)
        {
            c.pop_back();
            c.pop_front();
        }
    }

    void testRandomArguments() const{
        Container c;
        for (int action : randomActions_) {
        switch (action)
            {
            case 0:
                c.push_front(action);
                break;
            case 1:
                c.push_back(action);
                break;
            case 2:
                if (c.size() != 0){
                    c.pop_front();
                }
                break;
            case 3:
                if (c.size() != 0){
                    c.pop_back();
                }
                break;
            }
        }
        return;
    }

    // Runs the benchmark test suite, prints the results
    std::string operator()() const  {
        std::vector<double> times;
        std::vector<std::future<double>> futures;
        futures.emplace_back(std::async([*this]()
                            { return measure([*this]()
                                                { return testPushing(); }); }));
        futures.emplace_back(std::async([*this]()
                            { return measure([*this]()
                                    { return testAlternatingPush(); }); }));
        futures.emplace_back(std::async([*this]()
                            { return measure([*this]()
                                            { return testAlternatingPushPop(); }); }));
        futures.emplace_back(std::async([*this]()
                            { return measure([*this]()        
                                            { return testRandomArguments(); }); }));

        for (std::future<double> &f : futures)
        {
            times.push_back(f.get());
        }
        stringstream ss;
        ss << "Results for " << name_ << ": " << times[0] << " " << times[1] << " " << times[2] << " " << times[3];
        std::cout << ss.str() << std::endl;
        return ss.str();
    }
};


int main(int argc, char** argv) {

    size_t numOps = 10000;
    if (argc == 2){
        numOps = atoi(argv[1]);
    }

    std::vector<int> actions = Benchmark::getRandomActions(numOps, 4);
    Benchmark_Queue<std::list<int>>("std::list", actions)();
    Benchmark_Queue<UnrolledLinkedList<int, 8>> ("Unrolled Linked List K = 8", actions)();
    Benchmark_Queue<UnrolledLinkedList<int, 64>> ("Unrolled Linked List K = 64", actions)();
    Benchmark_Queue<UnrolledLinkedList<int, 256>> ("Unrolled Linked List K = 256", actions)();
    Benchmark_Queue<QuackAdapter<int>> ("Quack", actions)();
}