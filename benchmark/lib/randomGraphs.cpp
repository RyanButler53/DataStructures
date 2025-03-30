#include "randomGraphs.hpp"
#include <thread>
#include <iostream>

RandomGraphGenerator::RandomGraphGenerator(double sparsity, size_t n):sparsity_{sparsity}, n_{n}{
    primes_ = std::vector<size_t>{631, 919, 1657, 1801, 1951, 2269, 2437, 2791, 3169, 3571, 4219, 4447, 5167, 5419, 6211, 7057, 7351, 8269, 9241, 10267, 11719, 12097, 13267, 13669, 16651, 19441, 19927, 22447, 23497, 24571, 25117, 26227, 27361, 33391, 35317};
    if (sparsity > 1 or sparsity < 0) {
        throw(std::invalid_argument("Sparsity must between 0 and 1"));
    }
}

std::pair<std::vector<GraphAdjList*>, std::vector<GraphAdjMatrix*>>  RandomGraphGenerator::makeGraphs(size_t n){
    std::pair<std::vector<GraphAdjList*>, std::vector<GraphAdjMatrix*>> graphs;
    auto &[adjLists, adjMatrices] = graphs;
    adjLists.resize(n);
    adjMatrices.resize(n);

    std::vector<std::thread> threads;

    auto f = [&adjLists, &adjMatrices, this](size_t i, size_t prime)
    {
        std::vector<size_t> edges;
        std::vector<size_t> weights;
        getRandom(edges, prime);
        getRandom(weights, prime);

        // Do these in parallel
        GraphAdjList *g1 = new GraphAdjList(n_);
        populateGraph(g1, weights, edges);
        adjLists[i] = g1;
        GraphAdjMatrix *g2 = new GraphAdjMatrix(n_);
        populateGraph(g2, weights, edges);
        adjMatrices[i] = g2;
    };
    for (size_t i = 0; i < n; ++i){
        size_t prime = primes_[rand() % primes_.size()];
        threads.emplace_back(f, i, prime);
    }

    for (std::thread& t : threads){
        t.join();
    }
    
    return graphs;
}

void RandomGraphGenerator::getRandom(std::vector<size_t>& values, size_t prime){
    size_t numEdges = n_ * (n_ - 1) * sparsity_;
    size_t maxEdges = n_ * (n_ - 1);
    values.reserve(numEdges);
    size_t ind = 0;
    for (size_t i = 0; i < numEdges; ++i){
        values.push_back(ind % maxEdges);
        ind += prime;
    }
}

void RandomGraphGenerator::populateGraph(Graph *g, std::vector<size_t>& weights, std::vector<size_t>& edges){
    while (!edges.empty()) {
        size_t matrixInd = edges.back() / n_ + 1 + edges.back();
        size_t start = matrixInd/n_;
        size_t end = matrixInd%n_;
        g->addEdge(start, end, weights.back());
        edges.pop_back();
        weights.pop_back();
    }
}

GraphAdjList* RandomGraphGenerator::makeGraph() {
    std::vector<size_t> edges;
    std::vector<size_t> weights;

    size_t p1 = primes_[rand() % primes_.size()];
    size_t p2 = primes_[rand() % primes_.size()];

    std::thread helper([this, &weights, p2]()
                       { getRandom(weights, p2); });
    getRandom(edges, p1);
    helper.join();
    GraphAdjList *g = new GraphAdjList(n_);
    populateGraph(g, weights, edges);
    return g;
}