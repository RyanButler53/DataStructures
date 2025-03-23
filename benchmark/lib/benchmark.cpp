#include "benchmark.hpp"    

Benchmark::Benchmark(size_t numOps, std::string name):numOps_{numOps}, name_{name}{}

std::vector<int> Benchmark::getRandomActions(size_t numOps, size_t numActions){
    std::vector<int> actions(numOps);
    unsigned numThreads = std::thread::hardware_concurrency();

    if (numOps < 100000 or numThreads == 0){
        getActions(numActions, actions.begin(), actions.end());
        return actions;
    } else {
        // Use up to 8 threads to load it in faster.
        numThreads = std::min(8u, numThreads);
        std::vector<std::thread> threads;
        // Even splits case
        size_t perThread = numOps / numThreads;
        for (size_t i = 0; i < numThreads; ++i){
            threads.emplace_back(getActions, numActions, 
                        actions.begin() + i * perThread, 
                        actions.begin()+ (i + 1) * perThread);
        }
        // Main thread does the remainder (if there is one);
        getActions(numActions, actions.begin() + perThread * numThreads, actions.end());
        for (std::thread& t : threads){
            t.join();
        }
    }
    return actions;
}

void Benchmark::getActions(size_t numActions,
            std::vector<int>::iterator begin, std::vector<int>::iterator end){

    long seed = time(0);
    std::mt19937 rng(seed);
    std::vector<int> weights(numActions);
    std::ranges::fill(weights, 1);
    std::discrete_distribution<int> dist(weights.begin(), weights.end());
    std::vector<int> actions;

    while (begin != end){
        *begin = dist(rng);
        ++begin;
    }
}

// NEW BENCHMARKING LIBRARY

std::string BenchmarkResults::to_string() const {
    std::stringstream ss;
    ss << testName_ << ", " << inputSize_ << ", " << numTrials_ << ", " << avgTime_ << ", " << stdev_;
    return ss.str();
}

double BenchmarkLib::average(std::vector<double>& vec) {
    double sum = std::accumulate(vec.begin(), vec.end(), 0.0);
    return sum / double(vec.size());
}

double BenchmarkLib::stdev(std::vector<double> &vec) {
    double avg = average(vec);
    double deviation = std::accumulate(vec.begin(), vec.end(), 0.0, [&avg](double acc, double newVal)
                                       { return (avg - newVal) * (avg - newVal); });
    return std::sqrt(deviation / vec.size());
}

// BENCHMARK SUITE

BenchmarkSuite::BenchmarkSuite(std::string suitename):suiteName_{suitename}{}

BenchmarkSuite::~BenchmarkSuite(){
    for (BenchmarkConcept* test : tests_){
        delete test;
    }
}

void BenchmarkSuite::setConfig(size_t inputSize, size_t numTrials){
    testInputSizes_ = inputSize;
    testTrials_ = numTrials;
}

void BenchmarkSuite::runTests(size_t start, size_t end, std::vector<BenchmarkResults>& results){
    for (size_t i = start; i < end; ++i){
        results[i]= tests_[i]->runTest();
    }
}

void BenchmarkSuite::run(){
    
    // LaunchingThreadQueue<BenchmarkResults> pool;
    // for (BenchmarkConcept *test : tests_)
    // {
    //     pool.submit([test]()
    //                 { return test->runTest(); });
    // }
    // std::vector<BenchmarkResults> results = pool.run();
    unsigned int numThreads = std::thread::hardware_concurrency();
    if (numThreads < 2) { // if less than 2 or error
        numThreads = 1;
    } else {
        numThreads -= 1;
    }
    if (numThreads > tests_.size()){ // If too many threads available
        numThreads = tests_.size(); // do one test per thread
    }

    std::vector<BenchmarkResults> results(tests_.size());
    std::vector<std::thread> threads;
    size_t perThread = tests_.size() / numThreads;
    auto func = [this](size_t start, size_t end, std::vector<BenchmarkResults>& ret){
        runTests(start, end, ret);};
    for (size_t i = 0; i < numThreads; ++i){
        std::cout << i * perThread << " " << (i+1)*perThread << std::endl;
        threads.emplace_back(func, i * perThread, (i+1)*perThread, std::ref(results)); 
    }
    // Main thread does the remainder (if there is one);
    std::cout << "Main Thread: " << perThread*numThreads << " " << tests_.size() << std::endl;
    runTests(perThread*numThreads, tests_.size(), results);
    for (std::thread& t : threads){
        t.join();
    }

    clearTests();
    
    // Store results in memory
    for (BenchmarkResults &r : results) {
        results_.push_back(r);
    }
    
}

void BenchmarkSuite::resultsToCSV(std::string filename){
    try {
        std::ofstream out(filename);
        out << "testName, n, numSamples, avgTime, stdev\n";
        for (BenchmarkResults &s : results_)
        {
            out << s.to_string() << "\n";
        }
        out.close();
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return;
    }
}

void BenchmarkSuite::clearTests(){
    for (BenchmarkConcept*& test : tests_){
        delete test;
    }
    tests_.clear();
}

std::map<std::string, GroupedResults> BenchmarkSuite::getGroupedResults(){
    std::map<std::string, GroupedResults> groupedResults;
    for (BenchmarkResults res : results_){
        if (!groupedResults.contains(res.testName_)){
            GroupedResults g;
            g.inputSizes_.push_back(res.inputSize_);
            g.times_.push_back(res.avgTime_);
            g.stdevs_.push_back(res.stdev_);
            groupedResults.insert({res.testName_, g});
        } else {
            GroupedResults &g = groupedResults[res.testName_];
            g.inputSizes_.push_back(res.inputSize_);
            g.times_.push_back(res.avgTime_);
            g.stdevs_.push_back(res.stdev_);
        }
    }
    return groupedResults;
}

// Is there a way to make matplot++ an optional dependency? 
