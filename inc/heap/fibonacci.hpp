#ifndef FIBONACCI_HEAP_HPP_INCLUDED
#define FIBONACCI_HEAP_HPP_INCLUDED

#include <vector>
#include <list>
#include <map>
#include <functional>

template <typename T, typename P, typename Compare = std::less<P>>
class FibonacciHeap {
  
    // Private struct to hold heap items
    struct Item {
        T item_;
        P priority_;
    };

    struct Node {        
        std::vector<Node*> children_;
        Item item_;
        Node *parent_;
        uint8_t index_;
        bool mark_;

        Node(Item item_);
        ~Node();
    };

    // Data
    std::list<Node *> nodes_;
    std::map<T, Node *> nodeMap_;
    Compare comp_;
    Node *min_;
    size_t size_;

    // Private Helper functions
    void cleanup();
    Node* mergeNodes(Node* n1, Node* n2);
    void cut(Node* c);
    void cascadingCut(Node* c);

    public:

    using value_type = T;
    using priority_type = P;

    FibonacciHeap();
    ~FibonacciHeap();

    bool empty() const {return size_ == 0;}
    size_t size() const {return size_;}
    void pop();
    T top() const;
    void push(T item, P priority);
    void changeKey(T item, P newPriority);
};

#include "fibonacci-private.hpp"

#endif // FIBONACCI_HEAP_HPP_INCLUDED
