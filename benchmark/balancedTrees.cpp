#include <vector>
#include "tree/splay-tree.hpp"
#include <map>
#include <chrono>
#include <random>
#include <string>
#include <iostream>
#include "benchmark.hpp"
#include "interfaces.hpp"


class Setup {
    
    size_t n_;
  
  public:
    Setup(size_t n) : n_ { n }{}

    void getShuffled(std::vector<int>& v){
        v.reserve(n_);
        for (int i = 0; i < n_; ++i){
            v.push_back(i);
        }
        std::mt19937 rng(time(0));
        std::ranges::shuffle(v, rng);
    }

    void getBalanced(std::vector<int>& v){
        v.clear();
        balancedHelper(v, 0, n_);
    }

    void balancedHelper(std::vector<int>& out, size_t start, size_t end){
        if (start != end) { 
            size_t mid = start + (end - start) / 2;
            out.push_back(mid);
            balancedHelper(out, start, mid);
            balancedHelper(out, mid + 1, end);
        }
    }

    void getDuplicates(std::vector<int>& out){
        std::normal_distribution<double> dist(3, 1);
        std::vector<double> weights;
        mt19937 rng(time(0));
        for (int i = 0; i < n_; ++i)
        {
            weights.push_back(max(dist(rng), 0.0));
        }
        out.reserve(n_);
        std::discrete_distribution<int> keyDist(weights.begin(), weights.end());
        for (size_t i = 0; i < n_; ++i){
            out.push_back(keyDist(rng));
        }
    }

};

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

// Not used for benchmarking
template <typename T>
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

template <typename T>
int add(T x, T y){
    return x + y;
}

int main(int argc, char** argv) {

    size_t numOps = 10000;
    if (argc == 2){
        numOps = atoi(argv[1]);
    }
    std::vector<int> balanced, shuffled, duplicates;
    
    // Setup phase: 
    Setup s(numOps);
    std::thread balancedThread([&s, &balanced]()
                               { s.getBalanced(balanced); });
    std::thread shuffledThread([&s, &shuffled]()
                               { s.getShuffled(shuffled); });
    std::thread duplicatesThread([&s, &duplicates]()
                               { s.getDuplicates(duplicates); });

    balancedThread.join();
    shuffledThread.join();
    duplicatesThread.join();

    // Make a benchmark suite:
    BenchmarkSuite suite("Balanced Trees Suite");
    suite.setConfig(numOps, 10);

    suite.addConfiguredTest("In order insertion RB Tree", insert<std::map<int, int>>, std::ref(numOps));
    suite.addConfiguredTest("Balanced insertion RB Tree", insertElements<std::map<int, int>>, std::ref(balanced));
    suite.addConfiguredTest("Random insertion RB Tree", insertElements<std::map<int, int>>, std::ref(shuffled));
    suite.addConfiguredTest("Clear RB Tree", insertAndClear<std::map<int, int>>, std::ref(shuffled));
    suite.addConfiguredTest("Delete from RB Tree", insertAndDelete<std::map<int, int>>, std::ref(shuffled), std::ref(shuffled));
    suite.addConfiguredTest("Find Duplicates RB Tree", findDuplicates<std::map<int, int>>, std::ref(shuffled), std::ref(duplicates));
    suite.addConfiguredTest("RB Tree Sort", treeSort<std::map<int, int>>, std::ref(shuffled));


    suite.addConfiguredTest("In order insertion Splay Tree", insert<SplayTree<int, int>>, std::ref(numOps));
    suite.addConfiguredTest("Balanced insertion Splay Tree", insertElements<SplayTree<int, int>>, std::ref(balanced));
    suite.addConfiguredTest("Random insertion Splay Tree", insertElements<SplayTree<int, int>>, std::ref(shuffled));
    suite.addConfiguredTest("Clear Splay Tree", insertAndClear<SplayTree<int, int>>, std::ref(shuffled));
    suite.addConfiguredTest("Delete from Splay Tree", insertAndDelete<SplayTree<int, int>>, std::ref(shuffled), std::ref(shuffled));
    suite.addConfiguredTest("Find Duplicates Splay Tree", findDuplicates<SplayTree<int, int>>, std::ref(shuffled), std::ref(duplicates));
    suite.addConfiguredTest("Splay Tree Sort", treeSort<SplayTree<int, int>>, std::ref(shuffled));
    suite.run();
    suite.resultsToCSV("balancedTrees.csv");
}