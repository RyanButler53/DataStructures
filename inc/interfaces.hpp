#pragma once

#include <concepts>

// Interface for a Tree Map structure
template <typename TreeType>
concept Tree = requires(TreeType &tree,
                        TreeType::value_type &pair,
                        TreeType::const_iterator iter,
                        const TreeType::key_type &key,
                        TreeType::mapped_type value) {
    tree.insert(pair);
    iter = tree.find(key);
    iter = tree.begin();
    iter = tree.end();
    tree.operator[](key);
    tree.find(key);
    tree.clear();
    tree.erase(key);
};

// Interface for a Heap (Supporting Dijkstra)
template <typename T>
concept Heap = requires(T &heap,
                        T::value_type item,
                        T::priority_type priority) {
    heap.empty();
    heap.size();
    heap.pop();
    heap.push(item, priority);
    heap.top();
    heap.changeKey(item, priority);
};

// Basic heap without changeKey()
template <typename T>
concept BasicHeap = requires(T &heap,
                             T::value_type value,
                             T::priority_type priority) {
    heap.empty();
    heap.size();
    heap.pop();
    heap.push(value,priority);
    heap.top();
};

// Double Ended Queue: 
template <typename Container>
concept Queue = requires(Container &container, const typename Container::value_type &value) {
    container.push_back(value);
    container.push_front(value);
    container.pop_front();
    container.pop_back();
    container.size();
};

// Linked List
template <typename Container>
concept LinkedList = requires(Container &container, 
                            Container::value_type& value, 
                            typename Container::iterator it) {
    container.push_back(value);
    container.push_front(value);
    container.pop_front();
    container.pop_back();
    container.size();
    container.clear();
    it = container.begin();
    it = container.end();
    container.insert(it, value); // Gives linked list fast insertion
};

template <typename Heap>
concept RunningMedian = requires(Heap &heap,
                              Heap::value_type value){
    heap.insert(value);
    heap.findMedian();
    heap.deleteMedian();
};