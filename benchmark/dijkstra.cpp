// Benchmark heaps with Dijkstra's algorithm
#include <limits>
#include <iostream>

#include "d-ary-heap/heap.hpp"
#include "graph.hpp"
#include "randomGraphs.hpp"
#include "benchmark.hpp"

template <typename T>
concept Heap = requires(T &heap,
                        T::value_type item,
                        T::priority_type priority) {
    heap.empty();
    heap.size();
    heap.pop();
    heap.push(item, priority);
    heap.top();
    heap.decreaseKey(item, priority);
};

/**
 * @brief Implementation of Dijkstra's algorithm that takes in a graph 
 * and returns a vector of the distance from vertex 0 to any other point
 * 
 * @tparam heap_t Type of heap to use
 * @param g Graph
 * @return std::vector<size_t> Vector of the distances. 
 */
template <Heap heap_t>
std::vector<size_t> dijkstra(Graph *g){

    size_t n = g->getNumVertices();
    std::vector<size_t> paths(n);
    std::ranges::fill(paths, std::numeric_limits<size_t>::max());
    paths[0] = 0;

    heap_t h;
    for (size_t vertex_i = 0; vertex_i < n; ++vertex_i){
        h.push(vertex_i, paths[vertex_i]);
    }

    while (!h.empty()){
        size_t vertex = h.top();
        h.pop();
        size_t value = paths[vertex];

        // Get repeated edges
        std::vector<Edge> adjacent = g->edgesFromStart(vertex);
        for (Edge& e : adjacent){
            if (value != std::numeric_limits<size_t>::max()) {
                size_t offer = value + e.weight;
                if (paths[e.outgoing] > offer){
                    // Update weight:
                    h.decreaseKey(e.outgoing, offer);
                    paths[e.outgoing] = offer;
                }
            }

            
        }
    }
    return paths;
}

void populate1(Graph *g){
    g->addEdge(0, 1, 10);
    g->addEdge(0, 2, 5);
    g->addEdge(1, 2, 2);
    g->addEdge(2, 1, 3);
    g->addEdge(2, 3, 9);
    g->addEdge(1, 3, 1);
    g->addEdge(3, 4, 4);
    g->addEdge(4, 3, 6);
    g->addEdge(2, 4, 2);
    g->addEdge(4, 0, 7);
}

void populate2(Graph* g){
    g->addEdge(2, 6, 3);
    g->addEdge(2, 3, 7);
    g->addEdge(2, 1, 4);
    g->addEdge(2, 5, 4);
    g->addEdge(3, 2, 7);
    g->addEdge(3, 5, 4);
    g->addEdge(3, 4, 7);
    g->addEdge(4, 3, 9);
    g->addEdge(4, 5, 6);
    g->addEdge(5, 6, 8);
    g->addEdge(5, 2, 4);
    g->addEdge(5, 3, 4);
    g->addEdge(5, 4, 6);
    g->addEdge(6, 1, 2);
    g->addEdge(6, 2, 3);
    g->addEdge(6, 5, 8);
}

int sq(int x){
    return x * x;
}

int main(){

    // A dijkstra benchmark looks like this:
    // to trials 
    // n values from n = 10, 100, 1000, 10000, 100000
    // sparsity from 0.1, 0.4, 0.7, 1
    // D values 2, 5, 10 
    // binomial heap
    // fibonacci heap
    // 5 * 4 * 5 = 100 trials. BRUH 
    // Plots: Set sparsity constant. Graph time vs n

    // Input Generation:
    // Generator g(args);
    // Generator has operator();
    // suite.addTest(generator);

    BenchmarkSuite suite("Dijkstra");
    // std::vector<int> inputs{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // suite.addVaryingInputs("test", sq, inputs);
    RandomGraphGenerator gen(0.35, 1000);
    const auto [list, mat]  = gen.makeGraphs(10);
    suite.setConfig(1000, 10);

    suite.addVaryingInputs("Dijkstra DAryHeap 3 List", dijkstra<DAryHeap<size_t, size_t, 3>>, list);
    suite.addVaryingInputs("Dijkstra DAryHeap 3 Matrix", dijkstra<DAryHeap<size_t, size_t, 3>>, mat);

    suite.run();
    // suite.plot();

    // Clean up memory
    for (auto &g : list)
    {
        delete g;
    }
    for (auto& g : mat){
        delete g;
    }

    return 0;
}
