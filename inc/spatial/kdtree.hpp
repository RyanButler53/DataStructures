#ifndef KD_TREE_HPP_INCLUDED
#define KD_TREE_HPP_INCLUDED

#include <array>
#include <vector>
#include <queue>
#include <map>
#include <exception>
#include <algorithm>
#include <numeric>
#include <functional>
#include "rectangle.hpp"

#include <iostream>

enum class DistanceFunction {
    Euclidean = 0,
    Manhattan = 1
};    


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
    /// @brief Keys are std::arrays of size K holding type T. All point types must be homogenous
    using key_t = std::array<T, K>;

    /// @brief Rectangle Type is a bounding box. 
    using rect_t = Rectangle<T, K>;

    // Constructor and destructors
    KDTree();
    ~KDTree();

    /**
     * @brief inserts a key into the tree
     * @param key Key is an std::array<T, K> 
     */
    void insert(const key_t& key);

    /**
     * Removes a key from the tree
     */
    void remove(const key_t& key);

    /**
     * Checks if the specified key is in the tree
     */
    bool contains(const key_t& key);

    /**
     * @brief Finds the minimum value along dimension dim
     * @param dim 
     */
    T findMin(size_t dim);

    // QUERIES

    /**
     * @brief returns the single key that is the closest neighbor to the query key
     * @param query Key to find the single closest neighbor point to the query. 
     * @note If the tree contains the query point, it will be returned.
     * @param fn [optional] Distance function. Defaults to Euclidean Distance. 
     */
    key_t nearestNeighbor(const key_t& query, DistanceFunction fn = DistanceFunction::Euclidean) const ;

    /** 
     * @brief Overload of the NearestNeighbors function but finds the K nearest neighbors. 
     * @param query Key to find closest neighbors
     * @param k Number of closest neighbors to find.
     * @param fn [optional] Distance function to use. Defaults to Euclidean, Manhattan distance also available. 
    */
    std::vector<key_t> nearestNeighbor(const key_t& query, size_t k, DistanceFunction fn = DistanceFunction::Euclidean) const;

    /**
     * @brief Radial Range query. Finds all points within a radius of r from a given key
     * @param query Key to find all points nearby
     * @param r Radius to search 
     * @param fn [optional] Distance function. Defaults ot Euclidean
     */
    std::vector<key_t> rangeQuery(const key_t& query, double r, DistanceFunction fn = DistanceFunction::Euclidean) const;

    /**
     * @brief Rectangle Range Query. Finds all points in a rectangle
     * @param bounds. Bounding Rectangle of the search mem
     */
    std::vector<key_t> rangeQuery(Rectangle<T,K> bounds) const;

    private:

    struct Node {
        key_t data_;
        Node* right_;
        Node* left_;
    };

    /**
     * @class BoundedPQ 
     * @brief Thin wrapper around std::priority_queue to have a max size
     */
    class BoundedPQ {
        private:
            std::priority_queue<std::pair<double, Node*>> pq_;
            size_t maxSize_;

        public:
        BoundedPQ(size_t maxsize):maxSize_{maxsize}{}
        ~BoundedPQ() = default;

        bool empty() { return pq_.empty();}
        void push(std::pair<double, Node*> item){
            if (pq_.size() < maxSize_){
                pq_.push(item);
            } else if (item < pq_.top()) {
                pop();
                push(item);
            }
        }
        void pop(){pq_.pop();}
        const std::pair<double, Node*>& top(){return pq_.top();}
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

    void rangeHelper(double r, const key_t& query, Node* curNode, size_t dim, std::vector<key_t>& keys, DistanceFunction fn) const;

    void rectangleRangeHelper(Rectangle<T,K> bounds, Node* curNode, size_t dim, std::vector<key_t>& keys) const;

    double dist(DistanceFunction fn, const key_t& p1, const key_t& p2) const;

    void knearestNeighborHelper(const key_t& query, Node*n, size_t curDim, DistanceFunction fn, BoundedPQ& best) const;
};

template <typename T>
using Tree2D = KDTree<T, 2>;

template <typename T>
using Tree3D = KDTree<T, 3>;

#include "kdtree-private.hpp"

#endif // KDTREE_HPP_INCLUDED
