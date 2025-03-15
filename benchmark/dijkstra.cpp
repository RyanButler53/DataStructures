// Benchmark heaps with Dijkstra's algorithm
#include <limits>
#include <iostream>

#include "d-ary-heap/heap.hpp"
#include "binomial-heap/heap.hpp"
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
std::vector<uint32_t> dijkstra(Graph *g){

    uint16_t n = g->getNumVertices();
    std::vector<uint32_t> paths(n);
    std::ranges::fill(paths, std::numeric_limits<uint32_t>::max());
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

    BenchmarkSuite suite("Dijkstra");

    // Max size for a graph is n = 65536 for memory reasons. 
    for (size_t n : std::vector<size_t>{50,100,2000, 500, 1000}) {
        for (double sparsity : std::vector<double>{0.4}){ // 0.1, 0.4, 0.7, 1

            RandomGraphGenerator gen(sparsity, n);
            std::cout << "Started Generating inputs for n = " << n << std::endl;
            auto [list, mat] = gen.makeGraphs(10);
            std::cout << "Finished Generating inputs for n = " << n << std::endl;
            suite.setConfig(n, 10);
            suite.addVaryingInputs("Dijkstra Binary Heap List", dijkstra<BinaryHeap<size_t>>, list);
            suite.addVaryingInputs("Dijkstra Binary Heap Matrix", dijkstra<BinaryHeap<size_t>>, mat);
            suite.addVaryingInputs("Dijkstra DAry Heap 5 List", dijkstra<DAryHeap<size_t, size_t, 5>>, list);
            suite.addVaryingInputs("Dijkstra DAry Heap 5 Matrix", dijkstra<DAryHeap<size_t, size_t, 5>>, mat);
            suite.addVaryingInputs("Dijkstra DAry Heap 10 List", dijkstra<DAryHeap<size_t, size_t, 10>>, list);
            suite.addVaryingInputs("Dijkstra DAry Heap 10 Matrix", dijkstra<DAryHeap<size_t, size_t, 10>>, mat);
            suite.addVaryingInputs("Dijkstra Binomial Heap List", dijkstra<BinomialHeap<size_t, size_t>>, list);
            suite.addVaryingInputs("Dijkstra Binomial Heap Matrix", dijkstra<BinomialHeap<size_t, size_t>>, mat);
            // suite.addVaryingInputs("Dijkstra Fibonacci Heap List", dijkstra<FibonacciHeap<size_t, size_t>>, list);
            // suite.addVaryingInputs("Dijkstra Fibonacci Heap Matrix", dijkstra<FibonacciHeap<size_t, size_t>>, mat);
            std::cout << "Running n = " << n << " sparsity = " << sparsity << std::endl;
            suite.run();

            // Clean up memory NOW
            for (auto &g : list) {
                delete g;
            }
            for (auto& g : mat){
                delete g;
            }

        }
    }
    suite.resultsToCSV("DijkstraResults");
    // suite.plot();

    // Plotting. 


    return 0;
}
