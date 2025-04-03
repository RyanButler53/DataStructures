#ifndef DARY_HEAP_HPP_INCLUDED
#define DARY_HEAP_HPP_INCLUDED

#include <vector>
#include <unordered_map>
#include <functional>

// Compare must be a binary predicate
template <typename T, typename priority_t, size_t D=2, typename Compare = std::less<priority_t>>
class DAryHeap {
  
    struct Item{
      T item_;
      priority_t priority_;

      bool operator==(const Item &other) const { return (priority_ == other.priority_) && (item_ == other.item_); }

      void swap(Item& left, Item& right){
          std::swap(left.item_, right.item_);
          std::swap(left.priority_, left.item_);
      }
    };

    std::vector<Item> elements_;
    std::unordered_map<T, size_t> itemToIndex_;
    size_t size_;
    Compare comp_;
    // Gets the index of the parent node
    size_t parent(size_t i) const;

    // Gets the index of the leftmost child
    size_t leftmostChild(size_t i) const;

    // Gets the index of the rightmost child. 
    size_t getMinChild(size_t i) const;

    bool isLeaf(size_t i) const;

    void swap(size_t i1, size_t i2);

  public:

    using value_type = T;
    using priority_type = priority_t;

    DAryHeap();
    ~DAryHeap() = default;

    bool empty() const;
    size_t size()const;
    
    void pop();

    void push(const T& item, priority_t key);
    const T& top() const;

    void changeKey(T item, priority_t newKey);

};

template <typename T>
using BinaryMinHeap = DAryHeap<T, T, 2>;

template <typename T>
using BinaryHeap = DAryHeap<T, T, 2, std::greater<T>>;

#include "d-ary-private.hpp"

#endif // DARY_HEAP_HPP_INCLUDED
