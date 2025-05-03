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
    auto [minNode, d] = findMinHelper(dim, root_, 0);

    return minNode->data_[dim];
}

template <typename T, size_t K>
KDTree<T,K>::NodePair KDTree<T, K>::findMinHelper(size_t dim, Node* node, size_t cur_dim){
    if (node == nullptr){
        return {node, dim};
    } else if (cur_dim == dim) {
        if (node->left_ == nullptr){
            return {node, dim};
        } else {
            return findMinHelper(dim, node->left_, (cur_dim+1)%K);
        }
    } else { //  On a different dimension: 
        NodePair left  = findMinHelper(dim, node->left_, (cur_dim+1)%K);
        NodePair right = findMinHelper(dim, node->right_, (cur_dim+1)%K);
        auto cmp = [dim](NodePair n1, NodePair n2){return n1.first->data_[dim] < n2.first->data_[dim];};
        
        // find the minimum 
        std::vector<NodePair> nodes = {left, right, {node, cur_dim}};
        
        // Clear out nullptrs
        auto rm = std::ranges::remove_if(nodes, [](NodePair n){return !(n.first);});
        nodes.resize(std::distance(nodes.begin(), rm.begin()));
        // find minimum 
        return std::ranges::min(nodes, cmp); 
        
    }
}

template <typename T, size_t K>
bool KDTree<T, K>::contains(const key_t& key){
    auto [n, dim] = find(key, root_, 0);
    return !!n;
}

template <typename T, size_t K>
KDTree<T,K>::NodePair KDTree<T, K>::find(const key_t& key, Node* node, size_t cur_dim) {
    if (!node){
        return {node, cur_dim};
    } else if (node->data_ == key){
        return {node, cur_dim};
    }else if (key[cur_dim] >= node->data_[cur_dim]){
        return find(key, node->right_, (cur_dim+1)%K);
    } else {
        return find(key, node->left_, (cur_dim+1)%K);
    }
}

template <typename T, size_t K>
void KDTree<T,K>::remove(const key_t& key){
    auto [n, dim] = find(key, root_, 0);
    if (!n) {
        throw std::runtime_error("Cannot delete a key that doesn't exist");
    }
    removeHelper(n, dim);
}

template <typename T, size_t K>
void KDTree<T,K>::removeHelper(Node*& node, size_t dim){ // assuming that 
    // Base case: Leaf Node
    if (!node->left_ and !node->right_){
        delete node;
        node = nullptr;
        --size_;
    } else { // there will always be a right or left child
        // Find the "in order successor in the right tree"
        if (node->right_){
            // Find the min child of the current dimension in the rightChild
            auto [min, minDim] = findMinHelper(dim, node->right_, (dim+1)%K);
            // copy the data
            node->data_ = min->data_;
            removeHelper(min, minDim);
        } else { // there MUST be a left child
            auto [min, minDim] = findMinHelper(dim, node->left_, (dim+1)%K);
            node->data_ = min->data_;
            node->right_ = node->left_;
            node->left_ = nullptr;
            removeHelper(min, minDim);
        }
     }
}


// So are the queries!
