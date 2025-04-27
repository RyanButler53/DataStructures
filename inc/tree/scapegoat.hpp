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
    class Iterator;

    struct Node {

        std::pair<key_t, value_t> value_;
        Node* left_;
        Node* right_;

        Node(key_t key, value_t value);
    };

    // Data:
    size_t size_;
    size_t maxSize_;
    Node *root_;
    float alpha_;

  public:
    // Constructors
    ScapegoatTree();
    ScapegoatTree(float alpha);
    ~ScapegoatTree();
    ScapegoatTree(ScapegoatTree &other) = default;
    ScapegoatTree &operator=(const ScapegoatTree &other) = default;
    

    // Type Names: 
    using value_type = std::pair<key_t, value_t>;
    using key_type = key_t;
    using mapped_type = value_t;
    using const_iterator = Iterator;
    
    /**
     * @brief Inserts a key, value pair into the tree
     * 
     * @param key Key to insert
     * @param value Corresponding value to insert
     * @warning Inserting a value already in the tree causes undefined behavior
     */
    std::pair<Iterator, bool> insert(const value_type& value);

    /**
     * @brief Inserts keys from a range [first, last)
     */
    template<class InputIt>
    void insert(InputIt first, InputIt last);

    /**
     * @brief deletes a key-value pair from the tree
     * 
     * @param key key to delete
     * @return Returns 1 if the erase is successful and 0 otherwise
     */
    size_t erase(const key_t& key);

    /**
     * @brief Checks if something is in the tree
     * 
     * @param key Key to check
     * @return true Key is in tree
     * @return false Key is not in tree
     */
    bool contains(const key_t& key) const;

    /**
     * @brief Looks up a key. Returns the value
     * 
     * @param key Key to lookup
     * @return int Corresponding value
     */
    const_iterator find(const key_t& key);

    size_t size() const {return size_;}

    // Iterator functions
    
    const_iterator begin() const;
    const_iterator end() const;

    /**
     * @brief clears the tree
     */
    void clear();

    /**
     * @brief Prints the tree
     * 
     */
    void print(std::ostream& out) const;

    /**
     * @brief Looks up or inserts the value corresponding to the given key
     * 
     * @param key Key to lookup
     * @return int corresponding value
     */
    value_t& operator[](const key_t& key);

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
     * @return iterator at the node of the tree
     */
    const Iterator searchHelper(const key_t& key, Node *tree) const;

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
    
    class Iterator {
      friend class ScapegoatTree;

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

template<typename key_t, typename value_t>
std::ostream &operator<<(std::ostream& out, const ScapegoatTree<key_t, value_t>& sg);

#include "scapegoat-private.hpp"

#endif // SCAPEGOAT_HPP_INCLUDED
