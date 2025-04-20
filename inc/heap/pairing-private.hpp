
#include "pairing.hpp"
#include <iostream>

template <typename T, typename P, typename Compare>
PairingHeap<T, P, Compare>::PairingHeap():size_{0},root_{nullptr},comp_{}{}

template <typename T, typename P, typename Compare>
PairingHeap<T, P, Compare>::~PairingHeap(){
    destructorHelper(root_);
}

template <typename T, typename P, typename Compare>
void PairingHeap<T, P, Compare>::push(T item, P priority){
    if (!root_) {
        root_ = new Node(Item{item, priority});
        nodeMap_.insert({item, root_});
    } else {
        Node* n = new Node(Item{item, priority});
        nodeMap_.insert({item, n});
        root_ = link(root_,n);
    }
    ++size_;
    
}

// Delete Min
template <typename T, typename P, typename Compare>
void PairingHeap<T, P, Compare>::pop(){
    // Pop is implemeted in 2 phases
    if (!size_){
        throw std::logic_error("Cannot pop from an empty heap");
    } 
    if (size_ == 1){
        nodeMap_.erase(root_->item_.item_);
        delete root_;
        root_ = nullptr;
        --size_;
        return;
    }
    // Get the nodes that are the direct child of the minimum
    std::vector<Node*> nodes;
    Node* curNode = root_->lchild_;
    while (curNode)
    {
        nodes.push_back(curNode);
        curNode = curNode->sibling_;
        // Siblings are undefined at this point. 
        nodes.back()->sibling_ = nullptr; 
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

    // Cleanup memory and replace root
    nodeMap_.erase(root_->item_.item_);
    delete root_;
    root_ = paired.back();
    paired.pop_back();
    root_->parent_ = nullptr;

    // Reverse list and pair them together
    std::ranges::reverse(paired);
    for (Node* n : paired){
        n->sibling_ = nullptr; // The trees are "separated" so their sibling is undefined.
        n->parent_ = nullptr; // The parent in the binary tree is also undefined. 
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
void PairingHeap<T, P, Compare>::changeKey(T item, P priority){
    // Cut the tree from its parent in the D-ary tree. 
    // then link the resulting trees. 
    if (!nodeMap_.contains(item)){
        throw std::invalid_argument("Cannot change a key value for a key not in the heap");
    }
    Node* n = nodeMap_.at(item);
    P curPriority = n->item_.priority_;
    if (comp_(curPriority, priority)) {
        throw std::invalid_argument("Cannot change key priority to this value");
    }

    // Case where the root is getting its key decreased
    if (n == root_){
        n->item_.priority_ = priority;
        return;
    }

    // 2 cases: Lchild case or Sibling case
    Node* p = n->parent_;
    if (n == p->lchild_){ // N is the "youngest" child
        if (n->sibling_){
            n->sibling_->parent_ = p;
            p->lchild_ = n->sibling_;
            n->sibling_ = nullptr;
        } else { // n is an only child
            p->lchild_ = nullptr;
        }
    } else { // N is any other child
        if (n->sibling_){
            p->sibling_ = n->sibling_;
            n->sibling_->parent_ = p;
            n->sibling_ = nullptr;
        } else { // oldest child case
            p->sibling_ = nullptr;
        }
    }
    n->parent_ = nullptr;
    n->item_.priority_ = priority;
    // link
    root_ = link(n, root_);
}

template <typename T, typename P, typename Compare>
PairingHeap<T, P, Compare>::Node* PairingHeap<T, P, Compare>::link(Node* n1, Node* n2){

    // n1 < n2 -> n1 is on top. 
    if (comp_(n1->item_.priority_,  n2->item_.priority_)){
        Node* temp = n1->lchild_;
        n1->lchild_ = n2;
        n2->parent_ = n1;
        if (temp) temp->parent_ = n2;
        n2->sibling_ = temp;
        return n1;
    } else { // n2 < n1, n2 is on "top"
        Node* temp  = n2->lchild_;
        n2->lchild_ = n1;
        n1->parent_ = n2;
        if (temp) temp->parent_ = n1;
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
