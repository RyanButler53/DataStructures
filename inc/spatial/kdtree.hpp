#ifndef KD_TREE_HPP_INCLUDED
#define KD_TREE_HPP_INCLUDED
#include <array>
#include <vector>
#include <exception>
#include <algorithm>

/**
 * @class KD Tree: A K dimensional tree. 
 * 
 * @tparam T: Type of data. Data is represented as an std::array
 * @tparam K: Dimension
 */
template <typename T, size_t K>
class KDTree
{

struct Node;

public:
    using key_t = std::array<T, K>;

    KDTree();
    ~KDTree();

    // Inserts a key represented as an std::array
    void insert(const key_t& key);

    // Deletes a "random node"
    void remove(const key_t& key);

    bool contains(const key_t& key);

    T findMin(size_t dim);


    // Partial Match query
    // Returns a vector of all keys that match the first T dimensions
    std::vector<key_t> search(const std::vector<T> query, size_t t);

    key_t nearestNeighbor(const key_t& query);

    
    private:

    struct Node {
        key_t data_;
        Node* right_;
        Node* left_;

    };

    // A node and its dimension
    using NodePair = std::pair<Node* , size_t>;
    
    size_t size_;
    Node* root_;

    // Helper methods

    void destructorHelper(Node*& node);

    void insertHelper(const key_t& key, Node*& node, size_t dim);
    
    // Finds and returns the NODE and the dimension it is on
    NodePair find(const key_t& key, Node* node, size_t dim);

    NodePair findMinHelper(size_t dim, Node* node, size_t cur_dim);

    void removeHelper(const key_t& key, Node*& node, size_t dim);

};

template <typename T>
using Tree2D = KDTree<T, 2>;

template <typename T>
using Tree3D = KDTree<T, 3>;

#include "kdtree-private.hpp"

#endif

