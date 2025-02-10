/**
 * @file splayTree.hpp
 * @author Ryan Butler (rbutler@g.hmc.edu)
 * @brief Implementation of a splay tree with no parent pointer
 * @version 0.1
 * @date 2023-07-08
 * @note Full documentation is in README.md
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef SPLAY_TREE_HPP_INCLUDED
#define SPLAY_TREE_HPP_INCLUDED

#include <vector>
#include <iterator>
#include <utility>
#include <ostream>
#include <iostream>

template <typename key_t, typename value_t>
class SplayTree {
  private:
    class Iterator;
    struct Node {
        std::pair<key_t, value_t> value_;
        Node *right_;
        Node *left_;

        Node(key_t key, value_t value);
    };

    // Data
    Node* root_;
    size_t size_;

    // Helper Functions:

    /**
     * @brief Destroys the tree
     * 
     * @param node Root node to destroy from
     */
    void destructorHelper(Node*& node);

    /**
     * @brief Helper function for insertion
     * 
     * @param key key to insert
     * @param value value to insert
     * @param node current node in teh tree
     * @param path path, used for splaying
     */
    void insertHelper(const key_t &key, const value_t &value, Node*& node, std::vector<Node *> &path);

    /**
     * @brief Searches for a specific key and returns an iterator to the node. 
     * 
     * @param key key to search for
     * @param node Current node
     * @param path tracks path used for splaying
     * @return Iterator  to the item at node key. 
     */
    const Iterator searchHelper(const key_t &key, Node*& node, std::vector<Node*>& path);

    /**
     * @brief Helper functino for printing the tree. Used for testing. 
     * 
     * @param tree Current node
     * @param out ostream to print to. 
     */
    void printTree(Node* tree, std::ostream& out) const;

    /**
     * @brief Get the path to the maximum object in the tree
     * 
     * @param node top node
     * @param path Reference to the path down the tree. 
     */
    void getMaxPath(Node *node, std::vector<Node *>& path) const;

    // Splay Functions

    /**
     * @brief Splays the bottom node in path to the root
     * 
     * @param path Path of nodes down to a given root. Avoids parent pointer
     */
    void splay(std::vector<Node *> &path);

    /**
     * @brief Rotates the tree right. Node param is the lchild of new top node. 
     * 
     * @param node The uppermost node getting rotated. 
     */
    void rotateRight(Node *&node);

    /**
     * @brief Rotates the tree left. 
     * 
     * @param node Uppermost node getting rotated left
     */
    void rotateLeft(Node *&node);

    /**
     * @brief Helper function to clean up linking grandparent nodes
     * 
     * @param path Path down to root. 
     * @param newNodeInd Index of the new node on the path
     * @param newNode The node getting splayed to the root
     * @param newKey key associated with new node
     */
    void cleanGreatGP(std::vector<Node *> &path, size_t newNodeInd,Node*& newNode, const key_t& newKey);

public:

    // Constructors
    SplayTree();
    ~SplayTree();
    SplayTree(const SplayTree &other) = default;
    SplayTree &operator=(const SplayTree &other) = default;

    // Type Names: 
    using value_type = std::pair<key_t, value_t>;
    using const_iterator = Iterator;

    // Interface Functions
    
    /**
     * @brief Insert into the splay tree
     * 
     * @param key Key to inset
     * @param value Value to insert
     */     //
    std::pair<Iterator, bool>  insert(const value_type& value); //const key_t& key, value_t& value

    /**
     * @brief Deletes the given key from splay tree.
     * 
     * @param key Key to be removed
     * @note If key is not present, the closest node to the key gets splayed 
     */
    void remove(const key_t &key);

    /**
     * @brief Checks if a key is present
     * 
     * @param key Key to check
     * @return true Key is in tree
     * @return false Key is not in tree
     */
    bool exists(const key_t &key);

    /**
     * @brief Finds the given key and returns a const iterator to it. 
     * 
     * @param key key to lookup
     * @return const const_iterator. Iterator being returned. 
     */
    const_iterator find(const key_t &key);

    /**
     * @brief Returns the size of the tree. 
     * 
     * @return size_t Number of key-pairs
     */
    size_t size() const;

    // Iterator functions

    const_iterator begin() const;
    const_iterator end() const;

    // Other functions

    /**
     * @brief Public version of printing function used for printing
     * 
     * @param out Ostream to print to. 
     */
    void print(std::ostream &out) const;

    /**
     * @brief Looks up an item. The Item MUST BE IN THE TREE
     * 
     * @param key key to look up
     * @return value_t& Value associated with the key
     * @note This function does not support insertion
     */
    value_t& operator[](const key_t &key);

  private:
    class Iterator {
        friend class SplayTree;

    private:
        std::vector<Node *> stack_;
        void pushToMin(Node *tree);

    public:

        // Required for std::iterator
        using value_type = std::pair<key_t, value_t>;
        using reference = const value_type&;
        using pointer =  value_type*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        Iterator() = default;
        Iterator(Node *node, bool push = true);
        Iterator(const Iterator &other) = default;
        Iterator &operator=(const Iterator &other) = default;
        ~Iterator() = default;

        value_type operator*() const;
        Iterator &operator++();
        pointer operator->() const;
        bool operator==(const Iterator &other) const;
        bool operator!=(const Iterator &other) const;
        
    };
};

template <typename key_t,typename value_t>
std::ostream &operator<<(std::ostream& out, const SplayTree<key_t, value_t> &splaytree);

#include "splay-tree-private.hpp"

#endif // SPLAY_TREE_HPP_INCLUDED