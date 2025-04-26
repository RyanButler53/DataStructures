#include "scapegoat.hpp"
#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

template<typename key_t, typename value_t>
ScapegoatTree<key_t, value_t>::ScapegoatTree():
alpha_{2.0/3}, size_{0}, maxSize_ {0},root_{nullptr}
{
    // Nothing to do here
}

template<typename key_t, typename value_t>
ScapegoatTree<key_t, value_t>::ScapegoatTree(float alpha):
alpha_{alpha}, size_{0}, maxSize_{0}, root_{nullptr}
{
    // Nothing to do here
}

template<typename key_t, typename value_t>
ScapegoatTree<key_t, value_t>::~ScapegoatTree()
{
    destructorHelper(root_);
}

template<typename key_t, typename value_t>
void ScapegoatTree<key_t, value_t>::destructorHelper(Node* tree){
    if (tree != nullptr){
        if (tree->left_ != nullptr){
            destructorHelper(tree->left_);
        }
        if (tree->right_ != nullptr){
            destructorHelper(tree->right_);
        }
        delete tree;
    }
}

template<typename key_t, typename value_t>
bool ScapegoatTree<key_t, value_t>::exists(const key_t& key) const {
    return existsHelper(key, root_);
}

template<typename key_t, typename value_t>
bool ScapegoatTree<key_t, value_t>::existsHelper(const key_t& key, Node *tree) const {
    if (tree == nullptr){
        return false;
    } else if (tree->key_ == key)
        return true;
    else if (tree->key_ > key){
        return existsHelper(key, tree->right_);
    } else {
        return existsHelper(key, tree->left_);
    }
}
template<typename key_t, typename value_t>
value_t& ScapegoatTree<key_t, value_t>::search(const key_t& key) const{
    // Assume user has already run exists.
    return searchHelper(key, root_);
}

template<typename key_t, typename value_t>
value_t& ScapegoatTree<key_t, value_t>::searchHelper(const key_t& key, Node* tree) const{
    if (tree->key_ == key){
        return tree->value_;
    } else if (key > tree->key_ ) {
        return searchHelper(key, tree->right_);
    } else {
        return searchHelper(key, tree->left_);
    }
}

template<typename key_t, typename value_t>
void ScapegoatTree<key_t, value_t>::insert(const key_t& key, const value_t& value){

    // Max depth +1 since we include root
    size_t maxDepth = size_t(floor(-log(size_+1) / log(alpha_) + 1));
    vector<Node*> path;
    bool inserted = insertHelper(key, value, root_, path);
    if (inserted){
        ++size_;
    }
    // Update Max Size if needed
    if (maxSize_ < size_){
        ++maxSize_;
    }
    // Check if there was a deep node
    if (path.size() > maxDepth) {
        size_t *nodeSizes = new size_t[path.size()];
        nodeSizes[0] = 1; //  leaf has size 1
        for (size_t i = 1; i < path.size(); ++i) {
            // i+1 is parent of i. If nodes[i]->key > nodes[i-1]->key, check right
            size_t otherSize;
            // Checks if node i is a right child of its parent
            if (path[i]->key_ > path[i - 1]->key_) {
                otherSize = size(path[i]->right_);
            }
            else { // look in left child.
                otherSize = size(path[i]->left_);
            }
            size_t nodeSize = 1 + nodeSizes[i - 1] + otherSize;
            nodeSizes[i] = nodeSize;

            // Check if alpha balanced: Child/Parent > alpha
            if (float(nodeSizes[i-1])/nodeSizes[i] > alpha_){
                Node* scapegoat = path[i];
                // Have a scapegoat node. Rebuild entire tree.
                rebuild(scapegoat);
                
                // Set the correct child of the scapegoat node to the scapegoat node
                if (scapegoat->key_ > path[i+1]->key_){
                    path[i + 1]->right_ = scapegoat;
                } else {
                    path[i + 1]->left_ = scapegoat;
                }
                break;
            }
        }
        delete[] nodeSizes;
    }
}

template<typename key_t, typename value_t>
bool  ScapegoatTree<key_t, value_t>::insertHelper(const key_t& key, const value_t& value, Node*& tree, 
            vector<Node*>& path){
    bool result;
    if (tree == nullptr){
        tree = new Node{key, value};
        path.push_back(tree);
        result = true;
    } else if (key > tree->key_) {
        result = insertHelper(key, value, tree->right_, path);
        path.push_back(tree);
    } else if (key < tree->key_){
        result = insertHelper(key, value, tree->left_, path);
        path.push_back(tree);
    } else {
        return false;
    }
    return result;
}

template<typename key_t, typename value_t>
size_t ScapegoatTree<key_t, value_t>::size(Node* tree) const{
    if (tree == nullptr){
        return 0;
    } else {
        return 1 + size(tree->left_) + size(tree->right_);
    }
}

template<typename key_t, typename value_t>
void ScapegoatTree<key_t, value_t>::remove(const key_t& key){
    // Assumes that the key is in the tree
    removeHelper(key, root_);
    --size_;
    // Restructure if necessary
    if (size_ < alpha_ * maxSize_) {
        maxSize_ = size_;
        rebuild(root_);
    }
    return;
}

template<typename key_t, typename value_t>
void ScapegoatTree<key_t, value_t>::rebuild(Node*& n){
    vector<tuple<key_t, value_t>> elements;
    getElements(elements, n);
    // destroy entire old tree
    destructorHelper(n);
    n = nullptr;
    insertBalanced(elements, n, 0, elements.size());
    return;
}

template<typename key_t, typename value_t>
void ScapegoatTree<key_t, value_t>::getElements(vector<tuple<key_t,value_t>>& elements, Node* tree){
    if (tree != nullptr){
        getElements(elements, tree->left_);
        elements.push_back(tuple(tree->key_, tree->value_));
        getElements(elements, tree->right_);
    }
}

template<typename key_t, typename value_t>
void ScapegoatTree<key_t, value_t>::removeHelper(const key_t& key, Node*& tree){
    if (key == tree->key_) {
        if (tree->right_ == nullptr and tree->left_ == nullptr) { // Leaf Case
            delete tree;
            tree = nullptr;
        }
        else if (tree->right_ != nullptr and tree->left_ == nullptr)
        {
            //one child case
            Node *oldTop = tree;
            tree = tree->right_;
            delete oldTop;
            oldTop = nullptr;
        }
        else if (tree->right_ == nullptr and tree->left_ != nullptr) {
            Node *oldTop = tree;
            tree = tree->left_;
            delete oldTop;
        } else {
            // Find min
            Node *minParent = tree;
            Node *min = tree->right_;
            while (min->left_ != nullptr) {
                minParent = min;
                min = min->left_;
            }

            // Set the parent's left child to the other nodes greater than min
            minParent->left_ = min->right_;

            // Set the pointers for the new top
            min->right_ = tree->right_;
            min->left_ = tree->left_;

            // Clean up memory and assign to tree variable
            delete tree;
            tree = min;

        }
    }
    else if (key > tree->key_) {
        removeHelper(key, tree->right_);
    }  else if (key < tree->key_) {
        removeHelper(key, tree->left_);
    }
    return;
}
template<typename key_t, typename value_t>
void ScapegoatTree<key_t, value_t>::insertBalanced(std::vector<std::tuple<key_t, value_t>>& elements,
         Node*& tree, size_t start, size_t end){
    if (start != end) { 
        size_t mid = start + (end - start) / 2;
        auto [k, v] = elements[mid];
        insertBasic(k, v, tree);
        insertBalanced(elements, tree->left_, start, mid);
        insertBalanced(elements, tree->right_, mid + 1, end);
    }
}

template<typename key_t, typename value_t>
void ScapegoatTree<key_t, value_t>::insertBasic(const key_t& key, const value_t& value, Node*& topNode){
    if (topNode == nullptr) {
        topNode = new Node{key, value};
    }
    else if (key > topNode->key_) {
        insertBasic(key, value, topNode->right_);
    } else {
        insertBasic(key, value, topNode->left_);
    }
}

template<typename key_t, typename value_t>
void ScapegoatTree<key_t, value_t>::print(ostream& out) const{
    printHelper(root_, out);
    out << endl << "Alpha: " << alpha_ << " Size: " << size_;
}

template<typename key_t, typename value_t>
void ScapegoatTree<key_t, value_t>::printHelper(Node* tree, ostream& out) const{
    if (tree == nullptr){
        out << "-";
    } else {
        out << "(";
        printHelper(tree->left_, out);  
        out << ", " << tree->key_ << ":" << tree->value_ << ", ";
        printHelper(tree->right_, out);
        out << ")";
    }
}

template<typename key_t, typename value_t>
value_t& ScapegoatTree<key_t, value_t>::operator[](const key_t& key) const{
    return searchHelper(key, root_);
}

template<typename key_t, typename value_t>
ScapegoatTree<key_t, value_t>::Node::Node(key_t key, value_t value):
key_{key}, value_{value}, left_{nullptr}, right_{nullptr}{
}


template<typename key_t, typename value_t>
ostream& operator<<(ostream& out, const ScapegoatTree<key_t, value_t>& sg){
    sg.print(out);
    return out;
}