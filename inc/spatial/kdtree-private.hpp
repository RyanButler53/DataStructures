#include "kdtree.hpp"

template <typename T, size_t K>
KDTree<T, K>::KDTree():size_{0}, root_{nullptr}
{
}

template <typename T, size_t K>
KDTree<T, K>::~KDTree()
{
}

template <typename T, size_t K>
void KDTree<T, K>::insert(const key_t& key) {
    insertHelper(key, root_, 0);
}

template <typename T, size_t K>
void KDTree<T, K>::insertHelper(const key_t& key, Node*& node, size_t dim) {
    if (!node) {
        node = new Node(key, nullptr, nullptr);
    } else {
        if (key[dim] > node->data[dim]){
            insertHelper(key, node->right_, (dim+1)%K);
        } else {
            insertHelper(key, node->right_, (dim+1)%K);
        }
    }
}
