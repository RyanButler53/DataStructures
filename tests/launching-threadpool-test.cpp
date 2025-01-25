#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <mutex>
#include <utility>
#include <stddef.h>
#include <string>
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
// File IO is limited by the IO bus not thread speed
long long func(int n){
    long long result = fib(n- (n*n%47));
        return result;
}

int main(int argc, char** argv) {
    size_t n = 60;
    size_t numThreads = 10;
    if (argc > 1)
    {
        n = atoi(argv[1]);
    }
    if (argc > 2){
        numThreads = atoi(argv[2]);
    }
    std::cout << "Starting launching thradpool test" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    LaunchingThreadQueue<long long> t(numThreads);
    for (size_t i = 0; i < n; ++i){
        t.submit([i]()-> long long{ return func(i); });
    }
    
    std::vector<long long> results = t.run();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;
    
}