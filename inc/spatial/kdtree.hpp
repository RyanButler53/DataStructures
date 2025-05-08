#ifndef KD_TREE_HPP_INCLUDED
#define KD_TREE_HPP_INCLUDED
#include <array>
#include <vector>
#include <queue>
#include <exception>
#include <algorithm>
#include <numeric>
#include <functional>


enum class DistanceFunction {
    Euclidean = 0,
    Manhattan = 1
    // Hamming, Minkowski?
};    

    /**
     *     
     * bool empty() { return pq.empty();}
    T top() const { return pq.top(); }
    void pop() { pq.pop(); }
    void push(T item, P priority) {pq.push(item); }
    size_t size() const { return pq.size(); }
     */



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

    /**
     * @brief returns the single key that is the closest neighbor to 
     */
    key_t nearestNeighbor(const key_t& query, DistanceFunction fn = DistanceFunction::Euclidean) const ;

    std::vector<key_t> kNearestNeighbors(const key_t& query, size_t k, DistanceFunction fn = DistanceFunction::Euclidean) const;

    /**
     * @brief Finds a vector of points within r range of key
     */
    std::vector<key_t> range(double r, const key_t& key, DistanceFunction fn = DistanceFunction::Euclidean) const;


    private:

    struct Node {
        key_t data_;
        Node* right_;
        Node* left_;
    };

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

    double dist(DistanceFunction fn, const key_t& p1, const key_t& p2) const;

    void nearestNeighborHelper(const key_t& query, Node* n, size_t curDim, DistanceFunction fn, double& bestSoFar, Node*& bestNode) const;

    void knearestNeighborHelper(const key_t& query, Node*n, size_t curDim, DistanceFunction fn, BoundedPQ& best) const;
};

template <typename T>
using Tree2D = KDTree<T, 2>;

template <typename T>
using Tree3D = KDTree<T, 3>;

#include "kdtree-private.hpp"

#endif

/**
 * Queries: 
 * NearestNeighbor(q, k, distfunc);
 * RangeRadius(q, r, distfunc)
 * BoundingBox(q, upper/lower bound for ALL dimensions) (0, 70, 80) -> 70 < dim0 < 80
 */
