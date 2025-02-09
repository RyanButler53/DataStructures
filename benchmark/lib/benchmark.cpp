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