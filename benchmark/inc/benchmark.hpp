// Define a benchmark base class
#pragma once

#include <string>
#include <random>
#include <concepts>
#include <vector>
#include <thread>
#include <algorithm>
#include <functional>
#include <utility>
#include <cmath>
#include <numeric>
#include <filesystem>
#include <fstream>

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

struct BenchmarkResults {
    std::string testName_;
    size_t inputSize_;
    size_t numTrials_;
    double avgTime_;
    double stdev_;

    std::string to_string() const;
};

// Takes in a function, args, name, number of runs, size of test
class BenchmarkTest {
  private:
    // data
      std::function<void()> fn_;
      std::string name_;
      size_t inputSize_; // Number of trials
      size_t numTrials_;

      double average(std::vector<double>& vec) const;
      double stdev(std::vector<double> &vec) const;

  public:
      // Benchmark Test can only run one single test.
      // With a supplied argument, n, trials
      // Users should NOT have to curry their functions
      template <typename F, typename... Args>
      BenchmarkTest(std::string name, size_t inputSize, size_t numTrials, F func, Args &&...args);


      BenchmarkResults operator()();

      double measure();
};

template <typename F, typename... Args>
BenchmarkTest::BenchmarkTest(std::string name, size_t inputSize, size_t numTrials, F func, Args &&... args):
    name_{name}, inputSize_{inputSize},numTrials_{numTrials}{
    fn_ = [&func, &args...]() { func(std::forward<Args>(args)...); };
}

class BenchmarkSuite {

    std::vector<BenchmarkTest> tests_;
    std::string suiteName_;

  public:
    BenchmarkSuite(std::string suiteName); // make this parallel?

    void addTest(BenchmarkTest& test);
    // Runs and sends output to CSV
    void run(std::string filename = "");

};