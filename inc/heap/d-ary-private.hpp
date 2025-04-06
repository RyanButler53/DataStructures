#include "d-ary.hpp"
#include <algorithm>

template <typename T, typename priority_t, size_t D, typename Compare>
DAryHeap<T, priority_t, D, Compare>::DAryHeap():size_{0}, comp_{}{}

template <typename T, typename priority_t, size_t D, typename Compare>
bool DAryHeap<T, priority_t, D, Compare>::empty() const{
    return size_ == 0;
}

template <typename T, typename priority_t, size_t D, typename Compare>
size_t DAryHeap<T, priority_t, D, Compare>::size() const{
    return size_;
}

template <typename T, typename priority_t, size_t D, typename Compare>
const T& DAryHeap<T, priority_t, D, Compare>::top() const{
    if (size_ == 0){
        throw std::logic_error("Cannot get top of an empty heap");
    } else {
        return elements_[0].item_;
    }
}

template <typename T, typename priority_t, size_t D, typename Compare>
size_t DAryHeap<T, priority_t, D, Compare>::parent(size_t i) const{
    if (i > 0){
        return (i - 1) / D;
    } else {
        return 0;
    }
}

template <typename T, typename priority_t, size_t D, typename Compare>
size_t DAryHeap<T, priority_t, D, Compare>::leftmostChild(size_t i)const {
    return D * i + 1;
}

template <typename T, typename priority_t, size_t D, typename Compare>
void DAryHeap<T, priority_t, D, Compare>::swap(size_t i1, size_t i2) {
    std::swap(itemToIndex_[elements_[i1].item_], itemToIndex_[elements_[i2].item_]);
    std::swap(elements_[i1], elements_[i2]);
}

template <typename T, typename priority_t, size_t D, typename Compare>
size_t DAryHeap<T, priority_t, D, Compare>::getMinChild(size_t i) const{
    size_t lchild = leftmostChild(i);
    // function will not be called if size_ is zero
    size_t end = std::min(size_, lchild + D);
    auto compFunc = [this](const Item& i1, const Item& i2){return this->comp_(i1.priority_, i2.priority_);};
    auto it = std::min_element(elements_.begin() + lchild, elements_.begin ()+ end, compFunc);
    return (it - elements_.begin());
}

template <typename T, typename priority_t, size_t D, typename Compare>
bool DAryHeap<T, priority_t, D, Compare>::isLeaf(size_t i) const{
    return leftmostChild(i) >= size_;
}

// Modification Functions
template <typename T, typename priority_t, size_t D, typename Compare>
void  DAryHeap<T, priority_t, D, Compare>::push(const T &item, priority_t key){
    elements_.push_back(Item{item, key});
    itemToIndex_[item] = size_;
    size_t current_i = size_;

    // While current isn't zero and current is less than its parent
    while (current_i && comp_(elements_[current_i].priority_, elements_[parent(current_i)].priority_)){
        size_t p = parent(current_i);
        swap(current_i, p);
        current_i = p;
    }
    ++size_;
}

template <typename T, typename priority_t, size_t D, typename Compare>
void DAryHeap<T, priority_t, D, Compare>::pop(){
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
        if (comp_(elements_[cur].priority_, elements_[minChild].priority_)){
            break; // done
        } else {
            swap(cur, minChild);
            cur = minChild;
        }
    }
}

template <typename T, typename priority_t, size_t D, typename Compare>
void DAryHeap<T, priority_t, D, Compare>::changeKey(T item, priority_t newKey){

    priority_t curPriority = elements_[itemToIndex_[item]].priority_;
    if (comp_(curPriority,newKey)) {
        throw std::invalid_argument("Cannot change key priority to this value");
    }  else if (!itemToIndex_.contains(item)) {
        throw std::invalid_argument("Cannot change a key value for a key not in the heap");
    }
    // Update current index
    size_t current_i = itemToIndex_[item];
    elements_[current_i].priority_ = newKey;

    // While current isn't zero and current is less than its parent
    // std::less
    while (current_i && comp_(elements_[current_i].priority_, elements_[parent(current_i)].priority_)){
        size_t p = parent(current_i);
        swap(current_i, p);
        current_i = p;
    }
}
