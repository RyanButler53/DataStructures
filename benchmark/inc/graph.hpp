// Header for graph classes
#pragma once

#include <vector>

// Edge Struct
struct Edge {
    size_t outgoing;
    size_t weight;
};

// Abstract base class for graphs
class Graph {
  private:

  protected:
    const size_t v_;

  public:
      Graph(size_t v);
      virtual ~Graph() = default;

      virtual void addEdge(size_t start, size_t end, size_t weight) = 0;
      virtual std::vector<Edge> edgesFromStart(size_t start) const = 0;
      size_t getNumVertices() const;
};


class GraphAdjList : public Graph
{
private:
    std::vector<std::vector<Edge>> adjList_;

public:
    GraphAdjList(size_t v);
    ~GraphAdjList();

    void addEdge(size_t start, size_t end, size_t weight) override;
    std::vector<Edge> edgesFromStart(size_t start) const override;
};

class GraphAdjMatrix : public Graph {
    std::vector<size_t> adjMatrix_;

    public:
        GraphAdjMatrix(size_t v);
        ~GraphAdjMatrix();

    void addEdge(size_t start, size_t end, size_t weight) override;
    std::vector<Edge> edgesFromStart(size_t start) const override;

};
