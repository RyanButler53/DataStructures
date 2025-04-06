#ifndef MEDIAN_HPP_INCLUDED
#define MEDIAN_HPP_INCLUDED
#include "interfaces.hpp"
#include <concepts>

template <BasicHeap MinHeap, BasicHeap MaxHeap>
class MedianHeap
{
  private:
    // Bottom Heap is a max heap
    MaxHeap bottomHeap_;
    // Top heap is a min heap
    MinHeap topHeap_;

  public:

    using T = MaxHeap::value_type;

    MedianHeap() = default;
    ~MedianHeap() = default;

    void insert(T value);
    T findMedian()const ;
    void deleteMedian();
};

#include "median-private.hpp"

#endif // MEDIAN_HPP_INCLUDED

