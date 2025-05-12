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
    using key_t = std::array<T, K>;

    // This is a wrapper around a map mapping dimension to lower and upper bounds
    class RectangleRQ {

        std::map<size_t, std::pair<T,T>> bounds_;

        public:
        void insert(size_t dim, T low, T high){
            if (dim > K){throw std::invalid_argument("Invalid Dimension");}
            bounds_[dim] = std::make_pair(low, high);
        }

        bool keyInside(const key_t& key) const {
            for (auto [dim, bound] : bounds_){
                // If any key is out of the bounds, return false
                if (key[dim] < bound.first or key[dim] > bound.second){
                    return false;
                }
            }
            // All dimensions are valid!
            return true;
        }

        std::pair<T,T> operator[](size_t dim){return bounds_[dim];}

        bool contains (size_t dim){return bounds_.contains(dim);} 

        void clear(){bounds_.clear();}
    };

    // Constructor and destructors
    KDTree();
    ~KDTree();

    // Inserts a key represented as an std::array
    void insert(const key_t& key);

    // Deletes a "random node"
    void remove(const key_t& key);

    bool contains(const key_t& key);

    T findMin(size_t dim);


    // QUERIES

    /**
     * @brief returns the single key that is the closest neighbor to 
     */
    key_t nearestNeighbor(const key_t& query, DistanceFunction fn = DistanceFunction::Euclidean) const ;

    std::vector<key_t> kNearestNeighbors(const key_t& query, size_t k, DistanceFunction fn = DistanceFunction::Euclidean) const;

    /**
     * @brief Finds the vector of points within a k-dimensional sphere of radius r from point key
     */
    std::vector<key_t> radialRangeQuery(double r, const key_t& key, DistanceFunction fn = DistanceFunction::Euclidean) const;

    /**
     * @brief Returns all points within the K dimensional bounding box. All dimensions need not be speified. 
     */
    std::vector<key_t> rectangleRangeQuery(RectangleRQ bounds) const;

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

    void rectangleRangeHelper(RectangleRQ bounds, Node* curNode, size_t dim, std::vector<key_t>& keys) const;

    double dist(DistanceFunction fn, const key_t& p1, const key_t& p2) const;

    void knearestNeighborHelper(const key_t& query, Node*n, size_t curDim, DistanceFunction fn, BoundedPQ& best) const;
};

template <typename T>
using Tree2D = KDTree<T, 2>;

template <typename T>
using Tree3D = KDTree<T, 3>;

#include "kdtree-private.hpp"

#endif // KDTREE_HPP_INCLUDED
