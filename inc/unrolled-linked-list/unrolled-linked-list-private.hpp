#include "unrolled-linked-list.hpp"
#include <ranges>

template<typename T, size_t K>
UnrolledLinkedList<T, K>::UnrolledLinkedList():size_{0}{}

template <typename T, size_t K>
size_t UnrolledLinkedList<T, K>::size() const {
    return size_;
}

template <typename T, size_t K>
void UnrolledLinkedList<T, K>::push_front(const T& value){
    if (list_.size() == 0){
        Node n;
        n.insert_back(value);
        list_.push_back(std::move(n));
    } else {
        Node &n = list_.front();
        if (n.isFull()){
            size_t numToMove = K/2;

            Node newFront;
            newFront.insert_back(std::move(value));
            // Move the first half of the full node to the new one
            std::ranges::move(n.data_.begin(), n.data_.begin() + numToMove, newFront.data_.begin() + 1);
            // Move the second half of the full node to teh start of that node
            std::ranges::move(n.data_.begin() + numToMove, n.data_.end(), n.data_.begin());
            
            // Adjust Sizes
            newFront.size_ = numToMove + 1;
            n.size_ = K - numToMove;
            // Add to front
            list_.push_front(newFront);
        } else {
            n.insert_at(std::move(value), 0);
        }
    }
    ++size_;

}

template <typename T, size_t K>
void UnrolledLinkedList<T, K>::push_back(const T& value){
    // Empty list case is equivalent to push_front
    if (list_.empty()){
        push_front(value);
        return;
    }
    // Non empty list case
    Node &n = list_.back();
    if (n.isFull()){
        Node newNode;
        size_t numToMove = K / 2;
        std::ranges::move(n.data_.begin() + numToMove, n.data_.end(), newNode.data_.begin());
        // Update sizes
        n.size_ = K - numToMove;
        newNode.size_ = numToMove;
        // Insert last element and add to main list
        newNode.insert_back(std::move(value));
        list_.push_back(newNode);
    } else {
        n.insert_back(std::move(value));
    }
    ++size_;
}

template <typename T, size_t K>
T& UnrolledLinkedList<T,K>::front(){
    if (size_) {
        T& val = list_.front().data_[0];
        return val;
    } else {
        throw std::invalid_argument("Can't call front() on empty container!");
    }
}

template <typename T, size_t K>
const T& UnrolledLinkedList<T,K>::front() const {
    if (size_) {
        return list_.front().data_[0];
    } else {
        throw std::invalid_argument("Can't call front() on empty container!");
    }
}

template <typename T, size_t K>
T& UnrolledLinkedList<T, K>::back(){
    if (size_) {
        Node &n = list_.back();
        T &val = n.data_[n.size_ - 1];
        return val;
    } else {
        throw std::invalid_argument("Can't call front() on empty container!");
    }
}

template <typename T, size_t K>
const T& UnrolledLinkedList<T, K>::back() const{
    if (size_){
        const Node &n = list_.back();
        return std::move(n.data_[n.size_ - 1]);
    } else {
        throw std::invalid_argument("Can't call back() on an empty container");
    }
}

template<typename T, size_t K>
UnrolledLinkedList<T,K>::Node::Node():size_{0}{}

template<typename T, size_t K>
bool UnrolledLinkedList<T, K>::Node::isFull() const{
    return size_ >= K;
}

template <typename T, size_t K>
void UnrolledLinkedList<T, K>::Node::insert_back(const T& value){
    data_[size_] = std::move(value);
    ++size_;
}

template <typename T, size_t K>
void UnrolledLinkedList<T,K>::Node::insert_at(const T& value, size_t i){
    // Shift everything over and insert
    std::ranges::move_backward(data_.begin() + i, data_.begin() + size_, data_.begin() + size_ +1);
    data_[i] = std::move(value);
    ++size_;
}

template <typename T, size_t K>
void UnrolledLinkedList<T,K>::clear(){
    list_.clear();
    size_ = 0;
}

// to do
// iteration, pop_front and pop_back (using the iterator)
// Iteration
// Iterater tier insertion
// Erase

// insert_range

//overloaded constuctor