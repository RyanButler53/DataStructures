
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
