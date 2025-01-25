#include "quack.hpp"
#include <iostream>
using namespace std;

template <typename T>
Quack<T>::Quack() : queue_{}, stack_{},size_{0} {
}

template <typename T>
void Quack<T>::pop(){
    if (size_ == 0){
        std::cerr << "Popping an empty quack" << endl;
        throw("Popping an empty quack");
        exit(1);
    }
    if (stack_.size() == 0)
    {
        reshuffle(queue_, stack_);
    }
    stack_.pop_back();
    --size_;
}

template <typename T>
void Quack<T>::dequeue(){
    if (size_ == 0){
        cerr << "Dequeue on empty quack" << endl;
        throw("Dequeue on empty quack");
        exit(1);
    }
    if (queue_.size() == 0){
        reshuffle(stack_, queue_);
    }
    queue_.pop_back();
    --size_;
}

template <typename T>
void Quack<T>::push(T val){
    stack_.push_back(val);
    ++size_;
}

template <typename T>
void Quack<T>::push_front(T val){
    queue_.push_back(val);
    ++size_;
}

template <typename T>
void Quack<T>::reshuffle(vector<T> &full, vector<T> &empty){

    if (size_ == 1){
        empty.push_back(full.back());
        full.pop_back();
    } else {
        size_t numItems = size_ / 2 + size_ % 2;
        empty.resize(numItems);
        std::reverse_copy(begin(full), begin(full) + numItems, begin(empty));
        std::move(begin(full) + numItems, end(full), begin(full));
        full.resize(size_ - numItems);
    }
}

template <typename T>
T Quack<T>::front() {
    if (size_ == 0){
        cerr << "Calling front() on empty quack" << endl;
        exit(1);
    } else if (queue_.empty()){
        reshuffle(stack_, queue_);
    }
    return queue_.back();
}

template <typename T>
T Quack<T>::back() {
    if (size_ == 0){
        cerr << "Calling back() on empty quack" << endl;
        exit(1);
    } else if (stack_.empty()){
        reshuffle(queue_, stack_);
    }
    return stack_.back();
}

template <typename T>
void Quack<T>::printToStream(ostream& out) const{
    out << "Queue: ";
    for (auto &e : queue_) {
        out << e << " ";
    }
    out << "\nStack: ";
    for (auto&e : stack_){
        out << e << " ";
    }
    out << "\n";

}

template <typename T>
bool Quack<T>::empty(){
    return size_ == 0;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Quack<T>& q){
    q.printToStream(os);
    return os;
}
