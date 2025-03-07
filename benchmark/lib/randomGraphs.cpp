#include "randomGraphs.hpp"

RandomGraphGenerator::RandomGraphGenerator(double sparsity, size_t n):sparsity_{sparsity}, n_{n}
{
    if (sparsity > 1 or sparsity < 0) {
        throw(std::invalid_argument("Sparsity must between 0 and 1"));
    }
    long long seed_ = time(0);
    rng_ = std::mt19937(seed_);
    for (size_t i = 0; i < n; ++i) {
        values_.push_back(i);
    }
    std::vector<int> weights(n, 1);
    std::discrete_distribution<int> dist_(weights.begin(), weights.end());
}

std::vector<std::tuple<GraphAdjList, GraphAdjMatrix>> RandomGraphGenerator::makeGraphs(size_t n){
    std::vector<std::tuple<GraphAdjList, GraphAdjMatrix>> graphs;
    for (size_t i = 0; i < n; ++i) {
        std::vector<size_t> edges = getEdges();
        std::vector<size_t> weights = getWeights();
        GraphAdjList g1(n_);
        GraphAdjMatrix g2(n_);
        populateGraph(&g1, weights, edges);
        populateGraph(&g2, weights, edges);
        graphs.push_back(std::make_tuple(g1, g2));
    }
    return graphs;
}

std::vector<size_t> RandomGraphGenerator::getEdges(){
    size_t numEdges = n_ * (n_ - 1) * sparsity_;
    std::ranges::shuffle(values_, rng_);
    std::vector<size_t> edgeIds(values_.begin(), values_.begin() + numEdges);
    return edgeIds;
}

std::vector<size_t> RandomGraphGenerator::getWeights() {
    size_t numEdges = n_ * (n_ - 1) * sparsity_;
    std::vector<size_t> weights;
    for (size_t i = 0; i < numEdges; ++i) {
        weights.push_back(dist_(rng_));
    }
    return weights;
}

void RandomGraphGenerator::populateGraph(Graph *g, std::vector<size_t> weights, std::vector<size_t> edges){
    for (size_t i = 0; i < edges.size();  ++i) {
        size_t matrixInd = edges[i] / n_ + 1 + edges[i];
        size_t start = matrixInd/n_;
        size_t end = matrixInd%n_;
        g->addEdge(start, end, weights[i]);
    }
}