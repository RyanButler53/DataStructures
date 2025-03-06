#include <vector>
#include "splay-tree/splay-tree.hpp"
#include <map>
#include <chrono>
#include <random>
#include <string>
#include <iostream>
#include <concepts>
#include "benchmark.hpp"

template <typename TreeType>
concept Tree = requires(TreeType &tree,
                        TreeType::value_type &pair,
                        TreeType::const_iterator iter,
                        const TreeType::key_type &key,
                        TreeType::mapped_type value) {
    // tree();
    tree.insert(pair);
    iter = tree.find(key);
    iter = tree.begin();
    iter = tree.end();
    tree.operator[](key);
    tree.find(key);
    tree.clear();
    tree.erase(key);
};

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
class Benchmark_Tree : Benchmark
{
private:

    // Insert to make a balanced tree
    std::vector<int> balanced_;
    // shuffled to make a random tree
    std::vector<int> shuffled_;
    // keys but not necessarily all there and not once. 
    std::vector<int> findDup_;

  public:
    Benchmark_Tree(std::string name, std::vector<int>& balanced, std::vector<int>& random, std::vector<int>& findDup) : 
        Benchmark(balanced.size(), name),shuffled_{random}, balanced_{balanced}, findDup_{findDup} {}

    // Delete a bunch of random keys in the container

    // Insert a bunch of items in sorted order into RB tree and Splay Tree (Later SG tree)
    void insert(T& tree) const {
        for (int i = 0; i < numOps_; ++i){
            tree.insert({i, i});
        }
    }

    // Insert a bunch of items in balanced order
    void insertBalanced(T& tree) const {
        for (int x : balanced_) {
            tree.insert({x, x});
        }
    }
    
    // Insert a bunch of items in random order
    void insertShuffled(T& tree) const {
        for (int x : shuffled_){
            tree.insert({x, x});
        }
    }

    // Find a bunch of random keys in the container. 
    // Find needs an already populated tree. 
    void find(T& tree) const {
        for (int x : shuffled_){
            tree.find(x);
        }
        
    }

    // Find with duplicates needs an already populated tree. 
    void findDup(T &tree) const {
        for (int x : findDup_){
            tree.find(x);
        }
    }

    void deleteKeys(T& tree) const {
        for (int key : shuffled_){
            tree.erase(key);
        }
    }


    std::string operator()() const {
        T tree;
        std::stringstream ss("Results for ");
        ss << name_ << ": ";
        ss << Benchmark::measure([&tree, this]()
                                 { insert(tree); });

        ss << " ";
        ss << Benchmark::measure([&tree]()
                                 { tree.clear(); });

        ss << " ";
        ss << Benchmark::measure([&tree, this]()
                                 { insertBalanced(tree); });
        ss << " ";
        ss << Benchmark::measure([&tree, this]()
                                 { find(tree); });
        ss << " ";

        tree.clear();
        ss << Benchmark::measure([&tree, this]()
                                 { insertShuffled(tree); });
        ss << " ";

        ss << Benchmark::measure([&tree, this]()
                                 { findDup(tree); });
        ss << " ";

        ss << Benchmark::measure([&tree, this]()
                                 { deleteKeys(tree); });

        return ss.str();
    }
};

int main(int argc, char** argv) {

    size_t numOps = 10000;
    if (argc == 2){
        numOps = atoi(argv[1]);
    }
    std::vector<int> balanced, shuffled, duplicates;
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

    Benchmark_Tree<std::map<int, int>> map("rb tree", balanced, shuffled, duplicates);
    Benchmark_Tree<SplayTree<int, int>> b("splay tree", balanced, shuffled, duplicates);
    std::cout << map() << std::endl;
    std::cout << b() << std::endl;

}