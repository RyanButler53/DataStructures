#include "treelib.hpp"
#include <random>
#include <algorithm>

void TreeLib::Setup::getShuffled(std::vector<int>& v){
    v.reserve(n_);
    for (int i = 0; i < n_; ++i){
        v.push_back(i);
    }
    std::mt19937 rng(time(0));
    std::ranges::shuffle(v, rng);
}

void TreeLib::Setup::getBalanced(std::vector<int>& v){
    v.clear();
    balancedHelper(v, 0, n_);
}

void TreeLib::Setup::balancedHelper(std::vector<int>& out, size_t start, size_t end){
    if (start != end) { 
        size_t mid = start + (end - start) / 2;
        out.push_back(mid);
        balancedHelper(out, start, mid);
        balancedHelper(out, mid + 1, end);
    }
}

void TreeLib::Setup::getDuplicates(std::vector<int>& out){
    std::normal_distribution<double> dist(3, 1);
    std::vector<double> weights;
    std::mt19937 rng(time(0));
    for (int i = 0; i < n_; ++i)
    {
        weights.push_back(std::max(dist(rng), 0.0));
    }
    out.reserve(n_);
    std::discrete_distribution<int> keyDist(weights.begin(), weights.end());
    for (size_t i = 0; i < n_; ++i){
        out.push_back(keyDist(rng));
    }
}

