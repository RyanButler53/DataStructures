cmake_minimum_required(VERSION 3.24)

add_subdirectory(googletest)

set (CMAKE_CURRENT_BINARY_DIR bin)

if (CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)

    add_compile_options(-fsanitize=undefined)
    add_link_options(-fsanitize=undefined)
    message("-- Debug Mode: Enabling address and undefined behavior sanitizers")
endif()


add_executable(cuckoo-hash cuckoo-test.cpp)
add_executable(launching-threadpool launching-threadpool-test.cpp)
add_executable(scapegoat-tree scapegoat-test.cpp)
add_executable(splay-tree splay-tree-test.cpp)
add_executable(unrolled-ll ULL-test.cpp)
add_executable(heap heap-test.cpp)
add_executable(median median-test.cpp)

# Link to google test if necessary
target_link_libraries(unrolled-ll gtest) 
target_link_libraries(splay-tree gtest)
target_link_libraries(heap gtest)
target_link_libraries(median gtest)
target_link_libraries(scapegoat-tree gtest)

