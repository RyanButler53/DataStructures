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
#include <iostream>
#include <fstream>
#include "launching-threadpool/launching-threadpool.hpp"

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

class BenchmarkSuite {

    // Simply defines the interface
    class BenchmarkConcept {
      public:
        virtual BenchmarkResults runTest() = 0;
        virtual ~BenchmarkConcept() = default;
    };

   // Model
    template <typename F, typename... Args>
    class BenchmarkTest : public BenchmarkConcept {

        F func_;
        std::tuple<Args...> args_;

        std::string name_;
        size_t inputSize_;
        size_t numTrials_;

      public:
        BenchmarkTest(std::string name, size_t inputSize, size_t numTrials, F&& func, Args &&...args):
            name_{name}, inputSize_{inputSize}, numTrials_{numTrials}, func_{std::move(func)}, 
            args_{std::forward_as_tuple(args...)} {
        }

        ~BenchmarkTest() = default;

        BenchmarkResults runTest(){
            std::vector<double> times;
            for (size_t i = 0; i < numTrials_; ++i) {
                times.push_back(measure());
            }
            return BenchmarkResults{name_, inputSize_, numTrials_, average(times), stdev(times)};
        }

        double average(std::vector<double>& vec) const {
            double sum = std::accumulate(vec.begin(), vec.end(), 0.0);
            return sum / double(numTrials_);
        }

        double stdev(std::vector<double> &vec)const {
            double avg = average(vec);
            double deviation = std::accumulate(vec.begin(), vec.end(), 0.0, [&avg](double acc, double newVal)
                                               { return (avg - newVal) * (avg - newVal); });
            return std::sqrt(deviation / numTrials_);
        }

        double measure() {
            auto start = std::chrono::steady_clock::now();
            std::apply(func_, args_);
            auto end = std::chrono::steady_clock::now();
            long ms = std::chrono::duration_cast<std::chrono::milliseconds>((end - start)).count();
            return double(ms / 1000.0);
        };
    };

    // Private Data
    std::string suiteName_;
    size_t testInputSizes_;
    size_t testTrials_;

    std::vector<BenchmarkConcept *> tests_;

  public:

    BenchmarkSuite(std::string suiteName); // make this parallel?

    ~BenchmarkSuite();

    template <typename F, typename... Args>
    void addTest(std::string testName, size_t inputSize, size_t numTrials, F func, Args &&...args) {
        tests_.push_back(new BenchmarkTest(testName, inputSize, numTrials, std::move(func), std::forward<Args>(args)...));
    }

    template <typename F, typename... Args>
    void addConfiguredTest(std::string testName, F func, Args &&...args) {
        tests_.push_back(new BenchmarkTest(testName, testInputSizes_, testTrials_, std::move(func), std::forward<Args>(args)...));
    }

    void setConfig(size_t inputSize, size_t numTrials);

    // Runs and sends output to CSV
    void run(std::string filename = "");

};