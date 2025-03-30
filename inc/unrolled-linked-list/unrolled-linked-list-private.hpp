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
            ++pos.listIter_; // increment once to point to the new node
            ++pos.listIter_; // Increment a second time to reach the .end() iterator.
        } else if (pos.listIter_ == list_.begin()) { // Inserting at the beginning
            // Must check if node is full!
            Node &n = list_.front();
            if (n.isFull()){
                Node newNode; // make new first node
                size_t numToMove = K / 2; 
                // move the LAST K/2 items into the new node
                // n -> newNode -> rest
                std::ranges::move(n.data_.begin() + numToMove, n.data_.end(), newNode.data_.begin());
                n.size_ = numToMove;
                newNode.size_ = K- numToMove; // K - numToMove actually move if K is odd (shouldn't be)
                n.insert_at(value, 0); // insert at beginning of n
                ++pos.listIter_;
                list_.insert(pos.listIter_, newNode); // Add new node second node
                --pos.listIter_;
                --pos.listIter_;
            } else {
                list_.front().insert_at(value, 0);
            }

            ++pos.nodeInd_;
        } else { // go to the previous node and insert
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

// Deletion
template <typename T, size_t K>
UnrolledLinkedList<T,K>::iterator UnrolledLinkedList<T,K>::erase(iterator pos){
    if (pos == end()){
        throw std::invalid_argument("Cannot erase from end() iterator");
    }
    Node &curNode = *(pos.listIter_);
    // INVARIANT: Node size is NEVER zero.
    curNode.delete_at(pos.nodeInd_);
    if (curNode.size_ == 0) { // clean up an empty node;
        typename std::list<Node>::iterator nextListIter = list_.erase(pos.listIter_);
        pos = iterator(nextListIter);
    }  else if (pos.nodeInd_ == curNode.size_) {
        ++pos;
    }
    --size_;
    return pos;
}

template <typename T, size_t K>
void UnrolledLinkedList<T,K>::pop_front(){
    if (size_ == 0){
        throw std::invalid_argument("Can't call pop_front on an empty list");
    } else {
        Node &n = list_.front();
        if (n.size_ > 0){
            n.delete_at(0);
        } else { // empty first node. Remove it. 
            list_.pop_front();
        }
    }
    --size_;
}

template <typename T, size_t K>
void UnrolledLinkedList<T,K>::pop_back(){
    if (size_ == 0){
        throw std::invalid_argument("Can't call pop_back on an empty list");
    } else {
        Node &n = list_.back();
        --n.size_;
        if (n.size_ == 0){
            list_.pop_back();
        }
    }
    --size_;
}

// Element Access

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

template <typename T, size_t K>
void UnrolledLinkedList<T,K>::Node::delete_at(size_t i){
    // Shift everything over and insert
    std::ranges::move(data_.begin() + i + 1, data_.begin() + size_, data_.begin() + i);
    --size_;
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
// insert_range

//overloaded constuctor