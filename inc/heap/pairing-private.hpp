
#include "pairing.hpp"
#include <iostream>

template <typename T, typename P, typename Compare>
PairingHeap<T, P, Compare>::PairingHeap():size_{0},root_{nullptr},comp_{}{}

template <typename T, typename P, typename Compare>
PairingHeap<T, P, Compare>::~PairingHeap(){
    destructorHelper(root_);
}

// Delete Min
template <typename T, typename P, typename Compare>
void PairingHeap<T, P, Compare>::pop(){
    // Pop is implemeted in 2 phases
    if (!size_){
        throw std::logic_error("Cannot pop from an empty heap");
    } 
    if (size_ == 1){
        delete root_;
        root_ = nullptr;
        --size_;
        return;
    }
    std::vector<Node*> nodes;
    Node* curNode = root_->lchild_;
    while (curNode)
    {
        nodes.push_back(curNode);
        curNode =curNode->sibling_;
    }
    // Pair the nodes up
    std::vector<Node*> paired;
    for (size_t node_i = 0; node_i < nodes.size(); node_i+= 2){
        // Last node edge case
        if (node_i == nodes.size() - 1){
            paired.push_back(nodes.back());
        } else { // pair the two nodes together
            paired.push_back(link(nodes[node_i], nodes[node_i+1]));
        }
    }
    // bring them all together one by one
    delete root_;
    root_ = paired.back();
    paired.pop_back();
    std::ranges::reverse(paired);
    for (Node* n : paired){
        n->sibling_ = nullptr; // The trees are "separated" so their sibling is undefined.
        root_ = link(n, root_);
    }
    --size_;
}

template <typename T, typename P, typename Compare>
T PairingHeap<T, P, Compare>::top() const{
    if (!size_){
        throw std::logic_error("Cannot get top of an empty heap");
    } else {
        return root_->item_.item_;
    }
}

template <typename T, typename P, typename Compare>
void PairingHeap<T, P, Compare>::push(T item, P priority){
    if (!root_) {
            root_ = new Node(Item{item, priority});
    } else {
        root_ = link(root_,new Node(Item{item, priority}));
    }
    ++size_;
    
}

template <typename T, typename P, typename Compare>
void PairingHeap<T, P, Compare>::changeKey(T item, P priority){

}

template <typename T, typename P, typename Compare>
PairingHeap<T, P, Compare>::Node* PairingHeap<T, P, Compare>::link(Node* n1, Node* n2){
    // n2.sibling_ is pretty much always a nullptr. 

    if (n1->item_.priority_ < n2->item_.priority_){ // CUSTOM COMPARISON OPERATOR
        Node* temp = n1->lchild_;
        n1->lchild_ = n2;
        n2->sibling_ = temp;
        return n1;
    } else { // n2 < n1, n2 is on "top"
        Node* temp  = n2->lchild_;
        n2->lchild_ = n1;
        n1->sibling_ = temp;
        return n2;
    }
}

template <typename T, typename P, typename Compare>
PairingHeap<T, P, Compare>::Node::Node(Item item): 
    item_{item},
    lchild_{nullptr},
    sibling_{nullptr},
    parent_{nullptr}{}

template <typename T, typename P, typename Compare>
void PairingHeap<T, P, Compare>::destructorHelper(Node*& node){
    if (node != nullptr){
        destructorHelper(node->lchild_);
        destructorHelper(node->sibling_);
        delete node;        
    }
    
}
