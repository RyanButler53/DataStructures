#include "d-ary.hpp"
#include <algorithm>

template <typename T, typename priority_t, size_t D>
DAryHeap<T, priority_t, D>::DAryHeap():size_{0} {}

template <typename T, typename priority_t, size_t D>
bool DAryHeap<T, priority_t, D>::empty() const{
    return size_ == 0;
}

template <typename T, typename priority_t, size_t D>
size_t DAryHeap<T, priority_t, D>::size() const{
    return size_;
}

template <typename T, typename priority_t, size_t D>
const T& DAryHeap<T, priority_t, D>::top() const{
    if (size_ == 0){
        throw std::logic_error("Cannot get top of an empty heap");
    } else {
        return elements_[0].item_;
    }
}

template <typename T, typename priority_t, size_t D>
size_t DAryHeap<T, priority_t, D>::parent(size_t i) const{
    if (i > 0){
        return (i - 1) / D;
    } else {
        return 0;
    }
}

template <typename T, typename priority_t, size_t D>
size_t DAryHeap<T, priority_t, D>::leftmostChild(size_t i)const {
    return D * i + 1;
}

template <typename T, typename priority_t, size_t D>
void DAryHeap<T, priority_t, D>::swap(size_t i1, size_t i2) {
    std::swap(itemToIndex_[elements_[i1].item_], itemToIndex_[elements_[i2].item_]);
    std::swap(elements_[i1], elements_[i2]);
}

template <typename T, typename priority_t, size_t D>
std::vector<typename DAryHeap<T, priority_t, D>::Item> DAryHeap<T, priority_t, D>::getElements(){
    return elements_;
}

template <typename T, typename priority_t, size_t D>
bool DAryHeap<T, priority_t, D>::checkMaps() const {
    for (const Item& elem : elements_){
        auto ind = itemToIndex_.at(elem.item_);
        if (elements_[ind] != elem){
            return false;
        }
    }
    return true;
}

template <typename T, typename priority_t, size_t D>
size_t DAryHeap<T, priority_t, D>::getMinChild(size_t i) const{
    size_t lchild = leftmostChild(i);
    // function will not be called if size_ is zero
    size_t end = std::min(size_, lchild + D);
    auto it = std::min_element(elements_.begin() + lchild, elements_.begin ()+ end);
    return (it - elements_.begin());
}

template <typename T, typename priority_t, size_t D>
bool DAryHeap<T, priority_t, D>::isLeaf(size_t i) const{
    return leftmostChild(i) >= size_;
}

// Modification Functions
template <typename T, typename priority_t, size_t D>
void  DAryHeap<T, priority_t, D>::push(const T &item, priority_t key){
    elements_.push_back(Item{item, key});
    itemToIndex_[item] = size_;
    size_t current_i = size_;

    // While current isn't zero and current is less than its parent
    while (current_i && elements_[current_i] < elements_[parent(current_i)]){
        size_t p = parent(current_i);
        swap(current_i, p);
        current_i = p;
    }
    ++size_;
}

template <typename T, typename priority_t, size_t D>
void DAryHeap<T, priority_t, D>::pop(){
    if (!size_){
        throw("Cannot pop from an empty heap");
    }
    
    // Move last element to "root"
    swap(0, size_-1);
    size_t cur = 0;
    itemToIndex_.erase(elements_.back().item_);
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

template <typename T, typename priority_t, size_t D>
void DAryHeap<T, priority_t, D>::decreaseKey(T item, priority_t newKey){

    priority_t curPriority = elements_[itemToIndex_[item]].priority_;
    if (newKey >= curPriority) {
        throw std::invalid_argument("Cannot decrease a key value to a greater value");
    }  else if (!itemToIndex_.contains(item)) {
        throw std::invalid_argument("Cannot decrease a key value for a key not in the heap");
    }
    // Update current index
    size_t current_i = itemToIndex_[item];
    elements_[current_i].priority_ = newKey;

    // While current isn't zero and current is less than its parent
    while (current_i && elements_[current_i] < elements_[parent(current_i)]){
        size_t p = parent(current_i);
        swap(current_i, p);
        current_i = p;
    }
}
