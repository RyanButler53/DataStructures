#include <list>
#include <forward_list>
#include <vector>
#include "unrolled-linked-list/unrolled-linked-list.hpp"
#include "quack/quack.hpp"
#include <chrono>
#include <random>
#include <string>
#include <iostream>
#include <concepts>
#include <thread>
#include <mutex>
#include <chrono>
#include <future>

/// @brief Quack needs an adapter to have the correct name for Queue concept
/// @tparam T type
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

template <typename Container>
concept Queue = requires(Container &container, const typename Container::value_type &value) {
    container.push_back(value);
    container.push_front(value);
    container.pop_front();
    container.pop_back();
    container.size();
};


template <Queue Container>
class Benchmark {
  private:
      std::string name_;
      size_t numOps_;
      std::vector<int> randomActions_;

  public:
    Benchmark(size_t numOps, std::string name) :numOps_{numOps}, name_{name} {
        long seed = time(0);
        std::mt19937 rng(seed);
        std::discrete_distribution<int> dist({1, 1, 1, 1});
        std::vector<int> actions;
        for (size_t a = 0; a < numOps_; ++a) {
            randomActions_.push_back(dist(rng));
        }
    }

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

    template <typename F, typename... Args> 
    static double measure(F f, Args&&... args){
        auto start = std::chrono::steady_clock::now();
        f(std::forward<Args...>(args)...);
        auto end = std::chrono::steady_clock::now();
        long ms = std::chrono::duration_cast<std::chrono::milliseconds>((end - start)).count();
        return double(ms / 1000.0);
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


int main() {

    size_t numActions = 1000000;

    Benchmark<std::list<int>> (numActions, "std::list")();
    Benchmark<UnrolledLinkedList<int, 8>> (numActions, "Unrolled Linked List K = 8")();
    Benchmark<UnrolledLinkedList<int, 64>> (numActions, "Unrolled Linked List K = 64")();
    Benchmark<UnrolledLinkedList<int, 256>> (numActions, "Unrolled Linked List K = 256")();
    Benchmark<QuackAdapter<int>> (numActions, "Quack")();
}