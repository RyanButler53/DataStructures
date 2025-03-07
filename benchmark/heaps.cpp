
#include <concepts>

template <typename T>
concept BasicHeap = requires(T &heap,
                             T::value_type value) {
    heap();
    heap.empty();
    heap.size();
    heap.pop();
    heap.push(value);
    heap.top();
};

// There are 3 "heap based algorithms" I want to benchmark: 

// Kofn: Get the minimum K of a set of n numbers. Make N HUGE
// Heap Sort algorithm
// Dijkstra