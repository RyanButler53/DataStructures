#include "heap.hpp"
#include <iostream>
template <typename T, typename P>
FibonacciHeap<T,P>::FibonacciHeap():size_{0},min_{nullptr}{}

template <typename T, typename P>
FibonacciHeap<T,P>::~FibonacciHeap() {
    while (!nodes_.empty()) {
        Node *n = nodes_.front();
        delete n;
        nodes_.pop_front();
    }
}

template <typename T, typename P>
T FibonacciHeap<T,P>::top() const{
    if (!size_){
        throw std::logic_error("Cannot get top of an empty heap");
    }
    return min_->item_.item_;
}

template <typename T, typename P>
void FibonacciHeap<T,P>::pop(){
    if (!size_){
        throw std::logic_error("Cannot pop from an empty heap");
    }
    // Step 1: From the minimum node: Iterate over the nodes and move them to the main list
    for (Node* &n : min_->children_){
        n->parent_ = nullptr; // prevent dangling pointer. 
        nodes_.push_back(n);
    }
    // Step 2: Move the children to the main list
    T item = min_->item_.item_;
    nodeMap_.erase(item);
    min_->children_.clear();
    --size_;
    // Step 3: Cleanup
    cleanup();
}

template <typename T, typename P>
void FibonacciHeap<T,P>::push(T item, P priority){
    nodes_.push_front(new Node({item, priority}));
    nodeMap_.insert({item, nodes_.front()});
    if (min_ == nullptr or priority < min_->item_.priority_) {
        min_ = nodes_.front();
    }
    ++size_;
}

// Helper functions: 
template <typename T, typename P>
FibonacciHeap<T,P>::Node* FibonacciHeap<T,P>::mergeNodes(Node* n1, Node* n2){
    // Clear the mark on the CHILD NODE
    // May be possible to simplify this code. 
    if (n1->item_.priority_ > n2->item_.priority_){
        n1->parent_ = n2;
        n2->children_.push_back(n1);
        n1->index_ = n2->children_.size() -1;
        n1->mark_ = false;
        return n2;
    } else {
        n2->parent_ = n1;
        n1->children_.push_back(n2);
        n2->index_ = n1->children_.size() -1;
        n2->mark_ = false;
        return n1;
    }
}

template <typename T, typename P>
void FibonacciHeap<T,P>::cleanup() {
    short possibleRanks = std::ceil(std::log2(size_ + 1));
    std::vector<Node *> nodeArray(possibleRanks + 1);
    std::ranges::fill(nodeArray, nullptr);
    // Pop all nodes off node list and add them to the array and merge if necessary
    while (!nodes_.empty()){
        Node *n = nodes_.front();
        // If this is the min node, clean up its memory and move on
        if (n == min_){
            delete min_;
            min_ = nullptr;
            nodes_.pop_front();
            continue;
        }
        nodes_.pop_front();
        
        // Move the node and merge together
        size_t rank = n->children_.size();
        if (nodeArray[rank]){
            while(nodeArray[rank]){
                // Merge the nodes together
                n = mergeNodes(n, nodeArray[rank]);
                // At the END, they will be added to the list
                nodeArray[rank] = nullptr;
                ++rank;
            }
        } 
        // After all the combining, add to the list at the appropriate index.
        nodeArray[rank] = n;   
    }

    // Re add to node list
    for (Node *n : nodeArray) {
        if (n){
            // If this is the new minimum, then keep track of it. 
            nodes_.push_front(n);
            if ((min_ == nullptr) or (n->item_.priority_ < min_->item_.priority_)){
                min_ = n;
            }
        }
    }
}

template <typename T, typename P>
void FibonacciHeap<T,P>::decreaseKey(T item, P newPriority){
    if (!nodeMap_.contains(item)){
        throw std::invalid_argument("Cannot decrease a key value for a key not in the heap");
    }
    Node* n = nodeMap_.at(item);
    P curPriority = n->item_.priority_;
    if (newPriority >= curPriority) {
        throw std::invalid_argument("Cannot decrease a key value to a greater value");
    }

    n->item_.priority_ = newPriority;
    Node* p = n->parent_;
    // Check if tree structure needs changing
    if (p != nullptr and n->item_.priority_ < p->item_.priority_){
        cut(n);
        cascadingCut(p);
    }

    // Update minimum if necessary
    if (n->item_.priority_ < min_->item_.priority_){
        min_ = n;
    }
}

template <typename T, typename P>
void FibonacciHeap<T, P>::cut(Node* c){
    // Bookkeeping values
    Node* n = c->parent_;
    uint8_t oldIndex = c->index_;

    // Add to root list, set parent to nullptr, set index to 0
    nodes_.push_back(c);
    c->parent_ = nullptr;
    c->index_ = 0;
    c->mark_ = false;

    // Remove c from child list of n
    if (oldIndex == n->children_.size()-1){
        n->children_.pop_back();
    } else {
        n->children_[oldIndex] = n->children_.back();
        n->children_.pop_back();
        n->children_[oldIndex]->index_ = oldIndex;
    }
}

template <typename T, typename P>
void FibonacciHeap<T, P>::cascadingCut(Node* c){
    Node* p = c->parent_;
    if (p){
        if (!c->mark_){
            c->mark_ = true;
        } else {
            cut(c);
            cascadingCut(p);
        }
    }
}

// Node Functions: 
template <typename T, typename P>
FibonacciHeap<T, P>::Node::Node(FibonacciHeap<T,P>::Item item): 
    item_{item}, parent_{nullptr}, index_{0}, mark_{false}{}

template <typename T, typename P>
FibonacciHeap<T, P>::Node::~Node(){
    for (Node*& child : children_){
        delete child;
    }
}