#include "linear-probing.hpp"

template <typename T>
LinearProbing<T>::LinearProbing():maxLoad_{0.5}{
    uint64_t a = rand();
    uint64_t b = rand();
    hash_ = Hash<T>(a, b);
    clear();
}

template <typename T>
LinearProbing<T>::LinearProbing(double maxload):maxLoad_{maxload}{
    uint64_t a = rand();
    uint64_t b = rand();
    hash_ = Hash<T>(a, b);
    clear();
}

template <typename T>
void LinearProbing<T>::clear() {
    available_.resize(1);
    available_[0] = true;
    items_.resize(1);
}

template <typename T>
void LinearProbing<T>::insert(T key) {
    double newLoadFactor = double(size_+ 1)/ numBuckets_;
    if (newLoadFactor > maxLoad_){
        rehash(numBuckets_ * 2);
    }
    size_t index = hash_(key) % numBuckets_;
    while (!available_[index]) {
        ++index;
        index = index%numBuckets_;
    }
    items_[index] = key;
    available_[index] = false;
    ++size_;
}

template <typename T>
void LinearProbing<T>::rehash(size_t newNumBuckets) {
    std::vector<T> allItems;
    for (size_t i = 0; i < numBuckets_; ++i){
        if (!available_[i]) allItems.push_back(items_[i]);
    }
    numBuckets_ = newNumBuckets;
    items_.resize(numBuckets_);
    available_.resize(numBuckets_);
    std::ranges::fill(available_, true);
    size_ = 0;
    for (T item : allItems){
        insert(item);
    }
}

template <typename T>
bool LinearProbing<T>::contains(T key) const {
    size_t bucket = hash_(key) % numBuckets_;
    while (!available_[bucket]){
        if (items_[bucket] == key){
            return true;
        } else{
            ++bucket;
        }
    }
    return false;
    // once an open bucket is found, then it was never added to the set. 
}

template <typename T>
void LinearProbing<T>::remove(T key){
    throw std::runtime_error("Not implemented");
}