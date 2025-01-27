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
    auto beginIter = begin();
    insert(beginIter, value);
}

template <typename T, size_t K>
void UnrolledLinkedList<T, K>::push_back(const T& value){
    // Empty list case is equivalent to push_front
    auto endIter = end();
    insert(endIter, value);
}

template <typename T, size_t K>
void UnrolledLinkedList<T,K>::insert(UnrolledLinkedList<T,K>::iterator& pos, const T& value){    

    if (pos.nodeInd_ == 0){    // Go back to the previous node and handle it
        if (list_.size() == 0){ // Empty List case
            Node n;
            n.insert_back(value);
            list_.push_back(std::move(n));
            ++pos.nodeInd_;
        } else if (pos.listIter_ == list_.begin()) { // Inserting at the beginning
            list_.front().insert_at(value, 0);
            ++pos.nodeInd_;
        }
        else
        { // go to the previous node and insert
            typename std::list<Node>::iterator listIter = pos.listIter_;
            --listIter;
            // back on he previous node
            // n -> newNode -> pos.listIter (splitting n )
            Node &n = *listIter; // previous node to split potentially
            if (n.isFull()){ 
                Node newNode;
                size_t numToMove = K / 2;
                std::ranges::move(n.data_.begin() + numToMove, n.data_.end(), newNode.data_.begin());
                n.size_ = numToMove;
                newNode.size_ = K- numToMove; // K - numToMove actually move if K is odd (shuoldn't be)
                newNode.insert_back(value);
                list_.insert(pos.listIter_, newNode);
            } else { // add to back
                n.insert_back(value);
            }
        }
    } else { // nodeInd is not zero. The node may be full. 
        Node& n = *(pos.listIter_);
        if (n.isFull()){
            Node newNode;
            size_t numToMove = K / 2;
            std::ranges::move(n.data_.begin() + numToMove, n.data_.end(), newNode.data_.begin());
            n.size_ = numToMove;
            newNode.size_ = K - numToMove;
            // n -> newNode -> next
            if (pos.nodeInd_ < numToMove){ // value was added in the current node
                n.insert_at(value, pos.nodeInd_);
                ++pos.listIter_; //increment since .insert inserts before
                list_.insert(pos.listIter_, newNode);
                --pos.listIter_; // Iterator is in the first node, decrement to where newNode is. 
            } else { // value is inserted in the new node;
                pos.nodeInd_ -= numToMove;
                newNode.insert_at(value, pos.nodeInd_);
                ++pos.listIter_;
                list_.insert(pos.listIter_, newNode);
            }
            --pos.listIter_; // need to decrement once again for either case
        }
        else
        {
            n.insert_at(value, pos.nodeInd_);
        }
        ++pos.nodeInd_;
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

template <typename T, size_t K>
void UnrolledLinkedList<T,K>::clear(){
    list_.clear();
    size_ = 0;
}

// Iterator methods:
template <typename T, size_t K>
typename UnrolledLinkedList<T,K>::iterator UnrolledLinkedList<T,K>::begin() {
    // std::list<Node>::iterator iter = 
    return Iterator(list_.begin());
}

template <typename T, size_t K>
typename UnrolledLinkedList<T,K>::iterator UnrolledLinkedList<T,K>::end(){
    return Iterator(list_.end());
}

template <typename T, size_t K>
typename UnrolledLinkedList<T,K>::reverse_iterator UnrolledLinkedList<T,K>::rbegin(){
    return reverse_iterator(end());
}

template <typename T, size_t K>
typename UnrolledLinkedList<T,K>::reverse_iterator UnrolledLinkedList<T,K>::rend(){
    return reverse_iterator(begin());
}

// END UNROLLED LINKED LIST METHODS

// BEGIN NODE METHODS

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

// END NODE METHODS

//BEGIN ITERATOR METHODS

template <typename T, size_t K>
UnrolledLinkedList<T,K>::Iterator::Iterator(std::list<Node>::iterator listIter):listIter_{listIter}, nodeInd_{0}{}

template <typename T, size_t K>
typename UnrolledLinkedList<T,K>::Iterator::value_type UnrolledLinkedList<T,K>::Iterator::operator*() const {
    Node& n = *listIter_;
    if (nodeInd_ >= n.size_){
        throw std::runtime_error("Invalid iterator???");
    } else {
        return n.data_[nodeInd_];
    }
}

template <typename T, size_t K>
typename  UnrolledLinkedList<T,K>::Iterator& UnrolledLinkedList<T,K>::Iterator::operator++(){
    Node &n = *listIter_;
    if (nodeInd_ >= n.size_- 1){
        ++listIter_;
        nodeInd_ = 0;
    } else {
        ++nodeInd_;
    }
    return *this;
}

template <typename T, size_t K>
typename  UnrolledLinkedList<T,K>::Iterator& UnrolledLinkedList<T,K>::Iterator::operator--(){
    if (nodeInd_ == 0){
        --listIter_;
        Node &n = *listIter_;
        nodeInd_ = n.size_ - 1;
        
    } else {
        --nodeInd_;
    }
    return *this;
}

template <typename T, size_t K>
typename UnrolledLinkedList<T, K>::Iterator::pointer UnrolledLinkedList<T,K>::Iterator::operator->() const{
    return &(**this);
}

template <typename T, size_t K>
bool UnrolledLinkedList<T,K>::Iterator::operator==(const Iterator& other)const {
    if (listIter_ == other.listIter_){
        return nodeInd_ == other.nodeInd_;
    } else {
        return false;
    }
}

template <typename T, size_t K>
bool UnrolledLinkedList<T,K>::Iterator::operator!=(const Iterator& other)const {
    return !(*this == other);
}

// END ITERATOR METHODS

// to do 
// iteration, pop_front and pop_back (using the iterator)
// Iterater tier insertion
// Erase

// insert_range

//overloaded constuctor