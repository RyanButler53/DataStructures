cmake_minimum_required(VERSION 3.24.0)



set (BENCHMARK_LIB_SRC
    lib/benchmark.cpp)

set (GRAPH_LIB_SRC
    lib/graph.cpp
    lib/randomGraphs.cpp
    )

# Benchmark Library: Holds a benchmark class
add_library(benchmark STATIC ${BENCHMARK_LIB_SRC})
target_include_directories(benchmark PUBLIC inc )

# Graph library: Holds Graph classes and random graph generator
add_library(graph STATIC ${GRAPH_LIB_SRC})
target_include_directories(graph PUBLIC inc)

# All benchmark suites need this include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

add_subdirectory(matplotplusplus)

# Add executables
add_executable(queue queueInterface.cpp)
add_executable(quack quack.cpp)
add_executable(iteration listIteration.cpp)
add_executable(tree balancedTrees.cpp)
add_executable(dijkstra dijkstra.cpp)
add_executable(heaps heaps.cpp)
add_executable(medians medians.cpp)

# Link with appropriate libraries
target_link_libraries(queue benchmark)
target_link_libraries(quack benchmark)
target_link_libraries(iteration benchmark)
target_link_libraries(tree benchmark)
target_link_libraries(dijkstra graph benchmark matplot)
target_link_libraries(heaps benchmark)
target_link_libraries(medians benchmark)
