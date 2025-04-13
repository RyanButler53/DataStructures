#ifndef PAIRING_HEAP_HPP_INCLUDED
#define PAIRING_HEAP_HPP_INCLUDED

#include <functional>
#include <list>
#include <map>
#include <vector>

template <typename T, typename P, typename Compare = std::less<T>>
class PairingHeap {

    struct Item{
        T item_;
        P priority_;
    };


    struct Node
    {
        Item item_;
        Node* lchild_; // Points to its first child
        Node* sibling_; // Points to next "older" sibling
        Node* parent_; 

        Node(Item item_);

    };
    
    // Data
    std::map<T, Node *> nodeMap_;
    Compare comp_;
    Node *root_; // also the min!
    size_t size_;

    // Helper Functions
    void destructorHelper(Node*& node);

    // Returns the node that exists after linking
    Node* link(Node* n1, Node* n2);

  public: 
    using value_type = T;
    using priority_type = P;

    PairingHeap();
    ~PairingHeap();

    bool empty() const {return size_ == 0;}
    size_t size() const {return size_;}

    void pop();
    T top() const;
    void push(T item, P priority);
    void changeKey(T item, P newPriority);
};

#include "pairing-private.hpp"

#endif // PAIRING_HEAP_HPP_INCLUDED