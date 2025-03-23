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

#include <thread>
#include <future>

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

    uint32_t n = g->getNumVertices();
    std::vector<uint32_t> paths(n);
    std::ranges::fill(paths, std::numeric_limits<uint32_t>::max());
    paths[0] = 0;

    heap_t h;
    for (uint32_t vertex_i = 0; vertex_i < n; ++vertex_i){
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
        save(filename, "svg");
    }
}

int main(){

    // RandomGraphGenerator gen(0.55, 12);
    // GraphAdjList* g = gen.makeGraph();
    // auto path = dijkstra<BinomialHeap<uint32_t, uint32_t>>(g);


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


    // A dijkstra benchmark looks like this:
    // to trials
    // n values from n = 10, 100, 1000, 10000, 100000
    // sparsity from 0.1, 0.4, 0.7, 1
    // D values 2, 5, 10
    // binomial heap
    // fibonacci heap
    // 5 * 4 * 5 = 100 trials. BRUH
    
    // Alternative idea: Build the graphs for the larger N values while the smaller n values are testing.
    // Space isn't as much as an issue here:  
    for (std::string sparsityStr : std::vector<std::string>{"0.1", "0.4", "0.7"}) {
        double sparsity = std::stod(sparsityStr);
        BenchmarkSuite suite("Dijkstra: Sparsity = " + sparsityStr);
        // Want to make graphs for all the things first, but also want to be launching them
        // std::vector<GraphAdjList*> graphs(6);
        std::vector<size_t> sizes{5000, 10000, 30000, 50000, 75000, 100000};
        std::vector<std::future<GraphAdjList*>> futures;

        auto makeGraphs = [&sparsity](size_t n){
            RandomGraphGenerator gen(sparsity, n);
            GraphAdjList *g = gen.makeGraph();
            return g;
        };

        // 6 threads start making graphs 
        for (size_t i = 0; i < 6; ++i){
            futures.push_back(std::async(std::launch::async, makeGraphs, sizes[i]));
            std::cout << "Launched n = " << sizes[i] << " case" << std::endl;
        }
        // problem: The thread finishes and it terminates. 
        for (size_t i = 0; i < 6; ++i){
            // Wait for the future to be finished
            std::cout << "Waiting for " << sizes[i] << " graph" << std::endl;
            GraphAdjList* g = futures[i].get();
            std::cout << "Got a graph for n = " << sizes[i] << std::endl;
            size_t n = sizes[i];
            suite.setConfig(n, 10);

            suite.addConfiguredTest("Dijkstra DAry Heap D = 2", dijkstra<BinaryHeap<uint32_t>>, std::ref(g));
            suite.addConfiguredTest("Dijkstra DAry Heap D = 5", dijkstra<DAryHeap<uint32_t, uint32_t>>, std::ref(g));
            suite.addConfiguredTest("Dijkstra DAry Heap D = 10", dijkstra<DAryHeap<uint32_t, uint32_t>>, std::ref(g));
            suite.addConfiguredTest("Dijkstra Binomial Heap", dijkstra<BinomialHeap<uint32_t, uint32_t>>, std::ref(g));
            suite.addConfiguredTest("Dijkstra Fibonacci Heap", dijkstra<FibonacciHeap<uint32_t, uint32_t>>, std::ref(g));
            std::cout << "Launching n = " << n << " test" << std::endl;
            suite.run();
            suite.resultsToCSV("dijkstra1" + sparsityStr + ".csv");
            
            delete g;
        }
        // plot(suite, "dijkstra" + sparsityStr + ".svg");
    }

    return 0;
}
