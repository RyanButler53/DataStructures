cmake_minimum_required(VERSION 3.24.0)

# Benchmark Library: Holds a benchmark calss
add_library(benchmark SHARED lib/benchmark.cpp)
set_target_properties(benchmark PROPERTIES PUBLIC_HEADER inc/benchmark.hpp)
target_include_directories(benchmark PUBLIC inc)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

add_executable(benchmark_queue queueInterface.cpp)
add_executable(benchmark_quack quack.cpp)
add_executable(benchmark_iteration listIteration.cpp)

# Link with benchmark Library
target_link_libraries(benchmark_queue benchmark)
target_link_libraries(benchmark_iteration benchmark)
