cmake_minimum_required(VERSION 3.24)
project(data_structures)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(inc)

add_subdirectory(benchmark)
add_subdirectory(tests)
