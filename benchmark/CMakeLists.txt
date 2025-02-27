cmake_minimum_required(VERSION 3.24.0)

# Benchmark Library: Holds a benchmark class
add_library(benchmark SHARED lib/benchmark.cpp)
set_target_properties(benchmark PROPERTIES PUBLIC_HEADER inc/benchmark.hpp)
target_include_directories(benchmark PUBLIC inc)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

add_executable(queue queueInterface.cpp)
add_executable(quack quack.cpp)
add_executable(iteration listIteration.cpp)
add_executable(tree balancedTrees.cpp)

# Link with benchmark Library
target_link_libraries(queue benchmark)
target_link_libraries(iteration benchmark)
target_link_libraries(tree benchmark)
