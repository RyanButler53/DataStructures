// Header for graph classes
#pragma once

#include <vector>

// Edge Struct
struct Edge {
    uint32_t outgoing;
    uint32_t weight;
};

// Abstract base class for graphs
class Graph
{
private:
protected:
    const uint32_t v_;

  public:
    Graph(uint32_t v);
    virtual ~Graph() = default;


    virtual void addEdge(uint32_t start, uint32_t end, uint32_t weight) = 0;
    virtual std::vector<Edge> edgesFromStart(uint32_t start) const = 0;
    uint32_t getNumVertices() const;
};

class GraphAdjList : public Graph
{
private:
    std::vector<std::vector<Edge>> adjList_;

  public:
    GraphAdjList(uint32_t v);
    ~GraphAdjList();

    void addEdge(uint32_t start, uint32_t end, uint32_t weight) override;
    std::vector<Edge> edgesFromStart(uint32_t start) const override;
};

class GraphAdjMatrix : public Graph {
    std::vector<uint32_t> adjMatrix_;

  public:
        GraphAdjMatrix(uint32_t v);
        ~GraphAdjMatrix();

    void addEdge(uint32_t start, uint32_t end, uint32_t weight) override;
    std::vector<Edge> edgesFromStart(uint32_t start) const override;

};