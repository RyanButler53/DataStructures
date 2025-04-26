/**
 * @file scapegoat.hpp
 * @author Ryan Butler (rbutler@g.hmc.edu)
 * @brief Implementation of class template for a Scapegoat Tree
 * @version 1.0
 * @date 7/4/2023
 * @copyright Copyright (c) 2023
 * 
 */
#include <vector>
#include <tuple>

#ifndef SCAPEGOAT_HPP_INCLUDED
#define SCAPEGOAT_HPP_INCLUDED

template <typename key_t, typename value_t>
class ScapegoatTree {
  private:
  
    struct Node {
        key_t key_;
        value_t value_;
        Node* left_;
        Node* right_;

        Node(key_t key, value_t value);
    };

    // Data:
    float alpha_;
    size_t size_;
    size_t maxSize_;
    Node *root_;

  public:
    // Constructors
    ScapegoatTree();
    ScapegoatTree(float alpha);
    ~ScapegoatTree();
    ScapegoatTree(ScapegoatTree &other) = delete;
    
    /**
     * @brief Inserts a key, value pair into the tree
     * 
     * @param key Key to insert
     * @param value Corresponding value to insert
     * @warning Inserting a value already in the tree causes undefined behavior
     */
    void insert(const key_t& key, const value_t& value);

    /**
     * @brief Checks if something is in the tree
     * 
     * @param key Key to check
     * @return true Key is in tree
     * @return false Key is not in tree
     */
    bool exists(const key_t& key) const;

    /**
     * @brief Looks up a key. Returns the value
     * 
     * @param key Key to lookup
     * @return int Corresponding value
     */
    value_t& search(const key_t& key) const;

    /**
     * @brief deletes a key-value pair from the tree
     * 
     * @param key key to delete
     */
    void remove(const key_t& key);

    /**
     * @brief Prints the tree
     * 
     */
    void print(std::ostream& out) const;

    /**
     * @brief Looks up the key, uses the search function
     * 
     * @param key Key to lookup
     * @return int corresponding value
     */
    value_t& operator[](const key_t& key) const;

private: 
    // Helper methods

    /**
     * @brief Destructor helper function
     * 
     * @param tree Current node. Destroys left, right, itself
     */
    void destructorHelper(Node* tree);

    /**
     * @brief checks if a key is in the tree
     * 
     * @param key key to check
     * @param tree curent node
     * @return true key is in tree
     * @return false key isn't in tree
     */
    bool existsHelper(const key_t& key, Node *tree) const;

    /**
     * @brief Inserts a key into the tree. Checks if restructuring is needed
     * 
     * @param key Key to be inserted
     * @param value value to be inserted
     * @param curDepth Current depth traveled. 
     * @return Whether or not a node was actually added
     */
    bool insertHelper(const key_t& key, const value_t& value, Node*& tree, 
     std::vector<Node*>& path);

    /**
     * @brief Finds the size of a given node
     * 
     * @param tree 
     * @return size_t 
     */
    size_t size(Node *tree) const;

    /**
     * @brief recursive helper for search function
     *
     * @param key Key being searched for
     * @param tree Current node searching
     * @return int value associated with the key
     */
    value_t& searchHelper(const key_t& key, Node *tree) const;

    /**
     * @brief Recursive helper for delete function
     * 
     * @param key key to delete. Must be in the tree
     * @param tree Current node
     */
    void removeHelper(const key_t& key, Node*& tree);

    /**
     * @brief Inserts something into the tree. For restructuring purposes
     * No reshuffling
     * 
     * @param key Key to insert
     * @param topNode Node to recurse on. 
     */
    void insertBasic(const key_t& key, const value_t& value, Node*& topNode);

    /**
     * @brief Get all elements below a given node
     * 
     * @param elements Vector to update with all the elements
     * @param tree Node in which to get all the elements under
     */
    void getElements(std::vector<std::tuple<key_t, value_t>>& elements, Node *tree);

    /**
     * @brief Recursive helper for printinf the tree
     * 
     * @param tree Current node to print
     */
    void printHelper(Node *tree, std::ostream& out) const;

    /**
     * @brief Builds a perfectly balanced tree with elements in it
     * 
     * @param elements Vector of keys/values to insert balanced
     * @param tree Current node
     * @param start Start ind of vector
     * @param end End ind of vector
     */
    void insertBalanced(std::vector<std::tuple<key_t, value_t>>& elements, 
    Node*& tree, size_t start, size_t end);

    /**
     * @brief Rebuilds a perfectly balanced tree rooted at node n
     * Takes O(n) space
     * 
     * @param n Root node of the balanced tree. O(size(n))
     */
    void rebuild(Node *& n);    
};

template<typename key_t, typename value_t>
std::ostream &operator<<(std::ostream& out, const ScapegoatTree<key_t, value_t>& sg);

#include "scapegoat-private.hpp"

#endif // SCAPEGOAT_HPP_INCLUDED
