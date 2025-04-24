
#include <vector>
#include <tuple>
#include <functional>
#include <iostream>
#include <cmath>

#include "benchmark.hpp"

#include "heap/d-ary.hpp"
#include "heap/binomial.hpp"
#include "heap/fibonacci.hpp"
#include "heap/pairing.hpp"
#include "interfaces.hpp"
#include "adaptors.hpp" // for std::priority_queue


// Kofn: Get the minimum K of a set of n numbers.
template <BasicHeap heap_t>
std::vector<uint32_t> findKofN(std::vector<uint32_t>& numbers, size_t k){
    // Set up a heap with the max k items
    heap_t maxK;
    for (size_t i = 0; i < k; ++i){
        maxK.push(numbers[i], numbers[i]);
    }

    // Iterate over the items and add to the heap if necessary
    for (auto iter = begin(numbers) + k; iter != end(numbers); ++iter)
    {
        if (*iter > maxK.top()){
            maxK.pop();
            maxK.push(*iter, *iter);
        }
    }

    // Extract the greatest K items. 
    std::vector<uint32_t> answer(k, 0);
    for (size_t i = 0; i < k; ++i)
    {
        uint32_t value = maxK.top();
        answer[i] = value;
        maxK.pop();
    }
    return answer;
}

// Heap Sort algorithm
template <typename heap_t>
std::vector<uint32_t> hsort(std::vector<uint32_t> v){
    // Put into a heap
    heap_t h;
    for (uint32_t& x : v){
        h.push(x,x);
    }
    // Pop out from heap in sorted order
    std::vector<uint32_t> sorted;
    sorted.reserve(v.size());
    while (!h.empty()) {
        sorted.push_back(h.top());
        h.pop();
    }
    return sorted;
}


void kofn(size_t n){
    // Generate a HUGE LIST OF NUMBERS. 32 bit integer is enough
    // const size_t n = 100000000;// std::numeric_limits<uint32_t>::max(); // 2 billion. 
    // K is varied. Does the heap do better when k is small? 
    
    std::random_device rd;
    std::mt19937 g(rd());
    std::vector<uint32_t> numbers(n);
    std::iota(numbers.begin(), numbers.end(), 0ul);
    std::ranges::shuffle(numbers, g);

    BenchmarkSuite suite("K of N");
    for (size_t k : std::vector<size_t>{8, 64, 512, 4096}){
        suite.setConfig(k, 10);
        suite.addConfiguredTest("std::priority_queue", findKofN<PQAdaptor<uint32_t, uint32_t>>, std::ref(numbers),  std::ref(k));
        suite.addConfiguredTest("Binary Heap", findKofN<BinaryMinHeap<uint32_t>>, std::ref(numbers), std::ref(k));
        suite.addConfiguredTest("DAry Heap D = 5", findKofN<DAryHeap<uint32_t, uint32_t, 5>>, std::ref(numbers),  std::ref(k));
        suite.addConfiguredTest("DAryHeap D = 10", findKofN<DAryHeap<uint32_t, uint32_t, 10>>, std::ref(numbers),  std::ref(k));
        suite.addConfiguredTest("Binomial Heap", findKofN<BinomialHeap<uint32_t, uint32_t>>, std::ref(numbers),  std::ref(k));
        suite.addConfiguredTest("Fibonacci Heap", findKofN<FibonacciHeap<uint32_t, uint32_t>>, std::ref(numbers),  std::ref(k));
        suite.addConfiguredTest("Pairing Heap", findKofN<PairingHeap<int, int>>, std::move(numbers), std::ref(k));

        suite.run();
    }
    suite.resultsToCSV("kofn.csv");
}


void heapsort(size_t n) {

    std::random_device rd;
    std::mt19937 g(rd());
    std::vector<uint32_t> numbers(n);
    std::iota(numbers.begin(), numbers.end(), 0ul);
    std::ranges::shuffle(numbers, g);

    BenchmarkSuite suite("Heap Sort");
    suite.setConfig(n, 10);
    suite.addConfiguredTest("std::priority_queue", hsort<PQAdaptor<uint32_t, uint32_t>>, std::move(numbers));
    suite.addConfiguredTest("Binary Heap", hsort<BinaryMinHeap<uint32_t>>, std::move(numbers));
    suite.addConfiguredTest("DAry Heap D = 5", hsort<DAryHeap<uint32_t, uint32_t, 5>>, std::move(numbers));
    suite.addConfiguredTest("DAryHeap D = 10", hsort<DAryHeap<uint32_t, uint32_t, 10>>, std::move(numbers));
    suite.addConfiguredTest("Binomial Heap", hsort<BinomialHeap<uint32_t, uint32_t>>, std::move(numbers));
    suite.addConfiguredTest("Fibonacci Heap", hsort<FibonacciHeap<uint32_t, uint32_t>>, std::move(numbers));
    suite.addConfiguredTest("Pairing Heap", hsort<PairingHeap<int, int>>, std::move(numbers));
    suite.run();
    suite.resultsToCSV("heapsort.csv");
}

int main(int argc, char** argv) {
    
    if (argc < 3 || argc > 4)
    {
        // Wrong number of command-line arguments. Print message and quit
        std::cout << "usage: " << argv[0] << "  <kofn> <sort> [ <n for kofn> <n for sort> ]\n";
        return -1;
    }
    // Convert argument n from ascii string to a number (using base 10)
    size_t kofnn = strtoul(argv[1], nullptr, 10);
    // Convert argument k from ascii string to a number (using base 10).
    size_t sortn = strtoul(argv[2], nullptr, 10);

    kofn(kofnn);
    heapsort(sortn);
    return 0;
}