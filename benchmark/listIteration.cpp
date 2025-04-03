#include <list>
#include <sstream>
#include <iostream>
#include <future>

#include "benchmark.hpp"
#include "unrolled-linked-list/unrolled-linked-list.hpp"
#include "interfaces.hpp"

template <LinkedList List>
void setup(List& l, size_t numOps) {
    for (size_t i = 0; i < numOps / 2; ++i){
        l.push_back(i);
    }
}

template <LinkedList List>
void pushing(size_t numOps){
    List l;
    setup(l, numOps);
}

// full iteration
template <LinkedList List>
void iterate(size_t numOps){
    List l;
    setup(l, numOps);
    int set;
    for (size_t i = 0; i < 5; ++i){
        for (auto x : l) {
            set = x;
        }
    }

}

template <LinkedList List>
void randomInsertion(std::vector<int> actions)
{
    List l;
    setup(l, actions.size() / 4);
    typename List::iterator it = l.begin();
    for (int action : actions) {
        switch (action) {
        // Iterate forwards
        case 0:
            if (it != l.end()){
                ++it;
            } else {
                it = l.begin();
            }
            break;
        // iterate backwards
        case 1:
            if (it != l.begin()){
                --it;
            } else {
                it = l.end();
            }
            break;

        // Insert at iterator position
        case 2:
            l.insert(it, action);
            break;

        // Erase at iterator position
        case 3:
            if (it != l.end()){
                it = l.erase(it);
            }
            break;
        default : break;
        }
    }
}

// Move to some input generator class
void getActions(size_t numActions,
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

std::vector<int> getRandomActions(size_t numOps, size_t numActions){
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



int main(int argc, char** argv) {
    size_t numOps = 10000;
    if (argc == 2){
        numOps = atoi(argv[1]);
    }
    std::vector<int> actions = getRandomActions(numOps, 4);

    BenchmarkSuite suite("List Iteration");
    suite.setConfig(numOps, 10);
    suite.addConfiguredTest("std::list push back", pushing<std::list<int>>, std::ref(numOps));
    suite.addConfiguredTest("std::list iterate 5x", iterate<std::list<int>>, std::ref(numOps));
    suite.addConfiguredTest("std::list random Insertions", randomInsertion<std::list<int>>, std::ref(actions));
    suite.addConfiguredTest("Unrolled linked list: K = 8 push back", pushing<UnrolledLinkedList<int, 8>>, std::ref(numOps));
    suite.addConfiguredTest("Unrolled linked list: K = 8 5x", iterate<UnrolledLinkedList<int, 8>>, std::ref(numOps));
    suite.addConfiguredTest("Unrolled linked list: K = 8 random Insertions", randomInsertion<UnrolledLinkedList<int, 8>>, std::ref(actions));
    suite.addConfiguredTest("Unrolled linked list: K = 64 push back", pushing<UnrolledLinkedList<int, 64>>, std::ref(numOps));
    suite.addConfiguredTest("Unrolled linked list: K = 64 iterate 5x", iterate<UnrolledLinkedList<int, 64>>, std::ref(numOps));
    suite.addConfiguredTest("Unrolled linked list: K = 64 random Insertions", randomInsertion<UnrolledLinkedList<int, 64>>, std::ref(actions));
    suite.addConfiguredTest("Unrolled linked list: K = 256 push back", pushing<UnrolledLinkedList<int, 256>>, std::ref(numOps));
    suite.addConfiguredTest("Unrolled linked list: K = 256 iterate 5x", iterate<UnrolledLinkedList<int, 256>>, std::ref(numOps));
    suite.addConfiguredTest("Unrolled linked list: K = 256 random Insertions", randomInsertion<UnrolledLinkedList<int, 256>>, std::ref(actions));

    suite.run();
    suite.resultsToCSV("iteration.csv");
}