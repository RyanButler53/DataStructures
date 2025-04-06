#ifndef BINOMIAL_HEAP_HPP_INCLUDED
#define BINOMIAL_HEAP_HPP_INCLUDED

#include <vector>
#include <list>
#include <map>
#include <cmath>
#include <functional>

template <typename T, typename P, typename Compare = std::less<P>>
class BinomialHeap
{
private:

    // Private struct to hold heap items
    struct Item{
        T item_;
        P priority_;
    };
    // Private Node struct to hold node data and pointers
    struct Node
    {
        std::vector<Node *> children_;
        Item item_;
        Node *parent_;

        Node(Item item_);
        ~Node();
    };

    // Data
    std::list<Node *> nodes_;
    std::map<T, Node *> nodeMap_;
    Node *min_;
    size_t size_;
    Compare comp_;

    // Private Methods
    void cleanup();
    Node* mergeNodes(Node*n1, Node* n2);

  public:

    using value_type = T;
    using priority_type = P;

    // Constructors and destructor
    BinomialHeap();
    ~BinomialHeap();

    bool empty() const;
    size_t size() const;
    void pop();
    T top() const;
    void push(T item, P priority);
    void changeKey(T item, P priority);
};

#include "binomial-private.hpp"

#endif //BINOMIAL_HEAP_HPP_INCLUDED