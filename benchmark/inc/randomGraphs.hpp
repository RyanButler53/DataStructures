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
    std::vector<size_t> primes_;
    double sparsity_; // Edge Density
    size_t n_;

    // Get edges and weights
    void getRandom(std::vector<size_t>& values, size_t prime);

    // Populate graph, regardless of its type. 
    void populateGraph(Graph *g, std::vector<size_t>& weights, std::vector<size_t>& edges);

public:
    RandomGraphGenerator(double sparsity, size_t n);
    ~RandomGraphGenerator()= default;

    std::pair<std::vector<GraphAdjList*>, std::vector<GraphAdjMatrix*>> makeGraphs(size_t n);
};
