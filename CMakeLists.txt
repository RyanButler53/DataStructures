cmake_minimum_required(VERSION 3.24)
project(data_structures)

set (CMAKE_CXX_STANDARD 26) # uses reflection. 
set (CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options("-freflection")

include_directories(inc)

enable_testing()
add_subdirectory(benchmark)
add_subdirectory(tests)
