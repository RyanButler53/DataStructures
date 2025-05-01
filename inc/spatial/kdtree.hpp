#ifndef KD_TREE_HPP_INCLUDED
#define KD_TREE_HPP_INCLUDED
#include <array>

/**
 * @class KD Tree: A K dimensional tree. 
 * 
 * @tparam T: Type of data. Data is represented as an std::array
 * @tparam K: Dimension
 */
template <typename T, size_t K>
class KDTree
{
private:

    struct Node {
        std::array<T,K> data_;
        Node* right_;
        Node* left_;
    };
    
    size_t size_;
    Node* root_;

    // Helper methods

    void destructorHelper(Node*& node);

    void insertHelper(const key_t& key, Node*& node, size_t dim);

    void removeHelper(const key_t& key, Node* node);



public:
    using key_t = std::array<T, K>;

    KDTree();
    ~KDTree();

    // Inserts a key represented as an std::array
    void insert(const key_t& key);

    // Deletes a "random node"
    void remove(const key_t& key);

    // Partial Match query
    // Returns a vector of all keys that match the first T dimensions
    std::vector<key_t> search(const std::vector<T> query, size_t t);

    key_t nearestNeighbor(const key_t& query);
};

template <typename T>
using Tree2D = KDTree<T, 2>;

template <typename T>
using Tree3D = KDTree<T, 3>;

#include "kdtree-private.hpp"

#endif

