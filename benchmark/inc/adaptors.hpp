/**
 * Contains adaptors for various data strctures don't fit the 
 * interfaces specified in "interface.hpp"
 * 
 * Adaptors defined:
 * std::priority_queue<T> satisfying the BasicHeap concept
 * Quack<T> satisfying the Queue voncept
 */
#pragma once

#include <queue>

#include "quack/quack.hpp"


template <typename T, typename P, typename Compare = std::less<T>>
class PQAdaptor {
    std::priority_queue<T, std::vector<T>, Compare> pq;

public:
    using value_type = T;
    using priority_type = P;

    PQAdaptor() = default;
    ~PQAdaptor() = default;
    bool empty() { return pq.empty();}
    T top() const { return pq.top(); }
    void pop() { pq.pop(); }
    void push(T item, P priority) {pq.push(item); }
    size_t size() const { return pq.size(); }
};

template <typename T>
class QuackAdapter {
    Quack<T> q_;

public:
    using value_type = T;
    QuackAdapter() = default;
    ~QuackAdapter() = default;
    void push_back(const T &value) { q_.push(value); }
    void push_front(const T &value) { q_.push_front(value); }
    void pop_front() { q_.dequeue(); }
    void pop_back() { q_.pop(); }
    size_t size() { return q_.size(); }
};