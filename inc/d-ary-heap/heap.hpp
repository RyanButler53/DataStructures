#ifndef DARY_HEAP_HPP_INCLUDED
#define DARY_HEAP_HPP_INCLUDED

#include <vector>
#include <unordered_map>


template <typename T, typename priority_t, size_t D=2>
class DAryHeap {

  public: 

    struct Item{
    T item_;
    priority_t priority_;

    bool operator==(const Item &other) const { return (priority_ == other.priority_) && (item_ == other.item_); }
    bool operator<(const Item &other) const { return priority_ < other.priority_; }
    void swap(Item& left, Item& right){
        std::swap(left.item_, right.item_);
        std::swap(left.priority_, left.item_);
    }
    };

    using value_type = T;
    using priority_type = priority_t;

private:
    std::vector<Item> elements_;
    std::unordered_map<T, size_t> itemToIndex_;
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

    void push(const T& item, priority_t key);
    const T& top() const;

    void decreaseKey(T item, priority_t newKey);

    // Checking correctness functions
    std::vector<Item> getElements();

    bool checkMaps() const;
};

template <typename T>
using BinaryHeap = DAryHeap<T, T, 2>;

#include "heap-private.hpp"

#endif // DARY_HEAP_HPP_INCLUDED
