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

double BenchmarkTest::average(std::vector<double>& vec) const {
    double sum = std::accumulate(vec.begin(), vec.end(), 0);
    return sum / numTrials_;
}

double BenchmarkTest::stdev(std::vector<double> &vec) const {
    double avg = average(vec);
    double deviation = std::accumulate(vec.begin(), vec.end(), 0.0, [&avg](double acc, double newVal)
                                       { return (avg - newVal) * (avg - newVal); });
    return std::sqrt(deviation / numTrials_);
}

double BenchmarkTest::measure(){
    auto start = std::chrono::steady_clock::now();
    fn_();
    auto end = std::chrono::steady_clock::now();
    long ms = std::chrono::duration_cast<std::chrono::milliseconds>((end - start)).count();
    return double(ms / 1000.0);
}

BenchmarkResults BenchmarkTest::operator()(){
    std::vector<double> times;
    for (size_t i = 0; i < numTrials_; ++i) {
        times.push_back(measure());
    }
    return BenchmarkResults{name_, inputSize_, numTrials_, average(times), stdev(times)};
}

std::string BenchmarkResults::to_string() const {
    std::stringstream ss;
    ss << testName_ << ", " << inputSize_ << ", " << numTrials_ << ", " << avgTime_ << ", " << stdev_;
    return ss.str();
}

// BENCHMARK SUITE

BenchmarkSuite::BenchmarkSuite(std::string suitename):suiteName_{suitename}{}

void BenchmarkSuite::addTest(BenchmarkTest& test){
    tests_.push_back(std::move(test));
}

void BenchmarkSuite::run(std::string filename){
    if (filename == ""){
        filename = suiteName_+ ".csv";
    }
    // not parallel
    std::vector<std::string> results;
    results.push_back("testName, n, numSamples, avgTime, stdev");
    for (BenchmarkTest &test : tests_) {
        BenchmarkResults res = test();
        results.push_back(res.to_string());
    }
    std::ofstream out(filename);
    for (std::string& s : results){
        out << s << "\n";
    }
    out.close();
}