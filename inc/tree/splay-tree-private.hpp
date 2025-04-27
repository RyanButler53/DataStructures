#include "splay-tree.hpp"

using namespace std;

template <typename key_t, typename value_t>
SplayTree<key_t, value_t>::SplayTree(): root_{nullptr}, size_{0}
{
    // Nothing to do here
}

template <typename key_t, typename value_t>
SplayTree<key_t, value_t>::~SplayTree()
{
    destructorHelper(root_);
}

template <typename key_t, typename value_t>
void SplayTree<key_t, value_t>::pushToMin(std::vector<Node *> &stack, Node *startNode){
    while (startNode) {
        stack.push_back(startNode);
        startNode = startNode->left_;
    }
}


template <typename key_t, typename value_t>
void SplayTree<key_t, value_t>::destructorHelper(Node*& node){
    // Check if all memory is already cleaned up
    if (!root_){
        return;
    }

    // Setup stack 
    std::vector<Node *> stack{root_};
    
    Node *rootLeft = root_->left_;
    pushToMin(stack, rootLeft);

    while (!stack.empty()) {
        Node* curNode = stack.back();
        stack.pop_back();
        Node *rightNode = curNode->right_;
        pushToMin(stack, rightNode);
        delete curNode;
    }
}

// May have tail call recursion optimization. 
template <typename key_t, typename value_t>
const typename SplayTree<key_t, value_t>::const_iterator
SplayTree<key_t, value_t>::search(const key_t &key, std::vector<Node*>& path){
    Node **node = &root_;
    while(true){
        path.push_back(*node);
        if (*node == nullptr) {
            return end();
        } else if (key == (*node)->value_.first) {
            return const_iterator(*node);
        } else if (key > (*node)->value_.first){
            node = &((*node)->right_);
        } else {
            node = &((*node)->left_);
        }
    }
}

template <typename key_t, typename value_t>
void SplayTree<key_t, value_t>::printTree(Node* tree, ostream& out) const{
    if (tree == nullptr){
        out << "-";
    } else {
        out << "(";
        printTree(tree->left_, out);  
        out << ", " << tree->value_.first << ":" << tree->value_.second << ", ";
        printTree(tree->right_, out);
        out << ")";
    }
}

template <typename key_t, typename value_t> 
std::pair<typename SplayTree<key_t, value_t>::Iterator, bool>  SplayTree<key_t, value_t>::insert(const value_type& val){ // save this sytax for EMPLACE: const key_t& key, value_t& value
    const key_t& key = val.first;
    const value_t& value = val.second;
    vector<Node *> path;

    Node** node = &root_;
    bool success;
    while (true)
    {
        if (*node == nullptr){
             *node = new Node(key, value);
            path.push_back(*node);
            success = true;
            break;
        }
        path.push_back(*node);
        if (key > (*node)->value_.first) {
            node = &((*node)->right_);
        } else if (key < (*node)->value_.first) {
            node = &((*node)->left_);
        } else
        { // equal
            success = false;
            break;
        }
    }

    splay(path);
    // success
    size_ = size_ + int(success);

    // Leverage Splay operation: new node or blocking node is always at the root!
    return std::make_pair(Iterator(root_,false), success);
}

template <typename key_t, typename value_t>
template<class InputIt> 
void SplayTree<key_t, value_t>::insert(InputIt first, InputIt last){
    for (InputIt it = first; it != last; ++it){
        insert(*it);
    }
}


template <typename key_t, typename value_t>
bool SplayTree<key_t, value_t>::contains(const key_t& key){
    vector<Node *> path;
    search(key, path);
    bool notExists = !path.back();
    if (notExists){
        path.pop_back();
    }
    splay(path);
    return !notExists;
}

template <typename key_t, typename value_t>
size_t SplayTree<key_t, value_t>::erase(const key_t &key){
    // Not allowed to delete an empty tree! 
    if (size_ == 0){
        return 0;
    }
    vector<Node *> path;
    search(key, path);
    if (!path.back()){
        path.pop_back();
        splay(path);
        return 0;
    }
    --size_;
    splay(path);
    Node *leftSubtree = root_->left_;
    Node *rightSubtree = root_->right_;
    delete root_;

    if (!leftSubtree) {
        root_ = rightSubtree;
        return 1;
    } else if (!rightSubtree) {
        root_ = leftSubtree;
        return 1;
    }
    // Splay the rest of the 
    path.clear();
    getMaxPath(leftSubtree, path);
    splay(path);
    root_->right_ = rightSubtree;
    return 1;
}

template<typename key_t, typename value_t> 
void SplayTree<key_t, value_t>::getMaxPath(Node* node, vector<Node*>& path)const{
    while(node){
        path.push_back(node);
        node = node->right_;
    }
}


template <typename key_t, typename value_t>
typename SplayTree<key_t, value_t>::const_iterator SplayTree<key_t, value_t>::find(const key_t &key){
    vector<Node *> path;
    const_iterator iter = search(key, path);
    if (!path.back()){
        path.pop_back();
        return end();
    }
    splay(path);
    return const_iterator(root_, false);
}

template <typename key_t, typename value_t>
size_t SplayTree<key_t, value_t>::size() const{
    return size_;
}

template <typename key_t, typename value_t>
value_t& SplayTree<key_t, value_t>::operator[](const key_t& key){
    Iterator it = find(key);
    if (it != end()){
        return it->second;
    }
    return insert({key, value_t()}).first->second;
}

// Splaying functions

template <typename key_t, typename value_t>
void SplayTree<key_t, value_t>::splay(vector<Node*>& path){
    // Nothing to splay if no path
    if (path.size() == 0){
        return;
    }
    Node *newNode = path.back();
    const key_t newKey = newNode->value_.first;

    // Invariant: Path.back() = new Node
    while (path.size() > 1) {
        // Special single rotation case
        if (path.size() == 2) {
            if (newKey > path[0]->value_.first){
                rotateLeft(path[0]);
            } else {
                rotateRight(path[0]);
            }
            swap(path[0], path[1]);
            path.pop_back();
        } else { // Double Rotations cases
            size_t newNodeInd = path.size() - 1;
            Node *parent = path[newNodeInd - 1];
            Node *grandparent = path[newNodeInd - 2];
            
            if(newKey > grandparent->value_.first and newKey > parent->value_.first){
                rotateLeft(grandparent);
                swap(path[newNodeInd - 2], path[newNodeInd - 1]);
                rotateLeft(parent);
                swap(path[newNodeInd - 2], path[newNodeInd]); 
                cleanGreatGP(path, newNodeInd, newNode, newKey);
            } else if (newKey < grandparent->value_.first and newKey > parent->value_.first) {
                rotateLeft(parent);
                swap(path[newNodeInd - 1], path[newNodeInd]);
                grandparent->left_ = newNode;
                rotateRight(grandparent);
                swap(path[newNodeInd - 2], path[newNodeInd-1]);
                cleanGreatGP(path, newNodeInd, newNode, newKey);
            } else if (newKey > grandparent->value_.first and newKey < parent->value_.first) {
                rotateRight(parent);
                swap(path[newNodeInd - 1], path[newNodeInd]);
                grandparent->right_ = newNode;
                rotateLeft(grandparent);
                swap(path[newNodeInd - 2], path[newNodeInd-1]);
                cleanGreatGP(path, newNodeInd, newNode, newKey);
            } else { // newKey < grandparent and newKey < parent
                rotateRight(grandparent);
                swap(path[newNodeInd - 2], path[newNodeInd - 1]);
                rotateRight(parent);
                swap(path[newNodeInd - 2], path[newNodeInd]); 
                cleanGreatGP(path, newNodeInd, newNode, newKey);
            }
            // Pop back to maintain invariant.
            path.pop_back();
            path.pop_back();
        }
    }
    root_ = path[0];
    return;
}

template <typename key_t, typename value_t>
void SplayTree<key_t, value_t>::rotateRight(Node *&node){
    Node *oldtop = node;
    Node *newtop = node->left_;
    if (newtop) {
        oldtop->left_ = newtop->right_;
    }
    newtop->right_ = oldtop;
    node = newtop;
    return;
}


template <typename key_t, typename value_t>
void SplayTree<key_t, value_t>::rotateLeft(Node *&node){
    Node *oldtop = node;
    Node *newtop = node->right_;
    if (newtop) {
        oldtop->right_ = newtop->left_;
    }
    newtop->left_ = oldtop;
    node = newtop;
    return;
}

template <typename key_t, typename value_t>
void SplayTree<key_t, value_t>::cleanGreatGP(vector<Node*>& path, size_t newNodeInd, Node*& newNode, const key_t& newKey){
    if (path.size() > 3){
        Node *ggp = path[newNodeInd - 3];
        if (ggp->value_.first < newKey){
            ggp->right_ = newNode;
        } else {
            ggp->left_ = newNode;
        }
    } 
}
// Iterator Functions:

template <typename key_t, typename value_t>
typename SplayTree<key_t, value_t>::Iterator SplayTree<key_t, value_t>::begin() const {
    return Iterator(root_);
}

template <typename key_t, typename value_t>
typename SplayTree<key_t, value_t>::Iterator SplayTree<key_t, value_t>::end() const {
    return Iterator(nullptr);
}

template <typename key_t, typename value_t>
SplayTree<key_t, value_t>::Iterator::Iterator(Node* n, bool push){
    if (push){
        pushToMin(n);
    } else {
        stack_.push_back(n); // Just this node
    }
}

template <typename key_t, typename value_t>
typename SplayTree<key_t, value_t>::Iterator& SplayTree<key_t, value_t>::Iterator::operator++() {
    Node *current = stack_.back();
    stack_.pop_back();
    pushToMin(current->right_);
    return *this;
}

template <typename key_t, typename value_t>
typename SplayTree<key_t, value_t>::Iterator::value_type SplayTree<key_t, value_t>::Iterator::operator*() const{
    return stack_.back()->value_;
}

template <typename key_t, typename value_t>
bool SplayTree<key_t, value_t>::Iterator::operator==(const Iterator& other) const{
    return (this->stack_ == other.stack_);
}

template <typename key_t, typename value_t>
bool SplayTree<key_t, value_t>::Iterator::operator!=(const Iterator& other) const{
    return !(*this == other);
}

template <typename key_t, typename value_t>
typename SplayTree<key_t, value_t>::Iterator::pointer SplayTree<key_t, value_t>::Iterator::operator->() const{
    return &(stack_.back()->value_);
}

template<typename key_t, typename value_t>
void SplayTree<key_t, value_t>::Iterator::pushToMin(Node* tree){
    while (tree){
        stack_.push_back(tree);
        tree = tree->left_;
    }
}

template<typename key_t, typename value_t>
void SplayTree<key_t, value_t>::clear(){
    destructorHelper(root_);
    root_ = nullptr;
    size_ = 0;
}

template<typename key_t, typename value_t>
void SplayTree<key_t, value_t>::print(ostream& out) const{
    printTree(root_, out);
}

template<typename key_t, typename value_t>
SplayTree<key_t, value_t>::Node::Node(key_t key, value_t value):
        value_{std::make_pair(key, value)},right_{nullptr},left_{nullptr}{}

template <typename key_t, typename value_t>
std::ostream& operator<<(std::ostream& out, const SplayTree<key_t, value_t> &splaytree){
    splaytree.print(out);
    return out;
}
