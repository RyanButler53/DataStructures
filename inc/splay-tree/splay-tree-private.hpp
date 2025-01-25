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
void SplayTree<key_t, value_t>::destructorHelper(Node*& node){
    if (!node){
        return;
    }
    if (node->right_ != nullptr){
        destructorHelper(node->right_);
    }
    if (node->left_ != nullptr){
        destructorHelper(node->left_);
    }
    delete node;
}

template <typename key_t, typename value_t>
void SplayTree<key_t, value_t>::insertHelper(const key_t &key, const value_t &value, Node*& node, vector<Node *> &path){
    if (node == nullptr){
        node = new Node(key, value);
        path.push_back(node);
    } else{
        path.push_back(node);
        if (key > node->key_){
            insertHelper(key, value, node->right_, path);
        } else {
            insertHelper(key, value,node->left_, path);
        }
    }
    return;
}

template <typename key_t, typename value_t>
const typename SplayTree<key_t, value_t>::const_iterator
SplayTree<key_t, value_t>::searchHelper(const key_t &key, Node*& node, vector<Node*>& path){
    path.push_back(node);
    if (node == nullptr){
        return end();
    }
    if (key == node->key_)
    {
        return const_iterator(node);
    } else if (key > node->key_){
        return searchHelper(key, node->right_, path);
    } else {
        return searchHelper(key, node->left_, path);
    }
}

template <typename key_t, typename value_t>
void SplayTree<key_t, value_t>::printTree(Node* tree, ostream& out) const{
    if (tree == nullptr){
        out << "-";
    } else {
        out << "(";
        printTree(tree->left_, out);  
        out << ", " << tree->key_ << ":" << tree->value_ << ", ";
        printTree(tree->right_, out);
        out << ")";
    }
}

template <typename key_t, typename value_t>
void SplayTree<key_t, value_t>::insert(const key_t& key, const value_t& value){
    vector<Node *> path;
    insertHelper(key, value, root_, path);
    ++size_;
    splay(path);
    return;
}

template <typename key_t, typename value_t>
bool SplayTree<key_t, value_t>::exists(const key_t& key){
    vector<Node *> path;
    searchHelper(key, root_, path);
    bool notExists = !path.back();
    if (notExists){
        path.pop_back();
    }
    splay(path);
    return !notExists;
}

template <typename key_t, typename value_t>
void SplayTree<key_t, value_t>::remove(const key_t &key){
    // Not allowed to delete an empty treel 
    if (size_ == 0){
        return;
    }
    vector<Node *> path;
    searchHelper(key, root_, path);
    if (!path.back()){
        path.pop_back();
        splay(path);
        return;
    }
    --size_;
    splay(path);
    Node *leftSubtree = root_->left_;
    Node *rightSubtree = root_->right_;
    delete root_;
    if (!leftSubtree) {
        root_ = rightSubtree;
        return;
    } else if (!rightSubtree) {
        root_ = leftSubtree;
        return;
    }
    path.clear();
    getMaxPath(leftSubtree, path);
    splay(path);
    root_->right_ = rightSubtree;
    return;
}

template<typename key_t, typename value_t> 
void SplayTree<key_t, value_t>::getMaxPath(Node* node, vector<Node*>& path)const{
    while(node){
        path.push_back(node);
        node = node->right_;
    }
}


template <typename key_t, typename value_t>
const typename SplayTree<key_t, value_t>::const_iterator SplayTree<key_t, value_t>::find(const key_t &key){
    vector<Node *> path;
    const_iterator iter = searchHelper(key, root_, path);
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
    // key MUST be in the tree;
    vector<Node *> path;
    const const_iterator iter = searchHelper(key, root_, path);
    auto [k, v] = *iter;
    splay(path);
    if (k != key){
        throw("Invalid Key");
    } else {
        return v;
    }
}

// Splaying functions

template <typename key_t, typename value_t>
void SplayTree<key_t, value_t>::splay(vector<Node*>& path){
     Node *newNode = path.back();
    const key_t newKey = newNode->key_;

    // Invariant: Path.back() = new Node
    while (path.size() > 1) {
        // Special single rotation case
        if (path.size() == 2) {
            if (newKey > path[0]->key_){
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
            
            if(newKey > grandparent->key_ and newKey > parent->key_){
                rotateLeft(grandparent);
                swap(path[newNodeInd - 2], path[newNodeInd - 1]);
                rotateLeft(parent);
                swap(path[newNodeInd - 2], path[newNodeInd]); 
                cleanGreatGP(path, newNodeInd, newNode, newKey);
            } else if (newKey < grandparent->key_ and newKey > parent->key_) {
                rotateLeft(parent);
                swap(path[newNodeInd - 1], path[newNodeInd]);
                grandparent->left_ = newNode;
                rotateRight(grandparent);
                swap(path[newNodeInd - 2], path[newNodeInd-1]);
                cleanGreatGP(path, newNodeInd, newNode, newKey);
            } else if (newKey > grandparent->key_ and newKey < parent->key_) {
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
void SplayTree<key_t, value_t>::cleanGreatGP(vector<Node*>& path, size_t newNodeInd, Node*& newNode, const key_t& newKey){
    if (path.size() > 3){
        Node *ggp = path[newNodeInd - 3];
        if (ggp->key_ < newKey){
            ggp->right_ = newNode;
        } else {
            ggp->left_ = newNode;
        }
    } 
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

// Iterator Functions:

template <typename key_t, typename value_t>
typename SplayTree<key_t, value_t>::const_iterator SplayTree<key_t, value_t>::begin() const {
    return const_iterator(root_);
}

template <typename key_t, typename value_t>
typename SplayTree<key_t, value_t>::const_iterator SplayTree<key_t, value_t>::end() const {
    return const_iterator(nullptr);
}

template <typename key_t, typename value_t>
SplayTree<key_t, value_t>::const_iterator::const_iterator(Node* n, bool push){
    if (push){
        pushToMin(n);
    } else {
        stack_.push_back(n); // Just this node
    }
}

template <typename key_t, typename value_t>
typename SplayTree<key_t, value_t>::const_iterator& SplayTree<key_t, value_t>::const_iterator::operator++() {
    Node *current = stack_.back();
    stack_.pop_back();
    pushToMin(current->right_);
    return *this;
}

template <typename key_t, typename value_t>
typename SplayTree<key_t, value_t>::const_iterator::value_type SplayTree<key_t, value_t>::const_iterator::operator*() const{
    Node *curLoc = stack_.back();
    return {curLoc->key_, curLoc->value_};
}

template <typename key_t, typename value_t>
bool SplayTree<key_t, value_t>::const_iterator::operator==(const const_iterator& other) const{
    return (this->stack_ == other.stack_);
}

template <typename key_t, typename value_t>
bool SplayTree<key_t, value_t>::const_iterator::operator!=(const const_iterator& other) const{
    return !(*this == other);
}

template <typename key_t, typename value_t>
typename SplayTree<key_t, value_t>::const_iterator::pointer SplayTree<key_t, value_t>::const_iterator::operator->() const{
    return &(**this);
}

template<typename key_t, typename value_t>
void SplayTree<key_t, value_t>::const_iterator::pushToMin(Node* tree){
    while (tree){
        stack_.push_back(tree);
        tree = tree->left_;
    }
}

template<typename key_t, typename value_t>
void SplayTree<key_t, value_t>::print(ostream& out) const{
    printTree(root_, out);
}

template<typename key_t, typename value_t>
SplayTree<key_t, value_t>::Node::Node(key_t key, value_t value):
        key_{key},value_{value},right_{nullptr},left_{nullptr}{}

template <typename key_t, typename value_t>
std::ostream& operator<<(std::ostream& out, const SplayTree<key_t, value_t> &splaytree){
    splaytree.print(out);
    return out;
}