#include "heap.hpp"

template <typename T, typename P>
BinomialHeap<T, P>::BinomialHeap():size_{0}, min_{nodes_.end()}
{

}

template <typename T, typename P>
BinomialHeap<T, P>::~BinomialHeap()
{
    // Has LOTS of memory to clean up
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
    return (*min_)->item_.item_;
}

template <typename T, typename P>
void BinomialHeap<T,P>::push(T item, P priority){
    
    nodes_.push_front(new Node({item, priority}));
    if (min_ == nodes_.end() or priority < (*min_)->item_.priority_){
        min_ = nodes_.begin();
    }
    ++size_;
}

template <typename T, typename P>
void BinomialHeap<T,P>::pop(){
    if (!size_){
        throw std::logic_error("Cannot pop from an empty heap");
    }
    // Step 1: From the minimum node: Iterate over the nodes and move them to the main list
    for (Node* n : (*min_)->children_){
        nodes_.push_back(n);
    }
    (*min_)->children_.clear();
    delete *min_;
    nodes_.erase(min_);
    min_ = nodes_.end();
    --size_;
    if (size_ > 1){
        cleanup();
    } else { // otherwise no cleanup needed
        // BUT MIN POINTER NEEDS TO BE SET
        min_ = nodes_.begin();
    }
}

template <typename T, typename P>
BinomialHeap<T,P>::Node* BinomialHeap<T,P>::mergeNodes(Node* n1, Node* n2){
    if (n1->item_.priority_ > n2->item_.priority_){
        n2->children_.push_back(n1);
        return n2;
    } else {
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
        nodes_.pop_front();

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

    P newMin;
    for (Node *n : nodeArray)
    {
        if (n){
            // If this is the new minimum, then keep track of it. 
            nodes_.push_front(n);
            if ((min_ == nodes_.end()) or (n->item_.priority_ < newMin)){
                newMin = n->item_.priority_;
                min_ = nodes_.begin();
            }
        }
    }
}

template <typename T, typename P>
void BinomialHeap<T,P>::decreaseKey(T item, P priority){
    // return to this later. May need parent pointers to get this "bubble up" correct
}

template <typename T, typename P>
BinomialHeap<T, P>::Node::Node(BinomialHeap<T,P>::Item item) : item_{item}{}

template <typename T, typename P>
BinomialHeap<T, P>::Node::~Node(){
    for (Node*& child : children_){
        delete child;
    }
}
