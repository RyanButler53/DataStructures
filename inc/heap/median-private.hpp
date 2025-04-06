#include "median.hpp"

template <BasicHeap MinHeap, BasicHeap MaxHeap>
void MedianHeap<MinHeap, MaxHeap>::insert(MedianHeap::value_type value){

    // Empty case
    if (bottomHeap_.empty() && topHeap_.empty()){
        bottomHeap_.push(value, value);
        return;
    }

    value_type median = findMedian();

    // If same size, insert the value into the correct heap. 
    if (bottomHeap_.size() == topHeap_.size()){
        // Empty heap case;
        if (value > median){
            topHeap_.push(value, value);
        } else {
            bottomHeap_.push(value, value);
        }
    } else { 
        // If the item belongs in the top heap
        if (value > median){
            if (topHeap_.size() > bottomHeap_.size()){
                // Extra element in the top heap. 
                // Move it to the bottom heap
                bottomHeap_.push(median, median);
                // Make space 
                topHeap_.pop();
            } 
            topHeap_.push(value,value);
        } else { // value belongs in the bottom heap
            if (topHeap_.size() < bottomHeap_.size()){
                topHeap_.push(median, median);
                bottomHeap_.pop();
            } 
            bottomHeap_.push(value, value);
        }
    }
}

template <BasicHeap MinHeap, BasicHeap MaxHeap>
MaxHeap::value_type MedianHeap<MinHeap, MaxHeap>::findMedian() const {
    // If heaps are equal size, define the median to be the one at
    // the top of the bottom heap. 
    if (bottomHeap_.size() < topHeap_.size()) {
        return topHeap_.top();
    } else {
        return bottomHeap_.top();
    }
}

template <BasicHeap MinHeap, BasicHeap MaxHeap>
void MedianHeap<MinHeap, MaxHeap>::deleteMedian(){
    // If the top heap is bigger, the median is at the top of that. 
    if (bottomHeap_.size() < topHeap_.size()) {
        topHeap_.pop();
    } else { 
        // If bottom heap is bigger or sizes are equal, pop that since top of bottom
        // heap is defined to be the top. 
        bottomHeap_.pop();
    }
}
