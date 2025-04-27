#include <vector>
#include <thread>

#include "interfaces.hpp"
// Small setup class and namespace for manipulating inputs for Tree Map data types
namespace TreeLib
{

class Setup {
    size_t n_;

    public: 
    Setup(size_t n) : n_ { n }{}

    void getShuffled(std::vector<int>& v);

    void getBalanced(std::vector<int>& v);

    void balancedHelper(std::vector<int>& out, size_t start, size_t end);

    void getDuplicates(std::vector<int>& out);
};

// Benchmarking Functions
template <Tree T>
void insert(size_t numOps){
    T tree;
    for (int i = 0; i < numOps; ++i)
    {
        tree.insert({i, i});
    }
}

template <Tree T>
void insertElements(std::vector<int>& elements){
    T tree;
    for (int x : elements){
        tree.insert({x, x});
    }
}

// Not used for benchmarking directly
template <Tree T>
void insertItems(T& tree, std::vector<int>& elements){
    for (int x : elements){
        tree.insert({x, x});
    }
}

template <Tree T>
void insertAndClear(std::vector<int>& elements){
    T tree;
    insertItems(tree, elements);
    tree.clear();
}

template <Tree T>
void insertAndDelete(std::vector<int>& toInsert,std::vector<int>& toDelete){
    T tree;
    insertItems(tree, toInsert);
    for (int x : toDelete){
        tree.erase(x);
    }
}

template <Tree T>
void findDuplicates(std::vector<int>& toInsert, std::vector<int> toFind){
    T tree;
    insertItems(tree, toInsert);
    for (int x : toFind){
        tree.find(x);
    }
}

template <Tree T>
std::vector<int> treeSort(std::vector<int>& elements){
    T tree;
    for (int x : elements){
        tree.insert({x, x});
    }
    std::vector<int> sorted;
    sorted.reserve(elements.size());
    for (int x : elements){
        sorted.push_back(x);
    }
    return sorted;
}

} // namespace TreeLib
