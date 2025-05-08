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
        
        // Assemble list of nodes that are nullptr free
        std::vector<NodePair> nodes = {{node, cur_dim}};
        if (left.first) nodes.push_back(std::move(left));
        if (right.first) nodes.push_back(std::move(right));

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
    auto [n, ndim] = find(key, root_, 0);
    if (!n) {
        throw std::runtime_error("Cannot delete a key that doesn't exist");
    }
    removeHelper(key, n, ndim);
}

template <typename T, size_t K>
void KDTree<T,K>::removeHelper(const key_t& key, Node*& node, size_t dim){ // assuming that 
    // should never be a nullptr
    if (key == node->data_){
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
                // Traverse the tree for it: 
            } else { // there MUST be a left child
                auto [min, minDim] = findMinHelper(dim, node->left_, (dim+1)%K);
                node->data_ = min->data_;
                node->right_ = node->left_;
                node->left_ = nullptr;
            }
            // Recursively remove the data from the right subtree
            removeHelper(node->data_, node->right_, (dim+1)%K);
        }
    } else if (key[dim] >=node->data_[dim]){
        removeHelper(key, node->right_, (dim+1)%K);
    } else{
        removeHelper(key, node->left_, (dim+1)%K);
    }
    
}

template <typename T, size_t K>
std::vector<typename KDTree<T,K>::key_t> KDTree<T,K>::range(double r, const key_t& query, DistanceFunction fn) const {

    std::vector<key_t> keys;
    rangeHelper(r, query, root_, 0, keys, fn);
    return keys;
}

template <typename T, size_t K>
void KDTree<T,K>::rangeHelper(double r, const key_t& query, Node* curNode, size_t dim, 
                              std::vector<key_t>& keys, DistanceFunction fn) const {

    if (!curNode) {
        return; 
    }
    // Case where the point is out of range
    if (dist(fn, curNode->data_, query) > r){
        if (curNode->data_[dim] < (query[dim] - r)){ // only search keys in the right subtree
            rangeHelper(r, query, curNode->right_, (dim+1)%K, keys, fn);
        } else if (curNode->data_[dim] > (query[dim] + r)){ // only search left subtree
            rangeHelper(r, query, curNode->left_, (dim+1)%K, keys, fn);
        } else { // could be points in both halves
            rangeHelper(r, query, curNode->right_, (dim+1)%K, keys, fn);
            rangeHelper(r, query, curNode->left_, (dim+1)%K, keys, fn);
        }
    } else {  // point is in range. Search both sides
        keys.push_back(curNode->data_);
        rangeHelper(r, query, curNode->right_, (dim+1)%K, keys, fn);
        rangeHelper(r, query, curNode->left_, (dim+1)%K, keys, fn);
    }
    
}

template <typename T, size_t K>
double KDTree<T,K>::dist(DistanceFunction fn, const key_t& p1, const key_t& p2) const {
    T sum = 0;
    switch (fn)
    {
    case DistanceFunction::Euclidean:
        for (size_t i = 0; i < K; ++i){
            T diff = std::abs(p1[i]- p2[i]); 
            sum += diff*diff;
        }
        return std::sqrt(sum);
    case DistanceFunction::Manhattan:
        for (size_t i = 0; i < K; ++i){
            T diff = std::abs(p1[i]- p2[i]); 
            sum += diff;
        }
        return sum;
    default:
        break;
    }
}

// NEAREST NEIGHBOR

template <typename T, size_t K>
typename KDTree<T,K>::key_t KDTree<T,K>::nearestNeighbor(const key_t& query, DistanceFunction fn) const {
    double bestDist = std::numeric_limits<double>::max();
    if (!root_) throw std::invalid_argument ("Cannot find a nearest neighbor in an empty tree!");
    Node* bestNode = root_;
    nearestNeighborHelper(query, root_, 0, fn, bestDist, bestNode);
    return bestNode->data_;
}


template <typename T, size_t K>
void KDTree<T,K>::nearestNeighborHelper(const key_t& query, Node* n, size_t dim, DistanceFunction fn, double& bestSoFar, Node*& bestNode) const {
    if (!n){ return; }
    // Check distance and update new bests
    double newDist = dist(fn, n->data_, query);
    if (bestSoFar > newDist){
        bestSoFar = newDist;
        bestNode = n;
    }
    // Search for the node
    if (n->data_[dim] < query[dim]){ // search the right side first. 
        nearestNeighborHelper(query, n->right_, (dim+1)%K, fn, bestSoFar, bestNode);
        // Check if there is any possibility of there being a closer neighbor on the left subtree
        T deltaDist = query[dim] - n->data_[dim];
        if (deltaDist < bestSoFar){
            nearestNeighborHelper(query, n->left_, (dim+1)%K, fn, bestSoFar, bestNode);
        }
    } else if (n->data_[dim] > query[dim]){
        nearestNeighborHelper(query, n->left_, (dim+1)%K, fn, bestSoFar, bestNode);
        T deltaDist = n->data_[dim] - query[dim];
        if (deltaDist < bestSoFar){
            nearestNeighborHelper(query, n->right_, (dim+1)%K, fn, bestSoFar, bestNode);
        }
    } else { // could be points in both halves
        nearestNeighborHelper(query, n->right_, (dim+1)%K, fn, bestSoFar, bestNode);
        nearestNeighborHelper(query, n->left_, (dim+1)%K, fn, bestSoFar, bestNode);
    }
}
