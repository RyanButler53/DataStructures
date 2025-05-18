cmake_minimum_required(VERSION 3.24.0)


set (BENCHMARK_LIB_SRC
    lib/benchmark.cpp)

set (GRAPH_LIB_SRC
    lib/graph.cpp
    lib/randomGraphs.cpp
    )

set (TREE_LIB_SRC
    lib/treelib.cpp)

# Benchmark Library: Holds a benchmark class
add_library(benchmark STATIC ${BENCHMARK_LIB_SRC})
target_include_directories(benchmark PUBLIC inc )

# Graph library: Holds Graph classes and random graph generator
add_library(graph STATIC ${GRAPH_LIB_SRC})
target_include_directories(graph PUBLIC inc)

# Tree Library: Holds methods useful for benchmarking tree
add_library(treeutils STATIC ${TREE_LIB_SRC})
target_include_directories(treeutils PUBLIC inc)

# All benchmark suites need this include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

add_subdirectory(matplotplusplus)

function(addBenchmark benchmarkname src)
    add_executable(${benchmarkname} ${src})
    target_link_libraries(${benchmarkname} benchmark ${ARGN})
endfunction()

addBenchmark(queue queueInterface.cpp)
addBenchmark(quack quack.cpp)
addBenchmark(iteration listIteration.cpp)
addBenchmark(trees balancedTrees.cpp treeutils)
addBenchmark(scapegoatTrees scapegoat-trees.cpp treeutils)
addBenchmark(dijkstra dijkstra.cpp graph matplot)
addBenchmark(heaps heaps.cpp)
addBenchmark(medians medians.cpp)
addBenchmark(kdtree kd-tree.cpp)

