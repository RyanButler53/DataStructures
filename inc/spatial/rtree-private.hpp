#ifndef R_TREE_HPP_INCLUDED
#define R_TREE_HPP_INCLUDED


#include <array>
#include <vector>
#include "rectangle.hpp"

/**
 * @class R Tree: A K dimensional R tree. 
 * 
 * @tparam T: Type of data. Data is represented as an std::array
 * @tparam K: Dimension
 */
template <typename T, size_t K>
class RTree {

    public: 

    /// @brief Keys are std::arrays of size K holding type T. All point types must be homogenous
    using key_t = std::array<T, K>;

    /// @brief Rectangle Type is a bounding box. 
    using rect_t = Rectangle<T, K>;

    private: 
    // Initial attempt at an R tree implementation
    struct Node {

        virtual bool isLeaf() const = 0;
        virtual void insert( const key_t& key) = 0;
        virtual void split() const = 0;
    };

    struct InternalNode : public Node{};
    struct LeafNode : public Node{};

    /**
     * Problems and challenges: 
     * The logic for insertion is baked into the nodes, not the tree
     * The splitting logic isn't going to update the parents correctly. 
     * No delete...
     * Rectangle class may not be sufficient
     */
    public:


    RTree();
    ~RTree();

    /**
     * @brief inserts a key into the tree
     * @param key Key is an std::array<T, K> 
     */
    void insert(const key_t& key);

    /**
     * Removes a key from the tree
     */
    // void remove(const key_t& key);

    /**
     * Checks if the specified key is in the tree
     */
    // bool contains(const key_t& key);

/**
     * @brief returns the single key that is the closest neighbor to the query key
     * @param query Key to find the single closest neighbor point to the query. 
     * @note If the tree contains the query point, it will be returned.
     */
    key_t nearestNeighbor(const key_t& query) const ;

    /** 
     * @brief Overload of the NearestNeighbors function but finds the K nearest neighbors. 
     * @param query Key to find closest neighbors
     * @param k Number of closest neighbors to find.
    */
    std::vector<key_t> nearestNeighbor(const key_t& query, size_t k) const;

    /**
     * @brief Radial Range query. Finds all points within a radius of r from a given key
     * @param query Key to find all points nearby
     * @param r Radius to search 
     */
    std::vector<key_t> rangeQuery(const key_t& query, double r) const;

    /**
     * @brief Rectangle Range Query. Finds all points in a rectangle
     * @param bounds. Bounding Rectangle of the search region
     */
    std::vector<key_t> rangeQuery(Rectangle<T,K> bounds) const;


};

template <typename T>
using RTree2D = RTree<T, 2>;

template <typename T>
using RTree3D = RTree<T, 3>;

#include "rtree-private.hpp"

#endif // RTREE_HPP_INCLUDED
