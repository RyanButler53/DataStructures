cmake_minimum_required(VERSION 3.24)
project(data_structures)

option(WITH_PYTHON_BINDINGS OFF)

# Python bindings require C++26 reflection
if (WITH_PYTHON_BINDINGS)
    set (CMAKE_CXX_STANDARD 26) # uses reflection. 
    set (CMAKE_CXX_STANDARD_REQUIRED ON)
    add_compile_options("-freflection")
else()
    set (CMAKE_CXX_STANDARD 20) # Data Structures can be written with C++20
endif()

set (CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(inc)

if (WITH_PYTHON_BINDINGS)
    message(STATUS "Building Python Bindings")
    add_subdirectory(python_bindings)
else()
    enable_testing()
    add_subdirectory(tests)
    add_subdirectory(benchmark)
endif()
