# DataStructures
Repository to store all the C++ Templates of various Data Structures that I think are cool. 

Also has some benchmarks about how well they perform in practice. 

Uses `std::ranges` and `concepts` so C++ 20 is required to compile the data structures and benchmarks. 

## Data Structures in this repo: 

Linked List/Queue: 
- Unrolled Linked List
- Quack

HashMap/Hashset:
- Cuckoo Hashmap/Hashset

Binary Search Trees: 
- Splay Tree
- Scapegoat Tree

Heap
- D-Ary Heap
- Lazy Binomial Heap
- Fibonacci Heap
- Pairing Heap 

Threading: 
- Launching Threadpool (Submit jobs, launch at once)

Spatial: 
- KD-Tree
- Interval Tree

## Python Bindings
All data structures except launching thread pool have python bindings. The python bindings are generated
using C++26 Static reflection and require a C++26 compiler. The dockerfile makes a container where you can install 
them with CMAKE_BUILD_PARALLELISM_LEVEL=8 pip install .

### Known Bugs: 
Cuckoo Hash Set's `size()` does not work correctly

### Notes:
- All python tests are AI generated based off the C++ tests
- Quack C++ unit tests are also AI generated. 
