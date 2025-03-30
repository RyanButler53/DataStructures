#include "graph.hpp"
#include <iostream>

Graph::Graph(uint16_t v):v_{v}{}

uint16_t Graph::getNumVertices() const { return v_; };

//Adjacency List Graphs
GraphAdjList::GraphAdjList(uint16_t v) : Graph(v)
{
    adjList_.resize(v_);
}

GraphAdjList::~GraphAdjList() {}

void GraphAdjList::addEdge(uint16_t start, uint16_t end, uint16_t weight){
    if (start < v_ and end < v_){
        adjList_[start].push_back({end, weight});
    }
}



std::vector<Edge> GraphAdjList::edgesFromStart(uint16_t start)const{
    return std::move(adjList_[start]);
}

// Adjacency Matrix Graphs
GraphAdjMatrix::GraphAdjMatrix(uint16_t v) : Graph(v) {
    if (v > 20000){
        throw std::invalid_argument("Maximum size of graph is 20000 vertices due to memory usage");
    }
    adjMatrix_.resize(v_ * v_);
}

GraphAdjMatrix::~GraphAdjMatrix() {}

// Indexing: All starts are in the same row
void GraphAdjMatrix::addEdge(uint16_t start, uint16_t end, uint16_t weight){
    if (start < v_ && end < v_){
        size_t index = start * v_ + end;
        adjMatrix_[index] = weight;
    }
}

std::vector<Edge> GraphAdjMatrix::edgesFromStart(uint16_t start) const{
    size_t startIndex = start * v_;
    size_t endIndex = startIndex + v_;
    std::vector<Edge> edges;
    // i is the end vertex
    for (uint16_t i = 0; i < v_; ++i) {
        uint16_t weight = adjMatrix_[startIndex + i];
        if (weight) {
            edges.push_back(Edge{i, weight});
        }
    }
    return edges;
}