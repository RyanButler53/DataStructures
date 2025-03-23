#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <mutex>
#include <utility>
#include <stddef.h>
#include <string>
#include <deque>
#include <assert.h>

#include "launching-threadpool/launching-threadpool.hpp"

using namespace std;
using namespace std::chrono_literals;

long long fib(int n){
    if (n < 2){
        return 1;
    } else {
        return fib(n - 1) + fib(n - 2);
    }
}

int main(int argc, char** argv) {
    int n = 10;
    size_t numThreads = 10;
    if (argc > 1)
    {
        n = atoi(argv[1]);
    }
    if (argc > 2){
        numThreads = atoi(argv[2]);
    }
    std::cout << "Starting launching thradpool test" << std::endl;

    LaunchingThreadQueue<long long> t(numThreads);
    for (int i = 0; i < n; ++i){
        t.submit([i]()-> long long{ return fib((i%45)); });
    }
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<long long> results = t.run();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;
}