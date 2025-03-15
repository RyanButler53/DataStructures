#include "randomGraphs.hpp"

RandomGraphGenerator::RandomGraphGenerator(double sparsity, size_t n):sparsity_{sparsity}, n_{n}
{
    if (sparsity > 1 or sparsity < 0) {
        throw(std::invalid_argument("Sparsity must between 0 and 1"));
    }
    long long seed_ = time(0);
    rng_ = std::mt19937(seed_);
    for (size_t i = 0; i < n*(n-1); ++i) {
        values_.push_back(i);
    }
    std::vector<int> weights(n);
    std::ranges::fill(weights, 1);
}

std::pair<std::vector<GraphAdjList*>, std::vector<GraphAdjMatrix*>>  RandomGraphGenerator::makeGraphs(size_t n){
    std::pair<std::vector<GraphAdjList*>, std::vector<GraphAdjMatrix*>> graphs;
    auto &[adjLists, adjMatrices] = graphs;
    for (size_t i = 0; i < n; ++i)
    {
        // do these in parallel
        std::vector<size_t> edges = getRandom();
        std::vector<size_t> weights = getRandom();

        // Do these in parallel
        GraphAdjList* g1 = new GraphAdjList(n_);
        populateGraph(g1, weights, edges);
        adjLists.push_back(g1);

        GraphAdjMatrix* g2 = new GraphAdjMatrix(n_);
        populateGraph(g2, weights, edges);
        adjMatrices.push_back(g2);
    }
    return graphs;
}

std::vector<size_t> RandomGraphGenerator::getRandom(){
    size_t numEdges = n_ * (n_ - 1) * sparsity_;
    std::ranges::shuffle(values_, rng_);
    return std::vector<size_t>(values_.begin(), values_.begin() + numEdges);
}

void RandomGraphGenerator::populateGraph(Graph *g, std::vector<size_t>& weights, std::vector<size_t>& edges){
    for (size_t i = 0; i < edges.size();  ++i) {
        size_t matrixInd = edges[i] / n_ + 1 + edges[i];
        size_t start = matrixInd/n_;
        size_t end = matrixInd%n_;
        g->addEdge(start, end, weights[i]);
    }
}