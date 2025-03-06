#ifndef DARY_HEAP_HPP_INCLUDED
#define DARY_HEAP_HPP_INCLUDED

#include <vector>
#include <unordered_map>


template <typename T,  size_t D=2>
class DAryHeap {

    std::vector<T> elements_;
    std::unordered_map<T, size_t> elem_to_index_;
    size_t size_;

    // Gets the index of the parent node
    size_t parent(size_t i) const;

    // Gets the index of the leftmost child
    size_t leftmostChild(size_t i) const;

    // Gets the index of the rightmost child. 
    size_t getMinChild(size_t i) const;

    bool isLeaf(size_t i) const;

    void swap(size_t i1, size_t i2);

public:
    DAryHeap();
    ~DAryHeap() = default;

    bool empty() const;
    size_t size()const;
    
    void pop();

    void push(const T& value);
    const T& top() const;

    void decreaseKey(T key, T newKey);

    std::vector<T> getElements();

    bool checkMaps() const;
};

template <typename T>
using BinaryHeap = DAryHeap<T, 2>;

#include "heap-private.hpp"

#endif // DARY_HEAP_HPP_INCLUDED
