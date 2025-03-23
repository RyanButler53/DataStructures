#include "heap.hpp"

template <typename T, typename P>
BinomialHeap<T, P>::BinomialHeap():size_{0}, min_{nullptr}{}

template <typename T, typename P>
BinomialHeap<T, P>::~BinomialHeap()
{
    while (!nodes_.empty()) {
        Node *n = nodes_.front();
        delete n;
        nodes_.pop_front();
    }
}

template <typename T, typename P>
bool BinomialHeap<T,P>::empty() const{
    return size_ == 0;
}

template <typename T, typename P>
size_t BinomialHeap<T,P>::size() const {
    return size_;
}

template <typename T, typename P>
T BinomialHeap<T,P>::top() const{
    if (!size_){
        throw std::logic_error("Cannot get top of an empty heap");
    }
    return min_->item_.item_;
}

template <typename T, typename P>
void BinomialHeap<T,P>::push(T item, P priority){
    
    nodes_.push_front(new Node({item, priority}));
    nodeMap_.insert({item, nodes_.front()});
    if (min_ == nullptr or priority < min_->item_.priority_) {
        min_ = nodes_.front();
    }
    ++size_;
}

template <typename T, typename P>
void BinomialHeap<T,P>::pop(){
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
BinomialHeap<T,P>::Node* BinomialHeap<T,P>::mergeNodes(Node* n1, Node* n2){
    if (n1->item_.priority_ > n2->item_.priority_){
        n1->parent_ = n2;
        n2->children_.push_back(n1);
        return n2;
    } else {
        n2->parent_ = n1;
        n1->children_.push_back(n2);
        return n1;
    }
}

template <typename T, typename P>
void BinomialHeap<T,P>::cleanup() {
    short possibleRanks = std::ceil(std::log2(size_ + 1));
    std::vector<Node *> nodeArray(possibleRanks);
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
void BinomialHeap<T,P>::decreaseKey(T item, P newPriority){
    if (!nodeMap_.contains(item)){
        throw std::invalid_argument("Cannot decrease a key value for a key not in the heap");
    }
    Node* n = nodeMap_.at(item);
    P curPriority = n->item_.priority_;
    if (newPriority >= curPriority) {
        throw std::invalid_argument("Cannot decrease a key value to a greater value");
    }

    n->item_.priority_ = newPriority;
    while (n->parent_ && n->parent_->item_.priority_ > n->item_.priority_)
    {
        // just swap the values not the actual nodes.
        Node *parent = n->parent_;
        std::swap(parent->item_, n->item_);
        std::swap(nodeMap_[parent->item_.item_], nodeMap_[n->item_.item_]);
        n = parent;
    }
    // Update minimum if necessary
    if (n->item_.priority_ < min_->item_.priority_){
        min_ = n;
    }
}

template <typename T, typename P>
BinomialHeap<T, P>::Node::Node(BinomialHeap<T,P>::Item item) : item_{item},parent_{nullptr}{}

template <typename T, typename P>
BinomialHeap<T, P>::Node::~Node(){
    for (Node*& child : children_){
        delete child;
    }
}
