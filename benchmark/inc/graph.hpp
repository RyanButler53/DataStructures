// Header for graph classes
#pragma once

#include <vector>

// Edge Struct
struct Edge {
    uint16_t outgoing;
    uint16_t weight;
};

// Abstract base class for graphs
class Graph
{
private:
protected:
    const uint16_t v_;

  public:
    Graph(uint16_t v);
    virtual ~Graph() = default;


    virtual void addEdge(uint16_t start, uint16_t end, uint16_t weight) = 0;
    virtual std::vector<Edge> edgesFromStart(uint16_t start) const = 0;
    uint16_t getNumVertices() const;
};

class GraphAdjList : public Graph
{
private:
    std::vector<std::vector<Edge>> adjList_;

  public:
    GraphAdjList(uint16_t v);
    ~GraphAdjList();

    void addEdge(uint16_t start, uint16_t end, uint16_t weight) override;
    std::vector<Edge> edgesFromStart(uint16_t start) const override;
};

class GraphAdjMatrix : public Graph {
    std::vector<uint16_t> adjMatrix_;

  public:
        GraphAdjMatrix(uint16_t v);
        ~GraphAdjMatrix();

    void addEdge(uint16_t start, uint16_t end, uint16_t weight) override;
    std::vector<Edge> edgesFromStart(uint16_t start) const override;

};