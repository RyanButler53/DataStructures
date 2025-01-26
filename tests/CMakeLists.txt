cmake_minimum_required(VERSION 3.24)

add_subdirectory(googletest)

set (CMAKE_CURRENT_BINARY_DIR bin)

add_executable(cuckoo-hash cuckoo-test.cpp)
add_executable(launching-threadpool launching-threadpool-test.cpp)
add_executable(scapegoat-tree scapegoat-test.cpp)
add_executable(splay-tree splay-tree-test.cpp)
add_executable(unrolled-ll ULL-test.cpp)


target_include_directories(unrolled-ll PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(unrolled-ll gtest) #gtest_main
