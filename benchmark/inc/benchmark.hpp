// Define a benchmark base class
#include <string>
#include <random>
#include <concepts>
#include <vector>
#include <thread>
#include <algorithm>

class Benchmark {

    /**
     * @brief Get the Actions object
     * 
     * @param numActions Number of unique actions required
     * @param begin Iterator to the beginning of the region to input
     * @param end Past the end Iterator to the end of the region to input. 
     */
    static void getActions(size_t numActions,
                                std::vector<int>::iterator begin, std::vector<int>::iterator end);

public:
    Benchmark(size_t numOps, std::string name);

    // Operator to do the benchmark.
    // Derived classes must implement this and may use multiple threads 
    virtual std::string operator()() const = 0;

    // Utility to get random actions.
    // Uses multiple threads to speed this process up 
    static std::vector<int> getRandomActions(size_t numOps, size_t numActions);


  protected:
    std::string name_;
    size_t numOps_;

    template <std::invocable F, typename... Args>
    static double measure(F f, Args &&...args);

};

template <std::invocable F, typename... Args> 
double Benchmark::measure(F f, Args&&... args){
    auto start = std::chrono::steady_clock::now();
    f(std::forward<Args...>(args)...);
    auto end = std::chrono::steady_clock::now();
    long ms = std::chrono::duration_cast<std::chrono::milliseconds>((end - start)).count();
    return double(ms / 1000.0);
}