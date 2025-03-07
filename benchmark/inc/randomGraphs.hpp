// Generates Random Graphs: 
#pragma once
// Parameters
#include "graph.hpp"
#include <random>

//Measures of graph density: 
// Edge Density

class RandomGraphGenerator
{
private:    
    std::mt19937 rng_;
    std::discrete_distribution<int> dist_;
    std::vector<size_t> values_;
    long long seed_;
    double sparsity_; // Edge Density
    size_t n_;

    // Get edges and weights
    std::vector<size_t> getEdges();
    std::vector<size_t> getWeights();

    // Populate graph, regardless of its type. 
    void populateGraph(Graph *g, std::vector<size_t> weights, std::vector<size_t> edges);

public:
    RandomGraphGenerator(double sparsity, size_t n);
    ~RandomGraphGenerator()= default;

    std::vector<std::tuple<GraphAdjList, GraphAdjMatrix>> makeGraphs(size_t n);
};
