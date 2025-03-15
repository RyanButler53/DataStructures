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

void BenchmarkSuite::run(std::string filename){
    if (filename == ""){
        // filename = suiteName_+ ".csv";
    }

    LaunchingThreadQueue<BenchmarkResults> pool;
    for (BenchmarkConcept *test : tests_)
    {
        pool.submit([test]()
                    { return test->runTest(); });
    }
    std::vector<BenchmarkResults> results = pool.run();
    clearTests();
    
    if (filename != "") {
        std::ofstream out(filename);
        out << "testName, n, numSamples, avgTime, stdev\n";
        for (BenchmarkResults &s : results)
        {
            out << s.to_string() << "\n";
        }
        out.close();
    }
    else
    {
        // Store results in memory (cheaper than inputs).
        for (BenchmarkResults &r : results) {
            results_.push_back(r);
        }
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
void BenchmarkSuite::resultsToPlot(std::string filename){}
