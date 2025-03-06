#include "heap.hpp"
#include <algorithm>

template <typename T, size_t D>
DAryHeap<T, D>::DAryHeap():size_{0} {}

template <typename T, size_t D>
bool DAryHeap<T, D>::empty() const{
    return size_ == 0;
}

template <typename T, size_t D>
size_t DAryHeap<T, D>::size() const{
    return size_;
}

template <typename T, size_t D>
const T& DAryHeap<T,D>::top() const{
    if (size_ == 0){
        throw std::logic_error("Cannot get top of empty heap");
    } else {
        return elements_[0];
    }
}

template <typename T, size_t D>
size_t DAryHeap<T,D>::parent(size_t i) const{
    if (i > 0){
        return (i - 1) / D;
    } else {
        return 0;
    }
}

template <typename T, size_t D>
size_t DAryHeap<T,D>::leftmostChild(size_t i)const {
    return D * i + 1;
}

template <typename T, size_t D>
void DAryHeap<T,D>::swap(size_t i1, size_t i2) {
    std::swap(elem_to_index_[elements_[i1]], elem_to_index_[elements_[i2]]);
    std::swap(elements_[i1], elements_[i2]);
}

template <typename T, size_t D>
std::vector<T> DAryHeap<T,D>::getElements(){
    return elements_;
}

template <typename T, size_t D>
bool DAryHeap<T,D>::checkMaps() const {
    for (const T& elem : elements_){
        auto ind = elem_to_index_.at(elem);
        if (elements_[ind] != elem){
            return false;
        }
    }
    return true;
}

template <typename T, size_t D>
size_t DAryHeap<T,D>::getMinChild(size_t i) const{
    size_t lchild = leftmostChild(i);
    // function will not be called if size_ is zero
    size_t end = std::min(size_, lchild + D);
    auto it = std::min_element(elements_.begin() + lchild, elements_.begin ()+ end);
    return (it - elements_.begin());
}

template <typename T, size_t D>
bool DAryHeap<T, D>::isLeaf(size_t i) const{
    return leftmostChild(i) >= size_;
}

// Modification Functions
template <typename T, size_t D>
void  DAryHeap<T,D>::push(const T &value){
    elements_.push_back(value);
    elem_to_index_[value] = size_;
    size_t current_i = size_;

    // While current isn't zero and current is less than its parent
    while (current_i && elements_[current_i] < elements_[parent(current_i)]){
        size_t p = parent(current_i);
        swap(current_i, p);
        current_i = p;
    }
    ++size_;
}

template <typename T, size_t D>
void DAryHeap<T,D>::pop(){
    if (!size_){
        throw("Cannot pop from an empty heap");
    }
    
    // Move last element to "root"
    swap(0, size_-1);
    size_t cur = 0;
    elem_to_index_.erase(elements_.back());
    elements_.pop_back();
    --size_;
    while (!isLeaf(cur))
    {
        size_t minChild = getMinChild(cur);
        if (elements_[cur] < elements_[minChild]){
            break; // done
        } else {
            swap(cur, minChild);
            cur = minChild;
        }
    }
}

template <typename T, size_t D>
void DAryHeap<T, D>::decreaseKey(T key, T newKey){
    
    if (newKey >= key){
        throw std::invalid_argument("Cannot decrease a key value to a greater value");
    } else if (!elem_to_index_.contains(key)){
        throw std::invalid_argument("Cannot decrease a key value for a key not in the heap");
    }
    // Update current index
    size_t current_i = elem_to_index_[key];
    elements_[current_i] = newKey;

    // Update elem_to_index_ lookup
    elem_to_index_[newKey] = current_i;
    elem_to_index_.erase(key);

    // While current isn't zero and current is less than its parent
    while (current_i && elements_[current_i] < elements_[parent(current_i)]){
        size_t p = parent(current_i);
        swap(current_i, p);
        current_i = p;
    }
}
