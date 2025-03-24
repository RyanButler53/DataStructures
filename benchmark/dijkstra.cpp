// Benchmark heaps with Dijkstra's algorithm
#include <limits>
#include <iostream>
#include <chrono>

#include "d-ary-heap/heap.hpp"
#include "binomial-heap/heap.hpp"
#include "fibonacci-heap/heap.hpp"
#include "graph.hpp"
#include "randomGraphs.hpp"
#include "benchmark.hpp"
#include <matplot/matplot.h>

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
std::vector<uint16_t> dijkstra(Graph *g){

    uint16_t n = g->getNumVertices();
    std::vector<uint16_t> paths(n);
    std::ranges::fill(paths, std::numeric_limits<uint16_t>::max());
    paths[0] = 0;

    heap_t h;
    for (uint16_t vertex_i = 0; vertex_i < n; ++vertex_i){
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

void plot(BenchmarkSuite& suite, std::string filename){
    // Plotting results: Need to group by the test name. 
    //Test name has xs, avgs, stdevs
    using namespace matplot;


    // title(suite.suiteName_);
    xlabel("Input Size");
    ylabel("Time (s)");
    auto groupedResults = suite.getGroupedResults();
    
    for (auto &[name, g] : groupedResults)
    {
        hold(on);
        error_bar_handle h = errorbar(g.inputSizes_, g.times_, g.stdevs_, "-");
        h->display_name(name);
        h->line_width(1.5);
        hold(off);
    }
    legend_handle l = ::matplot::legend({});
    l->location(legend::general_alignment::topleft);

    if (filename == ""){
        show();
    } else {
        save(filename, "jpeg");
    }
}

int main(){

    BenchmarkSuite suite("Dijkstra");
    
    // Exploratory test to show the difference between adjacency list and adjacency matrix
    for (size_t n : std::vector<size_t>{50,100,200, 500, 1000}) {
        for (double sparsity : std::vector<double>{0.4}){ // 0.1, 0.4, 0.7, 1
            RandomGraphGenerator gen(sparsity, n);
            auto [list, mat] = gen.makeGraphs(10);
            suite.setConfig(n, 10);
            suite.addVaryingInputs("Dijkstra Binary Heap List", dijkstra<BinaryHeap<size_t>>, list);
            suite.addVaryingInputs("Dijkstra Binary Heap Matrix", dijkstra<BinaryHeap<size_t>>, mat);
            suite.addVaryingInputs("Dijkstra DAry Heap 5 List", dijkstra<DAryHeap<size_t, size_t, 5>>, list);
            suite.addVaryingInputs("Dijkstra DAry Heap 5 Matrix", dijkstra<DAryHeap<size_t, size_t, 5>>, mat);
            suite.addVaryingInputs("Dijkstra DAry Heap 10 List", dijkstra<DAryHeap<size_t, size_t, 10>>, list);
            suite.addVaryingInputs("Dijkstra DAry Heap 10 Matrix", dijkstra<DAryHeap<size_t, size_t, 10>>, mat);
            suite.addVaryingInputs("Dijkstra Binomial Heap List", dijkstra<BinomialHeap<size_t, size_t>>, list);
            suite.addVaryingInputs("Dijkstra Binomial Heap Matrix", dijkstra<BinomialHeap<size_t, size_t>>, mat);
            suite.addVaryingInputs("Dijkstra Fibonacci Heap List", dijkstra<FibonacciHeap<size_t, size_t>>, list);
            suite.addVaryingInputs("Dijkstra Fibonacci Heap Matrix", dijkstra<FibonacciHeap<size_t, size_t>>, mat);
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
    suite.resultsToCSV("DijkstraResults.csv");

    // Since Adjacency lists are faster, Try larger tests with adjacency list graphs
    for (std::string sparsityStr : std::vector<std::string>{"0.1", "0.4", "0.7"}) {
        double sparsity = std::stod(sparsityStr);
        BenchmarkSuite suite("Dijkstra: Sparsity = " + sparsityStr);
        for (size_t n : std::vector<size_t>{5000, 10000, 30000, 50000, 65000}){
            std::cout << "Starting Sparsity " <<sparsityStr << " with n = " << n << std::endl;
            suite.setConfig(n, 10);
            RandomGraphGenerator gen(sparsity, n);
            Graph *g = gen.makeGraph();
            suite.addConfiguredTest("Dijkstra DAry Heap D = 2", dijkstra<BinaryHeap<uint16_t>>, std::ref(g));
            suite.addConfiguredTest("Dijkstra DAry Heap D = 5", dijkstra<DAryHeap<uint16_t, uint16_t>>, std::ref(g));
            suite.addConfiguredTest("Dijkstra DAry Heap D = 10", dijkstra<DAryHeap<uint16_t, uint16_t>>, std::ref(g));
            suite.addConfiguredTest("Dijkstra Binomial Heap", dijkstra<BinomialHeap<uint16_t, uint16_t>>, std::ref(g));
            suite.addConfiguredTest("Dijkstra Fibonacci Heap", dijkstra<FibonacciHeap<uint16_t, uint16_t>>, std::ref(g));
            suite.run();
            suite.resultsToCSV("dijkstra" + sparsityStr + ".csv");
            delete g;
        }
        suite.resultsToCSV("dijkstra" + sparsityStr + ".csv");
    }

    return 0;
}
