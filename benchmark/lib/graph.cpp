#include "graph.hpp"

Graph::Graph(size_t v):v_{v}{}

size_t Graph::getNumVertices() const { return v_; };

//Adjacency List Graphs
GraphAdjList::GraphAdjList(size_t v) : Graph(v)
{
    adjList_.resize(v_);
}

GraphAdjList::~GraphAdjList() {}

void GraphAdjList::addEdge(size_t start, size_t end, size_t weight){
    if (start < v_ and end < v_){
        adjList_[start].push_back({end, weight});
    }
}

std::vector<Edge> GraphAdjList::edgesFromStart(size_t start)const{
    return std::move(adjList_[start]);
}

// Adjacency Matrix Graphs
GraphAdjMatrix::GraphAdjMatrix(size_t v) : Graph(v) {
    adjMatrix_.resize(v_ * v_);
}

GraphAdjMatrix::~GraphAdjMatrix() {}

// Indexing: All starts are in the same row
void GraphAdjMatrix::addEdge(size_t start, size_t end, size_t weight){
    if (start < v_ && end < v_){
        size_t index = start * v_ + end;
        adjMatrix_[index] = weight;
    }
}

std::vector<Edge> GraphAdjMatrix::edgesFromStart(size_t start) const{
    size_t startIndex = start * v_;
    size_t endIndex = startIndex + v_;
    std::vector<Edge> edges;
    // i is the end vertex
    for (size_t i = 0; i < v_; ++i) {
        size_t weight = adjMatrix_[startIndex + i];
        if (weight) {
            edges.push_back(Edge{i, weight});
        }
    }
    return edges;
}