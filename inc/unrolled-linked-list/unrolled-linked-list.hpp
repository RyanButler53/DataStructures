#ifndef UNROLLED_LINKED_LIST_HPP_INCLUDED
#define UNROLLED_LINKED_LIST_HPP_INCLUDED

#include <cstddef> // Unrolled linked list needs to have the interface of an std::list
// Unrolled Linked list is a wrapper around an std::list.
#include <list>
#include <iterator>

template <typename T, size_t K>
class UnrolledLinkedList
{
private:
    class Iterator;

    struct Node
    {
        std::array<T, K> data_;
        size_t size_;

        Node();

        bool isFull() const;
        void insert_back(const T &value);
        void insert_at(const T &value, size_t i);
        void delete_at(size_t i);
    };

    std::list<Node> list_;
    size_t size_;

public:
    using iterator = Iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using value_type = T;

    UnrolledLinkedList();

    // template <class InputIt>
    // UnrolledLinkedList<T>(InputIt begin, InputIt end);
    // assignment operator operator=
    
    ~UnrolledLinkedList() = default;

    size_t size() const;
    
    // Insertion
    // Iterators may be invalidated
    void push_front(const T& value);
    void push_back(const T &value);
    void insert(iterator& pos, const T& value);

    // Element Access
    T& front();
    const T &front() const;

    T &back();
    const T &back() const;

    // Deletion Functions
    void pop_front();
    void pop_back();
    iterator erase(iterator pos);

    void clear();

    // Iterator fuctions
    iterator begin();
    iterator end();

    reverse_iterator rbegin();
    reverse_iterator rend();


    
    // // template<container-compatible-range<T> rg>
    // // insert_range()
    // template <class iter>
    // void insert_range(iter begin, iter end);

    // void erase(iterator pos);

    // class iterator{
    // };
  private:
    class Iterator{
        friend class UnrolledLinkedList;

        private:
            std::list<Node>::iterator listIter_;
            size_t nodeInd_;

        public:
            using value_type = T;
            using reference = const value_type&;
            using pointer = const value_type*;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::bidirectional_iterator_tag;

            Iterator() = default;
            // Implmentation of which hidden list iterator is unknown to users
            Iterator(std::list<Node>::iterator listIter);
            Iterator (const Iterator& other) = default;
            Iterator &operator=(const Iterator &other) = default;
            ~Iterator() = default;

            value_type operator*() const;
            Iterator &operator++();
            Iterator &operator--();
            pointer operator->() const;
            bool operator==(const Iterator &other) const;
            bool operator!=(const Iterator &other) const;
    };
};

#include "unrolled-linked-list-private.hpp"

#endif

