cmake_minimum_required(VERSION 3.24)
project(data_structures)

set (CMAKE_CXX_STANDARD 20) #uses concepts and ranges. 
set (CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(inc)

enable_testing()
add_subdirectory(benchmark)
add_subdirectory(tests)
