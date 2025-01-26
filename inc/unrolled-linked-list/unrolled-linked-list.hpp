#ifndef UNROLLED_LINKED_LIST_HPP_INCLUDED
#define UNROLLED_LINKED_LIST_HPP_INCLUDED

#include <cstddef> // Unrolled linked list needs to have the interface of an std::list
// Unrolled Linked list is a wrapper around an std::list.
#include <list>

template <typename T, size_t K>
class UnrolledLinkedList
{
private:

    struct Node
    {
        std::array<T, K> data_;
        size_t size_;

        Node();

        bool isFull() const;
        void insert_back(const T &value);
        void insert_at(const T &value, size_t i);
        
    };

    std::list<Node> list_;
    size_t size_;

public:
    UnrolledLinkedList();
        
    // template <class InputIt>
    // UnrolledLinkedList<T>(InputIt begin, InputIt end);
    // assignment operator operator=
    
    ~UnrolledLinkedList() = default;

    size_t size() const;
    
    // Insertion
    void push_front(const T& value);
    void push_back(const T &value);
    // insert(iterator, value);

    // Element Access
    T& front();
    const T &front() const;

    T &back();
    const T &back() const;

    // Removal Functions();
    void pop_front();
    void pop_back();
    // void erase(iterator pos);

    void clear();

    
    // // template<container-compatible-range<T> rg>
    // // insert_range()
    // template <class iter>
    // void insert_range(iter begin, iter end);

    // void erase(iterator pos);

    // class iterator{
    // };
    
};

#include "unrolled-linked-list-private.hpp"

#endif

