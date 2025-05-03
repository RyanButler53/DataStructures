#include "kdtree.hpp"

template <typename T, size_t K>
KDTree<T, K>::KDTree():size_{0}, root_{nullptr}
{
}

template <typename T, size_t K>
KDTree<T, K>::~KDTree() {
    destructorHelper(root_);
}

template<typename T, size_t K>
void KDTree<T,K>::destructorHelper(Node*& tree){
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

template <typename T, size_t K>
void KDTree<T, K>::insert(const key_t& key) {
    insertHelper(key, root_, 0);
}

template <typename T, size_t K>
void KDTree<T, K>::insertHelper(const key_t& key, Node*& node, size_t dim) {
    if (!node) {
        node = new Node(key, nullptr, nullptr);
        ++size_;
    } else if (key == node->data_){ // key already exists
        return;
    } else if (key[dim] >= node->data_[dim]){
        insertHelper(key, node->right_, (dim+1)%K);
    } else {
        insertHelper(key, node->left_, (dim+1)%K);
    }
    
}

template <typename T, size_t K>
T KDTree<T, K>::findMin(size_t dim){
    if (dim >= K){
        throw std::runtime_error("Invalid Dimension");
    }
    Node* minNode = findMinHelper(dim, root_, 0);
    return minNode->data_[dim];
}

template <typename T, size_t K>
KDTree<T, K>::Node* KDTree<T, K>::findMinHelper(size_t dim, Node* node, size_t cur_dim){
    if (node == nullptr){
        return nullptr;
    } else if (cur_dim == dim) {
        if (node->left_ == nullptr){
            return node;
        } else {
            return findMinHelper(dim, node->left_, (cur_dim+1)%K);
        }
    } else { //  On a different dimension: 
        Node* left  = findMinHelper(dim, node->left_, (cur_dim+1)%K);
        Node* right = findMinHelper(dim, node->right_, (cur_dim+1)%K);
        auto cmp = [dim](Node* n1, Node* n2){return n1->data_[dim] < n2->data_[dim];};
        std::vector<Node*> nodes = {left, right, node};
        auto rm = std::ranges::remove_if(nodes, [](Node* n){return !n;});
        nodes.resize(std::distance(nodes.begin(), rm.begin()));
        return std::ranges::min(nodes, cmp); // Node* n1, Node*n2, 
        
    }
}

template <typename T, size_t K>
bool KDTree<T, K>::contains(const key_t& key){
    Node* n = searchHelper(key, root_, 0);
    return !!n;
}

template <typename T, size_t K>
KDTree<T, K>::Node* KDTree<T, K>::searchHelper(const key_t& key, Node* node, size_t cur_dim){
    if (!node){
        return node;
    } else if (node->data_ == key){
        return node;
    }else if (key[cur_dim] >= node->data_[cur_dim]){
        return searchHelper(key, node->right_, (cur_dim+1)%K);
    } else {
        return searchHelper(key, node->left_, (cur_dim+1)%K);
    }
}

//delete is particularly challenging - push this off. 
