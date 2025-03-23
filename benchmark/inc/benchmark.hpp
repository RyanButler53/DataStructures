// Define a benchmark library
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
#include <map>

namespace BenchmarkLib {
    // Utiility Functions 
    double average(std::vector<double> &vec);
    double stdev(std::vector<double> &vec);

    // Single Function Measuring
    template <typename F, typename... Args>
    double measure(F func, Args... args){
        auto start = std::chrono::steady_clock::now();
        func(std::forward<Args...>(args)...);
        auto end = std::chrono::steady_clock::now();
        long ms = std::chrono::duration_cast<std::chrono::milliseconds>((end - start)).count();
        return double(ms / 1000.0);
    }
}

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

struct GroupedResults {
    std::vector<size_t> inputSizes_;
    std::vector<double> times_;
    std::vector<double> stdevs_;
};

class BenchmarkSuite {

    // Simply defines the interface
    class BenchmarkConcept {
      public:
        virtual BenchmarkResults runTest() = 0;
        virtual ~BenchmarkConcept() = default;
    };

    template <typename F, typename... Args>
    class BenchmarkTest : public BenchmarkConcept {
    
        F func_;
        std::tuple<Args...> args_;
    
        std::string name_;
        size_t inputSize_;
        size_t numTrials_;
    
      public:
        BenchmarkTest(std::string name, size_t inputSize, size_t numTrials, F&& func, Args &&... args):
            name_{name}, inputSize_{inputSize}, numTrials_{numTrials}, func_{std::move(func)}, 
            args_{std::forward_as_tuple(args...)} {
        }
    
        ~BenchmarkTest() = default;
    
        BenchmarkResults runTest(){
            std::vector<double> times;
            for (size_t i = 0; i < numTrials_; ++i) {
                times.push_back(measure());
            }
            return BenchmarkResults{name_, inputSize_, numTrials_, BenchmarkLib::average(times), BenchmarkLib::stdev(times)};
        }
    
        double measure() {
            auto start = std::chrono::steady_clock::now();
            std::apply(func_, args_);
            auto end = std::chrono::steady_clock::now();
            long ms = std::chrono::duration_cast<std::chrono::milliseconds>((end - start)).count();
            return double(ms / 1000.0);
        };
    };

    template <typename F, typename Arg>
    class BenchmarkVaryingInput : public BenchmarkConcept {

        F func_;
        std::vector<Arg> argsVec_;

        std::string name_;
        size_t inputSize_;
        size_t numTrials_;

      public:
        BenchmarkVaryingInput(std::string name, size_t inputSize, size_t numTrials, F&& func, std::vector<Arg>& argsVec):
            name_{name}, inputSize_{inputSize}, numTrials_{numTrials}, func_{std::move(func)}, 
            argsVec_{std::move(argsVec)} {
        }

        ~BenchmarkVaryingInput() = default;

        BenchmarkResults runTest(){
            std::vector<double> times;
            for (Arg& args : argsVec_) {
                times.push_back(measure(args));
            }
            return BenchmarkResults{name_, inputSize_, numTrials_, BenchmarkLib::average(times), BenchmarkLib::stdev(times)};
        }

        double measure(Arg& arg) {
            auto start = std::chrono::steady_clock::now();
            func_(arg);
            auto end = std::chrono::steady_clock::now();
            long ms = std::chrono::duration_cast<std::chrono::milliseconds>((end - start)).count();
            return double(ms / 1000.0);
        };
    };

    // Private Data
    std::vector<BenchmarkConcept *> tests_;
    std::vector<BenchmarkResults> results_;
    std::string suiteName_;
    size_t testInputSizes_;
    size_t testTrials_;

    // Private Functions

    // Clears out already run tests. Cleans up their memory;
    void clearTests();

    void runTests(size_t start, size_t end, std::vector<BenchmarkResults>& results);

  public:

    BenchmarkSuite(std::string suiteName);
    ~BenchmarkSuite();

    template <typename F, typename... Args>
    void addTest(std::string testName, size_t inputSize, size_t numTrials, F func, Args &&...args) {
        tests_.push_back(new BenchmarkTest(testName, inputSize, numTrials, std::move(func), std::forward<Args>(args)...));
    }

    template <typename F, typename... Args>
    void addConfiguredTest(std::string testName, F func, Args &&...args) {
        tests_.push_back(new BenchmarkTest(testName, testInputSizes_, testTrials_, std::move(func), std::forward<Args>(args)...));
    }

    template <typename F, typename Arg>
    void addVaryingInputs(std::string testName, F func, std::vector<Arg> inputs){
        tests_.push_back(new BenchmarkVaryingInput(testName, testInputSizes_, inputs.size(), std::move(func), inputs));
    }

    void setConfig(size_t inputSize, size_t numTrials);

    // Runs all tests
    void run();

    // Sends the results of all tests run so far to a CSV
    void resultsToCSV(std::string filename);

    // Gets the name of the Benchmark Suite
    std::string getName() const { return suiteName_; }

    // Gets a vector of benchmark results
    std::vector<BenchmarkResults> getResults() const { return results_;} 

    // Groups the results by the test name
    std::map<std::string, GroupedResults> getGroupedResults();

    // #ifdef FOUND_MATPLOT_PLUS_PLUS
    // void plot(std::string filename);
    // #endif
};
