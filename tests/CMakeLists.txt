cmake_minimum_required(VERSION 3.24)

add_subdirectory(googletest)

set (CMAKE_CURRENT_BINARY_DIR bin)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("DEBUG")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fsanitize=undefined -fsanitize=address")
    set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=undefined  -fsanitize=address")
endif()


add_executable(cuckoo-hash cuckoo-test.cpp)
add_executable(launching-threadpool launching-threadpool-test.cpp)
add_executable(scapegoat-tree scapegoat-test.cpp)
add_executable(splay-tree splay-tree-test.cpp)
add_executable(unrolled-ll ULL-test.cpp)

# Link to google test if necessary
target_link_libraries(unrolled-ll gtest) #gtest_main
target_link_libraries(splay-tree gtest)

